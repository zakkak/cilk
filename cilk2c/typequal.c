#include "ast.h"

typedef enum {

     EMPTY_TQ_enum = 0,
/* 
   Declaration qualifiers.  A declaration can have multiple qualifiers,
   though some are mutually exclusive (like storage classes).
   During the parsing of a declaration, decl qualifiers are kept in 
   the tq field of its primitive type.  After the decl has been parsed,
   SetDeclType then moves the qualifiers to their proper place, the 
   Decl node.

   To add a new decl qualifier:
      1. Define a new symbol below, being careful that its bits do
         not conflict with existing decl and type qualifiers,
         since they can coexist in the same TypeQual variable.
      2. Insert the symbol in T_DECL_QUALS (below).
      3. If the qualifier is parsed in the source (rather than calculated),
         add a lexical token (c4.l) and a new production to storage.classes
         (ANSI-C.y).
      4. Add code to print out its name in TQtoText (type.c).
      5. Add merging logic to MergeTypeQuals (type.c), if necessary.
*/

  T_AUTO     = 0x00001,
  T_EXTERN   = 0x00002,
  T_REGISTER = 0x00003,
  T_STATIC   = 0x00004,
  T_TYPEDEF  = 0x00005,
  /* Insert new storage classes here */

  T_STORAGE_CLASSES = (T_AUTO | T_EXTERN | T_REGISTER | T_STATIC | T_TYPEDEF),
#define STORAGE_CLASS(tq) ((tq) & T_STORAGE_CLASSES)

  /* the following qualifiers are all mutually exclusive,
     so they can share bits */
  T_TOP_DECL    = 0x00010,   /* top-level decl */
  T_BLOCK_DECL  = 0x00020,   /* local decl in a block */
  T_FORMAL_DECL = 0x00030,   /* formal parameter decl */
  T_SU_DECL     = 0x00040,   /* struct/union field decl */
  T_ENUM_DECL   = 0x00050,   /* enumerated constant decl */
  /* Insert new decl locations here */
  
  T_DECL_LOCATIONS = (T_TOP_DECL | T_BLOCK_DECL | T_FORMAL_DECL |
		      T_SU_DECL | T_ENUM_DECL),
#define DECL_LOCATION(tq)  ((tq) & T_DECL_LOCATIONS)

     /* Flag for redundant external declaration, which is defined as
      * an external declaration (NOT a definition, so it must have no
      * initializer or function body) of a name previously declared external 
      * in the same scope.  A trivial example:
      *    extern int x;
      *    extern int x;    <-- redundant
      * But:
      *    { extern int x; }
      *    { extern int x; }  <-- not redundant
      * because the two declarations have different scopes.
      */
     T_REDUNDANT_EXTERNAL_DECL = 0x00100,

     /* Insert new decl qualifiers here */
     T_SHARED           =   0x00200,
     T_PRIVATE          =   0x00400,
     T_NOOUTPUT         =   0x00800,	
     T_CILKONLY         =   0x01000,	/* variable can only be used in cilk code */
     T_CILKSAFE         =   0x02000,	/* promise that this variable is safe to access in parallel */

     /* the following tq's are used to generate CILK_WHERE_AM_I directives */
     T_FAST_PROCEDURE   =  0x04000,
     T_SLOW_PROCEDURE   =  0x08000,
     T_FAST_INLET       =  0x10000,
     T_SLOW_INLET       =  0x20000,
     T_STANDALONE_INLET =  0x40000,
     T_C_CODE           =  0x80000,

     T_CILK_WHERE_AM_I_DECL_QUALS  = (T_FAST_PROCEDURE | T_SLOW_PROCEDURE | T_FAST_INLET | T_SLOW_INLET | T_STANDALONE_INLET | T_C_CODE),

     T_DECL_QUALS = (T_STORAGE_CLASSES | T_DECL_LOCATIONS 
		     | T_REDUNDANT_EXTERNAL_DECL 
		     | T_SHARED | T_PRIVATE | T_NOOUTPUT 
		     | T_CILKONLY | T_CILKSAFE
		     | T_CILK_WHERE_AM_I_DECL_QUALS
		     ),

#define DECL_QUALS(tq)    ((tq) & T_DECL_QUALS)


/*
   Type qualifiers.  Multiple type qualifiers may apply to a type.  
   They may be associated with any primitive or complex type.  
   Some type qualifiers may be moved after parsing -- for instance,
   T_INLINE is moved to the top-level Fdcl it is describing.

   To add a new type qualifier:
      1. Define a new symbol below, being careful that its bits do
         not conflict with existing storage classes and type qualifiers,
         since they can coexist in the same TypeQual variable.
      2. Insert the symbol in T_TYPE_QUALS (below).
      3. Add a lexical token (c4.l) and a new production to either
         type.qualifiers or pointer.type.qualifiers (ANSI-C.y), depending 
         on whether the type qualifier is allowed only at the beginning
         of a type, or can appear after '*' like const and volatile.
      4. Add code to print out its name in TQtoText (type.c).
      5. Add its symbol to TQ_ALWAYS_COMPATIBLE (below) if an object with
         the type qualifier is always assignment-compatible with an object
	 without the type qualifier.
      6. Add merging logic to MergeTypeQuals (type.c), if necessary.
 */

    T_RESTRICT = 0x100000,
    T_COMPLEX  = 0x200000,

    T_CONST    = 0x400000,   /* leave some room for new decl qualifiers */
    T_VOLATILE = 0x800000,
    T_INLINE   = 0x1000000,
    T_SUE_ELABORATED  = 0x2000000,   /* on an Sdcl/Udcl/Edcl, indicates 
				      whether SUE's field list appeared
				      at that point in the source */
#define SUE_ELABORATED(tq)   (((tq) & T_SUE_ELABORATED) != 0)
    /* Insert new type qualifiers here */
    T_PROCEDURE       = 0x4000000,

    T_INLET           = 0x8000000,

    T_EXTENSION       = 0x10000000, /* used for gcc __extension__ keyword.  Not exactly a TYPE_QUAL */

    T_TYPE_QUALS = (T_RESTRICT | T_COMPLEX | T_CONST | T_VOLATILE | T_INLINE | T_SUE_ELABORATED | T_PROCEDURE | T_INLET ),
#define TYPE_QUALS(tq)    ((tq) & T_TYPE_QUALS)
    
/* Type qualifiers listed in TQ_COMPATIBLE are ignored
 * when checking two types for compatibility (weaker than strict equality). */
    TQ_COMPATIBLE   = (T_CONST | T_VOLATILE | T_INLINE | T_SUE_ELABORATED | T_RESTRICT | T_EXTENSION)

}
 TypeQual_enum;

PRIVATE inline int bit_is_set(TypeQual tq, TypeQual_enum bit) {
    return (tq.tq&bit)==bit;
}
PRIVATE inline TypeQual TQ (TypeQual_enum e) {
    return (TypeQual){e,0};
}
GLOBAL TypeQual tq_attribute (List *attrib) {
    return (TypeQual){0,attrib};
}
PRIVATE inline TypeQual TQsetbit(TypeQual tq, TypeQual_enum bits) {
    return (TypeQual){tq.tq|bits, tq.attributes};
}
PRIVATE inline TypeQual TQclearbit(TypeQual tq, TypeQual_enum bits) {
    return (TypeQual){tq.tq&~bits, tq.attributes};
}

GLOBAL int tq_has_auto     (TypeQual tq) { return STORAGE_CLASS(tq.tq)==T_AUTO; }
GLOBAL int tq_has_extern   (TypeQual tq) { return STORAGE_CLASS(tq.tq)==T_EXTERN; }
GLOBAL int tq_has_register (TypeQual tq) { return STORAGE_CLASS(tq.tq)==T_REGISTER; }
GLOBAL int tq_has_static   (TypeQual tq) { return STORAGE_CLASS(tq.tq)==T_STATIC; }
GLOBAL int tq_has_typedef  (TypeQual tq) { return STORAGE_CLASS(tq.tq)==T_TYPEDEF; }
GLOBAL int tq_has_some_storageclass (TypeQual tq) { return STORAGE_CLASS(tq.tq)!=0; }

GLOBAL TypeQual tq_add_auto     (TypeQual tq) { return TQsetbit(tq,T_AUTO); }
GLOBAL TypeQual tq_add_extern   (TypeQual tq) { return TQsetbit(tq,T_EXTERN); }
GLOBAL TypeQual tq_add_register (TypeQual tq) { return TQsetbit(tq,T_REGISTER); }
GLOBAL TypeQual tq_add_static   (TypeQual tq) { return TQsetbit(tq,T_STATIC); }
GLOBAL TypeQual tq_add_typedef  (TypeQual tq) { return TQsetbit(tq,T_TYPEDEF); }

GLOBAL TypeQual tq_storage_class (TypeQual tq) { return TQ(STORAGE_CLASS(tq.tq)); }
GLOBAL TypeQual tq_remove_all_storage_classes (TypeQual tq) { return TQ(tq.tq&~T_STORAGE_CLASSES); }
GLOBAL TypeQual tq_set_storage_class_from (TypeQual dest, TypeQual source) { return tq_union(tq_remove_all_storage_classes(dest),tq_storage_class(source)); }


GLOBAL int tq_has_top_decl    (TypeQual tq) { return DECL_LOCATION(tq.tq)==T_TOP_DECL; }
GLOBAL int tq_has_block_decl  (TypeQual tq) { return DECL_LOCATION(tq.tq)==T_BLOCK_DECL; }
GLOBAL int tq_has_formal_decl (TypeQual tq) { return DECL_LOCATION(tq.tq)==T_FORMAL_DECL; }
GLOBAL int tq_has_su_decl     (TypeQual tq) { return DECL_LOCATION(tq.tq)==T_SU_DECL; }
GLOBAL int tq_has_enum_decl   (TypeQual tq) { return DECL_LOCATION(tq.tq)==T_ENUM_DECL; }

/* These are mutually exlusive, so take care to zero out the old data before oring in the new data. */
GLOBAL TypeQual tq_add_top_decl    (TypeQual tq) { return (TypeQual){(tq.tq&~T_DECL_LOCATIONS)|T_TOP_DECL, tq.attributes}; }
GLOBAL TypeQual tq_add_block_decl  (TypeQual tq) { return (TypeQual){(tq.tq&~T_DECL_LOCATIONS)|T_BLOCK_DECL, tq.attributes}; }
GLOBAL TypeQual tq_add_formal_decl (TypeQual tq) { return (TypeQual){(tq.tq&~T_DECL_LOCATIONS)|T_FORMAL_DECL, tq.attributes}; }
GLOBAL TypeQual tq_add_su_decl     (TypeQual tq) { return (TypeQual){(tq.tq&~T_DECL_LOCATIONS)|T_SU_DECL, tq.attributes}; }
GLOBAL TypeQual tq_add_enum_decl   (TypeQual tq) { return (TypeQual){(tq.tq&~T_DECL_LOCATIONS)|T_ENUM_DECL, tq.attributes}; }

GLOBAL TypeQual tq_decl_location (TypeQual tq) { return TQ(DECL_LOCATION(tq.tq)); }
GLOBAL TypeQual tq_set_decl_location_from (TypeQual tq, TypeQual source) { return (TypeQual){(tq.tq&~T_DECL_LOCATIONS)|DECL_LOCATION(source.tq), tq.attributes}; }

GLOBAL int tq_has_redundant_external_decl (TypeQual tq) { return bit_is_set(tq,T_REDUNDANT_EXTERNAL_DECL); }
GLOBAL TypeQual tq_add_redundant_external_decl(TypeQual tq) { return TQsetbit(tq,T_REDUNDANT_EXTERNAL_DECL); }
GLOBAL TypeQual tq_remove_redundant_external_decl(TypeQual tq) { return TQclearbit(tq, T_REDUNDANT_EXTERNAL_DECL); }


GLOBAL int tq_has_shared (TypeQual tq)   { return bit_is_set(tq,T_SHARED); }
GLOBAL int tq_has_private (TypeQual tq)  { return bit_is_set(tq,T_PRIVATE); }
GLOBAL int tq_has_nooutput (TypeQual tq) { return bit_is_set(tq,T_NOOUTPUT); }
GLOBAL int tq_has_cilkonly (TypeQual tq) { return bit_is_set(tq,T_CILKONLY); }
GLOBAL int tq_has_cilksafe (TypeQual tq) { return bit_is_set(tq,T_CILKSAFE); }

GLOBAL TypeQual tq_add_shared (TypeQual tq)   { return TQsetbit(tq,T_SHARED); }
GLOBAL TypeQual tq_add_private (TypeQual tq)  { return TQsetbit(tq,T_PRIVATE); }
GLOBAL TypeQual tq_add_nooutput (TypeQual tq) { return TQsetbit(tq,T_NOOUTPUT); }
GLOBAL TypeQual tq_add_cilkonly (TypeQual tq) { return TQsetbit(tq,T_CILKONLY); }
GLOBAL TypeQual tq_add_cilksafe (TypeQual tq) { return TQsetbit(tq,T_CILKSAFE); }

/* Consider decl_quals to include attributes. */
GLOBAL TypeQual tq_remove_all_decl_quals (TypeQual tq) { return (TypeQual){tq.tq&~T_DECL_QUALS,0}; }

GLOBAL int tq_has_fast_procedure (TypeQual tq)   { return bit_is_set(tq, T_FAST_PROCEDURE); }
GLOBAL int tq_has_slow_procedure (TypeQual tq)   { return bit_is_set(tq, T_SLOW_PROCEDURE); }
GLOBAL int tq_has_fast_inlet (TypeQual tq)       { return bit_is_set(tq, T_FAST_INLET); }
GLOBAL int tq_has_slow_inlet (TypeQual tq)       { return bit_is_set(tq, T_SLOW_INLET); }
GLOBAL int tq_has_standalone_inlet (TypeQual tq) { return bit_is_set(tq, T_STANDALONE_INLET); }
GLOBAL int tq_has_c_code (TypeQual tq)           { return bit_is_set(tq, T_C_CODE); }

GLOBAL TypeQual tq_add_fast_procedure (TypeQual tq)   { return TQsetbit(tq,T_FAST_PROCEDURE); }
GLOBAL TypeQual tq_add_slow_procedure (TypeQual tq)   { return TQsetbit(tq,T_SLOW_PROCEDURE); }
GLOBAL TypeQual tq_add_fast_inlet (TypeQual tq)       { return TQsetbit(tq,T_FAST_INLET); }
GLOBAL TypeQual tq_add_slow_inlet (TypeQual tq)       { return TQsetbit(tq,T_SLOW_INLET); }
GLOBAL TypeQual tq_add_standalone_inlet (TypeQual tq) { return TQsetbit(tq,T_STANDALONE_INLET); }
GLOBAL TypeQual tq_add_c_code (TypeQual tq)           { return TQsetbit(tq,T_C_CODE); }

GLOBAL int tq_has_cilk_where_am_i_decl_quals(TypeQual tq) {
    return (tq.tq & T_CILK_WHERE_AM_I_DECL_QUALS )!=0;
}
GLOBAL TypeQual tq_remove_cilk_where_am_i_decl_quals(TypeQual tq) {
    /* Remove the cilk "where-am-i" declaration qualifiers */
    return TQclearbit(tq, ~T_CILK_WHERE_AM_I_DECL_QUALS );
}

GLOBAL TypeQual TQ_DECL_QUALS(void) {
    return TQ(T_DECL_QUALS);
}

GLOBAL TypeQual tq_decl_quals(TypeQual tq) {
    return (TypeQual){DECL_QUALS(tq.tq), tq.attributes};
}

GLOBAL int tq_has_restrict (TypeQual tq)       { return bit_is_set(tq,T_RESTRICT); }
GLOBAL int tq_has_complex (TypeQual tq)        { return bit_is_set(tq,T_COMPLEX); }
GLOBAL int tq_has_const (TypeQual tq)          { return bit_is_set(tq,T_CONST); }
GLOBAL int tq_has_volatile (TypeQual tq)       { return bit_is_set(tq,T_VOLATILE); }
GLOBAL int tq_has_inline(TypeQual tq)          { return bit_is_set(tq,T_INLINE); }
GLOBAL int tq_has_sue_elaborated (TypeQual tq) { return bit_is_set(tq,T_SUE_ELABORATED); }
GLOBAL int tq_has_procedure(TypeQual tq)       { return bit_is_set(tq,T_PROCEDURE); }
GLOBAL int tq_has_inlet(TypeQual tq)           { return bit_is_set(tq,T_INLET); }
GLOBAL int tq_has_extension (TypeQual tq)      { return bit_is_set(tq,T_EXTENSION); }

GLOBAL TypeQual tq_add_restrict (TypeQual tq)       { return TQsetbit(tq,T_RESTRICT); }
GLOBAL TypeQual tq_add_complex (TypeQual tq)        { return TQsetbit(tq,T_COMPLEX); }
GLOBAL TypeQual tq_add_const (TypeQual tq)          { return TQsetbit(tq,T_CONST); }
GLOBAL TypeQual tq_add_volatile(TypeQual tq)        { return TQsetbit(tq,T_VOLATILE); }
GLOBAL TypeQual tq_add_inline (TypeQual tq)         { return TQsetbit(tq,T_INLINE); }
GLOBAL TypeQual tq_add_sue_elaborated (TypeQual tq) { return TQsetbit(tq,T_SUE_ELABORATED); }
GLOBAL TypeQual tq_add_procedure (TypeQual tq)      { return TQsetbit(tq,T_PROCEDURE); }
GLOBAL TypeQual tq_add_inlet (TypeQual tq)          { return TQsetbit(tq,T_INLET); }
GLOBAL TypeQual tq_add_extension (TypeQual tq)      { return TQsetbit(tq,T_EXTENSION); }

GLOBAL TypeQual tq_remove_complex(TypeQual tq)         { return TQclearbit(tq, T_COMPLEX); }
GLOBAL TypeQual tq_remove_const (TypeQual tq)          { return TQclearbit(tq, T_CONST); }
GLOBAL TypeQual tq_remove_inline (TypeQual tq)         { return TQclearbit(tq, T_INLINE); }
GLOBAL TypeQual tq_remove_sue_elaborated (TypeQual tq) { return TQclearbit(tq, T_SUE_ELABORATED); }
GLOBAL TypeQual tq_remove_procedure (TypeQual tq)      { return TQclearbit(tq, T_PROCEDURE); }
GLOBAL TypeQual tq_remove_inlet (TypeQual tq)          { return TQclearbit(tq, T_INLET); }

GLOBAL TypeQual tq_type_quals (TypeQual tq) { return TQ(TYPE_QUALS(tq.tq)); }
GLOBAL TypeQual tq_remove_compatible (TypeQual tq) { return TQclearbit(tq, TQ_COMPATIBLE); }

PRIVATE List* UnionLists(List *a, List *b) {
    if (a==NULL) return b;
    if (b==NULL) return a;
    if (a==b) return a;
    if (FindItem(b, FirstItem(a))) return UnionLists (Rest(a), b);
    return ConsItem(FirstItem(a), UnionLists(Rest(a), b));
}

GLOBAL TypeQual tq_union (TypeQual a, TypeQual b) { /* Produce a type qualifier which is the union of two others. */
    return (TypeQual){a.tq|b.tq, UnionLists(a.attributes, b.attributes)};
}
GLOBAL TypeQual tq_intersection (TypeQual a, TypeQual b) { return TQ(a.tq&b.tq); }
GLOBAL TypeQual tq_subtract (TypeQual a, TypeQual b) { return (TypeQual){a.tq & ~b.tq, a.attributes}; }

GLOBAL int tq_has_anything (TypeQual tq) { return tq.tq!=0; }
GLOBAL TypeQual tq_remove_everything (TypeQual tq __attribute__((__unused__))) { return EMPTY_TQ; }

GLOBAL int tq_equal(TypeQual a, TypeQual b) { return a.tq==b.tq; }
