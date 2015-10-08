
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
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

#ifndef _TYPE_H_
#define _TYPE_H_
FILE_IDENTITY(type_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/type.h $ $LastChangedBy: bradley $ $Rev: 2743 $ $Date: 2006-02-12 13:34:00 -0500 (Sun, 12 Feb 2006) $");

GLOBAL extern Node *EllipsisNode;	/* represents '...'  primitive type */

/* A declarator has several context-dependent scoping options: */
typedef enum {
     Redecl,			/* may redeclare a typdef name */
     NoRedecl,			/* may not redeclare a typedef name */
     SU,			/* is a structure or union field */
     Formal			/* is a formal parameter, goes into scope of the following body
				 * (storage class NOT extern, static, typedef, auto) */
} ScopeState;

/* Global Type constants */
extern Node *PrimVoid, *PrimChar, *PrimSchar, *PrimUchar, *PrimSshort,
*PrimUshort, *PrimSint, *PrimUint, *PrimSlong, *PrimUlong, *PrimSlonglong,
*PrimUlonglong, *PrimFloat, *PrimDouble, *PrimLongdouble;

extern Node *StaticString;

extern Node *SintZero, *UintZero, *SlongZero, *UlongZero, *FloatZero,
*DoubleZero;

extern Node *PtrVoid;
extern Node *PtrNull;

extern Node *SintOne, *UintOne, *SlongOne, *UlongOne, *FloatOne, *DoubleOne;

/* Global flags (yuck) */

#if 0
/* no longer necessary -- now parser detects whether '{' begins a create
 * scope by itself */
GLOBAL extern int NoNewScope;	/* type.c */
    /* This determines whether or not a '{' begins a create scope.
     * NoNewScope > 0 in the following cases:
     * struct { ... }
     * union { ... }
     * enum { ... }
     * array initializers:  = { a, b, ... }
     */
#endif

GLOBAL extern Bool OldStyleFunctionDefinition;	/* procedure.c */
    /* The parameter declarations of an old-style function definition
     * must be scoped into the body of function, not their current scope.
     * This flag tracks this condition; it is TRUE from the end of the
     * argument list (which is all identifiers) until the next '{', which
     * starts the body. */

/* Global Variables from type.c */
GLOBAL extern Node *Undeclared;	/* used for undeclared identifiers */

/* Global Procedures from type.c */
GLOBAL void InitTypes(void);

GLOBAL Node *TypeQualifyNode(Node *node, TypeQual tq, Coord *tq_coord);
GLOBAL TypeQual MergeTypeQuals(TypeQual left, TypeQual right, Coord coord);
GLOBAL Node *MakeMergedType(Node *type1, Node *qual1);
GLOBAL TypeQual NodeTq(Node *node);
GLOBAL void NodeUpdateTq(Node *node, TypeQual(*f)(TypeQual));
GLOBAL void NodeUpdateTq2(Node *node, TypeQual(*f)(TypeQual, TypeQual), TypeQual tq2);
GLOBAL TypeQual NodeTypeQuals(Node *node);
GLOBAL TypeQual NodeDeclTq (Node *node); /* Return all the typequals on a decl (error if it is not a decl) */
/*GLOBAL TypeQual NodeDeclQuals(Node *node);*/
GLOBAL TypeQual NodeStorageClass(Node *node);
GLOBAL void NodeSetStorageClass(Node *node, TypeQual sc);
GLOBAL TypeQual NodeDeclLocation(Node *node);
GLOBAL void NodeSetDeclLocation(Node *node, TypeQual dl);
GLOBAL void TQtoText(char array[], int arraysize, TypeQual tq); /* TQtoText is only used to produce error messages.  Hence no need for a full OutputContext. */
GLOBAL void PrintTQ(GBUF *out, TypeQual tq, int offset, Bool norecurse); /* PrintTQ is only used to produce diagnostics, and we don't really need a full OutputContext */

GLOBAL Node *StartPrimType(BasicType basic, Coord coord);
GLOBAL Node *MergePrimTypes(Node *node, Node *node2);
GLOBAL Node *FinishPrimType(Node *PrimNode);
GLOBAL Node *MakeDefaultPrimType(TypeQual tq, Coord coord);
GLOBAL Node *SansSign(Node *type);

/* return the number of characters printed, and if any are printed and lastc!=NULL, set *lastc to the last character sent. */
GLOBAL int PrintPrimType(GBUF *gb, Node *type, char *lastc);


GLOBAL Node *LookupFunction(Node *call);
GLOBAL Node *LookupPostfixExpression(Node *post);
GLOBAL Bool IsAType(const char *name);
GLOBAL Node *GetTypedefType(Node *id);
GLOBAL void OutOfScope(Node *var);

GLOBAL unsigned int NodeSizeof(Node *node, Node *node_type, 
			       Bool allow_unsized_array);
GLOBAL int NodeAlignment(Node *node, Node *node_type);

GLOBAL Node *ArrayRefType(Node *atype, List *dims);
GLOBAL Bool IsObjectType(Node *node);
GLOBAL Bool IsFunctionType(Node *node);
GLOBAL Bool IsIncompleteType(Node *node);
GLOBAL Bool IsVoidType(Node *node);
GLOBAL Bool IsArrayType(Node *node);
GLOBAL Bool IsSueType(Node *node);
GLOBAL Bool IsStructType(Node *node);
GLOBAL Bool IsUnionType(Node *node);
GLOBAL Bool IsEnumType(Node *node);
GLOBAL Bool IsPointerType(Node *node);
GLOBAL Bool IsScalarType(Node *node);
GLOBAL Bool IsArithmeticType(Node *node);
GLOBAL Bool IsIntegralType(Node *node);
GLOBAL Bool IsFloatingType(Node *node);
GLOBAL Bool IsLvalue(Node *node);
GLOBAL Bool IsModifiableLvalue(Node *node);
GLOBAL Bool IsVoidArglist(List *arglist);
GLOBAL Bool IsEllipsis(Node *node);
GLOBAL Bool IsRelationalExpression(Node *node);
GLOBAL Bool IsLogicalExpression(Node *node);
GLOBAL Bool IsLogicalOrRelationalExpression(Node *node);
GLOBAL Bool IsPtrToObject(Node *node);
GLOBAL Bool IsPtrToVoid(Node *node);
GLOBAL Bool IsPtrToFunction(Node *node);
GLOBAL Node *ArrayType(Node *array);
GLOBAL Bool IsStructType(Node *node);
GLOBAL Node *PtrSubtype(Node *node);
GLOBAL Bool DeclIsExtern(Node *node);
GLOBAL Bool DeclIsEnumConst(Node *node);
GLOBAL Bool DeclIsTypedef(Node *node);
GLOBAL Bool DeclIsBlock(Node *node);
GLOBAL Bool NodeIsConstQual(Node *node);
GLOBAL Bool NodeIsCilkProcedure(Node *node);
GLOBAL Bool IsUnsizedArray(Node *node);
GLOBAL Bool IsStructComplete(Node *node);
GLOBAL Bool IsUnionComplete(Node *node);
GLOBAL Bool TypeIsChar(Node *type);
GLOBAL Bool TypeIsSint(Node *type);
GLOBAL Bool TypeIsUint(Node *type);
GLOBAL Bool TypeIsSlong(Node *type);
GLOBAL Bool TypeIsUlong(Node *type);
GLOBAL Bool TypeIsFloat(Node *type);
GLOBAL Bool TypeIsDouble(Node *type);
GLOBAL Bool TypeIsString(Node *type);
GLOBAL Bool NodeTypeIsChar(Node *node);
GLOBAL Bool NodeTypeIsSint(Node *node);
GLOBAL Bool NodeTypeIsUint(Node *node);
GLOBAL Bool NodeTypeIsSlong(Node *node);
GLOBAL Bool NodeTypeIsUlong(Node *node);
GLOBAL Bool NodeTypeIsFloat(Node *node);
GLOBAL Bool NodeTypeIsDouble(Node *node);
GLOBAL Bool NodeTypeIsString(Node *node);
GLOBAL Bool NodeTypeIsIntegral(Node *node);
GLOBAL Bool NodeTypeIsUnsizedArray(Node *node);
GLOBAL Bool IsPrimChar(Node *node);
GLOBAL Bool IsPrimCharModuloSign(Node *node);
GLOBAL Bool IsArrayOfChar(Node *node);
GLOBAL Bool IsArrayOfCharModuloSign(Node *node);
GLOBAL Bool IsStringConstant(Node *node);
GLOBAL Bool IsAggregateType(Node *node);
GLOBAL Bool DeclIsStatic(Node *node);
GLOBAL Bool DeclIsExternal(Node *node);
GLOBAL Node *FunctionReturnType(Node *node);
GLOBAL void FunctionSetReturnType(Node *node, Node *new_type);
GLOBAL void FunctionPushArglist(Node *node, Node *new_arg);
GLOBAL Bool IsPrimitiveStmt(Node *node);

GLOBAL Bool TypeEqual(Node *type1, Node *type2);
GLOBAL Bool TypeEqualFormals(Node *type1, Node *type2);
GLOBAL Bool TypeEqualQualified(Node *type1, Node *type2, Bool strict_toplevel, Bool strict_recursive);

/* Global Procedures from complex-types.c */

GLOBAL Node *SetBaseType(Node *complex, Node *base);
GLOBAL Node *GetShallowBaseType(Node *complex);
GLOBAL Node *GetDeepBaseType(Node *complex);
GLOBAL Bool  DeepBaseTypeIsComplete(Node *complex);
GLOBAL Node *ExtendArray(Node *array, Node *dim, Coord coord);
GLOBAL Node *AddArrayDimension(Node *array, Node *dim);
GLOBAL Node *ModifyDeclType(Node *decl, Node *modifier);
GLOBAL Node *SetDeclType(Node *decl, Node *type, ScopeState redeclare);
GLOBAL Node *SetDeclInit(Node *decl, Node *init);
GLOBAL Node *SetDeclBitSize(Node *decl, Node *bitsize);
GLOBAL Node *SetDeclAttribs(Node *decl, TypeQual);
GLOBAL Node *SetDeclAsm(Node *decl, Node *asmnode);
GLOBAL List *AppendDecl(List *list, Node *decl, ScopeState redeclare);
GLOBAL Node *FinishType(Node *type);
GLOBAL Node *FinishDecl(Node *type);

/* Global procedures from sue.c */

GLOBAL void PrintSUE(GBUF *out, SUEtype *sue, int offset, Bool norecurse);
GLOBAL Node *SetSUdclFields(Node *sudcl, Node *id, List *fields, Coord left_coord, Coord right_coord, List *attributes);
GLOBAL Node *SetSUdclNameFields(Node *sudcl, Node *id, List *fields, Coord left_coord, Coord right_coord, List *attributes);
GLOBAL Node *SetSUdclName(Node *sudcl, Node *id, Coord coord);
GLOBAL Node *BuildEnum(Node *id, List *values, Coord enum_coord, Coord left_coord, Coord right_coord);
GLOBAL Node *BuildEnumConst(Node *id, Node *value);
GLOBAL void ShadowTag(SUEtype *create, SUEtype *shadowed);
GLOBAL void VerifySUEcomplete(Node *type);
GLOBAL Node *ForceNewSU(Node *sudcl, Coord coord);
GLOBAL int SUE_Sizeof(SUEtype *sue);
GLOBAL int SUE_Alignment(SUEtype *sue);
GLOBAL Node *SUE_FindField(SUEtype *sue, Node *field_name);
GLOBAL Bool SUE_SameTagp(SUEtype *sue1, SUEtype *sue2);

/* Global procedures from procedure.c */

GLOBAL Node *AddParameterTypes(Node *decl, List *types);
GLOBAL Node *AddDefaultParameterTypes(Node *decl);
GLOBAL Node *DefineProc(Bool old_style, Node *decl);
GLOBAL Node *DemoteProcArgs(Node *decl);
GLOBAL Node *SetProcBody(Node *proc, Node *block);
GLOBAL Node *AddReturn(Node *returnnode);
GLOBAL void FunctionConflict(Node *orig, Node *create);
GLOBAL Node *BuildLabel(Node *id);
GLOBAL Node *ResolveGoto(Node *id, Coord coord);
GLOBAL void EndOfLabelScope(Node *label);
GLOBAL Node *DefineInlet(Node *decl);
GLOBAL Node *SetInletBody(Node *proc, Node *block);
GLOBAL void  MaybeCheckLabelAtEndOfCompoundStatement(List *);

/* Global operators from type.c */
GLOBAL Node *NodeDataType(Node *node);
GLOBAL Node *NodeDataTypeSuperior(Node *node);
GLOBAL void SetNodeDataType(Node *node, Node *type);
GLOBAL Node *SdclFindField(Node *sdcl, Node *field_name);
GLOBAL const char*TypeName(TypeSpecifier);

/* From operators.c */
GLOBAL const char *OperatorName(OpType op);
GLOBAL const char *OperatorText(OpType op);
GLOBAL Bool IsAssignmentOp(OpType op);
GLOBAL Bool IsIncDecOp(OpType op);
GLOBAL Bool IsComparisonOp(OpType op);
GLOBAL Bool IsConversionOp(OpType op);
GLOBAL Bool IsArithmeticOp(OpType op);

GLOBAL OpType OpOfAssignmentOp(OpType op); /* What is the underlying operator of an assignment?  E.g. += -->  +.  Only defined for assignment ops that have such an operator.*/

/* From constexpr.c */

GLOBAL Bool NodeIsConstant(Node *node);
GLOBAL Node *NodeGetConstantValue(Node *node);
GLOBAL void NodeSetConstantValue(Node *node, Node *value);

GLOBAL int NodeConstantCharValue(Node *node);
GLOBAL int NodeConstantSintValue(Node *node);
GLOBAL unsigned int NodeConstantUintValue(Node *node);
GLOBAL long NodeConstantSlongValue(Node *node);
GLOBAL unsigned long NodeConstantUlongValue(Node *node);
GLOBAL float NodeConstantFloatValue(Node *node);
GLOBAL double NodeConstantDoubleValue(Node *node);
GLOBAL const char *NodeConstantStringValue(Node *node);
GLOBAL unsigned long NodeConstantIntegralValue(Node *node);
GLOBAL Bool NodeConstantBooleanValue(Node *node);

GLOBAL void NodeSetCharValue(Node *node, int i);
GLOBAL void NodeSetSintValue(Node *node, int i);
GLOBAL void NodeSetUintValue(Node *node, unsigned u);
GLOBAL void NodeSetSlongValue(Node *node, long l);
GLOBAL void NodeSetUlongValue(Node *node, unsigned long ul);
GLOBAL void NodeSetFloatValue(Node *node, float f);
GLOBAL void NodeSetDoubleValue(Node *node, double d);
GLOBAL void NodeSetStringValue(Node *node, const char *s);

GLOBAL Bool IsConstantZero(Node *node);
GLOBAL Bool IsConstantString(Node *node);
GLOBAL Bool IsIntegralConstant(Node *node);
GLOBAL Bool IntegralConstEqual(Node *value1, Node *value2);

GLOBAL void ConstFoldTernary(Node *node);
GLOBAL void ConstFoldCast(Node *node);

#endif				/* ifndef _TYPE_H_ */
