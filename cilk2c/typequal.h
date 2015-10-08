#include <stdarg.h>
/* TypeQual: storage classes, type qualifiers, and additional attributes */

/* First the old definition.  Using the enumeration types directly is deprecated. */

typedef struct typequal {
    unsigned int tq;
    List *attributes;
} TypeQual;
#define EMPTY_TQ ((TypeQual){0,0})

GLOBAL TypeQual tq_attribute (List *attrib);

/* These are the new typequal operations.   Use these. */

GLOBAL int tq_has_auto (TypeQual);
GLOBAL int tq_has_extern (TypeQual);
GLOBAL int tq_has_register (TypeQual);
GLOBAL int tq_has_static (TypeQual);
GLOBAL int tq_has_typedef (TypeQual);
GLOBAL int tq_has_some_storageclass (TypeQual);

GLOBAL TypeQual tq_add_auto (TypeQual);
GLOBAL TypeQual tq_add_extern (TypeQual);
GLOBAL TypeQual tq_add_register (TypeQual);
GLOBAL TypeQual tq_add_static (TypeQual);
GLOBAL TypeQual tq_add_typedef (TypeQual);

GLOBAL TypeQual tq_storage_class (TypeQual);

GLOBAL TypeQual tq_remove_register (TypeQual);
GLOBAL TypeQual tq_remove_all_storage_classes (TypeQual);
GLOBAL TypeQual tq_set_storage_class_from (TypeQual, TypeQual);

#define TQ_AUTO (tq_add_auto(EMPTY_TQ))
#define TQ_EXTERN (tq_add_extern(EMPTY_TQ))
#define TQ_REGISTER (tq_add_register(EMPTY_TQ))
#define TQ_STATIC (tq_add_static(EMPTY_TQ))
#define TQ_TYPEDEF (tq_add_typedef(EMPTY_TQ))

GLOBAL int tq_has_top_decl (TypeQual);
GLOBAL int tq_has_block_decl (TypeQual);
GLOBAL int tq_has_formal_decl (TypeQual);
GLOBAL int tq_has_su_decl (TypeQual);
GLOBAL int tq_has_enum_decl (TypeQual);

GLOBAL TypeQual tq_add_top_decl (TypeQual);
GLOBAL TypeQual tq_add_block_decl (TypeQual);
GLOBAL TypeQual tq_add_formal_decl (TypeQual);
GLOBAL TypeQual tq_add_su_decl (TypeQual);
GLOBAL TypeQual tq_add_enum_decl (TypeQual);

#define TQ_TOP_DECL (tq_add_top_decl(EMPTY_TQ))
#define TQ_BLOCK_DECL (tq_add_block_decl(EMPTY_TQ))
#define TQ_FORMAL_DECL (tq_add_formal_decl(EMPTY_TQ))
#define TQ_SU_DECL (tq_add_su_decl(EMPTY_TQ))
#define TQ_ENUM_DECL (tq_add_enum_decl(EMPTY_TQ))

GLOBAL TypeQual tq_decl_location (TypeQual);
GLOBAL TypeQual tq_set_decl_location_from (TypeQual tq, TypeQual source);

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
GLOBAL int tq_has_redundant_external_decl (TypeQual);
GLOBAL TypeQual tq_add_redundant_external_decl(TypeQual);
GLOBAL TypeQual tq_remove_redundant_external_decl(TypeQual);
#define TQ_REDUNDANT_EXTERNAL_DECL (tq_add_redundant_external_decl(EMPTY_TQ))

GLOBAL int tq_has_shared (TypeQual);
GLOBAL int tq_has_private (TypeQual); /* does it have a private decl? */
GLOBAL int tq_has_nooutput (TypeQual);
GLOBAL int tq_has_cilkonly (TypeQual);
GLOBAL int tq_has_cilksafe (TypeQual);

GLOBAL TypeQual tq_add_shared (TypeQual);
GLOBAL TypeQual tq_add_private (TypeQual);
GLOBAL TypeQual tq_add_nooutput (TypeQual);
GLOBAL TypeQual tq_add_cilkonly (TypeQual);
GLOBAL TypeQual tq_add_cilksafe (TypeQual);

#define TQ_SHARED (tq_add_shared(EMPTY_TQ))
#define TQ_PRIVATE (tq_add_private(EMPTY_TQ))
#define TQ_NOOUTPUT (tq_add_nooutput(EMPTY_TQ))
#define TQ_CILKONLY (tq_add_cilkonly(EMPTY_TQ))
#define TQ_CILKSAFE (tq_add_cilksafe(EMPTY_TQ))

/* Cilk "where-am-i" declaration specifiers. */
GLOBAL int tq_has_fast_procedure (TypeQual);
GLOBAL int tq_has_slow_procedure (TypeQual);
GLOBAL int tq_has_fast_inlet (TypeQual);
GLOBAL int tq_has_slow_inlet (TypeQual);
GLOBAL int tq_has_standalone_inlet (TypeQual);
GLOBAL int tq_has_c_code (TypeQual);

GLOBAL TypeQual tq_add_fast_procedure (TypeQual);
GLOBAL TypeQual tq_add_slow_procedure (TypeQual);
GLOBAL TypeQual tq_add_fast_inlet (TypeQual);
GLOBAL TypeQual tq_add_slow_inlet (TypeQual);
GLOBAL TypeQual tq_add_standalone_inlet (TypeQual);
GLOBAL TypeQual tq_add_c_code (TypeQual);

#define TQ_FAST_PROCEDURE (tq_add_fast_procedure(EMPTY_TQ))
#define TQ_SLOW_PROCEDURE (tq_add_slow_procedure(EMPTY_TQ))
#define TQ_FAST_INLET (tq_add_fast_inlet(EMPTY_TQ))
#define TQ_SLOW_INLET (tq_add_slow_inlet(EMPTY_TQ))
#define TQ_STANDALONE_INLET (tq_add_slow_inlet(EMPTY_TQ))
#define TQ_C_CODE (tq_add_c_code(EMPTY_TQ))

GLOBAL int tq_has_cilk_where_am_i_decl_quals(TypeQual);
GLOBAL TypeQual tq_remove_cilk_where_am_i_decl_quals(TypeQual); /* Remove the cilk "where-am-i" declaration qualifiers */

GLOBAL TypeQual TQ_DECL_QUALS(void);
GLOBAL TypeQual tq_decl_quals(TypeQual); /* Get out the decl qualifiers */

/* Type qualifiers. */
GLOBAL TypeQual tq_add_restrict (TypeQual);
GLOBAL TypeQual tq_add_complex (TypeQual);

#define TQ_RESTRICT (tq_add_restrict(EMPTY_TQ))
#define TQ_COMPLEX (tq_add_complex(EMPTY_TQ))

#define TQ_CONST (tq_add_const(EMPTY_TQ))
#define TQ_VOLATILE (tq_add_volatile(EMPTY_TQ))
#define TQ_INLINE (tq_add_inline(EMPTY_TQ))
#define TQ_SUE_ELABORATED (tq_add_sue_elaborated(EMPTY_TQ))

#define TQ_PROCEDURE (tq_add_procedure(EMPTY_TQ))
#define TQ_INLET (tq_add_inlet(EMPTY_TQ))
#define TQ_EXTENSION (tq_add_extension(EMPTY_TQ))

GLOBAL int tq_has_restrict (TypeQual);
GLOBAL int tq_has_complex (TypeQual);
GLOBAL int tq_has_const (TypeQual);
GLOBAL int tq_has_volatile (TypeQual);
GLOBAL int tq_has_inline(TypeQual);
GLOBAL int tq_has_sue_elaborated (TypeQual); /* on an Sdcl/Udcl/Edcl, did the SUE's field list appear at that point in the source? */
GLOBAL int tq_has_procedure(TypeQual);
GLOBAL int tq_has_inlet(TypeQual);
GLOBAL int tq_has_extension (TypeQual);

GLOBAL TypeQual tq_add_restrict (TypeQual);
GLOBAL TypeQual tq_add_complex (TypeQual);
GLOBAL TypeQual tq_add_const (TypeQual);
GLOBAL TypeQual tq_add_volatile(TypeQual);
GLOBAL TypeQual tq_add_inline (TypeQual);
GLOBAL TypeQual tq_add_sue_elaborated (TypeQual);
GLOBAL TypeQual tq_add_procedure (TypeQual);
GLOBAL TypeQual tq_add_inlet (TypeQual);
GLOBAL TypeQual tq_add_extension (TypeQual);

GLOBAL TypeQual tq_remove_complex(TypeQual);
GLOBAL TypeQual tq_remove_const (TypeQual);
GLOBAL TypeQual tq_remove_inline (TypeQual);
GLOBAL TypeQual tq_remove_sue_elaborated (TypeQual);
GLOBAL TypeQual tq_remove_procedure (TypeQual);
GLOBAL TypeQual tq_remove_inlet (TypeQual);

GLOBAL TypeQual tq_type_quals (TypeQual);
GLOBAL TypeQual tq_remove_compatible (TypeQual);

GLOBAL TypeQual tq_remove_all_decl_quals (TypeQual);
//TypeQual tq_add_decl_quals_from (TypeQual, TypeQual);

GLOBAL TypeQual tq_union (TypeQual, TypeQual); /* Produce a type qualifier which is the union of two others. */
GLOBAL TypeQual tq_intersection (TypeQual, TypeQual);
GLOBAL TypeQual tq_subtract (TypeQual, TypeQual);

GLOBAL int tq_has_anything (TypeQual);
GLOBAL TypeQual tq_remove_everything (TypeQual);

//GLBOAL int tq_has_cilkonly (TypeQual);

//#define EMPTY_TQ ((TypeQual){0,0})

GLOBAL int tq_equal(TypeQual, TypeQual);

// Always print a space at the end.
GLOBAL void TQtoGBUF(GBUF *gb, TypeQual tq);

/* For debugging.  Print a type qualifier. */
GLOBAL void DPTQ(TypeQual tq);

