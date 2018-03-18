%{
    #include <stdio.h>
    #include <string.h>
    #include <ctype.h>
    #include <fstream>
    #include <iostream>
    #include <string.h>
    #include <vector>
    #include <algorithm>
    #include "y.tab.h"
    using namespace std;
    void yyerror(char *);
    int parse(void);
    char* trim(char* input);
    extern "C" int yylex();
    extern "C" FILE *yyin;
    int error=0;
    struct latency{
        char *command;
        int clock_cycle;
    };
    vector <latency> latency_obj;
%}
%token COMMAND EQUALS SEMICOLON NUM

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
                temp.clock_cycle = $3;
                latency_obj.push_back(temp);
            }
            ;
%%

char* trim(char* input){
    char* output = input;
    int i = 0, j = 0;
        
    while (input[i] != NULL){
        if (input[i] != ' '){
            output[j] = input[i];
            j++;
            i++;
        }
        else{
            i++;
        }
    }
    output[j]=NULL;
    return output;	
}

void yyerror(char *s){
    error=-1;
    extern int yylineno;
    fprintf(stderr, "Line Number%d:-%s\n",yylineno, s);
}
void printvector(){
    for(int i=0;i<latency_obj.size();i++){
        cout<<latency_obj[i].command<<"--> "<<latency_obj[i].clock_cycle<<endl;
    }
}
int parse(void){
   FILE *pt = fopen("latency.txt", "r" );
    if(!pt){
        cout << "No such file exists" << endl;
        return -1;
    }
    yyin = pt;
    do{
        yyparse();
    }while (!feof(yyin));
    // printvector();
    return error;
}
