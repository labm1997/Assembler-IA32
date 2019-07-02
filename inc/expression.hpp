#ifndef HEADER_EXPRESSION
#define HEADER_EXPRESSION

#include <string>

using namespace std;

typedef enum {
    t_AddExpression,
    t_ContentOfLabel,
    t_ContentOfRegister,
    t_Label,
    t_LabelAdd,
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

class LabelAdd : public Expression {
    private:
    Label *label;
    Integer *offset;
    public:
    LabelAdd(Label *, Integer *);
    void prettyPrinter();
    ExpressionType type() { return t_LabelAdd; };
    uint32_t getAddress() {
        if(label == nullptr) return 0;
        if(offset == nullptr) return label->getAddress();
        return label->getAddress() + offset->getValue();
    };
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

// Defines class for [label+integer] on AT&T IA32 Assembly
class ContentOfLabel : public Expression {
    private:
    Label *label;
    Integer *offset;
    public:
    ContentOfLabel(Label *, Integer *);
    void prettyPrinter();
    ExpressionType type() { return t_ContentOfLabel; };
    Label *getLabel() {return label;};
    uint32_t getAddress() {
        if(label == nullptr) return 0;
        if(offset == nullptr) return this->label->getAddress();
        return this->label->getAddress() + this->offset->getValue();
    };
};

// Defines class for [register+integer] on AT&T IA32 Assembly
class ContentOfRegister : public Expression {
    private:
    Register *reg;
    Integer *offset;
    public:
    ContentOfRegister(Register *, Integer *);
    void prettyPrinter();
    ExpressionType type() { return t_ContentOfRegister; };
    Register *getRegister() {return reg;};
    Integer *getOffset() {return offset;};
};

#endif