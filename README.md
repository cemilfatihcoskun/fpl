# FPL 
Fatih's Pascal Clone

# Grammar
<digit>  ::= "0" | "1" | "2" | "3"| "4" | "5" | "6" | "7" | "8" | "9"
<number> ::= <digit> | <digit> <number>
<symbol> ::= "a" | "b" | "c" | "d" | "e" | "f"
<factor> ::= <number> | <symbol> | "(" <expr> ")"
<term>   ::= <factor> ("*" | "/") <term> | <factor>
<expr>   ::= <term> ("+" | "-") <term> | <factor>
<asgn>   ::= <symbol> "=" <expr>
<line>   ::= <symbol> | <expr>


