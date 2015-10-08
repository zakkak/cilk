pkglib_PROGRAMS = cilk2c

cilk2c_SOURCES = analyze.c ast.c complex-types.c constexpr.c		\
container.c conversions.c dataflow.c elide.c initializer.c list.c	\
main.c operators.c output.c print-ast.c procedure.c profile.c		\
simplify.c sideeffects.c sem-check.c strings.c sue.c symbol.c		\
transform.c type.c type2.c typequal.c verify-parse.c warning.c gbuf.h	\
gbuf.c ast.h basics.h config.h conversions.h dataflow.h heap.h		\
initializer.h list.h operators.h symbol.h type.h typequal.h ANSI-C.y	\
c4.l hint.c astModify.h astModify.c stringParse.h stringParse.c		\
wildcard.h wildcard.c astEquals.h astEquals.c wildcardTable.h		\
wildcardTable.c testEquals.h testEquals.c atmTransform.h		\
atmTransform.c


AM_YFLAGS = -d

EXTRA_DIST = ANSI-C.c ANSI-C.h BUGS astgen .indent.pro
$(cilk2c_OBJECTS) : ANSI-C.c ANSI-C.h
