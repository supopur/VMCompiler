program     ::= statement*

statement   ::= assignStmt
              | ifStmt
              | whileStmt
              | forStmt
              | funcDecl
              | callStmt
              | ";"                      (* prázdný statement, volitelné *)

assignStmt  ::= IDENTIFIER "=" expression ";"

ifStmt      ::= "if" expression "then" statement*
                ("else" statement*)?
                "end"

whileStmt   ::= "while" expression "do" statement* "end"

(* stejně jako Lua má dvě podoby - numerickou a generickou *)
forStmt     ::= "for" IDENTIFIER "=" expression ".." expression "do"
                statement* "end"
              | "for" IDENTIFIER "in" expression "do"
                statement* "end"

funcDecl    ::= "func" IDENTIFIER "(" paramList? ")" statement* "end"
paramList   ::= IDENTIFIER ("," IDENTIFIER)*

callStmt    ::= IDENTIFIER "(" argList? ")" ";"
argList     ::= expression ("," expression)*

(* hierarchie priority operátorů, od nejnižší po nejvyšší *)
expression  ::= orExpr

orExpr      ::= andExpr ("or" andExpr)*
andExpr     ::= equality ("and" equality)*
equality    ::= comparison (("==" | "!=") comparison)*
comparison  ::= term (("<" | "<=" | ">" | ">=") term)*
term        ::= factor (("+" | "-") factor)*
factor      ::= unary (("*" | "/" | "%") unary)*
unary       ::= ("not" | "-") unary
              | primary

primary     ::= NUMBER
              | STRING
              | "true"
              | "false"
              | IDENTIFIER
              | IDENTIFIER "(" argList? ")"     (* volání funkce jako výraz *)
              | "(" expression ")"
