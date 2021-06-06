%{
#include<iostream>
#include<stdio.h>
#include<string>
#include<stdexcept>
#include "parser.hpp"
#include "ast/ast.hpp"

#undef YY_DECL
#define YY_DECL int yylex(spc::parser::semantic_type* lval, spc::parser::location_type* loc)
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

using token = spc::parser::token::yytokentype;
%}

NQUOTE [^']
%option caseless
%option noyywrap

%%
%{
    spc::parser::semantic_type* yylval = lval;
%}

"("     {return token::LP;}
")"     {return token::RP;}
"["     {return token::LB;}
"]"     {return token::RB;}
"."     {return token::DOT;}
".."    {return token::DOTDOT;}
";"     {return token::SEMI;}
","     {return token::COMMA;}
":"     {return token::COLON;}
"*"     {return token::MUL;}
"/"     {return token::TRUEDIV;}
"+"     {return token::PLUS;}
"-"     {return token::MINUS;}
">="    {return token::GE;}
">"     {return token::GT;}
"<="    {return token::LE;}
"<"     {return token::LT;}
"<>"    {return token::UNEQUAL;}
"="     {return token::EQUAL;}
":="    {return token::ASSIGN;}

"AND"       {  return token::AND;}
"ARRAY"     {  return token::ARRAY;}
"CASE"      {  return token::CASE;}
"CONST"     {  return token::CONST;}
"DIV"       {  return token::DIV;}
"MOD"       {  return token::MOD;}
"DO"        {  return token::DO;}
"DOWNTO"    {
      
    yylval->build<spc::ForDirection>(spc::ForDirection::Downto);
    return token::DOWNTO;
}
"ELSE"      {  return token::ELSE;}
"END"       {  return token::END;}
"FOR"       {  return token::FOR;}
"FUNCTION"  {  return token::FUNCTION;}
"GOTO"      {  return token::GOTO;}
"IF"        {  return token::IF;}
"NOT"       {  return token::NOT;}
"OF"        {  return token::OF;}
"OR"        {  return token::OR;}
"XOR"       {  return token::XOR;}
"BEGIN"     {  return token::PBEGIN;}
"PROCEDURE" {  return token::PROCEDURE;}
"PROGRAM"   {  return token::PROGRAM;}
"READLN"      {
      
    yylval->build<spc::SysFunc>(spc::SysFunc::Readln);
    return token::SYS_PROC;
}
"READ"      {
      
    yylval->build<spc::SysFunc>(spc::SysFunc::Read);
    return token::SYS_PROC;
}
"REPEAT"    {  return token::REPEAT;}
"THEN"      {  return token::THEN;}
"TO"        {
     
    yylval->build<spc::ForDirection>(spc::ForDirection::To); 
    return token::TO;
}
"TYPE"      {  return token::TYPE;}
"UNTIL"     {  return token::UNTIL;}
"VAR"       {  return token::VAR;}
"WHILE"     {  return token::WHILE;}
"RECORD"    {  return token::RECORD;}

"FALSE"     {
     
    yylval->build<std::shared_ptr<ConstValueNode>>(make_node<BooleanNode>(false)); 
    return token::SYS_CON;
}
"MAXINT"    {
     
    yylval->build<std::shared_ptr<ConstValueNode>>(make_node<IntegerNode>(std::numeric_limits<int>::max()));
    return token::SYS_CON;
}
"TRUE"      {
     
    yylval->build<std::shared_ptr<ConstValueNode>>(make_node<BooleanNode>(true)); 
    return token::SYS_CON;
}
"ABS"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Abs);
    return token::SYS_FUNCT;
}
"CHR"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Chr);
    return token::SYS_FUNCT;
}
"CONCAT"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Concat);
    return token::SYS_FUNCT;
}
"LENGTH"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Length);
    return token::SYS_FUNCT;
}
"ODD"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Odd);
    return token::SYS_FUNCT;
}
"ORD"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Ord);
    return token::SYS_FUNCT;
}
"PRED"      {
         
    yylval->build<spc::SysFunc>(spc::SysFunc::Pred);
    return token::SYS_FUNCT;
}
"SQR"       {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Sqr);
    return token::SYS_FUNCT;
}
"SQRT"      {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Sqrt);
    return token::SYS_FUNCT;
}
"STR"      {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Str);
    return token::SYS_FUNCT;
}
"SUCC"      {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Succ);
    return token::SYS_FUNCT;
}
"VAL"     {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Val);
    return token::SYS_FUNCT;
}
"WRITE"     {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Write);
    return token::SYS_PROC;
}
"WRITELN"   {
     
    yylval->build<spc::SysFunc>(spc::SysFunc::Writeln);
    return token::SYS_PROC;
}

"BOOLEAN"   {
     
    yylval->build<std::shared_ptr<SimpleTypeNode>>(make_node<SimpleTypeNode>(spc::Type::Bool));
    return token::SYS_TYPE;
}
"CHAR"      {
     
    yylval->build<std::shared_ptr<SimpleTypeNode>>(make_node<SimpleTypeNode>(spc::Type::Char));  
    return token::SYS_TYPE;
}
"INTEGER"   {
     
    yylval->build<std::shared_ptr<SimpleTypeNode>>(make_node<SimpleTypeNode>(spc::Type::Int)); 
    return token::SYS_TYPE;
}
"LONGINT"   {
     
    yylval->build<std::shared_ptr<SimpleTypeNode>>(make_node<SimpleTypeNode>(spc::Type::Long)); 
    return token::SYS_TYPE;
}
"REAL"      {
     
    yylval->build<std::shared_ptr<SimpleTypeNode>>(make_node<SimpleTypeNode>(spc::Type::Real)); 
    return token::SYS_TYPE;
}
"STRING"    {
     
    return token::STR_TYPE;
}

[+-]?[0-9]+      {
   
    yylval->build<std::shared_ptr<IntegerNode>>(make_node<IntegerNode>(atoi(yytext))); 
    return token::INTEGER;
}
[+-]?[0-9]+"."[0-9]+("e"[+-]?[0-9]+)?   {

    yylval->build<std::shared_ptr<RealNode>>(make_node<RealNode>(atof(yytext))); 
    return token::REAL;
}
'{NQUOTE}'  {

    yylval->build<std::shared_ptr<CharNode>>(make_node<CharNode>(yytext[1])); 
    return token::CHAR;
}
'({NQUOTE}|'')+'  {
 
    yytext[yyleng-1] = 0; 
    yylval->build<std::shared_ptr<StringNode>>(make_node<StringNode>(yytext + 1)); 
    return token::STRING;
}
[a-zA-Z_]([a-zA-Z0-9_])*  {

    yytext[yyleng] = 0;
    yylval->build<std::shared_ptr<IdentifierNode>>(make_node<IdentifierNode>(yytext));  
    return token::ID;
}
[ \t\f]    { continue;}
[\n\r]     {loc->lines();}

"(*" {
    char c;
    while(c = yyinput()) 
    {
        if (c == '\n') loc->lines();
        else if(c == '*') 
        {
            if((c = yyinput()) == ')')
                break;
            else unput(c);
        }
    }
}
"{" {
    char c;
    while(c = yyinput()) 
    {
        if (c == '\n') loc->lines();
        else if(c == '}') break;
    }
}
"//" {
    char c;
    while(c = yyinput()) 
    {
        if(c == '\n') 
        {
            loc->lines();
            break;
        }
        else if(c == EOF) {
            break;
        }
    }
}

. {
    std::cerr << std::endl << "Scanner: Error at " << *loc << ":" << std::endl;
    throw std::invalid_argument(std::string("Invalid token \'") + yytext + "\'");
}

%%

