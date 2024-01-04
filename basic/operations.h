#pragma once

#include <memory>

#include "parser.h"
#include "scheme.h"

std::shared_ptr<Object> Calc(std::shared_ptr<Object> obj);

std::shared_ptr<Object> Apply(std::shared_ptr<Object> first, std::shared_ptr<Object> second);

class Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) {
        return obj;
    }

    virtual ~Operation() = default;
};

class Quote : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Plus : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Minus : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Multiplication : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Division : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Equal : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Less : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class More : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class LessE : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class MoreE : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Not : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class And : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Or : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Max : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Min : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Abs : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class List : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class ListRef : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class ListTail : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Cons : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Car : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Cdr : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class IsList : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class IsNull : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class IsPair : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class IsNumber : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class IsBoolean : public Operation {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<Object> obj) override;
};

class Application {
public:
    static inline std::map<std::string, std::shared_ptr<Operation>> rf = {
        {"quote", std::shared_ptr<Operation>(new Quote())},
        {"+", std::shared_ptr<Operation>(new Plus())},
        {"-", std::shared_ptr<Operation>(new Minus())},
        {"*", std::shared_ptr<Operation>(new Multiplication())},
        {"/", std::shared_ptr<Operation>(new Division())},

        {"=", std::shared_ptr<Operation>(new Equal())},
        {"<", std::shared_ptr<Operation>(new Less())},
        {">", std::shared_ptr<Operation>(new More())},
        {"<=", std::shared_ptr<Operation>(new LessE())},
        {">=", std::shared_ptr<Operation>(new MoreE())},

        {"not", std::shared_ptr<Operation>(new Not())},
        {"and", std::shared_ptr<Operation>(new And())},
        {"or", std::shared_ptr<Operation>(new Or())},

        {"max", std::shared_ptr<Operation>(new Max())},
        {"min", std::shared_ptr<Operation>(new Min())},
        {"abs", std::shared_ptr<Operation>(new Abs())},

        {"list", std::shared_ptr<Operation>(new List())},
        {"list-ref", std::shared_ptr<Operation>(new ListRef())},
        {"list-tail", std::shared_ptr<Operation>(new ListTail())},

        {"cons", std::shared_ptr<Operation>(new Cons())},
        {"car", std::shared_ptr<Operation>(new Car())},
        {"cdr", std::shared_ptr<Operation>(new Cdr())},

        {"list?", std::shared_ptr<Operation>(new IsList())},
        {"null?", std::shared_ptr<Operation>(new IsNull())},
        {"pair?", std::shared_ptr<Operation>(new IsPair())},
        {"number?", std::shared_ptr<Operation>(new IsNumber())},
        {"boolean?", std::shared_ptr<Operation>(new IsBoolean())}};
};

std::shared_ptr<Object> GetFirstArg(std::shared_ptr<Object> obj);

std::shared_ptr<Object> GetSecondArg(std::shared_ptr<Object> obj);

size_t Length(std::shared_ptr<Object> obj);

bool AsBool(std::shared_ptr<Object> obj);
