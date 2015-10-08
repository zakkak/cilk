/* -*- c-indentation-style: "k&r" -*- */
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

#include "ast.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/type.c $ $LastChangedBy: bradley $ $Rev: 2780 $ $Date: 2006-03-30 08:45:19 -0500 (Thu, 30 Mar 2006) $");


PRIVATE Bool IsLvalue_Local(Node *node, Bool modifiablep);
PRIVATE void UnwindTdefs(Node **pnode, TypeQual *ptq);
PRIVATE const char *TypeSpecifierName(TypeSpecifier tm);

GLOBAL Bool TrackIds = FALSE;

GLOBAL Node *EllipsisNode;
GLOBAL Node *Undeclared;	/* used for undeclared variables */

/* Global Type constants */
GLOBAL Node *PrimVoid, *PrimChar, *PrimSchar, *PrimUchar, *PrimSshort,
*PrimUshort, *PrimSint, *PrimUint, *PrimSlong, *PrimUlong, *PrimSlonglong,
*PrimUlonglong, *PrimFloat, *PrimDouble, *PrimLongdouble;

GLOBAL Node *StaticString;

GLOBAL Node *SintZero, *UintZero, *SlongZero, *UlongZero, *FloatZero,
*DoubleZero;

GLOBAL Node *PtrVoid, *PtrNull;

GLOBAL Node *SintOne, *UintOne, *SlongOne, *UlongOne, *FloatOne, *DoubleOne;

PRIVATE const char *TypeNames[MaxBasicType];

GLOBAL void InitTypes()
{
     TypeNames[Uchar] = "unsigned char";
     TypeNames[Schar] = "signed char";
     TypeNames[Char] = "char";
     TypeNames[Sshort] = "short";
     TypeNames[Ushort] = "unsigned short";
     TypeNames[Sint] = "int";
     TypeNames[Uint] = "unsigned";
     TypeNames[Int_ParseOnly] = "int";
     TypeNames[Slong] = "long";
     TypeNames[Ulong] = "unsigned long";
     TypeNames[Slonglong] = "long long";
     TypeNames[Ulonglong] = "unsigned long long";
     TypeNames[Float] = "float";
     TypeNames[Double] = "double";
     TypeNames[Longdouble] = "long double";
     TypeNames[Void] = "void";
     TypeNames[Ellipsis] = "...";

     EllipsisNode = MakePrim(EMPTY_TQ, Ellipsis);
     Undeclared = MakeDecl("undeclared!", EMPTY_TQ, NULL, NULL, NULL);

     PrimVoid = MakePrim(EMPTY_TQ, Void);
     PrimChar = MakePrim(EMPTY_TQ, Char);
     PrimSchar = MakePrim(EMPTY_TQ, Schar);
     PrimUchar = MakePrim(EMPTY_TQ, Uchar);
     PrimSshort = MakePrim(EMPTY_TQ, Sshort);
     PrimUshort = MakePrim(EMPTY_TQ, Ushort);
     PrimSint = MakePrim(EMPTY_TQ, Sint);
     PrimUint = MakePrim(EMPTY_TQ, Uint);
     PrimSlong = MakePrim(EMPTY_TQ, Slong);
     PrimUlong = MakePrim(EMPTY_TQ, Ulong);
     PrimSlonglong = MakePrim(EMPTY_TQ, Slonglong);
     PrimUlonglong = MakePrim(EMPTY_TQ, Ulonglong);
     PrimFloat = MakePrim(EMPTY_TQ, Float);
     PrimDouble = MakePrim(EMPTY_TQ, Double);
     PrimLongdouble = MakePrim(EMPTY_TQ, Longdouble);
     StaticString = MakePtr(EMPTY_TQ, MakePrim(TQ_STATIC, Char));

     /* Make some standard zeros */
     SintZero = MakeConstSint(0);
     UintZero = MakeConstUint(0);
     SlongZero = MakeConstSlong(0);
     UlongZero = MakeConstUlong(0);
     FloatZero = MakeConstFloat(0.0);
     DoubleZero = MakeConstDouble(0.0);

     /* Make some standard ones */
     SintOne = MakeConstSint(1);
     UintOne = MakeConstUint(1);
     SlongOne = MakeConstSlong(1);
     UlongOne = MakeConstUlong(1);
     FloatOne = MakeConstFloat(1.0);
     DoubleOne = MakeConstDouble(1.0);

     PtrVoid = MakePtr(EMPTY_TQ, PrimVoid);
     PtrNull = MakeConstPtr(0);
}

/*************************************************************************/
/*                                                                       */
/*         Type qualifiers, storage classes, decl qualifiers             */
/*                                                                       */
/*************************************************************************/

GLOBAL void TQtoText(char array[], int arraysize, TypeQual tq)
/* TQtoText is used only to produce diagnostics.  Hence there is no need for a full OutputContext. */
{
    GBUF *gb=make_string_gbuf(array,arraysize);
    TQtoGBUF(gb, tq);
}

GLOBAL void PrintTQ(GBUF *out, TypeQual tq, int offset, UNUSED(Bool norecurse))
/* PrintTQ is used only to produce diagnostics.  Hence there is no need for a full OutputContext. */
{
    TQtoGBUF(out, tq);
    if (tq.attributes) PrintList(out, tq.attributes, offset);
}

GLOBAL void DPTQ(TypeQual tq) {
    WITH_FILE_GBUF(gb,stdout,PrintTQ(gb,tq,0,0));
}

GLOBAL TypeQual MergeTypeQuals(TypeQual left, TypeQual right, Coord coord)
{
     TypeQual scl, scr, dql, dqr, tql, tqr;
     TypeQual result = tq_attribute(JoinLists(left.attributes,right.attributes));

     scl = tq_storage_class(left);
     scr = tq_storage_class(right);

     if (tq_has_anything(scl) && tq_has_anything(scr)) {
	  if (tq_equal(scl, scr)) {	/* scl == scr == single storage class */
	       char name[20];
	       TQtoText(name, 20, scl);
	       name[strlen(name) - 1] = 0;
	       WarningCoord(4, coord, "redundant storage class `%s'", name);
	  } else {
	       char namel[20], namer[20];
	       TQtoText(namel, 20, scl);
	       namel[strlen(namel) - 1] = 0;
	       TQtoText(namer, 20, scr);
	       namer[strlen(namer) - 1] = 0;
	       SyntaxErrorCoord(coord, "conflicting storage classes `%s' and `%s'",
				namel, namer);
	  }
	  result = tq_union(result, scl);
     } else {
	  result = tq_union(result, tq_union(scl, scr));
     }

     dql = tq_subtract(tq_decl_quals(left), scl);
     dqr = tq_subtract(tq_decl_quals(right), scr);

     if (tq_has_anything(tq_intersection(dql, dqr))) {
	  char name[20];
	  TQtoText(name, 20, tq_intersection(dql, dqr));
	  name[strlen(name) - 1] = 0;
	  WarningCoord(4, coord, "redundant declaration qualifier `%s'", name);
     }
     result = tq_union(result, tq_union(dql, dqr));

     tql = tq_type_quals(left);
     tqr = tq_type_quals(right);

     if (tq_has_anything(tq_intersection(tql, tqr))) {
	  char name[20];
	  TQtoText(name, 20, tq_intersection(tql, tqr));
	  name[strlen(name) - 1] = 0;
	  WarningCoord(4, coord, "redundant type qualifier `%s'", name);
     }
     result = tq_union(result, tq_union(tql, tqr));

     return (result);
}

/* The tq_coord is passed in and the resulting coord is the min of the original coord and the new coord.
 * That is because in the process of parsing, you can actually have a terminal X that appears after terminal Y and
 * yet terminal X's coordinate is before terminal Y's coord, and we end up with nonidempotent parsing.
 * How can this happen?  Line directives can make it happen.
 */
GLOBAL Node *TypeQualifyNode(Node *node, TypeQual tq, Coord *tq_coord)
{
     switch (node->typ) {
	 case Prim:
	      node->u.prim.tq = MergeTypeQuals(node->u.prim.tq, tq, node->coord);
	      break;
	 case Tdef:
	      node->u.tdef.tq = MergeTypeQuals(node->u.tdef.tq, tq, node->coord);
	      break;
	 case Ptr:
	      node->u.ptr.tq = MergeTypeQuals(node->u.ptr.tq, tq, node->coord);
	      break;
	 case Adcl:
	      node->u.adcl.tq = MergeTypeQuals(node->u.adcl.tq, tq, node->coord);
	      break;
	 case Sdcl:
	      node->u.sdcl.tq = MergeTypeQuals(node->u.sdcl.tq, tq, node->coord);
	      break;
	 case Udcl:
	      node->u.udcl.tq = MergeTypeQuals(node->u.udcl.tq, tq, node->coord);
	      break;
	 case Edcl:
	      node->u.edcl.tq = MergeTypeQuals(node->u.edcl.tq, tq, node->coord);
	      break;

/* added by Manish 1/31/94 */
	 case Fdcl:
	      node->u.fdcl.tq = MergeTypeQuals(node->u.fdcl.tq, tq, node->coord);
	      break;
	 default:
	      fPrintNode(stdout, node, 0);
	      printf("\n");

	      /* this assertion always fails */
	      FAIL("Unexpected node type");
     }
     if (tq_coord) minf_coord(&node->coord, tq_coord);
     return (node);
}

/* Make a type that is merged from <type1> and quals of <qual2> */
GLOBAL Node *MakeMergedType(Node *type1, Node *qual2)
{
     return TypeQualifyNode(NodeCopy(type1, NodeOnly), NodeTypeQuals(qual2), &qual2->coord);
}

GLOBAL TypeQual NodeTq(Node *node)
{
     assert(node);

     switch (node->typ) {
	 case Prim:
	      return node->u.prim.tq;
	 case Tdef:
	      return node->u.tdef.tq;
	 case Ptr:
	      return node->u.ptr.tq;
	 case Adcl:
	      return node->u.adcl.tq;
	 case Sdcl:
	      return node->u.sdcl.tq;
	 case Udcl:
	      return node->u.udcl.tq;
	 case Edcl:
	      return node->u.edcl.tq;
	 case Decl:
	      return node->u.decl.tq;
	 case Fdcl:
	      return node->u.fdcl.tq;
	 default:
	      fprintf(stderr, "Internal Error! Unrecognized type\n");
	      fPrintNode(stderr, node, 0);
	      fprintf(stderr, "\n");
	      assert(FALSE);
     }
     return (EMPTY_TQ);		/* unreachable */
}

#define UPDATEF(field) field = f(field)
GLOBAL void NodeUpdateTq(Node *node, TypeQual(*f)(TypeQual))
{
    switch (node->typ) {
    case Prim:
	UPDATEF(node->u.prim.tq);
	break;
    case Tdef:
	UPDATEF(node->u.tdef.tq);
	break;
    case Ptr:
	UPDATEF(node->u.ptr.tq);
	break;
    case Adcl:
	UPDATEF(node->u.adcl.tq);
	break;
    case Sdcl:
	UPDATEF(node->u.sdcl.tq);
	break;
    case Udcl:
	UPDATEF(node->u.udcl.tq);
	break;
    case Edcl:
	UPDATEF(node->u.edcl.tq);
	break;
    case Decl:
	UPDATEF(node->u.decl.tq);
	break;
    case Fdcl:
	UPDATEF(node->u.fdcl.tq);
	break;
    default:
	fprintf(stderr, "Internal Error! Unrecognized type\n");
	fPrintNode(stderr, node, 0);
	fprintf(stderr, "\n");
	assert(FALSE);
    }
}
#undef UPDATEF

#define UPDATEF(field) field = f(field, tq2)
GLOBAL void NodeUpdateTq2(Node *node, TypeQual(*f)(TypeQual, TypeQual), TypeQual tq2)
{
    switch (node->typ) {
    case Prim:
	UPDATEF(node->u.prim.tq);
	break;
    case Tdef:
	UPDATEF(node->u.tdef.tq);
	break;
    case Ptr:
	UPDATEF(node->u.ptr.tq);
	break;
    case Adcl:
	UPDATEF(node->u.adcl.tq);
	break;
    case Sdcl:
	UPDATEF(node->u.sdcl.tq);
	break;
    case Udcl:
	UPDATEF(node->u.udcl.tq);
	break;
    case Edcl:
	UPDATEF(node->u.edcl.tq);
	break;
    case Decl:
	UPDATEF(node->u.decl.tq);
	break;
    case Fdcl:
	UPDATEF(node->u.fdcl.tq);
	break;
    default:
	fprintf(stderr, "Internal Error! Unrecognized type\n");
	fPrintNode(stderr, node, 0);
	fprintf(stderr, "\n");
	assert(FALSE);
    }
}
#undef UPDATEF

/* 
 * NodeTypeQuals, NodeStorageClass, and NodeDeclQuals only work
 * properly AFTER the parse is over, when storage class and
 * decl qualifiers have been moved to the decl.
 */
GLOBAL TypeQual NodeTypeQuals(Node *node)
{
     return tq_type_quals(NodeTq(node));
}

GLOBAL TypeQual NodeStorageClass(Node *node)
{
     return tq_storage_class(NodeDeclTq(node));
}

GLOBAL void NodeSetStorageClass(Node *node, TypeQual sc)
{
    NodeUpdateTq2(node, tq_set_storage_class_from, sc);
}

GLOBAL TypeQual NodeDeclTq (Node *node) {
    assert(node->typ==Decl);
    assert(IsDecl(node));
    return node->u.decl.tq;
}

/*
GLOBAL TypeQual NodeDeclQuals(Node *node)
{
     return DECL_QUALS(NodeDeclTq(node));
}
*/

GLOBAL TypeQual NodeDeclLocation(Node *node)
{
     assert(node->typ == Decl);
     assert(IsDecl(node));
     return tq_decl_location(node->u.decl.tq);
}

GLOBAL void NodeSetDeclLocation(Node *node, TypeQual dl)
{
    NodeUpdateTq2(node, tq_set_decl_location_from, dl);
}

/*************************************************************************/
/*                                                                       */
/*                      Primitive types                                  */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *StartPrimType(BasicType basic, Coord coord)
{
     return MakePrimCoord(EMPTY_TQ, basic, coord);
}

/*
 * requires: node and n2 be partial Prim types (created by StartPrimType and
 * not yet finished with FinishPrimType).
 * changes: node
 */
GLOBAL Node *MergePrimTypes(Node *node, Node *n2)
{
     ExplodedType et1, et2;

     /* note: memory leak of *n2 */

     assert(node != NULL && node->typ == Prim);
     assert(n2   != NULL && n2->typ == Prim);

     BASIC2EXPLODED(node->u.prim.basic, et1);
     BASIC2EXPLODED(n2->u.prim.basic, et2);

     /*
      * First merge base type (int, char, float, double, ...).
      * At most one base type may be specified. 
      */
     if (et1.base && et2.base)
	  SyntaxErrorCoord(node->coord,
			   "conflicting type specifiers: `%s' and `%s'",
			   TypeSpecifierName(et1.base),
			   TypeSpecifierName(et2.base));
     else
	  et1.base |= et2.base;

     /*
      * Now merge signs.  At most one sign may be specified; it appears to
      * be legal in ANSI to repeat the same sign (as in 
      * "unsigned unsigned int"), but a warning is generated.
      */
     if (et1.sign && et2.sign) {
	  if (et1.sign != et2.sign)
	       SyntaxErrorCoord(node->coord,
			    "conflicting type specifiers: `%s' and `%s'",
				TypeSpecifierName(et1.sign),
				TypeSpecifierName(et2.sign));
	  else
	       WarningCoord(3, node->coord,
			    "repeated type specifier: `%s'",
			    TypeSpecifierName(et1.sign));
     } else
	  et1.sign |= et2.sign;

     /*
      * Check that the resulting sign is compatible with the resulting
      * base type.
      * Only int and char may have a sign specifier.
      */
     if (et1.sign && et1.base) {
	  if (et1.base != Int_ParseOnly && et1.base != Char) {
	       SyntaxErrorCoord(node->coord,
			    "conflicting type specifiers: `%s' and `%s'",
				TypeSpecifierName(et1.base),
				TypeSpecifierName(et1.sign));
	       et1.sign = 0;
	  }
     }
     /*
      * Merge lengths (short, long, long long).
      */
     if (et1.length && et2.length) {
	  if (et1.length == Long && et2.length == Long
	      && !ANSIOnly)
	       et1.length = Longlong;
	  else
	       SyntaxErrorCoord(node->coord,
			    "conflicting type specifiers: `%s' and `%s'",
				TypeSpecifierName(et1.length),
				TypeSpecifierName(et2.length));
     } else
	  et1.length |= et2.length;

     /*
      * Check that the resulting length is compatible with the resulting
      * base type.
      * Only int may have any length specifier; double may have long.
      */
     if (et1.length && et1.base) {
	  if (et1.base != Int_ParseOnly && !(et1.base == Double && et1.length == Long)) {
	       SyntaxErrorCoord(node->coord,
			    "conflicting type specifiers: `%s' and `%s'",
				TypeSpecifierName(et1.base),
				TypeSpecifierName(et1.length));
	       et1.length = 0;
	  }
     }
     EXPLODED2BASIC(et1, node->u.prim.basic);

     return (node);
}

GLOBAL Node *FinishPrimType(Node *node)
{
     BasicType bt;
     ExplodedType et;

     assert(node);
     assert(node->typ == Prim);

     BASIC2EXPLODED(node->u.prim.basic, et);

     bt = et.base;

     switch (et.base) {
	 case 0:
	 case Int_ParseOnly:
	      switch (et.length) {
		  case Short:
		       bt = (et.sign == Unsigned) ? Ushort : Sshort;
		       break;
		  case Long:
		       bt = (et.sign == Unsigned) ? Ulong : Slong;
		       break;
		  case Longlong:
		       bt = (et.sign == Unsigned) ? Ulonglong : Slonglong;
		       break;
		  case 0:
		       bt = (et.sign == Unsigned) ? Uint : Sint;
		       break;
		  default:
		       UNREACHABLE;
	      }
	      break;

	 case Char:
	      assert(et.length == 0);
	      switch (et.sign) {
		  case Unsigned:
		       bt = Uchar;
		       break;
		  case Signed:
		       bt = Schar;
		       break;
		  case 0:
		       bt = Char;
		       break;
		  default:
		       UNREACHABLE;
	      }
	      break;

	 case Double:
	      assert(et.sign == 0);
	      assert(et.length == 0 || et.length == Long);
	      bt = (et.length == Long) ? Longdouble : Double;
	      break;

	 default:
	      assert(et.sign == 0);
	      assert(et.length == 0);
     }

     node->u.prim.basic = bt;
     return node;

#if 0
/* this code is obsoleted by the new version of MergeBasicTypes,
 * which makes sure that basic types are always "cleaned up", with
 * signs fully specified.  -- rcm */

/* FinishType cleans up the passed in BasicType, making sure that
 * all of the possibly signed types are explictly specified.  This
 * simplifies the type-checking phase */

     assert(node->typ == Prim);

     switch (node->u.prim.basic) {
	 case Unsigned:
	      node->u.prim.basic = Uint;
	      return (node);
	 case Signed:
	      node->u.prim.basic = Sint;
	      return (node);
	 case Long:
	      node->u.prim.basic = DEFAULT_LONG;
	      return (node);
	 case Short:
	      node->u.prim.basic = DEFAULT_SHORT;
	      return (node);
#if 0
	 case Char:		/* char defaults to signed char */
	      node->u.prim.basic = DEFAULT_CHAR;
	      return (node);
/* this was non-ANSI standard -- "char" and "signed char" are
 * explicitly different, incompatible types.  Thus:
 * 
 * signed char *psch;
 * char *pch;
 * 
 * if (pch == psch) {...}
 * 
 * should generate a diagnostic message, because the pointers
 * are incompatible. So we can't simply fold char into signed char.
 * We do treat Char identically with Schar for the purposes of
 * conversion, however, so only TypeEqual notices the difference
 * between them. -- rcm
 */
#endif
	 case Int:
	      node->u.prim.basic = DEFAULT_INT;
	      return (node);
	 default:
	      return (node);
     }
#endif

}

/* construct a primitive type with unspecified BasicType */
GLOBAL Node *MakeDefaultPrimType(TypeQual tq, Coord coord)
{
     Node *create = StartPrimType(0, coord);
     create->u.prim.tq = tq;
     return FinishPrimType(create);
}

GLOBAL const char *TypeName(TypeSpecifier ts) {
    assert(ts<MaxBasicType); // ts is an unsigned value
    return TypeNames[ts];
}

PRIVATE const char *TypeSpecifierName(TypeSpecifier ts)
{
     if ((ts & BasicTypeMask) != 0)
	  return TypeNames[ts];

     switch (ts) {
	 case Signed:
	      return "signed";
	 case Unsigned:
	      return "unsigned";

	 case Short:
	      return "short";
	 case Long:
	      return "long";
	 case Longlong:
	      return "long long";

	      /* INSERT EXTENSIONS HERE */

	 default:
	      UNREACHABLE;
	      return NULL;
     }
}

/* SansSign folds signed types into unsigned types of the same length. */
GLOBAL Node *SansSign(Node *type)
{
     Node *new;
     TypeQual tq;

     if (type->typ != Prim)
	  return type;

     switch (type->u.prim.basic) {
	 case Uchar:
	 case Schar:
	 case Char:
	      new = PrimChar;
	      break;

	 case Ushort:
	 case Sshort:
	      new = PrimUshort;
	      break;

	 case Ulong:
	 case Slong:
	      new = PrimUlong;
	      break;

	 case Ulonglong:
	 case Slonglong:
	      new = PrimUlonglong;
	      break;

	 case Uint:
	 case Sint:
	      new = PrimUint;
	      break;

	 default:
	      return type;
     }

     if (tq_has_anything(tq = NodeTq(type))) {
	  new = NodeCopy(new, NodeOnly);
	  NodeUpdateTq2(new, tq_union, tq);
     }
     return new;
}

/*************************************************************************/
/*                                                                       */
/*         Resolving identifiers through the symbol table                */
/*                                                                       */
/*************************************************************************/

PRIVATE Node *lookup_identifier(Node *id)
{
     Node *var;
     const char *name;

     assert(id && id->typ == Id);
     name = id->u.id.text;

     if (!LookupSymbol(Identifiers, name, (GenericREF) & var)) {
	  var = Undeclared;
	  SyntaxErrorCoord(id->coord, "undeclared variable `%s'", name);
     } else {
	  REFERENCE(var);
	  if (TrackIds) {
	       fprintf(stderr, "=== `%s' = ", name);
	       fPrintNode(stderr, var, 0);
	       printf("\n");
	  }
     }
     return (var);
}

GLOBAL Bool IsAType(const char *name)
{
     Node *var;
     return (LookupSymbol(Identifiers, name, (GenericREF) & var)
	     && DeclIsTypedef(var));
}

GLOBAL Node *GetTypedefType(Node *id)
{
     Node *var = lookup_identifier(id);

     assert(var);
     assert(var->typ == Decl);
     return (var->u.decl.type);
}

GLOBAL Node *LookupFunction(Node *call)
{
     Node *id, *var;

     assert(call != NULL && call->typ == Call);
     id = call->u.call.name;
     assert(id->typ == Id);

     if (!LookupSymbol(Identifiers, id->u.id.text, (GenericREF) & var)) {
	  if (!LookupSymbol(Externals, id->u.id.text, (GenericREF) & var)) {
	       WarningCoord(2, id->coord,
		     "implicitly declaring function to return int: %s()",
			    id->u.id.text);

	       var = MakeDeclCoord(id->u.id.text,
				   TQ_TOP_DECL,
				   MakeFdclCoord(EMPTY_TQ, NULL, MakeDefaultPrimType(EMPTY_TQ, id->coord),
						 id->coord),
				   NULL,
				   NULL,
				   id->coord);
	       REFERENCE(var);
	       id->u.id.decl = var;
	       InsertSymbol(Identifiers, id->u.id.text, var, NULL);
	       InsertSymbol(Externals, id->u.id.text, var, NULL);
	  } else {		/* only in Externals */
	       id->u.id.decl = var;
	       /* already referenced => no REFERENCE(var) */
	       InsertSymbol(Identifiers, id->u.id.text, var, NULL);
	  }
     } else {
	  id->u.id.decl = var;
	  REFERENCE(var);
	  if (TrackIds) {
	       fprintf(stderr, "=== `%s' = ", id->u.id.text);
	       fPrintNode(stderr, var, 0);
	  }
     }
     return (call);
}

GLOBAL Node *LookupPostfixExpression(Node *post)
{
     switch (post->typ) {
	 case Const:
	 case Comma:
	 case Block:
	 case Ternary:
	 case Cast:
	 case ImplicitCast:
	 case Constructor:
	 case Synched:
	      break;
	 case Id:
	      post->u.id.decl = lookup_identifier(post);
	      break;
	 case Binop:		/* structure reference */
	      if (post->u.binop.op == '.' || post->u.binop.op == ARROW)
		   LookupPostfixExpression(post->u.binop.left);
	      break;
	 case Unary:		/* post inc/dec */
	      if (post->u.unary.op == POSTINC || post->u.unary.op == POSTDEC)
		   LookupPostfixExpression(post->u.unary.expr);
	      break;
	 case Array:
	      LookupPostfixExpression(post->u.array.name);
	      return (post);
	 case Call:
	      if (post->u.call.name->typ == Id) {
		   LookupFunction(post);
	      } else {
		   LookupPostfixExpression(post->u.call.name);
	      }
	      break;
	 default:
	      Warning(1, "Internal Error!");
	      fprintf(stderr, "\tLookupPostfixExpression: unexpected node:\n");
	      PrintASTcoords=TRUE;
	      fPrintNode(stderr, post, 2);
	      FAIL("Unexpected node type");
     }
     return (post);
}

GLOBAL void OutOfScope(Node *var)
{
     Node *type;

     assert(var);
     assert(var->typ == Decl);

     type = var->u.decl.type;
     assert(type);

#if 0
     printf("OutOfScope\n");
     PrintNode(stdout, var, 0);
     printf("\n");
     PrintNode(stdout, type, 0);
     printf("\n");
#endif

     if (var->u.decl.references == 0 && type->typ != Fdcl) {
	  /* give warning only if unused local variable */
	 if (tq_has_block_decl(NodeDeclLocation(var)) &&
	     !tq_has_extern(NodeStorageClass(var)))
	     WarningCoord(2, var->coord, "unused variable `%s'", VAR_NAME(var));
     }
}

/*************************************************************************/
/*                                                                       */
/*             Size and alignment calculations                           */
/*                                                                       */
/*************************************************************************/

GLOBAL unsigned int NodeSizeof(Node *node, Node *node_type, 
			       Bool allow_unsized_array)
{
     Node *type;

     if (!node) {
	  Warning(1, "Internal Error!");
	  fprintf(stderr, "NodeSizeof called with nil\n");
	  return 0;
     }

     type = (node_type) ? NodeDataType(node_type) : NodeDataType(node);

     switch (type->typ) {
	 case Prim:
	      switch (type->u.prim.basic) {
		  case Char:
		  case Schar:
		  case Uchar:
		       return CHAR_SIZE;
		  case Sshort:
		  case Ushort:
		       return SHORT_SIZE;
		  case Sint:
		  case Uint:
		       return INT_SIZE;
		  case Slong:
		  case Ulong:
		       return LONG_SIZE;
		  case Slonglong:
		  case Ulonglong:
		       return LONGLONG_SIZE;
		  case Float:
		       return FLOAT_SIZE;
		  case Double:
		       return DOUBLE_SIZE;
		  case Longdouble:
		       return LONGDOUBLE_SIZE;
		  case Void:
		       if (ANSIOnly)
			    SyntaxErrorCoord(node->coord, "Can't compute size of type void");
		       return 1;
		  default:
		       WarningCoord(1, node->coord,
			  "NodeSizeof(): Unrecognized primitive type %d",
				    type->u.prim.basic);
		       fPrintNode(stderr, node, 0);
		       fputc('\n', stderr);
		       return 1;
	      }

	 case Ptr:
	      return POINTER_SIZE;

	 case Sdcl:
	      if (!IsStructComplete(type)) {
		   SyntaxErrorCoord(node->coord,
		      "Can't compute size of incomplete structure type");
		   return 1;
	      } else
		   return SUE_Sizeof(type->u.sdcl.type);

	 case Udcl:
	      if (!IsUnionComplete(type)) {
		   SyntaxErrorCoord(node->coord,
			  "Can't compute size of incomplete union type");
		   return 1;
	      } else
		   return SUE_Sizeof(type->u.udcl.type);

	 case Edcl:
	      return INT_SIZE;

     case Adcl:
	      if (IsStringConstant(node)) {
		   return strlen(NodeConstantStringValue(node)) + 1 /* for null terminator */ ;
	      } else if (IsUnsizedArray(type)) {
		   if (allow_unsized_array)
			return 0;

		   SyntaxErrorCoord(node->coord,
				    "Incorrect use of undimensioned array");
		   return 1;
	      } else
		   return type->u.adcl.dimp->size;

     case Fdcl:
	  if (ANSIOnly)
	       SyntaxErrorCoord(node->coord, "Can't compute size of a function type (ISO C forbids taking the size of functions or performing pointer arithmetic on pointers to functions)");
	  return 1;

     case Int_ParseOnly:
     case Ellipsis:     
     case MaxBasicType: 
	      WarningCoord(1, node->coord,
			   "NodeSizeof(): Unrecognized node type %d",
			   type->typ);
	      fPrintNode(stderr, node, 0);
	      fputc('\n', stderr);
	      return 1;
     default:
	 FAIL("Unknown type");
     }
}

GLOBAL int NodeAlignment(Node *node, Node *node_type)
{
     Node *type = (node_type) ? node_type : NodeDataType(node);

     switch (type->typ) {
	 case Prim:
	      switch (type->u.prim.basic) {
		  case Char:
		  case Schar:
		  case Uchar:
		       return CHAR_ALIGN;
		  case Sshort:
		  case Ushort:
		       return SHORT_ALIGN;
		  case Sint:
		  case Uint:
		       return INT_ALIGN;
		  case Slong:
		  case Ulong:
		       return LONG_ALIGN;
		  case Slonglong:
		  case Ulonglong:
		       return LONGLONG_ALIGN;
		  case Float:
		       return FLOAT_ALIGN;
		  case Double:
		       return DOUBLE_ALIGN;
		  case Longdouble:
		       return LONGDOUBLE_ALIGN;
		  case Void:
		       SyntaxErrorCoord(node->coord, "Can't find alignment for type void");
		       return 1;
		  default:
		       WarningCoord(1, node->coord,
		       "NodeAlignment(): Unrecognized primitive type %d",
				    type->u.prim.basic);
		       return 1;
	      }

	 case Ptr:
	      return POINTER_ALIGN;

	 case Adcl:
	      return NodeAlignment(node, NodeDataType(type->u.adcl.type));

	 case Sdcl:
	      if (!IsStructComplete(type)) {
		   SyntaxErrorCoord(node->coord,
		      "Can't compute size of incomplete structure type");
		   return 1;
	      } else
		   return SUE_Alignment(type->u.sdcl.type);

	 case Udcl:
	      if (!IsUnionComplete(type)) {
		   SyntaxErrorCoord(node->coord,
			  "Can't compute size of incomplete union type");
		   return 1;
	      } else
		   return SUE_Alignment(type->u.sdcl.type);

	 case Edcl:
	      return INT_ALIGN;

	 default:
	      WarningCoord(1, node->coord,
			   "NodeAlignment(): Unrecognized node type %d",
			   type->typ);
	      return 1;
     }
}

/*************************************************************************/
/*                                                                       */
/*             Various predicates and selectors for types                */
/*                                                                       */
/*************************************************************************/

GLOBAL Bool DeclIsExtern(Node *node)
{
     assert(node->typ == Decl);
     return tq_has_extern(node->u.decl.tq);
}

GLOBAL Bool NodeIsConstQual(Node *node)
{
    return tq_has_const(NodeTq(node));
}

GLOBAL Bool NodeIsCilkProcedure(Node *node)
{
    return tq_has_procedure(NodeTq(node));
}

GLOBAL Bool DeclIsEnumConst(Node *node)
{
     assert(node->typ == Decl);
     return tq_has_enum_decl(node->u.decl.tq);
}

GLOBAL Bool DeclIsTypedef(Node *node)
{
     return tq_has_typedef(NodeStorageClass(node));
}

GLOBAL Bool DeclIsStatic(Node *node)
{
     return tq_has_static(NodeStorageClass(node));
}

GLOBAL Bool DeclIsExternal(Node *node)
{
     TypeQual sc = NodeStorageClass(node);
     TypeQual dl = NodeDeclLocation(node);

     return tq_has_extern(sc) || tq_has_top_decl(dl);
}

GLOBAL Bool DeclIsBlock(Node *node)
{
    return tq_has_block_decl(NodeDeclLocation(node));
}

GLOBAL Node *PtrSubtype(Node *ptr)
{
     assert(ptr->typ == Ptr);
     return ptr->u.ptr.type;
}

GLOBAL Node *ArrayRefType(Node *atype, List *dims)
{
     ListMarker marker;
     Node *dim;

     IterateList(&marker, dims);
     while (NextOnList(&marker, (GenericREF) & dim))
	  if (atype->typ == Adcl)
	       atype = NodeDataType(atype->u.adcl.type);
	  else if (atype->typ == Ptr)
	       atype = NodeDataType(atype->u.ptr.type);
	  else
	       assert(FALSE);

     return atype;
}

GLOBAL Bool IsObjectType(Node *node)
{
     return !(IsFunctionType(node) || IsIncompleteType(node));
}

GLOBAL Bool IsFunctionType(Node *node)
{
     assert(node);
     return node->typ == Fdcl;
}

GLOBAL Bool IsIncompleteType(Node *node)
{
     if (IsArrayType(node))
	  return IsUnsizedArray(node);
     else if (IsStructType(node))
	  return !IsStructComplete(node);
     else if (IsUnionType(node))
	  return !IsUnionComplete(node);
     else
	  return FALSE;
}

GLOBAL Bool IsUnsizedArray(Node *node)
{
     assert(node);
     assert(node->typ == Adcl);
     assert(node->u.adcl.dimp);
     return node->u.adcl.dimp->dim == NULL;
}

GLOBAL Bool IsStructComplete(Node *node)
{
     assert(node);
     assert(node->typ == Sdcl);
     return node->u.sdcl.type->complete;
}

GLOBAL Bool IsUnionComplete(Node *node)
{
     assert(node);
     assert(node->typ == Udcl);
     return node->u.udcl.type->complete;
}

GLOBAL Bool IsVoidType(Node *node)
{
     assert(node);
     node = NodeDataType(node);
     return node->typ == Prim && node->u.prim.basic == Void;
}

GLOBAL Bool IsArrayType(Node *node)
{
     assert(node);
     return node->typ == Adcl;
}

GLOBAL Bool IsSueType(Node *node)
{
     assert(node);
     return node->typ == Sdcl || node->typ == Udcl || node->typ == Edcl;
}

GLOBAL Bool IsStructType(Node *node)
{
     assert(node);
     return node->typ == Sdcl;
}

GLOBAL Bool IsUnionType(Node *node)
{
     assert(node);
     return node->typ == Udcl;
}

GLOBAL Bool IsEnumType(Node *node)
{
     assert(node);
     return node->typ == Edcl;
}

GLOBAL Bool IsPointerType(Node *node)
{
     assert(node);
     return NodeDataType(node)->typ == Ptr;
}

GLOBAL Bool IsScalarType(Node *node)
{
     assert(node);
     return IsArithmeticType(node) || IsPointerType(node);
}

GLOBAL Bool IsArithmeticType(Node *node)
{
     assert(node);
     return IsIntegralType(node) || IsFloatingType(node);
}

GLOBAL Bool IsIntegralType(Node *node)
{
     assert(node);

     switch (node->typ) {
	 case Prim:
	      {
		   switch (node->u.prim.basic) {
		       case Uchar:
		       case Schar:
		       case Char:
		       case Ushort:
		       case Sshort:
		       case Uint:
		       case Sint:
		       case Ulong:
		       case Slong:
		       case Ulonglong:
		       case Slonglong:
			    return TRUE;
		       default:
			    return FALSE;
		   }
		   break;
	      }

	 case Edcl:
	      return TRUE;
	 default:
	      return FALSE;
     }
}

GLOBAL Bool IsFloatingType(Node *node)
{
     assert(node);

     /* WCH: changed because of potential bug */
     return
	 (node->typ == Prim &&
	  (node->u.prim.basic == Float ||
	   node->u.prim.basic == Double ||
	   node->u.prim.basic == Longdouble));
}

GLOBAL Bool IsModifiableLvalue(Node *node)
{
     return IsLvalue_Local(node, TRUE);
}

GLOBAL Bool IsLvalue(Node *node)
{
     return IsLvalue_Local(node, FALSE);
}

PRIVATE Bool IsLvalue_Local(Node *node, Bool modifiablep)
{
     TypeQual tq;
     Node *type = NodeDataTypeSuperior(node);

     UnwindTdefs(&type, &tq);

     /* test first for modifiability, if required by caller */
     if (modifiablep && tq_has_const(tq))
	  return FALSE;

     /* now test whether node is an lvalue */
     switch (node->typ) {
	 case Id:
	      /* an identifier is an lvalue if its type is arithmetic,
	       * structure, union, or pointer */
	      switch (type->typ) {
		  case Prim:
		  case Sdcl:
		  case Udcl:
		  case Edcl:
		  case Ptr:
		       return TRUE;
		  case Adcl:
		  case Fdcl:
		      if (tq_has_formal_decl(NodeDeclLocation(node->u.id.decl)))
			    /* formals of type array or function are treated as pointers to same,
			     * thus lvalues */
			    return TRUE;
		       else
			    return FALSE;
		  default:
		       return FALSE;
	      }

	      /* *p is an lvalue */
	 case Unary:
	      if (node->u.unary.op == INDIR)
		   return TRUE;
	      else
		   return FALSE;

	      /* a[i] is equivalent to *(a+i), hence is an lvalue */
	 case Array:
	      return TRUE;

	 case Binop:
	      /* s.f and ps->f are lvalues if s is an lvalue and f is not an array */
	      if (((node->u.binop.op == '.' &&
		    IsLvalue_Local(node->u.binop.left, modifiablep))
		   ||
		   node->u.binop.op == ARROW)
		  &&
		  type->typ != Adcl)
		   return TRUE;
	      else
		   return FALSE;

	 case Comma:
	      /* (a,b,c) is a lvalue if c is an lvalue for gcc, but not for ISO C */
	      if (ANSIOnly)
		   SyntaxErrorCoord(node->coord, "ISO C forbids use of compound expressions as lvalues");
	      return IsLvalue_Local(LastItem(node->u.comma.exprs), modifiablep);

         case Constructor:
	     if (ANSIOnly) return FALSE; /* We should not have even been able to parse a constructor if we are in ansi-only mode, but we'll treat it as a non-lvalue for now.*/
	     {
		 Node *initializer= node->u.constructor.initializerlist;
		 List *exprs;
		 assert(initializer->typ==Initializer);
		 exprs = initializer->u.initializer.exprs;
		 /* If every expr is a constant, then we have an lvalue*/
		 while (exprs) {
		     if (!NodeIsConstant(FirstItem(exprs))) return FALSE;
		     exprs = Rest(exprs);
		 }
		 return TRUE;
	     }
	 case Cast:
	     return IsLvalue_Local(node->u.cast.expr, FALSE); /* flush the modifiable requirement if we had a cast.*/

	 default:
	      return FALSE;
     }
}


GLOBAL Bool IsVoidArglist(List *arglist)
{
     return (ListLength(arglist) == 1) && IsVoidType(FirstItem(arglist));
}

GLOBAL Bool IsEllipsis(Node *node)
{
     return node->typ == Prim && node->u.prim.basic == Ellipsis;
}

GLOBAL Bool IsLogicalOrRelationalExpression(Node *node)
{
     return IsLogicalExpression(node) || IsRelationalExpression(node);
}

GLOBAL Bool IsRelationalExpression(Node *node)
{
     if (node->typ == Binop) {
	  OpType opcode = node->u.binop.op;

	  if (opcode == EQ || opcode == NE ||
	      opcode == GE || opcode == '>' ||
	      opcode == LE || opcode == '<')
	       return TRUE;
     }
     return FALSE;
}

GLOBAL Bool IsLogicalExpression(Node *node)
{
     if (node->typ == Binop) {
	  OpType opcode = node->u.binop.op;

	  return (opcode == ANDAND || opcode == OROR);
     } else if (node->typ == Unary)
	  return node->u.unary.op == '!';
     else
	  return FALSE;
}

GLOBAL Bool IsPtrToObject(Node *node)
{
     Node *n = NodeDataType(node);

     if (IsPointerType(n))
	  return IsObjectType(NodeDataType(n->u.ptr.type));
     else
	  return FALSE;
}

GLOBAL Bool IsPtrToFunction(Node *node)
{
     Node *n = NodeDataType(node);

     if (IsPointerType(n))
	  return IsFunctionType(NodeDataType(n->u.ptr.type));
     else
	  return FALSE;
}

GLOBAL Bool IsPtrToVoid(Node *node)
{
     Node *n = NodeDataType(node);

     if (IsPointerType(n))
	  return IsVoidType(NodeDataType(n->u.ptr.type));
     else
	  return FALSE;
}

GLOBAL Node *FunctionReturnType(Node *node)
{
     Node *ret;

     assert(node->typ == Proc);
     ret = node->u.proc.decl;
     assert(ret->typ == Decl);
     ret = ret->u.decl.type;
     assert(ret->typ == Fdcl);
     return ret->u.fdcl.returns;
}

GLOBAL void FunctionSetReturnType(Node *node, Node *new_type)
{
     Node *ret;

     assert(node->typ == Proc);
     ret = node->u.proc.decl;
     assert(ret->typ == Decl);
     ret = ret->u.decl.type;
     assert(ret->typ == Fdcl);

     ret->u.fdcl.returns = new_type;
}

GLOBAL void FunctionPushArglist(Node *node, Node *new_arg)
{
     Node *ret;

     assert(node->typ == Proc);
     ret = node->u.proc.decl;
     assert(ret->typ == Decl);
     ret = ret->u.decl.type;
     assert(ret->typ == Fdcl);

     ret->u.fdcl.args = ConsItem(new_arg, ret->u.fdcl.args);
}

/* IsPrimitiveStmt() is true for expressions and statements which
 * never contain other statements */
GLOBAL Bool IsPrimitiveStmt(Node *node)
{
     assert(node);

     if (node->typ == Block)
	  return FALSE;
     else if (IsExpr(node))
	  return TRUE;
     else
	  switch (node->typ) {
	      case Goto:
	      case Continue:
	      case Break:
	      case Return:
		   return TRUE;

	      default:
		   return FALSE;
	  }
}

GLOBAL Bool TypeIsChar(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Char;
}

GLOBAL Bool TypeIsSint(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Sint;
}

GLOBAL Bool TypeIsUint(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Uint;
}

GLOBAL Bool TypeIsSlong(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Slong;
}

GLOBAL Bool TypeIsUlong(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Ulong;
}

GLOBAL Bool TypeIsFloat(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Float;
}

GLOBAL Bool TypeIsDouble(Node *type)
{
     assert(type);
     return type->typ == Prim && type->u.prim.basic == Double;
}

GLOBAL Bool TypeIsString(Node *type)
{
     Node *elementtype;

     assert(type);
     if (type->typ != Adcl)
	  return FALSE;

     elementtype = type->u.adcl.type;

     return (elementtype->typ == Prim && elementtype->u.prim.basic == Char);
}

GLOBAL Bool NodeTypeIsSint(Node *node)
{
     return TypeIsSint(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsChar(Node *node)
{
     return TypeIsChar(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsUint(Node *node)
{
     return TypeIsUint(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsSlong(Node *node)
{
     return TypeIsSlong(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsUlong(Node *node)
{
     return TypeIsUlong(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsFloat(Node *node)
{
     return TypeIsFloat(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsDouble(Node *node)
{
     return TypeIsDouble(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsString(Node *node)
{
     return TypeIsString(NodeDataType(node));
}

GLOBAL Bool NodeTypeIsIntegral(Node *node)
{
     Node *type = NodeDataType(node);
     return IsIntegralType(type);
}

GLOBAL Bool NodeTypeIsUnsizedArray(Node *node)
{
     return IsArrayType(node) && IsUnsizedArray(node);
}

GLOBAL Bool IsPrimChar(Node *node)
{
     assert(node);

     if (node->typ == Prim)
	  return (node->u.prim.basic == Char);
     else
	  return FALSE;
}

GLOBAL Bool IsPrimCharModuloSign(Node *node)
{
     assert(node);

     if (node->typ == Prim)
	  return (node->u.prim.basic == Char ||
		  node->u.prim.basic == Schar ||
		  node->u.prim.basic == Uchar);
     else
	  return FALSE;
}

GLOBAL Bool IsArrayOfChar(Node *node)
{
     assert(node);

     if (node->typ == Adcl)
	  return IsPrimChar(NodeDataType(node->u.adcl.type));
     else
	  return FALSE;
}

GLOBAL Bool IsArrayOfCharModuloSign(Node *node)
{
     assert(node);

     if (node->typ == Adcl)
	  return IsPrimCharModuloSign(NodeDataType(node->u.adcl.type));
     else
	  return FALSE;
}

GLOBAL Bool IsStringConstant(Node *node)
{
     assert(node);

     if (node->typ == Const) {
	  assert(node->u.Const.type);

	  return node->u.Const.type->typ == Adcl &&
	      IsPrimChar(node->u.Const.type->u.adcl.type);
     } else
	  return FALSE;
}

GLOBAL Bool IsAggregateType(Node *node)
{
     assert(node);
     return node->typ == Adcl || node->typ == Sdcl;
}

/*************************************************************************/
/*                                                                       */
/*             Type compatibility and equality                           */
/*                                                                       */
/*************************************************************************/


/*******************************************************************

  TypeEqual -- implements type equivalence according to K&R2 section
               A8.10 (fix: what section in ANSI standard?)

     strict_toplevel and strict_recursive control whether 
        const and volatile (and other type qualifiers specified
	in TQ_COMPATIBLE) are ignored:

	!strict_toplevel ==> type quals are ignored when comparing
	                     roots of type1 and type2
        !strict_recursive => type quals are ignored when comparing
	                     children of type1 and type2
	
*******************************************************************/

PRIVATE void UnwindTdefs(Node **pnode, TypeQual *ptq)
{
     *ptq = NodeTypeQuals(*pnode);
     while ((*pnode)->typ == Tdef) {
	  *pnode = (*pnode)->u.tdef.type;
	  *ptq = tq_union(*ptq, NodeTypeQuals(*pnode));
     }
}

GLOBAL Bool TypeEqual(Node *node1, Node *node2)
{
     return TypeEqualQualified(node1, node2, TRUE, TRUE);
}

/*
 * equality test for types in formal arguments.
 */
GLOBAL Bool TypeEqualFormals(Node *node1, Node *node2)
{
     if (node1->typ == Adcl && node2->typ == Ptr) 
	  return TypeEqual(node1->u.adcl.type, node2->u.adcl.type);

     if (node2->typ == Adcl && node1->typ == Ptr) 
	  return TypeEqual(node2->u.adcl.type, node1->u.adcl.type);

     return TypeEqual(node1, node2);
}

GLOBAL Bool TypeEqualQualified(Node *node1, Node *node2, Bool strict_toplevel, Bool strict_recursive)
{
     TypeQual tq1, tq2;

     assert(node1 && node2);

     UnwindTdefs(&node1, &tq1);
     UnwindTdefs(&node2, &tq2);

     tq1 = tq_remove_sue_elaborated(tq1);
     tq2 = tq_remove_sue_elaborated(tq2);
     if (!strict_toplevel) {
	  tq1 = tq_remove_compatible(tq1);
	  tq2 = tq_remove_compatible(tq2);
     }
     if (!tq_equal(tq1, tq2))
	  return FALSE;

     if (node1->typ != node2->typ)
	  return FALSE;

     switch (node1->typ) {
	 case Prim:
	      return (node1->u.prim.basic == node2->u.prim.basic);
	 case Ptr:
	      return TypeEqualQualified(node1->u.ptr.type, node2->u.ptr.type, strict_recursive, strict_recursive);

	 case Adcl:
	      if (!TypeEqualQualified(node1->u.adcl.type, node2->u.adcl.type, strict_recursive, strict_recursive))
		   return FALSE;

	      /* If both dims are specified they must be the same */
	      if (node1->u.adcl.dimp->dim)	/* original */
		   if (node2->u.adcl.dimp->dim) {
			if (NodeIsConstant(node1->u.adcl.dimp->dim) &&
			    NodeIsConstant(node2->u.adcl.dimp->dim))
			     return 
				  IntegralConstEqual(node1->u.adcl.dimp->dim,
						     node2->u.adcl.dimp->dim);
			else
			     return TRUE; /*
					   * fix: what do we do if
					   * constant expressions
					   * haven't been computed
					   * yet?
					   */
		   } else {
			/* set real dim in new, sharing */
			node2->u.adcl.dimp = node1->u.adcl.dimp;
		   }
	      else {
		   /* set real dim in original */
		   node1->u.adcl.dimp = node2->u.adcl.dimp;
	      }
	      return TRUE;
	 case Sdcl:
	      /* This is the normal case */
	      if (node1 == node2)
		   return TRUE;

	      /* Check the tags also in case we are still just scanning */
	      return SUE_SameTagp(node1->u.sdcl.type, node2->u.sdcl.type);
	 case Udcl:
	      /* This is the normal case */
	      if (node1 == node2)
		   return TRUE;

	      /* Check the tags also in case we are still just scanning */
	      return SUE_SameTagp(node1->u.udcl.type, node2->u.udcl.type);
	 case Edcl:
	      /* This is the normal case */
	      if (node1 == node2)
		   return TRUE;

	      /* Check the tags also in case we are still just scanning */
	      return SUE_SameTagp(node1->u.edcl.type, node2->u.edcl.type);
	 case Tdef:
	      UNREACHABLE;	/* UnwindTdefs already removed this */
	 case Fdcl:
	      if (!TypeEqualQualified(node1->u.fdcl.returns, node2->u.fdcl.returns, strict_recursive, strict_recursive))
		   return FALSE;

	      /* if either list is "unspecified" (indicated by NULL value),
	       * assume comparison is successful */
	      if (node1->u.fdcl.args == NULL || node2->u.fdcl.args == NULL)
		   return TRUE;

	      if (ListLength(node1->u.fdcl.args) != ListLength(node2->u.fdcl.args))
		   return FALSE;

	      {
		   ListMarker marker1;
		   Node *arg1;
		   ListMarker marker2;
		   Node *arg2;

		   IterateList(&marker1, node1->u.fdcl.args);
		   IterateList(&marker2, node2->u.fdcl.args);
		   while (NextOnList(&marker1, (GenericREF) & arg1)
			  && NextOnList(&marker2, (GenericREF) & arg2)) {
			if (arg1->typ == Decl)
			     arg1 = arg1->u.decl.type;
			if (arg2->typ == Decl)
			     arg2 = arg2->u.decl.type;
			if (!TypeEqualQualified(arg1, arg2, strict_recursive, strict_recursive))
			     return FALSE;
		   }
	      }
	      return TRUE;

	 default:
	      FAIL("TypeEqual(): Unrecognized type");
     }
     return FALSE;		/* eliminates warning */
}
