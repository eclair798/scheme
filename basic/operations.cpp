#include "operations.h"
#include "error.h"

std::shared_ptr<Object> GetFirstArg(std::shared_ptr<Object> obj) {
    if (!Is<Cell>(obj)) {
        return obj;
    }
    obj = As<Cell>(obj)->first;
    obj = Calc(obj);
    return obj;
}

std::shared_ptr<Object> GetSecondArg(std::shared_ptr<Object> obj) {
    if (!Is<Cell>(obj)) {
        return nullptr;
    }
    obj = As<Cell>(obj)->second;
    obj = GetFirstArg(obj);
    return obj;
}

size_t Length(std::shared_ptr<Object> obj) {
    size_t count = 0;
    while (obj != nullptr || Is<Cell>(obj)) {
        obj = As<Cell>(obj)->second;
        ++count;
    }
    if (obj != nullptr) {
        ++count;
    }
    return count;
}

bool AsBool(std::shared_ptr<Object> obj) {
    if (Is<Symbol>(obj) &&
        (As<Symbol>(obj)->GetName() == "#t" || As<Symbol>(obj)->GetName() == "#f")) {
        return As<Symbol>(obj)->GetName() == "#t";
    }
    return true;
}

std::shared_ptr<Object> Apply(std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
    if (!Is<Symbol>(first)) {
        throw RuntimeError("Why not symbol");
    }
    std::string name = As<Symbol>(first)->GetName();
    if (Application::rf.find(name) == Application::rf.end()) {
        throw NameError("Unknown name");
    }
    auto obj = Application::rf[name]->Apply(second);
    return obj;
}
std::shared_ptr<Object> Calc(std::shared_ptr<Object> obj) {
    if (!Is<Cell>(obj)) {
        return obj;
    }
    auto first = As<Cell>(obj)->first;
    auto second = As<Cell>(obj)->second;
    return Apply(first, second);
}

std::shared_ptr<Object> Quote::Apply(std::shared_ptr<Object> obj) {
    return obj;
}

std::shared_ptr<Object> Plus::Apply(std::shared_ptr<Object> obj) {
    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Number(0));
    }

    auto first = GetFirstArg(obj);
    auto second = As<Cell>(obj)->second;

    int64_t ans1 = As<Number>(first)->GetValue();

    int64_t ans2 = As<Number>(Apply(second))->GetValue();

    return std::shared_ptr<Object>(new Number(ans1 + ans2));
}
std::shared_ptr<Object> Minus::Apply(std::shared_ptr<Object> obj) {
    if (obj == nullptr) {
        throw RuntimeError("RE in Minus");
    }

    auto first = GetFirstArg(obj);
    auto second = As<Cell>(obj)->second;

    int64_t ans1 = As<Number>(first)->GetValue();

    Plus p;
    int64_t ans2 = As<Number>(p.Apply(second))->GetValue();

    return std::shared_ptr<Object>(new Number(ans1 - ans2));
}

std::shared_ptr<Object> Multiplication::Apply(std::shared_ptr<Object> obj) {
    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Number(1));
    }

    auto first = GetFirstArg(obj);
    auto second = As<Cell>(obj)->second;

    int64_t ans1 = As<Number>(first)->GetValue();

    int64_t ans2 = As<Number>(Apply(second))->GetValue();

    return std::shared_ptr<Object>(new Number(ans1 * ans2));
}

std::shared_ptr<Object> Division::Apply(std::shared_ptr<Object> obj) {
    if (obj == nullptr) {
        throw RuntimeError("RE in Division");
    }

    auto first = GetFirstArg(obj);
    auto second = As<Cell>(obj)->second;

    int64_t ans1 = As<Number>(first)->GetValue();

    Multiplication m;
    int64_t ans2 = As<Number>(m.Apply(second))->GetValue();

    return std::shared_ptr<Object>(new Number(ans1 / ans2));
}

std::shared_ptr<Object> Equal::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) <= 1) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    bool ans = false;

    while (second != nullptr && Is<Cell>(obj)) {
        ans = As<Number>(first)->GetValue() == As<Number>(second)->GetValue();
        if (!ans) {
            return std::shared_ptr<Object>(new Symbol("#f"));
        }
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
    }

    return std::shared_ptr<Object>(new Symbol(ans ? "#t" : "#f"));
}
std::shared_ptr<Object> Less::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) <= 1) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    bool ans = false;

    while (second != nullptr && Is<Cell>(obj)) {
        ans = As<Number>(first)->GetValue() < As<Number>(second)->GetValue();
        if (!ans) {
            return std::shared_ptr<Object>(new Symbol("#f"));
        }
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
    }

    return std::shared_ptr<Object>(new Symbol(ans ? "#t" : "#f"));
}
std::shared_ptr<Object> More::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) <= 1) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    bool ans = false;

    while (second != nullptr && Is<Cell>(obj)) {
        ans = As<Number>(first)->GetValue() > As<Number>(second)->GetValue();
        if (!ans) {
            return std::shared_ptr<Object>(new Symbol("#f"));
        }
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
    }

    return std::shared_ptr<Object>(new Symbol(ans ? "#t" : "#f"));
}
std::shared_ptr<Object> LessE::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) <= 1) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    bool ans = false;

    while (second != nullptr && Is<Cell>(obj)) {
        ans = As<Number>(first)->GetValue() <= As<Number>(second)->GetValue();
        if (!ans) {
            return std::shared_ptr<Object>(new Symbol("#f"));
        }
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
    }

    return std::shared_ptr<Object>(new Symbol(ans ? "#t" : "#f"));
}
std::shared_ptr<Object> MoreE::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) <= 1) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    bool ans = false;

    while (second != nullptr && Is<Cell>(obj)) {
        ans = As<Number>(first)->GetValue() >= As<Number>(second)->GetValue();
        if (!ans) {
            return std::shared_ptr<Object>(new Symbol("#f"));
        }
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
    }

    return std::shared_ptr<Object>(new Symbol(ans ? "#t" : "#f"));
}

std::shared_ptr<Object> Not::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);
    bool ans = AsBool(obj);
    return std::shared_ptr<Object>(new Symbol(!ans ? "#t" : "#f"));
}

std::shared_ptr<Object> And::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) == 0) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    std::shared_ptr<Object> first;

    while (Is<Cell>(obj)) {
        first = GetFirstArg(obj);
        obj = As<Cell>(obj)->second;
        if (!AsBool(first)) {
            return first;
        }
    }
    return first;
}
std::shared_ptr<Object> Or::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) == 0) {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }

    std::shared_ptr<Object> first;

    while (Is<Cell>(obj)) {
        first = GetFirstArg(obj);
        obj = As<Cell>(obj)->second;
        if (AsBool(first)) {
            return first;
        }
    }
    return first;
}

std::shared_ptr<Object> Max::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) == 0) {
        throw RuntimeError("0 arg(s)");
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    int64_t ans = As<Number>(first)->GetValue();

    while (second != nullptr && Is<Cell>(obj)) {
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
        ans = ans > As<Number>(first)->GetValue() ? ans : As<Number>(first)->GetValue();
    }

    return std::shared_ptr<Object>(new Number(ans));
}
std::shared_ptr<Object> Min::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) == 0) {
        throw RuntimeError("0 arg(s)");
    }

    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    int64_t ans = As<Number>(first)->GetValue();

    while (second != nullptr && Is<Cell>(obj)) {
        obj = As<Cell>(obj)->second;
        first = GetFirstArg(obj);
        second = GetSecondArg(obj);
        ans = ans < As<Number>(first)->GetValue() ? ans : As<Number>(first)->GetValue();
    }

    return std::shared_ptr<Object>(new Number(ans));
}

std::shared_ptr<Object> Abs::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);

    return std::unique_ptr<Object>(new Number(std::abs(As<Number>(obj)->GetValue())));
}

std::shared_ptr<Object> List::Apply(std::shared_ptr<Object> obj) {
    return Operation::Apply(obj);
}
std::shared_ptr<Object> ListRef::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 2) {
        throw RuntimeError("Not 2 arg(s)");
    }
    auto list = GetFirstArg(obj);
    auto temp = GetSecondArg(obj);
    int64_t arg = As<Number>(temp)->GetValue();

    for (int64_t i = 0; i < arg; ++i) {
        if (list == nullptr) {
            throw RuntimeError("Out of range");
        }
        list = As<Cell>(list)->second;
    }
    list = As<Cell>(list)->first;
    return list;
}
std::shared_ptr<Object> ListTail::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 2) {
        throw RuntimeError("Not 2 arg(s)");
    }
    auto list = GetFirstArg(obj);
    auto temp = GetSecondArg(obj);
    int64_t arg = As<Number>(temp)->GetValue();

    for (int64_t i = 0; i < arg; ++i) {
        if (list == nullptr) {
            throw RuntimeError("Out of range");
        }
        list = As<Cell>(list)->second;
    }
    return list;
}
std::shared_ptr<Object> Cons::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 2) {
        throw RuntimeError("Not 2 arg(s)");
    }
    auto first = GetFirstArg(obj);
    auto second = GetSecondArg(obj);

    return std::shared_ptr<Object>(new Cell(first, second));
}
std::shared_ptr<Object> Car::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);

    auto first = As<Cell>(obj)->first;
    auto second = As<Cell>(obj)->second;
    return first;
}
std::shared_ptr<Object> Cdr::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);

    auto first = As<Cell>(obj)->first;
    auto second = As<Cell>(obj)->second;
    return second;
}

std::shared_ptr<Object> IsList::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);

    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    if (Is<Cell>(obj)) {

        auto first = As<Cell>(obj)->first;
        auto second = As<Cell>(obj)->second;

        while (second != nullptr && Is<Cell>(second)) {
            second = As<Cell>(second)->second;
        }

        if (second == nullptr) {
            return std::shared_ptr<Object>(new Symbol("#t"));
        } else {
            return std::shared_ptr<Object>(new Symbol("#f"));
        }

    } else {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }
}
std::shared_ptr<Object> IsNull::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);

    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    if (Is<Cell>(obj) && As<Cell>(obj)->first == nullptr) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    }

    return std::shared_ptr<Object>(new Symbol("#f"));
}
std::shared_ptr<Object> IsPair::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }

    obj = GetFirstArg(obj);
    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }

    if (Is<Cell>(obj)) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    } else {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }
}
std::shared_ptr<Object> IsNumber::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);
    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }

    if (Is<Number>(obj)) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    } else {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }
}

std::shared_ptr<Object> IsBoolean::Apply(std::shared_ptr<Object> obj) {
    if (Length(obj) != 1) {
        throw RuntimeError("Not 1 arg(s)");
    }
    obj = GetFirstArg(obj);
    if (obj == nullptr) {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }

    if (Is<Symbol>(obj) &&
        (As<Symbol>(obj)->GetName() == "#t" || As<Symbol>(obj)->GetName() == "#f")) {
        return std::shared_ptr<Object>(new Symbol("#t"));
    } else {
        return std::shared_ptr<Object>(new Symbol("#f"));
    }
}
