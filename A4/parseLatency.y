%{
    #include <stdio.h>
    #include "y.tab.h"
    #include "latency.h"
    using namespace std;
    void yyerror(char *);
    char* trim(char* input);
    void convertToLower(char* input);
    extern "C" int yylex();
    extern "C" FILE *yyin;
    vector <latency> latency_obj;
    int error = 0;
%}
%token EQUALS SEMICOLON
%union{
    int num;
    char * str;
}
%token <num> NUM
%token <str> COMMAND
%%
program:    program statement '\n'
            | program statement
            | program '\n'
            | program ' '
            |
            ;
statement:  commands
            ;
commands:   COMMAND EQUALS NUM SEMICOLON
            {
                latency temp;
                temp.command = trim($1);
                convertToLower(temp.command);
                temp.clock_cycle = $3;
                latency_obj.push_back(temp);
            }
            ;
%%

char* trim(char* input){
    char* output = input;
    int i = 0, j = 0;

    while (input[i] != '\0'){
        if(input[i] != ' '){
            output[j] = input[i];
            j++;
            i++;
        }
        else{
            i++;
        }
    }
    output[j] = '\0';
    return output;
}

void convertToLower(char* s){
    int index = 0;
    while(s[index] != '\0'){
        if(s[index] >= 'A' && s[index] <= 'Z'){
            s[index] = s[index] + 32;
        }
        index ++;
    }
}

void yyerror(char *s){
    error = -1;
    extern int yylineno;
    fprintf(stderr, "Line Number %d: %s\n",yylineno, s);
}
/* JUST for DEBUGGING PURPOSE
void printvector(){
    for(int i = 0; i < latency_obj.size(); i++){
        cout << latency_obj[i].command << "--> " << latency_obj[i].clock_cycle << endl;
    }
}
*/
int parse(void){
   FILE *pt = fopen("latency.txt", "r");
    if(!pt){
        fprintf(stderr, "No file named latency.txt exists!\n");
        return -1;
    }
    yyin = pt;
    yyparse();
    // printvector();
    return error;
}
