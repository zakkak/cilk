
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Rob Miller
 * 
 *************************************************************************/
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/dataflow.c $ $LastChangedBy: bradley $ $Rev: 1469 $ $Date: 2004-08-02 07:27:43 -0400 (Mon, 02 Aug 2004) $");


/*************************************************************************

  Data-flow frameworks

  Specify your data-flow problem by providing the following.
  Note especially the "Very Important" sections.

    1. Datatype: a set V of values to be propagated, inserted in the
       FlowValue union in dataflow.h.  Some typical datatypes are
       already present: bit vectors, lists, and void pointers (which
       can be used for anything).  V also contains a distinguished
       Undefined element, represented below by U (and in the
       implementation by a FlowValue structure with field "undefined"
       == TRUE).

    2. Direction of propagation: Forwards or Backwards.

    3. Meet operation (^): a function mapping FlowValue x FlowValue to 
       FlowValue, which satisfies the following properties:

       **** VERY IMPORTANT ****
          - commutative:  x^y = y^x
	  - associative:  (x^y)^z = x^(y^z)
	  - idempotent:   (x^y)^y = x^y
	  - has identity U (the distinguished Undefined element):
                 x ^ U = x
       **** end very important part ****

       With these properties, (V,^) is a lattice.  In particular, we can
       define the partial ordering <= on V to be
 
                 x <= y  iff  x ^ y = x

       (Thus the distinguished Undefined element U is the top of the
       lattice, since U >= x for all elements x in V.)

       Finally, the lattice (V,^) must be finite:

          - finite:   all chains x_1 <= x_2 <= ... in V must have finite 
	              length.  (If V is finite, you don't have to worry
		      about this one.)

    4. Equality operation (=): a function mapping FlowValue x FlowValue to
       Bool, which determines whether two elements of V are the same.

    5. Transfer function (trans): a function mapping Node x Point x
       FlowValue to FlowValue, which transforms a value in V as it
       passes through the specified point (Entry or Exit) in the specified
       AST node.

       The transfer function for a particular node/point, trans(N,P), is a
       member of a set of transfer functions F, which map FlowValue to
       FlowValue.  The functions in F must satisfy the following:

          - closed under composition:  f, g in F ==> fg in F
	  - contains identity: 
                there exists i in F s.t. i(x) = x for all x in V

       **** VERY IMPORTANT ****
	  - monotonic:
	        f(x ^ y) <= f(x) ^ f(y)  for all f in F and x,y in V

            an equivalent statement of monotonicity which may be easier
            for you to establish:

	        x <= y  ==>  f(x) <= f(y)  for all f in F and x,y in V

            Without monotonicity, your framework will not converge in all
	    cases.
       **** end very important part ****

       The functions in F may optionally satisfy:

          - strict:  f(U) = U   (where U is distinguished Undefined element)
            If F is strict, unreachable nodes will remain U throughout
	    the data-flow algorithm, so they will never contribute flow
	    values to the rest of the program.  This is normally desirable.
	    Causing f(U) = U is as simple as checking the undefined bit
	    in your input FlowValue and returning the same value immediately
	    if the bit is set.

       The trans subroutine also receives a boolean flag, FinalPass,
       that indicates whether this is the final visit to the
       node/point in the data-flow algorithm.  When FinalPass is true,
       the trans subroutine can consider its FlowValue input to be 
       a final, converged value, and can use it to mark up the tree
       with permanent analysis results.
       

    6. Initial element I: an element in V which should be passed into
       to the topmost node of the AST.

  Except for the datatype, each of these parts of the framework must be
  passed to the dataflow control routine, IterateDataFlow.

 *************************************************************************/

/*************************************************************************/
/*                                                                       */
/*                      Generic data-flow framework                      */
/*                                                                       */
/*************************************************************************/

PRIVATE Bool Forw;
PRIVATE MeetOp Meet;
PRIVATE EqualOp Equal;
PRIVATE TransOp Trans;
PRIVATE FlowValue Init;

PRIVATE Bool Changed;
PRIVATE Bool Final;

#define Entry(n, v)  Trans(n, v, EntryPoint, Final)
#define Exit(n, v)   Trans(n, v, ExitPoint, Final)

PRIVATE FlowValue DataFlow(Node *node, FlowValue in);
PRIVATE FlowValue DataFlowSerialList(List *list, FlowValue in);
PRIVATE inline FlowValue DataFlowBranch(Node *cond, Node *true, Node *false, FlowValue in);

PRIVATE FlowValue FlowInto(FlowValue *dest, FlowValue in);

/*************************************************************************/
/*                                                                       */
/*                     Data flow for each node type                      */
/*                                                                       */
/*************************************************************************/

PRIVATE inline FlowValue DataFlowConst(Node *node, UNUSED(ConstNode *u), FlowValue v)
{
     if (Forw)
	  return Exit(node, Entry(node, v));
     else
	  return Entry(node, Exit(node, v));
}

PRIVATE inline FlowValue DataFlowId(Node *node, UNUSED(idNode *u), FlowValue v)
{
     if (Forw)
	  return Exit(node, Entry(node, v));
     else
	  return Entry(node, Exit(node, v));
}

PRIVATE inline FlowValue DataFlowBinop(Node *node, binopNode *u, FlowValue v)
{
     if (u->op == ANDAND || u->op == OROR) {
	  /* short-circuiting Boolean operators act like a branch:
	   * always evaluates left operand, sometimes evaluates right operand */
	  if (Forw)
	       return Exit(node,
			   DataFlowBranch(u->left, u->right, NULL,
					  Entry(node, v)));
	  else
	       return Entry(node,
			    DataFlowBranch(u->left, u->right, NULL,
					   Exit(node, v)));

     } else {
	  /* all other operators always evaluate both operands */
	  if (Forw)
	       return Exit(node,
			   DataFlow(u->right,
				    DataFlow(u->left,
					     Entry(node, v))));
	  else
	       return Entry(node,
			    DataFlow(u->left,
				     DataFlow(u->right,
					      Exit(node, v))));
     }

}

PRIVATE inline FlowValue DataFlowUnary(Node *node, unaryNode *u, FlowValue v)
{
     if (u->op == SIZEOF || u->op == ALIGNOF) {
	  /* expression in sizeof/__alignof__ is never executed */
	  if (Forw)
	       return Exit(node, Entry(node, v));
	  else
	       return Entry(node, Exit(node, v));
     } else {
	  if (Forw)
	       return Exit(node, DataFlow(u->expr, Entry(node, v)));
	  else
	       return Entry(node, DataFlow(u->expr, Exit(node, v)));
     }
}

PRIVATE inline FlowValue DataFlowCast(Node *node, castNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlow(u->expr, Entry(node, v)));
     else
	  return Entry(node, DataFlow(u->expr, Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowComma(Node *node, commaNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlowSerialList(u->exprs, Entry(node, v)));
     else
	  return Entry(node, DataFlowSerialList(u->exprs, Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowConstructor(Node *node, constructorNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlow(u->initializerlist, Entry(node, v)));
     else
	  return Entry(node, DataFlow(u->initializerlist, Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowTernary(Node *node, ternaryNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowBranch(u->cond, u->true, u->false,
				     Entry(node, v)));
     else
	  return Entry(node,
		       DataFlowBranch(u->cond, u->true, u->false,
				      Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowArray(Node *node, arrayNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->dims,
					 DataFlow(u->name,
						  Entry(node, v))));
     else
	  return Entry(node,
		       DataFlow(u->name,
				DataFlowSerialList(u->dims,
						   Exit(node, v))));
}

PRIVATE inline FlowValue DataFlowCall(Node *node, callNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->args,
					 DataFlow(u->name,
						  Entry(node, v))));
     else
	  return Entry(node,
		       DataFlow(u->name,
				DataFlowSerialList(u->args,
						   Exit(node, v))));
}

PRIVATE inline FlowValue DataFlowInitializer(Node *node, initializerNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlowSerialList(u->exprs, Entry(node, v)));
     else
	  return Entry(node, DataFlowSerialList(u->exprs, Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowImplicitCast(Node *node, implicitcastNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlow(u->expr, Entry(node, v)));
     else
	  return Entry(node, DataFlow(u->expr, Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowLabel(Node *node, labelNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      Entry(node,
			    FlowInto(&u->label_values, v)));
     else
	  return FlowInto(&u->label_values,
			  Entry(node,
				Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowSwitch(Node *node, SwitchNode *u, FlowValue v)
{
     if (Forw) {
	  FlowInto(&u->switch_values, DataFlow(u->expr, Entry(node, v)));

	  /* if expression doesn't match any case label, control may short-circuit
	   * directly from expression to the end of the switch.  This can only
	   * happen when switch has no default case: */
	  if (!u->has_default)
	       FlowInto(&u->break_values, u->switch_values);

	  /* expression does not flow directly to statement -- it's a jump to
	   * a case label */
	  v.undefined = TRUE;	/* pass in "Undefined" to statement */
	  FlowInto(&u->break_values, DataFlow(u->stmt, v));

	  return Exit(node, u->break_values);
     } else {
	  DataFlow(u->stmt, FlowInto(&u->break_values, Exit(node, v)));
	  if (!u->has_default)
	       FlowInto(&u->switch_values, u->break_values);
	  return Entry(node, DataFlow(u->expr, u->switch_values));
     }
}

PRIVATE inline FlowValue DataFlowCase(Node *node, CaseNode *u, FlowValue v)
{
     assert(u->container->typ == Switch);
     if (Forw)
	  return Exit(node,
		      Entry(node,
			    Meet(v,
				 u->container->u.Switch.switch_values)));
     else {
	  v = Entry(node,
		    Exit(node, v));
	  FlowInto(&u->container->u.Switch.switch_values, v);
	  return v;
     }
}

PRIVATE inline FlowValue DataFlowDefault(Node *node, DefaultNode *u, FlowValue v)
{
     assert(u->container->typ == Switch);
     if (Forw)
	  return Exit(node,
		      Entry(node,
			    Meet(v,
				 u->container->u.Switch.switch_values)));
     else {
	  v = Entry(node,
		    Exit(node, v));
	  FlowInto(&u->container->u.Switch.switch_values, v);
	  return v;
     }
}

PRIVATE inline FlowValue DataFlowIf(Node *node, IfNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowBranch(u->expr, u->stmt, NULL,
				     Entry(node, v)));
     else
	  return Entry(node,
		       DataFlowBranch(u->expr, u->stmt, NULL,
				      Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowIfElse(Node *node, IfElseNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowBranch(u->expr, u->true, u->false,
				     Entry(node, v)));
     else
	  return Entry(node,
		       DataFlowBranch(u->expr, u->true, u->false,
				      Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowWhile(Node *node, WhileNode *u, FlowValue v)
{
     FlowValue e, s;

     if (Forw) {
	  FlowInto(&u->loop_values, Entry(node, v));
	  FlowInto(&u->loop_values,
		   DataFlow(u->stmt,
			    e = DataFlow(u->expr, u->loop_values)));
	  FlowInto(&u->break_values, e);
	  return Exit(node, u->break_values);
     } else {
	  FlowInto(&u->break_values, Exit(node, v));
	  s = DataFlow(u->stmt, u->loop_values);
	  FlowInto(&u->loop_values, DataFlow(u->expr, Meet(s, u->break_values)));
	  return Entry(node, u->loop_values);
     }
}

PRIVATE inline FlowValue DataFlowDo(Node *node, DoNode *u, FlowValue v)
{
     FlowValue e;

     if (Forw) {
	  FlowInto(&u->loop_values, Entry(node, v));
	  FlowInto(&u->continue_values, DataFlow(u->stmt, u->loop_values));
	  FlowInto(&u->loop_values,
		   e = DataFlow(u->expr, u->continue_values));
	  FlowInto(&u->break_values, e);
	  return Exit(node, u->break_values);
     } else {
	  FlowInto(&u->break_values, Exit(node, v));
	  FlowInto(&u->continue_values,
	       DataFlow(u->expr, Meet(u->break_values, u->loop_values)));
	  FlowInto(&u->loop_values,
		   DataFlow(u->stmt, u->continue_values));
	  return Entry(node, u->loop_values);
     }
}

PRIVATE inline FlowValue DataFlowFor(Node *node, ForNode *u, FlowValue v)
{
     FlowValue e;

     if (Forw) {
	  FlowInto(&u->loop_values, DataFlow(u->init, Entry(node, v)));

	  if (u->cond)
	       FlowInto(&u->break_values, e = DataFlow(u->cond, u->loop_values));
	  else
	       /* definitely infinite loop */
	       e = u->loop_values;

	  FlowInto(&u->continue_values, DataFlow(u->stmt, e));
	  FlowInto(&u->loop_values, DataFlow(u->next, u->continue_values));
	  return Exit(node, u->break_values);
     } else {
	  FlowInto(&u->break_values, Exit(node, v));
	  FlowInto(&u->continue_values, DataFlow(u->next, u->loop_values));
	  e = DataFlow(u->stmt, u->continue_values);

	  if (u->cond)
	       FlowInto(&u->loop_values, DataFlow(u->cond, Meet(u->break_values, e)));
	  else
	       FlowInto(&u->loop_values, e);

	  return Entry(node, DataFlow(u->init, u->loop_values));
     }
}

PRIVATE inline FlowValue DataFlowGoto(Node *node, GotoNode *u, FlowValue v)
{
     if (Forw) {
	  FlowInto(&u->label->u.label.label_values, Exit(node, Entry(node, v)));

	  /* return "undefined" value, since control never continues past a goto */
	  v.undefined = TRUE;
	  return v;
     } else
	  return Entry(node, Exit(node, u->label->u.label.label_values));
}

PRIVATE inline FlowValue DataFlowContinue(Node *node, ContinueNode *u, FlowValue v)
{
     FlowValue *pv;

     switch (u->container->typ) {
	 case Do:
	      pv = &u->container->u.Do.continue_values;
	      break;
	 case While:
	      pv = &u->container->u.While.loop_values;
	      break;
	 case For:
	      pv = &u->container->u.For.continue_values;
	      break;
	 default:
	      UNREACHABLE;
	      pv = NULL;
	      break;
     }

     if (Forw) {
	  FlowInto(pv, Exit(node, Entry(node, v)));
	  /* return "undefined" value */
	  v.undefined = TRUE;
	  return v;
     } else
	  return Entry(node, Exit(node, *pv));
}

PRIVATE inline FlowValue DataFlowBreak(Node *node, BreakNode *u, FlowValue v)
{
     FlowValue *pv;

     switch (u->container->typ) {
	 case Do:
	      pv = &u->container->u.Do.break_values;
	      break;
	 case While:
	      pv = &u->container->u.While.break_values;
	      break;
	 case For:
	      pv = &u->container->u.For.break_values;
	      break;
	 case Switch:
	      pv = &u->container->u.Switch.break_values;
	      break;
	 default:
	      UNREACHABLE;
	      pv = NULL;
	      break;
     }

     if (Forw) {
	  FlowInto(pv, Exit(node, Entry(node, v)));
	  /* return "undefined" value */
	  v.undefined = TRUE;
	  return v;
     } else
	  return Entry(node, Exit(node, *pv));
}

PRIVATE inline FlowValue DataFlowReturn(Node *node, ReturnNode *u, FlowValue v)
{
     if (Forw) {
	  FlowInto(&u->proc->u.proc.return_values,
		   Exit(node, DataFlow(u->expr, Entry(node, v))));
	  /* return "undefined" value */
	  v.undefined = TRUE;
	  return v;
     } else
	  return Entry(node,
		       DataFlow(u->expr,
				Exit(node, Init)));
}

PRIVATE inline FlowValue DataFlowBlock(Node *node, BlockNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->stmts,
					 DataFlowSerialList(u->decl,
						       Entry(node, v))));
     else
	  return Entry(node,
		       DataFlowSerialList(u->decl,
					  DataFlowSerialList(u->stmts,
							Exit(node, v))));
}


PRIVATE inline FlowValue DataFlowPrim(UNUSED(Node *node), UNUSED(primNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowTdef(UNUSED(Node *node), UNUSED(tdefNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowPtr(UNUSED(Node *node), UNUSED(ptrNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowAdcl(UNUSED(Node *node), UNUSED(adclNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowFdcl(UNUSED(Node *node), UNUSED(fdclNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowSdcl(UNUSED(Node *node), UNUSED(sdclNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowUdcl(UNUSED(Node *node), UNUSED(udclNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowEdcl(UNUSED(Node *node), UNUSED(edclNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowDecl(Node *node, declNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlow(u->init, Entry(node, v)));
     else
	  return Entry(node, DataFlow(u->init, Exit(node, v)));
}

PRIVATE inline FlowValue DataFlowAttrib(UNUSED(Node *node), UNUSED(attribNode *u), FlowValue v)
{
     return v;
}

PRIVATE inline FlowValue DataFlowProc(Node *node, procNode *u, FlowValue v)
{
     /* Modified for local procedure declarations: We ensure that each
      * procedure (i.e. inlet) is data-flowed separately.  We do this
      * by restarting the dataflow analysis at Init to start the body
      * of the procedure, and make the proc appears as a NOP to the
      * code in which its declaration appears.  -KHR
      */
     if (Forw)
	  Exit(node,
	       FlowInto(&u->return_values,
			DataFlow(u->body,
				 Entry(node, Init))));
     else
	  Entry(node,
		FlowInto(&u->return_values,
			 DataFlow(u->body,
				  Exit(node, Init))));

     return v;			/* proc decl has no effect - it only causes subproc to
				 * be "dataflowed"
				 */
}

PRIVATE inline FlowValue DataFlowText(UNUSED(Node *node), UNUSED(textNode *u), FlowValue v)
{
     return v;
}

/*************************************************************************/
/*                                                                       */
/*                           GCC asm Extensions                          */
/*                                                                       */
/*************************************************************************/

PRIVATE inline FlowValue DataFlowAsm(Node *node, asmNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->input,
					 DataFlowSerialList(u->output,
						    DataFlow(u->template,
						      Entry(node, v)))));
     else
	  return Entry(node,
		       DataFlow(u->template,
				DataFlowSerialList(u->output,
					     DataFlowSerialList(u->input,
						       Exit(node, v)))));
}

PRIVATE inline FlowValue DataFlowAsmArg(Node *node, asmargNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlow(u->expr, Entry(node, v)));
     else
	  return Entry(node, DataFlow(u->expr, Exit(node, v)));
}

/*************************************************************************/
/*                                                                       */
/*                            GCC stdarg.h support                       */
/*                                                                       */
/*************************************************************************/
PRIVATE inline FlowValue DataFlowBuiltinVaArg(Node *node, builtinvaargNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node, DataFlow(u->expr, Entry(node, v)));
     else
	  return Entry(node, DataFlow(u->expr, Exit(node, v)));
}

/*************************************************************************/
/*                                                                       */
/*                           Extensions                                  */
/*                                                                       */
/*************************************************************************/

PRIVATE inline FlowValue DataFlowSpawn(Node *node, spawnNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->args,
					 DataFlow(u->name,
						  DataFlow(u->receiver,
						      Entry(node, v)))));
     else
	  return Entry(node,
		       DataFlow(u->receiver,
				DataFlow(u->name,
					 DataFlowSerialList(u->args,
						       Exit(node, v)))));
}

PRIVATE inline FlowValue DataFlowSync(Node *node, UNUSED(syncNode *u), FlowValue v)
{
     if (Forw)
	  return Exit(node, Entry(node, v));
     else
	  return Entry(node, Exit(node, v));
}

PRIVATE inline FlowValue DataFlowInletCall(Node *node, inletcallNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->args,
					 DataFlow(u->name,
						  Entry(node, v))));
     else
	  return Entry(node,
		       DataFlow(u->name,
				DataFlowSerialList(u->args,
						   Exit(node, v))));
}

PRIVATE inline FlowValue DataFlowAbort(Node *node, UNUSED(abortNode *u), FlowValue v)
{
     if (Forw)
	  return Exit(node, Entry(node, v));
     else
	  return Entry(node, Exit(node, v));
}

PRIVATE inline FlowValue DataFlowSynched(Node *node, UNUSED(synchedNode *u), FlowValue v)
{
     if (Forw)
	  return Exit(node, Entry(node, v));
     else
	  return Entry(node, Exit(node, v));
}


PRIVATE inline FlowValue DataFlowXBlock(Node *node, XBlockNode *u, FlowValue v)
{
     if (Forw)
	  return Exit(node,
		      DataFlowSerialList(u->stmts,
					 DataFlowSerialList(u->decl,
						       Entry(node, v))));
     else
	  return Entry(node,
		       DataFlowSerialList(u->decl,
					  DataFlowSerialList(u->stmts,
							Exit(node, v))));
}

/*************************************************************************/
/*                                                                       */
/*                  Computing flow through nodes and lists               */
/*                                                                       */
/*************************************************************************/

PRIVATE FlowValue DataFlow(Node *node, FlowValue v)
{
     if (node == NULL)
	  return v;

#define CODE(name, node, union) v = DataFlow##name(node, union, v);
     ASTSWITCH(node, CODE)
#undef CODE
	 return v;
}

PRIVATE FlowValue DataFlowSerialList(List *list, FlowValue v)
{
     if (list == NULL)
	  return v;

     if (Forw)
	  return DataFlowSerialList(Rest(list), DataFlow(FirstItem(list), v));
     else
	  return DataFlow(FirstItem(list), DataFlowSerialList(Rest(list), v));
}

PRIVATE inline FlowValue DataFlowBranch(Node *cond, Node *true, Node *false, FlowValue v)
{
     FlowValue c, t, f;
     if (Forw) {
	  c = DataFlow(cond, v);
	  t = DataFlow(true, c);
	  f = DataFlow(false, c);
	  return Meet(t, f);
     } else {
	  t = DataFlow(true, v);
	  f = DataFlow(false, v);
	  return DataFlow(cond, Meet(t, f));
     }
}

/*************************************************************************/
/*                                                                       */
/*                       Generic dataflow framework                      */
/*                                                                       */
/*************************************************************************/

PRIVATE Bool Changed;

/*
 *   InitConfluencePoints 
 */
PRIVATE void InitConfluencePoints(Node *node, UNUSED(void *ignore))
{
     switch (node->typ) {
	 case Label:
	      node->u.label.label_values.undefined = TRUE;
	      Changed = TRUE;
	      break;
	 case Switch:
	      node->u.Switch.switch_values.undefined = TRUE;
	      node->u.Switch.break_values.undefined = TRUE;
	      Changed = TRUE;
	      break;
	 case While:
	      node->u.While.loop_values.undefined = TRUE;
	      node->u.While.break_values.undefined = TRUE;
	      Changed = TRUE;
	      break;
	 case Do:
	      node->u.Do.loop_values.undefined = TRUE;
	      node->u.Do.continue_values.undefined = TRUE;
	      node->u.Do.break_values.undefined = TRUE;
	      Changed = TRUE;
	      break;
	 case For:
	      node->u.For.loop_values.undefined = TRUE;
	      node->u.For.continue_values.undefined = TRUE;
	      node->u.For.break_values.undefined = TRUE;
	      Changed = TRUE;
	      break;
	 case Proc:
	      node->u.proc.return_values.undefined = TRUE;
	      Changed = TRUE;
	      break;
	 default:
	      break;
     }
}

/*
 *   FlowInto meets v with a confluence point, dest.  
 *   returns new value of dest.
 */
PRIVATE FlowValue FlowInto(FlowValue *dest, FlowValue v)
{
     FlowValue new = Meet(*dest, v);
     if (!Equal(*dest, new)) {
	  *dest = new;
	  Changed = TRUE;
     }
     return new;
}

/*
 * IterateDataFlow 
 */

GLOBAL void IterateDataFlow(
				Node *root,	/* root node */
				FlowValue init,		/* input value for root node */
				Direction dir,	/* direction */
				MeetOp meet,	/* meet operation */
				EqualOp equal,	/* equality operation */
				TransOp trans	/* transfer function */
)
{
     Forw = (dir == Forwards);
     Meet = meet;
     Equal = equal;
     Trans = trans;
     Init = init;

     /* Initialize all confluence points */
     Final = FALSE;
     Changed = FALSE;
     WalkTree(root, InitConfluencePoints, NULL, Preorder);

     /* Iterate until confluence points are stable */
     while (Changed) {
	  Changed = FALSE;
	  DataFlow(root, init);
     }

     /* Make final pass to capture data-flow information in permanent form */
     Final = TRUE;
     DataFlow(root, init);
}
