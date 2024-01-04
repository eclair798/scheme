#pragma once

#include <memory>

#include "tokenizer.h"

// enum class TokenName {
//     SymbolToken,
//     QuoteToken,
//     DotToken,
//     ConstantToken,
//     OpenBracketToken,
//     CloseBracketToken,
//     EmptyToken
// };

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;

    virtual bool IsNumber() {
        return false;
    }
    virtual bool IsSymbol() {
        return false;
    }
    virtual bool IsCell() {
        return false;
    }
    virtual bool IsEmpty() {
        return true;
    }
};

class Number : public Object {
public:
    Number(int value) : value(value) {
    }

    bool IsNumber() override {
        return true;
    }
    bool IsEmpty() override {
        return false;
    }

    int GetValue() const {
        return value;
    }

    int value;
};

class Symbol : public Object {
public:
    Symbol(std::string name) : name(name) {
    }

    bool IsSymbol() override {
        return true;
    }
    bool IsEmpty() override {
        return false;
    }

    const std::string& GetName() const {
        return name;
    }

    std::string name;
};

class Cell : public Object {
public:
    Cell(std::shared_ptr<Object> f, std::shared_ptr<Object> s) : first(f), second(s) {
    }

    bool IsCell() override {
        return true;
    }
    bool IsEmpty() override {
        return false;
    }

    std::shared_ptr<Object> GetFirst() const {
        return first;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second;
    }

    std::shared_ptr<Object> first;
    std::shared_ptr<Object> second;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and conversion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(obj);
    if (derived == nullptr) {
        if constexpr (std::is_same_v<T, Cell>) {
            std::shared_ptr<T> ptr(new Cell(obj, nullptr));
            return ptr;
        }
    }
    return derived;
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    if constexpr (std::is_same_v<T, Cell>) {
        return true;
    }
    std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(obj);
    return derived != nullptr;
}
