#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; // Container of temporary locations of Value objects for results of expressions, variables values and constants
queue<Value> *ValQue;            // declare a pointer variable to a queue of Value objects

namespace Parser
{
    bool pushed_back = false;
    LexItem pushed_token;

    static LexItem GetNextToken(istream &in, int &line)
    {
        if (pushed_back)
        {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(LexItem &t)
    {
        if (pushed_back)
        {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;
}
// Prog ::= PROGRAM IDENT ; DeclPart CompoundStmt
bool Prog(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    if (t != PROGRAM)
    {
        ParseError(line, "Missing PROGRAM keyword");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != IDENT)
    {
        ParseError(line, "Missing program name IDENT");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != SEMICOL)
    {
        ParseError(line, "Missing ; after program name");
        return false;
    }

    status = DeclPart(in, line);
    if (!status)
    {
        ParseError(line, "Error in variable declaration");
        return false;
    }

    status = CompoundStmt(in, line);
    if (!status)
    {
        ParseError(line, "Error in program body");
    }

    return status;
}

// done
// DeclPart ::= VAR DeclStmt; { DeclStmt ; }
bool DeclPart(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    if (t != VAR)
    {
        ParseError(line, "Missing VAR keyword in variable declaration");
        return false;
    }

    status = DeclStmt(in, line);
    if (!status)
    {
        ParseError(line, "Error in variable declaration statement");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != SEMICOL)
    {
        Parser::PushBackToken(t);
    }

    while (true)
    {
        status = DeclStmt(in, line);
        if (!status)
        {
            break;
        }

        t = Parser::GetNextToken(in, line);
        if (t != SEMICOL)
        {
            ParseError(line, "Missing ; after variable declaration");
            return false;
        }

        // peek ahead if BEGIN, DeclPart is done if so;
        t = Parser::GetNextToken(in, line);
        Parser::PushBackToken(t);
        if (t == BEGIN)
        {
            return true;
        }
    }

    return status;
}

// DeclStmt ::= IDENT {, IDENT } : Type [:= Expr]
bool DeclStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;
    vector<LexItem> currDecl;
    Token varType;
    Value retVal;

    t = Parser::GetNextToken(in, line);
    if (t != IDENT)
    {
        ParseError(line, "Missing variable IDENT in variable declaration");
        return false;
    }
    else
    {
        // add to currDecl if isn't defined already
        if (defVar.find(t.GetLexeme()) != defVar.end())
        {
            ParseError(line, "Variable redeclaration");
            return false;
        }
        else
        {
            currDecl.push_back(t);
        }
    }

    while (true)
    {
        t = Parser::GetNextToken(in, line);
        if (t != COMMA)
        {
            Parser::PushBackToken(t);
            break;
        }

        t = Parser::GetNextToken(in, line);
        if (t != IDENT)
        {
            ParseError(line, "Missing variable IDENT in variable declaration");
            return false;
        }
        else
        {
            // add to currDecl if isn't defined already
            if (defVar.find(t.GetLexeme()) != defVar.end())
            {
                ParseError(line, "Variable redeclaration");
                return false;
            }
            else
            {
                currDecl.push_back(t);
            }
        }
    }

    t = Parser::GetNextToken(in, line);
    if (t != COLON)
    {
        ParseError(line, "Missing : after variable identifier list");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (!(t == INTEGER || t == REAL || t == STRING || t == BOOLEAN))
    {
        ParseError(line, "Variable type missing after colon in declaration");
        return false;
    }
    else
    {
        varType = t.GetToken();
    }

    bool isASSOP = false;
    t = Parser::GetNextToken(in, line);
    if (t == ASSOP)
    {
        status = Expr(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Missing expression after assop in variable declaration");
            return false;
        }

        isASSOP = true;
    }
    else
    {
        Parser::PushBackToken(t);
    }

    // add all to defVar
    for (LexItem item : currDecl)
    {
        defVar[item.GetLexeme()] = isASSOP;
        SymTable[item.GetLexeme()] = varType;

        // add all to TempsResults if assigned.
        if (isASSOP)
        {

            TempsResults[item.GetLexeme()] = retVal;
        }
    }

    return true;
}

// CompoundStmt ::= BEGIN Stmt {; Stmt } END
bool CompoundStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    if (t != BEGIN)
    {
        ParseError(line, "Missing BEGIN keyword before program body");
        return false;
    }

    status = Stmt(in, line);
    if (!status)
    {
        ParseError(line, "Missing statement in compound statement");
        return false;
    }

    while (true)
    {
        t = Parser::GetNextToken(in, line);
        if (t != SEMICOL)
        {
            Parser::PushBackToken(t);
            break;
        }

        status = Stmt(in, line);
        if (!status)
        {
            ParseError(line, "Missing statement in compound statement");
            return false;
        }
    }

    t = Parser::GetNextToken(in, line);
    if (t != END)
    {
        ParseError(line, "Missing keyword END at end of program body");
        return false;
    }

    return status;
}

// Stmt ::= SimpleStmt | StructuredStmt
bool Stmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    /*
        AssignStmt -> Var -> IDENT
        WriteLnStmt -> WRITELN
        WriteStmt -> WRITE
        IfStmt -> IF
        CompoundStmt -> BEGIN
    */

    t = Parser::GetNextToken(in, line);
    Parser::PushBackToken(t);

    switch (t.GetToken())
    {
    case IDENT:
    case WRITELN:
    case WRITE:
    {
        status = SimpleStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error parsing SimpleStmt in Stmt");
            return false;
        }
    }
    break;

    case IF:
    case BEGIN:
    {
        status = StructuredStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error parsing StructuredStmt in Stmt");
            return false;
        }
    }
    break;

    default:
        ParseError(line, "Error in Stmt at (" + t.GetLexeme() + ")");
        return false;
    }

    return status;
}

// SimpleStmt ::= AssignStmt | WriteLnStmt | WriteStmt
bool SimpleStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    Parser::PushBackToken(t);

    switch (t.GetToken())
    {
    case IDENT:
    {
        status = AssignStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error in AssignStmt");
            return false;
        }
        break;
    }

    case WRITELN:
    {
        status = WriteLnStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error in WriteLnStmt");
            return false;
        }
        break;
    }

    case WRITE:
    {
        status = WriteStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error in WriteStmt");
            return false;
        }
        break;
    }

    default:
        ParseError(line, "Error in SimpleStmt");
        return false;
    }

    return status;
}

// StructuredStmt ::= IfStmt | CompoundStmt
bool StructuredStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    Parser::PushBackToken(t);

    switch (t.GetToken())
    {
    case IF:
    {
        status = IfStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error in IfStmt");
            return false;
        }
        break;
    }

    case BEGIN:
    {
        status = CompoundStmt(in, line);
        if (!status)
        {
            ParseError(line, "Error in CompoundStmt");
            return false;
        }
        break;
    }

    default:
        ParseError(line, "Error in StructuredStmt");
        return false;
        break;
    }

    return true;
}

// WriteLnStmt ::= WRITELN (ExprList)
bool WriteLnStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    ValQue = new queue<Value>;

    t = Parser::GetNextToken(in, line);
    if (t != WRITELN)
    {
        ParseError(line, "Missing keyword WRITELN in WriteStmt");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {
        ParseError(line, "Missing ( in WriteStmt");
        return false;
    }

    status = ExprList(in, line);
    if (!status)
    {
        ParseError(line, "Error in ExprList");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {
        ParseError(line, "Missing ) in WriteStmt");
        return false;
    }

    // Evaluate: print out the list of expressions' values
    while (!(*ValQue).empty())
    {
        Value nextVal = (*ValQue).front();
        cout << nextVal;
        ValQue->pop();
    }
    cout << endl;

    return status;
}

// WriteStmt ::= WRITE (ExprList)
bool WriteStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;

    ValQue = new queue<Value>;

    t = Parser::GetNextToken(in, line);
    if (t != WRITE)
    {
        ParseError(line, "Missing keyword WRITE in WriteStmt");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {
        ParseError(line, "Missing ( in WriteStmt");
        return false;
    }

    status = ExprList(in, line);
    if (!status)
    {
        ParseError(line, "Error in ExprList");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {
        ParseError(line, "Missing ) in WriteStmt");
        return false;
    }

    while (!(*ValQue).empty())
    {
        Value nextVal = (*ValQue).front();
        cout << nextVal;
        ValQue->pop();
    }

    return true;
}

// ROOT retVal
// IfStmt ::= IF Expr THEN Stmt [ ELSE Stmt ]
bool IfStmt(istream &in, int &line)
{
    LexItem t;
    bool status;
    Value retVal;

    t = Parser::GetNextToken(in, line);
    if (t != IF)
    {
        ParseError(line, "Missing keyword IF in IfStmt");
        return false;
    }

    status = Expr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing Expr in IfStmt");
        return false;
    }

    if (!retVal.IsBool())
    {
        ParseError(line, "Runtime Error: Expected boolean expression");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != THEN)
    {
        ParseError(line, "Missing keyword THEN in IfStmt");
        return false;
    }

    if (retVal.GetBool())
    {
        status = Stmt(in, line);
        if (!status)
        {
            ParseError(line, "Missing Stmt in IfStmt");
            return false;
        }

        
        /*
            if BEGIN -> consume till END
            if anything else -> consume till SEMICOL
        */

        // check if ELSE clause
        t = Parser::GetNextToken(in, line);
        if (t != ELSE)
        {
            Parser::PushBackToken(t);
        }
        else
        {
            t = Parser::GetNextToken(in, line);
            if (t == BEGIN)
            {
                while (true)
                {
                    t = Parser::GetNextToken(in, line);
                    if (t == END)
                    {
                        break;
                    }
                }
            }
            else
            {
                while (true)
                {
                    t = Parser::GetNextToken(in, line);
                    if (t == SEMICOL)
                    {
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // consume THEN stmt
        t = Parser::GetNextToken(in, line);
        if (t == BEGIN)
        {
            while (true)
            {
                t = Parser::GetNextToken(in, line);
                if (t == END)
                {
                    break;
                }
            }
        }
        else
        {
            while (true)
            {
                t = Parser::GetNextToken(in, line);
                if (t == ELSE || t == SEMICOL)
                {
                    break;
                }
            }
        }

        // while (true)
        // {
        //     t = Parser::GetNextToken(in, line);
        //     if (t == ELSE || t == SEMICOL)
        //     {
        //         Parser::PushBackToken(t);
        //         break;
        //     }
        // }

        t = Parser::GetNextToken(in, line);
        if (t == ELSE)
        {
            status = Stmt(in, line);
            if (!status)
            {
                ParseError(line, "Missing Stmt in IfStmt after ELSE");
                return false;
            }
        }
        else
        {
            Parser::PushBackToken(t);
        }
    }

    return true;
}

// ROOT retVal
// AssignStmt ::= Var := Expr
bool AssignStmt(istream &in, int &line)
{
    LexItem t;
    bool status = false;
    Value retVal;
    LexItem identToken;

    status = Var(in, line, identToken);
    if (!status)
    {
        ParseError(line, "Missing Var in AssignStmt");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != ASSOP)
    {
        ParseError(line, "Missing ASSOP in AssignStmt");
        return false;
    }

    status = Expr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing Expr in AssignStmt");
        return false;
    }

    /*
        Val.h enum    ->    Lex.h enum
        VINT, VREAL (0,1)   INTEGER (4)
        VINT, VREAL (0,1)   REAL (5)
        VSTRING (2)         STRING (7)
        VBOOL (3)           BOOLEAN (6)
    */
    switch (SymTable[identToken.GetLexeme()])
    {
    case INTEGER:
        if (retVal.GetType() == VINT)
        {
            TempsResults[identToken.GetLexeme()] = retVal;
        }
        else if (retVal.GetType() == VREAL)
        {
            TempsResults[identToken.GetLexeme()] = Value((int)retVal.GetReal());
        }
        else
        {
            ParseError(line, "Runtime Error: Illegal Mixed-Mode Assignment");
            return false;
        }
        break;
    case REAL:
        if (retVal.GetType() == VREAL)
        {
            TempsResults[identToken.GetLexeme()] = retVal;
        }
        else if (retVal.GetType() == VINT)
        {
            TempsResults[identToken.GetLexeme()] = Value((float)retVal.GetInt());
        }
        else
        {
            ParseError(line, "Runtime Error: Illegal Mixed-Mode Assignment");
            return false;
        }
        break;
    case STRING:
        if (retVal.GetType() != VSTRING)
        {
            ParseError(line, "Runtime Error: Illegal Mixed-Mode Assignment");
            return false;
        }
        else
        {
            TempsResults[identToken.GetLexeme()] = retVal;
        }
        break;
    case BOOLEAN:
        if (retVal.GetType() != VBOOL)
        {
            ParseError(line, "Runtime Error: Illegal Mixed-Mode Assignment");
            return false;
        }
        else
        {
            TempsResults[identToken.GetLexeme()] = retVal;
        }
        break;

    default:
        break;
    }

    return status;
}

// Var ::= IDENT
bool Var(istream &in, int &line, LexItem &idtok)
{
    LexItem t;

    t = Parser::GetNextToken(in, line);
    if (t != IDENT)
    {
        ParseError(line, "Missing IDENT in Var");
        return false;
    }
    else
    {
        if (defVar.find(t.GetLexeme()) == defVar.end())
        {
            ParseError(line, "Undeclared variable (" + t.GetLexeme() + ")");
            return false;
        }

        idtok = t;
    }

    return true;
}

// ROOT retVal
// ExprList ::= Expr { , Expr }
bool ExprList(istream &in, int &line)
{
    LexItem t;
    bool status = false;
    Value retVal;

    status = Expr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing Expression");
        return false;
    }
    else
    {
        ValQue->push(retVal);
    }

    while (true)
    {
        t = Parser::GetNextToken(in, line);
        if (t != COMMA)
        {
            Parser::PushBackToken(t);
            break;
        }

        status = Expr(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Expected Expression after COMMA");
            return false;
        }
        else
        {
            ValQue->push(retVal);
        }
    }

    return status;
}

// Expr ::= LogAndExpr { OR LogAndExpr }
bool Expr(istream &in, int &line, Value &retVal)
{
    LexItem t;
    bool status = false;

    status = LogANDExpr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing LogANDExpr in Expr");
        return false;
    }

    Value val1 = retVal;

    while (true)
    {
        t = Parser::GetNextToken(in, line);
        if (t != OR)
        {
            Parser::PushBackToken(t);
            break;
        }

        status = LogANDExpr(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Expected LogANDExpr after OR");
            return false;
        }

        retVal = val1 || retVal;
    }

    return status;
}

// LogAndExpr ::= RelExpr {AND RelExpr }
bool LogANDExpr(istream &in, int &line, Value &retVal)
{
    LexItem t;
    bool status = false;

    status = RelExpr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing RelExpr in LogANDExpr");
        return false;
    }

    Value val1 = retVal;

    while (true)
    {
        t = Parser::GetNextToken(in, line);
        if (t != AND)
        {
            Parser::PushBackToken(t);
            break;
        }

        status = RelExpr(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Expected RelExpr after AND");
            return false;
        }

        retVal = val1 && retVal;
    }

    return status;
}

// RelExpr ::= SimpleExpr [ ( = | < | > ) SimpleExpr ]
bool RelExpr(istream &in, int &line, Value &retVal)
{
    LexItem t;
    bool status = false;

    status = SimpleExpr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing SimpleExpr in RelExpr");
        return false;
    }

    Value val1 = retVal;

    t = Parser::GetNextToken(in, line);
    if (t == EQ || t == LTHAN || t == GTHAN)
    {
        status = SimpleExpr(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Expected SimpleExpr afer Operator");
            return false;
        }

        switch (t.GetToken())
        {
        case EQ:
            retVal = val1 == retVal;
            break;
        case LTHAN:
            retVal = val1 < retVal;
            break;
        case GTHAN:
            retVal = val1 > retVal;
            break;
        default:
            break;
        }
    }
    else
    {
        Parser::PushBackToken(t);
    }

    return status;
}

// SimpleExpr :: Term { ( + | - ) Term }
bool SimpleExpr(istream &in, int &line, Value &retVal)
{
    LexItem t;
    bool status = false;

    status = Term(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing Term in SimpleExpr");
        return false;
    }

    Value val1 = retVal;

    while (true)
    {
        t = Parser::GetNextToken(in, line);
        if (!(t == PLUS || t == MINUS))
        {
            Parser::PushBackToken(t);
            break;
        }

        status = Term(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Expected Term after Operator");
            return false;
        }

        switch (t.GetToken())
        {
        case PLUS:
            val1 = val1 + retVal;
            break;
        case MINUS:
            val1 = val1 - retVal;
            break;
        default:
            break;
        }
    }

    retVal = val1;

    return status;
}

// Term ::= SFactor { ( * | / | DIV | MOD ) SFactor }
bool Term(istream &in, int &line, Value &retVal)
{
    LexItem t;
    bool status = false;

    status = SFactor(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing SFactor in Term");
        return false;
    }

    Value val1 = retVal;

    while (true)
    {
        t = Parser::GetNextToken(in, line);

        if (t == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern: (" + t.GetLexeme() + ")");
        }

        if (!(t == MULT || t == DIV || t == IDIV || t == MOD))
        {
            Parser::PushBackToken(t);
            break;
        }

        status = SFactor(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Expected SFactor after OP");
            return false;
        }

        switch (t.GetToken())
        {
        case MULT:
            val1 = val1 * retVal;
            break;
        case DIV:
            if ((retVal.GetType() == VINT && retVal.GetInt() == 0) || (retVal.GetType() == VREAL && retVal.GetReal() == 0))
            {
                ParseError(line, "Runtime Error: Division by zero");
                return false;
            }
            val1 = val1 / retVal;
            break;
        case IDIV:
            if ((retVal.GetType() == VINT && retVal.GetInt() == 0) || (retVal.GetType() == VREAL && retVal.GetReal() == 0))
            {
                ParseError(line, "Runtime Error: Integer division by zero");
                return false;
            }

            val1 = val1.div(retVal);
            break;
        case MOD:
            val1 = val1 % retVal;
            break;

        default:
            break;
        }
    }

    retVal = val1;

    return status;
}

// SFactor ::= [( - | + | NOT )] Factor
bool SFactor(istream &in, int &line, Value &retVal)
{
    LexItem t, op;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    if (t == MINUS || t == PLUS || t == NOT)
    {
        op = t;
    }
    else
    {
        Parser::PushBackToken(t);
    }

    status = Factor(in, line, retVal, 0);
    if (!status)
    {
        ParseError(line, "Missing factor in SFactor");
        return false;
    }

    switch (op.GetToken())
    {
    case MINUS:
        retVal = Value(-1) * retVal;
        break;
    case PLUS:
        retVal = Value(1) * retVal;
        break;
    case NOT:
        retVal = !retVal;
        break;

    default:
        break;
    }

    if (retVal.GetType() == VERR)
    {
        ParseError(line, "Runtime Error: Illegal Operation");
        return false;
    }

    return status;
}

// Factor ::= IDENT | ICONST | RCONST | SCONST | BCONST | (Expr)
bool Factor(istream &in, int &line, Value &retVal, int sign)
{
    LexItem t;
    bool status = false;

    t = Parser::GetNextToken(in, line);
    if (t == LPAREN)
    {
        status = Expr(in, line, retVal);
        if (!status)
        {
            ParseError(line, "Missing expression in Factor");
            return false;
        }

        t = Parser::GetNextToken(in, line);
        if (t != RPAREN)
        {
            ParseError(line, "Expected ) after expression");
            return false;
        }
    }
    else if (!(t == IDENT || t == ICONST || t == RCONST || t == SCONST || t == BCONST))
    {
        ParseError(line, "Expected an IDENT or CONST");
        return false;
    }
    else if ((t == IDENT || t == ICONST || t == RCONST || t == SCONST || t == BCONST))
    {
        switch (t.GetToken())
        {
        case IDENT:
            // check if variable is defined
            if (TempsResults.find(t.GetLexeme()) == TempsResults.end())
            {
                ParseError(line, "Runtime Error: Uninitialized Variable");
                return false;
            }
            else
            {
                // get the variable from storage
                retVal = TempsResults[t.GetLexeme()];
            }
            break;
        case ICONST:
            retVal = Value(std::stoi(t.GetLexeme()));
            break;
        case RCONST:
            retVal = Value(std::stof(t.GetLexeme()));
            break;
        case SCONST:
            retVal = Value(t.GetLexeme());
            break;
        case BCONST:
            if (t.GetLexeme() == "true")
            {
                retVal = Value(true);
            }
            else
            {
                retVal = Value(false);
            }
            break;
        default:
            break;
        }
    }

    return true;
}