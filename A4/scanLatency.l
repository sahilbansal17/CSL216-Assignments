%{
    #include <stdio.h>
    #define YY_DECL extern "C" int yylex()
    #include "y.tab.h"
    void yyerror(char *);
%}
%option yylineno
%option caseless
SPACE           " "
EQUAL           "="
%%
{SPACE}*(add|sub|mul|mov|ldr|str|cmp|cmn|bge|b|beq|bne|bl|blt|ldr_pseudo)        { yylval.str = strdup(yytext); return COMMAND; }

{SPACE}*{EQUAL}                                     { return EQUALS; }

{SPACE}*([0-9]+)                                    { yylval.num = atoi(yytext); return NUM; }

{SPACE}*[;]{SPACE}*                                 { return SEMICOLON; }

[ \n]                                               { return *yytext; }

.                                                   yyerror("invalid character");

%%

int yywrap(void) {
    return 1;
}
