/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/sue.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");


     
/* From ANSI-C.y */
GLOBAL Bool OmitLookups();

/***********************************************************************\
* structure/union/enum procedures
\***********************************************************************/

GLOBAL SUEtype *make_SUE(NodeType typ, const char *name, List *fields)
{
     SUEtype *create = HeapNew(SUEtype);
     create->typ = typ;
     create->complete = FALSE;
     create->coord = UnknownCoord;
     create->right_coord = UnknownCoord;
     create->name = name;
     create->visited = FALSE;
     create->transformed = FALSE;
     create->size = 0;
     create->align = 1;
     create->fields = fields;
     create->attributes = 0;
     return (create);
}

GLOBAL void PrintSUE(GBUF *out, SUEtype *sue, int offset, Bool recursep)
{
     ListMarker marker;
     Node *decl;
     const char *name;

     if (sue == NULL) {
	  gbprintf(out, "Null SUE");
	  return;
     }
     name = (sue->name) ? sue->name : "nil";

     switch (sue->typ) {
	 case Sdcl:
	      gbprintf(out, "struct %s (%d)", name, sue->size);
	      break;
	 case Udcl:
	      gbprintf(out, "union %s", name);
	      break;
	 case Edcl:
	      gbprintf(out, "enum %s", name);
	      break;
	 default:
	      gbprintf(out, "unexpected SUE type %d", sue->typ);
     }

     if (recursep) {
	  IterateList(&marker, sue->fields);
	  while (NextOnList(&marker, (GenericREF) & decl)) {
	       assert(decl->typ == Decl);
	       assert(IsDecl(decl));
	       PrintCRSpaces(out, offset);
	       PrintNode(out, decl, offset);
	  }
	  if (sue->attributes) {
	      PrintCRSpaces(out, offset);
	      PrintList(out, sue->attributes, offset);
	  }
     }
}

PRIVATE void tag_conflict(SUEtype *o, SUEtype *n)
{
     assert(o->typ == Sdcl || o->typ == Udcl || o->typ == Edcl);
     assert(n->typ == Sdcl || n->typ == Udcl || n->typ == Edcl);

     if (o == n)
	  return;

     if (o->typ != n->typ) {
	  SyntaxErrorCoord(n->coord,
			"redeclaration of structure/union/enum tag `%s'",
			   o->name);
	  fprintf(stderr, "\tPrevious declaration: ");
	  PRINT_COORD(stderr, o->coord);
	  fputc('\n', stderr);
     } else if (!o->complete) {	/* update original with nlist */
	  o->fields = n->fields;
	  o->complete = TRUE;
	  o->coord = n->coord;
	  o->right_coord = n->right_coord;
     } else if (n->complete) {
	  /* both are complete (i.e., with list of fields) => conflict */
	  switch (o->typ) {
	      case Sdcl:
		   SyntaxErrorCoord(n->coord,
			       "multiple declarations of structure `%s'",
				    o->name);
		   break;
	      case Udcl:
		   SyntaxErrorCoord(n->coord,
				    "multiple declarations of union `%s'",
				    o->name);
		   break;
	      case Edcl:
		   SyntaxErrorCoord(n->coord,
				    "multiple declarations of enum `%s'",
				    o->name);
		   break;
	      default:
		   UNREACHABLE;
	  }
	  fprintf(stderr, "\tPrevious definition: ");
	  PRINT_COORD(stderr, o->coord);
	  fputc('\n', stderr);
     }				/* else o->complete, !n->complete => leave unchanged */
}

GLOBAL Node *SetSUdclFields(Node *sudcl, Node *id, List *fields, Coord left_coord, Coord right_coord, List *attributes)
{
     SUEtype *sue;

     assert(sudcl != NULL && (sudcl->typ == Sdcl || sudcl->typ == Udcl));
     assert(sudcl->u.sdcl.type == NULL);	/* not allocated yet */

     /* create structure/union */
     sue = make_SUE(sudcl->typ, NULL, fields);
     sue->complete = TRUE;
     sue->coord = left_coord;
     sue->right_coord = right_coord;

     sudcl->u.sdcl.tq = tq_add_sue_elaborated(sudcl->u.sdcl.tq);
     sudcl->u.sdcl.type = sue;

     assert(sue->attributes==0);
     sue->attributes = attributes;

     return (sudcl);
}

GLOBAL Node *SetSUdclNameFields(Node *sudcl, Node *id, List *fields, Coord left_coord, Coord right_coord, List *attributes)
{
     SUEtype *sue;
     const char *name;

     assert(sudcl != NULL && (sudcl->typ == Sdcl || sudcl->typ == Udcl));
     assert(sudcl->u.sdcl.type == NULL);	/* not allocated yet */

     if (id != NULL) {		/* add to Tags namespace */
	  assert(id->typ == Id);
	  name = id->u.id.text;
     } else
	  name = NULL;

     /* create structure/union */
     sue = make_SUE(sudcl->typ, name, fields);
     sue->complete = TRUE;
     sue->coord = left_coord;
     sue->right_coord = right_coord;

     if (name) {
       if (!OmitLookups()) {
	 sue = InsertSymbol(Tags, name, sue, (ConflictProc) tag_conflict);
       }
     }
     else {
       sue->name = (OmitLookups() ? "___unnamed_sue_from_string" : InsertUniqueSymbol(Tags, sue, "___sue"));
     }

     sudcl->u.sdcl.tq = tq_add_sue_elaborated(sudcl->u.sdcl.tq);
     sudcl->u.sdcl.type = sue;

     assert(sue->attributes==0);
     sue->attributes = attributes;

     return (sudcl);
}

GLOBAL Node *SetSUdclName(Node *sudcl, Node *id, Coord coord)
{
     SUEtype *sue, *tmp;
     const char *name;

     assert(sudcl != NULL && (sudcl->typ == Sdcl || sudcl->typ == Udcl));
     assert(sudcl->u.sdcl.type == NULL);	/* not allocated yet */

     if (id != NULL) {		/* add to Tags namespace */
       assert(id->typ == Id);
       name = id->u.id.text;

       if (LookupSymbol(Tags, name, (GenericREF) & tmp)) {
	 /* use previous definition */
	 sue = tmp;
       } else {
	 /* create structure/union */
	 sue = make_SUE(sudcl->typ, name, NULL);
	 sue->coord = coord;
	 if (!OmitLookups()) {
	   sue = InsertSymbol(Tags, name, sue, (ConflictProc) tag_conflict);
	 }
       }
     } else {
       sue = make_SUE(sudcl->typ, NULL, NULL);
       sue->coord = coord;
       /* create unique name for anonymous sue */
       sue->name = (OmitLookups() ? "___unnamed_sue_from_string" : InsertUniqueSymbol(Tags, sue, "___sue"));
	  
     }

     sudcl->u.sdcl.tq = tq_remove_sue_elaborated(sudcl->u.sdcl.tq);
     sudcl->u.sdcl.type = sue;
     return (sudcl);
}

/* the following is only called from ForceNewSU */
PRIVATE void forced_tag_conflict(SUEtype *old, SUEtype *create)
{
     assert(old != create);
     /* ignore create, since it assumed we wouldn't get here */
     return;
}

GLOBAL Node *ForceNewSU(Node *sudcl, Coord coord)
{
     SUEtype *sue, *tmp;

     /* this procedure handles the "recondite" rule that says that
      * struct.or.union identifier ';'
      * creates a create struct/union even if the tag is defined in an outer
      * scope.  See K&R2, p213 */

     /* assume that this tag is not already in the innermost scope,
      * 1) create a create struct/union
      * 2) add it to the current scope
      * 3) if there's a conflict, then the assumption was false
      * and we use the previous version instead  */

     sue = sudcl->u.sdcl.type;
     if (sue->name == NULL)
	  return (sudcl);

     /* ignore sue->fields */

     sue = make_SUE(sudcl->typ, sue->name, NULL);
     sue->coord = coord;

     if (!OmitLookups()) {
       tmp = InsertSymbol(Tags, sue->name, sue,
			  (ConflictProc) forced_tag_conflict);
     }

     /* tmp != sue  implies conflict and memory leak of *sue */
     return (sudcl);
}

GLOBAL Node *BuildEnum(Node *id, List *values, Coord enum_coord, Coord left_coord, Coord right_coord)
{
     SUEtype *tmp, *sue;
     const char *name;
     Node *new;

     if (id != NULL) {		/* add to Tags namespace */
	  Bool insert = TRUE;

	  assert(id->typ == Id);
	  name = id->u.id.text;
	  sue = make_SUE(Edcl, name, values);
	  sue->complete = TRUE;
	  sue->coord = left_coord;
	  sue->right_coord = right_coord;

	  if (values == NULL) {
	       /* incomplete enums are invalid, so if there is no list
	        * of values, then the enum tag must be in scope */
	       if (!LookupSymbol(Tags, name, (GenericREF) & tmp)) {
		    SyntaxErrorCoord(id->coord, "undeclared enum `%s'", name);
		    /* put it in scope to prevent further errors */
	       } else
		    insert = FALSE;
	  }
	  if ((insert) && (!OmitLookups()))
	       sue = InsertSymbol(Tags, name, sue, (ConflictProc) tag_conflict);
     } else {
	  sue = make_SUE(Edcl, NULL, values);
	  sue->coord = left_coord;
	  sue->right_coord = right_coord;
	  sue->name = (OmitLookups() ? "___unnamed_sue_from_string" : InsertUniqueSymbol(Tags, sue, "___sue"));

	  /* no tag, so must have a value list (property of the grammar) */
	  assert(values != NULL);
     }

     new = MakeEdclCoord(EMPTY_TQ, sue, enum_coord);
     if (values)
	 new->u.edcl.tq = tq_add_sue_elaborated(new->u.edcl.tq);
     else
	 new->u.edcl.tq = tq_remove_sue_elaborated(new->u.edcl.tq);

     return new;
}

GLOBAL void VerifySUEcomplete(Node *type)
{
     SUEtype *sue;
     const char *kind;

     if (type == NULL)
	  return;

     switch (type->typ) {
	 case Ptr:		/* pointers may use incomplete types */
	      return;
	 case Adcl:
	      VerifySUEcomplete(type->u.adcl.type);
	      return;
	 case Fdcl:
	      VerifySUEcomplete(type->u.fdcl.returns);
	      return;
	 case Sdcl:
	      sue = type->u.sdcl.type;
	      kind = "structure";
	      break;
	 case Udcl:
	      sue = type->u.udcl.type;
	      kind = "union";
	      break;
	 case Edcl:		/* incomplete enums are always invalid and are
				 * caught by BuildEnum (K&R2 A8.4, p215) */
	 default:
	      return;
     }

     if (!sue->complete) {
	  if (sue->name == NULL) {
	       SyntaxError("incomplete unnamed %s", kind);
	  } else {
	       SyntaxError("incomplete %s `%s'", kind, sue->name);
	  }
     }
}

PRIVATE void enum_const_conflict(Node *orig, Node *create)
{
     SyntaxErrorCoord(create->coord,
		      "enum constant `%s' redeclares previous identifier",
		      VAR_NAME(orig));
     fprintf(stderr, "\tPrevious definition: ");
     PRINT_COORD(stderr, orig->coord);
     fputc('\n', stderr);
}

GLOBAL Node *BuildEnumConst(Node *name, Node *value)
{
     Node *decl = ConvertIdToDecl(name, TQ_ENUM_DECL,
				  MakeDefaultPrimType(TQ_CONST, name->coord),
				  value, NULL);
     if (!OmitLookups()) {
       InsertSymbol(Identifiers, decl->u.decl.name, decl,
		    (ConflictProc) enum_const_conflict);
     }
     return (decl);
}

GLOBAL void ShadowTag(SUEtype *create, SUEtype *shadowed)
{
     /* the two are equal only for redundant function/extern declarations */
     if (create != shadowed && WarningLevel == 5) {
	  WarningCoord(5, create->coord,
	       "struct/union/enum tag `%s' shadows previous declaration",
		       create->name);
	  fprintf(stderr, "\tPrevious declaration: ");
	  PRINT_COORD(stderr, shadowed->coord);
	  fputc('\n', stderr);
     }
}

GLOBAL int SUE_Sizeof(SUEtype *sue)
{
     assert(sue);
     return sue->size;
}

GLOBAL int SUE_Alignment(SUEtype *sue)
{
     assert(sue);
     return sue->align;
}

GLOBAL Node *SUE_FindField(SUEtype *sue, Node *field_name)
{
     ListMarker marker;
     Node *field;
     const char *name;

     assert(field_name->typ == Id);

     name = field_name->u.id.text;

     /* Find the field in the struct/union fields */
     IterateList(&marker, sue->fields);
     while (NextOnList(&marker, (GenericREF) & field)) {
	  assert(field->typ == Decl);
	  if (field->u.decl.name) {
	       if (strcmp(name, field->u.decl.name) == 0)
		    return field;
	  } else {
	       /* unnamed struct/union member */
	       Node *type = NodeDataType(field);
	       if (type->typ == Sdcl || type->typ == Udcl) {
		    Node *x;
		    x = SdclFindField(type, field_name);
		    if (x != NULL)
			 return x;
	       }
	  }
     }

     return NULL;
}

GLOBAL Bool SUE_SameTagp(SUEtype *sue1, SUEtype *sue2)
{
     if (!sue1->name || !sue2->name)
	  return FALSE;
     else
	  return strcmp(sue1->name, sue2->name) == 0;
}
