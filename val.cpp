#include "val.h"

// Numeric Operators
Value Value::operator+(const Value &op) const
{
    // cout << "Adding ";
    
    float op1, op2;
    bool error = false;
    bool isFloat = false;

    switch (GetType())
    {
    case VINT:
    {
        op1 = GetInt();
    }
    break;
    case VREAL:
    {
        op1 = GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (op.GetType())
    {
    case VINT:
    {
        op2 = op.GetInt();
    }
    break;
    case VREAL:
    {
        op2 = op.GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    // cout << op1 << " + " << op2 << endl;

    if (isFloat)
    {
        return Value(op1 + op2);
    }
    else
    {
        return Value((int)(op1 + op2));
    }
}

Value Value::operator-(const Value &op) const
{
    // cout << "Subtracting ";
    
    float op1, op2;
    bool error = false;
    bool isFloat = false;

    switch (GetType())
    {
    case VINT:
    {
        op1 = GetInt();
    }
    break;
    case VREAL:
    {
        op1 = GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (op.GetType())
    {
    case VINT:
    {
        op2 = op.GetInt();
    }
    break;
    case VREAL:
    {
        op2 = op.GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    // cout << op1 << " - " << op2 << endl;

    if (isFloat)
    {
        return Value(op1 - op2);
    }
    else
    {
        return Value((int)(op1 - op2));
    }
}

Value Value::operator*(const Value &op) const
{
    // cout << "Multiplying ";
    
    float op1, op2;
    bool error = false;
    bool isFloat = false;

    switch (GetType())
    {
    case VINT:
    {
        op1 = GetInt();
    }
    break;
    case VREAL:
    {
        op1 = GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (op.GetType())
    {
    case VINT:
    {
        op2 = op.GetInt();
    }
    break;
    case VREAL:
    {
        op2 = op.GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    // cout << op1 << " * " << op2 << endl;

    if (isFloat)
    {
        return Value(op1 * op2);
    }
    else
    {
        return Value((int)(op1 * op2));
    }
}

Value Value::operator/(const Value &op) const
{
    // cout << "Dividing ";
    
    float op1, op2;
    bool error = false;
    bool isFloat = false;

    switch (GetType())
    {
    case VINT:
    {
        op1 = GetInt();
    }
    break;
    case VREAL:
    {
        op1 = GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (op.GetType())
    {
    case VINT:
    {
        op2 = op.GetInt();
    }
    break;
    case VREAL:
    {
        op2 = op.GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    // cout << op1 << " / " << op2 << endl;

    if (isFloat)
    {
        return Value(op1 / op2);
    }
    else
    {
        return Value((int)(op1 / op2));
    }
}

Value Value::div(const Value &op) const
{
    float op1, op2;
    bool error = false;
    bool isFloat = false;

    switch (GetType())
    {
    case VINT:
    {
        op1 = GetInt();
    }
    break;
    case VREAL:
    {
        op1 = GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (op.GetType())
    {
    case VINT:
    {
        op2 = op.GetInt();
    }
    break;
    case VREAL:
    {
        op2 = op.GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }
    
    if (isFloat)
    {
        return Value((int)(op1 / op2));
    }
    else
    {
        return Value((int)(op1 / op2));
    }
}

Value Value::operator%(const Value &oper) const
{
    // cout << "Modulo ";
    
    float op1, op2;
    bool error = false;
    bool isFloat = false;

    switch (GetType())
    {
    case VINT:
    {
        op1 = GetInt();
    }
    break;
    case VREAL:
    {
        op1 = GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (oper.GetType())
    {
    case VINT:
    {
        op2 = oper.GetInt();
    }
    break;
    case VREAL:
    {
        op2 = oper.GetReal();
        isFloat = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        error = true;
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    // cout << op1 << " % " << op2;
    // cout << " = " << (int)op1 % (int)op2 << endl;

    if (isFloat)
    {
        return Value();
    }
    else
    {
        return Value((int)op1 % (int)op2);
    }
}

// Comparison Operators
Value Value::operator==(const Value &op) const
{
    // cout << "Equality\n";
    
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == type2)
    {
        switch (op.GetType())
        {
        case VINT:
        {
            return Value(GetInt() == op.GetInt());
        }
        break;
        case VREAL:
        {
            return Value(GetReal() == op.GetReal());
        }
        break;
        case VSTRING:
        {
            return Value(GetString() == op.GetString());
        }
        break;
        case VBOOL:
        {
            return Value(GetBool() == op.GetBool());
        }
        break;
        case VERR:
        {
            return Value();
        }
        break;
        }
    }
    else if (type1 == VINT && type2 == VREAL)
    {
        return Value((float)GetInt() == op.GetReal());
    }
    else if (type1 == VREAL && type2 == VINT)
    {
        return Value(GetReal() == (float)op.GetInt());
    }

    return Value();
}

Value Value::operator>(const Value &op) const
{
    ValType type1 = GetType();
    ValType type2 = op.GetType();

    // cout << type1 << ", " << type2 << " ";

    if (type1 == type2)
    {
        // cout << "matching types ";
        switch (type2)
        {
        case VINT:
        {
            return Value(GetInt() > op.GetInt());
        }
        break;
        case VREAL:
        {
            return Value(GetReal() > op.GetReal());
        }
        break;
        case VSTRING:
        {
            return Value();
        }
        break;
        case VBOOL:
        {
            return Value();
        }
        break;
        case VERR:
        {
            return Value();
        }
        break;
        }
    }
    else if (type1 == VINT && type2 == VREAL)
    {
        return Value((float)GetInt() > op.GetReal());
    }
    else if (type1 == VREAL && type2 == VINT)
    {
        return Value(GetReal() > (float)op.GetInt());
    }

    return Value();
}

Value Value::operator<(const Value &op) const
{
    // cout << "<\n";

    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == type2)
    {
        switch (op.GetType())
        {
        case VINT:
        {
            return Value(GetInt() < op.GetInt());
        }
        break;
        case VREAL:
        {
            return Value(GetReal() < op.GetReal());
        }
        break;
        case VSTRING:
        {
            return Value();
        }
        break;
        case VBOOL:
        {
            return Value();
        }
        break;
        case VERR:
        {
            return Value();
        }
        break;
        }
    }
    else if (type1 == VINT && type2 == VREAL)
    {
        return Value((float)GetInt() < op.GetReal());
    }
    else if (type1 == VREAL && type2 == VINT)
    {
        return Value(GetReal() < (float)op.GetInt());
    }

    return Value();
}

// Logic Operators
Value Value::operator&&(const Value &oper) const
{
    bool op1, op2;
    bool error = false;

    switch (GetType())
    {
    case VINT:
    {
        error = true;
    }
    break;
    case VREAL:
    {
        error = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        op1 = GetBool();
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (oper.GetType())
    {
    case VINT:
    {
        error = true;
    }
    break;
    case VREAL:
    {
        error = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        op2 = oper.GetBool();
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    return Value(op1 && op2);
}

Value Value::operator||(const Value &op) const
{
    bool op1, op2;
    bool error = false;

    switch (GetType())
    {
    case VINT:
    {
        error = true;
    }
    break;
    case VREAL:
    {
        error = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        op1 = GetBool();
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    switch (op.GetType())
    {
    case VINT:
    {
        error = true;
    }
    break;
    case VREAL:
    {
        error = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        op2 = op.GetBool();
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    return Value(op1 || op2);
}

Value Value::operator!() const
{
    // cout << "NOT ";
    
    bool op;
    bool error = false;

    switch (GetType())
    {
    case VINT:
    {
        error = true;
    }
    break;
    case VREAL:
    {
        error = true;
    }
    break;
    case VSTRING:
    {
        error = true;
    }
    break;
    case VBOOL:
    {
        op = GetBool();
    }
    break;
    case VERR:
    {
        error = true;
    }
    break;
    }

    if (error)
    {
        return Value();
    }

    // cout << "NOT " << op << endl;

    return Value(!op);
}