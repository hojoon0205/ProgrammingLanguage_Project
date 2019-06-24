#include <stdio.h>
#include <stdlib.h>
#include "scanner.c"
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "AST.h"
#include "parsingtable.c"
#include "parser.c"

#define INPUT_CODE_SIZE 10000
#define MAX_BLOCK_SIZE 10
#define MAX_VAR_NUM 10
#define MAX_REGISTER 1000
#define MAX_PATH_LEN 10000

char input[INPUT_CODE_SIZE];
struct Input code[500000];

FILE *assm_out;
FILE *symb_out;

int cur_reg_num = 0, minimum;
int cur_line_num = 0;
int block_num[MAX_BLOCK_SIZE], block_size = 0, block_ss = 0;
int N_label = 0;

struct PL{
    int assmtype;
    /* LD 0
     * ST 1
     * ADD 2
     * MUL 3
     * LT 4
     * CALL 5
     * JUMPF 6
     * JUMPT 7
     * JUMP 8
     * EQ 9 (==)      EQ Reg#1, Reg#2, Reg#3     1 if (Reg#2==Reg#3), 0 otherwise, store into Reg#1
     * LABEL: 10      LABEL: label               define label's position
     * RET 11         RET Reg#1                  return Reg#1
     */
    int reg1, reg2, reg3;
    int label;
    int num;
    char *addr;
}print_list[INPUT_CODE_SIZE];

int lifetime[MAX_REGISTER][2] = {0}; //0 for birth 1 for dead
int graph[MAX_REGISTER][MAX_REGISTER] = {0};

void set_print_list(int assmtype, int reg1, int reg2, int reg3, int label, int num, char* addr)
{
    ++cur_line_num;
    print_list[cur_line_num].assmtype = assmtype;
    print_list[cur_line_num].reg1 = reg1;
    print_list[cur_line_num].reg2 = reg2;
    print_list[cur_line_num].reg3 = reg3;
    print_list[cur_line_num].label = label;
    print_list[cur_line_num].num = num;
    print_list[cur_line_num].addr = addr;
    return;
}

void print_the_list()
{
    int i;
    for(i=1; i<=cur_line_num; ++i)
    {
        struct PL t = print_list[i];
        int type = t.assmtype;
        if(type == 0){
            fprintf(assm_out, "  LD\tReg#%d, ", t.reg1);
            if(t.num==-1) fprintf(assm_out, "%s\n", t.addr);
            else fprintf(assm_out, "%d\n", t.num);
        }
        else if(type == 1){
            fprintf(assm_out, "  ST\tReg#%d, %s\n", t.reg1, t.addr);
        }
        else if(type == 2){
            fprintf(assm_out, "  ADD\tReg#%d, Reg#%d, Reg#%d\n", t.reg1, t.reg2, t.reg3);
        }
        else if(type == 3){
            fprintf(assm_out, "  MUL\tReg#%d, Reg#%d, Reg#%d\n", t.reg1, t.reg2, t.reg3);
        }
        else if(type == 4){
            fprintf(assm_out, "  LT\tReg#%d, Reg#%d, Reg#%d\n", t.reg1, t.reg2, t.reg3);
        }
        else if(type == 5){
            fprintf(assm_out, "  CALL\t%s\n", t.addr);
        }
        else if(type == 6){
            fprintf(assm_out, "  JUMPF\tReg#%d\t%d\n", t.reg1, t.label);
        }
        else if(type == 7){
            fprintf(assm_out, "  JUMPT\tReg#%d\t%d\n", t.reg1, t.label);
        }
        else if(type == 8){
            fprintf(assm_out, "  JUMP\t\t%d\n", t.label);
        }
        else if(type == 9){
            fprintf(assm_out, "  EQ\tReg#%d, Reg#%d, Reg#%d\n", t.reg1, t.reg2, t.reg3);
        }
        else if(type == 10){
            fprintf(assm_out, "LABEL:%d\n", t.label);
        }
        else if(type == 11){
            fprintf(assm_out, "  RET\tReg#%d\n", t.reg1);
        }
    }
}

#define CUR_BLOCK_NUM (block_num[block_ss])

void _Prog (struct Prog *,char*);
void _Decls(struct Decls*,char*);
void _Decl (struct Decl *,char*);
void _Words(struct Words*,char*);
void _Vtype(struct Vtype*,char*);
void _Block(struct Block*,char*);
void _Slist(struct Slist*,char*);
void _Stat (struct Stat *,char*);
int  _Cond (struct Cond *,char*);
int  _Expr (struct Expr *,char*);
int  _Term (struct Term *,char*);
int  _Fact (struct Fact *,char*);

int reg_stack[MAX_REGISTER] = {0}, reg_ss = 0;
int check[MAX_REGISTER] = {0};
int change[MAX_REGISTER] = {0};

void register_optimization()
{
    //lifetime[regnum][0] , lifetime[regnum][1]
    //make lifetime table
    int reg, i, j;
    for(i=1; i<=cur_line_num; ++i)
    {
        reg = print_list[i].reg1;
        if(reg==-1) continue;
        if(lifetime[reg][0] == 0) lifetime[reg][0] = i;
        lifetime[reg][1] = i;

        reg = print_list[i].reg2;
        if(reg==-1) continue;
        if(lifetime[reg][0] == 0) lifetime[reg][0] = i;
        lifetime[reg][1] = i;

        reg = print_list[i].reg3;
        if(reg==-1) continue;
        if(lifetime[reg][0] == 0) lifetime[reg][0] = i;
        lifetime[reg][1] = i;
    }
    int l = 1, r = cur_reg_num, mid;
    int remain;
    while(l<=r)
    {
        mid = (l+r)/2;
        remain = cur_reg_num;
        //make graph
        for(i=1; i<cur_reg_num; ++i)
        {
            for(j=i+1; j<=cur_reg_num; ++j)
            {
                if(lifetime[i][1]<lifetime[j][0] || lifetime[j][1]<lifetime[i][0])
                    continue;
                graph[i][j] = graph[j][i] = 1;
            }
        }
        for(i=1; i<=cur_reg_num; ++i) check[i] = 0;
        while(remain)
        {
            int reduced = 0;
            for(i=1; i<=cur_reg_num; ++i)
            {
                if(check[i]) continue;
                int s = 0;
                for(j=1; j<=cur_reg_num; ++j) s += graph[i][j];
                if(s >= mid) continue;
                reduced = 1;
                --remain;
                for(j=1; j<=cur_reg_num; ++j)
                {
                    if(graph[i][j])
                        graph[i][j] = graph[j][i] = 0;
                }
                check[i] = 1;
            }
            if(reduced == 0) break;
        }
        if(remain){ l = mid+1; }
        else{ minimum = mid; r = mid-1; }
    }

    remain = cur_reg_num;
    for(i=1; i<cur_reg_num; ++i)
    {
        for(j=i+1; j<=cur_reg_num; ++j)
        {
            if(lifetime[i][1]<lifetime[j][0] || lifetime[j][1]<lifetime[i][0])
                continue;
            graph[i][j] = graph[j][i] = 1;
        }
    }
    for(i=1; i<=cur_reg_num; ++i) check[i] = 0;
    while(remain)
    {
        for(i=1; i<=cur_reg_num; ++i)
        {
            if(check[i]) continue;
            int s = 0;
            for(j=1; j<=cur_reg_num; ++j) s += (graph[i][j]==1?1:0);
            if(s >= minimum) continue;
            --remain;
            for(j=1; j<=cur_reg_num; ++j)
            {
                if(graph[i][j])
                    graph[i][j] = graph[j][i] = -1;
            }
            check[i] = 1;
            reg_stack[reg_ss++] = i;
        }
    }
    while(reg_ss)
    {
        int node = reg_stack[reg_ss-1];
        for(i=1; i<=minimum; ++i)
        {
            for(j=1; j<=cur_reg_num; ++j)
            {
                if(graph[node][j]==0) continue;
                if(i == change[j]) break;
            }
            if(j>cur_reg_num){
                change[node] = i;
                break;
            }
        }
        --reg_ss;
    }
    for(i=1; i<=cur_line_num; ++i)
    {
        reg = print_list[i].reg1;
        if(reg==-1) continue;
        print_list[i].reg1 = change[reg];

        reg = print_list[i].reg2;
        if(reg==-1) continue;
        print_list[i].reg2 = change[reg];

        reg = print_list[i].reg3;
        if(reg==-1) continue;
        print_list[i].reg3 = change[reg];
    }
    return;
}

char loc[MAX_PATH_LEN] = "global";
int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 2){
        printf("usage: ./main <your_input_file_name>\n");
        return 0;
    }
    assm_out = fopen("myprogram.code","w");
    symb_out = fopen("myprogram.symbol","w");
    int fd;
    fd = open(argv[1],0,0644);
    read(fd,input,INPUT_CODE_SIZE);
    scanner(input);
    Table();
    parser();
    //printf("asdf\n"); fflush(stdout);
    _Prog(program, loc);
    fprintf(assm_out, "\n# number of registers: %d\n", minimum);
}
void _Prog(struct Prog *prog, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(prog == NULL){ printf("prog not found\n"); fflush(stdout); exit(0); }
    _Vtype(prog->vtype, Loc);
    fprintf(assm_out, "BEGIN %s\n", prog->word);
    fprintf(symb_out, "name:%s\ttype:function\tscope:%s\n", prog->word, loc);
    sprintf(Loc, "%s->%s", loc, prog->word);
    _Words(prog->words, Loc);
    _Block(prog->block, Loc);
    printf("Changed into Pseudo-Assembly Code\n");
    register_optimization();
    print_the_list();
    fprintf(assm_out, "END %s\n", prog->word);
}

void _Decls(struct Decls *decls, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(decls == NULL){ printf("decls not found\n"); fflush(stdout); exit(0); }
    if(decls->decls) _Decls(decls->decls, Loc);
    if(decls->decl ) _Decl(decls->decl, Loc);
}

void _Decl(struct Decl *decl, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(decl == NULL){ printf("decl not found\n"); fflush(stdout); exit(0); }
    _Vtype(decl->vtype, Loc);
    _Words(decl->words, Loc);
}

void _Words(struct Words *words, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(words == NULL){ printf("words not found\n"); fflush(stdout); exit(0); }
    if(words->words) _Words(words->words, Loc);
    fprintf(symb_out, "name:%s\ttype:variable\tscope:%s\n", words->word, Loc);
}

void _Vtype(struct Vtype *vtype, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(vtype == NULL){ printf("vtype not found\n"); fflush(stdout); exit(0); }
    if(vtype->e == eInt) fprintf(symb_out, "type:int\n");
    if(vtype->e == eChar) fprintf(symb_out, "type:char\n");
    if(vtype->e == eVoid) fprintf(symb_out, "type:void\n");
}
void _Block(struct Block *block, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(block == NULL){ printf("block not found\n"); fflush(stdout); exit(0); }
    int cur_block_num = ++block_size;
    block_num[++block_ss] = cur_block_num;
    if(block->decls) _Decls(block->decls, Loc);
    if(block->slist) _Slist(block->slist, Loc);
    --block_ss;
}
void _Slist(struct Slist *slist, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(slist == NULL){ printf("slist not found\n"); fflush(stdout); exit(0); }
    if(slist->slist) _Slist(slist->slist, Loc);
    _Stat(slist->stat, Loc);
}
void _Stat(struct Stat *stat, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    char Loc2[MAX_PATH_LEN]; strcpy(Loc2, loc);
    if(stat->e==eIf) // IF
    {
        int after_if_label = ++N_label;
        int ret_reg_num = _Cond(stat->cond, Loc);
        int false_label = ++N_label;
        // JUMPF Reg#ret_reg_num false_label
        set_print_list(6, ret_reg_num, -1, -1, false_label, -1, NULL);
        sprintf(Loc, "%s->IF_THEN", loc);
        _Block(stat->block1, Loc);
        // JUMP: after_if_label
        set_print_list(8, -1, -1, -1, after_if_label, -1, NULL);
        // LABEL: false_label
        set_print_list(10, -1, -1, -1, false_label, -1, NULL);
        sprintf(Loc2,"%s->IF_ELSE", loc);
        _Block(stat->block2, Loc2);
        // LABEL: after_if_label
        set_print_list(10, -1, -1, -1, after_if_label, -1, NULL);
    }
    else if(stat->e==eWhile) // WHILE
    {
        int test_label = ++N_label;
        int repeat_label = ++N_label;
        // JUMP test_label
        set_print_list(8, -1, -1, -1, test_label, -1, NULL);
        // LABEL: repeat_label
        set_print_list(10, -1, -1, -1, repeat_label, -1, NULL);
        sprintf(Loc, "%s->WHILE", loc);
        _Block(stat->block1, Loc);
        // LABEL: test_label
        set_print_list(10, -1, -1, -1, test_label, -1, NULL);
        int ret_reg_num = _Cond(stat->cond, Loc);
        // JUMPT Reg#ret_reg_num repeat_label
        set_print_list(7, ret_reg_num, -1, -1, repeat_label, -1, NULL);
    }
    else if(stat->e==eAssign)
    {
        int ret_reg_num = _Expr(stat->expr, Loc);
        // ST ret_reg_num stat->word
        set_print_list(1, ret_reg_num, -1, -1, -1, -1, stat->word);
    }
    else if(stat->e==eFunction)
    {
        // CALL function_name
        set_print_list(5, -1, -1, -1, -1, -1, stat->word);
        _Words(stat->words, Loc);
    }
    else if(stat->e==eRet)
    {
        int ret_reg_num = _Expr(stat->expr, Loc);
        // RET ret_reg_num
        set_print_list(11, ret_reg_num, -1, -1, -1, -1, NULL);
    }
}
int _Cond(struct Cond *cond, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(cond->e==eNE)
    {
        int ret_reg_num1 = _Expr(cond->expr1, Loc);
        int ret_reg_num2 = _Expr(cond->expr2, Loc);
        int for_return = ++cur_reg_num;
        // LT for_return, ret_reg_num2, ret_reg_num1
        set_print_list(4, for_return, ret_reg_num2, ret_reg_num1, -1, -1, NULL);
        return for_return;
    }
    else if(cond->e==eEQ)
    {
        int ret_reg_num1 = _Expr(cond->expr1, Loc);
        int ret_reg_num2 = _Expr(cond->expr2, Loc);
        int for_return = ++cur_reg_num;
        // EQ for_return, ret_reg_num1, ret_reg_num2
        set_print_list(9, for_return, ret_reg_num1, ret_reg_num2, -1, -1, NULL);
        return for_return;
    }
}
int _Expr(struct Expr *expr, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(expr->term2)
    {
        int ret_reg_num1 = _Term(expr->term1, Loc);
        int ret_reg_num2 = _Term(expr->term2, Loc);
        int for_return = ++cur_reg_num;
        // ADD for_return, ret_reg_num1, ret_reg_num2
        set_print_list(2, for_return, ret_reg_num1, ret_reg_num2, -1, -1, NULL);
        return for_return;
    }
    else
    {
        int ret_reg_num = _Term(expr->term1, Loc);
        return ret_reg_num;
    }
}
int _Term(struct Term *term, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(term->fact2)
    {
        int ret_reg_num1 = _Fact(term->fact1, Loc);
        int ret_reg_num2 = _Fact(term->fact2, Loc);
        int for_return = ++cur_reg_num;
        // MUL for_return, ret_reg_num1, ret_reg_num2
        set_print_list(3, for_return, ret_reg_num1, ret_reg_num2, -1, -1, NULL);
        return for_return;
    }
    else
    {
        int ret_reg_num = _Fact(term->fact1, Loc);
        return ret_reg_num;
    }
}
int _Fact(struct Fact *fact, char *loc)
{
    char Loc[MAX_PATH_LEN]; strcpy(Loc, loc);
    if(fact->word)
    {
        int new_reg = ++cur_reg_num;
        // LD new_reg, fact->word
        set_print_list(0, new_reg, -1, -1, -1, -1, fact->word);
        return new_reg;
    }
    else
    {
        int new_reg = ++cur_reg_num;
        // LD new_reg, fact->num
        set_print_list(0, new_reg, -1, -1, -1, fact->num, NULL);
        return new_reg;
    }
}
