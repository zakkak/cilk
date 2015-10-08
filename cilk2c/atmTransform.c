
/* atmTransform.c*/

#include "ast.h"
#include "stringParse.h"
#include "atmTransform.h"

/**
 * Code for Pre-simplifying the expressions
 *   before we do the Atomic transformation
 **/

PRIVATE Node *PreSimplifyNode(Node *node);

GLOBAL Node *PreSimplify(Node *node) {
  if (node == NULL) {
    printf("simplfiying null node! \n");
    return NULL;
  }

  /*  printf("presim root before:\n");*/
  /* PrintNode(stdout, node, 0);*/
  node = PreSimplifyNode(node);
  /*  printf("\npreSim root after \n");*/
  /* PrintNode(stdout, node, 0);*/
  /*  printf("\n");*/
  /*  printf("Now going to children \n");*/
#define CODE(node) node = PreSimplify(node)
  ASTWALK(node, CODE)
#undef CODE
      /*    printf("got here with a node... \n");*/
      /*  PrintNode(stdout, node, 0);*/
  return node;
}


/* This function is sort of a hack to set the types*/
/*  on the statements generated below*/
/*   when simplifying the preDec and postDec expressions*/
PRIVATE void SetAssignmentTypes(List* decls, List* stmts, Node* type) {

  Node* tempDecl = FirstItem(decls);
  Node* item1 = tempDecl->u.decl.init;
  Node* item2 = FirstItem(stmts);

  List* lastStmt = Rest(stmts);

  
/*    printf("Mkaing it to here... \n"); */

/*    PrintNode(stdout, item1, 0); */
/*    printf("\n item 2 sept. \n"); */
/*    PrintNode(stdout, item2, 0); */
/*    printf("\n"); */
  
  item1->u.binop.type = NodeCopy(type, Subtree);
  item2->u.binop.type = NodeCopy(type, Subtree);

  if (item1->typ == Binop) {
    item1->u.binop.type = NodeCopy(type, Subtree);
    item2->u.binop.right->u.id.decl = tempDecl;
  }
  else {
    Node* item3 = FirstItem(lastStmt);
    item2->u.binop.right->u.binop.type = NodeCopy(type, Subtree);
    item2->u.binop.right->u.binop.left->u.id.decl = tempDecl;
    item3->u.id.decl = tempDecl;
  }
}


PRIVATE Node *PreSimplifyNode(Node *node) {
  switch(node->typ) {
  case Binop:
    {
      Node* answer;
      
      switch(node->u.binop.op) {

      case ARROW:
	  /*	printf("Getting here, to an arrow operation. \n");*/
	  /*	PrintNode(stdout, node, 0);*/
	answer = ParseWildcardStringNode("(*%e1).%d1;", node->u.binop.left, GetNodeName(node->u.binop.right));
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.left->u.unary.type = NodeCopy(node->u.binop.type->u.ptr.type, Subtree);
	SetCoords(answer, node->coord, Subtree);
	return answer;

      case MULTassign:
	answer = ParseWildcardStringNode("%e1 = %e1 * %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;

      case DIVassign:
	answer = ParseWildcardStringNode("%e1 = %e1 / %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;

      case MODassign:
	answer = ParseWildcardStringNode("%e1 = %e1 % %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;

      case PLUSassign:
	answer = ParseWildcardStringNode("%e1 = %e1 + %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;

      case MINUSassign:
	answer = ParseWildcardStringNode("%e1 = %e1 - %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;
	
      case LSassign:
	answer = ParseWildcardStringNode("%e1 = %e1 << %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;
	
      case RSassign:
	answer = ParseWildcardStringNode("%e1 = %e1 >> %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;
	
      case ANDassign:
	answer = ParseWildcardStringNode("%e1 = %e1 & %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;
	
      case ERassign:
	answer = ParseWildcardStringNode("%e1 = %e1 ^ %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;
      case ORassign:
	answer = ParseWildcardStringNode("%e1 = %e1 | %e2;", node->u.binop.left, node->u.binop.right);
	answer->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	answer->u.binop.right->u.binop.type = NodeCopy(node->u.binop.type, Subtree);
	return answer;
	
      default:
	return node;
      }	    
    }
  case Unary: {
    Node *answer;
    Node *copyType;
    switch(node->u.unary.op) {
    case POSTINC:
      copyType = NodeCopy(node->u.unary.type, Subtree);
      copyType->coord = node->coord;
      answer = ParseWildcardStringNode("({%t _tempVal = %e1; %e1 = _tempVal + 1; _tempVal;});",
				       copyType,
				       node->u.unary.expr);
      SetAssignmentTypes(answer->u.Block.decl, answer->u.Block.stmts, copyType);
      break;
 
    case POSTDEC:
      copyType = NodeCopy(node->u.unary.type, Subtree);
      copyType->coord = node->coord;
      answer = ParseWildcardStringNode("({%t _tempVal = %e1; %e1 = _tempVal - 1; _tempVal;});",
				       copyType,
				       node->u.unary.expr);
      SetAssignmentTypes(answer->u.Block.decl, answer->u.Block.stmts, copyType);
      break;
      
    case PREINC:
      copyType = NodeCopy(node->u.unary.type, Subtree);
      copyType->coord = node->coord;
      answer = ParseWildcardStringNode("({%t _tempVal = %e1 + 1; %e1 = _tempVal;});",
				       copyType,
				       node->u.unary.expr);
      SetAssignmentTypes(answer->u.Block.decl, answer->u.Block.stmts, copyType);
      break;
      
    case PREDEC:
      copyType = NodeCopy(node->u.unary.type, Subtree);
      copyType->coord = node->coord;
      answer = ParseWildcardStringNode("({%t _tempVal = %e1 - 1; %e1 = _tempVal;});",
				       copyType,
				       node->u.unary.expr);
      SetAssignmentTypes(answer->u.Block.decl, answer->u.Block.stmts, copyType);
      break;
    default:
      answer =  node;
    }

    answer = SetCoords(answer, node->coord, Subtree);
    return answer;
  }
  case Array:
    {
      Node *answer;
      Coord oldCoord = node->coord;
      arrayNode u = node->u.array;
      if (ListLength(u.dims) == 1) {
	answer =  ParseWildcardStringNode("*(%e1 + %e2);", u.name, (Node *)FirstItem(u.dims));
      }
      else {
	answer = PreSimplifyNode(MakeArray(ParseWildcardStringNode("*(%e1 + %e2);", u.name, (Node *)FirstItem(u.dims)),
					 Rest(u.dims)));
      }

      answer->coord = oldCoord;

      answer = SemCheckNode(answer);
      return answer;
    }

    
  default:
    return node;
  }

}



GLOBAL List *PreSimplifyList(List *list)
{
     List *aptr;

     for (aptr = list; aptr; aptr = Rest(aptr)) {
	  Node *item = FirstItem(aptr);

	  SetItem(aptr, PreSimplify(item));
     }

     /*     printf("\nBefore returning list... \n");*/
     /*     PrintList(stdout, list, 0);*/
     /*   printf("\n");*/
     return list;
}


/*******************************************************************/



/***
 * User needs to provide these functions to generate
 *  whatever code necessary for LD/ST
 */
PRIVATE Node *MakeST_TRANSFORM(Node* address, Node* size, Node *type, Node* value,
			       Bool insideTransaction) {
/*  Node *answer; */

/*    printf("executing this here... \n"); */
/*    PrintNode(stdout, address, 0); */
/*    printf("\n"); */

/*    printf("Size: \n"); */
/*    PrintNode(stdout, size, 0); */
/*    printf("\n"); */

  
/*    printf("type: \n"); */
/*    PrintNode(stdout, type, 0); */
/*    printf("\n"); */

  
/*    answer = ParseWildcardStringNode("(%t1 ) %d1(%e1, %e2, %e3);", */
/*  				   MakePtr(EMPTY_TQ, type), */
/*  				   "ST_TRANSFORM", */
/*  				   address, */
/*  				   size, */
/*  				   value); */

    
/*    answer = ParseWildcardStringNode("(%t1 ) %d1(%e1, %e2, %e3);", */
/*  				   type, */
/*  				   "ST_TRANSFORM", */
/*  				   address, */
/*  				   size, */
/*  				   value); */


  type = type;

  
  if (insideTransaction) {
    return ParseWildcardStringNode("({%t _tempST = %e1; if (!%d1(%e2, &_tempST, %e3)) %e4; _tempST; });",
				   type,
				   value,
				   "xST",
				   address, 
				   size,
				   MakeGoto(MakeLabel("failed")));
    
  }
  else {
    return ParseWildcardStringNode("({%t _tempST = %e1; %d1(%e2, &_tempST, %e3);  _tempST; });",
				   type,
				   value,
				   "ST",
				   address, 
				   size);
  }
}

PRIVATE Node *MakeLD_TRANSFORM(Node* address, Node* size, Node *type,
			       Node *UNUSED(failedLabel),
			       Bool insideTransaction) {
    /*  return ParseWildcardStringNode("(%t1 ) %d1(%e1, %e2);", MakePtr(EMPTY_TQ, type), "LD_TRANSFORM", address, size);*/
    /*  return ParseWildcardStringNode("(%t1 ) %d1(%e1, %e2);", type, "LD_TRANSFORM", address, size);*/

  type = type;

  if (insideTransaction) {
    return ParseWildcardStringNode("({%t _tempVar; if (!%d1(&_tempVar, %e1, %e2)) %e3; _tempVar;});",
				   type,
				   "xLD",
				   address, 
				   size,
				   MakeGoto(MakeLabel("failed")));
    
    
    /*return ParseWildcardStringNode("({if (x < 0) x++;});");*/
  }
  else {
      return ParseWildcardStringNode("({%t _tempVar; %d1(&_tempVar, %e1, %e2); _tempVar;});",
				     type,
				     "LD",
				     address,
				     size);
  }
  

}

/******************************************************************/

/**
 *Functions/variables we use for keeping track of 
 *  the scope of variables.
 *
 */


/* The symbol table*/
PRIVATE SymbolTable *currentTable;

/* Current scope depth in our atomic transaction*/
PRIVATE int CurrentTableDepth;

/* The depth where we began declaring a function.*/
/*   (this should really be renamed)*/
PRIVATE int AtomicStartDepth = 0;

PRIVATE Node* currentFailedLabel;
PRIVATE Bool insideTransaction = FALSE;

/* Whenever we see an identifier, we add*/
/*   a pointer to an int to the table.*/
/*   The int is the current level.*/

PRIVATE int* ScopePtrTable[MAX_SCOPE_DEPTH + 1];
PRIVATE int ScopeNumberTable[MAX_SCOPE_DEPTH + 1];

/**
 *Initialization function.  Only needs to be done once.
 */
PRIVATE void InitScopeNumTable() {
  int i;
  for (i = 0; i < MAX_SCOPE_DEPTH + 1; i++) {
    ScopeNumberTable[i] = i;
    ScopePtrTable[i] = &(ScopeNumberTable[i]);
  }
}



/**
 *Function takes in a List of Decls. It adds
 *  all the declarations to the table,
 *  at the level specified by CurrentTableDepth
 */
PRIVATE void AddDeclsToTable(List* declList, int currentDepth) {
  ListMarker lWalk;
  Node* currentNode = NULL;

  if (declList != NULL) {
    IterateList(&lWalk, declList);
    while (!EndOfList(&lWalk)) {
      NextOnList(&lWalk, (GenericREF)&currentNode);
      if (currentNode->typ != Prim) {
	printf("Inserting Decl %s at level %d \n", currentNode->u.decl.name, *ScopePtrTable[currentDepth]);
	InsertSymbol(currentTable, currentNode->u.decl.name, ScopePtrTable[currentDepth], NULL);
      }
    }
  }
}



/* Function to call when we exit a scope*/
/*   in the symbol table, CurrentTable*/
PRIVATE void exitLevelTable(int* level) {
  assert(*level > CurrentTableDepth);
}



/**
 *Returns TRUE if there is a variable VAR_NAME 
 *  in the symbol table that is local.
 *  Local in this case means
 *   it was declared within the atomic transaction
 *   (variable's level >= AtomicStartDepth)
 */
PRIVATE Bool VariableIsLocal(const char* varName) {
  Bool isLocalVar = FALSE;
  int* lookupLevel;
  
  if (LookupSymbol(currentTable,
		   varName,
		   (GenericREF)&lookupLevel)) {
    printf("Found symbol %s at depth %d \n",
	   varName,
	   *lookupLevel);
    printf("Atomic start depth is: %d \n", AtomicStartDepth);
    isLocalVar = (*lookupLevel >= AtomicStartDepth );
    printf("Is local? %d \n", isLocalVar);	  
  }

  return isLocalVar;
}


/************************************************************/
/* The functions that do the atomic transform.*/

PRIVATE Node* AtomicTransformHelper(Node *node);
GLOBAL List *AtomicTransformList(List *list);
     
GLOBAL Node* AtomicTransform(Node *node) {
 
  return AtomicTransformHelper(node);
}


PRIVATE Node* MakeXBlockIntoTransaction(Node *node) {
  Node* acContextType;
  Node* acContextDecl;
  Node* attemptTransLabel = MakeLabel("attempt_transaction");
  Node* failedLabel = MakeLabel("failed");
  Node* commitLabel = MakeLabel("commit");
  Node* gotoAttemptNode = MakeGoto(attemptTransLabel);
  Node* gotoCommitNode = MakeGoto(commitLabel);

  List* failedStatements;
  List* commitStatements;


  currentFailedLabel = failedLabel;
  
  /* Change an Xblock into a normal block*/
  node = MakeBlockCoord(node->u.xBlock.type,
			node->u.xBlock.decl,
			node->u.xBlock.stmts,
			node->coord,
			node->u.xBlock.right_coord);

 
    
  acContextType = MakeTdef(EMPTY_TQ, "AtomicContext");
  acContextDecl = ParseWildcardStringNode("%t* ac = createAtomicContext();", acContextType);
  node->u.xBlock.decl = ConsItem(acContextDecl, node->u.xBlock.decl);

  node->u.xBlock.stmts = ConsItem(ParseWildcardStringNode("initTransaction(ac);"),
				  node->u.xBlock.stmts);
  node->u.xBlock.stmts = ConsItem(attemptTransLabel,
				  node->u.xBlock.stmts);

  failedStatements = List5(gotoCommitNode,
			   failedLabel,
			   ParseStringIntoASTNode("doAbort(ac);"),
			   ParseStringIntoASTNode("doBackoff(ac);"),
			   gotoAttemptNode);
			  
			   
  commitStatements = List3(commitLabel,
			   ParseStringIntoASTNode("doCommit(ac);"),
			   ParseStringIntoASTNode("destroyAtomicContext(ac);"));

  failedStatements = JoinLists(failedStatements, commitStatements);
  node->u.xBlock.stmts = JoinLists(node->u.xBlock.stmts, failedStatements);

  return node;
  
}


/** Transforming an atomic block of code
 *
 */
PRIVATE Node* AtomicTransformXBlock(Node *node) {

  Node* answer;

  CurrentTableDepth++;
  EnterScope();

  printf("Increase recursion depth to %d\n", CurrentTableDepth);
  AddDeclsToTable(node->u.xBlock.decl, CurrentTableDepth);
  PrintSymbolTable(stdout, currentTable);

  insideTransaction = TRUE;

  /* recurse*/
#define CODE(node) node = AtomicTransformHelper(node)
    ASTWALK(node, CODE)
#undef CODE
      answer = node;

    CurrentTableDepth--;
    ExitScope();

    insideTransaction = FALSE;

    printf("AFter recursion: \n");
    PrintSymbolTable(stdout, currentTable);
    printf("Decrease recursion depth to %d\n", CurrentTableDepth);

    answer = MakeXBlockIntoTransaction(node);
			      
    return answer;
}


/* This is exactly the same as AtomicTransformXBlock,*/
PRIVATE Node* AtomicTransformBlock(Node *node) {

  Node* answer;

  CurrentTableDepth++;
  EnterScope();
  printf("Increase recursion depth to %d\n", CurrentTableDepth);
  AddDeclsToTable(node->u.Block.decl, CurrentTableDepth);
  PrintSymbolTable(stdout, currentTable);

  /* recurse*/
#define CODE(node) node = AtomicTransformHelper(node)
    ASTWALK(node, CODE)
#undef CODE
      answer = node;

    CurrentTableDepth--;
    ExitScope();
    printf("AFter recursion: \n");
    PrintSymbolTable(stdout, currentTable);
    printf("Decrease recursion depth to %d\n", CurrentTableDepth);
    return answer;
}

PRIVATE Node* AtomicTransformProc(Node *node) {
  Node* theDecl = node->u.proc.decl;

  
  int oldDepth = AtomicStartDepth;
  AtomicStartDepth = CurrentTableDepth+1;
  /*PrintNode(stdout, theDecl->u.decl.type, 0);*/
  if (theDecl != NULL) {
    fPrintNode(stdout, theDecl->u.decl.type, 0);
    AddDeclsToTable(theDecl->u.decl.type->u.fdcl.args, CurrentTableDepth+1);
  }
  PrintSymbolTable(stdout, currentTable);

  /* recurse on body*/
  node->u.proc.body = AtomicTransformHelper(node->u.proc.body);

  AtomicStartDepth = oldDepth;
  
  return node;
}


PRIVATE Node* AtomicTransformBinop(Node *node) {
  Node *answer;
  Node *oldLeftSide = node->u.binop.left;
  Node *oldRightSide = node->u.binop.right;
  Node *copyType = node->u.binop.type;
  Node *sizeofType;
  Node *address;
  Node *newRight;
  Bool doAtomicTransform = TRUE;

  if (copyType == NULL) {
    printf("Node is: \n");
    fPrintNode(stdout, node, 0);
    printf("\n");
    FAIL("Binop node does not have type field filled in;");
  }

  sizeofType = ParseWildcardStringNode("sizeof (%t1);", copyType);
					   

  switch(node->u.binop.op) {

  case '=':
      /* Anything on the left side which is *(...)*/
    if ((oldLeftSide->typ == Unary) && (oldLeftSide->u.unary.op == INDIR)) {

	/*  We have *a*/
      if (oldLeftSide->u.unary.expr->typ == Id) {
	address = ParseWildcardStringNode("%e1;", oldLeftSide->u.unary.expr);
      }
      else {
	address = ParseWildcardStringNode("%e1;", AtomicTransformHelper(oldLeftSide->u.unary.expr));
      }
    }
    else {
      address = ParseWildcardStringNode("&%e1;", oldLeftSide);
      if (oldLeftSide->typ == Id) {
	doAtomicTransform = !VariableIsLocal(oldLeftSide->u.id.text);	
      }
      else {
	if ((oldLeftSide->typ == Binop)
	    && (oldLeftSide->u.binop.op == '.')) {
	  if (oldLeftSide->u.binop.left->typ == Id) {
	    doAtomicTransform = !VariableIsLocal(oldLeftSide->u.binop.left->u.id.text);
	  }
	}
      }
    }


    newRight = AtomicTransformHelper(oldRightSide);
    if (doAtomicTransform) {
      answer = MakeST_TRANSFORM(address, sizeofType, copyType, newRight, insideTransaction);
    }
    else {
      answer = node;
    }

    /*   printf("The answer : \n");*/
    /*  PrintNode(stdout, answer, 0);*/
    /*  printf("\n");*/
    
    return SetCoords(answer, node->coord, Subtree);
 
  case '.':
      /* "Anything "a.(rest) tran*/
    if (oldLeftSide->typ == Id) {
      if (VariableIsLocal(oldLeftSide->u.binop.left->u.id.text)) {
	return node;
      }
      else {
	return MakeLD_TRANSFORM(ParseWildcardStringNode("&%e1;", node),
				sizeofType,
				copyType,
				currentFailedLabel,
				insideTransaction);
      }
    }
    else {
      return MakeBinopCoord('.',
			    MakeLD_TRANSFORM(ParseWildcardStringNode("&%e1;",
								     AtomicTransformHelper(oldLeftSide)),
					     sizeofType,
					     copyType,
					     currentFailedLabel,
					     insideTransaction),
			    oldRightSide,
			    node->coord);
    }
  default:
      /* recurse in default case.*/
#define CODE(node) node = AtomicTransformHelper(node)
    ASTWALK(node, CODE)
#undef CODE
      return node;
  }
}

PRIVATE Node* AtomicTransformId(Node *node) {
  Node* copyType;    
  Node* sizeofType;
  Node* answer;
  Bool doAtomicTransform = TRUE;

  if (node->u.id.decl == NULL) {
    printf("Node is: \n");
    fPrintNode(stdout, node, 0);
    FAIL("Id node has no decl field. \n");
  }
  
  copyType = node->u.id.decl->u.decl.type;
  if (copyType == NULL) {
    FAIL("Id node's decl field has no type. \n");
  }
  else{
    sizeofType = ParseWildcardStringNode("sizeof (%t1);", copyType);
  }


  doAtomicTransform = !VariableIsLocal(node->u.id.text);
  printf("Finding a LD variable. Doing transform is %d \n", doAtomicTransform);

  if (VariableIsLocal(node->u.id.text)) {
    answer = node;
  }
  else {
    answer = MakeLD_TRANSFORM(ParseWildcardStringNode("&%e1;",node),
			      sizeofType,
			      copyType,
			      currentFailedLabel,
			      insideTransaction);

  }
  return SetCoords(answer, node->coord, Subtree);
}

PRIVATE Node* AtomicTransformUnary(Node *node) {
  Node *answer;
  Node *copyType = node->u.unary.type;
  Node *sizeofType;

  if (copyType == NULL) {
    FAIL("Unary node doesn't have type field set. \n");
  }
  else {
    sizeofType = ParseWildcardStringNode("sizeof (%t1);", node->u.unary.type);
  }
  switch(node->u.unary.op) {
  case INDIR:
      answer = MakeLD_TRANSFORM(ParseWildcardStringNode("%e1;", AtomicTransformHelper(node->u.unary.expr)),
				sizeofType,
				copyType,
				currentFailedLabel,
				insideTransaction);

    break;

  case ADDRESS:
    answer = node;
    if (node->u.unary.expr->typ == Unary) {
      if (node->u.unary.expr->u.unary.op == INDIR) {
	printf("Double &*... \n");
	answer = AtomicTransformHelper(node->u.unary.expr->u.unary.expr);
      }
    }

    break;
  default:
#define CODE(node) node = AtomicTransformHelper(node)
    ASTWALK(node, CODE)
#undef CODE
      answer = node;
  }

  return SetCoords(answer, node->coord, Subtree);
}


PRIVATE Node *AtomicTransformCall(Node *node) {
  
  Node* name = node->u.call.name;
  printf("The function we are calling is: %s \n", name->u.id.text);

  node->u.call.args = AtomicTransformList(node->u.call.args);

  return node;
}

GLOBAL Node *AtomicTransformHelper(Node *node) {
  if (node == NULL) {
    return NULL;
  }
  
  switch(node->typ) {
  case Binop: 
    return AtomicTransformBinop(node);
  case Id: 
    return AtomicTransformId(node);		    
  case Unary:
    return AtomicTransformUnary(node);
  case XBlock:
    return AtomicTransformXBlock(node);
  case Block:
    return AtomicTransformBlock(node);
  case Call:
    printf("We are doign a call transform \n");
    return AtomicTransformCall(node);
  case Proc:
    return AtomicTransformProc(node);
  case Array:
    FAIL("Should be no array accesses left. \n");
    return node;
  default:
      /* In the other cases, we have to recurse.*/
#define CODE(node) node = AtomicTransformHelper(node)
  ASTWALK(node, CODE)
#undef CODE
    return node;
  }
}




GLOBAL List *AtomicTransformList(List *list)
{
     List *aptr;
     for (aptr = list; aptr; aptr = Rest(aptr)) {
	  Node *item = FirstItem(aptr);

	  SetItem(aptr, AtomicTransform(item));
     }

     /*     printf("\nBefore returning list... \n");*/
     /*     PrintList(stdout, list, 0);*/
     /*   printf("\n");*/
     return list;
}


GLOBAL List *AtomicTransformProgram(List *program) {
  currentTable = NewSymbolTable("AtomicID", Nested, NULL,
				(ExitscopeProc) exitLevelTable);
  CurrentTableDepth = 0;
  InitScopeNumTable();

  return AtomicTransformList(program);
}

