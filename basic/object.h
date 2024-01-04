#pragma once

#include <memory>

#include "error.h"
#include "tokenizer.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;

    virtual std::string Print() {
    }
};

class Number : public Object {
public:
    Number(int value) : value(value) {
    }

    virtual std::string Print() override {
        return std::to_string(value);
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

    virtual std::string Print() override {
        return name;
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

    std::shared_ptr<Object> GetFirst() const {
        return first;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second;
    }

    virtual std::string Print() override {
        std::string ans;

        ans += "(";
        if (first == nullptr) {
            ans += "()";
        } else {
            ans += first->Print();
        }
        while (second != nullptr && std::dynamic_pointer_cast<Cell>(second) != nullptr) {
            first = std::dynamic_pointer_cast<Cell>(second)->first;
            second = std::dynamic_pointer_cast<Cell>(second)->second;
            ans += " ";
            ans += first->Print();
        }

        if (second == nullptr) {
            ans += ")";
            return ans;
        } else {
            ans += " . ";
            ans += second->Print();
            ans += ")";
        }

        return ans;
    }

    std::shared_ptr<Object> first = nullptr;
    std::shared_ptr<Object> second = nullptr;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and convertion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(obj);
    if (derived == nullptr) {
        throw RuntimeError("Wrong type");
    }
    return derived;
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(obj);
    return derived != nullptr;
}
