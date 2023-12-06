# Simple Pascal-like Interpreter
This is an interpreter written in C++ for NJIT's CS280: Programming Language Concepts class. 

It follows the following EBNF structure:
1. Prog ::= PROGRAM IDENT ; DeclPart CompoundStmt . 
2. DeclPart ::= VAR DeclStmt; { DeclStmt ; }
3. DeclStmt ::= IDENT {, IDENT } : Type [:= Expr]
4. Type ::= INTEGER | REAL | BOOLEAN | STRING
5. Stmt ::= SimpleStmt | StructuredStmt 
6. SimpleStmt ::= AssignStmt | WriteLnStmt | WriteStmt
7. StructuredStmt ::= IfStmt | CompoundStmt
8. CompoundStmt ::= BEGIN Stmt {; Stmt } END
9. WriteLnStmt ::= WRITELN (ExprList)
10. WriteStmt ::= WRITE (ExprList)
11. IfStmt ::= IF Expr THEN Stmt [ ELSE Stmt ] 
12. AssignStmt ::= Var := Expr
13. Var ::= IDENT 
14. ExprList ::= Expr { , Expr }
15. Expr ::= LogOrExpr ::= LogAndExpr { OR LogAndExpr }
16. LogAndExpr ::= RelExpr {AND RelExpr }
17. RelExpr ::= SimpleExpr [ ( = | < | > ) SimpleExpr ]
18. SimpleExpr :: Term { ( + | - ) Term }
19. Term ::= SFactor { ( * | / | DIV | MOD ) SFactor }
20. SFactor ::= [( - | + | NOT )] Factor 
21. Factor ::= IDENT | ICONST | RCONST | SCONST | BCONST | '(' Expr ')'