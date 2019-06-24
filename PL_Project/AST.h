#ifndef AST_H
#define AST_H

typedef char bool;
#define true 1
#define false 0


typedef enum
{eInt, eChar, eVoid} Type_e;
typedef enum
{eAssign, eRet, eWhile, eIf, eFunction} Stat_e;
typedef enum
{eEQ, eNE} Eqlt_e;



//prog 	::= 	vtype word "(" words ")" block 
//22
struct Prog {

	struct Vtype *vtype;
	char *word;
	struct Words *words;
	struct Block *block;
};
 
//decls 	::= 	decls decl 
//26
struct Decls {

	struct Decls *decls;
	struct Decl *decl;
	
};


//decl 	::= 	vtype words ";" ;
//27
struct Decl {
	struct Vtype *vtype;
	struct Words *words;
	
};


//words	:: = words "," word | word;
//24
struct Words {

	struct Words *words;
	char *word;

};

//vtype 	::= 	int | char| void ;
//23
struct Vtype {
	Type_e e;
};

//block	::=	"{" decls slist "}"	| ;
//25
struct Block {
	struct Decls *decls;
	struct Slist *slist;
	
};


//slist 	::= 	slist stat| stat;
//28
struct Slist {
	struct Slist *slist;
	struct Stat *stat;
	
};

//stat 	:: = IF cond THEN block ELSE block
//| WHILE cond block
//| word "=" expr ";"
//| word "(" words ")" ";"
//| RETURN expr ";";
//29
struct Stat {

	Stat_e e;

	struct Cond *cond;
	struct Block *block1;
	struct Block *block2;
	char *word;
	struct Expr *expr;
	struct Words *words;
		
};


//cond 	::= 	expr ">" expr
//			| 	expr "==" expr ;
//30
struct Cond {

	Eqlt_e e;
	struct Expr *expr1;
	struct Expr *expr2;	
		
};


//expr 	::= 	term
//| term "+" term;
//31
struct Expr {
	struct Term *term1;
	struct Term *term2;
	
};

//term 	::= 	fact
//| fact "*" fact;
//32
struct Term {
	struct Fact *fact1;
	struct Fact *fact2;
};

//fact 	::= 	num
//| word;
//33
struct Fact {
	int num;
	char *word;
};

#endif
