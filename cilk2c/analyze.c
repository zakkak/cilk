/*
 *
 *  C-to-C Translator
 *
 *  Rob Miller
 */

/*
 * Copyright (c) 1994-2002 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "ast.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/analyze.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");


#define Gen(n)  ((n)->analysis.gen)
#define Kill(n) ((n)->analysis.kill)
#define LiveVars(n)  ((n)->analysis.livevars)
#define DirtyVars(n) ((n)->analysis.dirtyvars)


/*************************************************************************/
/*                                                                       */
/*                        Bit vector frameworks                          */
/*                                                                       */
/*************************************************************************/

PRIVATE FlowValue EmptySet = {FALSE, {(BitVector)0}};
/*PRIVATE*/ FlowValue TotalSet = {FALSE, {~(BitVector)0}};

#define Bit(bitnum)    (((BitVector)1)<<(bitnum))
#define SetBit(v, bitnum)    ((v).u.bitvector |= Bit(bitnum))
#define ClearBit(v, bitnum)  ((v).u.bitvector &= ~Bit(bitnum))
#define IsBitSet(v, bitnum)  (((v).u.bitvector & Bit(bitnum)) != 0)


/*
 * SetUnion: meet operation for superset lattice
 */
PRIVATE inline FlowValue SetUnion(FlowValue v1, FlowValue v2)
{
     if (v1.undefined)
	  return v2;
     else if (v2.undefined)
	  return v1;
     else {
	  FlowValue v;

	  v.undefined = FALSE;
	  v.u.bitvector = v1.u.bitvector | v2.u.bitvector;
	  return v;
     }
}


/*
 * SetEqual: equality operation for bitvector frameworks
 */
PRIVATE inline Bool SetEqual(FlowValue v1, FlowValue v2)
{
     if (v1.undefined)
	  return v2.undefined;
     else
	  return !v2.undefined && v1.u.bitvector == v2.u.bitvector;
}

/*
 * GenKill: used in transfer function of a bitvector framework
 */
PRIVATE inline FlowValue GenKill(Node *node, FlowValue v)
{
     if (v.undefined) 
	  v = EmptySet; /* identity element under union */

     v.u.bitvector =
	  (v.u.bitvector & ~Kill(node).u.bitvector) | Gen(node).u.bitvector;
     return v;
}

/*
 * ListFind: returns bitvector with bit i set to 1 iff ith member of
 * list is node.  Useful for setting up Gen and Kill sets.
 */
PRIVATE FlowValue ListFind(List *list, Node *node)
{
     int di = 0;
     FlowValue v;

     v.undefined = FALSE;
     v.u.bitvector = 0;
     while (list) {
	  if (node == FirstItem(list))
	       SetBit(v, di);
	  ++di;
	  list = Rest(list);
     }
     return v;
}

/*
 * ListSelect: returns sublist of list, containing the ith member of
 * list iff ith bit of v is 1.  Used by a transfer function to convert
 * a bitvector flow value to persistent form during final pass.
 */
PRIVATE List *ListSelect(List *list, FlowValue v)
{
     List *new = NULL;

     while (v.u.bitvector != 0 && list != NULL) {
	  if (v.u.bitvector & 1)
	       new = ConsItem(FirstItem(list), new);
	  list = Rest(list);
	  v.u.bitvector >>= 1;
     }
     return new;
}

/* same as above, but the returned list is restricted to be a subset
 * of set.
 */
PRIVATE List *ListSelectSubset(List *list, FlowValue v, List *set)
{
     List *new = NULL;

     while (v.u.bitvector != 0 && list != NULL) {
	  if ((v.u.bitvector & 1) && FindItem(set, FirstItem(list)))
	       new = ConsItem(FirstItem(list), new);
	  list = Rest(list);
	  v.u.bitvector >>= 1;
     }
     return new;
}

/*
 *  ApplyInChunks: splits up a list into bitvector-length sublists and applies
 *                 proc to each
 */
PRIVATE void ApplyInChunks(void (*proc) (Node *, List *), Node *root, List *list)
{
     ListMarker m;
     List *chunk;

     IterateList(&m, list);

     while ((chunk = NextChunkOnList(&m, MAX_BITVECTOR_LENGTH)) != NULL)
	  proc(root, chunk);
}

/*************************************************************************/
/*                                                                       */
/*                        Live-variable analysis                         */
/*                                                                       */
/*************************************************************************/

/*
 * VariableSet: set of variables currently being analyzed 
 */
PRIVATE List *VariableSet;

/* 
 * Live variable analysis is a typical c2c analysis framework, with three
 * parts:
 * 
 *   Control: splits up variable set into manageable (32-element) 
 *             partitions for analysis
 *   Initialization: sets up tree for analysis
 *   Transfer function: transforms live-variable set as it flows through
 *                      a node, and stores results on tree in final pass
 */

/* 
 * Initialization
 */

PRIVATE void InitLVLists(Node *node)
{
     LiveVars(node) = NULL;
}

PRIVATE void InitLVGenKill(Node *node)
{
     switch (node->typ) {
	 case Id:
	      Gen(node) = ListFind(VariableSet, node->u.id.decl);
	      Kill(node) = EmptySet;
	      break;

	 case Decl:
	      Gen(node) = EmptySet;
	      Kill(node) = ListFind(VariableSet, node);
	      break;

	 case Binop:
	      if (node->u.binop.op == '=' && node->u.binop.left->typ == Id) {
		   Node *left = node->u.binop.left;

		   Gen(node) = EmptySet;
		   Kill(node) = ListFind(VariableSet, left->u.id.decl);

		   /* reset variable's gen-kill sets, since it's an lvalue and
		    * shouldn't generate a use.  (Note that this function is
		    * being executed postorder, so left has already been visited.) */
		   Gen(left) = EmptySet;
		   Kill(left) = EmptySet;
	      }
	      break;

	 case Spawn:
	      {
		   Node *receiver = node->u.spawn.receiver;
		   if (receiver && receiver->typ == Id && node->u.spawn.assign_op == '=') {
			Gen(node) = EmptySet;
			Kill(node) = ListFind(VariableSet, receiver->u.id.decl);
			Gen(receiver) = EmptySet;
			Kill(receiver) = EmptySet;
		   } else {
			Gen(node) = EmptySet;
			Kill(node) = EmptySet;
		   }

		   break;
	      }

	 case InletCall:
	      {
		   Gen(node) = EmptySet;
		   Kill(node) = EmptySet;
		   /* additionally, we must add to gen all of the variables used by
		    * the inlet body.  These will be added in TransLV. -KHR
		    */
		   break;
	      }

	 default:
	      Gen(node) = EmptySet;
	      Kill(node) = EmptySet;
	      break;
     }
}

/*
 * Transfer function
 */

PRIVATE FlowValue TransLV(Node *node, FlowValue v, Point p, Bool final)
{
     switch (p) {
	 case EntryPoint:
#if 0
	      /* if you care about live variables on entry to an expression,
	       * instead of exit, then uncomment this code and comment out
	       * the duplicate code in the ExitPoint case */

	      if (final) {
		   List *livevars = ListSelect(VariableSet, v);
		   LiveVars(node) = JoinLists(livevars, LiveVars(node));
	      }
#endif
	      if (final && node->typ == Return) {
		   /* we need this to implement implicit syncs! */
		   List *livevars = ListSelect(VariableSet, v);
		   node->u.Return.livevars = JoinLists(livevars, node->u.Return.livevars);
	      }
	      return v;

	 case ExitPoint:
	      if (final) {
		   List *livevars = ListSelect(VariableSet, v);
		   LiveVars(node) = JoinLists(livevars, LiveVars(node));
	      }
	      /* do gen/kill only at ExitPoint of each expression, where Kills
	       * (assignments) presumably occur */
	      v = GenKill(node, v);

	      /* add variables used by inlet to flow value */
	      if (node->typ == InletCall) {
		   if (node->u.inletcall.name->typ == Id &&
		       node->u.inletcall.name->u.id.decl->u.decl.type->typ == Fdcl) {
			assert(node->u.inletcall.name->u.id.decl->u.decl.type->u.fdcl.proc_node != NULL);
			v = SetUnion(v, node->u.inletcall.name->u.id.decl->u.decl.type->u.fdcl.proc_node->u.proc.return_values);
			/* note: this is conservative - we aren't removing anything from
			 * the live variable set, such as variables which may be written
			 * by the inlet.  -KHR
			 */
		   } else {
			/*
			 * Inlet is referenced by pointer.  We don't know anything
			 * about the variables it uses, so add all of them to live set.
			 */
			assert(0);	/* not working yet */
			v = TotalSet;
		   }
	      }
	 default:
	      return v;
     }
}

/*
 * Control
 */

/* 
 * AnalyzeLV: performs analysis for small (<=32) set of variables 
 */
PRIVATE void AnalyzeLV(Node *root, List *vars)
{
     VariableSet = vars;

     WalkTree(root, (WalkProc) InitLVGenKill, NULL, Postorder);

     IterateDataFlow(
			 root,	/* root node */
			 EmptySet,	/* initial value: no variables live */
			 Backwards,	/* direction */
			 SetUnion,	/* meet operation */
			 SetEqual,	/* equality operation */
			 TransLV	/* transfer function */
	 );
}

/* 
 * AnalyzeLiveVariables: handles any size set of variables by splitting up
 *                        into small partitions
 */
GLOBAL void AnalyzeLiveVariables(Node *root, List *vars)
{
     /* initialize live variables list on every node to NULL */
     WalkTree(root, (WalkProc) InitLVLists, NULL, Preorder);

     /* compute flow for 32 variables at a time (to fit easily in bitvector) */
     ApplyInChunks(AnalyzeLV, root, vars);
}

/*************************************************************************/
/*                                                                       */
/*            Checking for function without return                       */
/*                                                                       */
/*************************************************************************/

#define REACHABLE        0
#define VALUE_RETURNED   1
#define NOTHING_RETURNED 2

PRIVATE FlowValue TransRF(Node *node, FlowValue v, Point p, Bool final)
{
     switch (p) {

	 case EntryPoint:
	      if (v.undefined && IsExpr(node) && node->typ != Block) {
		   if (final)
			WarningCoord(3, node->coord, "unreachable code");
		   /* make it defined so we don't get multiple warnings */
		   v = EmptySet;
	      }
	      return v;

	 case ExitPoint:
	      if (node->typ == Return) {
		   ClearBit(v, REACHABLE);
		   if (node->u.Return.expr)
			SetBit(v, VALUE_RETURNED);
		   else
			SetBit(v, NOTHING_RETURNED);
	      } else if (node->typ == Proc && final) {
		   Node *type = FunctionReturnType(node);
		   Coord end_coord;
		   assert(node->u.proc.body);
		   assert(node->u.proc.body->typ == Block);
		   end_coord = node->u.proc.body->u.Block.right_coord;

		   if (!IsVoidType(type) &&
		       (!TypeIsSint(type) ||
			tq_has_procedure(node->u.proc.decl->u.decl.type->u.fdcl.tq) ||
			tq_has_inlet    (node->u.proc.decl->u.decl.type->u.fdcl.tq)) &&
		       IsBitSet(v, REACHABLE)) {
			WarningCoord(3, end_coord,
			     "control reaches end of non-void function");
			node->u.proc.needs_return = TRUE;
		   } else if (IsBitSet(v, VALUE_RETURNED) &&
			      (IsBitSet(v, NOTHING_RETURNED) || IsBitSet(v, REACHABLE)))
			WarningCoord(3, node->coord,
			"function returns both with and without a value");

		   /* mark procedure as needing an implicit return */
		   if (IsVoidType(type) && IsBitSet(v, REACHABLE)) {
			node->u.proc.needs_return = TRUE;
		   }
	      }
	      return v;
	 default:
	      return v;
     }
}

GLOBAL void AnalyzeReturnFlow(Node *proc)
{
     FlowValue v = EmptySet;
     SetBit(v, REACHABLE);
     IterateDataFlow(proc,
		     v,
		     Forwards,
		     SetUnion,
		     SetEqual,
		     TransRF);
}

/*************************************************************************/
/*                                                                       */
/*      Checking for returns that might have outstanding children        */
/*                                                                       */
/*************************************************************************/

#define CHILDREN_OUTSTANDING 1

/* 
 * Initialization
 */

PRIVATE void InitSF(Node *node, void *UNUSED(ignore))
{
     Gen(node) = EmptySet;
     Kill(node) = EmptySet;

     switch (node->typ) {
	 case Spawn:
	 case InletCall:
	      SetBit(Gen(node), CHILDREN_OUTSTANDING);
	      break;
	 case Sync:
	      SetBit(Kill(node), CHILDREN_OUTSTANDING);
	      break;
	 case Return:
	      /* we will add implicit sync's to these returns */
	      SetBit(Kill(node), CHILDREN_OUTSTANDING);
	 default:
	      break;
     }
}

/*
 * Transfer function
 */

PRIVATE FlowValue TransSF(Node *node, FlowValue v, Point p, Bool final)
{
     switch (p) {

	 case EntryPoint:
	      return v;

	 case ExitPoint:
	      if (final && IsBitSet(v, CHILDREN_OUTSTANDING)) {
		   if (node->typ == Return)
			node->u.Return.needs_sync = TRUE;
		   if (node->typ == Proc) {
			assert(node->u.proc.needs_return);
			node->u.proc.needs_sync = TRUE;
		   }
	      }
	      v = GenKill(node, v);
	      return v;

	 default:
	      return v;
     }
}

GLOBAL void AnalyzeSyncFlow(Node *proc)
{
     WalkTree(proc, InitSF, NULL, Preorder);
     IterateDataFlow(proc,
		     EmptySet,
		     Forwards,
		     SetUnion,
		     SetEqual,
		     TransSF);
}

/*************************************************************************/
/*                                                                       */
/*                        Dirty-variable analysis                        */
/*                                                                       */
/*************************************************************************/

/* 
 * Initialization
 */

PRIVATE void InitDVLists(Node *node)
{
     DirtyVars(node) = NULL;
}

PRIVATE void InitDVGenKill(Node *node)
{
     Gen(node) = EmptySet;
     Kill(node) = EmptySet;

     switch (node->typ) {
	 case Binop:
	      if (IsAssignmentOp(node->u.binop.op) &&
		  node->u.binop.left->typ == Id)
		   Gen(node) = ListFind(VariableSet, node->u.binop.left->u.id.decl);
	      break;

	 case Decl:
	      if (node->u.decl.init)
		   Gen(node) = ListFind(VariableSet, node);
	      break;

	 case Unary:
	      if (node->typ == Unary && node->u.unary.op == SIZEOF)
		   return;	/* don't look inside sizeof() */
	      if (node->typ == Unary && node->u.unary.op == ALIGNOF)
		   return;	/* don't look inside __alignof__() */
	      else if (IsIncDecOp(node->u.unary.op) &&
		       node->u.unary.expr->typ == Id)
		   Gen(node) = ListFind(VariableSet, node->u.binop.left->u.id.decl);
	      break;

	 case Spawn:
	      if (!node->u.spawn.in_inlet) {
		   if (node->u.spawn.receiver &&
		       node->u.spawn.receiver->typ == Id)
			Gen(node) = ListFind(VariableSet, node->u.spawn.receiver->u.id.decl);
		   /* a spawn saves all dirty, live variables */
		   Kill(node) = TotalSet;
	      }
	      break;

	 case Sync:
	      /*
	       * I am not sure what should happen here. Now I pretend that
	       * sync is a no-op for the purposes of dataflow analysis.
	       * In Cilk-n, n<4, sync saved all dirty/live variables.
	       * This is no longer true in Cilk-4.
	       * -- athena
	       * You are correct -KHR
	       */
#if 0
	      /* a sync saves all dirty, live variables */
	      Kill(node) = TotalSet;
#endif

	      break;

	 case InletCall:
	      /* an inletcall saves all dirty, live variables */
	      Kill(node) = TotalSet;
	      /* additionally, we must add to gen all of the variables written by
	       * the inlet body.  These will be added in TransDV. -KHR
	       */
	 default:
	      break;
     }
}

/*
 * Transfer function
 */

PRIVATE FlowValue TransDV(Node *node, FlowValue v, Point p, Bool final)
{
     switch (p) {
	 case EntryPoint:
	      if (final && node->typ == Return) {
		   /* we need this to implement implicit syncs! */
		   node->u.Return.dirtyvars =
		       JoinLists(ListSelectSubset(VariableSet,
						  v,
						node->u.Return.livevars),
				 node->u.Return.dirtyvars);
	      }
	      return v;

	 case ExitPoint:
	      if (final) {
		   /* Dirty variables are variables which need to be written to
		    * the frame during a spawn or sync.  They include any variable
		    * modified (i.e. in v) before the spawn's assignment (if any),
		    * or before the sync which is also live after the spawn or sync.
		    */
		   DirtyVars(node) = JoinLists(ListSelectSubset(VariableSet,
								v,
							 LiveVars(node)),
					       DirtyVars(node));
	      }
	      v = GenKill(node, v);

	      /* add variables written by inlet to flow value */
	      if (node->typ == InletCall) {
		   if (node->u.inletcall.name->typ == Id &&
		       node->u.inletcall.name->u.id.decl->u.decl.type->typ == Fdcl) {
			assert(node->u.inletcall.name->u.id.decl->u.decl.type->u.fdcl.proc_node != NULL);
			v = SetUnion(v, node->u.inletcall.name->u.id.decl->u.decl.type->u.fdcl.proc_node->u.proc.return_values);
		   } else {
			/*
			 * Inlet is referenced by pointer.  We don't know which
			 * variables it will dirty, so dirty all of them.
			 */
			assert(0);	/* not working yet */
			v = TotalSet;
		   }
	      }
	      return v;

	 default:
	      return v;
     }
}

/*
 * Control
 */

/* 
 * AnalyzeDV: performs analysis for small (<=32) set of variables 
 */
PRIVATE void AnalyzeDV(Node *root, List *vars)
{
     FlowValue formals = EmptySet;
     ListMarker marker;
     Node *var;

     VariableSet = vars;

     /* start with formal parameters being dirty (accurate for fast version,
      * conservative for slow version). */
     IterateList(&marker, vars);
     while (NextOnList(&marker, (GenericREF) & var)) {
	  if (tq_has_formal_decl(NodeDeclLocation(var)))
	       formals = SetUnion(formals, ListFind(vars, var));
     }

     WalkTree(root, (WalkProc) InitDVGenKill, NULL, Postorder);

     IterateDataFlow(
			 root,	/* root node */
			 formals,	/* initial value: formal variables dirty */
			 Forwards,	/* direction */
			 SetUnion,	/* meet operation */
			 SetEqual,	/* equality operation */
			 TransDV	/* transfer function */
	 );
}

/* 
 * AnalyzeDirtyVariables: handles any size set of variables by splitting up
 *                        into small partitions
 */
GLOBAL void AnalyzeDirtyVariables(Node *root, List *vars)
{
     /* initialize dirty variables list on every node to NULL */
     WalkTree(root, (WalkProc) InitDVLists, NULL, Preorder);

     /* compute flow for 32 variables at a time (to fit easily in bitvector) */
     ApplyInChunks(AnalyzeDV, root, vars);
}

/*************************************************************************/
/*                                                                       */
/*                        Printing analysis results                      */
/*                                                                       */
/*************************************************************************/

GLOBAL void PV(List *vars)
{
    GBUF *gb = make_file_gbuf(stdout);
    PrintVariables(gb, vars);
    
    putchar('\n');
    fflush(stdout);
    free_gbuf(gb);
}

GLOBAL int PrintVariables(GBUF *gb, List *vars)
{
     ListMarker m;
     Node *var;
     int len = 0;

     IterateList(&m, vars);
     while (NextOnList(&m, (GenericREF) & var))
	  len += gbprintf(gb, "%s ", var->u.decl.name);
     return len;
}

GLOBAL int PrintAnalysis(GBUF *gb, Node *node)
{
     int len = 0;

     len += gbprintf(gb, "Live: ");
     len += PrintVariables(gb, LiveVars(node));
     len += gbprintf(gb, "Dirty: ");
     len += PrintVariables(gb, DirtyVars(node));
     return len;
}

/*************************************************************************/
/*                                                                       */
/*               Finding unaliased local variables                       */
/*                                                                       */
/*************************************************************************/

PRIVATE List *RemoveAliased(Node *node, List *vars);

/* 
 * Returns list of Decls which are potential candidates for registers.
 * Includes local, scalar variable decls appearing below node.
 * Omits arrays, structures, statics, externs, and variables whose
 * address has been taken.
 */
GLOBAL List *RegisterVariables(Node *node, List *vars)
{
     switch (node->typ) {
	 case Decl:
	      {
		   TypeQual dl = NodeDeclLocation(node);
		   TypeQual sc = NodeStorageClass(node);

		   if ((tq_has_block_decl(dl) || tq_has_formal_decl(dl)) &&
		       (!tq_has_typedef(sc) && !tq_has_extern(sc) && !tq_has_static(sc)) &&
		       (IsScalarType(NodeDataType(node)) ||
			IsPointerType(NodeDataType(node))))
			vars = ConsItem(node, vars);
		   break;
	      }

	 case Unary:
	      if (node->u.unary.op == ADDRESS)
		   /* remove any decls referenced by & */
		   vars = RemoveAliased(node->u.unary.expr, vars);
	      break;

	 default:
	      break;
     }

     /* An inlet left in the declaration for SerialVersion need not be searched,
      * because an equivalent block has already been appended at the block end.
      * But at the Analyze phase, the inlet has not yet been expanded, so it
      * should be searched.
      */
     /*  if ((node->typ != Proc) || strcmp(PhaseName, "Transform")) { KHR */

     /* don't look inside local procedures for register variables */
     if ((node->typ == Proc) &&
	 !tq_has_top_decl(node->u.proc.decl->u.decl.tq))
	  return vars;

#define CODE(n)   vars = RegisterVariables(n, vars)
     ASTWALK(node, CODE)
#undef CODE

	 return vars;
}

PRIVATE List *RemoveAliased(Node *node, List *vars)
{
     switch (node->typ) {
	 case Id:
	      assert(node->u.id.decl);
	      assert(node->u.id.decl->typ == Decl);
	      vars = RemoveItem(vars, node->u.id.decl);
	      break;

	 case Binop:
	      /* do we need this case? Records can't be register variables anyway. -KHR */
	      if (node->u.binop.op == '.')
		   vars = RemoveAliased(node->u.binop.left, vars);
	      break;

	 default:
	      break;
     }

     return vars;
}

/*************************************************************************/
/*                                                                       */
/*                                                                       */
/*            Finding unchecked calls with pointer arguments             */
/*                                                                       */
/*************************************************************************/

/* 
 * Make a name list of global procedure/functions (with pointer arguments)
 * being checked, in order to detect those calls has read/write sideeffects.
 */
PRIVATE List *CheckedProcNameList = NULL;

PRIVATE const char *KnownSafeProc[] = { 
  "Cilk_read_check", "Cilk_write_check",
  "Cilk_free", "Cilk_internal_free", "Cilk_die",
  "Cilk_realloc", "Cilk_lock", "Cilk_unlock", "Cilk_lock_init",
  "Cilk_publish", "Cilk_publish_array",
  "Cilk_parse_command_line", "Cilk_init", "Cilk_run",
  NULL
};

PRIVATE Node *CastToEnd(Node *node)
{
  if (node->typ == Cast)
    return CastToEnd(node->u.cast.expr);
  else if (node->typ == ImplicitCast)
    return CastToEnd(node->u.implicitcast.expr);
  else
    return node;
}

PRIVATE int HasPointerArg(List *args)
{
  ListMarker m;
  Node *n;

  IterateList(&m, args);
  while (NextOnList(&m, (GenericREF) &n)) {
    if ((IsPointerType(NodeDataType(n)) || IsArrayType(NodeDataType(n))) &&
	(!NodeIsConstant(CastToEnd(n))))
      return TRUE;
  }
  return FALSE;
}

PRIVATE void GrabCheckedProcName(List *program)
{
  ListMarker m;
  Node *n;

  IterateList(&m, program);
  while (NextOnList(&m, (GenericREF) &n))
    if (n && n->typ == Proc && n->memorycheckedp && 
	HasPointerArg(n->u.proc.decl->u.decl.type->u.fdcl.args))
      CheckedProcNameList = ConsItem(n->u.proc.decl, CheckedProcNameList);
}

PRIVATE int InCheckedProcNameList(const char *name)
{
  ListMarker m;
  Node *n;
  const char **p;

  IterateList(&m, CheckedProcNameList);
  while (NextOnList(&m, (GenericREF) &n)) {
    if (strcmp(name, n->u.decl.name) == 0)
      return TRUE;
  }

  /* check whether in KnownSafeProc */
  for (p = KnownSafeProc; *p; ++p) {
    if (strncmp(name, *p, strlen(*p)) == 0)
      return TRUE;
  }

  return FALSE;
}

PRIVATE Node *ClearNd(Node *node)
{
  node->memorycheckedp = FALSE;
  node->nondeterminator = FALSE;

#define CHILD(n) ClearNd(n)
  ASTWALK(node, CHILD)
#undef CHILD

  return node;
}  

PRIVATE void ModifyCilkCheck(Node *node)
{
  Node *n;

  if (MemoryChecking && (node->u.call.name->typ == Id)) {
    if (strcmp(node->u.call.name->u.id.text, "Cilk_read_check") == 0) {
      assert(ListLength(node->u.call.args) == 2);
      n = MakeId("_READ_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
      node->u.call.args = AppendItem(
			    AppendItem(node->u.call.args, MakeConstSint(9)),
			    FirstItem(node->u.call.args));
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_write_check") == 0) {
      assert(ListLength(node->u.call.args) == 2);
      n = MakeId("_WRITE_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
      node->u.call.args =AppendItem(
			   AppendItem(node->u.call.args, MakeConstSint(9)),
			   FirstItem(node->u.call.args));
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_malloc") == 0) {
      assert(ListLength(node->u.call.args) == 1);
      n = MakeId("_MALLOC_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_free") == 0) {
      assert(ListLength(node->u.call.args) == 1);
      n = MakeId("_FREE_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
      node->u.call.args = AppendItem(node->u.call.args,
				     ClearNd(NodeCopy(
					       FirstItem(node->u.call.args),
					       Subtree)));
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_memalign") == 0) {
      assert(ListLength(node->u.call.args) == 2);
      n = MakeId("_MEMALIGN_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_calloc") == 0) {
      assert(ListLength(node->u.call.args) == 2);
      n = MakeId("_CALLOC_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_realloc") == 0) {
      assert(ListLength(node->u.call.args) == 2);
      n = MakeId("_REALLOC_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_valloc") == 0) {
      assert(ListLength(node->u.call.args) == 1);
      n = MakeId("_VALLOC_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_lock") == 0) {
      assert(ListLength(node->u.call.args) == 1);
      n = MakeId("_LOCK_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
    if (strcmp(node->u.call.name->u.id.text, "Cilk_unlock") == 0) {
      assert(ListLength(node->u.call.args) == 1);
      n = MakeId("_UNLOCK_CHECK");
      n->coord = node->u.call.name->coord;
      node->u.call.name = n;
    }
  }
}

PRIVATE int HasCheckAttrib(List *attrib_list)
{
  ListMarker m;
  Node *n;

  IterateList(&m, attrib_list);
  while (NextOnList(&m, (GenericREF) &n)) {
    assert(n->typ == Attrib);
    if (strcmp(n->u.attrib.name, "check") == 0)
      return 1;
  }
  return 0;
}

PRIVATE int HasNoCheckAttrib(List *attrib_list)
{
  ListMarker m;
  Node *n;

  IterateList(&m, attrib_list);
  while (NextOnList(&m, (GenericREF) &n)) {
    assert(n->typ == Attrib);
    if (strcmp(n->u.attrib.name, "nocheck") == 0)
      return 1;
  }
  return 0;
}

/*************************************************************************/
/*                                                                       */
/*                    Finding shared memory references                   */
/*                                                                       */
/*************************************************************************/

/*
 * use bottom-up tree-search to find all shared memory reference.
 * only those Ids which are global, non-private, non-function will be checked.
 * node->nondeterminator is TRUE if node potentially accesses shared location.
 */
PRIVATE void AnalyzeGlobalReadWrite(Node *node)
{
  switch (node->typ) {
  case Id:
    node->nondeterminator = node->u.id.decl ? 
                     ((DeclIsExternal(node->u.id.decl) ||
		       DeclIsStatic(node->u.id.decl)) &&
		      (!tq_has_private(NodeDeclTq(node->u.id.decl))) &&
		      (!IsFunctionType(node->u.id.decl->u.decl.type)) &&
		      (!HasNoCheckAttrib(node->u.id.decl->u.decl.attribs))) :
			FALSE;
    break;
  case Call:
    if (node->memorycheckedp && node->memorywarningp && 
	HasPointerArg(node->u.call.args)) {
      Node *callid;

      callid = CastToEnd(node->u.call.name);
      if ((callid->typ != Id) || (!InCheckedProcNameList(callid->u.id.text))) {
	if (callid->typ == Id) {
	  if (!HasCheckAttrib(callid->u.id.decl->u.decl.attribs))
	    WarningCoord(1, node->coord,
			 "(for the Nondeterminator) `%s' may access shared "
			 "memory through its pointer arguments", 
			 callid->u.id.text);
	}
	else
	  WarningCoord(1, node->coord,
		       "(for the Nondeterminator) the procedure or function "
		       "may access shared memory through its pointer "
		       "arguments");
      }
    }
    ModifyCilkCheck(node);
    break;
  case Spawn:
    if (node->memorycheckedp && node->memorywarningp && 
	HasPointerArg(node->u.spawn.args)) {
      Node *spawnid;

      spawnid = CastToEnd(node->u.spawn.name);
      if ((spawnid->typ != Id)||(!InCheckedProcNameList(spawnid->u.id.text))) {
	if (spawnid->typ == Id) {
	  if (!HasCheckAttrib(spawnid->u.id.decl->u.decl.attribs))
	    WarningCoord(1, node->coord,
			 "(for the Nondeterminator) `%s' may access shared "
			 "memory through its pointer arguments", 
			 spawnid->u.id.text);
	}
	else
	  WarningCoord(1, node->coord,
		       "(for the Nondeterminator) the procedure or function "
		       "may access shared memory through its pointer "
		       "arguments");
      }
    }
    break;
  default:
    break;
  }

#define CODE(n)   AnalyzeGlobalReadWrite(n)
  ASTWALK(node, CODE)
#undef CODE
    
  switch (node->typ) {
  case Array:
    if (!((node->u.array.name->typ == Id) &&
	  (tq_has_block_decl(NodeDeclLocation(node->u.array.name->u.id.decl)) ||
	   tq_has_private(NodeDeclTq(node->u.array.name->u.id.decl))) &&
	  (IsArrayType(NodeDataType(node->u.array.name))))) {
      node->nondeterminator = TRUE;
      node->u.array.name->nondeterminator = FALSE;
    }
    break;
  case Binop:
    if (IsAssignmentOp(node->u.binop.op) && 
	node->u.binop.left->nondeterminator) {
      node->nondeterminator = TRUE;
      node->u.binop.left->nondeterminator = FALSE;
    }
    else if ((node->u.binop.op == '.') && 
	     node->u.binop.left->nondeterminator) {
      node->nondeterminator = TRUE;
      node->u.binop.left->nondeterminator = FALSE;
    }
    else if (node->u.binop.op == ARROW) {
      /* may need check left operator when left is not Cilk_private_vars */
      if ((node->u.binop.left->typ == Id) &&
	  (strcmp(node->u.binop.left->u.id.text, "Cilk_private_vars") == 0))
	node->u.binop.left->nondeterminator = FALSE;
      else
	node->nondeterminator = TRUE;
    }
    break;
  case Unary:
    if (IsIncDecOp(node->u.unary.op) && node->u.unary.expr->nondeterminator) {
      node->nondeterminator = TRUE;
      node->u.unary.expr->nondeterminator = FALSE;
    }
    else if (node->u.unary.op == INDIR) {
      if (!((node->u.unary.expr->typ == Id) &&
	    (tq_has_block_decl(NodeDeclLocation(node->u.unary.expr->u.id.decl)) ||
	     tq_has_private(NodeDeclTq(node->u.unary.expr->u.id.decl))) &&
	    (NodeDataType(node->u.unary.expr)->typ == Adcl))) {
	node->nondeterminator = TRUE;
	/* may need check expression
	node->u.unary.expr->nondeterminator = FALSE;
	*/
      }
    }
    else if (node->u.unary.op == ADDRESS) {
      node->nondeterminator = FALSE;
      node->u.unary.expr->nondeterminator = FALSE;
    }
    break;
  case Spawn:
    if (node->u.spawn.receiver && node->u.spawn.receiver->nondeterminator) {
      node->u.spawn.receiver->nondeterminator = FALSE;
    }
    break;
  case InletCall:
    break;
  case Call:
    if (MemoryChecking && (node->u.call.name->typ == Id)) {
      if (strcmp(node->u.call.name->u.id.text, "_LOCK_CHECK") == 0)
	((Node *) FirstItem(node->u.call.args))->nondeterminator = FALSE;
      if (strcmp(node->u.call.name->u.id.text, "_UNLOCK_CHECK") == 0)
	((Node *) FirstItem(node->u.call.args))->nondeterminator = FALSE;
    }
  default:
    break;
  }
}

/*************************************************************************/
/*                                                                       */
/*                    Optimize shared array references                   */
/*                                                                       */
/*************************************************************************/

/* 
 * simple loop variable analysis to move checking of an array out of For loop.
 * this optimization only works on Id and one-dimentional array reference.
 * eliminate some redundant checks when an Id or array references is checked.
 */
PRIVATE List *global_read_id_list = NULL;
PRIVATE List *global_write_id_list = NULL;
PRIVATE List *global_read_array_list = NULL;
PRIVATE List *global_write_array_list = NULL;
PRIVATE List *global_read_array_name_list = NULL;
PRIVATE List *global_write_array_name_list = NULL;
PRIVATE List *global_read_array_index_list = NULL;
PRIVATE List *global_write_array_index_list = NULL;
PRIVATE List *global_read_array_index_offset_list = NULL;
PRIVATE List *global_write_array_index_offset_list = NULL;
PRIVATE Node *loop_var_decl;
PRIVATE int loop_var_start;
PRIVATE int loop_var_offset;

PRIVATE void add_into_list(Node *node_write, Node *node, List **id_list,
			   List **array_list, List **array_name_list,
			   List **array_index_list, 
			   List **array_index_offset_list)
{
  ListMarker marker1, marker2, marker3, marker4;
  Node *item1, *item2, *item3, *node1, *index0, *index1;
  size_t offset, offset_value;
  unsigned int item_offset;

  if (node->typ == Id) {
    IterateList(&marker1, *id_list);
    while (NextOnList(&marker1, (GenericREF) &item1)) {
      assert(item1->typ == Id);
      if (node->u.id.decl == item1->u.id.decl) {
	node_write->nondeterminator = FALSE;
	return;
      }
    }
    *id_list = ConsItem(node, *id_list);
  }
  else if ((node->typ == Array) &&
	   (ListLength(node->u.array.dims) == 1) && 
	   (((node1 = node->u.array.name)->typ == Id) ||
	    ((node->u.array.name->typ == Cast) &&
	     ((node1 = CastToEnd(node->u.array.name))->typ == Id)))) {
    /* we only handle one-dimentional array at this moment */

    offset = 0;
    index0 = index1 = (Node *) FirstItem(node->u.array.dims);
    if ((index1->typ == Id) ||
	((index1->typ == Binop) && (index1->u.binop.op == '+') &&
	 (NodeIsConstant(index1->u.binop.right)) &&
	 ((offset = NodeConstantIntegralValue(index1->u.binop.right)) > 0) &&
	 ((index1 = index1->u.binop.left)->typ == Id)) ||
	NodeIsConstant(index1)) {
      IterateList(&marker1, *array_list);
      IterateList(&marker2, *array_name_list);
      IterateList(&marker3, *array_index_list);
      IterateList(&marker4, *array_index_offset_list);
      while (NextOnList(&marker1, (GenericREF) &item1)) {
	NextOnList(&marker2, (GenericREF) &item2);
	NextOnList(&marker3, (GenericREF) &item3);
	NextOnList(&marker4, (GenericREF) &offset_value);
	if (item2->u.array.name->typ == Cast) {
	  if ((node->u.array.name->typ != Cast) ||
	      (!TypeEqual(node->u.array.type, item2->u.array.type)))
	    continue;
	  item2 = CastToEnd(item2->u.array.name);
	  assert(item2->typ == Id);
	  item2 = item2->u.id.decl;
	}
	else {
	  if ((node->u.array.name->typ == Cast) ||
	      (!TypeEqual(node->u.array.type, item2->u.array.type)))
	    continue;
	  item2 = item2->u.array.name->u.id.decl;
	}
	if (node1->u.id.decl == item2) {
	  if (item3->typ == Id) {
	    if (index1->typ == Id) {
	      if (index1->u.id.decl == item3->u.id.decl) {
		if (offset <= offset_value) {
		  node_write->nondeterminator = FALSE;
		  return;
		}
		else if (offset == (offset_value + 1)) {
		  SetCurrentOnList(&marker4, (GenericREF *) offset);
		  node_write->nondeterminator = FALSE;
		  item1->nondeterminator = offset_value + 2;
		  return;
		}
	      }
	    }
	  }
	  else { /* item3 is a constant */
	    if (NodeIsConstant(index1)) {
	      offset = NodeConstantIntegralValue(index1);
	      item_offset = NodeConstantIntegralValue(item3);
	      if ((offset >= item_offset) && 
		  (offset <= (item_offset + offset_value))) {
		node_write->nondeterminator = FALSE;
		return;
	      }
	      else if (offset == (item_offset + offset_value + 1)) {
		SetCurrentOnList(&marker4, (GenericREF *)(offset_value + 1));
		node_write->nondeterminator = FALSE;
		item1->nondeterminator = offset_value + 2;

		/* see whether we could merge any one in the list */
		{ ListMarker m1, m2, m3, m4;
		  Node *i1, *i2, *i3;
		  size_t i4;

		  IterateList(&m1, *array_list);
		  IterateList(&m2, *array_name_list);
		  IterateList(&m3, *array_index_list);
		  IterateList(&m4, *array_index_offset_list);
		  while (NextOnList(&m1, (GenericREF) &i1)) {
		    NextOnList(&m2, (GenericREF) &i2);
		    NextOnList(&m3, (GenericREF) &i3);
		    NextOnList(&m4, (GenericREF) &i4);
		    if ((node1->u.id.decl == 
			 i2->u.array.name->u.id.decl) &&
			NodeIsConstant(i3) &&
			(NodeConstantIntegralValue(i3) == (offset + 1))) {
		      i1->nondeterminator = FALSE;
		      SetCurrentOnList(&marker4, 
				       (GenericREF *)(offset_value + i4 + 2));
		      *array_list = RemoveItem(*array_list, i1);
		      *array_name_list = RemoveItem(*array_name_list, i2);
		      *array_index_list = RemoveItem(*array_index_list, i3);
		      *array_index_offset_list = RemoveItem(*array_index_offset_list,
							    (GenericREF *) i4);
		    }
		  }
		}

		return;
	      }
	    }
	  }
	}
      }
      if ((index0->typ == Id) || NodeIsConstant(index0)) {
	*array_list = ConsItem(node_write, *array_list);
	*array_name_list = ConsItem(node, *array_name_list);
	*array_index_list = ConsItem(index0, *array_index_list);
	*array_index_offset_list = ConsItem((GenericREF *) 0, 
					    *array_index_offset_list);
      }
    }
  }
}

PRIVATE int is_simple_for(Node *n)
{
  Node *init, *cond, *next;
  int init_value, cond_value, region;
  OpType cond_op, next_op;

  assert(n->typ == For);
  init = n->u.For.init;
  cond = n->u.For.cond;
  next = n->u.For.next;
  if ((!init) ||
      (init->typ != Binop) || 
      (init->u.binop.op != '=') ||
      (!IsIntegralType(init->u.binop.type)) ||
      (init->u.binop.left->typ != Id) ||
      (!NodeIsConstant(init->u.binop.right)))
    return 0;
  loop_var_decl = init->u.binop.left->u.id.decl;
  init_value = NodeConstantIntegralValue(init->u.binop.right);

  if ((!cond) ||
      (cond->typ != Binop) || 
      (!IsComparisonOp(cond->u.binop.op)) ||
      (!IsIntegralType(cond->u.binop.type)) ||
      (cond->u.binop.left->typ != Id) ||
      (cond->u.binop.left->u.id.decl != loop_var_decl) ||
      (!NodeIsConstant(cond->u.binop.right)))
    return 0;
  cond_op = cond->u.binop.op;
  cond_value = NodeConstantIntegralValue(cond->u.binop.right);
  
  if ((!next) ||
      (next->typ != Unary) ||
      (!IsIncDecOp(next->u.unary.op)) ||
      (next->u.unary.expr->typ != Id) ||
      (next->u.unary.expr->u.id.decl != loop_var_decl)) {
    if ((next->typ != Binop) ||
	(next->u.binop.op != PLUSassign) ||
	(next->u.binop.left->typ != Id) ||
	(next->u.binop.left->u.id.decl != loop_var_decl) ||
	(!NodeIsConstant(next->u.binop.right)))
      return 0;
    else {
      next_op = next->u.binop.op;
      loop_var_offset = NodeConstantIntegralValue(next->u.binop.right);
    }
  }
  else {
    next_op = next->u.unary.op;
    loop_var_offset = 1;
  }


  switch (cond_op) {
  case EQ:
    switch (next_op) {
    case PREINC:
    case POSTINC:
    case PLUSassign:
      loop_var_start = init_value;
      region = cond_value - init_value + 1;
      break;
    case PREDEC:
    case POSTDEC:
      loop_var_start = cond_value;
      region = init_value - cond_value + 1;
      break;
    default:
      return 0;
    }
    break;
  case '<':
    loop_var_start = init_value;
    region = cond_value - init_value;
    break;
  case LE:
    loop_var_start = init_value;
    region = cond_value - init_value + 1;
    break;
  case '>':
    loop_var_start = cond_value;
    region = init_value - cond_value;
    break;
  case GE:
    loop_var_start = cond_value;
    region = init_value - cond_value + 1;
    break;
  default:
    return 0;
  }
  
  if (next_op == PLUSassign)
    return(((region + loop_var_offset - 1) / loop_var_offset) * loop_var_offset);
  else
    return region;
}

PRIVATE void OptimizeGlobalReadWrite(Node *node)
{
  int loop_times;

      List *rlist = NULL, *wlist = NULL;
      ListMarker marker1, marker2, marker3, marker4;
      Node *item1, *item2, *item3, *array;
      int item4;

  if (node->nondeterminator)
    switch (node->typ) {
    case Binop:
      if (node->u.binop.op == '=')
	add_into_list(node,
		      node->u.binop.left,
		      &global_write_id_list,
		      &global_write_array_list,
		      &global_write_array_name_list,
		      &global_write_array_index_list,
		      &global_write_array_index_offset_list);
      break;
    /* since we need record the write operator, we don't handle general write
    case Binop:
      if (IsAssignmentOp(node->u.binop.op))
	add_into_list(node,
		      node->u.binop.left,
		      &global_write_id_list,
		      &global_write_array_list,
		      &global_write_array_name_list,
		      &global_write_array_index_list,
		      &global_write_array_index_offset_list);
      break;
    case Unary:
      if (IsIncDecOp(node->u.unary.op))
	add_into_list(node,
		      node->u.unary.expr,
		      &global_write_id_list,
		      &global_write_array_list,
		      &global_write_array_name_list,
		      &global_write_array_index_list,
		      &global_write_array_index_offset_list);
      break;
    */
    case Proc:
      break;
    default:
      add_into_list(node,
		    node,
		    &global_read_id_list,
		    &global_read_array_list,
		    &global_read_array_name_list,
		    &global_read_array_index_list,
		    &global_read_array_index_offset_list);
      break;
    }

  switch(node->typ) {
  case For:
    global_read_id_list = NULL;
    global_write_id_list = NULL;
    global_read_array_list = NULL;
    global_write_array_list = NULL;
    global_read_array_name_list = NULL;
    global_write_array_name_list = NULL;
    global_read_array_index_list = NULL;
    global_write_array_index_list = NULL;
    global_read_array_index_offset_list = NULL;
    global_write_array_index_offset_list = NULL;
    break;
  default:
    break;
  }

#define CODE(n)   OptimizeGlobalReadWrite(n)
  ASTWALK(node, CODE)
#undef CODE
    
  switch (node->typ) {
  case For:
    if ((loop_times = is_simple_for(node)) > 1) {
      if (global_read_id_list) {
	IterateList(&marker1, global_read_id_list);
	while (NextOnList(&marker1, (GenericREF) &item1))
	  item1->nondeterminator = FALSE;
      }
	    
      if (global_write_id_list) {
	IterateList(&marker1, global_write_id_list);
	while (NextOnList(&marker1, (GenericREF) &item1))
	  item1->nondeterminator = FALSE;
      }

      if (global_read_array_list) {
	IterateList(&marker1, global_read_array_list);
	IterateList(&marker2, global_read_array_name_list);
	IterateList(&marker3, global_read_array_index_list);
	IterateList(&marker4, global_read_array_index_offset_list);
	while (NextOnList(&marker1, (GenericREF) &item1)) {
	  NextOnList(&marker2, (GenericREF) &item2);
	  NextOnList(&marker3, (GenericREF) &item3);
	  NextOnList(&marker4, (GenericREF) &item4);
	  if (item3->typ == Id) {
	    if ((item3->u.id.decl == loop_var_decl) &&
		(item4 == (loop_var_offset - 1))) {
	      array = NodeCopy(item2, Subtree);
	      assert(array->typ == Array);
	      array->coord = UnknownCoord;
	      array->u.array.type->coord = UnknownCoord;
	      array->u.array.name->coord = UnknownCoord;
	      array->u.array.dims = List1(MakeConstSint(loop_var_start));
	      rlist = ConsItem(List2(array, MakeConstSint(loop_times)), 
			       rlist);
	      item1->nondeterminator = FALSE;
	    }
	  }
	}
      }
	    
      if (global_write_array_list) {
	IterateList(&marker1, global_write_array_list);
	IterateList(&marker2, global_write_array_name_list);
	IterateList(&marker3, global_write_array_index_list);
	IterateList(&marker4, global_write_array_index_offset_list);
	while (NextOnList(&marker1, (GenericREF) &item1)) {
	  NextOnList(&marker2, (GenericREF) &item2);
	  NextOnList(&marker3, (GenericREF) &item3);
	  NextOnList(&marker4, (GenericREF) &item4);
	  if (item3->typ == Id) {
	    if ((item3->u.id.decl == loop_var_decl) &&
		(item4 == (loop_var_offset - 1))) {
	      array = NodeCopy(item2, Subtree);
	      assert(array->typ == Array);
	      array->coord = UnknownCoord;
	      array->u.array.type->coord = UnknownCoord;
	      array->u.array.name->coord = UnknownCoord;
	      array->u.array.dims = List1(MakeConstSint(loop_var_start));
	      wlist = ConsItem(List2(array, MakeConstSint(loop_times)), 
			       wlist);
	      item1->nondeterminator = FALSE;
	    }
	  }
	}
      }
	    
      if (global_read_id_list || global_write_id_list || rlist || wlist) {
	node->nondeterminator = TRUE;
	node->u.For.rwlist = List4(global_read_id_list, 
				    global_write_id_list,
				    rlist, wlist);
      }
    }
  case Ternary:
  case Initializer:
  case Switch:
  case If:
  case IfElse:
  case While:
  case Do:
  case Goto:
  case Continue:
  case Break:
  case Return:
  case Spawn:
  case Sync:
  case InletCall:
  case Abort:
  case Synched:
  case Proc:
    global_read_id_list = NULL;
    global_write_id_list = NULL;
  case Call:
    global_read_array_list = NULL;
    global_write_array_list = NULL;
    global_read_array_name_list = NULL;
    global_write_array_name_list = NULL;
    global_read_array_index_list = NULL;
    global_write_array_index_list = NULL;
    global_read_array_index_offset_list = NULL;
    global_write_array_index_offset_list = NULL;
    break;
  default:
    break;
  }
}

/*************************************************************************/
/*                                                                       */
/*                        Running analysis                               */
/*                                                                       */
/*************************************************************************/

GLOBAL void AnalyzeProgram(List *program)
{
     ListMarker m;
     Node *n;

     if (MemoryChecking)
	 GrabCheckedProcName(program);
     IterateList(&m, program);
     while (NextOnList(&m, (GenericREF) &n))
	 if (n && n->typ == Proc) {
	     List *locals = RegisterVariables(n, NULL);
	    
	     AnalyzeLiveVariables(n, locals);
	     AnalyzeDirtyVariables(n, locals);
	     AnalyzeReturnFlow(n);
	     AnalyzeSyncFlow(n);
	     AnalyzeGlobalReadWrite(n);
	     if (OptimizeChecking)
		 OptimizeGlobalReadWrite(n);
	 }
}
