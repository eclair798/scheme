#include <parser.h>

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Empty");
        //        return nullptr;
    }

    if (std::holds_alternative<BracketToken>(tokenizer->GetToken())) {
        if (std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::OPEN) {
            tokenizer->Next();
            return ReadList(tokenizer);
        }
    }

    if (std::holds_alternative<DotToken>(tokenizer->GetToken())) {
        tokenizer->Next();
        return nullptr;
    }

    if (std::holds_alternative<QuoteToken>(tokenizer->GetToken())) {
        //        tokenizer->Next();
        throw SyntaxError("Quote error");
        //        return nullptr;
    }

    if (std::holds_alternative<ConstantToken>(tokenizer->GetToken())) {
        int value = std::get<ConstantToken>(tokenizer->GetToken()).value;
        tokenizer->Next();
        return std::shared_ptr<Object>(new Number(value));
    }

    if (std::holds_alternative<SymbolToken>(tokenizer->GetToken())) {
        std::string name = std::get<SymbolToken>(tokenizer->GetToken()).name;
        tokenizer->Next();
        return std::shared_ptr<Object>(new Symbol(name));
    }

    tokenizer->Next();
    return nullptr;
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Brackets error");
        //        return nullptr;
    }
    if (std::holds_alternative<DotToken>(tokenizer->GetToken())) {
        throw SyntaxError("Dot Error");
    }
    if (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
        std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE) {
        tokenizer->Next();
        return nullptr;
    }
    std::shared_ptr<Object> first = nullptr;
    std::shared_ptr<Object> second;
    while (first == nullptr && !tokenizer->IsEnd()) {
        first = Read(tokenizer);
    }
    if (first == nullptr) {
        return nullptr;
    }
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Brackets error");
        //        return first;
    }
    if (std::holds_alternative<DotToken>(tokenizer->GetToken())) {
        tokenizer->Next();
        if (tokenizer->IsEnd() ||
            (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
             std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE)) {
            throw SyntaxError("Dot error");
        }
        second = Read(tokenizer);
        if (tokenizer->IsEnd() || !std::holds_alternative<BracketToken>(tokenizer->GetToken()) ||
            std::get<BracketToken>(tokenizer->GetToken()) != BracketToken::CLOSE) {
            throw SyntaxError("Dot error");
        }
        tokenizer->Next();
        return std::shared_ptr<Object>(new Cell(first, second));
    }
    second = ReadList(tokenizer);
    if (second == nullptr) {
        return first;
    }
    return std::shared_ptr<Object>(new Cell(first, second));
}