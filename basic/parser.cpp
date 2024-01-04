#include <parser.h>

#include "error.h"

bool IsBracketOpen(Tokenizer* tokenizer) {
    return (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
            std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::OPEN);
}

bool IsBracketClose(Tokenizer* tokenizer) {
    return (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
            std::get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE);
}

bool IsDot(Tokenizer* tokenizer) {
    return std::holds_alternative<DotToken>(tokenizer->GetToken());
}

bool IsQuote(Tokenizer* tokenizer) {
    return std::holds_alternative<QuoteToken>(tokenizer->GetToken());
}

bool IsConstant(Tokenizer* tokenizer) {
    return std::holds_alternative<ConstantToken>(tokenizer->GetToken());
}

bool IsSymbol(Tokenizer* tokenizer) {
    return std::holds_alternative<SymbolToken>(tokenizer->GetToken());
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Empty");
    }

    if (IsBracketClose(tokenizer)) {
        throw SyntaxError("Why )");
    }

    if (IsBracketOpen(tokenizer)) {
        tokenizer->Next();

        if (std::holds_alternative<SymbolToken>(tokenizer->GetToken()) &&
            std::get<SymbolToken>(tokenizer->GetToken()).name == "quote") {
            tokenizer->Next();
            if (tokenizer->IsEnd() || IsBracketClose(tokenizer)) {
                throw SyntaxError("Quote error");
            }
            std::shared_ptr<Object> first(new Symbol("quote"));
            auto second = Read(tokenizer);
            if (tokenizer->IsEnd() || !IsBracketClose(tokenizer)) {
                throw SyntaxError("Quote error");
            }
            tokenizer->Next();
            return std::shared_ptr<Object>(new Cell(first, second));
        }

        if (!tokenizer->IsEnd() && IsBracketClose(tokenizer)) {
            tokenizer->Next();
            return nullptr;
        }
        auto read = ReadList(tokenizer);
        return read;
    }

    if (IsDot(tokenizer)) {
        throw SyntaxError("Dot error");
    }

    if (IsQuote(tokenizer)) {
        std::shared_ptr<Object> first(new Symbol("quote"));
        tokenizer->Next();
        if (tokenizer->IsEnd() || IsBracketClose(tokenizer)) {
            throw SyntaxError("Quote error");
        }
        //        return first;
        auto second = Read(tokenizer);
        return std::shared_ptr<Object>(new Cell(first, second));
    }

    if (IsConstant(tokenizer)) {
        int64_t value = std::get<ConstantToken>(tokenizer->GetToken()).value;
        tokenizer->Next();
        return std::shared_ptr<Object>(new Number(value));
    }

    if (IsSymbol(tokenizer)) {
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
    }

    if (IsDot(tokenizer)) {
        throw SyntaxError("Dot Error");
    }

    if (IsBracketClose(tokenizer)) {
        tokenizer->Next();
        return nullptr;
    }

    //    if (IsQuote(tokenizer)) {
    //        auto first = Read(tokenizer);
    //        if (tokenizer->IsEnd() || !IsBracketClose(tokenizer)) {
    //            throw SyntaxError("Quote error");
    //        }
    //        tokenizer->Next();
    //        return first;
    //    }

    std::shared_ptr<Object> first;
    std::shared_ptr<Object> second;

    first = Read(tokenizer);

    if (tokenizer->IsEnd()) {
        throw SyntaxError("Brackets error");
    }

    if (IsDot(tokenizer)) {
        tokenizer->Next();
        if (tokenizer->IsEnd() || IsBracketClose(tokenizer)) {
            throw SyntaxError("Dot error");
        }
        second = Read(tokenizer);
        if (tokenizer->IsEnd() || !IsBracketClose(tokenizer)) {
            throw SyntaxError("Dot error");
        }
        tokenizer->Next();
        return std::shared_ptr<Object>(new Cell(first, second));
    }

    second = ReadList(tokenizer);

    return std::shared_ptr<Object>(new Cell(first, second));
}