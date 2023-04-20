#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*** Stack and HashMap**/
#define MAP_CAPACITY 128
#define stack_cap 256
int globalVar = 0;
typedef struct {
    //Stack element
    char *name;
} Stack_Variable;
typedef struct {
    //Stack struct
    Stack_Variable *content;
    int size;
} Stack;
Stack *Stack_construct() {
    //creates a stack
    Stack *stack = malloc(sizeof(Stack));
    stack->content = calloc(stack_cap, sizeof(Stack_Variable));
    stack->size=0;
    return stack;
}

void push(Stack* stack,char* key){
    //pushes an element at the top of the stack
    stack->content[stack->size++].name = key;
}
char* pop(Stack* stack){
    //removes the element at the top of the stack and returns it
    return stack->content[--(stack->size)].name;
}
char* peek(Stack* stack){
    //returns a copy of the element at the top of the stack
    //the element is not removed
    char* ans = malloc(strlen(stack->content[(stack->size)-1].name)+1);
    ans = strcpy(ans,stack->content[(stack->size)-1].name);
    return ans;
}

typedef struct {
    //key value pair in Hashmap
    long long value;
    char* name;
} Hash_Variable;

typedef struct {
    //Hashmap struct
    Hash_Variable *entries;
    int capacity;
} Map;

typedef struct Hashmap Hashmap;

long long hash(char *key) {
    //calculates hash value of the key
    long long hash = 0;
    for(int i=0;i<strlen(key)-1;i++){
        hash = (hash << 2) + hash + key[i];
    }
    return hash;
}

Map *HashMap_construct() {
    //creates a hashmap
    Map *map = malloc(sizeof(Map));
    map->capacity = MAP_CAPACITY;
    map->entries = calloc(MAP_CAPACITY, sizeof(Hash_Variable));
    return map;
}
void put(Map *map, char *key, long long value) {
    //maps the value key pair on the hashmap
    long long index = hash(key) % map->capacity;
    while (map->entries[index].name != NULL) {
        if (strcmp(map->entries[index].name, key) == 0) {
            map->entries[index].value = value;
            return;
        }
        index = (index + 1) % map->capacity;
    }
    map->entries[index].value = value;
    map->entries[index].name = key;
}
long long get(Map *map, char *key) {
    //gets the value of given key which is mapped on hashmap
    long long index = hash(key) % map->capacity;
    while (map->entries[index].name != NULL) {
        if (strcmp(map->entries[index].name, key) == 0) {
            return map->entries[index].value;
        }
        index = (index + 1) % map->capacity;
    }
    put(map,key,0);
    return 0;
}
/*****/
/**** Helper Functions ************************/
void print(int num, char* items[],FILE *file);
int isInt(char* key){
    //checks whether the given string is only consists of digits
    int var = 0;
    for(int i=0;i< strlen(key);i++){
        if(isdigit(key[i])==0){
            if(i==0 && '-' == key[i]&& strlen(key)>1){
                continue;
            }else{
                var = 1;
            }
        }
    }
    return var;
}
int isVar(char* key){
    //checks whether the given string is only consists of letters
    int var = 0;
    for(int i=0;i< strlen(key);i++){
        if(isalpha(key[i])==0){
            var = 1;
        }
    }
    return var;
}
int is_Func(char* item){
    //checks whether the given string is keyword(function)
    if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 || strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0){
        return 0;
    }
    return 1;
}
int precedence(char* key){
    //returns order of precedence of given operator/function
    //higher the number means it has more priority
    if (strcmp(key, "not") == 0){
        return 6;
    }else if (strcmp(key, "*") == 0||strcmp(key, "/") == 0||strcmp(key, "%") == 0){
        return 5;
    }else if (strcmp(key, "+") == 0|| strcmp(key,"-")==0){
        return 4;
    }else if(strcmp(key, "ls") == 0|| strcmp(key,"rs")==0){
        return 3;
    }else if(strcmp(key, "&") == 0){
        return 2;
    }else if(strcmp(key, "xor") == 0){
        return 1;
    }else{
        return 0;
    }
}
int is_left_Parenthesis(char* key){
    return strcmp(key,"(");
}
int is_right_Parenthesis(char* key){
    return strcmp(key,")");
}
int push_to_stack(Stack* stack_operator,Stack* output,char* key){
    // 0 means no error 1 means error
    /* this function converts in-fix notation to postfix notation
     * If there is a function it is pushed to function stack in another part of program and
     * it is given to this function if comma is encountered and function is pushed to operator stack according to precedence
     */
    if(is_Func(key)==0){
        while(stack_operator->size>0){
            if(precedence(peek(stack_operator))>= precedence(key)){
                if(strcmp(peek(stack_operator),"(")!=0){
                    push(output,pop(stack_operator));
                }else{
                    push(stack_operator,key);
                    break;
                }
            }else{
                push(stack_operator,key);
                break;
            }
        }
        return 0;
    }else if(is_left_Parenthesis(key)==0){
        //the left parenthesis is directly pushed to the operator stack
        push(stack_operator,key);
        return 0;
    }else if(is_right_Parenthesis(key)==0){
        //if a right parenthesis is encountered elements are popped from operator stack
        //until the popped element is left parenthesis
        //then both left and the right parentheses are eliminated
        //If there is no left parenthesis error is raised
        int error = 1;
        while(stack_operator->size>0){
            char* temp= pop(stack_operator);
            if(is_left_Parenthesis(temp)!=0){
                push(output,temp);
            }else{
                error=0;
                break;
            }
        }
        return error;
    }else if(isVar(key)==0|| isInt(key)==0){
        // variables and integers are directly pushed to final stack
        push(output,key);
        return 0;
    } else{
        //operators are pushed to operator stack according to precedence
        while(stack_operator->size>0){
            if(strcmp(peek(stack_operator),"(")==0){
                break;
            }
            if(precedence(peek(stack_operator))>= precedence(key)){
                push(output,pop(stack_operator));
            }else{
                break;
            }
        }
        push(stack_operator,key);
        return 0;
    }
}

long long postfix(Map *map, Stack *stack, int *hks_error, char** p, FILE *file) {
    /*this function evaluates given stack of elements it stars by
     * popping one element and pops one or two element if the popped element
     * is function or operator
     * else if the element is operand then value of the element is returned
     */
    char* temp = pop(stack);
    long long ans;
    if(stack->size<0){
        *hks_error = 1;
        return 0;
    }else{
        if(isInt(temp)==0){
        ans = atoll(temp);
        int varSize = snprintf(NULL, 0, "%lld", ans);
        varSize += 1; // Add 1 for null-terminator
        *p = (char*) malloc(varSize * sizeof(char));
        snprintf(*p, sizeof(*p), "%lld", ans);
        return ans;
        }else{
            int varSize = snprintf(NULL, 0, "%d", globalVar);
            varSize += 1; // Add 1 for null-terminator
            *p = (char*) malloc(varSize * sizeof(char));
            snprintf(*p, sizeof(p), "%d", globalVar++);
            char* p1=NULL;
            char* p2=NULL;
            if(isVar(temp)==0){
                ans = get(map,temp);
                char* items[2] = {*p,temp};
                if(hks_error==0)print(2,items,file);
                return ans;
            }else if(strcmp("not",temp)==0){
                ans =  ~(postfix(map, stack, hks_error, &p1,file));
                char* items[4] = {*p,"xor",p1,"1"};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("*",temp)==0){
                ans= postfix(map, stack, hks_error, &p1,file) * postfix(map, stack, hks_error, &p2,file);
                char* items[4] = {*p,"mul",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("%",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans = b%a;
                char* items[4] = {*p,"srem",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("/",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans = b/a;
                char* items[4] = {*p, "sdiv", p1, p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("-",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans = b-a;
                char* items[4] = {*p, "sub", p1, p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("+",temp)==0){
                ans= postfix(map, stack, hks_error, &p1,file) + postfix(map, stack, hks_error, &p2,file);
                char* items[4] = {*p,"add",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("ls",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans= b << a;
                char* items[4] = {*p,"shl",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("rs",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans= b >> a;
                char* items[4] = {*p,"lshr",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("lr",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans=((b << a)|(b >> (64 - a)));

                char* items2[4] = {*p,"sub","64\0",p2};
                if(hks_error==0)print(3,items2,file);

                int varSize2 = snprintf(NULL, 0, "%d", globalVar);
                varSize2 += 1; // Add 1 for null-terminator
                char *var2 = (char*) malloc(varSize2 * sizeof(char));
                snprintf(var2, sizeof(var2), "%d", globalVar++);
                char* items3[4] = {var2,"shl",p1,p2};
                if(hks_error==0)print(3,items3,file);

                int varSize3 = snprintf(NULL, 0, "%d", globalVar);
                varSize3 += 1; // Add 1 for null-terminator
                char *var3 = (char*) malloc(varSize3 * sizeof(char));
                snprintf(var3, sizeof(var3), "%d", globalVar++);
                char* items[4] = {var3,"or",var2,var3};
                if(hks_error==0)print(3,items,file);

                return ans;
            }else if(strcmp("rr",temp)==0){
                long long a = postfix(map, stack, hks_error, &p2,file);
                long long b = postfix(map, stack, hks_error, &p1,file);
                ans=((b >> a)|(b << (64 - a)));

                char* items2[4] = {*p,"sub","64\0",p2};
                if(hks_error==0)print(3,items2,file);

                int varSize2 = snprintf(NULL, 0, "%d", globalVar);
                varSize2 += 1; // Add 1 for null-terminator
                char *var2 = (char*) malloc(varSize2 * sizeof(char));
                snprintf(var2, sizeof(var2), "%d", globalVar++);
                char* items3[4] = {var2,"lshr",p1,p2};
                if(hks_error==0)print(3,items3,file);

                int varSize3 = snprintf(NULL, 0, "%d", globalVar);
                varSize3 += 1; // Add 1 for null-terminator
                char *var3 = (char*) malloc(varSize3 * sizeof(char));
                snprintf(var3, sizeof(var3), "%d", globalVar++);
                char* items[4] = {var3,"or",var2,var3};
                if(hks_error==0)print(3,items,file);

                return ans;
            }else if(strcmp("&",temp)==0){
                ans=(postfix(map, stack, hks_error, &p1,file) & postfix(map, stack, hks_error, &p2,file));
                char* items[4] = {*p,"and",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("xor",temp)==0){
                ans=(postfix(map, stack, hks_error, &p1,file) ^ postfix(map, stack, hks_error, &p2,file));
                char* items[4] = {*p,"xor",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else if(strcmp("|",temp)==0){
                ans=(postfix(map, stack, hks_error, &p1,file) | postfix(map, stack, hks_error, &p2,file));
                char* items[4] = {*p,"or",p1,p2};
                if(hks_error==0)print(3,items,file);
                return ans;
            }else{
                return 0;
            }
        }
    }
}
/*****************************************/
char* parseAfterLeftStrip (char *side);

int main(int argc, char* argv[]) {
    bool globalerror;
    bool error;
    bool equals;
    Map* HashMap = HashMap_construct();//this hashmap contains variables and their corresponding value
    char *fileName = strdup(argv[0]);
    char* out = malloc(sizeof(char )* (strlen(argv[0])+1));
    snprintf(out,strlen(argv[0])+1,"%s.ll",strtok(fileName,"."));
    FILE *input = fopen(argv[0],"r");
    FILE *output = fopen(out,"w");
    fprintf(output, "; ModuleID = 'advcalc2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n\ndefine i32 @main() {");
    enum State {ALLOCA = 0, STORE = 1, LOAD = 2, OPERATION = 3, PRINT = 4};
    char line[256];
    for (int lineNo=0; fgets(line, sizeof(line), input) != NULL; lineNo++) {
        Stack* Operator = Stack_construct();//holds operators in the process of reordering of the elements to obtain postfix notation
        Stack* Output = Stack_construct();//final stack that contains postfix notation of operation
        Stack* Funcs = Stack_construct();//holds functions in the process of reordering of the elements to obtain postfix notation
        char *left;
        char *right;
        char *third;
        error = false;
        equals = false;
        char *p = line;
        int par = 0;
        bool ch = false;
        bool sp = false;
        bool nu = false;
        // in this loop we traverse through the input
        while (*p != '\n') {
            if (*p == '=') {
            // when we see equals sign we approve that the input is an assignment input, this boolean will be used later
            // other booleans will be explained later
                equals = true;
                ch = false;
                sp = false;
                nu = false;
                char *p2 = p;
                p2++;
                if (*p2 == '=') {
                    // if there are two equal signs side by side we raise error
                    error = true;
                    break;
                }
            } else if (*p == '(') {
                // we keep the count of open parentheses here
                ch = false;
                sp = false;
                nu = false;
                par++;
            } else if (*p == ')') {
                // we increase the number of open parentheses
                ch = false;
                sp = false;
                nu = false;
                par--;
            } else if (isalpha(*p)) {
                if ((ch && sp) || (ch && nu)) {
                    error = true;
                    break;
                }
                ch = true;
            } else if (isdigit(*p)) {
                if ((nu && sp) || (nu && ch)) {
                    error = true;
                    break;
                }
                nu = true;
            } else if (*p == ' ') {
                if (ch || nu) {
                    sp = true;
                }
            }
            else if (isspace(*p)) {
                if (ch || nu) {
                    sp = true;
                }
                *p = ' ';
            }
            else if (*p == '+' || *p == '*' || *p == '-' || *p == '&' || *p == '|' || *p == ',' || *p == '%' || *p == '/') {
                ch = false;
                sp = false;
                nu = false;
            } else { // illegal characters will be detected here
                error = true;
                break;
            }
            p++;
        }
        if (par != 0 || error) { //if the number of parentheses don't match or any other gets detected, we raise error
            printf("Error on line %d!\n", lineNo);
            globalerror = true;
            continue;
        }

        left = strtok(line, "=");
        right = strtok(NULL, "=");
        third = strtok(NULL, "=");
        if (third != NULL) { // if the input can be splitted more than once there is a error
            printf("Error on line %d!\n", lineNo);
            globalerror = true;
            continue;
        }
        char *variable;
        char *variable2;

        if (right != NULL && strcmp(right, "\n") != 0 && strcmp(right, " ") != 0) {
            // this if block works when we have to make an assignment
            int i = 0;
            int length = strlen(left);
            while (left[i] == ' ') {
                // we check whether the LHS is empty or not
                i++;
                if (i == length - 1) {
                    error = true;
                    break;
                }
            }
            if (error) {
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            variable = strtok(left, " ");
            variable2 = strtok(NULL, " ");
            if (variable == NULL) {
                // checks whether the LHS is empty or not
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
            }
            if (variable2 != NULL) {
                // checks whether there is a space between chars in LHS
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            char *po = variable;
            while (*po != '\0') {
                if (!isalpha(*po)) {
                    // raise error if there a non-alphabetic character in variable name
                    error = true;
                    break;
                }
                po++;
            }
            if (strcmp(variable, "xor") == 0 || strcmp(variable, "ls") == 0 || strcmp(variable, "rs") == 0 ||
                strcmp(variable, "lr") == 0 || strcmp(variable, "rr") == 0 || strcmp(variable, "not") == 0) {
                // raise error when the variable name is a reserved word
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            int k = 0;
            int length2 = strlen(right);
            // this part checks iterates through the RHS and raises error if the token is all empty
            char *p8 = right;
            char *r = malloc(length2*sizeof(char));
            while (*p8 != '\n') {
                r[k] = *p8;
                k++;
                p8++;
            }
            r[k] = '\0';
            if (strtok(r, " ") == NULL) {
                error = true;
            }
            if (error) {
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            char *nsRight = parseAfterLeftStrip(right);
            char *p3 = nsRight;
            int type = 0; // 0=start / 1=string / 2=number / 3=operator / 4=function / 5 = open parenthesis / 6 = closed parenthesis
            int length3 = strlen(nsRight);
            while (*p3 != '\n' && *p3 != '\0') {
                char *item = malloc(length3 + 1 * sizeof(char));
                item[0] = '\0';
                int a = 0;
                while (isalpha(*p3)) {
                    if (type == 1 || type == 2 || type == 6) {
                        // raise error if the letter is after a forbidden token
                        a=0;
                        error = true;
                        break;
                    }
                    // go until the end of token and store it in the variable token
                    item[a++] = *p3;
                    p3++;
                }
                if (a > 0) {
                    item[a] = '\0';
                    if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 || strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0) {
                        // if the token is a reserved word
                        type = 4;
                        if (*p3 != '(') {
                            // if there is no open parenthesis after the function name raise error
                            error = true;
                            break;
                        } else {
                            char *p4 = p3;
                            p4++;
                            int parenthesis = 1;
                            int function; // represents the encountered functions which take two arguments
                            if (strcmp(item, "not") == 0) {
                                function = 0;
                            } else {
                                function = 1;
                            }
                            int comma = 0;
                            while (parenthesis > 0) {
                                // traverse until the parentheses sequence ends
                                if (*p4 == '(') {
                                    parenthesis++;
                                    p4--;
                                    if (*p4 == 'r' || *p4 == 's') {
                                        function++;
                                    }
                                    p4++;
                                } else if (*p4 == ',') {
                                    comma++;
                                }
                                if (comma > function) {
                                    // if there are more commas the encountered functions raise error
                                    error = true;
                                    break;
                                }
                                if (*p4 == ')') {
                                    parenthesis--;
                                }
                                p4++;
                            }
                            if (comma != function) {
                                // if the number of commas is not equal to number of functions raise error
                                error = true;
                            }
                        }
                    } else {
                        // the token is a variable
                        type = 1;
                        if (*p3 == '(') {
                            error = true;
                            break;
                        }
                    }
                    if (error) {
                        break;
                    } else {
                        if (type == 4) {
                            // push the item in funcs
                            push(Funcs,item);
                        } else if (type == 1) {
                            //this code gets variables value from the Hashmap and puhses the value to the stack for further operations
                            push_to_stack(Operator,Output,item);
                        }
                    }
                    continue;
                }
                while (isdigit(*p3)) {
                    // take the variable token, check whether it is in the true place not, if there is no issue send it
                    if (type == 1 || type == 2 || type == 6) {
                        a=0;
                        error = true;
                        break;
                    }
                    item[a++] = *p3;
                    p3++;
                }
                if (a > 0) {
                    type = 2;
                    item[a] = '\0';
                    push_to_stack(Operator,Output,item);
                    continue;
                }
                while (*p3 == '(') {
                    // same thing as digit
                    if (type == 1 || type == 2 || type == 6) {
                        a=0;
                        error = true;
                        break;
                    }
                    item[a++] = *p3;
                    p3++;
                    break;
                }
                if (a>0) {
                    type = 5;
                    item[a] = '\0';
                    if(Funcs->size>0){
                        if(strcmp(peek(Funcs),"not")==0){//this handles not operation
                            push(Operator, pop(Funcs));
                        }
                    }
                    push_to_stack(Operator,Output,item);
                    continue;
                }
                while (*p3 == ')') {
                    // same thing as digit
                    if (type == 0 || type == 3 || type == 5) {
                        a=0;
                        error = true;
                        break;
                    }
                    item[a++] = *p3;
                    p3++;
                    break;
                }
                if (a>0) {
                    type = 6;
                    item[a] = '\0';
                    push_to_stack(Operator,Output,item);
                    continue;
                }
                while (*p3 == '+' || *p3 == '*' || *p3 == '-' || *p3 == '&' || *p3 == '|' || *p3 == ',' || *p3 == '/' || *p3 == '%') {
                    // same thing as digit but this time we only take one variable because otherwise we would miss some errors
                    if (type == 0 || type == 3 || type == 5) {
                        a=0;
                        error = true;
                        break;
                    }
                    item[a++] = *p3;
                    p3++;
                }
                if (a==1) {
                    type = 3;
                    item[a] = '\0';
                    if(strcmp(item,",")!=0) {
                        push_to_stack(Operator, Output, item);//pushes the functions to operator stack
                    }else{
                        push_to_stack(Operator, Output, pop(Funcs));
                    }
                    continue;
                } else if (a!=1) {
                    error =true;
                }
                if (error) {
                    break;
                }
            }
            if (error) {
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            /*************************************************************/
            int parentheses_error = 0;
            while (Operator->size>0){//pushes everything to the output stack
                if(is_left_Parenthesis(peek(Operator))==0){
                    parentheses_error = 1;//raises error if there is left parenthesis
                    break;
                }else{
                    push(Output, pop(Operator));
                }
            }
            if(parentheses_error==1){
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            /*******/
            int operation_error = 0;
            if(Output->size==0){
                continue; // continues if the input is empty
            }
            char *var = NULL;
            long long ans = postfix(HashMap, Output, &operation_error, &var,output);//calculates the final value and returns error value
            if(operation_error==1){
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            char* items0 = malloc(sizeof  variable);
            strcpy(items0,variable);
            put(HashMap,items0,ans);
            int items1size = snprintf(NULL, 0, "%lld", ans);
            items1size += 1; // Add 1 for null-terminator
            char* items1 = (char*) malloc(items1size* sizeof(char));
            snprintf(items1, sizeof(items1),"%lld",ans);
            char* items[2] = {items0,items1};
            print(0,items,output);
            print(1,items,output);


            /**************************************************************/
        }
        else {
            // this part is the printing case, we do totally the same thing as RHS in assignment case
            // the differences are commented
            if (equals) {
                // raise error if there is an equal sign in the input
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            int i = 0;
            int length = strlen(left);
            while (left[i] == ' ') {
                i++;
                if (i == length - 1) {
                    error = true;
                    break;
                }
            }
            if (error) {
                // when the input is all empty don't raise error but continue
                continue;
            }
            char *nsLeft = parseAfterLeftStrip(left);
            char *p5 = nsLeft;
            int type = 0; // 0=start / 1=string / 2=number / 3=operator / 4=function / 5 = open parenthesis / 6 = closed parenthesis
            int length3 = strlen(nsLeft);
            while (*p5 != '\0' && *p5 != '\n') {
                char *item = malloc(length3 + 1 * sizeof(char));
                item[0] = '\0';
                int a = 0;
                while (isalpha(*p5)) {
                    if (type == 1 || type == 2 || type == 6) {
                        // raise error if the letter is after a forbidden token
                        a = 0;
                        error = true;
                        break;
                    }
                    // go until the end of token and store it in the variable token
                    item[a++] = *p5;
                    p5++;
                }
                if (a > 0) {
                    item[a] = '\0';
                    if (strcmp(item, "xor") == 0 || strcmp(item, "not") == 0 || strcmp(item, "lr") == 0 ||
                        strcmp(item, "rr") == 0 || strcmp(item, "rs") == 0 || strcmp(item, "ls") == 0) {
                        // if the token is a reserved word
                        type = 4;
                        if (*p5 != '(') {
                            // if there is no open parenthesis after the function name raise error
                            error = true;
                            break;
                        } else {
                            char *p6 = p5;
                            p6++;
                            int parenthesis = 1;
                            int function; // represents the encountered functions which take two arguments
                            if (strcmp(item, "not") == 0) {
                                function = 0;
                            } else {
                                function = 1;
                            }
                            int comma = 0;
                            while (parenthesis > 0) {
                                // traverse until the parentheses sequence ends
                                if (*p6 == '(') {
                                    parenthesis++;
                                    p6--;
                                    if (*p6 == 'r' || *p6 == 's') {
                                        function++;
                                    }
                                    p6++;
                                } else if (*p6 == ',') {
                                    comma++;
                                }
                                if (comma > function) {
                                    // if there are more commas the encountered functions raise error
                                    error = true;
                                    break;
                                }
                                if (*p6 == ')') {
                                    parenthesis--;
                                }
                                p6++;
                            }
                            if (comma != function) {
                                // if the number of commas is not equal to number of functions raise error
                                error = true;
                            }
                        }
                    } else {
                        // the token is a variable
                        type = 1;
                        if (*p5 == '(') {
                            error = true;
                            break;
                        }
                    }
                    if (error) {
                        break;
                    }  else {
                        if (type == 4) {
                            // push the item in funcs
                            push(Funcs,item);
                        } else if (type == 1) {
                            //this code gets variables value from the Hashmap and puhses the value to the stack for further operations
                            push_to_stack(Operator,Output,item);
                        }
                    }
                    continue;
                }
                while (isdigit(*p5)) {
                    // take the variable token, check whether it is in the true place not, if there is no issue send it
                    if (type == 1 || type == 2 || type == 6) {
                        a = 0;
                        error = true;
                        break;
                    }
                    item[a++] = *p5;
                    p5++;
                }
                if (a > 0) {
                    type = 2;
                    item[a] = '\0';
                    push_to_stack(Operator,Output,item);
                    continue;
                }
                while (*p5 == '(') {
                    // same thing as digit
                    if (type == 1 || type == 2 || type == 6) {
                        a = 0;
                        error = true;
                        break;
                    }
                    item[a++] = *p5;
                    p5++;
                    break;
                }
                if (a > 0) {
                    type = 5;
                    item[a] = '\0';
                    if(Funcs->size>0){ //this handles not operation
                        if(strcmp(peek(Funcs),"not")==0){
                            push(Operator, pop(Funcs));
                        }
                    }
                    push_to_stack(Operator,Output,item);
                    continue;
                }
                while (*p5 == ')') {
                    // same thing as digit
                    if (type == 0 || type == 3 || type == 5) {
                        a = 0;
                        error = true;
                        break;
                    }
                    item[a++] = *p5;
                    p5++;
                    break;
                }
                if (a > 0) {
                    type = 6;
                    item[a] = '\0';
                    push_to_stack(Operator,Output,item);
                    continue;
                }
                while (*p5 == '+' || *p5 == '*' || *p5 == '-' || *p5 == '&' || *p5 == '|' || *p5 == ',' || *p5 == '%' || *p5 == '/') {
                    // same thing as digit but this time we only take one variable because otherwise we would miss some errors
                    if (type == 0 || type == 3 || type == 5) {
                        a = 0;
                        error = true;
                        break;
                    }
                    item[a++] = *p5;
                    p5++;
                }
                if (a == 1) {
                    type = 3;
                    item[a] = '\0';
                    if(strcmp(item,",")!=0) {
                        push_to_stack(Operator, Output, item);//pushes the functions to operator stack
                    }else{
                        push_to_stack(Operator, Output, pop(Funcs));
                    }
                    continue;
                } else if (a != 1) {
                    error = true;
                }
                if (error) {
                    break;
                }
            }
            if (error) {
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }

            /*************************************************************/
            int parentheses_error = 0;
            while (Operator->size>0){//pushes everything to the output stack
                if(is_left_Parenthesis(peek(Operator))==0){
                    parentheses_error = 1;//raises error if there is left parenthesis
                    break;
                }else{
                    push(Output, pop(Operator));
                }
            }
            if(parentheses_error==1){
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }
            /*******/
            int operation_error = 0;
            if(Output->size==0){
                continue; // continues if the input is empty
            }
            char *var = NULL;
            long long ans = postfix(HashMap, Output, &operation_error, &var,output);//calculates the final value and returns error value
            if(operation_error==1){
                printf("Error on line %d!\n", lineNo);
                globalerror = true;
                continue;
            }


            printf("%lld\n",ans); //İNCELENECEK
            char items[1] = {ans};
            print(4, items, output);
            /**************************************************************/

        }
    }
    fprintf(output, "ret i32 0\n}");
    if (globalerror) {
        remove(out);
    }
    fclose(input);
    fclose(output);
    return 0;
}
char* parseAfterLeftStrip (char *side) {
    /* this function removes the spaces from a given string, and returns the new string */
    char *equation = malloc(strlen(side));
    equation[0] = '\0';
    char *p = strtok(side, " ");
    while (p != NULL) {
        strcat(equation, p);
        p = strtok(NULL, " ");
    }
    return equation;
}
void print(int num, char* items[], FILE *file) {
    switch (num) {
        case 0: //alloca
            fprintf(file, "%%%s = alloca i32\n", items[0]);
            break;
        case 1: //store
            fprintf(file, "store i32 %s, i32* %%%s\n", items[1], items[0]);
            break;
        case 2: //load
            fprintf(file, "%%%s = load i32, i32* %%%s""\n", items[0], items[1]);
            break;
        case 3: //operation
            fprintf(file, "%%%s = %s i32 %%%s, %%%s\n", items[0], items[1], items[2], items[3]);
            break;
        case 4:
            fprintf(file, "call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %%%s} )\n",items[0]);
            break;
    }
}
