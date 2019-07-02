#ifndef HEADER_EXPRESSION
#define HEADER_EXPRESSION

#include <string>

using namespace std;

typedef enum {
    t_AddExpression,
    t_ContentOf,
    t_Label,
    t_Integer,
    t_Register
} ExpressionType;

class Expression {
    public:
    virtual void prettyPrinter() = 0;
    virtual ExpressionType type() = 0;
    bool isType(ExpressionType type) { return type == this->type(); };
};

class AtomicExpression : public Expression {
    public:
    virtual void prettyPrinter() = 0;
    virtual ExpressionType type() = 0;
};

// Defines class for Expression+Expression on AT&T IA32 Assembly
class AddExpression : public Expression {
    AtomicExpression *lhs;
    AtomicExpression *rhs;
    public:
    AddExpression(AtomicExpression *, AtomicExpression *);
    void prettyPrinter();
    ExpressionType type() { return t_AddExpression; };
};

// Defines class for [Expression] on AT&T IA32 Assembly
class ContentOf : public Expression {
    private:
    Expression *exp;
    public:
    ContentOf(Expression *);
    void prettyPrinter();
    ExpressionType type() { return t_ContentOf; };
    Expression *getExpression() {return exp;};
};

// Defines class for a label that points to some memory address
class Label : public AtomicExpression {
    private:
    string label;
    uint32_t address;

    public:
    Label(string);
    uint32_t getAddress();
    void setAddress(uint32_t);
    string getName();
    void prettyPrinter();
    ExpressionType type() { return t_Label; };
};

// Defines class for an integer
class Integer : public AtomicExpression {
    private:
    int32_t value;

    public:
    Integer(int32_t);
    void prettyPrinter();
    ExpressionType type() { return t_Integer; };
    int32_t getValue() { return value; };
};

// Defines class for a register
class Register : public AtomicExpression {
    private:
    string registerName;

    public:
    Register(string);
    string getName();
    void prettyPrinter();
    ExpressionType type() { return t_Register; };
};

#endif