#pragma once

#include <string>
#include <memory>
#include <map>

#include "tokenizer.h"

class Interpreter {
public:
    std::string Run(const std::string&);

    void Reset();

    ~Interpreter();

    Tokenizer* tokenizer = nullptr;
};
