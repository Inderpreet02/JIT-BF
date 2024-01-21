#include <stdio.h>

#define NOB_IMPLEMENTATION
#include "nob.h"


typedef enum {
    OP_INC              = '+',
    OP_DEC              = '-',
    OP_LEFT             = '<',
    OP_RIGHT            = '>',
    OP_INPUT            = '.',
    OP_OUTPUT           = ',',
    OP_JUMP_IF_ZERO     = '[',
    OP_JUMP_IF_NONZERO  = ']',
} Op_Kind;

typedef struct {
    Op_Kind kind;
    size_t operand;
} Op;

typedef struct {
    Op *items;
    size_t count;
    size_t capacity;
} Ops;

typedef struct {
    Nob_String_View content;
    size_t pos;
} Lexer;

bool is_bf_cmd(char ch){
    const char *cmds = "+-<>.,,[]";
    return strchr(cmds, ch) != NULL;
}

char lexer_next(Lexer *l){

    while(l->pos < l->content.count && !is_bf_cmd(l->content.data[l->pos])) {
        l->pos += 1;
    }
    
    if(l->pos >= l->content.count) return 0;
    return l->content.data[l->pos++];
}

int main(int argc, char **argv) {
    const char *program = nob_shift_args(&argc, &argv);

    if(argc <= 0){
        nob_log(NOB_ERROR, "Usage: %s <input.bf>", program);
        nob_log(NOB_ERROR, "No input is provided");
        return 1;
    }

    const char *file_path = nob_shift_args(&argc, &argv);
    Nob_String_Builder sb = {0};
    if(!nob_read_entire_file(file_path, &sb)) return 1;
    
    Lexer l = {
        .content = {
            .data = sb.items,
            .count = sb.count
        }
    };
    Ops ops = {0};
    char c = lexer_next(&l);
    while(c){
        switch(c){
            case '.':
            case ',':
            case '<':
            case '>':
            case '-':
            case '+': {
                size_t count = 1;
                char s = lexer_next(&l);
                while(s == c){
                    count += 1;
                    s = lexer_next(&l);
                };
                Op op = {
                    .kind = c,
                    .operand = count,
                };
                nob_da_append(&ops, op);
                c = s;
            } break;
            case '[': {
                c = lexer_next(&l);
                // assert(0 && "IDK will have to do later");
            } break;
            case ']': {
                c = lexer_next(&l);
                // assert(0 && "IDK will have to do later");
            } break;
            default: {

            }
        }
    }

    for(size_t i=0; i < ops.count; ++i) {
        printf("%c (%zu)\n", ops.items[i].kind, ops.items[i].operand);
    }
    return 0; 
}