// Avanish Kulkarni
// CS280-007
// PA

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include "lex.h"

LexItem getNextToken(istream& in, int& linenum)
{
    // make a big tree drawing
    
    enum LexState 
    {
        START, ERROR, IN_COM, IN_WORD,

        IN_INT, IN_REAL,

        IN_STR,

        POS_ASS,

    }; 


    LexState state = START;
    std::string lexeme;
    char ch;

    while (in.get(ch))
    {
        // std::cout << "c: " << ch;
        // std::cout << " st: " << state;
        // std::cout << " lx: " << lexeme;
        // std::cout << std::endl;
        
        switch (state)
        {
        case START:

            lexeme.clear();

            if (ch == '\n')
            {
                state = START;
                linenum++;
            }
            else if (isspace(ch))
            {
                state = START;
            }
            else if (ch == '+') 
            { 
                lexeme.push_back(ch);
                return LexItem(PLUS, lexeme, linenum); 
            }
            else if (ch == '-') 
            {
                lexeme.push_back(ch);
                return LexItem(MINUS, lexeme, linenum);
            }
            else if (ch == '*')
            {
                lexeme.push_back(ch);
                return LexItem(MULT, lexeme, linenum);
            }
            else if (ch == '/')
            {
                lexeme.push_back(ch);
                return LexItem(DIV, lexeme, linenum);
            }
            else if (ch == '=')
            {
                lexeme.push_back(ch);
                return LexItem(EQ, lexeme, linenum);
            }
            else if (ch == '<')
            {
                lexeme.push_back(ch);
                return LexItem(LTHAN, lexeme, linenum);
            }
            else if (ch == '>')
            {
                lexeme.push_back(ch);
                return LexItem(GTHAN, lexeme, linenum);
            }
            else if (ch == ',')
            {
                lexeme.push_back(ch);
                return LexItem(COMMA, lexeme, linenum);
            }
            else if (ch == ';')
            {
                lexeme.push_back(ch);
                return LexItem(SEMICOL, lexeme, linenum);
            }
            else if (ch == '(')
            {
                lexeme.push_back(ch);
                return LexItem(LPAREN, lexeme, linenum);
            }
            else if (ch == ')')
            {
                lexeme.push_back(ch);
                return LexItem(RPAREN, lexeme, linenum);
            }
            else if (ch == '.')
            {
                lexeme.push_back(ch);
                return LexItem(DOT, lexeme, linenum);
            }

            // comments            
            else if (ch == '{')
            {
                state = IN_COM;
            }

            // words -> idents, keywords
            else if (isalpha(ch))
            {
                state = IN_WORD;
                lexeme.push_back(ch);
            }
            
            else if (isdigit(ch))
            {
                state = IN_INT;
                lexeme.push_back(ch);
            }

            // string const
            else if (ch == '\'')
            {
                state = IN_STR;
                lexeme.push_back(ch);
            }

            // assignment operator
            else if (ch == ':')
            {
                state = POS_ASS;
                lexeme.push_back(ch);
            }

            else 
            {
                state = ERROR;
                lexeme.push_back(ch);
            }

            break;
            
        case IN_COM:

            if (ch == '\n')
            {
                linenum++;
            }

            if (ch == '}') 
            {
                state = START;
            }

            break;

        case IN_WORD:

            if (isalnum(ch) || ch == '_' || ch == '$')
            {
                lexeme.push_back(ch);
            }
            else 
            {
                state = START;
                in.putback(ch);
                return id_or_kw(lexeme, linenum);
            }

            break;

        case IN_INT:

            if (isdigit(ch)) 
            {
                state = IN_INT;
                lexeme.push_back(ch);
            }
            else if (ch == '.')
            {
                state = IN_REAL;
                lexeme.push_back(ch);
            }
            else 
            {
                state = START;
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }

            break;

        case IN_REAL:
            if (isdigit(ch))
            {
                lexeme.push_back(ch);
            }
            else if (ch == '.')
            {
                state = ERROR;
                lexeme.push_back(ch);
            }
            else 
            {
                state = START;
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
            }

            break;

        case IN_STR:
            
            if (ch == '\'')
            {
                state = START;
                return LexItem(SCONST, lexeme.substr(1, lexeme.length() - 1), linenum);
            }
            else if (ch == '\n')
            {
                state = ERROR;
                in.putback(ch);
            }
            else 
            {
                state = IN_STR;
                lexeme.push_back(ch);
            }
            
            break;
        
        case POS_ASS:
            if (ch == '=')
            {
                state = START;
                return LexItem(ASSOP, lexeme, linenum);
            }
            else 
            {
                state = START;
                return LexItem(COLON, lexeme, linenum);
            }

            break;

        case ERROR:
            return LexItem(ERR, lexeme, linenum);
            break;
        }
    }    
    
    // compensate for extra linenum added
    linenum--;

    return LexItem(DONE, "", linenum);
}


LexItem id_or_kw(const string& lexeme, int linenum)
{
    std::map<std::string, Token> reservedWords = {
        {"and", AND},
        {"begin", BEGIN},
        {"boolean", BOOLEAN},
        {"end", END},
        {"else", ELSE},
        {"div", IDIV},
        {"if", IF},
        {"integer", INTEGER},
        {"mod", MOD},
        {"not", NOT},
        {"or", OR},
        {"program", PROGRAM},
        {"real", REAL},
        {"string", STRING},
        {"then", THEN},
        {"write", WRITE},
        {"writeln", WRITELN},
        {"var", VAR},
    };

    if (reservedWords.find(lexeme) != reservedWords.end())
    {
        LexItem item(reservedWords[lexeme], lexeme, linenum);
        return item;
    }
    else if (lexeme == "true" || lexeme == "false")
    {
        LexItem item(BCONST, lexeme, linenum);
        return item;
    }
    else
    {
        LexItem item(IDENT, lexeme, linenum);
        return item;
    }
}

// done
ostream& operator<<(ostream& out, const LexItem& tok) 
{ 
    const char* tokenArray[] =
    {
        "IF", "ELSE", "WRITELN", "WRITE", "INTEGER", "REAL",
        "BOOLEAN", "STRING", "BEGIN", "END", "VAR", "THEN", "PROGRAM",

        "IDENT", "TRUE", "FALSE",
        "ICONST", "RCONST", "SCONST", "BCONST",

        "PLUS", "MINUS", "MULT", "DIV", "IDIV", "MOD", "ASSOP", "EQ", 
        "GTHAN", "LTHAN", "AND", "OR", "NOT", 

        "COMMA", "SEMICOL", "LPAREN", "RPAREN", "DOT", "COLON",
        
        "ERR",

        "DONE",
    };
    
    
    
    if (tok == IDENT || tok == ICONST || tok == RCONST || tok == SCONST || tok == BCONST)
    {
        std::cout << tokenArray[tok.GetToken()] << ": \"" << tok.GetLexeme() << "\"";
    }
    else 
    {
        std::cout << tokenArray[tok.GetToken()];
    }

    return out;
}


