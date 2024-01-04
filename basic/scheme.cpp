#include <sstream>

#include "error.h"
#include "operations.h"
#include "parser.h"
#include "scheme.h"

void Interpreter::Reset() {
    if (tokenizer != nullptr) {
        delete tokenizer;
        tokenizer = nullptr;
    }
}

Interpreter::~Interpreter() {
    Reset();
}

std::string Interpreter::Run(const std::string& string) {
    Reset();
    std::stringstream ss{string};
    tokenizer = new Tokenizer(&ss);

    std::shared_ptr<Object> obj = Read(tokenizer);
    if (!tokenizer->IsEnd()) {
        throw SyntaxError("Why not end");
    }

    if (obj == nullptr) {
        throw RuntimeError("Empty");
    }

    auto ret = Calc(obj);
    if (ret == nullptr) {
        return "()";
    } else {
        return ret->Print();
    }
}
