#pragma once

#include <memory>

#include "object.h"
#include "tokenizer.h"

std::shared_ptr<Object> Read(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);

bool IsBracketOpen(Tokenizer* tokenizer);

bool IsBracketClose(Tokenizer* tokenizer);

bool IsDot(Tokenizer* tokenizer);

bool IsQuote(Tokenizer* tokenizer);

bool IsConstant(Tokenizer* tokenizer);

bool IsSymbol(Tokenizer* tokenizer);
