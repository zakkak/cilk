#ifndef ATM_TRANSFORM_H
#define ATM_TRANSFORM_H

/* atmTransform.h*/


GLOBAL Node *PreSimplify(Node *node);
GLOBAL List *PreSimplifyList(List *list);
GLOBAL List *AtomicTransformProgram(List *list);


#endif
