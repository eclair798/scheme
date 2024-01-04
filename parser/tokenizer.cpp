#include <tokenizer.h>

#include "error.h"

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}
bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}
bool DotToken::operator==(const DotToken&) const {
    return true;
}
bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}
bool EmptyToken::operator==(const EmptyToken& other) const {
    return true;
}
Node* Node::Step(char c, std::string& token_text) {
    if (edges.find(c) != edges.end()) {
        token_text += c;
        return edges[c];
    }

    return nullptr;
}
Graph::Graph() : root(new Node()) {
    for (auto pair : symbols) {
        Node* vertex = new Node(pair.first);
        for (auto x : pair.second) {
            root->edges[x] = vertex;
        }

        if (pair.first == TokenName::SymbolToken && pair.second[0] == 'A') {
            for (auto x : inside_symbol) {
                vertex->edges[x] = vertex;
            }
        } else if (pair.first == TokenName::SymbolToken) {
            for (auto x : inside_constant) {
                vertex->edges[x] = root->edges[x];
            }
        }

        if (pair.first == TokenName::ConstantToken) {
            for (auto x : pair.second) {
                vertex->edges[x] = vertex;
            }
        }
    }
}

bool Graph::Read(std::istream* in) {
    Node* now = root;
    token_text = "";
    token_name = TokenName::EmptyToken;
    if (end) {
        return false;
    }

    while (true) {
        char c = in->get();
        if (in->eof()) {
            break;
        }
        now = now->Step(c, token_text);
        if (now == nullptr) {
            if (!token_text.empty()) {
                in->putback(c);
            } else {
                if (c != ' ' && c != '\n') {
                    throw SyntaxError("HOOEY");
                }
                now = root;
                continue;
            }
            break;
        } else {
            token_name = now->name;
        }
    }
    if (token_name == TokenName::EmptyToken) {
        end = true;
        return false;
    } else {
        return true;
    }
}
Graph::~Graph() {
    std::string codes = "A0()+-'.";
    for (char x : codes) {
        delete root->edges[x];
    }
    delete root;
}

Tokenizer::Tokenizer(std::istream* in) : in(in) {
    Next();
}
bool Tokenizer::Next() {
    if (!graph.Read(in)) {
        return false;
    }
    switch (graph.token_name) {
        case TokenName::SymbolToken:
            current_token = SymbolToken(graph.token_text);
            return true;
        case TokenName::QuoteToken:
            current_token = QuoteToken();
            return true;
        case TokenName::DotToken:
            current_token = DotToken();
            return true;
        case TokenName::ConstantToken:
            current_token = ConstantToken(std::stoi(graph.token_text));
            return true;
        case TokenName::OpenBracketToken:
            current_token = BracketToken::OPEN;
            return true;
        case TokenName::CloseBracketToken:
            current_token = BracketToken::CLOSE;
            return true;
        case TokenName::EmptyToken:
            return false;
    }
}
