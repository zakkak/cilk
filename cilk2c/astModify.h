/************************************************************************
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
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

#ifndef _AST_MODIFY_H
#define _AST_MODIFY_H

#include "ast.h"
#include "wildcardTable.h"

#define EQDEBUG FALSE

/*****************************************************
 *  Additional Macros for switching/traversing trees
 *   when there are two trees 
 *
 * DUAL_ASTSWITCH
 *   This function does a switch when we have two nodes
 *     which have the same type.
 *
 * Parameters:
 * n1, n2    The two nodes, n1 and n2.  They can not be NULL and 
 *             they have the same type. (n1->typ field)
 *             
 * CODE    #defined macro taking three parameters:
 * 
 * name       enumerated constant corresponding to node->typ
 * n1, n2       same nodes passed to DUAL_ASTSWITCH
 */

#define DUAL_ASTSWITCH(n1, n2, CODE) \
 switch(n1->typ) {  \
  case Const:\
    if (EQDEBUG) printf("Calling const switch. \n"); \
      {CODE(Const, &(n1->u.Const), &(n2->u.Const));} break; \
  case Id:\
    if (EQDEBUG) printf("Calling id switch. \n");  \
    {CODE(Id, &(n1->u.id), &(n2->u.id));} break; \
  case Binop:\
    if (EQDEBUG) printf("Calling binop switch. \n");  \
      {CODE(Binop, &(n1->u.binop), &(n2->u.binop));} break; \
  case Unary:\
    {CODE(Unary, &(n1->u.unary), &(n2->u.unary));} break; \
  case Cast:\
    {CODE(Cast, &(n1->u.cast), &(n2->u.cast));} break; \
  case Constructor: \
    {CODE(Constructor, &(n1->u.constructor), &(n2->u.constructor)); } \
    break; \
  case Comma:\
    {CODE(Comma, &(n1->u.comma), &(n2->u.comma));} break; \
  case Ternary:\
    {CODE(Ternary, &(n1->u.ternary), &(n2->u.ternary));} break; \
  case Array:\
    {CODE(Array, &(n1->u.array), &(n2->u.array));} break; \
  case Call:\
    {CODE(Call, &(n1->u.call), &(n2->u.call));} break; \
  case Initializer:\
    {CODE(Initializer, &(n1->u.initializer), &(n2->u.initializer));} break; \
  case ImplicitCast:\
    {CODE(ImplicitCast, &(n1->u.implicitcast), &(n2->u.implicitcast));} break; \
  case Label:\
    {CODE(Label, &(n1->u.label), &(n2->u.label));} break; \
  case Switch:\
    {CODE(Switch, &(n1->u.Switch), &(n2->u.Switch));} break; \
  case Case:\
    {CODE(Case, &(n1->u.Case), &(n2->u.Case));} break; \
  case Default:\
    {CODE(Default, &(n1->u.Default), &(n2->u.Default));} break; \
  case If:\
    {CODE(If, &(n1->u.If), &(n2->u.If));} break; \
  case IfElse:\
    {CODE(IfElse, &(n1->u.IfElse), &(n2->u.IfElse));} break; \
  case While:\
    {CODE(While, &(n1->u.While), &(n2->u.While));} break; \
  case Do:\
    {CODE(Do, &(n1->u.Do), &(n2->u.Do));} break; \
  case For:\
    {CODE(For, &(n1->u.For), &(n2->u.For));} break; \
  case Goto:\
    {CODE(Goto, &(n1->u.Goto), &(n2->u.Goto));} break; \
  case Continue:\
    {CODE(Continue, &(n1->u.Continue), &(n2->u.Continue));} break; \
  case Break:\
    {CODE(Break, &(n1->u.Break), &(n2->u.Break));} break; \
  case Return:\
    {CODE(Return, &(n1->u.Return), &(n2->u.Return));} break; \
  case Block:\
    {CODE(Block, &(n1->u.Block), &(n2->u.Block));} break; \
  case Prim:\
    {CODE(Prim, &(n1->u.prim), &(n2->u.prim));} break; \
  case Tdef:\
    {CODE(Tdef, &(n1->u.tdef), &(n2->u.tdef));} break; \
  case Ptr:\
    {CODE(Ptr, &(n1->u.ptr), &(n2->u.ptr));} break; \
  case Adcl:\
    {CODE(Adcl, &(n1->u.adcl), &(n2->u.adcl));} break; \
  case Fdcl:\
    {CODE(Fdcl, &(n1->u.fdcl), &(n2->u.fdcl));} break; \
  case Sdcl:\
    if (EQDEBUG) printf("Calling Sdcl switch. \n"); \
    {CODE(Sdcl, &(n1->u.sdcl), &(n2->u.sdcl));} break; \
  case Udcl:\
    {CODE(Udcl, &(n1->u.udcl), &(n2->u.udcl));} break; \
  case Edcl:\
    {CODE(Edcl, &(n1->u.edcl), &(n2->u.edcl));} break; \
  case Decl:\
    {CODE(Decl, &(n1->u.decl), &(n2->u.decl));} break; \
  case Attrib:\
    {CODE(Attrib, &(n1->u.attrib), &(n2->u.attrib));} break; \
  case Proc: \
    {CODE(Proc, &(n1->u.proc), &(n2->u.proc));} break; \
  case Text:\
    {CODE(Text, &(n1->u.text), &(n2->u.text));} break; \
  case Spawn:\
    {CODE(Spawn, &(n1->u.spawn), &(n2->u.spawn));} break; \
  case Sync:\
    {CODE(Sync, &(n1->u.sync), &(n2->u.sync));} break; \
  case InletCall:\
    {CODE(InletCall, &(n1->u.inletcall), &(n2->u.inletcall));} break; \
  case XBlock:\
    {CODE(XBlock, &(n1->u.xBlock), &(n2->u.xBlock));} break; \
  case Abort:\
    {CODE(Abort, &(n1->u.cilkabort), &(n2->u.cilkabort));} break; \
  case Synched:\
    {CODE(Synched, &(n1->u.synched), &(n2->u.synched));} break; \
  case Asm:\
    {CODE(Asm, &(n1->u.Asm), &(n2->u.Asm));} break; \
  case AsmArg:\
    {CODE(AsmArg, &(n1->u.AsmArg), &(n2->u.AsmArg));} break; \
  /* GCC builtin stdarg.h support */ \
  case BuiltinVaArg:  {CODE(BuiltinVaArg, &(n1->u.builtinvaarg), &(n2->u.builtinvaarg));} break; \
}

/*****************************************************
 *
 * DUAL_LIST_WALK
 *   This function walks along two list of nodes, l1 and l2,
 *    simultaneously. l1 and l2 typically have the 
 *    same number of elements.  The macro
 *    CODE is called on the corresponding nodes of
 *    l1 and l2
 *
 * Parameters:
 * l1, l2    The two lists, l1 and l2.
 *
 *             
 * CODE    #defined macro or function taking three parameters:
 *         This is the main code executed on corresponding nodes
 * 
 * n1, n2       The corresponding nodes of l1 and l2
 * answer       The variable where we can store the result
 *               Typically this is a pointer to an external
 *                structure.
 *
 * CONTINUE  #defined macro or function taking the same parameters
 *            as CODE.  This checks the current state of n1, n2, answer,
 *            and returns TRUE if we should continue walking down the list
 * 
 * FAILCODE  #defined macro or function taking one parameter, answer.
 *            This is executed when one of l1, l2 is NULL but not the other,
 *             or if we discover that l1 and l2 have different lengths
 *             and the nodes don't match up.
 *
 *
 * An example use for this macro would be to compare two lists to see
 *   if they are equal.  In this case, answer could be a *Bool
 *   which stores whether the lists so far are equal.
 *   CODE would be (*answer = (Equal(n1, n2) && *answer))
 *   CONTINUE would be (return *answer)
 *   FAILCODE would be (*answer = FALSE;)
 *
 *   *answer would need to be initialized as TRUE
 */
#define DUAL_LIST_WALK(l1, l2, answer, CODE, CONTINUE, FAILCODE) \
{ ListMarker _listwalk_marker1, _listwalk_marker2; \
  Node *_listwalk_ref1=0; Node *_listwalk_ref2=0; /* shut up the mipspro compiler warnings */ \
 if (((l1 == NULL) && (l2 != NULL))               \
     || ((l1 != NULL) && (l2 == NULL))) {          \
    {FAILCODE(answer);}                         \
 }                                               \
 else {                                          \
   IterateList(&_listwalk_marker1, l1);          \
   IterateList(&_listwalk_marker2, l2);          \
   while ( (!EndOfList(&_listwalk_marker1)) && \
           (!EndOfList(&_listwalk_marker2)) && \
           (CONTINUE(_listwalk_ref1, _listwalk_ref2, answer))) {             \
       NextOnList(&_listwalk_marker1, (GenericREF)&_listwalk_ref1); \
       NextOnList(&_listwalk_marker2, (GenericREF)&_listwalk_ref2); \
     if (_listwalk_ref1 &&  _listwalk_ref2) {                         \
          {CODE(_listwalk_ref1, _listwalk_ref2, answer);}  \
     } \
     SetCurrentOnList(&_listwalk_marker1, (Generic *)_listwalk_ref1);  \
     SetCurrentOnList(&_listwalk_marker2, (Generic *)_listwalk_ref2);  \
   }  \
   if (!(EndOfList(&_listwalk_marker1) && EndOfList(&_listwalk_marker2))) {\
    {FAILCODE(answer);}       \
   }\
 } \
}   





/**************************************************************
 *
 * DUAL_AST_WALK
 *   This function walks along the two syntax trees, rooted at
 *    n1 and n2, simultaneously.
 *    n1 and n2 must be the same type of node.
 *    CODE is called on the corresponding children of n1 and n2
 *
 *  This macro works as DUAL_LIST_WALK does, only for a tree
 *  The parameters it takes are similar, except it takes nodes
 *   n1 and n2 instead of lists l1 and l2.
 *
 */
#define DUAL_AST_WALK(n1, n2, answer, CODE, CONTINUE, FAILCODE) \
{ \
  switch ((n1)->typ) {\
     case Const:    {CODE((n1)->u.Const.type, (n2)->u.Const.type, answer);} break; \
     case Id:       break; \
     case Binop:    {CODE((n1)->u.binop.left, (n2)->u.binop.left, answer);} \
                    if (CONTINUE((n1)->u.binop.left, (n2)->u.binop.right, answer))  {CODE((n1)->u.binop.right, (n2)->u.binop.right, answer);} \
                    break;  \
     case Unary:    {CODE((n1)->u.unary.expr, (n2)->u.unary.expr, answer);} break; \
     case Cast:     {CODE((n1)->u.cast.type, (n2)->u.cast.type, answer);}  \
                    if (CONTINUE((n1)->u.cast.type, (n2)->u.cast.type, answer)) \
                      {CODE((n1)->u.cast.expr, (n2)->u.cast.expr, answer);} \
                    break; \
     case Constructor: {CODE((n1)->u.constructor.type, (n2)->u.constructor.type, answer); \
                        if (CONTINUE((n1)->u.constructor.type, \
                                     (n2)->u.constructor.type, answer))  \
                          { \
                           CODE((n1)->u.constructor.initializerlist,  \
                                (n2)->u.constructor.initializerlist, answer);  \
                          } \
                       } break; \
     case Comma:    {DUAL_LIST_WALK((n1)->u.comma.exprs, (n2)->u.comma.exprs, answer, CODE, CONTINUE, FAILCODE);} break;  \
     case Ternary:  {CODE((n1)->u.ternary.cond, (n2)->u.ternary.cond, answer);}  \
                    if (CONTINUE((n1)->u.ternary.cond, (n2)->u.ternary.cond, answer)) \
                       {CODE((n1)->u.ternary.true, (n2)->u.ternary.true, answer);} \
                    if (CONTINUE((n1)->u.ternary.true, (n2)->u.ternary.true, answer)) \
                       {CODE((n1)->u.ternary.false, (n2)->u.ternary.false, answer);} \
                    break; \
     case Array:    {CODE((n1)->u.array.name, (n2)->u.array.name, answer);} \
		    if (CONTINUE((n1)->u.array.name, (n2)->u.array.name, answer))  \
                      {DUAL_LIST_WALK((n1)->u.array.dims, (n2)->u.array.dims, answer, CODE, CONTINUE, FAILCODE);} \
                    break; \
     case Call:     {CODE((n1)->u.call.name, (n2)->u.call.name, answer);}  \
                    if (CONTINUE((n1)->u.call.name, (n2)->u.call.name, answer)) \
                      {DUAL_LIST_WALK((n1)->u.call.args, (n2)->u.call.args, answer, CODE, CONTINUE, FAILCODE);}  \
                    break; \
     case Initializer:   {DUAL_LIST_WALK((n1)->u.initializer.exprs, (n2)->u.initializer.exprs, answer, CODE, CONTINUE, FAILCODE);} break; \
     case ImplicitCast:  {CODE((n1)->u.implicitcast.expr, (n2)->u.implicitcast.expr, answer);} break;  \
     case Label:         break;  \
     case Switch:        {CODE((n1)->u.Switch.expr, (n2)->u.Switch.expr, answer);} \
                         if (CONTINUE((n1)->u.Switch.expr, (n2)->u.Switch.expr, answer)) \
                            {CODE((n1)->u.Switch.stmt, (n2)->u.Switch.stmt, answer);}  \
                         break; \
     case Case:          {CODE((n1)->u.Case.expr, (n2)->u.Case.expr, answer);} \
                         break; \
     case Default:       break; \
     case If:            {CODE((n1)->u.If.expr, (n2)->u.If.expr, answer);} \
                         if (CONTINUE((n1)->u.If.expr, (n2)->u.If.expr, answer))  {CODE((n1)->u.If.stmt, (n2)->u.If.stmt, answer);} \
                         break; \
     case IfElse:        {CODE((n1)->u.IfElse.expr, (n2)->u.IfElse.expr, answer);} \
                         if (CONTINUE((n1)->u.IfElse.expr, (n2)->u.IfElse.expr, answer)) \
                            {CODE((n1)->u.IfElse.true, (n2)->u.IfElse.true, answer);}  \
                         if (CONTINUE((n1)->u.IfElse.true, (n2)->u.IfElse.true, answer)) \
                            {CODE((n1)->u.IfElse.false, (n2)->u.IfElse.false, answer);}  \
                         break; \
     case While:         {CODE((n1)->u.While.expr, (n2)->u.While.expr, answer);} \
                         if (CONTINUE((n1)->u.While.expr, (n2)->u.While.expr, answer)) \
                           {CODE((n1)->u.While.stmt, (n2)->u.While.stmt, answer);} \
                         break; \
     case Do:            {CODE((n1)->u.Do.stmt, (n2)->u.Do.stmt, answer);} \
                         if (CONTINUE((n1)->u.Do.stmt, (n2)->u.Do.stmt, answer))  \
                            {CODE((n1)->u.Do.expr, (n2)->u.Do.expr, answer);}  \
                         break; \
     case For:           {CODE((n1)->u.For.init, (n2)->u.For.init, answer); }  \
	     	         if (CONTINUE((n1)->u.For.init, (n2)->u.For.init, answer))  \
                           {CODE((n1)->u.For.cond, (n2)->u.For.cond, answer);}  \
                         if (CONTINUE((n1)->u.For.cond, (n2)->u.For.cond, answer))  \
                           {CODE((n1)->u.For.next, (n2)->u.For.next, answer);}     \
                         if (CONTINUE((n1)->u.For.next, (n2)->u.For.next, answer))  \
                           {CODE((n1)->u.For.stmt, (n2)->u.For.stmt, answer);}  \
                         break; \
     case Goto:          break; \
     case Continue:      break; \
     case Break:         break; \
     case Return:        {CODE((n1)->u.Return.expr, (n2)->u.Return.expr, answer);} break; \
     case Block:         if (DUALWALKDEBUG) printf("Taking block branch. \n"); \
                          {DUAL_LIST_WALK((n1)->u.Block.decl, (n2)->u.Block.decl, answer, CODE, CONTINUE, FAILCODE);} \
                          {DUAL_LIST_WALK((n1)->u.Block.stmts, (n2)->u.Block.stmts, answer, CODE, CONTINUE, FAILCODE);} break; \
     case Prim:          break; \
     case Tdef:          break; \
     case Ptr:           {CODE((n1)->u.ptr.type, (n2)->u.ptr.type, answer);} break; \
     case Adcl:          {CODE((n1)->u.adcl.type, (n2)->u.adcl.type, answer);} \
                         if (CONTINUE((n1)->u.adcl.dimp->dim, (n2)->u.adcl.dimp->dim, answer)) \
                            {CODE((n1)->u.adcl.dimp->dim, (n2)->u.adcl.dimp->dim, answer);} \
                         break; \
     case Fdcl:          if (DUALWALKDEBUG) printf("Taking Fdecl branch. \n"); \
                         {DUAL_LIST_WALK((n1)->u.fdcl.args, (n2)->u.fdcl.args, answer, CODE, CONTINUE, FAILCODE);} \
                         {CODE((n1)->u.fdcl.returns, (n2)->u.fdcl.returns, answer);} \
                         break; \
     case Sdcl:          if (DUALWALKDEBUG) printf("Taking Sdcl branch... \n"); \
                         if (tq_has_sue_elaborated((n1)->u.sdcl.tq) && tq_has_sue_elaborated((n2)->u.sdcl.tq)) \
                          {DUAL_LIST_WALK((n1)->u.sdcl.type->fields, (n2)->u.sdcl.type->fields, answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Udcl:          if (DUALWALKDEBUG) printf("Taking Udcl branch... \n"); \
                         if (tq_has_sue_elaborated((n1)->u.udcl.tq) && tq_has_sue_elaborated((n2)->u.udcl.tq)) \
                           {DUAL_LIST_WALK((n1)->u.udcl.type->fields, (n2)->u.udcl.type->fields, answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Edcl:          if (DUALWALKDEBUG) printf("Taking Edcl branch... \n"); \
                         if (tq_has_sue_elaborated((n1)->u.edcl.tq) && tq_has_sue_elaborated((n2)->u.edcl.tq)) \
                           {DUAL_LIST_WALK((n1)->u.edcl.type->fields, (n2)->u.edcl.type->fields, answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Asm:	         {CODE((n1)->u.Asm.template, (n2)->u.Asm.template, answer);} \
                         if (CONTINUE((n1)->u.Asm.template, (n2)->u.Asm.template, answer)) { \
                           {DUAL_LIST_WALK((n1)->u.Asm.output, (n2)->u.Asm.output, answer, CODE, CONTINUE, FAILCODE);} \
                           {DUAL_LIST_WALK((n1)->u.Asm.input, (n2)->u.Asm.input, answer, CODE, CONTINUE, FAILCODE);} \
                           {DUAL_LIST_WALK((n1)->u.Asm.clobbered, (n2)->u.Asm.clobbered, answer, CODE, CONTINUE, FAILCODE);} \
                         } break; \
     case AsmArg:        {CODE((n1)->u.AsmArg.constraint, (n2)->u.AsmArg.constraint, answer);} \
                         if (CONTINUE((n1)->u.AsmArg.constraint, (n2)->u.AsmArg.constraint, answer)) \
                           {CODE((n1)->u.AsmArg.expr, (n2)->u.AsmArg.expr, answer);} \
                         break; \
     case BuiltinVaArg:  {CODE((n1)->u.builtinvaarg.expr, (n2)->u.builtinvaarg.expr, answer);} \
                         if (CONTINUE((n1)->u.builtinvaarg.expr, (n2)->u.builtinvaarg.expr, answer)) \
                           {CODE((n1)->u.builtinvaarg.type, (n2)->u.builtinvaarg.type, answer);} \
                         break; \
     case Spawn:         {CODE((n1)->u.spawn.receiver, (n2)->u.spawn.receiver, answer);} \
                         if (CONTINUE((n1)->u.spawn.receiver, (n2)->u.spawn.receiver, answer)) \
                            {CODE((n1)->u.spawn.name, (n2)->u.spawn.name, answer);} \
                         if (CONTINUE((n1)->u.spawn.name, (n2)->u.spawn.name, answer))  \
                            {DUAL_LIST_WALK((n1)->u.spawn.args, (n2)->u.spawn.args, answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Sync:          break; \
     case InletCall:     {CODE((n1)->u.inletcall.name, (n2)->u.inletcall.name, answer);}  \
                         if (CONTINUE((n1)->u.inletcall.name, (n2)->u.inletcall.name, answer)) \
                           {DUAL_LIST_WALK((n1)->u.inletcall.args, (n2)->u.inletcall.args, answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Abort:         break; \
     case Synched:       break; \
     case XBlock:         if (DUALWALKDEBUG) printf("Taking block branch. \n"); \
                          {DUAL_LIST_WALK((n1)->u.xBlock.decl, (n2)->u.xBlock.decl, answer, CODE, CONTINUE, FAILCODE);} \
                          {DUAL_LIST_WALK((n1)->u.xBlock.stmts, (n2)->u.xBlock.stmts, answer, CODE, CONTINUE, FAILCODE);} break; \
     case Decl:         {CODE((n1)->u.decl.type, (n2)->u.decl.type, answer);} \
                         if (CONTINUE((n1)->u.decl.type, (n2)->u.decl.type, answer)) {\
                           {CODE((n1)->u.decl.init, (n2)->u.decl.init, answer);}  \
                           if (CONTINUE((n1)->u.decl.init, (n2)->u.decl.init, answer)) {\
                              {CODE((n1)->u.decl.bitsize, (n2)->u.decl.bitsize, answer);} \
			   } \
                         } \
                         break; \
     case Attrib:        {DUAL_LIST_WALK((n1)->u.attrib.arglist, (n2)->u.attrib.arglist, answer, CODE, CONTINUE, FAILCODE);}  break; \
     case Proc:          if (DUALWALKDEBUG) printf("Taking Proc  branch... \n"); \
                         {CODE((n1)->u.proc.decl, (n2)->u.proc.decl, answer);}  \
                         if (CONTINUE((n1)->u.proc.decl, (n2)->u.proc.decl, answer)) \
                           {CODE((n1)->u.proc.body, (n2)->u.proc.body, answer);} \
                         break; \
     case Text:          break; \
     default:            printf("Taking default??? \n"); FAIL("Unrecognized node type"); break; \
   } \
}

/*****************************************************
 *
 * LISTWALKTWO
 *   This function is similar to LISTWALK, except
 *    that it takes extra parameters.
 *
 *    It walks along a list l1, and takes in a node n2,
 *    storing the result in answer.
 *
 *    We keep scanning down the list if CONTINUE returns
 *      true.
 *    FAILCODE is executed if l1 is ever NULL.

 * An example use for this macro would be to scan a list to see if
 *   it contains a particular node.  
 *   In this case, answer could be a *Bool
 *   which is FALSE if the node hasn't been found.  
 *   CODE would be (*answer = (Equal(n1, n2) || *answer))
 *   CONTINUE would be (return !(*answer))
 *   FAILCODE would be (*answer = FALSE;) (or empty)
 *
 *   *answer would be initialized as FALSE 
 */

#define LISTWALKTWO(list1, node2, answer, CODE, CONTINUE, FAILCODE) \
{ ListMarker _listwalk_marker; \
  Node *_listwalk_ref=0;  /* initialize to 0 to shut up compiler warnings about possibly uninitialized variables.  The MIPSPRO compiler seems not to like it. */  \
 if (list1 == NULL) {             \
     {FAILCODE(answer);}                           \
 }                                               \
 else {                                          \
   IterateList(&_listwalk_marker, list1);          \
   while ( (!EndOfList(&_listwalk_marker)) && \
           (CONTINUE(_listwalk_ref, node2, answer))) {             \
       NextOnList(&_listwalk_marker, (GenericREF)&_listwalk_ref); \
     if (_listwalk_ref ) {                         \
          {CODE(_listwalk_ref, node2, answer);}  \
     } \
     SetCurrentOnList(&_listwalk_marker, (Generic *)_listwalk_ref);  \
   }  \
 } \
}   

/* This macro walks along the tree rooted at n1, using the
   node n2 in CODE.  See LISTWALKTWO for description. */
#define ASTWALKTWO(n1, n2, answer, CODE, CONTINUE, FAILCODE) \
{\
  switch ((n1)->typ) {\
     case Const:    {CODE((n1)->u.Const.type, (n2), answer);} break; \
     case Id:       if (EQDEBUG) printf("Checking ID?? \n"); break; \
     case Binop:    {CODE((n1)->u.binop.left, (n2), answer);} \
                    if (CONTINUE((n1)->u.binop.left, (n2), answer))  {CODE((n1)->u.binop.right, (n2), answer);} \
                    break;  \
     case Unary:    if (EQDEBUG) printf("Taking unary branch. \n"); \
                    {CODE((n1)->u.unary.expr, (n2), answer);} break; \
     case Cast:     {CODE((n1)->u.cast.type, (n2), answer);}  \
                    if (CONTINUE((n1)->u.cast.type, (n2), answer)) {CODE((n1)->u.cast.expr, (n2), answer);}  break; \
     case Comma:    {LISTWALKTWO((n1)->u.comma.exprs, (n2), answer, CODE, CONTINUE, FAILCODE);} break;  \
     case Ternary:  {CODE((n1)->u.ternary.cond, (n2), answer);}  \
                    if (CONTINUE((n1)->u.ternary.cond, (n2), answer)) \
                       {CODE((n1)->u.ternary.true, (n2), answer);} \
                    if (CONTINUE((n1)->u.ternary.true, (n2), answer)) \
                       {CODE((n1)->u.ternary.false, (n2), answer);} \
                    break; \
     case Array:    {CODE((n1)->u.array.name, (n2), answer);} \
		    if (CONTINUE((n1)->u.array.name, (n2), answer))  \
                      {LISTWALKTWO((n1)->u.array.dims, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                    break; \
     case Call:     {CODE((n1)->u.call.name, (n2), answer);}  \
                    if (CONTINUE((n1)->u.call.name, (n2), answer)) \
                      {LISTWALKTWO((n1)->u.call.args, (n2), answer, CODE, CONTINUE, FAILCODE);}  \
                    break; \
     case Initializer:   {LISTWALKTWO((n1)->u.initializer.exprs, (n2), answer, CODE, CONTINUE, FAILCODE);} break; \
     case ImplicitCast:  {CODE((n1)->u.implicitcast.expr, (n2), answer);} break;  \
     case Label:         break;  \
     case Switch:        {CODE((n1)->u.Switch.expr, (n2), answer);} \
                         if (CONTINUE((n1)->u.Switch.expr, (n2), answer)) \
                            {CODE((n1)->u.Switch.stmt, (n2), answer);}  \
                         break; \
     case Case:          {CODE((n1)->u.Case.expr, (n2), answer);} \
                         break; \
     case Default:       break; \
     case If:            {CODE((n1)->u.If.expr, (n2), answer);} \
                         if (CONTINUE((n1)->u.If.expr, (n2), answer))  {CODE((n1)->u.If.stmt, (n2), answer);} \
                         break; \
     case IfElse:        {CODE((n1)->u.IfElse.expr, (n2), answer);} \
                         if (CONTINUE((n1)->u.IfElse.expr, (n2), answer)) \
                            {CODE((n1)->u.IfElse.true, (n2), answer);}  \
                         if (CONTINUE((n1)->u.IfElse.true, (n2), answer)) \
                            {CODE((n1)->u.IfElse.false, (n2), answer);}  \
                         break; \
     case While:         {CODE((n1)->u.While.expr, (n2), answer);} \
                         if (CONTINUE((n1)->u.While.expr, (n2), answer)) \
                           {CODE((n1)->u.While.stmt, (n2), answer);} \
                         break; \
     case Do:            {CODE((n1)->u.Do.stmt, (n2), answer);} \
                         if (CONTINUE((n1)->u.Do.stmt, (n2), answer))  \
                            {CODE((n1)->u.Do.expr, (n2), answer);}  \
                         break; \
     case For:           {CODE((n1)->u.For.init, (n2), answer); }  \
	     	         if (CONTINUE((n1)->u.For.init, (n2), answer))  \
                           {CODE((n1)->u.For.cond, (n2), answer);}  \
                         if (CONTINUE((n1)->u.For.cond, (n2), answer))  \
                           {CODE((n1)->u.For.next, (n2), answer);}     \
                         if (CONTINUE((n1)->u.For.next, (n2), answer))  \
                           {CODE((n1)->u.For.stmt, (n2), answer);}  \
                         break; \
     case Goto:          break; \
     case Continue:      break; \
     case Break:         break; \
     case Return:        {CODE((n1)->u.Return.expr, (n2), answer);} break; \
     case Block:         {LISTWALKTWO((n1)->u.Block.decl, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         {LISTWALKTWO((n1)->u.Block.stmts, (n2), answer, CODE, CONTINUE, FAILCODE);} break; \
     case Prim:          break; \
     case Tdef:          break; \
     case Ptr:           {CODE((n1)->u.ptr.type, (n2), answer);} break; \
     case Adcl:          {CODE((n1)->u.adcl.type, (n2), answer);} \
                         if (CONTINUE((n1)->u.adcl.dimp->dim, (n2), answer)) \
                            {CODE((n1)->u.adcl.dimp->dim, (n2), answer);} \
                         break; \
     case Fdcl:          {LISTWALKTWO((n1)->u.fdcl.args, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         {CODE((n1)->u.fdcl.returns, (n2), answer);} \
                         break; \
     case Sdcl:          if (tq_has_sue_elaborated((n1)->u.sdcl.tq))  \
                          {LISTWALKTWO((n1)->u.sdcl.type->fields, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Udcl:          if (tq_has_sue_elaborated((n1)->u.udcl.tq)) \
                           {LISTWALKTWO((n1)->u.udcl.type->fields, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Edcl:          if (tq_has_sue_elaborated((n1)->u.edcl.tq)) \
                           {LISTWALKTWO((n1)->u.edcl.type->fields, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Asm:	         {CODE((n1)->u.Asm.template, (n2), answer);} \
                         if (CONTINUE((n1)->u.Asm.template, (n2), answer)) { \
                           {LISTWALKTWO((n1)->u.Asm.output, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                           {LISTWALKTWO((n1)->u.Asm.input, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                           {LISTWALKTWO((n1)->u.Asm.clobbered, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         } break; \
     case AsmArg:        {CODE((n1)->u.AsmArg.constraint, (n2), answer);} \
                         if (CONTINUE((n1)->u.AsmArg.constraint, (n2), answer)) \
                           {CODE((n1)->u.AsmArg.expr, (n2), answer);} \
                         break; \
     case Spawn:         {CODE((n1)->u.spawn.receiver, (n2), answer);} \
                         if (CONTINUE((n1)->u.spawn.receiver, (n2), answer)) \
                            {CODE((n1)->u.spawn.name, (n2), answer);} \
                         if (CONTINUE((n1)->u.spawn.name, (n2), answer))  \
                            {LISTWALKTWO((n1)->u.spawn.args, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Sync:          break; \
     case InletCall:     {CODE((n1)->u.inletcall.name, (n2), answer);}  \
                         if (CONTINUE((n1)->u.inletcall.name, (n2), answer)) \
                           {LISTWALKTWO((n1)->u.inletcall.args, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         break; \
     case Abort:         break; \
     case Synched:       break; \
     case XBlock:         {LISTWALKTWO((n1)->u.xBlock.decl, (n2), answer, CODE, CONTINUE, FAILCODE);} \
                         {LISTWALKTWO((n1)->u.xBlock.stmts, (n2), answer, CODE, CONTINUE, FAILCODE);} break; \
     case Decl:          {CODE((n1)->u.decl.type, (n2), answer);} \
                         if (CONTINUE((n1)->u.decl.type, (n2), answer)) \
                           {CODE((n1)->u.decl.init, (n2), answer);}  \
                         if (CONTINUE((n1)->u.decl.init, (n2), answer)) \
                           {CODE((n1)->u.decl.bitsize, (n2), answer);} \
                         break; \
     case Attrib:        {LISTWALKTWO((n1)->u.attrib.arglist, (n2), answer, CODE, CONTINUE, FAILCODE);}  break; \
     case Proc:          {CODE((n1)->u.proc.decl, (n2), answer);}  \
                         if (CONTINUE((n1)->u.proc.decl, (n2), answer)) \
                           {CODE((n1)->u.proc.body, (n2), answer);} \
                         break; \
     case Text:          break; \
     default:            FAIL("Unrecognized node type"); break; \
   } \
}


/*  Does the same thing as NodeCopy(n, SubtreeExact); */
GLOBAL Node* CopyAST(Node *n);


/* Replaces the node N1 points to with a copy of 
 * the tree N2 points to.  Requires that N1 and
 *  N2 are both not NULL */
GLOBAL void ReplaceAST(Node *n1, Node *n2);


/* Takes in PATTERN_TREE, an AST with wildcards, and a WildcardTable.
 * The tree is traversed, and the wildcards are filled in using
 *  values stored in the table.
 *
 *
 */
GLOBAL void FillWildcardsFromTable(Node *patternTree, WildcardTable *wcTable);



#endif /*_AST_MODIFY_H */






