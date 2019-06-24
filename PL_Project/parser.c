#include <stdio.h>
#include "AST.h"
#include <string.h>
#include <stdlib.h>
#define STACK_SIZE 1000000
#define MAX_CODE_LEN 1000000

struct _STACK {
    int state;
    void *ptr;
}stack_parser[STACK_SIZE];
int stack_size = 0;
void stack_push(int state, void *ptr)
{
    stack_parser[stack_size].state = state;
    stack_parser[stack_size].ptr = ptr;
    ++stack_size;
}
struct _STACK stack_top()
{
    struct _STACK ret;
    ret.state = stack_parser[stack_size-1].state;
    ret.ptr = stack_parser[stack_size-1].ptr;
    return ret;
}
void stack_pop(){ --stack_size; }

struct Prog *program;

void parser()
{
    program = (struct Prog *)malloc(sizeof(struct Prog));
    program->vtype = NULL;
    program->word = NULL;
    program->words = NULL;
    program->block = NULL;
    int action;
    int code_ptr = 0;
    struct _STACK tmp;
    stack_push(action, NULL); // $ = 21
    //while(code[code_ptr].type != 21)
    while(1)
    {
        /*
        printf("code_ptr: %d, current_state: %d, stack_size: %d\n", code_ptr, stack_top().state, stack_size);
        printf("cur_text: %d\n", code[code_ptr].type);
        fflush(stdout);
        */

        tmp = stack_top();
        action = ParseTable[tmp.state][code[code_ptr].type];
        if(action == 10000){
            //accepted
            printf("parsing accepted\n");
            break;
        }
        else if(action >= 0 && action < 100) // why here?
        {
            //printf("parsing something gonna wrong...\n");
            //exit(0);
        }
        else if(action < 200) // shift
        {
            action -= 100;
            if(code[code_ptr].type == 0) // word
                stack_push(action, code[code_ptr].str);
            else if(code[code_ptr].type == 20) //num
                stack_push(action, &(code[code_ptr].value));
            else
                stack_push(action, NULL);
            ++code_ptr;
        }
        else if(action >= 200) // reduce
        {
            action -= 200;
            //prog -> vtype word "(" words ")" block
            if(action == 0){
                program->block = (struct Block*)stack_top().ptr; stack_pop();
                stack_pop(); // ")"
                program->words = (struct Words*)stack_top().ptr; stack_pop();
                stack_pop(); // "("
                program->word  = (char*)stack_top().ptr; stack_pop();
                program->vtype = (struct Vtype*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][22]; // prog = 22
                stack_push(action, program);
            }
            //decls -> decls decl
            else if(action == 1){
                struct Decls *decls = (struct Decls*)malloc(sizeof(struct Decls));
                decls->decl  = (struct Decl* )stack_top().ptr; stack_pop();
                decls->decls = (struct Decls*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][26]; // decls = 26
                stack_push(action, decls);
            }
            //decls -> 
            else if(action == 2){
                struct Decls *decls = (struct Decls*)malloc(sizeof(struct Decls));
                decls->decl  = NULL;
                decls->decls = NULL;
                action = ParseTable[stack_top().state][26]; // decls = 26
                stack_push(action, decls);
            }
            //decl -> vtype words ";"
            else if(action == 3){
                struct Decl *decl = (struct Decl*)malloc(sizeof(struct Decl));
                stack_pop(); // ";"
                decl->words = (struct Words*)stack_top().ptr; stack_pop();
                decl->vtype = (struct Vtype*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][27]; // decl = 27
                stack_push(action, decl);
            }
            //words -> words "," word
            else if(action == 4){
                struct Words *words = (struct Words*)malloc(sizeof(struct Words));
                words->word = (char*)stack_top().ptr; stack_pop();
                stack_pop(); // ","
                words->words = (struct Words*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][24]; // words = 24
                stack_push(action, words);
            }
            //words -> word
            else if(action == 5){
                struct Words *words = (struct Words*)malloc(sizeof(struct Words));
                words->word = (char*)stack_top().ptr; stack_pop();
                words->words = NULL;
                action = ParseTable[stack_top().state][24]; // words = 24
                stack_push(action, words);
            }
            //vtype -> "int"
            else if(action == 6){
                struct Vtype *vtype = (struct Vtype*)malloc(sizeof(struct Vtype));
                vtype->e = eInt; stack_pop();
                action = ParseTable[stack_top().state][23]; // vtype = 23
                stack_push(action, vtype);
            }
            //vtype -> "char"
            else if(action == 7){
                struct Vtype *vtype = (struct Vtype*)malloc(sizeof(struct Vtype));
                vtype->e = eChar; stack_pop();
                action = ParseTable[stack_top().state][23]; // vtype = 23
                stack_push(action, vtype);
            }
            //vtype -> "void"
            else if(action == 8){
                struct Vtype *vtype = (struct Vtype*)malloc(sizeof(struct Vtype));
                vtype->e = eVoid; stack_pop();
                action = ParseTable[stack_top().state][23]; // vtype = 23
                stack_push(action, vtype);
            }
            //block -> "{" decls slist "}"
            else if(action == 9){
                struct Block *block = (struct Block*)malloc(sizeof(struct Block));
                stack_pop(); // "}"
                block->slist = (struct Slist*)stack_top().ptr; stack_pop();
                block->decls = (struct Decls*)stack_top().ptr; stack_pop();
                stack_pop(); // "{"
                action = ParseTable[stack_top().state][25]; // block = 25
                stack_push(action, block);
            }
            //block -> 
            else if(action == 10){
                struct Block *block = (struct Block*)malloc(sizeof(struct Block));
                block->slist = NULL;
                block->decls = NULL;
                action = ParseTable[stack_top().state][25]; // block = 25
                stack_push(action, block);
            }
            //slist -> slist stat
            else if(action == 11){
                struct Slist *slist = (struct Slist*)malloc(sizeof(struct Slist));
                slist->stat  = (struct Stat *)stack_top().ptr; stack_pop();
                slist->slist = (struct Slist*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][28]; // slist = 28
                stack_push(action, slist);
            }
            //slist -> stat
            else if(action == 12){
                struct Slist *slist = (struct Slist*)malloc(sizeof(struct Slist));
                slist->stat  = (struct Stat *)stack_top().ptr; stack_pop();
                slist->slist = NULL;
                action = ParseTable[stack_top().state][28]; // slist = 28
                stack_push(action, slist);
            }
            //stat -> IF cond THEN block ELSE block
            else if(action == 13){
                struct Stat  *stat  = (struct Stat *)malloc(sizeof(struct Stat));
                stat->block2 = (struct Block*)stack_top().ptr; stack_pop();
                stack_pop(); // ELSE
                stat->block1 = (struct Block*)stack_top().ptr; stack_pop();
                stack_pop(); // THEN
                stat->cond = (struct Cond*)stack_top().ptr; stack_pop();
                stack_pop(); // IF
                stat->e = eIf;
                stat->word = NULL;
                stat->expr = NULL;
                stat->words = NULL;
                action = ParseTable[stack_top().state][29]; // stat = 29
                stack_push(action, stat);
            }
            //stat -> WHILE cond block
            else if(action == 14){
                struct Stat  *stat  = (struct Stat *)malloc(sizeof(struct Stat));
                stat->block1 = (struct Block*)stack_top().ptr; stack_pop();
                stat->cond = (struct Cond*)stack_top().ptr; stack_pop();
                stack_pop(); // WHILE
                stat->e = eWhile;
                stat->block2 = NULL;
                stat->word = NULL;
                stat->expr = NULL;
                stat->words = NULL;
                action = ParseTable[stack_top().state][29]; // stat = 29
                stack_push(action, stat);
            }
            //stat -> word "=" expr ";"
            else if(action == 15){
                struct Stat  *stat  = (struct Stat *)malloc(sizeof(struct Stat));
                stack_pop(); // ";"
                stat->expr = (struct Expr*)stack_top().ptr; stack_pop();
                stack_pop(); // "="
                stat->word = (char*)stack_top().ptr; stack_pop();
                stat->e = eAssign;
                stat->cond = NULL;
                stat->block1 = NULL;
                stat->block2 = NULL;
                stat->words = NULL;
                action = ParseTable[stack_top().state][29]; // stat = 29
                stack_push(action, stat);
            }
            //stat -> word "(" words ")" ";"
            else if(action == 16){
                struct Stat  *stat  = (struct Stat *)malloc(sizeof(struct Stat));
                stack_pop(); // ";"
                stack_pop(); // ")"
                stat->words = (struct Words*)stack_top().ptr; stack_pop();
                stack_pop(); // "("
                stat->word = (char*)stack_top().ptr; stack_pop();
                stat->e = eFunction;
                stat->cond = NULL;
                stat->block1 = NULL;
                stat->block2 = NULL;
                stat->expr = NULL;
                action = ParseTable[stack_top().state][29]; // stat = 29
                stack_push(action, stat);
            }
            //stat -> RETURN expr ";"
            else if(action == 17){
                struct Stat  *stat  = (struct Stat *)malloc(sizeof(struct Stat));
                stack_pop(); // ";"
                stat->expr = (struct Expr*)stack_top().ptr; stack_pop();
                stack_pop(); // RETURN
                stat->e = eRet;
                stat->cond = NULL;
                stat->block1 = NULL;
                stat->block2 = NULL;
                stat->word = NULL;
                stat->words = NULL;
                action = ParseTable[stack_top().state][29]; // stat = 29
                stack_push(action, stat);
            }
            //cond -> expr ">" expr
            else if(action == 18){
                struct Cond *cond = (struct Cond*)malloc(sizeof(struct Cond));
                cond->expr2 = (struct Expr*)stack_top().ptr; stack_pop();
                stack_pop(); // ">"
                cond->expr1 = (struct Expr*)stack_top().ptr; stack_pop();
                cond->e = eNE;
                action = ParseTable[stack_top().state][30]; // cond = 30
                stack_push(action, cond);
            }
            //cond -> expr "==" expr
            else if(action == 19){
                struct Cond *cond = (struct Cond*)malloc(sizeof(struct Cond));
                cond->expr2 = (struct Expr*)stack_top().ptr; stack_pop();
                stack_pop(); // "=="
                cond->expr1 = (struct Expr*)stack_top().ptr; stack_pop();
                cond->e = eEQ;
                action = ParseTable[stack_top().state][30]; // cond = 30
                stack_push(action, cond);
            }
            //expr -> term
            else if(action == 20){
                struct Expr *expr = (struct Expr*)malloc(sizeof(struct Expr));
                expr->term1 = (struct Term*)stack_top().ptr; stack_pop(); 
                expr->term2 = NULL;
                action = ParseTable[stack_top().state][31]; // expr = 31
                stack_push(action, expr);
            }
            //expr -> term "+" term
            else if(action == 21){
                struct Expr *expr = (struct Expr*)malloc(sizeof(struct Expr));
                expr->term2 = (struct Term*)stack_top().ptr; stack_pop();
                stack_pop(); // "+"
                expr->term1 = (struct Term*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][31]; // expr = 31
                stack_push(action, expr);
            }
            //term -> fact
            else if(action == 22){
                struct Term *term = (struct Term*)malloc(sizeof(struct Term));
                term->fact1 = (struct Fact*)stack_top().ptr; stack_pop();
                term->fact2 = NULL;
                action = ParseTable[stack_top().state][32]; // term = 32
                stack_push(action, term);
            }
            //term -> fact "*" fact
            else if(action == 23){
                struct Term *term = (struct Term*)malloc(sizeof(struct Term));
                term->fact2 = (struct Fact*)stack_top().ptr; stack_pop();
                stack_pop(); // "*"
                term->fact1 = (struct Fact*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][32]; // term = 32
                stack_push(action, term);
            }
            //fact -> num
            else if(action == 24){
                struct Fact *fact = (struct Fact*)malloc(sizeof(struct Fact));
                fact->num = *((int*)stack_top().ptr); stack_pop();
                fact->word = NULL;
                action = ParseTable[stack_top().state][33]; // fact = 33
                stack_push(action, fact);
            }
            //fact -> word
            else if(action == 25){
                struct Fact *fact = (struct Fact*)malloc(sizeof(struct Fact));
                fact->num = -1; // is null
                fact->word = (char*)stack_top().ptr; stack_pop();
                action = ParseTable[stack_top().state][33]; // fact = 33
                stack_push(action, fact);
            }
        }
        else // error
        {
            printf("parsing error\n");
            exit(0);
        }
    }
}

