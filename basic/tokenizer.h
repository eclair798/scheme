#pragma once

#include <istream>
#include <map>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

enum class TokenName {
    SymbolToken,
    QuoteToken,
    DotToken,
    ConstantToken,
    OpenBracketToken,
    CloseBracketToken,
    EmptyToken
};

struct SymbolToken {
    std::string name;

    SymbolToken(std::string name) : name(name) {
    }
    bool operator==(const SymbolToken&) const;
};

struct QuoteToken {
    QuoteToken() {
    }
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    DotToken() {
    }
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int64_t value;
    ConstantToken(int64_t value) : value(value) {
    }
    bool operator==(const ConstantToken&) const;
};

struct EmptyToken {
    EmptyToken() {
    }
    bool operator==(const EmptyToken&) const;
};

using Token =
    std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, EmptyToken>;

struct Node {
    Node() {
    }

    Node(TokenName name) : name(name) {
    }

    TokenName name;

    Node* Step(char c, std::string& token_text);

    std::map<char, Node*> edges;
};

struct Graph {

    Graph();

    ~Graph();

    bool Read(std::istream* in);

    const std::vector<std::pair<TokenName, std::string>> symbols{
        {TokenName::ConstantToken, "0123456789"},
        {TokenName::SymbolToken, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<=>*/#"},
        {TokenName::SymbolToken, "+"},
        {TokenName::SymbolToken, "-"},
        {TokenName::DotToken, "."},
        {TokenName::QuoteToken, "'"},
        {TokenName::OpenBracketToken, "("},
        {TokenName::CloseBracketToken, ")"},
    };
    const std::string inside_symbol =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<=>*/#0123456789?!-";
    const std::string inside_constant = "0123456789";

    Node* root;
    bool end = false;

    std::string token_text;
    TokenName token_name;
};

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd() {
        return graph.end;
    }

    bool Next();

    Token GetToken() {
        return current_token;
    }

    ~Tokenizer() = default;

    Graph graph;
    Token current_token = EmptyToken();
    std::istream* in;
};