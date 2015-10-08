/* -*- c-indentation-style: "k&r"; c-basic-offset: 5 -*- */
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Rob Miller
 * 
 *************************************************************************/
/*
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 * Copyright (c) 2003 Jim Sukha
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/transform.c $ $LastChangedBy: bradley $ $Rev: 2739 $ $Date: 2006-02-09 00:16:43 -0500 (Thu, 09 Feb 2006) $");

#include <ctype.h>
#include "astEquals.h"
#include "stringParse.h"

/* CilkPrefix: prefix for all preprocessor-generated symbols */
GLOBAL const char *CilkPrefix = "_cilk_";

PRIVATE char CilkNameBuf[100];
PRIVATE const char *ApplyPrefix(const char *root)
{
     sprintf(CilkNameBuf, "%s%s", CilkPrefix, root);
     return UniqueString(CilkNameBuf);
}

PRIVATE const char *MakeSyncName(int n)
{
     sprintf(CilkNameBuf, "%s%s%d", CilkPrefix, "sync", n);
     return (UniqueString(CilkNameBuf));
}
PRIVATE const char *MakeFrameTypeName(const char *proc)
{
     sprintf(CilkNameBuf, "%s%s_frame", CilkPrefix, proc);
     return (UniqueString(CilkNameBuf));
}
PRIVATE const char *MakeSlowProcName(const char *proc)
{
     sprintf(CilkNameBuf, "%s%s_slow", CilkPrefix, proc);
     return (UniqueString(CilkNameBuf));
}
PRIVATE const char *MakeFastProcName(const char *proc)
{
/*     sprintf(CilkNameBuf, "%s%s_fast", CilkPrefix, proc);
     return (UniqueString(CilkNameBuf));
*/
	return proc;
}
const char *MakeExportProcName(const char *proc)
{
     sprintf(CilkNameBuf, "mt_%s", proc);
     return (UniqueString(CilkNameBuf));
}

const char *MakeImportProcName(const char *proc)
{
     sprintf(CilkNameBuf, "%s%s_import", CilkPrefix, proc);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeScopeName(int n)
{
     sprintf(CilkNameBuf, "scope%d", n);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeLinkName(const char *proc)
{
     sprintf(CilkNameBuf, "%s%s_sig", CilkPrefix, proc);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeTempName(void)
{
     static int temp_count = 0;
     sprintf(CilkNameBuf, "%s%s%d", CilkPrefix, "temp", temp_count++);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeSlowInletName(const char *proc, const char *inlet)
{
     sprintf(CilkNameBuf, "%s%s_%s_inlet", CilkPrefix, proc, inlet);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeInnerSlowInletName(const char *proc, const char *inlet)
{
     sprintf(CilkNameBuf, "%s%s_%s_inlet_slow", CilkPrefix, proc, inlet);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeInnerFastInletName(const char *proc, const char *inlet)
{
     sprintf(CilkNameBuf, "%s%s_%s_inlet_fast", CilkPrefix, proc, inlet);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeImplicitInletName(const char *proc, int entry)
{
     sprintf(CilkNameBuf, "%s%s_%d_inlet", CilkPrefix, proc, entry);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeInletArgsTypeName(const char *inlet)
{
     sprintf(CilkNameBuf, "%s_args", inlet);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeProcArgsTypeName(const char *proc)
{
     sprintf(CilkNameBuf, "%s%s_args", CilkPrefix, proc);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *MakeRemovedInletRemark(const char *name)
{
     sprintf(CilkNameBuf,
	     "/*------- %s inlet as inner function was removed ------*/",
	     name);
     return (UniqueString(CilkNameBuf));
}

PRIVATE const char *FrameName, *HeaderName, *DummyReturnName;
PRIVATE const char *WorkerStateTypeName, *WorkerStateName;
const char *ContextTypeName, *ContextName;
PRIVATE const char *ExportCilkStartName;
PRIVATE const char *EntryName, *ReceiverName, *SizeName, *FunctionName;
PRIVATE const char *SynchronizeName, *SynchedName;
PRIVATE const char *GenericFrameTypeName;
PRIVATE const char *InitFrameName, *PushFrameName;
PRIVATE const char *XPopFrameNoResultName;
PRIVATE const char *SetNoResultName;
PRIVATE const char *InletArgsName, *InletResultName;
PRIVATE const char *ProcArgsName, *ProcArgsName_v, *ProcResultName;
PRIVATE const char *AbortSlowName, *AbortStandaloneName;
PRIVATE const char *StartThreadFastName, *StartThreadSlowName,
     *BeforeSpawnFastName, *BeforeSpawnSlowName, *AfterSpawnFastName,
     *AfterSpawnSlowName, *AfterSyncSlowName, *BeforeSyncSlowName,
     *AtSyncFastName, *BeforeReturnFastName, *BeforeReturnSlowName,
     *BeforeReturnInletName, *AtThreadBoundarySlowName;
PRIVATE const char *OffsetofName;

/* nondeterminator support */
PRIVATE const char *AccessReadProfileName, *AccessWriteProfileName;
PRIVATE const char *AccessReadProfileName4, *AccessWriteProfileName4;
PRIVATE const char *AccessReadProfileName8, *AccessWriteProfileName8;
PRIVATE const char *AccessReadProfileAddrName, *AccessWriteProfileAddrName;
PRIVATE const char *AccessReadProfileAddrName4, *AccessWriteProfileAddrName4;
PRIVATE const char *AccessReadProfileAddrName8, *AccessWriteProfileAddrName8;
PRIVATE const char *SpawnVarProfileName;

PRIVATE void InitCilkTransform(void)
{
     /* frame names */
     FrameName = ApplyPrefix("frame");

     /* parts-of-frame names */
     HeaderName = UniqueString("header");
     DummyReturnName = UniqueString("dummy_return");

     /* parts of header */
     EntryName = UniqueString("entry");
     ReceiverName = UniqueString("receiver");
     SizeName = UniqueString("size");
     FunctionName = UniqueString("fn");

     /* parts of RTS */
     GenericFrameTypeName = UniqueString("CilkStackFrame");
     WorkerStateTypeName = UniqueString("CilkWorkerState");
     ContextTypeName = UniqueString("CilkContext");

     /* name of worker state */
     WorkerStateName = ApplyPrefix("ws");

     /* name of Conetxt */
     ContextName = UniqueString("context");

     /* name of Cilk_start function - for export functions */
     ExportCilkStartName = UniqueString("Cilk_start");
     
     /* RTS macros */
     InitFrameName = UniqueString("CILK2C_INIT_FRAME");
     PushFrameName = UniqueString("CILK2C_PUSH_FRAME");
     XPopFrameNoResultName = UniqueString("CILK2C_XPOP_FRAME_NORESULT");
     SetNoResultName = UniqueString("CILK2C_SET_NORESULT");
     SynchronizeName = UniqueString("CILK2C_SYNC");
     SynchedName = UniqueString("CILK2C_SYNCHED");
     AbortSlowName = UniqueString("CILK2C_ABORT_SLOW");
     AbortStandaloneName = UniqueString("CILK2C_ABORT_STANDALONE");
     StartThreadFastName = UniqueString("CILK2C_START_THREAD_FAST");
     StartThreadSlowName = UniqueString("CILK2C_START_THREAD_SLOW");
     BeforeSpawnFastName = UniqueString("CILK2C_BEFORE_SPAWN_FAST");
     BeforeSpawnSlowName = UniqueString("CILK2C_BEFORE_SPAWN_SLOW");
     AfterSpawnFastName = UniqueString("CILK2C_AFTER_SPAWN_FAST");
     AfterSpawnSlowName = UniqueString("CILK2C_AFTER_SPAWN_SLOW");
     AtSyncFastName = UniqueString("CILK2C_AT_SYNC_FAST");
     AtThreadBoundarySlowName = UniqueString("CILK2C_AT_THREAD_BOUNDARY_SLOW");
     BeforeSyncSlowName = UniqueString("CILK2C_BEFORE_SYNC_SLOW");
     AfterSyncSlowName = UniqueString("CILK2C_AFTER_SYNC_SLOW");
     BeforeReturnFastName = UniqueString("CILK2C_BEFORE_RETURN_FAST");
     BeforeReturnSlowName = UniqueString("CILK2C_BEFORE_RETURN_SLOW");
     BeforeReturnInletName = UniqueString("CILK2C_BEFORE_RETURN_INLET");
     OffsetofName = UniqueString("CILK_OFFSETOF");

     InletArgsName = ApplyPrefix("inletargs");
     InletResultName = ApplyPrefix("inletresult");

     /* cilk library support */
     ProcArgsName = ApplyPrefix("procargs");
     ProcArgsName_v  = ApplyPrefix("procargs_v");
     ProcResultName = ApplyPrefix("proc_result");

     /* nondeterminator support */
     AccessReadProfileName = "_READ_CHECK_VALUE";
     AccessWriteProfileName = "_WRITE_CHECK_VALUE";
     AccessReadProfileName4 = "_READ_CHECK_VALUE4";
     AccessWriteProfileName4 = "_WRITE_CHECK_VALUE4";
     AccessReadProfileName8 = "_READ_CHECK_VALUE8";
     AccessWriteProfileName8 = "_WRITE_CHECK_VALUE8";
     AccessReadProfileAddrName = "_READ_CHECK";
     AccessWriteProfileAddrName = "_WRITE_CHECK";
     AccessReadProfileAddrName4 = "_READ_CHECK4";
     AccessWriteProfileAddrName4 = "_WRITE_CHECK4";
     AccessReadProfileAddrName8 = "_READ_CHECK8";
     AccessWriteProfileAddrName8 = "_WRITE_CHECK8";
     SpawnVarProfileName = "_SPAWNVAR_CHECK";
}

/* true iff transforming a cilk procedure */
PRIVATE int in_procedure = 0;
PRIVATE int in_inlet = 0;

PRIVATE Node *curr_proc;
PRIVATE const char *proc_name;
PRIVATE Node *curr_inlet;
PRIVATE const char *inlet_name;
PRIVATE int sync_count;
typedef enum {
     Fast,			/* generating fast version of procedure */
     Slow,			/* generating slow version of procedure */
     Link,			/* generating linkage information       */
     InletProc,			/* generating stand-alone inlet         */
     Export,
     Import
} VersionType;
VersionType version;
List *link_initializers;
List *standalone_inlets;

/* for Nondeterminator */
typedef enum {
     Read,
     Write
} AccessProfileType;

#define CHECK_READ(node, type) \
  AccessProfile(node, type, Read, 0, 0, 1, '=')
#define CHECK_READ_ADDR(node, type) \
  AccessProfile(node, type, Read, 1, 0, 1, '=')
#define CHECK_READ_MULT(node, type, multiple) \
  AccessProfile(node, type, Read, 0, 0, multiple, '=')
#define CHECK_READ_OPT(node, type) \
  AccessProfile(node, type, Read, 0, 1, 1, '=')
#define CHECK_READ_OPT_MULT(node, type, multiple) \
  AccessProfile(node, type, Read, 0, 1, multiple, '=')

#define CHECK_WRITE(node, type, operator) \
  AccessProfile(node, type, Write, 0, 0, 1, operator)
#define CHECK_WRITE_ADDR(node, type, operator) \
  AccessProfile(node, type, Write, 1, 0, 1, operator)
#define CHECK_WRITE_MULT(node, type, multiple, operator) \
  AccessProfile(node, type, Write, 0, 0, multiple, operator)
#define CHECK_WRITE_OPT(node, type, operator) \
  AccessProfile(node, type, Write, 0, 1, 1, operator)
#define CHECK_WRITE_OPT_MULT(node, type, multiple, operator) \
  AccessProfile(node, type, Write, 0, 1, multiple, operator)

PRIVATE Node *AccessProfile(Node *node, Node *type, 
			    AccessProfileType accesstype, int withaddress,
			    int opt, int multiple, int operator);
PRIVATE Node *SpawnVarProfile(Node *node, Node *type, OpType assign_op);
PRIVATE void AnalyzeFrameReadWrite(Node *node);

PRIVATE Node *MakeFrameType(const char *procname)
{
     return MakeSdcl(EMPTY_TQ,
		     make_SUE(Sdcl, MakeFrameTypeName(procname), NULL));
}

PRIVATE Node *MakeGenericFrameType(void)
{
     return MakeTdef(EMPTY_TQ, GenericFrameTypeName);
}

PRIVATE Node *MakeWorkerStateType(void)
{
     return MakeTdef(EMPTY_TQ, WorkerStateTypeName);
}

Node *MakeContextType(void)
{
     return MakeTdef(EMPTY_TQ, ContextTypeName);
}

PRIVATE Node *MakeInletArgsType(const char *inletname)
{
     return MakeSdcl(EMPTY_TQ,
		 make_SUE(Sdcl, MakeInletArgsTypeName(inletname), NULL));
}

PRIVATE Node *MakeProcArgsType(const char *procname)
{
     return MakeSdcl(EMPTY_TQ,
		 make_SUE(Sdcl, MakeProcArgsTypeName(procname), NULL));
}

PRIVATE Bool IsInletProc(Node *proc)
{
     Node *decl, *fdcl;
     assert(proc->typ == Proc);
     decl = proc->u.proc.decl;
     assert(decl->typ == Decl);
     fdcl = decl->u.decl.type;
     assert(fdcl->typ == Fdcl);
     return tq_has_inlet(fdcl->u.fdcl.tq);
}

/*
 * Figures out the type (Fdcl) of a spawned function.  Pass the
 * function expression (p->u.spawn.name) to this routine.
 * Also works for inlet decls.
 */
PRIVATE Node *GetSpawnFdcl(Node *spawned_fn)
{
     Node *type = NodeDataType(spawned_fn);

     if (IsPtrToFunction(type))
	  type = type->u.ptr.type;

     assert(type->typ == Fdcl);
     return type;
}

PRIVATE Node *TransformNode(Node *node);

PRIVATE List *TransformList(List *list)
{
     List *aptr;

     for (aptr = list; aptr; aptr = Rest(aptr)) {
	  Node *item = FirstItem(aptr);

	  SetItem(aptr, TransformNode(item));
     }

     return list;
}

PRIVATE inline Node *TransformConst(Node *node, UNUSED(ConstNode *u))
{
     USE_UNUSED(u);
     return node;
}

PRIVATE inline Node *TransformIdOld(Node *node, idNode *u)
{
     const char *scope;

     if (!u->decl)
	  return node;

     /* rename id to decl's name: implements static variable renaming */
     assert(u->decl->typ == Decl);
     u->text = u->decl->u.decl.name;

     /* get scope name of id's declaration */
     scope = u->decl->u.decl.scope;

     switch (version) {
	 case Fast:
	      /* EBA
	       * adding case "kind == 1" to make formal args of outer cilk 
	       * proc to be accessed through "frame" (inside inlets)
	       * also adding (kind == 3) for inlets
	       */
	      if (((u->decl->u.decl.kind == 1) && (in_inlet == 1)) ||
		  ((u->decl->u.decl.kind == 3) && (in_inlet == 1)) ||
		  (u->decl->u.decl.kind == 2) ||
		  (node->memorycheckedp && (u->decl->u.decl.kind == 3))) {
		   Node *result = MakeBinop('.',
					    MakeBinop(ARROW,
						      MakeId(FrameName),
						      MakeId(scope)),
					    MakeId(u->decl->u.decl.name));
		   SetCoords(result, node->coord, Subtree);
		   if (node->memorycheckedp && node->nondeterminator)
		     return CHECK_READ(result, NodeCopy(u->decl->u.decl.type, 
							Subtree));
		   else
		     return (result);
	      }
	      break;
	 case Slow:
	      if (u->decl->u.decl.kind == 2 || u->decl->u.decl.kind == 3) {
		   Node *result = MakeBinop('.',
					    MakeBinop(ARROW,
						      MakeId(FrameName),
						      MakeId(scope)),
					    MakeId(u->decl->u.decl.name));
		   SetCoords(result, node->coord, Subtree);
		   if (node->memorycheckedp && node->nondeterminator)
		     return CHECK_READ(result, NodeCopy(u->decl->u.decl.type, 
							Subtree));
		   else
		     return (result);
	      }
	      break;
	 case Link:
	      return MakeBinop('.',
			       MakeId(scope),
			       MakeId(u->decl->u.decl.name));
	 case InletProc:
	      if (u->decl->u.decl.kind == 4) {
		   Node *result = MakeBinop(ARROW,
					    MakeId(InletArgsName),
					    MakeId(u->decl->u.decl.name));
		   SetCoords(result, node->coord, Subtree);
		   return result;
	      } else if (u->decl->u.decl.kind != 0) {
		   Node *result = MakeBinop('.',
					    MakeBinop(ARROW,
						      MakeId(FrameName),
						      MakeId(scope)),
					    MakeId(u->decl->u.decl.name));
		   SetCoords(result, node->coord, Subtree);
		   return result;
	      }
	      break;
         case Export:
         case Import:
	     FAIL("I don't know what to think of Export or Import");
     }
     if (node->memorycheckedp && node->nondeterminator)
	return CHECK_READ(node, NodeCopy(u->decl->u.decl.type, Subtree));
     else
       return node;
}

PRIVATE inline Node *TransformId(Node *node, idNode *u)
{
     const char *scope;

     if (!u->decl)
	  return node;

     /* rename id to decl's name: implements static variable renaming */
     assert(u->decl->typ == Decl);
     u->text = u->decl->u.decl.name;

     /* get scope name of id's declaration */
     scope = u->decl->u.decl.scope;

     switch (version) {
	 case Fast:
	      /* EBA: Adding case kind=1 and kind=3, in inlets, to make variables accessed through frame. */
	      if ((u->decl->u.decl.kind == 2) ||
		  (in_inlet && (u->decl->u.decl.kind == 1)) ||
		  (in_inlet && (u->decl->u.decl.kind == 3)) ||
		  (node->memorycheckedp && (u->decl->u.decl.kind == 3))) {
		Node *result = ParseWildcardStringNode("%d->%d.%d;",
						   FrameName,
						   scope,
						   u->decl->u.decl.name);
		/*     Node *result = MakeBinop('.', */
/*  					    MakeBinop(ARROW, */
/*  						      MakeId(FrameName), */
/*  						      MakeId(scope)), */
/*  					    MakeId(u->decl->u.decl.name)); */
		   SetCoords(result, node->coord, Subtree);
		   if (node->memorycheckedp && node->nondeterminator)
		     return CHECK_READ(result, NodeCopy(u->decl->u.decl.type, 
							Subtree));
		   else
		     return (result);
	      }
	      break;
	 case Slow:
	      /* EBA: Adding case for kind=3, in inlets, to make variables accessed through frame. */
	      if (u->decl->u.decl.kind == 2 ||
		  u->decl->u.decl.kind == 3 ||
		  (in_inlet && u->decl->u.decl.kind == 1)) {
		Node *result = ParseWildcardStringNode("%d->%d.%d;",
						   FrameName,
						   scope,
						   u->decl->u.decl.name);
	/*  	Node *result = MakeBinop('.', */
/*  					    MakeBinop(ARROW, */
/*  						      MakeId(FrameName), */
/*  						      MakeId(scope)), */
/*  					    MakeId(u->decl->u.decl.name)); */
		   SetCoords(result, node->coord, Subtree);
		   if (node->memorycheckedp && node->nondeterminator)
		     return CHECK_READ(result, NodeCopy(u->decl->u.decl.type, 
							Subtree));
		   else
		     return (result);
	      }
	      break;
	 case Link:
	   return ParseWildcardStringNode("%d.%d;",
					  scope,
					  u->decl->u.decl.name);
	   /*     return MakeBinop('.', */
/*  			       MakeId(scope), */
/*  			       MakeId(u->decl->u.decl.name)); */
	 case InletProc:
	      if (u->decl->u.decl.kind == 4) {
		Node *result = ParseWildcardStringNode("%d->%d;",
						       InletArgsName,
						       u->decl->u.decl.name);
		/*     Node *result = MakeBinop(ARROW, */
/*  					    MakeId(InletArgsName), */
/*  					    MakeId(u->decl->u.decl.name)); */
		   SetCoords(result, node->coord, Subtree);
		   return result;
	      } else if (u->decl->u.decl.kind != 0) {
		Node *result = ParseWildcardStringNode("%d->%d.%d;",
						       FrameName,
						       scope,
						       u->decl->u.decl.name);
		/*     Node *result = MakeBinop('.', */
/*  					    MakeBinop(ARROW, */
/*  						      MakeId(FrameName), */
/*  						      MakeId(scope)), */
/*  					    MakeId(u->decl->u.decl.name)); */
		   SetCoords(result, node->coord, Subtree);
		   return result;
	      }
	      break;
         case Export:
         case Import:
	     FAIL("I don't know what to think of Export or Import");
     }
     if (node->memorycheckedp && node->nondeterminator)
	return CHECK_READ(node, NodeCopy(u->decl->u.decl.type, Subtree));
     else
       return node;
}


PRIVATE inline Node *TransformBinop(Node *node, binopNode *u)
{
     u->left = TransformNode(u->left);

     if (IsAssignmentOp(u->op) && node->memorycheckedp &&
	 node->nondeterminator) {
	  /* This could be written as*/
	  /*  if (PatternEqualsAST(u->left, "*%e1;"))*/
       
       if ((u->left->typ == Unary) && (u->left->u.unary.op == INDIR)) {
	 u->left->u.unary.expr = CHECK_WRITE_ADDR(u->left->u.unary.expr,
						  NodeCopy(u->type, Subtree),
						  u->op);
       }
       else {
	 u->left = CHECK_WRITE_MULT(u->left,
				    NodeCopy(u->type, Subtree),
				    node->nondeterminator,
				    u->op);
       }
     }

     u->right = TransformNode(u->right);

     if (node->memorycheckedp && node->nondeterminator) {
	  /*  Could be written as */
	  /*if (PatternEqualsAST(node, "%e1->%d1;") || PatternEqualsAST(node, "%e1.%d1;"))*/
       if ((u->op == ARROW) || (u->op == '.'))
	 node = CHECK_READ(node, NodeCopy(u->type, Subtree));
       else if (!IsAssignmentOp(u->op))
	 UNREACHABLE;
     }

     return node;
}

PRIVATE inline Node *TransformUnaryOriginal(Node *node, unaryNode *u)
{
     if (version == Link && u->op == INDIR) {
	  /* 
	   * transform *a to a[0], *(a+n) to a[n], *(n+a) to a[n], 
	   * and *&x to x 
	   */
	  switch (u->expr->typ) {
	      case Id:
		   return (MakeArray(TransformNode(u->expr), List1(MakeConstSint(0))));
	      case Binop:
		   if (u->expr->u.binop.op == '+' ||
		       u->expr->u.binop.op == '-') {
			if (NodeIsConstant(u->expr->u.binop.left))
			     return (MakeArray(TransformNode(u->expr->u.binop.right),
					       List1(TransformNode(u->expr->u.binop.left))));
			if (NodeIsConstant(u->expr->u.binop.right))
			     return (MakeArray(TransformNode(u->expr->u.binop.left),
					       List1(TransformNode(u->expr->u.binop.right))));
		   }
		   break;
	      case Unary:
		   if (u->expr->u.unary.op == ADDRESS)
			return (TransformNode(u->expr->u.unary.expr));
		   break;
	      default:
		   break;
	  }
     }
     u->expr = TransformNode(u->expr);

     if (node->memorycheckedp && node->nondeterminator) {
       if (IsIncDecOp(u->op)) {
	 u->expr = CHECK_WRITE(u->expr,NodeCopy(u->type, Subtree), u->op);
       }
       else if (u->op == INDIR)
	 u->expr = CHECK_READ_ADDR(u->expr, NodeCopy(u->type, Subtree));
       else 
	 UNREACHABLE;
     }

     return node;
}


PRIVATE inline Node *TransformUnaryLatest(Node *node, unaryNode *u)
{
     WildcardTable *wcTable = NewWildcardTable(0);
     if (version == Link && PatternMatch(node,
					 ParseWildcardStringNode("%e1;"),
					 wcTable)) {
       /* 
	   * transform *a to a[0], *(a+n) to a[n], *(n+a) to a[n], 
	   * and *&x to x 
	   */
       switch (u->expr->typ) {
       case Id:
	 return ParseWildcardStringNode("%e[0];", TransformNode(u->expr));
	 /*return (MakeArray(TransformNode(u->expr), List1(MakeConstSint(0))));*/
       case Binop:
	 if (PatternMatch(node, ParseWildcardStringNode("*(%e2 + %e3);"), wcTable)) {
	   Node *e2 = LookupWildcard(wcTable, "%e2");
	   Node *e3 = LookupWildcard(wcTable, "%e3");
	   printf("Executing this strange line here. \n");
	   if (NodeIsConstant(e2)) {
	     return ParseWildcardStringNode("%e3[%e2];",
					    TransformNode(e3),
					    TransformNode(e2));
	   }

	   if (NodeIsConstant(e3)) {
	     return ParseWildcardStringNode("%e2[%e3];",
					    TransformNode(e2),
					    TransformNode(e3));
	   }
	 }

	 if (u->expr->u.binop.op == '+' ||
	     u->expr->u.binop.op == '-') {
	   if (NodeIsConstant(u->expr->u.binop.left))
	     return ParseWildcardStringNode("%e[%e];",
					    TransformNode(u->expr->u.binop.right),
					    TransformNode(u->expr->u.binop.left));
								       
	   /*			     return (MakeArray(TransformNode(u->expr->u.binop.right),*/
	   /*		       List1(TransformNode(u->expr->u.binop.left))));*/
	   if (NodeIsConstant(u->expr->u.binop.right))
	     return ParseWildcardStringNode("%e[%e];",
					    TransformNode(u->expr->u.binop.left),
					    TransformNode(u->expr->u.binop.right));
	   /*			     return (MakeArray(TransformNode(u->expr->u.binop.left),*/
	   /*	       List1(TransformNode(u->expr->u.binop.right))));*/
	 }
	 break;
       case Unary:
	 if (u->expr->u.unary.op == ADDRESS)
	   return (TransformNode(u->expr->u.unary.expr));
	 break;
       default:
	 break;
       }
     }
     u->expr = TransformNode(u->expr);

     if (node->memorycheckedp && node->nondeterminator) {
       if (IsIncDecOp(u->op)) {
	 u->expr = CHECK_WRITE(u->expr,NodeCopy(u->type, Subtree), u->op);
       }
       else if (u->op == INDIR)
	 u->expr = CHECK_READ_ADDR(u->expr, NodeCopy(u->type, Subtree));
       else 
	 UNREACHABLE;
     }

     return node;
}


PRIVATE inline Node *TransformUnary(Node *node, unaryNode *u)
{
     if (version == Link && u->op == INDIR) {
	  /* 
	   * transform *a to a[0], *(a+n) to a[n], *(n+a) to a[n], 
	   * and *&x to x 
	   */
	  switch (u->expr->typ) {
	      case Id:
		return ParseWildcardStringNode("%e[0];", TransformNode(u->expr));
		/*return (MakeArray(TransformNode(u->expr), List1(MakeConstSint(0))));*/
	      case Binop:
		   if (u->expr->u.binop.op == '+' ||
		       u->expr->u.binop.op == '-') {
			if (NodeIsConstant(u->expr->u.binop.left))
			  return ParseWildcardStringNode("%e[%e];",
							 TransformNode(u->expr->u.binop.right),
							 TransformNode(u->expr->u.binop.left));
								       
			/*			     return (MakeArray(TransformNode(u->expr->u.binop.right),*/
			/*		       List1(TransformNode(u->expr->u.binop.left))));*/
			if (NodeIsConstant(u->expr->u.binop.right))
			   return ParseWildcardStringNode("%e[%e];",
							 TransformNode(u->expr->u.binop.left),
							 TransformNode(u->expr->u.binop.right));
			/*			     return (MakeArray(TransformNode(u->expr->u.binop.left),*/
			/*	       List1(TransformNode(u->expr->u.binop.right))));*/
		   }
		   break;
	      case Unary:
		   if (u->expr->u.unary.op == ADDRESS)
			return (TransformNode(u->expr->u.unary.expr));
		   break;
	      default:
		   break;
	  }
     }
     u->expr = TransformNode(u->expr);

     if (node->memorycheckedp && node->nondeterminator) {
       if (IsIncDecOp(u->op)) {
	 u->expr = CHECK_WRITE(u->expr,NodeCopy(u->type, Subtree), u->op);
       }
       else if (u->op == INDIR)
	 u->expr = CHECK_READ_ADDR(u->expr, NodeCopy(u->type, Subtree));
       else 
	 UNREACHABLE;
     }

     return node;
}


PRIVATE inline Node *TransformCast(Node *node, castNode *u)
{
     u->type = TransformNode(u->type);
     u->expr = TransformNode(u->expr);

     return node;
}

PRIVATE inline Node *TransformComma(Node *node, commaNode *u)
{
     u->exprs = TransformList(u->exprs);
     return node;
}

PRIVATE inline Node *TransformConstructor(Node *node, constructorNode *u)
{
     u->type  = TransformNode(u->type);
     u->initializerlist = TransformNode(u->initializerlist);
     return node;
}

PRIVATE inline Node *TransformTernary(Node *node, ternaryNode *u)
{
     u->cond = TransformNode(u->cond);
     u->true = TransformNode(u->true);
     u->false = TransformNode(u->false);

     return node;
}

PRIVATE inline Node *TransformArray(Node *node, arrayNode *u)
{
     u->name = TransformNode(u->name);
     u->dims = TransformList(u->dims);

     if (node->memorycheckedp && node->nondeterminator)
       node = CHECK_READ_MULT(node, NodeCopy(u->type, Subtree), 
			      node->nondeterminator);

     return node;
}

PRIVATE Node *name_strip(Node *node);

PRIVATE inline Node *TransformCall(Node *node, callNode *u)
{

     u->name = TransformNode(u->name);
     u->args = TransformList(u->args);

     if ((u->name->typ == Id) &&
	 u->name->u.id.text &&
	 (strcmp(u->name->u.id.text, "_LOCK_CHECK") == 0))
       u->args = List2(FirstItem(u->args),
		       name_strip(NodeCopy(FirstItem(u->args), Subtree)));

     return node;
}

PRIVATE inline Node *TransformInitializer(Node *node, initializerNode *u)
{
     u->exprs = TransformList(u->exprs);
     return node;
}

PRIVATE inline Node *TransformImplicitCast(Node *node, implicitcastNode *u)
{
     u->expr = TransformNode(u->expr);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

PRIVATE inline Node *TransformLabel(Node *node, UNUSED(labelNode *u))
{
     USE_UNUSED(u);
     return node;
}

PRIVATE inline Node *TransformSwitch(Node *node, SwitchNode *u)
{
     u->expr = TransformNode(u->expr);
     u->stmt = TransformNode(u->stmt);

     return node;
}

PRIVATE inline Node *TransformCase(Node *node, UNUSED(CaseNode *u))
{
     return node;
}

PRIVATE inline Node *TransformDefault(Node *node, UNUSED(DefaultNode *u))
{
     return node;
}

PRIVATE inline Node *TransformIf(Node *node, IfNode *u)
{
     u->expr = TransformNode(u->expr);
     u->stmt = TransformNode(u->stmt);
     return node;
}

PRIVATE inline Node *TransformIfElse(Node *node, IfElseNode *u)
{
     u->expr = TransformNode(u->expr);
     u->true = TransformNode(u->true);
     u->false = TransformNode(u->false);
     return node;
}

PRIVATE inline Node *TransformWhile(Node *node, WhileNode *u)
{
     u->expr = TransformNode(u->expr);
     u->stmt = TransformNode(u->stmt);
     return node;
}

PRIVATE inline Node *TransformDo(Node *node, DoNode *u)
{
     u->expr = TransformNode(u->expr);
     u->stmt = TransformNode(u->stmt);
     return node;
}

PRIVATE inline Node *TransformFor(Node *node, ForNode *u)
{
     u->cond = TransformNode(u->cond);
     u->init = TransformNode(u->init);
     u->next = TransformNode(u->next);
     u->stmt = TransformNode(u->stmt);

     if (OptimizeChecking && node->nondeterminator) {
       List *profile;
       List *rvar, *wvar, *rarr, *warr, *list;
       ListMarker marker;
       Node *item;
       
       rvar = (List *) FirstItem(u->rwlist);
       wvar = (List *) FirstItem(Rest(u->rwlist));
       rarr = (List *) FirstItem(Rest(Rest(u->rwlist)));
       warr = (List *) FirstItem(Rest(Rest(Rest(u->rwlist))));
       profile = List1(node);

       IterateList(&marker, rvar);
       while (NextOnList(&marker, (GenericREF) &item)) {
	 profile = ConsItem(
		     CHECK_READ_OPT(item,
				    NodeCopy(item->u.id.decl->u.decl.type,  
					     Subtree)),
		     profile);
       }

       IterateList(&marker, wvar);
       while (NextOnList(&marker, (GenericREF) &item)) {
	 profile = ConsItem(
		     CHECK_WRITE_OPT(item,
				     NodeCopy(item->u.id.decl->u.decl.type,  
					      Subtree),
				     '='),
		     profile);
       }

       IterateList(&marker, rarr);
       while (NextOnList(&marker, (GenericREF) &list)) {
	 item = (Node *) FirstItem(list);
	 profile = ConsItem(
		     CHECK_READ_OPT_MULT(item,
					 NodeCopy(item->u.array.type,  
						  Subtree),
					 ((Node *) FirstItem(Rest(list)))->u.Const.value.i),
		     profile);
       }

       IterateList(&marker, warr);
       while (NextOnList(&marker, (GenericREF) &list)) {
	 item = (Node *) FirstItem(list);
	 profile = ConsItem(
		     CHECK_WRITE_OPT_MULT(item,
					  NodeCopy(item->u.array.type,  
						   Subtree),
					  ((Node *) FirstItem(Rest(list)))->u.Const.value.i,
					  '='),
		     profile);
       }

       return MakeBlock(NULL, NULL, profile);
     }
     else
       return node;
}

PRIVATE inline Node *TransformGoto(Node *node, UNUSED(GotoNode *u))
{
     return node;
}

PRIVATE inline Node *TransformContinue(Node *node, UNUSED(ContinueNode *u))
{
     return node;
}

PRIVATE inline Node *TransformBreak(Node *node, UNUSED(BreakNode *u))
{
     return node;
}

PRIVATE inline Node *TransformReturn(Node *node, ReturnNode *u)
{
     Node *result;
     List *statements = NULL;
     List *decls = NULL;

     if (!in_procedure)
	  return node;

     if (u->needs_sync) {
	  result = MakeBlockCoord(NULL,
				  NULL,
				  List2(MakeSyncCoord(node->coord),
				  MakeReturnCoord(u->expr, node->coord)),
				  node->coord,
				  UnknownCoord);
	  /* copy over analysis results from return to sync node */
	  ((Node *) FirstItem(result->u.Block.stmts))->analysis.livevars = u->livevars;
	  ((Node *) FirstItem(result->u.Block.stmts))->analysis.dirtyvars = u->dirtyvars;
	  return TransformNode(result);
     }
     u->expr = TransformNode(u->expr);

     /* I'm not sure if this condition is right... -KHR */
     if (in_inlet && (version == Fast || version == Slow))
	  return node;
     else if (version == Fast) {
	  /* return x ==> { T _tmp = x; _BEFORE_RETURN_FAST(); return _tmp; } */
	  /* return ==> { _BEFORE_RETURN_FAST(); return; } */
	  /* if u->needs_sync == 1 then no need to create temparary value
	   * as it is already done in AnalyzeReturnExpressions. -fengmd */
	  if (u->expr && (!(u->needs_sync))) {
	       const char *tmp = MakeTempName();
	       Node *returntype = curr_proc->u.proc.decl->u.decl.type->u.fdcl.returns;
               decls = ParseWildcardString("%t %d = %e;",
       					    NodeCopy(returntype, Subtree),
					    tmp,
					    u->expr);
	       
	    /*     Node *decl = MakeDecl(tmp, */
/*  				     EMPTY_TQ, */
/*  				     NodeCopy(returntype, Subtree), */
/*  				     u->expr, */
/*  				     NULL); */
/*  	       decls = List1(decl); */

	       statements = ParseWildcardString("%d(); %e;",
						BeforeReturnFastName,
						MakeReturn(MakeId(tmp)));
	       
/*  	       statements = List2(MakeCall(MakeId(BeforeReturnFastName), NULL), */
/*  				  MakeReturn(MakeId(tmp))); */
	  } else
	    statements = ParseWildcardString("%d(); %e;",
					     BeforeReturnFastName,
					     node);
	 /*   statements = List2(MakeCall(MakeId(BeforeReturnFastName), NULL), */
/*  			     node); */
     } else if (version == Slow) {
	  /* return x  ==>  {_SET_RESULT(x); _SET_VALID; return;} */
	  /* return  ==>  {_SET_VALID; return;} */
	  if (u->expr) {
	    Node *returntype = curr_proc->u.proc.decl->u.decl.type->u.fdcl.returns;
	    statements = ParseWildcardString("{%t __tmp = %e; Cilk_set_result(_cilk_ws, &__tmp, sizeof(__tmp)); } %d();",
					     NodeCopy(returntype, Subtree),
					     u->expr,
					     BeforeReturnSlowName);
	  }
	  
/*  	       statements = List1(MakeCall(MakeId(SetResultName), */
/*  					   List1(u->expr))); */
	  else
	    statements = ParseWildcardString("%d(); %d();",
					     SetNoResultName,
					     BeforeReturnSlowName);
	  /* statements = List1(MakeCall(MakeId(SetNoResultName), NULL));*/

	  /*	  statements = AppendItem(statements,*/
	  /*			   MakeCall(MakeId(BeforeReturnSlowName), NULL));*/
	  statements = AppendItem(statements, MakeReturn(NULL));
     } else if (version == InletProc) {
	  /* return x  ==>  {*_inletresult = x; return;} */
	  /* return  ==>  return */
	  if (!u->expr)
	    return ParseWildcardStringNode("{%d(); %e;}", BeforeReturnInletName, node);
	    /*     return MakeBlock(NULL, NULL, */
/*  				List2(MakeCall(MakeId(BeforeReturnInletName), */
/*  					       NULL), */
/*  				      node)); */
	  statements = ParseWildcardString("*%d = %e; %d(); %e;",
					   InletResultName,
					   u->expr,
					   BeforeReturnInletName,
					   MakeReturn(NULL));
	  
	/*    statements = List3(MakeBinop('=', */
/*  				       MakeUnary('*', */
/*  						 MakeId(InletResultName)), */
/*  				       u->expr), */
/*  			     MakeCall(MakeId(BeforeReturnInletName), NULL), */
/*  			     MakeReturn(NULL)); */
     } else
	  FAIL("bad version in TransformReturn");

     result = MakeBlock(NULL, decls, statements);
     SetCoords(result, node->coord, Subtree);
     return result;
}

#if 0
PRIVATE inline Node *TransformReturnOld(Node *node, ReturnNode *u)
{
     Node *result;
     List *statements = NULL;
     List *decls = NULL;

     if (!in_procedure)
	  return node;

     if (u->needs_sync) {
	  result = MakeBlockCoord(NULL,
				  NULL,
				  List2(MakeSyncCoord(node->coord),
				  MakeReturnCoord(u->expr, node->coord)),
				  node->coord,
				  UnknownCoord);
	  /* copy over analysis results from return to sync node */
	  ((Node *) FirstItem(result->u.Block.stmts))->analysis.livevars = u->livevars;
	  ((Node *) FirstItem(result->u.Block.stmts))->analysis.dirtyvars = u->dirtyvars;
	  return TransformNode(result);
     }
     u->expr = TransformNode(u->expr);

     /* I'm not sure if this condition is right... -KHR */
     if (in_inlet && (version == Fast || version == Slow))
	  return node;
     else if (version == Fast) {
	  /* return x ==> { T _tmp = x; _BEFORE_RETURN_FAST(); return _tmp; } */
	  /* return ==> { _BEFORE_RETURN_FAST(); return; } */
	  /* if u->needs_sync == 1 then no need to create temparary value 
	   * as it is already done in AnalyzeReturnExpressions. -fengmd */
	  if (u->expr && (!(u->needs_sync))) {
	       const char *tmp = MakeTempName();
	       Node *returntype = curr_proc->u.proc.decl->u.decl.type->u.fdcl.returns;
	       Node *decl = MakeDecl(tmp,
				     EMPTY_TQ,
				     NodeCopy(returntype, Subtree),
				     u->expr,
				     NULL);
	       decls = List1(decl);

	       statements = List2(MakeCall(MakeId(BeforeReturnFastName), NULL),
				  MakeReturn(MakeId(tmp)));
	  } else
	       statements = List2(MakeCall(MakeId(BeforeReturnFastName), NULL),
				  node);
     } else if (version == Slow) {
	  /* return x  ==>  {_SET_RESULT(x); _SET_VALID; return;} */
	  /* return  ==>  {_SET_VALID; return;} */
	  if (u->expr)
	       statements = List1(MakeCall(MakeId(SetResultName),
					   List1(u->expr)));
	  else
	       statements = List1(MakeCall(MakeId(SetNoResultName), NULL));

	  statements = AppendItem(statements,
			   MakeCall(MakeId(BeforeReturnSlowName), NULL));
	  statements = AppendItem(statements, MakeReturn(NULL));
     } else if (version == InletProc) {
	  /* return x  ==>  {*_inletresult = x; return;} */
	  /* return  ==>  return */
	  if (!u->expr)
	       return MakeBlock(NULL, NULL,
				List2(MakeCall(MakeId(BeforeReturnInletName),
					       NULL),
				      node));
	  statements = List3(MakeBinop('=',
				       MakeUnary('*',
						 MakeId(InletResultName)),
				       u->expr),
			     MakeCall(MakeId(BeforeReturnInletName), NULL),
			     MakeReturn(NULL));
     } else
	  FAIL("bad version in TransformReturn");

     result = MakeBlock(NULL, decls, statements);
     SetCoords(result, node->coord, Subtree);
     return result;
}
#endif


PRIVATE inline Node *TransformBlock(Node *node, BlockNode *u)
{
     ListMarker decl_marker;
     Node *decl;
     List *new_decl_list = NULL;

     IterateList(&decl_marker, u->decl);
     while (NextOnList(&decl_marker, (GenericREF) & decl)) {
	  decl = TransformNode(decl);
	  SetCurrentOnList(&decl_marker, decl);
	  /* build the generated list after extraction of declarations 
	   * from blocks (see TransformDecl for why a Block can apear in here)
	   */
	  if (decl != NULL) {
	       if (decl->typ == Block) {
		    new_decl_list = JoinLists(new_decl_list, decl->u.Block.decl);
	       } else {
		    new_decl_list = AppendItem(new_decl_list, decl);
	       }
	  }

     }

     u->decl = new_decl_list;
     u->stmts = TransformList(u->stmts);

     return node;
}


PRIVATE inline Node *TransformXBlock(Node *node, XBlockNode *u)
{
     ListMarker decl_marker;
     Node *decl;

     IterateList(&decl_marker, u->decl);
     while (NextOnList(&decl_marker, (GenericREF) & decl)) {
	  decl = TransformNode(decl);
	  SetCurrentOnList(&decl_marker, decl);
     }

     u->stmts = TransformList(u->stmts);

     return node;
}


/*************************************************************************/
/* Determine whether a declaration has the CONST type qualifier          */
/*************************************************************************/
PRIVATE Bool HasConstQual(Node *node)
{
     return (NodeIsConstQual(NodeDataType(node)) ||
	     NodeIsConstQual(NodeDataTypeSuperior(node)));
}

/*************************************************************************/
/*                                                                       */
/*                             Type nodes                                */
/*                                                                       */
/*************************************************************************/

PRIVATE inline Node *TransformPrim(Node *node, UNUSED(primNode *u))
{
     return node;
}

PRIVATE inline Node *TransformTdef(Node *node, UNUSED(tdefNode *u))
{
     return node;
}

PRIVATE inline Node *TransformPtr(Node *node, ptrNode *u)
{
     u->type = TransformNode(u->type);
     return node;
}

PRIVATE inline Node *TransformAdcl(Node *node, adclNode *u)
{
     u->type = TransformNode(u->type);
     u->tq = NodeTq(u->type);
     u->dimp->dim = TransformNode(u->dimp->dim);
     return node;
}

PRIVATE inline Node *TransformFdcl(Node *node, fdclNode *u)
{
     u->args = TransformList(u->args);
     u->returns = TransformNode(u->returns);

     /* 
      * EBA:
      * for an inlet function, add :
      * 1) the frame of the containing Cilk procedure to be the end of the 
      *     arguments list.
      * 2) the cilk worker state of the containing Cilk procedure to the end
      *     of the argument list (last formal argument). this is required for 
      *     the Cilk_die() macro & others.
      */
     if (in_inlet == 1) {
	  Node*  frame;
	  Node*  worker_state;

	  frame = MakeDecl(FrameName,
			   EMPTY_TQ,
			   MakePtr(EMPTY_TQ,
				   MakeFrameType(proc_name)),
			   NULL,
			   NULL);
	  worker_state = MakeDecl(WorkerStateName,
				  TQ_FORMAL_DECL,
				  MakePtr(TQ_CONST, MakeWorkerStateType()),
				  NULL,
				  NULL);
	  AppendItem(u->args, frame);
	  AppendItem(u->args, worker_state);
     }

     if (tq_has_procedure(u->tq)) {
	  List *args;
	  Node *n;
	  args = u->args;
	  n = MakeDecl(WorkerStateName,
		       TQ_FORMAL_DECL,
		       MakePtr(TQ_CONST, MakeWorkerStateType()),
		       NULL,
		       NULL);

	  if (args) {
	       if (IsVoidType(FirstItem(args))) {
		    u->args = List1(n);
	       } else {
	            u->args = ConsItem(n, args);
	       }
	  } else {
	       /* old-style fdecl, don't do anything and hope for the best */
	  }
     }

     return node;
}


#define UnionCheckFields(x) x=x
#define AssignEnumValues(x) x=x

PRIVATE void TransformSUE(SUEtype *sue)
{
     if (sue->transformed == FALSE) {
	  ListMarker marker;
	  Node *field;

	  /* To stop infinite recursion */
	  sue->transformed = TRUE;

	  /* Loop over the fields of the SDCL */
	  IterateList(&marker, sue->fields);
	  while (NextOnList(&marker, (GenericREF) & field)) {
	       assert(field->typ == Decl);
	       field->u.decl.type = TransformNode(field->u.decl.type);
	  }
     }
}


PRIVATE inline Node *TransformSdcl(Node *node, sdclNode *u)
{
     TransformSUE(u->type);
     return node;
}

PRIVATE inline Node *TransformUdcl(Node *node, udclNode *u)
{
     TransformSUE(u->type);
     return node;
}

PRIVATE inline Node *TransformEdcl(Node *node, edclNode *u)
{
     TransformSUE(u->type);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                      Other nodes (decls et al.)                       */
/*                                                                       */
/*************************************************************************/
PRIVATE inline Node *TransformDecl(Node *node, declNode *u)
{
     u->type = TransformNode(u->type);
     u->init = TransformNode(u->init);
     u->bitsize = TransformNode(u->bitsize);

     if (in_procedure && (u->kind == 2 ||
			  ((version == Slow || node->memorycheckedp) && 
			   u->kind == 3))) {
	  if (!u->init)
	       return NULL;
	  {
	       /* We do an incredible hack to get a frame-only variable
	        * with initialization to compile correctly.
	        *     t x = i;
 	        * compiles to (ANSI compliant):
 	        *     int tmp1 = i;
 		*     void* tmp2 = (f->x=tmp1),&tmp2;
		* or
 	        *     int tmp1 = i;
 		*     void* tmp2 = memcpy(&f->x, &tmp1, sizeof(tmp1));
 		* Why is this required ?
 		* Assume you have a variable x of type t (t is a structure)
 		* that is initialized when declared in the Cilk code.
 		* If the variable is designated as frame-only, cilk2c will 
 		* not generate it on the stack - it will replace every
 		* usage of it with the value from the "_frame" structure.
 		* but there are 2 problems:
 		* 1) The initialization of the structure with values
 		*    (e.g.: {1,2,"str",{TRUE, FALSE}, 'u'}) is allowed only
 		*    when the variable is declared - it is not a valid syntax
 		*    as an assignment statement.
 		* 2) Assignment cannot be put in the declarative part of a 
 		*    block. this means that we cannot convert the declaration
 		*    into an assignment - we have to do the assignment in a
 		*    way that declares a new temporary (unused) variable.
 		* so this transformation solves the problem by declaring a
 		* temporary variable & assigning it the initialization at
 		* declaration time & then copying that temporary variable 
 		* into the frame variable using 'memcpy' whos result is the
 		* initialization of another temporary variable. this makes
 		* the memcpy part of a valid variable declaration !!!
 		*
 		* one technical issue is that we now have 2 declarations as
 		* a result of transforming a single declaration. to overcome
 		* this we wrap the 2 declarations in a Block node & return it
 		* as the result of the transformation. the caller will extract
 		* the declarations from the block & get rid of the Block node
 		* by concatenating the 2 declarations into the declarative 
 		* list.
	       */

	       const char *tmp1 = MakeTempName();
	       const char *tmp2 = MakeTempName();
	       Node *newdecl1 = MakeDecl(tmp1, u->tq, u->type, u->init, u->bitsize);
	       Node *newdecl2 ;
	       Node *id = MakeId(u->name);
	       id->u.id.decl = node;

	       if (NodeDataType(node)->typ == Adcl) {
		    newdecl2 = ParseWildcardStringNode("void * %d = memcpy(%e, &%d, sizeof(%d));",
						     tmp2, TransformNode(id), tmp1, tmp1);
	       } else {
		    newdecl2 = ParseWildcardStringNode("void * %d = ((%e = %d),&%d);",
						     tmp2, TransformNode(id), tmp1, tmp2);
	       }
	       /* 
		* a block in which the 2 generated declarations will be 
		* stored for having a single node returned.
		* the caller (which is a block) will identify this & extract 
		* the declarations from the block.  See TransformBlock() above.
		*/
	       return MakeBlockCoord(NULL,
				     List2(newdecl1, newdecl2),
				     NULL,
				     node->coord, UnknownCoord);
	  }
     }
     /* delete static declarations (they have been globalized) -KHR */
     if (in_procedure && tq_has_static(NodeStorageClass(node)))
	  return NULL;

     /*
      * remove const in slow declaration (to allow it to be loaded from 
      * frame) 
      */
     if (in_procedure && version == Slow)
	  NodeUpdateTq(node->u.decl.type, tq_remove_const);

     /*
      * Generate attribute for T_SHARED and T_PRIVATE variables
      */
     if (tq_has_private(NodeDeclTq(node))) {
	  if (GenerateSegments) {
	       if (!HasConstQual(node))
		    u->attribs =
			 ConsItem(
			      MakeAttrib(
				   UniqueString("CILK_ATTRIBUTE_PRIVATE_VAR"),
				   NULL),
			      u->attribs);
	  } else
	       WarningCoord(2, node->coord,
		   "`private' qualifier not supported on this target\n");
     } else if (tq_has_shared(NodeDeclTq(node)) ||
		(GenerateSegments &&
		 tq_has_top_decl(NodeDeclLocation(node)) &&
		 !tq_has_extern(NodeStorageClass(node)) &&
		 !tq_has_register(NodeStorageClass(node)) &&
		 !tq_has_typedef(NodeStorageClass(node)) &&
		 NodeDataType(node)->typ != Fdcl)) {
	  if (tq_has_shared(NodeDeclTq(node)) && !GenerateSegments) {
#if 0	       
	       WarningCoord(2, node->coord,
		    "`shared' qualifier not supported on this target\n");
#endif
	  } else if (!HasConstQual(node))
	       u->attribs = 
		    ConsItem(
			 MakeAttrib(UniqueString("CILK_ATTRIBUTE_SHARED_VAR"),
				    NULL),
			 u->attribs);
     }

     return node;
}

#if 0
/* this is no longer valid code, and it is drifting quickly. */
***OBSOLETE*** PRIVATE inline Node *TransformDeclOld(Node *node, declNode *u)
***OBSOLETE*** {
***OBSOLETE***      u->type = TransformNode(u->type);
***OBSOLETE***      u->init = TransformNode(u->init);
***OBSOLETE***      u->bitsize = TransformNode(u->bitsize);
***OBSOLETE*** 
***OBSOLETE***      if (in_procedure && (u->kind == 2 ||
***OBSOLETE*** 			  ((version == Slow || node->memorycheckedp) && 
***OBSOLETE*** 			   u->kind == 3))) {
***OBSOLETE*** 	  if (!u->init)
***OBSOLETE*** 	       return NULL;
***OBSOLETE***	  {
***OBSOLETE***	       /* We do an incredible hack to get a frame-only variable
***OBSOLETE***		* with initialization to compile correctly.
***OBSOLETE***	        *     t x = i;
***OBSOLETE***	        * compiles to:
***OBSOLETE***	        *     int tmp1 = ({t tmp2 = i; f->x = tmp2; tmp1;});
***OBSOLETE***	        */
***OBSOLETE***	       const char *tmp1 = MakeTempName();
***OBSOLETE***	       const char *tmp2 = MakeTempName();
***OBSOLETE***	       Node *newinit, *newdecl, *assign;
***OBSOLETE***	       Node *id = MakeId(u->name);
***OBSOLETE***	       id->u.id.decl = node;
***OBSOLETE***
***OBSOLETE***	       if (NodeDataType(node)->typ == Adcl)
***OBSOLETE***		    assign = MakeCall(MakeId(UniqueString("memcpy")),
***OBSOLETE***				      List3(TransformNode(id),
***OBSOLETE***					    MakeId(tmp2),
***OBSOLETE***					    MakeUnary(SIZEOF,
***OBSOLETE***						      MakeId(tmp2))));
***OBSOLETE***	       else
***OBSOLETE***		    assign = MakeBinop('=', TransformNode(id), MakeId(tmp2));
***OBSOLETE***
***OBSOLETE***	       newinit = MakeBlock(NULL,
***OBSOLETE***				   List1(MakeDecl(tmp2,
***OBSOLETE***						  u->tq,
***OBSOLETE***						  u->type,
***OBSOLETE***						  u->init,
***OBSOLETE***						  u->bitsize)),
***OBSOLETE***				   List2(assign,
***OBSOLETE***					 MakeId(tmp1)));
***OBSOLETE***	       newdecl = MakeDecl(tmp1,
***OBSOLETE***				  EMPTY_TQ,
***OBSOLETE***				  MakePrim(EMPTY_TQ, Sint),
***OBSOLETE***				  newinit,
***OBSOLETE***				  NULL);
***OBSOLETE***	       SetCoords(newdecl, node->coord, Subtree);
***OBSOLETE***	       return newdecl;
***OBSOLETE***	  }
***OBSOLETE***     }
***OBSOLETE***     /* delete static declarations (they have been globalized) -KHR */
***OBSOLETE***     if (in_procedure && NodeStorageClass(node) == T_STATIC)
***OBSOLETE***	  return NULL;
***OBSOLETE***
***OBSOLETE***     /*
***OBSOLETE***      * remove const in slow declaration (to allow it to be loaded from 
***OBSOLETE***      * frame) 
***OBSOLETE***      */
***OBSOLETE***     if (in_procedure && version == Slow)
***OBSOLETE***	  NodeRemoveTq(node->u.decl.type, T_CONST);
***OBSOLETE***
***OBSOLETE***     /*
***OBSOLETE***      * Generate attribute for T_SHARED and T_PRIVATE variables
***OBSOLETE***      */
***OBSOLETE***     if (NodeDeclQuals(node) & T_PRIVATE) {
***OBSOLETE***	  if (GenerateSegments) {
***OBSOLETE***	       if (!HasConstQual(node))
***OBSOLETE***		    u->attribs =
***OBSOLETE***			 ConsItem(
***OBSOLETE***			      MakeAttrib(
***OBSOLETE***				   UniqueString("CILK_ATTRIBUTE_PRIVATE_VAR"),
***OBSOLETE***				   NULL),
***OBSOLETE***			      u->attribs);
***OBSOLETE***	  } else
***OBSOLETE***	       WarningCoord(2, node->coord,
***OBSOLETE***		   "`private' qualifier not supported on this target\n");
***OBSOLETE***     } else if (NodeDeclQuals(node) & T_SHARED ||
***OBSOLETE***		(GenerateSegments &&
***OBSOLETE***		 NodeDeclLocation(node) == T_TOP_DECL &&
***OBSOLETE***		 NodeStorageClass(node) != T_EXTERN &&
***OBSOLETE***		 NodeStorageClass(node) != T_REGISTER &&
***OBSOLETE***		 NodeStorageClass(node) != T_TYPEDEF &&
***OBSOLETE***		 NodeDataType(node)->typ != Fdcl)) {
***OBSOLETE***	  if ((NodeDeclQuals(node) & T_SHARED) && !GenerateSegments) {
***OBSOLETE***#if 0	       
***OBSOLETE***	       WarningCoord(2, node->coord,
***OBSOLETE***		    "`shared' qualifier not supported on this target\n");
***OBSOLETE***#endif
***OBSOLETE***	  } else if (!HasConstQual(node))
***OBSOLETE***	       u->attribs = 
***OBSOLETE***		    ConsItem(
***OBSOLETE***			 MakeAttrib(UniqueString("CILK_ATTRIBUTE_SHARED_VAR"),
***OBSOLETE***				    NULL),
***OBSOLETE***			 u->attribs);
***OBSOLETE***     }
***OBSOLETE***
***OBSOLETE***     return node;
***OBSOLETE***}
#endif

PRIVATE inline Node *TransformAttrib(Node *node, UNUSED(attribNode *u))
{
     USE_UNUSED(u);
     return node;
}


void MakeInletProc(Node *node)
{
     Node *decl = node->u.proc.decl;
     const char *name = MakeSlowInletName(proc_name, decl->u.decl.name);
     Node *args_decl, *args_type;
     /*     Node *inlet_type;*/
     Node *inlet_decl;
     Node *inlet;

     if (!IsVoidType(FirstItem(decl->u.decl.type->u.fdcl.args))) {
	  args_decl = MakeSdcl(TQ_SUE_ELABORATED,
			       make_SUE(Sdcl,
					MakeInletArgsTypeName(name),
					decl->u.decl.type->u.fdcl.args));
	  args_type = MakeInletArgsType(name);
     } else {
	  args_decl = NULL;
	  args_type = MakePrim(EMPTY_TQ, Void);
     }


					  
/*       inlet_type = MakeFdcl(EMPTY_TQ, */
/*  			   List4(MakeDecl(WorkerStateName, */
/*  					  T_FORMAL_DECL, */
/*  					  MakePtr(T_CONST, */
/*  						  MakeWorkerStateType()), */
/*  					  NULL, */
/*  					  NULL), */
/*  				 MakeDecl(FrameName, */
/*  					  EMPTY_TQ, */
/*  					  MakePtr(EMPTY_TQ, */
/*  					       MakeFrameType(proc_name)), */
/*  					  NULL, */
/*  					  NULL), */
/*  				 MakeDecl(InletArgsName, */
/*  					  EMPTY_TQ, */
/*  					  MakePtr(EMPTY_TQ, */
/*  						  args_type), */
/*  					  NULL, */
/*  					  NULL), */
/*  				 MakeDecl(InletResultName, */
/*  					  EMPTY_TQ, */
/*  					  MakePtr(EMPTY_TQ, */
/*  						  NodeCopy(decl->u.decl.type->u.fdcl.returns, */
/*  							   Subtree)), */
/*  					  NULL, */
/*  					  NULL)), */
/*  			   MakePrim(EMPTY_TQ, Void)); */

/*       inlet_decl = MakeDecl(name, */
/*  			   T_STATIC | T_SLOW_INLET, */
/*  			   inlet_type, */
/*  			   NULL, */
/*  			   NULL); */

     inlet_decl = ParseWildcardStringNode("static void %d(%t %d, %t *%d, %t *%d, %t *%d);",
				     name,
				     MakePtr(TQ_CONST, MakeWorkerStateType()), WorkerStateName,
				     MakeFrameType(proc_name), FrameName,
				     args_type, InletArgsName,
				     NodeCopy(decl->u.decl.type->u.fdcl.returns, Subtree), InletResultName);
     NodeUpdateTq(inlet_decl, tq_add_slow_inlet);
					  
     if (args_decl)
	  SetCoords(args_decl, node->coord, Subtree);
          SetCoords(inlet_decl, node->coord, Subtree);
     in_inlet = 1;
     version = InletProc;
     assert(node->u.proc.body->typ==Block);
     node->u.proc.body->u.Block.stmts = JoinLists(ParseWildcardString("(void)%d;(void)%d;(void)%d;", WorkerStateName, FrameName, InletResultName),
						  node->u.proc.body->u.Block.stmts);
     inlet = MakeProc(inlet_decl,
    		      TransformNode(node->u.proc.body));
     version = Slow;
     in_inlet = 0;
     standalone_inlets = AppendItem(standalone_inlets, args_decl);
     standalone_inlets = AppendItem(standalone_inlets, inlet);
}


void MakeInletProcOld(Node *node)
{
     Node *decl = node->u.proc.decl;
     const char *name = MakeSlowInletName(proc_name, decl->u.decl.name);
     Node *args_decl, *args_type;
     Node *inlet_type;
     Node *inlet_decl;
     Node *inlet;

     if (!IsVoidType(FirstItem(decl->u.decl.type->u.fdcl.args))) {
	  args_decl = MakeSdcl(TQ_SUE_ELABORATED,
			       make_SUE(Sdcl,
					MakeInletArgsTypeName(name),
					decl->u.decl.type->u.fdcl.args));
	  args_type = MakeInletArgsType(name);
     } else {
	  args_decl = NULL;
	  args_type = MakePrim(EMPTY_TQ, Void);
     }

     inlet_type = MakeFdcl(EMPTY_TQ,
			   List4(MakeDecl(WorkerStateName,
					  TQ_FORMAL_DECL,
					  MakePtr(TQ_CONST,
						  MakeWorkerStateType()),
					  NULL,
					  NULL),
				 MakeDecl(FrameName,
					  EMPTY_TQ,
					  MakePtr(EMPTY_TQ,
					       MakeFrameType(proc_name)),
					  NULL,
					  NULL),
				 MakeDecl(InletArgsName,
					  EMPTY_TQ,
					  MakePtr(EMPTY_TQ,
						  args_type),
					  NULL,
					  NULL),
				 MakeDecl(InletResultName,
					  EMPTY_TQ,
					  MakePtr(EMPTY_TQ,
			      NodeCopy(decl->u.decl.type->u.fdcl.returns,
				       Subtree)),
					  NULL,
					  NULL)),
			   MakePrim(EMPTY_TQ, Void));

     inlet_decl = MakeDecl(name,
			   tq_add_static(TQ_SLOW_INLET),
			   inlet_type,
			   NULL,
			   NULL);
     if (args_decl)
	  SetCoords(args_decl, node->coord, Subtree);
     SetCoords(inlet_decl, node->coord, Subtree);

     
     /*     printf("The node here...: \n");*/
     /*     PrintNode(stdout, inlet_decl, 3);*/

     
     in_inlet = 1;
     version = InletProc;
     inlet = MakeProc(inlet_decl,
		      TransformNode(node->u.proc.body));
     version = Slow;
     in_inlet = 0;
     standalone_inlets = AppendItem(standalone_inlets, args_decl);
     standalone_inlets = AppendItem(standalone_inlets, inlet);
}

PRIVATE void MakeImplicitInletProc(Node *node)
{
     const char *name = MakeImplicitInletName(proc_name, sync_count + 1);	/* +1 because inlet is made before entry number is allocated */
     Node *inlet_type, *inlet_decl, *stmt, *inlet;
     Node *rhs_type, *lhs_type;

     assert(node->typ == Spawn);
     assert(node->u.spawn.assign_op != '=');
     /*
      * Generate inlet code for implicit inlet.  It should look
      * something like this:
      * void _foo_inlet(_foo_frame *_frame, int *arg, int *result)
      * { *result += *arg; }
      * Note: frame pointer is not used.
      */
     rhs_type = NodeCopy(GetSpawnFdcl(node->u.spawn.name)->u.fdcl.returns, Subtree);
     lhs_type = NodeCopy(NodeDataType(node->u.spawn.receiver), Subtree);

     inlet_type = MakeFdcl(EMPTY_TQ,
			   List4(MakeDecl(WorkerStateName,
					  TQ_FORMAL_DECL,
					  MakePtr(TQ_CONST,
						  MakeWorkerStateType()),
					  NULL,
					  NULL),
				 MakeDecl(FrameName,
					  EMPTY_TQ,
					  MakePtr(EMPTY_TQ,
					       MakeFrameType(proc_name)),
					  NULL,
					  NULL),
				 MakeDecl(InletArgsName,
					  EMPTY_TQ,
					  MakePtr(EMPTY_TQ,
						  rhs_type),
					  NULL,
					  NULL),
				 MakeDecl(InletResultName,
					  EMPTY_TQ,
					  MakePtr(EMPTY_TQ,
						  lhs_type),
					  NULL,
					  NULL)),
			   MakePrim(EMPTY_TQ, Void));

     inlet_decl = MakeDecl(name,
			   tq_add_static(TQ_SLOW_INLET),
			   inlet_type,
			   NULL,
			   NULL);
     stmt = MakeBinop(node->u.spawn.assign_op,
		      MakeUnary('*', MakeId(InletResultName)),
		      MakeUnary('*', MakeId(InletArgsName)));
     inlet = MakeProc(inlet_decl,
		      MakeBlock(NULL,
				NULL,
				AppendItem(ParseWildcardString("(void)%d;(void)%d;(void)%d;", WorkerStateName, FrameName, InletResultName),
					   stmt)));

     SetCoords(inlet, UnknownCoord, Subtree);
     standalone_inlets = AppendItem(standalone_inlets, inlet);
}

PRIVATE inline Node *TransformProc(Node *node, procNode *u)
{
     Node *standalone_copy = NULL;

     if (IsInletProc(node)) {
	  assert(in_procedure && !in_inlet);
	  if (version == Slow)
	       standalone_copy = NodeCopy(node, Subtree);
	  in_inlet = 1;
	  curr_inlet = node;
	  inlet_name = UniqueString(u->decl->u.decl.name);
     }
     else {
          /* indicate this starts C code for CILK_WHERE_AM_I */
	  NodeUpdateTq(u->decl, tq_add_c_code);
	  curr_inlet = NULL;
	  inlet_name = NULL;
     }
     
     u->decl = TransformNode(u->decl);
     u->body = TransformNode(u->body);

     if (IsInletProc(node)) {
	  in_inlet = 0;

          /* Change the type qualifier into T_SLOW_INLET for CILK_WHERE_AM_I */
          if (version == Fast)
	       NodeUpdateTq(u->decl, tq_add_fast_inlet);
          if (version == Slow)
	       NodeUpdateTq(u->decl, tq_add_slow_inlet);

	  /* make standalone inlet */
	  if (version == Slow)
	       MakeInletProc(standalone_copy);
     }
     return node;
}

PRIVATE inline Node *TransformText(Node *node, UNUSED(textNode *u))
{
     USE_UNUSED(u);
     return node;
}

PRIVATE List *SaveVariables(List *vars, Node *except)
{
     ListMarker m;
     Node *var;
     List *statements = NULL;

     IterateList(&m, vars);
     while (NextOnList(&m, (GenericREF) & var)) {
	  if (var != except &&
	      (var->u.decl.kind == 1 ||
	       (version == Fast && (!(var->memorycheckedp)) &&
		var->u.decl.kind == 3))) {
	       statements = AppendItem(statements,
				       MakeBinop('=',
						 MakeBinop('.',
							 MakeBinop(ARROW,
						       MakeId(FrameName),
					      MakeId(var->u.decl.scope)),
					       MakeId(var->u.decl.name)),
					      MakeId(var->u.decl.name)));
	  }
     }
     return (statements);
}

/* if a is an empty list, then return a list with a NULL in it.*/
/* otherwise return a.*/
PRIVATE List *NonEmptyList (List *a) {
     if (a==0) return List1(0);
     else return a;
}

PRIVATE List *RestoreVariables(List *vars)
{
     ListMarker m;
     Node *var;
     List *statements = NULL;

     IterateList(&m, vars);
     while (NextOnList(&m, (GenericREF) & var)) {
	  if ((var->u.decl.kind == 1 ||
	       (version == Fast && (!(var->memorycheckedp)) &&
		var->u.decl.kind == 3))) {
	       statements = AppendItem(statements,
				       MakeBinop('=',
						 MakeId(var->u.decl.name),
						 MakeBinop('.',
							 MakeBinop(ARROW,
						       MakeId(FrameName),
					      MakeId(var->u.decl.scope)),
					     MakeId(var->u.decl.name))));
	  }
     }
     return (statements);
}

/*
 * This function generates one of two constructions
 *
 * {typeof(__tmp); _XPOP_FRAME_RESULT(frame, return_value, stuff for slow, result);}
 * _XPOP_FRAME_NORESULT(frame, return_value, stuff for slow)
 *
 * depending on the presence of a receiver for the result.
 *
 * The _tmp variable is defined here to avoid actually emititng a non-ansi "typeof" construct.
 */

PRIVATE inline Node *MakeXPopFrame(UNUSED(Node *node), spawnNode *u)
{
     Node *frame;
     Node *return_value;

     /* return type of the procedure being compiled, not the procedure *
      * being spawned! */
     Node *returntype = curr_proc->u.proc.decl->u.decl.type->u.fdcl.returns;

     USE_UNUSED(node);
     frame = MakeId(FrameName);

     if (version == Slow)
	  return_value = MakeText(UniqueString("/* return nothing */"),
				  FALSE);
     else if (IsScalarType(returntype))
	  return_value = MakeConstSint(0);
     else if (IsVoidType(returntype))
	  return_value = MakeText(UniqueString("/* return nothing */"),
				  FALSE);
     else
	  return_value = MakeBinop(ARROW,
				   MakeId(FrameName),
				   MakeId(DummyReturnName));

     if (u->receiver) {
	  return ParseWildcardStringNode("{ %t __tmp; CILK2C_XPOP_FRAME_RESULT(%e, %e, %e); }",
					 NodeCopy(NodeDataType(u->receiver), Subtree),
					 frame,
					 return_value,
					 u->receiver);
     }
     else
	  return MakeCall(MakeId(XPopFrameNoResultName),
			  List2(frame,
				return_value));
}

PRIVATE Node *MakeSetReceiver(Node *receiver_addr)
{
     /* sets index of current frame to be the index of receiver in the *
      * frame. */
     /* _frame->_header.receiver = (void*) receiver_addr */

     return MakeBinop('=',
		      MakeBinop('.',
				MakeBinop(ARROW,
					  MakeId(FrameName),
					  MakeId(HeaderName)),
				MakeId(ReceiverName)),
		      MakeCast(MakePtr(EMPTY_TQ, MakePrim(EMPTY_TQ, Void)),
			       receiver_addr));
}

PRIVATE Bool SimplePointer(Node *receiver);

/* returns TRUE if receiver's address in the frame can be calculated
 * statically, and if receiver is side-effect-free.  Returns FALSE otherwise.
 */
PRIVATE Bool SimpleReceiver(Node *receiver)
{
     List *dims;

     switch (receiver->typ) {
	 case Id:
	      if (receiver->u.id.decl->u.decl.kind != 0)
		   return TRUE;
	      break;
	 case Binop:
	      if (receiver->u.binop.op == '.')
		   return SimpleReceiver(receiver->u.binop.left);
	      break;
	 case Unary:
	      if (receiver->u.unary.op == INDIR)
		   return SimplePointer(receiver->u.unary.expr);
	      break;
	 case Array:
     	      /* First check if array is stored in the frame */
	      if (receiver->u.id.decl->u.decl.kind != 0) {
     	           /* simple if all array dimensions are constant */
     	           for (dims = receiver->u.array.dims; dims; dims = Rest(dims))
     		        if (!NodeIsConstant(FirstItem(dims)))
     			     return FALSE;
     	           return SimplePointer(receiver->u.array.name);
	      }
	      break;
	 default:
	      /* there are probably other cases, but I don't feel like *
	       * dealing * with them now. KHR */
	      break;
     }
     return FALSE;
}
/* returns TRUE if pointer's offset in frame can be calculated statically,
 * and if receiver is side-effect-free.  Returns FALSE otherwise.
 */
PRIVATE Bool SimplePointer(Node *pointer)
{
     switch (pointer->typ) {
	 case Id:
	      if (pointer->u.id.decl->u.decl.type->typ == Adcl)
		   return TRUE;
	      break;
	 case Binop:
	      if (pointer->u.binop.op == '+' || pointer->u.binop.op == '-') {
		   if (NodeIsConstant(pointer->u.binop.right) &&
		       SimplePointer(pointer->u.binop.left))
			return TRUE;
		   if (NodeIsConstant(pointer->u.binop.left) &&
		       SimplePointer(pointer->u.binop.right))
			return TRUE;
	      }
	      break;
	 case Unary:
	      if (pointer->u.unary.op == ADDRESS)
		   return SimpleReceiver(pointer->u.unary.expr);
	      break;
	 case Cast:
	      return SimplePointer(pointer->u.cast.expr);
	 case ImplicitCast:
	      return SimplePointer(pointer->u.implicitcast.expr);
	 default:
	      break;
     }
     return FALSE;
}

/* gcc asm extensions */

PRIVATE inline Node *TransformAsm(Node *node, asmNode *u)
{
     u->template = TransformNode(u->template);
     u->output = TransformList(u->output);
     u->input = TransformList(u->input);
     u->clobbered = TransformList(u->clobbered);
     return node;
}
PRIVATE inline Node *TransformAsmArg(Node *node, asmargNode *u)
{
     u->constraint = TransformNode(u->constraint);
     u->expr = TransformNode(u->expr);
     return node;
}

/* gcc stdarg.h support */
PRIVATE inline Node *TransformBuiltinVaArg(Node *node, builtinvaargNode *u)
{
     u->expr = TransformNode(u->expr);
     u->type = TransformNode(u->type);
     return node;
}

PRIVATE Bool side_effect_free;

PRIVATE void CheckSideEffectFree(Node *node)
{
     switch (node->typ) {
	 case Binop:
	      if (IsAssignmentOp(node->u.binop.op)) {
		   side_effect_free = FALSE;
		   return;
	      }
	      break;
	 case Unary:
	      if (IsIncDecOp(node->u.unary.op)) {
		   side_effect_free = FALSE;
		   return;
	      }
	      break;
	 case Block:
	 case Call:
	 case Spawn:
	      side_effect_free = FALSE;
	      return;
	 case Label:
	 case Switch:
	 case Case:
	 case Default:
	 case If:
	 case IfElse:
	 case While:
	 case Do:
	 case For:
	 case Goto:
	 case Continue:
	 case Break:
	 case Return:
	 case Sync:
	      FAIL("Statement in spawn arg");
	      break;
	 case Prim:
	 case Tdef:
	 case Adcl:
	 case Fdcl:
	 case Sdcl:
	 case Udcl:
	 case Edcl:
	 case Decl:
	 case Attrib:
	 case Proc:
	      /* don't need to touch types */
	      return;
	 default:
	      break;
     }

#define CHILD(n)   CheckSideEffectFree(n)
     ASTWALK(node, CHILD);
#undef CHILD
}

PRIVATE Bool SideEffectFree(Node *node)
{
     side_effect_free = TRUE;
     CheckSideEffectFree(node);
     return side_effect_free;
}

/* returns a decl of a temporary variable with type <type> */
PRIVATE Node *MakeTempDecl(Node *type)
{
     const char *temp_arg = MakeTempName();
     return MakeDecl(temp_arg,
		     EMPTY_TQ,
		     NodeCopy(type, Subtree),
		     NULL,
		     NULL);
}

PRIVATE inline List *TransformSpawnArgs(List *args,
					List **declarations,
					List **statements)
{
     List *side_effect_free_args;
     Node *arg, *temp_arg, *temp_decl;
     ListMarker marker;

     side_effect_free_args = NULL;
     IterateList(&marker, args);
     while (NextOnList(&marker, (GenericREF) & arg)) {
	  if (!SideEffectFree(arg)) {
	       temp_decl = MakeTempDecl(NodeDataType(arg));
	       temp_arg = MakeId(temp_decl->u.decl.name);

	       *declarations = AppendItem(*declarations,
					  temp_decl);
	       *statements = AppendItem(*statements,
					MakeBinop('=',
					  MakeId(temp_decl->u.decl.name),
						  TransformNode(arg)));
	  } else
	       temp_arg = TransformNode(arg);
	  side_effect_free_args = AppendItem(side_effect_free_args, temp_arg);
     }

     return side_effect_free_args;
}

PRIVATE Node *TransformReceiver(Node *receiver,
				OpType assign_op,
				Node *return_type,
				Node **linkage_index,	/* linkage info         */
				Node **assignment,	/* assignment *
							 * statement */
				List **declarations,
				List **statements)
{
     Node *rec, *receiver_tmp, *receiver_addr;
     Node *spawn_result = NULL;
     int receiver_type;		/*
				 * 0 - no receiver
				 * 1 - register variable
				 * 2 - fixed location in frame
				 *       (SimpleReceiver)
				 * 3 - unknown location
				 *  or side-effecting receiver */
     /* figure out receiver type */
     if (!receiver)
	  receiver_type = 0;
     else if (receiver->typ == Id &&
	      (receiver->u.id.decl->u.decl.kind == 1 ||
	     (version == Fast && (!(receiver->memorycheckedp)) &&
	      receiver->u.id.decl->u.decl.kind == 3)))
	  receiver_type = 1;
     else if (SimpleReceiver(receiver))
	  receiver_type = 2;
     else
	  receiver_type = 3;

     /* generate linkage_index part of link information - slow version only. */
     if (version == Slow)
	  switch (receiver_type) {
	      case 0:		/* no receiver */
		   *linkage_index = MakeConstSint(0);
		   break;
	      case 1:		/* register variable */
		   *linkage_index = MakeCall(MakeId(OffsetofName),
					     List2(MakeFrameType(proc_name),
						   MakeBinop('.',
							     MakeId(receiver->u.id.decl->u.decl.scope),
							     MakeId(receiver->u.id.decl->u.decl.name))));
		   break;
	      case 2:		/* fixed loc in frame */
		   rec = NodeCopy(receiver, Subtree);
		   version = Link;
		   rec = TransformNode(rec);
		   version = Slow;
		   *linkage_index = MakeCall(MakeId(OffsetofName),
					     List2(MakeFrameType(proc_name),
						   rec));
		   break;
	      case 3:		/* unknown location */
		   *linkage_index = MakeConstSint(-1);
		   break;
	      default:
		   FAIL("bad receiver type");
		   break;
	  }
     /* make <spawn_result>, <assignment> */
     switch (receiver_type) {
	 case 0:		/* no receiver */
	      spawn_result = NULL;
	      *assignment = NULL;
	      break;
	 case 1:		/* register variable */
	      if (assign_op == '=') {
		   spawn_result = receiver;
		   *assignment = NULL;
		   break;
	      }
	      /* otherwise, fall through */
	 case 2:		/* fixed loc in frame */
	      receiver_tmp = MakeTempDecl(NodeCopy(return_type, Subtree));
	      *declarations = AppendItem(*declarations, receiver_tmp);
	      spawn_result = MakeIdFromDecl(receiver_tmp);
	      rec = NodeCopy(receiver, Subtree);
	      rec->nondeterminator = FALSE;
	      *assignment = MakeBinop(assign_op,
				      TransformNode(rec),
				      MakeId(receiver_tmp->u.decl.name));
	      break;
	 case 3:		/* unknown location */
	      receiver_tmp = MakeTempDecl(NodeCopy(return_type, Subtree));
	      *declarations = AppendItem(*declarations, receiver_tmp);
	      receiver_addr = MakeTempDecl(MakePtr(EMPTY_TQ,
						NodeDataType(receiver)));
	      *declarations = AppendItem(*declarations, receiver_addr);
	      rec = NodeCopy(receiver, Subtree);
	      *statements = AppendItem(*statements,
				       MakeBinop('=',
				      MakeId(receiver_addr->u.decl.name),
					 TransformNode(MakeUnary(ADDRESS,
							    rec))));
	      spawn_result = MakeIdFromDecl(receiver_tmp);
	      *assignment = MakeBinop(assign_op,
				      MakeUnary('*',
				     MakeId(receiver_addr->u.decl.name)),
				      MakeId(receiver_tmp->u.decl.name));
	      *statements = AppendItem(*statements,
				       MakeSetReceiver(MakeId(receiver_addr->u.decl.name)));
	      break;
     }
     return spawn_result;
}

PRIVATE void TransformSpawn2(Node *node, spawnNode *u,
			     List **declarations, List **statements)
{
     Node *linkage_index, *assignment;
     Node *worker_state;
     List *nargs;
     Node *return_type = GetSpawnFdcl(u->name)->u.fdcl.returns;

     assert(in_procedure && !in_inlet);

     /* reserve a sync number */
     sync_count++;

     u->receiver = TransformReceiver(u->receiver, u->assign_op, return_type, &linkage_index,
				  &assignment, declarations, statements);
     assert(SideEffectFree(u->name));	/* need this for now -KHR */
     u->name = TransformNode(u->name);
     /* Just change the name */
     u->name->u.decl.name = MakeFastProcName( u->name->u.decl.name );

     u->args = TransformSpawnArgs(u->args, declarations, statements);

     /* add extra argument for worker state */
     worker_state = MakeId(WorkerStateName);
     nargs = ConsItem(worker_state, u->args);

     /* set up link information */
     if (version == Slow && !u->in_inlet) {
	  Node *size, *inlet, *argsize, *argindex;
	  if (u->receiver)
	       size = MakeUnary(SIZEOF, NodeCopy(return_type, Subtree));
	  else
	       size = MakeConstSint(0);

	  if (u->assign_op == '=') {
	       inlet = MakeConstSint(0);
	       argsize = MakeConstSint(0);
	       argindex = MakeConstSint(0);
	  } else {
	       inlet = MakeId(MakeImplicitInletName(proc_name, sync_count));
	       argsize = MakeUnary(SIZEOF, NodeCopy(return_type, Subtree));
	       argindex = MakeConstSint(0);
	  }

	  link_initializers =
	    AppendItem(link_initializers,
		       MakeInitializer(List5(size,
					     linkage_index,
					     inlet,
					     argsize,
					     argindex)));
     }
     /* make entry setting statement */
     *statements = AppendItem(*statements,
			      MakeBinop('=',
					MakeBinop('.',
						  MakeBinop(ARROW,
						       MakeId(FrameName),
						     MakeId(HeaderName)),
						  MakeId(EntryName)),
					MakeConstSint(sync_count)));

     /* save live, dirty variables */
     *statements = JoinLists(*statements,
			  SaveVariables(node->analysis.dirtyvars, NULL));

     /* Insert _BEFORE_SPAWN_[FAST|SLOW] hook */
     *statements =
	 AppendItem(*statements,
		  MakeCall(MakeId(version == Slow ? BeforeSpawnSlowName :
				  BeforeSpawnFastName),
			   NULL)
	 );

     /* push frame */
     *statements = AppendItem(*statements,
			      MakeCall(MakeId(PushFrameName),
				       List1(MakeId(FrameName))));

     /* do call */
     if (u->receiver) {
	  *statements = AppendItem(*statements,
				   MakeBinop('=',
					     u->receiver,
					     MakeCall(u->name,
						      nargs)));
	  if (node->memorycheckedp && (!(u->in_inlet))) {
	    /* inform the Nondeterminator that this is a spawn var */
	    *statements = AppendItem(*statements,
				     SpawnVarProfile(TransformNode(u->receiver),
						     NodeCopy(return_type,
							      Subtree),
						     u->assign_op));
	  }
     }
     else
	  *statements = AppendItem(*statements,
				   MakeCall(u->name,
					    nargs));

     /* Extended pop: pop, check for steal, locking (in some unspecified order) */
     *statements = AppendItem(*statements,
			      MakeXPopFrame(node, u));

     /* Insert _AFTER_SPAWN_[FAST|SLOW] hook */
     *statements =
	 AppendItem(*statements,
		    MakeCall(MakeId(version == Slow ? AfterSpawnSlowName :
				    AfterSpawnFastName),
			     NULL)
	 );

     /* save spawn result somewhere, if necessary */
     if (assignment != NULL)
	  *statements = AppendItem(*statements, assignment);
}

PRIVATE inline Node *TransformSpawn(Node *node, spawnNode *u)
{
     List *declarations = NULL;
     List *statements = NULL;
     Node *result;
     Coord coord = u->receiver ? u->receiver->coord : node->coord;

     /* make standalone implicit inlet */
     if (version == Slow && u->assign_op != '=')
	  MakeImplicitInletProc(node);

     /* do guts of transformation */
     TransformSpawn2(node, u, &declarations, &statements);

     /* delay the call of Cilk_lreturn_spbags after the inlet is called */
     if (MemoryChecking && u->receiver)
       statements = AppendItem(statements,
			       MakeCall(MakeId("_ND_LRETURN_SPBAGS"),
					List1(MakeId(FrameName))));
       
     /* Add entry recovery statements to slow version */
     if (version == Slow) {
	  statements = AppendItem(statements,
				  MakeIf(MakeConstSint(0),
					 MakeBlock(NULL, NULL,
						   ConsItem(MakeLabel(MakeSyncName(sync_count)),
							    NonEmptyList(RestoreVariables(node->analysis.livevars))))));
	  statements = AppendItem(statements,
				  MakeCall(MakeId(AtThreadBoundarySlowName),
						  NULL));
     }
     /* make block of stuff to replace spawn statement */
     result = MakeBlock(NULL, declarations, statements);
     SetCoords(result, coord, Subtree);
     return result;
}

PRIVATE inline Node *TransformSync(Node *node, UNUSED(syncNode *u))
{
     Node *result;
     List *statements;
     List *outerstatements;

     USE_UNUSED(u);
     assert(in_procedure && !in_inlet);

     /* reserve a sync number */
     sync_count++;

     if (version == Fast) {
	  result = MakeCall(MakeId(AtSyncFastName), NULL);
	  SetCoords(result, node->coord, Subtree);
	  return result;
     }
     /* set up dummy link information */
     if (version == Slow) {
	  link_initializers =
	      AppendItem(link_initializers,
			 MakeInitializer(List5(MakeConstSint(0),
					       MakeConstSint(0),
					       MakeConstSint(0),
					       MakeConstSint(0),
					       MakeConstSint(0))));
     }
     /*
      * generate sync code.  Sync code goes something like this:
      * { 
      *     _BEFORE_SYNC_SLOW(); 
      *     _frame->_header.entry = 3;
      *     <save dirty variables> 
      *     if (_SYNC) { 
      *          _POP_FRAME(_frame); 
      *          return; 
      *    _sync3:; 
      *          <restore live variables> 
      *    } 
      *    _AFTER_SYNC_SLOW();
      * } 
      */

     outerstatements = List1(MakeCall(MakeId(BeforeSyncSlowName), NULL));

     /* set entry */
     outerstatements = AppendItem(outerstatements,
			     MakeBinop('=',
				  MakeBinop('.',
					    MakeBinop(ARROW,
						      MakeId(FrameName),
						      MakeId(HeaderName)),
					    MakeId(EntryName)),
				  MakeConstSint(sync_count)));

     /* save live, dirty variables */
     outerstatements = JoinLists(outerstatements,
			  SaveVariables(node->analysis.dirtyvars, NULL));

     /* return */
     statements = List1(MakeReturn(NULL));

     /* sync label */
     statements = AppendItem(statements,
			     MakeLabel(MakeSyncName(sync_count)));

     /* restore variables */
     statements = JoinLists(statements,
			    NonEmptyList(RestoreVariables(node->analysis.livevars)));

     outerstatements = AppendItem(outerstatements,
				  MakeIf(MakeId(SynchronizeName),
				     MakeBlock(NULL, NULL, statements)));

     outerstatements = AppendItem(outerstatements,
			      MakeCall(MakeId(AfterSyncSlowName), NULL));
     outerstatements = AppendItem(outerstatements,
				  MakeCall(MakeId(AtThreadBoundarySlowName),
						  NULL));

     result = MakeBlock(NULL, NULL, outerstatements);

     SetCoords(result, node->coord, Subtree);
     return result;
}

PRIVATE Node *TransformInletCall(Node *node, inletcallNode *u)
{
     List *declarations = NULL;
     List *statements = NULL;
     Node *result;
     Node *spawnarg;
     List *otherargs;
     List *formals, *actuals;
     Node *temp_arg, *temp_decl;
     Node *linkage_index, *assignment;
     Coord coord = node->coord;
     Bool void_spawn;

     assert(in_procedure && !in_inlet);

     /* get spawn argument and any other arguments */
     assert(u->args != NULL);
     spawnarg = FirstItem(u->args);

     assert(spawnarg->typ == Spawn);
     void_spawn = IsVoidType(spawnarg);
     otherargs = Rest(u->args);

     {
	  void *tmp= TransformReceiver(NULL, '=', NodeDataType(spawnarg), &linkage_index,
				       &assignment, &declarations, &statements);
	  assert(tmp==NULL);
     }
     u->name = TransformNode(u->name);
     otherargs = TransformSpawnArgs(otherargs, &declarations, &statements);

     /* set up link information */
     if (version == Slow) {
	  Node *size, *inlet, *argindex, *argsize;
	  const char *scope;
	  const char *name;

	  /* calculate size entry - size of spawn's return value */
	  if (void_spawn)
	       size = MakeConstSint(0);
	  else
	       size = MakeUnary(SIZEOF, NodeDataType(spawnarg));

	  /* make inlet pointer */
	  name = MakeSlowInletName(proc_name, u->name->u.id.decl->u.decl.name);
	  inlet = MakeId(name);

	  /* make argsize and argindex */
	  formals = GetSpawnFdcl(u->name)->u.fdcl.args;
	  if (formals != NULL && !IsVoidType(FirstItem(formals))) {
	       scope = ((Node *) FirstItem(formals))->u.decl.scope;
	       assert(scope != NULL);
	       argsize = MakeUnary(SIZEOF,
				   MakeInletArgsType(name));
	       argindex = MakeCall(MakeId(OffsetofName),
				   List2(MakeFrameType(proc_name),
					 MakeId(scope)));
	  } else {
	       argsize = MakeConstSint(0);
	       argindex = MakeConstSint(0);
	  }
	  link_initializers =
	      AppendItem(link_initializers,
			 MakeInitializer(List5(size,
					       linkage_index,
					       inlet,
					       argsize,
					       argindex)));
     }
     /* save other arguments besides spawn argument to frame */
     actuals = otherargs;
     if (void_spawn)
	  formals = GetSpawnFdcl(u->name)->u.fdcl.args;
     else
	  formals = Rest(GetSpawnFdcl(u->name)->u.fdcl.args);
     for (; actuals != NULL; actuals = Rest(actuals), formals = Rest(formals)) {
	  Node *actual = FirstItem(actuals);
	  Node *formal = FirstItem(formals);
	  formal = MakeBinop('.',
			     MakeBinop(ARROW,
				       MakeId(FrameName),
				       MakeId(formal->u.decl.scope)),
			     MakeId(formal->u.decl.name));
	  statements = AppendItem(statements,
				  MakeBinop('=',
					    formal,
					    actual));
     }

     /* transform spawn argument */
     if (!void_spawn) {
	  temp_decl = MakeTempDecl(NodeDataType(spawnarg));

	  /* treat this variable as a register variable, regardless of type */
	  temp_decl->u.decl.kind = 1;
	  declarations = AppendItem(declarations,
				    temp_decl);

	  temp_arg = MakeId(temp_decl->u.decl.name);
	  temp_arg->u.id.decl = temp_decl;

	  spawnarg->u.spawn.receiver = temp_arg;
	  otherargs = ConsItem(temp_arg, otherargs);
     } else
	  spawnarg->u.spawn.receiver = NULL;

     TransformSpawn2(spawnarg, &spawnarg->u.spawn, &declarations, &statements);

     /* tell the Nondeterminator that we are entering an inlet */
     if (MemoryChecking)
       statements = AppendItem(statements,
			       MakeCall(MakeId("_ND_ENTER_INLET"), 
					List1(MakeCast(
						MakePtr(EMPTY_TQ,
						  MakeGenericFrameType()),
						MakeId(FrameName)))));

     /* EBA: 
      * append "_cilk_frame" & "_cilk_ws" argument to inlet call, as last
      * argument
      */
     AppendItem(otherargs, MakeId(FrameName));
     AppendItem(otherargs, MakeId(WorkerStateName));
     
     /* do call of inlet */
     statements = AppendItem(statements,
			     MakeCall(u->name,
				      otherargs));

     /* EBA:
      * Restore variables from "_cilk_frame" into local variables of procedure.
      */
     statements = AppendItem(statements,
			     MakeText("/* Restore variables from frame */\n", 
				      TRUE));
     statements = AppendItem(statements,
			     MakeBlock(NULL, NULL,
				       NonEmptyList(RestoreVariables(node->analysis.livevars/*dirtyvars*/))));

     /* tell the Nondeterminator that we are leaving an inlet */
     if (MemoryChecking)
       statements = AppendItem(statements,
			       MakeCall(MakeId("_ND_LEAVE_INLET"), NULL));

     /* save inlet result somewhere, if necessary */
     if (assignment != NULL)
	  statements = AppendItem(statements, assignment);

     /* delay the call of Cilk_lreturn_spbags after the inlet is called */
     if (MemoryChecking)
       statements = AppendItem(statements,
			       MakeCall(MakeId("_ND_LRETURN_SPBAGS"),
					List1(MakeId(FrameName))));
       
     /* Add entry recovery statements to slow version */
     if (version == Slow) {
	  statements = AppendItem(statements,
				  MakeIf(MakeConstSint(0),
					 MakeBlock(NULL, NULL,
						   ConsItem(MakeLabel(MakeSyncName(sync_count)),
							    NonEmptyList(RestoreVariables(node->analysis.livevars))))));
	  statements = AppendItem(statements,
				  MakeCall(MakeId(AtThreadBoundarySlowName),
						  NULL));
     }
     /* make block of stuff to replace inlet_call statement */
     result = MakeBlock(NULL, declarations, statements);
     SetCoords(result, coord, Subtree);
     return result;
}


PRIVATE inline Node *TransformAbort(Node *node, UNUSED(abortNode *u))
{
     Node *result;

     USE_UNUSED(u);
     assert(in_procedure);

     switch (version) {
	 case Fast:
	      return MakeTextCoord(UniqueString("/* abort */;"), FALSE, node->coord);
	 case Slow:
	      result = MakeCall(MakeId(AbortSlowName), NULL);
	      SetCoords(result, node->coord, Subtree);
	      return result;
	 case InletProc:
	      result = MakeCall(MakeId(AbortStandaloneName), NULL);
	      SetCoords(result, node->coord, Subtree);
	      return result;
	 default:
	      FAIL("Unknown version");
     }

     return node;
}

PRIVATE inline Node *TransformSynched(Node *node, UNUSED(synchedNode *u))
{
     USE_UNUSED(u);
     assert(in_procedure && !in_inlet);

     switch (version) {
	 case Fast:
	      return MakeConstSint(1);
	 case Slow:
	 case InletProc:
	      return MakeId(SynchedName);
	 default:
	      FAIL("Unknown version");
     }

     return node;
}

/* if implicit sync is needed, calculating return expressions and
 * saving the result into temporary variables before implicit sync.
 * This is done first in TransformCilkProc so that the temporary
 * variables will be included in the procedure frames. -fengmd */
PRIVATE Node *AnalyzeReturnExpressions(Node *node)
{

#define CHILD(n) n = AnalyzeReturnExpressions(n)
     {
	  ASTWALK(node, CHILD)
     }
#undef CHILD

     if ((node->typ == Return) &&
	 (node->u.Return.needs_sync) &&
	 (node->u.Return.expr)) {
	  const char *tmp = MakeTempName();
	  Node *returntype = node->u.Return.proc->u.proc.decl->u.decl.type->u.fdcl.returns;
	  Node *decl = MakeDecl(tmp,
				TQ_BLOCK_DECL,
				NodeCopy(returntype, Subtree),
				node->u.Return.expr,
				NULL);
	  Node *tmp_id = MakeIdCoord(tmp, node->coord);
	  Node *return_node = MakeReturnCoord(tmp_id, node->coord);
	  List *decls = List1(decl);
	  Node *result;

	  tmp_id->u.id.decl = decl;
	  return_node->u.Return.needs_sync = 1;

	  /* set analysis results of return node for later passing to sync node */
	  return_node->u.Return.livevars = decls;
	  return_node->u.Return.dirtyvars = decls;

	  result = MakeBlockCoord(NULL, decls, List1(return_node),
				  node->coord, UnknownCoord);
	  SetCoords(result, node->coord, Subtree);
	  return result;
     } else
	  return node;
}


PRIVATE Node *MakeSlowDecl(const char *name)
{
     Node *arg = MakeDecl(FrameName,
			  TQ_FORMAL_DECL,
			  MakePtr(EMPTY_TQ, MakeFrameType(name)),
			  NULL,
			  NULL);

     Node *ws = MakeDecl(WorkerStateName,
			 TQ_FORMAL_DECL,
			 MakePtr(TQ_CONST, MakeWorkerStateType()),
			 NULL,
			 NULL);

    Node *n =
	  MakeDecl(MakeSlowProcName(name),
		   tq_add_static(tq_add_top_decl(TQ_SLOW_PROCEDURE)),
		   MakeFdcl(EMPTY_TQ, List2(ws, arg),
			    MakePrim(EMPTY_TQ, Void)),
		   NULL,
		   NULL);
     return n;
}

PRIVATE Node *MakeExportDecl(procNode *u)
{
     Node *n;
     List *args;
     Node *context;

     n = NodeCopy(u->decl, Subtree);
     n->u.decl.name = MakeExportProcName(n->u.decl.name);
     n->u.decl.tq = tq_add_top_decl(tq_add_c_code(n->u.decl.tq));
     args = TransformList(n->u.decl.type->u.fdcl.args);
     /*printf("MakeExportDecl -name=%s\n", name);*/
     context = MakeDecl(ContextName,
			 TQ_FORMAL_DECL,
			 MakePtr(TQ_CONST, MakeContextType()),
			 NULL,
			 NULL);

    if (args) {
	if (IsVoidType(FirstItem(args))) {
	    n->u.decl.type->u.fdcl.args = MakeNewList(context);
	       } else {
		    n->u.decl.type->u.fdcl.args = ConsItem(context, args);
	       }
	  } else {
	       /* old-style fdecl, don't do anything and hope for the best */
	  }


     return n;
}

PRIVATE Node *MakeImportDecl(procNode *u)
{
     Node *n;
     List *args;
     Node *returns;
     Node *ws, *arg_struct;

     n = NodeCopy(u->decl, Subtree);
     n->u.decl.name = MakeImportProcName(n->u.decl.name);
     n->u.decl.tq = tq_add_static(tq_add_top_decl(TQ_C_CODE));
     n->u.decl.type->u.fdcl.returns = MakePrim(EMPTY_TQ, Void);
     args = TransformList(n->u.decl.type->u.fdcl.args);
     returns = u->decl->u.decl.type->u.fdcl.returns;

     ws = MakeDecl(WorkerStateName,
		   TQ_FORMAL_DECL,
		   MakePtr(TQ_CONST, MakeWorkerStateType()),
		   NULL,
		   NULL);

     arg_struct = MakeDecl(ProcArgsName_v,
			   TQ_FORMAL_DECL,
			   MakePtr(EMPTY_TQ, NodeCopy(PrimVoid, Subtree)), /*MakeProcArgsType(u->decl->u.decl.name)),*/
			   NULL,
			   NULL);

     /* We still want to have the arg_struct in the fdcl.args list
      * even in the case there the function has no incoming arguments
      * and no return value
     if (IsVoidArglist(args) && returns && IsVoidType(returns)){
	  n->u.decl.type->u.fdcl.args = List1(ws);
     } else {
	  n->u.decl.type->u.fdcl.args = List2(ws, arg_struct);
     }*/
     n->u.decl.type->u.fdcl.args = List2(ws, arg_struct);

     return n;
}

PRIVATE void SetDecl(Node *decl, List *register_vars, List *spawn_vars,
		     int scope_count)
{
     TypeQual sc = NodeStorageClass(decl);
     if (tq_has_extern(sc) || tq_has_static(sc) || tq_has_typedef(sc) ||
	 decl->u.decl.type->typ == Fdcl)
	  return;		/* leave as kind 0 */

     if (scope_count > 0 && tq_has_formal_decl(decl->u.decl.tq)) {
	  /* inlet argument variable */
	  decl->u.decl.kind = 4;
     } else if (FindItem(register_vars, decl)) {
	  /* decl is for a register variable. */
	  if (FindItem(spawn_vars, decl))
	       decl->u.decl.kind = 3;
	  else
	       decl->u.decl.kind = 1;
     } else {
	  /* decl is for a frame-only variable. */
	  decl->u.decl.kind = 2;
     }

     decl->u.decl.scope = MakeScopeName(scope_count);
}

PRIVATE List *FindAllScopes(Node *node, List *declscopes)
{
     if (node->typ == Block && node->u.Block.decl) {
	  declscopes =
	       AppendItem(declscopes, node->u.Block.decl);
     } else if (node->typ == Proc && IsInletProc(node)) {
	  /* add function arguments to scope list */
	  Node *type = node->u.proc.decl->u.decl.type;
	  assert(type->typ == Fdcl);

	  declscopes = AppendItem(declscopes, type->u.fdcl.args);
	  return declscopes;
	  /* return keeps FindAllScopes from looking inside local
	   * procedure */
     }
     /* transform children */
#define CHILD(n)  declscopes = FindAllScopes(n, declscopes)
     ASTWALK(node, CHILD)
#undef CHILD

     return declscopes;
}

PRIVATE List *FindSideEffectedVariables(Node *node, List *sofar)
{
     switch (node->typ) {
	 case Binop:
	      if (IsAssignmentOp(node->u.binop.op) &&
		  node->u.binop.left->typ == Id)
		   sofar = ConsItem(node->u.binop.left->u.id.decl, sofar);
	      break;
	 case Unary:
	      if (IsIncDecOp(node->u.unary.op) &&
		  node->u.unary.expr->typ == Id)
		   sofar = ConsItem(node->u.unary.expr->u.id.decl, sofar);
	      break;
	 default:
	      break;
     }

#define CODE(n)   sofar = FindSideEffectedVariables(n, sofar)
     ASTWALK(node, CODE)
#undef CODE

	 return sofar;
}

PRIVATE List *FindSpawnVariables(Node *node, List *sofar)
{
     if (node->typ == Spawn) {
	  if (node->u.spawn.receiver &&
	      node->u.spawn.receiver->typ == Id)
	       return ConsItem(node->u.spawn.receiver->u.id.decl, sofar);
     } else if (node->typ == InletCall) {
	  ; /*  sofar should just be returned, since inlets don't have spawns or inlets inside them.  But just fall through is OK too. */
     } else if (node->typ == Proc && IsInletProc(node)) {
	  /* need to find all variables modified by the inlet.  These
	   * variables need to be in the frame for the slow version. */
	  return JoinLists(sofar,
			   FindSideEffectedVariables(node, NULL));
     }
     /* transform children */
#define CHILD(n)  sofar = FindSpawnVariables(n, sofar)
     ASTWALK(node, CHILD)
#undef CHILD

	 return sofar;
}

PRIVATE void AnalyzeVariables(Node *node)
{
     ListMarker marker;
     Node *decl;
     int scope_count;

     /* find out which variables can be stored in registers */
     List *register_vars = RegisterVariables(node, NULL);
     List *spawn_vars = FindSpawnVariables(node, NULL);

     /* find all local scopes */
     List *scopes = FindAllScopes(node, NULL);
     scopes = ConsItem(node->u.proc.decl->u.decl.type->u.fdcl.args, scopes);

     /* iterate over all local variables in the procedure */
     scope_count = 0;
     while (scopes) {
	  IterateList(&marker, FirstItem(scopes));
	  while (NextOnList(&marker, (GenericREF) & decl)) {
	       if (decl->typ == Decl)
		    SetDecl(decl, register_vars, spawn_vars, scope_count);
	  }
	  scopes = Rest(scopes);
	  scope_count++;
     }

     /* make frame references as global */
     if (node->memorycheckedp)
       AnalyzeFrameReadWrite(node);
}

PRIVATE Node *ExpandTypedefs(Node *node)
{
     Node *junk;
     Node *new = NodeCopy(node, NodeOnlyExact);

#define CHILD(n) n = ExpandTypedefs(n)
     {
	  ASTWALK(new, CHILD)
     }
#undef CHILD

     /* expand locally declared typedefs */
     if (new->typ == Tdef) {
	  if (LookupSymbol(Identifiers, new->u.tdef.name,
			   (GenericREF) & junk)) {
	       if (tq_has_typedef(NodeStorageClass(junk)) &&
		   junk->u.decl.type == new->u.tdef.type)
		    return new;	/* external declaration is OK */
	  }
	  /*
	   * external declaration is not the same as the local one - must
	   * expand the typedef. 
	   */
	  /* 
	   * tdef.type is not visited when doing an ASTWALK! could we
	   * get into circularity problems?  I don't think so.  -KHR 
	   */
	  new->u.tdef.type = ExpandTypedefs(new->u.tdef.type);

	  /*
	   * is this the right thing to do with tdef's type qualifiers? -
	   *  KHR
	   */
	  NodeUpdateTq2(new->u.tdef.type, tq_union, new->u.tdef.tq);
	  return new->u.tdef.type;
     } else
	  return new;
}


PRIVATE int is_unused_attrib(Generic *item) 
{
     Node *attrib = item;
     assert(attrib && attrib->typ==Attrib);
     if (strcmp(attrib->u.attrib.name, "__unused__")==0) return 1;
     else return 0;
}

PRIVATE void remove_const_register_unused_attribute(Generic *item) 
{
     Node *decl=item;
     assert(decl != NULL && decl->typ == Decl);
     if (tq_has_register(decl->u.decl.tq)) { 
	  decl->u.decl.tq = tq_remove_all_storage_classes(decl->u.decl.tq);
     }
     NodeUpdateTq(decl->u.decl.type, tq_remove_const);
     decl->u.decl.attribs = DeleteIf(decl->u.decl.attribs, is_unused_attrib);
}

PRIVATE Node *MakeFrame(procNode *u)
{
     List *scopes, *scope;
     int scope_count;
     List *fields;
     ListMarker marker1, marker2;
     Node *result;
     Node *type;

     /*
      * make frame:
      *
      * typedef struct foo_frame {
      *   StackFrameT _header;
      *   { ... args ... } _scope0;
      *   { ... scope 1 vars ...} _scope1;
      *   ...
      *   { ... scope <n> vars ...} _scope<n>;
      * }
      *
      * for each different type of inlet, we also have a scope that
      * holds its argument list.
      * { ... args of inlet 1 ... } _scope<x>;
      * { ... args of inlet <n> ... } _scope<y>;
      *
      * The inlet scopes are mixed in with the regular scopes in no
      * particular order.  We could put all of the inlet argument
      * scopes in a oneof because only one is being used at a time.
      */
     fields = List1(MakeDecl(HeaderName,
			     TQ_SU_DECL,
			     MakeGenericFrameType(),
			     NULL,
			     NULL));

     /* if proc return type is not an arithmetic type, put it in frame */
     type = u->decl->u.decl.type->u.fdcl.returns;
     if (!IsVoidType(type) && !IsScalarType(type)) {
	  fields = AppendItem(fields,
			      MakeDecl(DummyReturnName,
				       TQ_SU_DECL,
				       NodeCopy(type, Subtree),
				       NULL,
				       NULL));
     }
     /* add scopes to frame */
     scopes = FindAllScopes(u->body, NULL);
     scopes = ConsItem(u->decl->u.decl.type->u.fdcl.args, scopes);

     scope_count = 0;
     IterateList(&marker1, scopes);
     while (NextOnList(&marker1, (GenericREF) & scope)) {
	  List *scopefields = NULL;
	  Node *decl;

	  IterateList(&marker2, scope);
	  while (NextOnList(&marker2, (GenericREF) & decl)) {
	       if (decl->typ != Decl)
		    continue;
	       if (decl->u.decl.kind == 0)
		    continue;
	       SetCurrentOnList(&marker2, NodeCopy(decl, Subtree));
	       remove_const_register_unused_attribute(decl);
	       decl->u.decl.type = ExpandTypedefs(decl->u.decl.type);
	       decl->u.decl.init = NULL;
	       decl = TransformNode(NodeCopy(decl, Subtree));
	       scopefields = AppendItem(scopefields, decl);
	  }

	  if (scopefields)
	       fields = AppendItem(fields,
				   MakeDecl(MakeScopeName(scope_count),
					    TQ_SU_DECL,
					    MakeSdcl(TQ_SUE_ELABORATED,
						     make_SUE(Sdcl, NULL, scopefields)),
					    NULL,
					    NULL));
	  scope_count++;
     }

     /* finally, make struct declaration for frame */
     result = MakeDecl(NULL,
		       TQ_TOP_DECL,
		       MakeSdcl(TQ_SUE_ELABORATED,
	make_SUE(Sdcl, MakeFrameTypeName(u->decl->u.decl.name), fields)),
		       NULL,
		       NULL);
     SetCoords(result, UnknownCoord, Subtree);
     return result;
}

PRIVATE Node *MakeArgsAndResultStruct(procNode *u) {

     Node *decl, *fdcl;
     Node *args_decl;
     List* fields = NULL;
     Node *returns;

     decl = u->decl;
     assert(decl->typ == Decl);
     fdcl = decl->u.decl.type;
     assert(fdcl->typ == Fdcl);


     if( !IsVoidType(FirstItem(fdcl->u.fdcl.args) ) ) {
	  fields = Mapc(ListCopyNodes(fdcl->u.fdcl.args, Subtree),
			remove_const_register_unused_attribute);
	  fields = Mapc(fields, (void (*)(Generic *))TransformNode);
     }

     /* Append result item to the field list*/
     if( !IsVoidType(fdcl->u.fdcl.returns) ) {
     	returns = NodeCopy(fdcl->u.fdcl.returns, Subtree);

     	fields = ConsItem(  MakeDecl(ProcResultName,
			    	     TQ_FORMAL_DECL,
				     returns,
				     NULL,
				     NULL),
			    fields);
     }

     /* Create the structure */
     if ( fields ) {
	  args_decl = MakeSdcl(TQ_SUE_ELABORATED,
			       make_SUE(Sdcl,
					MakeProcArgsTypeName(decl->u.decl.name),
					fields));
	  /*args_type = MakeProcArgsType(decl->u.decl.name);*/
     } else {
	  args_decl = NULL;
	  /*args_type = MakePrim(EMPTY_TQ, Void);*/
     }

     return args_decl;

}


PRIVATE inline Node *MakeLinkage(UNUSED(Node *node))
{
     Node *linkage;

     USE_UNUSED(node);
     /* make CilkProcInfo declaration */
     linkage = MakeDecl(MakeLinkName(proc_name),
			TQ_STATIC,
			MakeAdcl(EMPTY_TQ,
				 MakeTdef(EMPTY_TQ,
					  UniqueString("CilkProcInfo")),
				 NULL),
			MakeInitializer(link_initializers),
			NULL);
     SetCoords(linkage, UnknownCoord, Subtree);
     return (linkage);
}

/*
 * Add code to allocate frame to beginning of fast body.  For example:
 *    _foo_frame *_frame;
 *    _INIT_FRAME(_frame, sizeof(_foo_frame), _foo_sig);
 *    _START_THREAD_FAST();
 *    (_INIT_FRAME does, among other things,
 *           _frame->_header.size = sizeof(_foo_frame);
 *           _frame->_header.sig = _foo_sig; )
 *
 *    ... transfer non-register arguments to frame ...
 */
PRIVATE Node *AddFastStuff(Node *body, List *formals)
{
     List *stmts;
     ListMarker marker;
     Node *decl;

     stmts = List1(MakeCall(MakeId(InitFrameName),
			    List3(MakeId(FrameName),
				  MakeUnary(SIZEOF,
					    MakeFrameType(proc_name)),
				  MakeId(MakeLinkName(proc_name)))));

     stmts = AppendItem(stmts,
			MakeCall(MakeId(StartThreadFastName), NULL));

     /* move any frame-only formals to frame */
     IterateList(&marker, formals);
     while (NextOnList(&marker, (GenericREF) & decl)) {
	  if (decl->u.decl.kind == 2) {
	       const char *name = decl->u.decl.name;
	       const char *scope = decl->u.decl.scope;

	       /*
	        * we need to do something different for array arguments
	        * -KHR
	        */
	       stmts = AppendItem(stmts,
				  MakeBinop('=',
					    MakeBinop('.',
						      MakeBinop(ARROW,
						       MakeId(FrameName),
							  MakeId(scope)),
						      MakeId(name)),
					    MakeId(name)));
	  }
     }

     /* append code of procedure here */
     stmts = AppendItem(stmts, body);

     return MakeBlock(NULL,
		      List1(MakeDecl(FrameName,
				     TQ_BLOCK_DECL,
				     MakePtr(EMPTY_TQ,
					     MakeFrameType(proc_name)),
				     NULL,
				     NULL)),
		      stmts);
}

/*
 * Add case statement and argument unpacking to beginning of slow body.
 * For example:
 *    int n;       < declaration for each register formal >
 *    _START_THREAD_SLOW();
 *    switch (_frame->_header.entry) {
 *      case 1: goto _sync1;
 *      case 2: goto _sync2;
 *      case 3: goto _sync3;
 *    }
 *    n = _frame->n;   < initialization for each register formal >
 */
Node *AddSlowStuff(Node *body, List *formals)
{
     List *cases = NULL;
     List *regdecls = NULL;
     ListMarker marker;
     Node *decl;

     /* make cases for case statement */
     for (; sync_count; sync_count--) {
	  cases = ConsItem(MakeCase(MakeConstSint(sync_count),
				    NULL),
			   ConsItem(MakeGoto(MakeLabel(MakeSyncName(sync_count))),
				    cases));
     }

     /* get declarations for register formals */
     IterateList(&marker, formals);
     while (NextOnList(&marker, (GenericREF) & decl)) {
	  if (decl->u.decl.kind == 1) {
	       decl = NodeCopy(decl, Subtree);
	       remove_const_register_unused_attribute(decl);
	       regdecls = AppendItem(regdecls, decl);
	  }
     }

     return MakeBlock(NULL,
		      regdecls,
		      ConsItem(
			     MakeCall(MakeId(StartThreadSlowName), NULL),
				   ConsItem(
						MakeSwitch(MakeBinop('.',
							 MakeBinop(ARROW,
						       MakeId(FrameName),
						     MakeId(HeaderName)),
						      MakeId(EntryName)),
					    MakeBlock(NULL, NULL, cases),
							   cases),
				   AppendItem(RestoreVariables(regdecls),
					      body))));
}

Node *MakeExportBody(Node *body, procNode *u)
{
   List *original_args = u->decl->u.decl.type->u.fdcl.args;
   Node *returns = u->decl->u.decl.type->u.fdcl.returns;
   ListMarker marker;
   Node *arg;
   Node *arg_struct;
   List *call_args = NULL;
   Node *arg_assignment;
   Node *malloc_call;
   Node *free_call = NULL;

   body->u.Block.decl = NULL;
   body->u.Block.stmts = NULL;

   /* Create arg struct  */
   if( (original_args && !IsVoidType(FirstItem(original_args))) || (returns && !IsVoidType(returns))) {
   	   /* define the struct */
	   arg_struct = MakeDecl(ProcArgsName,
				EMPTY_TQ,
				MakePtr(EMPTY_TQ, MakeProcArgsType(u->decl->u.decl.name)),
				NULL,
				NULL);

	   body->u.Block.decl = List1(arg_struct);
   	  /* Allocate the struct */
   	  malloc_call = MakeCall(MakeId(UniqueString("Cilk_malloc_fixed")),
   			  	 List1( MakeUnary(SIZEOF,
				 		  MakeProcArgsType(u->decl->u.decl.name))));
   	  body->u.Block.stmts = List1( MakeBinop('=',
   				          	MakeId(ProcArgsName),
					  	MakeCast(MakePtr(EMPTY_TQ,
					  		  	MakeProcArgsType(u->decl->u.decl.name)),
					  	   	 malloc_call)));
   	  free_call = MakeCall(MakeId(UniqueString("Cilk_free")),
   			       List1(MakeId(ProcArgsName)));
   }

   /* Initialize the argument struct*/
   IterateList(&marker, original_args);
   while (NextOnList(&marker, (GenericREF) & arg)) {
        if( ! IsVoidType(arg) ) {
   		arg_assignment = MakeBinop('=',
				           MakeBinop(ARROW,
				 	   MakeId(ProcArgsName),
			       		   MakeId(arg->u.id.text)),
				   	   MakeId(arg->u.id.text));

		body->u.Block.stmts = AppendItem( body->u.Block.stmts, arg_assignment );
	}
   }

   /* Create arg list for Cilk_start */
   if( (original_args && !IsVoidType(FirstItem(original_args))) || (returns && !IsVoidType(returns)))
   	call_args = List3( MakeId(ContextName),
   			   MakeId(MakeImportProcName(u->decl->u.decl.name)),
	        	   MakeId(ProcArgsName));
   else
   	call_args = List3( MakeId(ContextName),
   			   MakeId(MakeImportProcName(u->decl->u.decl.name)),
			   MakeConstPtr(0) /*NULL*/);


   if(returns && !IsVoidType(returns) ) {
        Node *result;

	/* Add retval to defionitions */
	body->u.Block.decl = AppendItem( body->u.Block.decl,
					 MakeDecl(ProcResultName,
						  EMPTY_TQ,
						  NodeCopy(returns, Subtree),
						  NULL,
						  NULL) );

    	call_args = AppendItem( call_args,
			        MakeUnary(SIZEOF, NodeCopy(returns, Subtree)));
       body->u.Block.stmts = AppendItem( body->u.Block.stmts,
				         MakeCall(MakeId(ExportCilkStartName), call_args ) );
   	result = MakeBinop(ARROW,
			   MakeId(ProcArgsName),
			   MakeId(ProcResultName));

        /* Assign the return value to local varible */
   	body->u.Block.stmts = AppendItem( body->u.Block.stmts,
					  MakeBinop('=',
					  	    MakeId(ProcResultName),
				        	    MakeBinop(ARROW,
				 	    		      MakeId(ProcArgsName),
						   	      MakeId(ProcResultName))));

	/* Delete the allocated arg staruct */
	if( free_call )
	   	body->u.Block.stmts = AppendItem( body->u.Block.stmts,
						  free_call );

   	body->u.Block.stmts = AppendItem( body->u.Block.stmts,
					  MakeReturn(MakeId(ProcResultName)) );
   }else {
    	call_args = AppendItem( call_args,
				MakeConstSint(0));
   	body->u.Block.stmts = AppendItem( body->u.Block.stmts,
				     	  MakeCall(MakeId(ExportCilkStartName), call_args ) );

	/* Delete the allocated arg staruct */
	if( free_call )
	   	body->u.Block.stmts = AppendItem( body->u.Block.stmts,
						  free_call );
   }

   /*printf("MakeExportBody stmts typ=%d\n", body->u.Block.stmts->typ);*/

   return body;
}

Node *MakeImportBody(Node *body, procNode *u)
{
   List *original_args = u->decl->u.decl.type->u.fdcl.args;
   Node *returns = u->decl->u.decl.type->u.fdcl.returns;
   ListMarker marker;
   Node *arg;
   List *use_unused = ParseWildcardString("(void)%d;(void)%d;", WorkerStateName, ProcArgsName_v);
   List *call_args = NULL;
   Node *call, *result;

   /* Create the list of the call arguments */
   call_args = List1( MakeId(WorkerStateName));

   IterateList(&marker, original_args);
   while (NextOnList(&marker, (GenericREF) & arg)) {
        if( ! IsVoidType(arg) )
		call_args = AppendItem( call_args,
					MakeBinop(ARROW,
						  MakeCast(MakePtr(EMPTY_TQ, MakeProcArgsType(u->decl->u.decl.name)),
							   MakeId(ProcArgsName_v)),
						  MakeId(arg->u.id.text) ) );
   }

   call = MakeCall(MakeId(u->decl->u.decl.name), call_args );

   body->u.Block.decl = NULL;
   
   if( returns && !IsVoidType(returns) ) {
   	result = MakeBinop(ARROW,
			   MakeCast(MakePtr(EMPTY_TQ, MakeProcArgsType(u->decl->u.decl.name)),
				    MakeId(ProcArgsName_v)),
			   MakeId(ProcResultName));

	body->u.Block.stmts = AppendItem( use_unused, MakeBinop('=', result,call ));
   }
   else {
	body->u.Block.stmts = AppendItem( use_unused, call);
   }
   /*printf("MakeExportBody stmts typ=%d\n", body->u.Block.stmts->typ);*/

   return body;
}

Node *GlobalizeSUE(SUEtype *type)
{
     Node *junk;
     assert(type->complete);

     /* choose a globally unique name for this SUE */
     if (LookupSymbol(Tags, type->name, (GenericREF) & junk)) {
	  char buf[40];
	  sprintf(buf, "%s%.16s", CilkPrefix, type->name);
	  type->name = InsertUniqueSymbol(Tags, type, buf);
     }
     switch (type->typ) {
	 case Sdcl:
	      return MakeSdcl(TQ_SUE_ELABORATED, type);
	 case Udcl:
	      return MakeUdcl(TQ_SUE_ELABORATED, type);
	 case Edcl:
	      return MakeEdcl(TQ_SUE_ELABORATED, type);
	 default:
	      FAIL("bad type in GlobalizeSUE");
	      return NULL;
     }
}

List *MoveSUEToTop(Node *node, List *sofar)
{
     Node *new = NULL;

#define CODE(n)   sofar = MoveSUEToTop(n, sofar)
     ASTWALK(node, CODE)
#undef CODE

	 switch (node->typ) {
	 case Sdcl:
	      if (tq_has_sue_elaborated(node->u.sdcl.tq)) {
		   new = GlobalizeSUE(node->u.sdcl.type);
		   NodeUpdateTq(node, tq_remove_sue_elaborated);
	      }
	      break;
	 case Udcl:
	      if (tq_has_sue_elaborated(node->u.udcl.tq)) {
		   new = GlobalizeSUE(node->u.udcl.type);
		   NodeUpdateTq(node, tq_remove_sue_elaborated);
	      }
	      break;
	 case Edcl:
	      if (tq_has_sue_elaborated(node->u.edcl.tq)) {
		   new = GlobalizeSUE(node->u.edcl.type);
		   NodeUpdateTq(node, tq_remove_sue_elaborated);
	      }
	      break;
	 case Decl:
	      if (tq_has_static(NodeStorageClass(node)) &&
		  tq_has_block_decl(NodeDeclLocation(node))) {
		   Node *junk;
		   /* choose a globally unique name for this static *
		    * variable */
		   if (LookupSymbol(Identifiers, node->u.decl.name, (GenericREF) & junk)) {
			node->u.decl.name = InsertUniqueSymbol(Identifiers, node, node->u.decl.name);
		   } else {
			InsertSymbol(Identifiers, node->u.decl.name, node, NULL);
		   }
		   new = node;
		   NodeSetDeclLocation(new, TQ_TOP_DECL);
		   new = TransformDecl(new, &new->u.decl);

		   /* 
		    * Note: the static variables will be renamed (if necessary)
		    * by TransformId, and the local static declaration will be
		    * removed by TransformDecl.    -KHR
		    */
	      }
	      break;
	 default:
	      break;
     }

     if (new) {
	  /*
	   * make sure we expand any typedefs of declarations moved to
	   * the top level.   -KHR
	   */
	  new = ExpandTypedefs(new);
	  SetCoords(new, UnknownCoord, Subtree);
	  sofar = AppendItem(sofar, new);
     }
     return sofar;
}

PRIVATE const char* MakeInnerInletName(const char*   this_inlet_name)
{
     if (version == Slow) {
          return (MakeInnerSlowInletName(proc_name, this_inlet_name));
     } else {
          assert(version == Fast);
          return (MakeInnerFastInletName(proc_name, this_inlet_name));
     }
}

/*
 * EBA:
 * move the inlet function from the scope of the Cilk procedure to top 
 * (global) scope.
 * also, change its name as described in RenameCallsToInnerInletNames
 *
 * arguments:
 * node - current node on the AST.
 * inlets - a list of nodes of the inlet procedures. filled by this recursion.
 * inlets_original_name - a list of string name of the inlet procedures as it 
 *      apears in the original code. filled by this recursion.
 */
PRIVATE Node* ExtractInletFunc (Node*   node, 
				List**  inlets,
				List**  inlets_original_name)
{
     if ((node->typ == Proc)  && (IsInletProc(node) == FALSE)) {
	  /* set the curr_proc & proc_name global variables. */
	  proc_name = node->u.proc.decl->u.decl.name;
	  /*
	  printf("entering Cilk proc %s\n", proc_name);
	  */
     }
     /* EBA
      * Extract the inlet out of the procedure's body. it will be added 
      * later as a top level function.
      */
     if ((node->typ == Proc) && 
	 (tq_has_slow_inlet(node->u.proc.decl->u.decl.tq) ||
	  tq_has_fast_inlet(node->u.proc.decl->u.decl.tq))) {
	  const char*   this_inlet_name = MakeInnerInletName(node->u.proc.decl->u.decl.name);
	  const char*   short_name;

	  assert((version == Slow) || (version == Fast));
	  if (version == Slow) {
	       short_name = "Slow";
	  } else {
	       short_name = "Fast";
	  }
	  /*
	  printf("changing inlet name from '%s' to '%s'\n", 
		 node->u.proc.decl->u.decl.name,
		 this_inlet_name);
	  */
	  *inlets_original_name = 
	       AppendItem(*inlets_original_name,
			  UniqueString(node->u.proc.decl->u.decl.name));
	  node->u.proc.decl->u.decl.name = this_inlet_name;
	  *inlets = AppendItem(*inlets, node);
	  return (MakeText(MakeRemovedInletRemark(short_name), 
			   TRUE));
     }
     
     /* transform children */
#define CHILD(n)  n = ExtractInletFunc(n, inlets, inlets_original_name)
     ASTWALK(node, CHILD)
#undef CHILD
	  
     return (node);
}

#if 0
/*
 * EBA:
 * remove all inlet functions from the scope of the Cilk procedure.
 * This is done by replacing them with text nodes containing a remark.
 */
PRIVATE Node* RemoveInletFunctions (Node*  node)
{
     /* EBA:
      * the "Proc" node that declares an INLET is found, the original node
      * is replaced with the returned node. so i replace it with an text
      * node :)
      */
     if ((node->typ == Proc) && 
	 (node->u.proc.decl->u.decl.tq & T_FAST_INLET)) {
	  return (MakeText("/*------- Fast inlet as inner function was removed"
			   " ------*/", 
			   TRUE));
     }

     /* transform children */
#define CHILD(n)  n = RemoveInletFunctions(n)
     ASTWALK(node, CHILD)
#undef CHILD
	  return node;
}
#endif

/* EBA
 * because the inner inlet is extracted to global scope its name is changed.
 * the inlet named INLET in Cilk procedure PROC will be named 
 * "_cilk_PROC_INLET_inlet_{slow|fast}".
 */
PRIVATE Node* RenameCallsToInnerInletNames (Node*  node,
					    List*  inlets_original_name)
{
     if ((node->typ == Proc) && (IsInletProc(node) == FALSE)) {
	  /* set the curr_proc & proc_name global variables. */
	  proc_name = node->u.proc.decl->u.decl.name;
	  /*
	  printf("entering Cilk proc %s\n", proc_name);
	  */
     }
     if (node->typ == Call) {
	  int         is_call_to_inlet;

	  /* check whether this call is made to an inlet */
	  is_call_to_inlet  = FALSE;
#define FIND_INLET_OP(iname)    \
if (strcmp(node->u.call.name->u.id.text, \
           (const char*)iname) == 0) \
              is_call_to_inlet = TRUE;
	  LISTWALK(inlets_original_name, FIND_INLET_OP)
#undef FIND_INLET_OP

	  if (is_call_to_inlet == TRUE) {
	       /* its a call to an inlet - replace name of function to be 
		* called
		*/
	       const char* this_inlet_name = MakeInnerInletName(node->u.proc.decl->u.decl.name);
	       /*
	       printf("changing inlet name from '%s' to '%s'\n", 
		      node->u.proc.decl->u.decl.name,
		      this_inlet_name);
	       */
	       node->u.proc.decl->u.decl.name = this_inlet_name;
	       return node;
	  }

     }

    /* transform children */
#define CHILD(n)  n = RenameCallsToInnerInletNames(n, inlets_original_name)
     ASTWALK(node, CHILD)
#undef CHILD
	  return node;
}

PRIVATE inline List *TransformCilkProc(Node *node, procNode *u)
{
     Node *fastnode, *slownode, *exportnode, *importnode;
     Node *decl;
     List *result;
     List *inlets_nodes = NULL;
     List *slow_inlets_original_name = NULL;
     List *fast_inlets_original_name = NULL;


     if (!strcmp("main",u->decl->u.decl.name)) {
	  u->decl->u.decl.name = UniqueString("cilk_main");
     }

     /* if implicit sync is needed, calculating return expressions and
      * saving the result into tempary variables before implicit sync.
      * -fengmd */
     node = AnalyzeReturnExpressions(node);

     /* analyze procedure variables */
     AnalyzeVariables(node);

     /* start with empty text node - just enforces coord start */
     result = List1(MakeTextCoord("", FALSE, node->coord));

     /* move structures/unions/enums, static variables to outer scope */
     result = JoinLists(result, MoveSUEToTop(node, NULL));

     /* make frame declaration for procedure */
     result = AppendItem(result, MakeFrame(u));

     /* Make arg structure declaration for import/export procedures */
     result = AppendItem(result, MakeArgsAndResultStruct(u));

     /* write down some global variables for other routines to read */
     proc_name = u->decl->u.decl.name;
     curr_proc = node;

     fastnode = NodeCopy(node, Subtree);
     slownode = NodeCopy(node, Subtree);
     exportnode = NodeCopy(node, Subtree);
     importnode = NodeCopy(node, Subtree);

     /*printf("TransformCilkProc - exoprtnode->decl=%p exoprtnode->decl->typ=%d\n", exportnode->u.proc.decl, exportnode->u.proc.decl->typ);*/

     /* add implicit sync and return to slow and fast bodies, if needed */
     if (node->u.proc.needs_sync) {
	  slownode->u.proc.body->u.Block.stmts =
	      AppendItem(slownode->u.proc.body->u.Block.stmts,
	      MakeSyncCoord(slownode->u.proc.body->u.Block.right_coord));
	  fastnode->u.proc.body->u.Block.stmts =
	      AppendItem(fastnode->u.proc.body->u.Block.stmts,
	      MakeSyncCoord(fastnode->u.proc.body->u.Block.right_coord));
     }
     if (node->u.proc.needs_return) {
	  slownode->u.proc.body->u.Block.stmts =
	      AppendItem(slownode->u.proc.body->u.Block.stmts,
			 MakeReturnCoord(NULL,
			    slownode->u.proc.body->u.Block.right_coord));
	  fastnode->u.proc.body->u.Block.stmts =
	      AppendItem(fastnode->u.proc.body->u.Block.stmts,
			 MakeReturnCoord(NULL,
			    fastnode->u.proc.body->u.Block.right_coord));
     }
     /* transform export body */
     sync_count = 0;
     version = Export;
     exportnode->u.proc.decl = MakeExportDecl(u);
     in_procedure = 1;
     exportnode->u.proc.body = MakeExportBody(exportnode->u.proc.body, u);
     in_procedure = 0;
     SetCoords(exportnode->u.proc.decl, exportnode->coord, Subtree);

     /* transform import body */
     sync_count = 0;
     version = Import;
     importnode->u.proc.decl = MakeImportDecl(u);
     in_procedure = 1;
     importnode->u.proc.body = MakeImportBody(importnode->u.proc.body, u);
     in_procedure = 0;
     SetCoords(importnode->u.proc.decl, importnode->coord, Subtree);

     /* transform fast body */
     sync_count = 0;
     version = Fast;
     fastnode->u.proc.decl = TransformNode(fastnode->u.proc.decl);
     in_procedure = 1;
     fastnode->u.proc.body = TransformNode(fastnode->u.proc.body);
     in_procedure = 0;

     /* transform slow body */
     sync_count = 0;
     version = Slow;
     /* add info for entry 0 */
     link_initializers  =
	  List1(MakeInitializer(
	       List5(IsVoidType(u->decl->u.decl.type->u.fdcl.returns) ?
		     MakeConstSint(0) :
		     MakeUnary(SIZEOF, NodeCopy(u->decl->u.decl.type->u.fdcl.returns, Subtree)),
		     MakeUnary(SIZEOF, MakeFrameType(proc_name)),
		     MakeId(MakeSlowProcName(proc_name)),
		     MakeConstSint(0),
		     MakeConstSint(0)
		    )
	       ));
     standalone_inlets = NULL;
     slownode->u.proc.decl = TransformNode(slownode->u.proc.decl);
     in_procedure = 1;
     slownode->u.proc.body = TransformNode(slownode->u.proc.body);
     in_procedure = 0;

     /* add stuff to beginning of procedure */
     fastnode->u.proc.body = AddFastStuff(fastnode->u.proc.body,
			fastnode->u.proc.decl->u.decl.type->u.fdcl.args);
     slownode->u.proc.body = AddSlowStuff(slownode->u.proc.body,
			slownode->u.proc.decl->u.decl.type->u.fdcl.args);

     /* EBA: 
      * remove inner inlet function of FAST clone.
      * move inner inlet function of SLOW clone to global scope.
      * change name of the inlet function that has now become global.
      */
     version = Slow;
     slownode = ExtractInletFunc(slownode, 
				 &inlets_nodes, 
				 &slow_inlets_original_name);
     RenameCallsToInnerInletNames(slownode, slow_inlets_original_name);
     version = Fast;
     fastnode = ExtractInletFunc(fastnode, 
				 &inlets_nodes, 
				 &fast_inlets_original_name);
     RenameCallsToInnerInletNames(fastnode, fast_inlets_original_name);

     /* make slow decl */
     /*
      * cilk int foo(int x, int y)  ==>  void _foo_slow(_foo_frame *_frame)
      */
     decl = MakeSlowDecl(proc_name);
     SetCoords(decl, slownode->coord, Subtree);
     slownode->u.proc.decl = decl;

     /* make fast decl */
     /*
      * cilk int foo(int x, int y)  ==>  int _foo_fast(int x, int y)
      */
     /* Just change the name */
     fastnode->u.proc.decl->u.decl.name = MakeFastProcName( fastnode->u.proc.decl->u.decl.name );

     /* set fast decl flag */
     NodeUpdateTq(fastnode->u.proc.decl, tq_add_fast_procedure);

     /* make slow procedure declaration */
     result = AppendItem(result,
			 MakeSlowDecl(u->decl->u.decl.name));

     /* make export procedure declaration */
    /* result = AppendItem(result,
			 MakeExportDecl(u));
*/
     /* add in standalone inlets */
     result = JoinLists(result, standalone_inlets);

     /* add linkage stuff */
     result = AppendItem(result, MakeLinkage(node));

     /*printf("TransformCilkProc - exportnode=%p exoprtnode->typ=%d\n", exportnode, exportnode->typ);*/
     /*printf("TransformCilkProc - exoprtnode->decl=%p exoprtnode->decl->typ=%d\n", exportnode->u.proc.decl, exportnode->u.proc.decl->typ);*/
     /*printf("TransformCilkProc - slownode->decl->typ=%d\n", slownode->u.proc.decl->typ);*/
     result = JoinLists(result, inlets_nodes);
     result = AppendItem(result, fastnode);
     result = AppendItem(result, slownode);
     result = AppendItem(result, importnode);
     result = AppendItem(result, exportnode);

     return result;
}

PRIVATE Node *TransformNode(Node *node)
{
     if (node == NULL)
	  return NULL;
#define CODE(name, node, union) return Transform##name(node, union)
     ASTSWITCH(node, CODE)
#undef CODE
	 UNREACHABLE;
     return NULL;
}

GLOBAL List *TransformTopDecl(Node *node)
{
     List *result;

     if (!node)
	  return NULL;

     if (node->typ == Proc &&
	 node->u.proc.decl->u.decl.type->typ == Fdcl &&
	 tq_has_procedure(node->u.proc.decl->u.decl.type->u.fdcl.tq)) {
	  /* cilk procedure */
	  result = TransformCilkProc(node, &node->u.proc);
     } else
	  result = List1(TransformNode(node));

     return (result);
}

GLOBAL List *TransformPass1(List *program)
{
     ListMarker marker;
     Node *node;
     List *newprogram = NULL;

     IterateList(&marker, program);
     while (NextOnList(&marker, (GenericREF) & node)) {
	  newprogram = JoinLists(newprogram, TransformTopDecl(node));
     }

     return newprogram;
}

/*
 * TransformProgram should convert a type-checked source language tree
 * into a standard C tree.  Type information does not need to be preserved,
 * but all new Decl nodes must be properly annotated with their DECL_LOCATION
 * (e.g. top-level, block, structure field, etc.).
 */

GLOBAL List *TransformProgram(List *program)
{
     InitCilkTransform();
     program = TransformPass1(program);
     program = ConsItem(MakeText("#include <cilk-cilk2c.h>\n", TRUE),
			program);
     return program;
}

PRIVATE int operator_type_number(OpType assign_op, Node *type)
{
  unsigned int operator = 0;
  unsigned int type_num;

  /* commutative operators have same op_num */
  switch (assign_op) {
  case PLUSassign:
  case MINUSassign:
  case PREINC:
  case PREDEC:
  case POSTINC:
  case POSTDEC:
    operator = 0;
    break;
  case MULTassign:
    if (IsFloatingType(type))
      operator = 1;
    else
      operator = 2;
    break;
  case DIVassign:
    operator = 1;
    break;
  case ANDassign:
  case ERassign:
  case ORassign:
    operator = 2;
    break;
  case LSassign:
  case RSassign:
  case MODassign:
  case '=':
    operator = 3;
    break;
  default: 
    FAIL("bad operator in operator_number");
  }
  
  if (IsIntegralType(type))
    type_num = 0;
  else if (IsFloatingType(type))
    type_num = 1;
  else if (IsPointerType(type))
    type_num = 2;
  else
    type_num = 3;

  assert(operator < 4);
  assert(type_num < 4);
  return ((type_num << 2) + operator);
}

PRIVATE void ClearCoord(Node *node)
{
  node->coord = UnknownCoord;
#define CHILD(n) ClearCoord(n)
  ASTWALK(node, CHILD)
#undef CHILD
}  

PRIVATE Node *name_strip(Node *node)
{
  Node *anode;

#define CHILD(n)  n = name_strip(n)
  ASTWALK(node, CHILD)
#undef CHILD

  if ((node->typ == Unary) && (node->u.unary.op == INDIR)) {
    anode = node->u.unary.expr;
    if (anode->typ == Cast) {
      anode = anode->u.cast.expr;
      if ((anode->typ == Call) && (anode->u.call.name->typ == Id)) {
	if ((strcmp(anode->u.call.name->u.id.text, 
		    AccessReadProfileName) == 0) ||
	    (strcmp(anode->u.call.name->u.id.text, 
		    AccessReadProfileName4) == 0) ||
	    (strcmp(anode->u.call.name->u.id.text, 
		    AccessReadProfileName8) == 0) ||
	    (strcmp(anode->u.call.name->u.id.text, 
		    AccessWriteProfileName) == 0) ||
	    (strcmp(anode->u.call.name->u.id.text, 
		    AccessWriteProfileName4) == 0) ||
	    (strcmp(anode->u.call.name->u.id.text, 
		    AccessWriteProfileName8) == 0))
	  return FirstItem(anode->u.call.args);
	else if ((strcmp(anode->u.call.name->u.id.text, 
			 AccessReadProfileAddrName) == 0) ||
		 (strcmp(anode->u.call.name->u.id.text,
			 AccessReadProfileAddrName4) == 0) ||
		 (strcmp(anode->u.call.name->u.id.text,
			 AccessReadProfileAddrName8) == 0) ||
		 (strcmp(anode->u.call.name->u.id.text, 
			 AccessWriteProfileAddrName) == 0) ||
		 (strcmp(anode->u.call.name->u.id.text,
			 AccessWriteProfileAddrName4) == 0) ||
		 (strcmp(anode->u.call.name->u.id.text,
			 AccessWriteProfileAddrName8) == 0) ||
		 (strcmp(anode->u.call.name->u.id.text,
			 SpawnVarProfileName) == 0))
	  return MakeUnary(INDIR, FirstItem(anode->u.call.args));
      }
    }
  }
  else if ((node->typ == Binop) && (node->u.binop.op == '.') &&
	   (node->u.binop.left->typ == Binop) && 
	   (node->u.binop.left->u.binop.op == ARROW) &&
	   (node->u.binop.left->u.binop.left->typ == Id) &&
	   (strcmp(node->u.binop.left->u.binop.left->u.id.text, "_frame") == 0))
    return node->u.binop.right;

  return node;
}

PRIVATE Node *AccessProfile(Node *node, Node *type, AccessProfileType accesstype, int withaddress, int opt, int multiple, int operator)
{
     Node *call = NULL, *atype, *op_type_node, *var_node;
     int size;
     /*
     if (!(node->memorycheckedp))
         return node;
     */
     if (!node)
         return node;

     /* for array, we are taking address of the array, no memory access.
	for example, int a[2], and any use of a is no access of memory */
     if (type->typ == Adcl)
       return node;

     /* if the real type of node is an array (though typedef), we convert
	this into a pointer type. 
	for example, typedef int aa[10]; typedef aa *bb; bb y; then *y.
	however, the cast type is unchanged, so *y=*((aa *)(y,sizeof(int *))).
	The sizeof is correct for function call with argument *y because
	we pass pointer rather than the whole array.
	But the sizeof would be wrong if *y is used elsewhere (I couldn't
	think of any other legal use (*y), so I leave it unconsidered */
     atype = NodeCopy(NodeDataType(type), Subtree);
     assert(atype);
     if (atype->typ == Adcl)
       atype = MakePtr(EMPTY_TQ, atype->u.adcl.type);
     if (atype->typ == Ptr) {
       assert(atype->u.ptr.type);
     }
     ClearCoord(atype);

     assert(type);
     if (type->typ == Ptr) {
       assert(type->u.ptr.type);
     }
     ClearCoord(type);

     size = NodeSizeof(node, atype, FALSE) * multiple;
     op_type_node = MakeConstSint(operator_type_number(operator, atype));

     var_node = name_strip(NodeCopy(node, Subtree));

     switch (accesstype) {
     case Read:
       if (size == sizeof(int)) {
	 call = MakeId(withaddress ? 
		       AccessReadProfileAddrName4 :
		       AccessReadProfileName4);
	 call = MakeCall(call, List3(node, op_type_node, var_node));
       }
       else if (size == sizeof(double)) {
	 call = MakeId(withaddress ?
		       AccessReadProfileAddrName8 :
		       AccessReadProfileName8);
	 call = MakeCall(call, List3(node, op_type_node, var_node));
       }
       else {
	 call = MakeId(withaddress ?
		       AccessReadProfileAddrName :
		       AccessReadProfileName);
	 call = MakeCall(call, List4(node, MakeConstSint(size), op_type_node,
				     var_node));
       }
       break;
     case Write:
       if (size == sizeof(int)) {
	 call = MakeId(withaddress ? 
		       AccessWriteProfileAddrName4 :
		       AccessWriteProfileName4);
	 call = MakeCall(call, List3(node, op_type_node, var_node));
       }
       else if (size == sizeof(double)) {
	 call = MakeId(withaddress ?
		       AccessWriteProfileAddrName8 :
		       AccessWriteProfileName8);
	 call = MakeCall(call, List3(node, op_type_node, var_node));
       }
       else {
	 call = MakeId(withaddress ?
		       AccessWriteProfileAddrName :
		       AccessWriteProfileName);
	 call = MakeCall(call, List4(node, MakeConstSint(size), op_type_node,
				     var_node));
       }
       break;
     default:
       FAIL("bad access type in AccessProfile");
     }

     if (opt == 0) {
       call = MakeCast(MakePtr(EMPTY_TQ, type), call);
       call->parenthesized = 1;
       if (!withaddress) {
	 call = MakeUnary(INDIR, call);
	 call->u.unary.type = type;
	 call->parenthesized = 1;
       }
       call->coord = node->coord;
     }
     return call;
}

PRIVATE Node *SpawnVarProfile(Node *node, Node *type, OpType assign_op)
{
     Node *var_node;

     /*
     if (node->memorycheckedp)
         return node;
     */

     if (!node)
         return node;

     assert(type);
     type = NodeDataType(type);
     assert(type);

     var_node = name_strip(NodeCopy(node, Subtree));

     return MakeCall(MakeId(SpawnVarProfileName),
                     List5(node,
                           MakeConstSint(NodeSizeof(node, type, FALSE)),
			   MakeConstSint(
			     operator_type_number(assign_op, type)),
			   MakeCast(MakePtr(EMPTY_TQ,
					    MakeGenericFrameType()),
				    MakeId(FrameName)),
			   var_node));
     /* Could be*/
     /*  ParseWildcardStringNode("%d1(%e1, %e2, %e3, (%t1)%d2, %e4);",*/
     /*                           SpawnVarProfileName,*/
     /*                           node,*/
     /*                           MakeConstSint(NodeSizeof(node, type, FALSE)),*/
     /*                           MakeConstSint(operator_type_nmber(assign_op, type))*/
     /*                           MakePtr(EMPTY_TQ, MakeGenericFrameType())*/
     /*                           FrameName,*/
     /*                           var_node);*/
}

/*
 * similar routine as AnalyzeGlobalReadWrite in analyze.c.
 * use bottom-up tree-search to find all frame reference.
 * called in AnalyzeVariables after the kind of Id is analyzed.
 * node->nondeterminator is TRUE if node potentially accesses shared location.
 */
PRIVATE void AnalyzeFrameReadWrite(Node *node)
{
  switch (node->typ) {
  case Id:
    if (node->u.id.decl && ((node->u.id.decl->u.decl.kind == 2) || 
			    (node->u.id.decl->u.decl.kind == 3)))
      node->nondeterminator = TRUE;
    break;
  default:
    break;
  }

#define CODE(n)   AnalyzeFrameReadWrite(n)
  ASTWALK(node, CODE)
#undef CODE
    
  switch (node->typ) {
  case Array:
    if (node->u.array.name->nondeterminator) {
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
    break;
  case Unary:
    if (IsIncDecOp(node->u.unary.op) && node->u.unary.expr->nondeterminator) {
      /*
      if ((node->u.unary.expr->typ == Id) &&
	  (node->u.unary.expr->u.id.decl->u.decl.kind == 3))
	node->u.unary.expr->nondeterminator = FALSE;
      */
      node->nondeterminator = TRUE;
      node->u.unary.expr->nondeterminator = FALSE;
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
  default:
    break;
  }
}













