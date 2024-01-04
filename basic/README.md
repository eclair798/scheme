# scheme-basic

Это третья часть [scheme](../../scheme). Её нужно делать после [scheme-parser](../parser).

- [scheme-basic](#scheme-basic)
  - [Что нужно сделать?](#что-нужно-сделать)
    - [1. Особая форма quote](#1-особая-форма-quote)
    - [2. Integer](#2-integer)
    - [3. Boolean](#3-boolean)
    - [4. List](#4-list)
  - [Обработка ошибок](#обработка-ошибок)
  - [Подсказки](#подсказки)
    - [Уровни абстракции](#уровни-абстракции)
    - [Как организовать процесс вычисления](#как-организовать-процесс-вычисления)
    - [Тестирование](#тестирование)

## Что нужно сделать?

В этой части мы научимся вычислять простейшие выражения, а именно такие, которые не содержат переменных и лямбда-функций.

### 1. Особая форма quote

В языке scheme существует ряд так называемых "особых форм". Одной из них является `quote` - операция, которая при вычислении просто возвращает свой аргумент, например

```scheme
$ (quote 1)
> 1

$ (quote (+ 1 2))
> (+ 1 2)
```

Также нужно поддержать специальный токен `'` - это синтаксический сахар для `quote`. То есть, `'1` = `(quote 1)`, `'(1 2)` = `(quote (1 2))` и так далее. Предлагается реализовывать поддержку `'` на уровне парсера, заменяя выражение с `'` на выражение с `quote`. 

Примеры использования:

```scheme
$ '(+ 1 2)
> (+ 1 2)

$ (quote (quote 1))
> (quote 1)

$ (quote '1)
> (quote 1)
```

### 2. Integer

В файле [test_integer.cpp](./tests/test_integer.cpp) вы найдете функции, которые нужно реализовать. Понять их смысл несложно из контекста. Обратите внимание, что многие из них работают не с двумя аргументами, а со списками произвольной (в том числе нулевой) длины.

Для представления чисел используйте целочисленный тип `int64_t`, считайте, что переполнения не происходит.

### 3. Boolean

В файле [test_boolean.cpp](./test_boolean.cpp) вы найдете все функции, которые нужно реализовать.

В тестах вы найдете функции `and`, `or` - логические выражения с _short-circuit evaluation_.

* `(and)`, `(and (= 2 2) (> 2 1))`
* `(or #t)`, `(or #t (crash everything))`

Они работают похоже на `&&` и `||` в C++ - вычисляют аргументы по порядку, останавливаясь, когда значение всего выражения уже не может поменяться. Их можно рассматривать как еще две особые формы.

При этом аргументами `and` и `or` могут быть не только boolean-ы, но и любые другие выражения. В scheme все выражения кроме `#f` считаются "истинными". Если вычисление дошло до последнего аргумента, и он не равен `#f`, то нужно его вернуть. Примеры:

* `(and 1 '1 (+ 1 2))` -> `3`, так как `1` и `'1` неявно истинны, а `(+ 1 2)` -> `3`
* `(and 3 4 (> 1 2) 5)` -> `#f`, так как `3` и `4` неявно истинны, а `(> 1 2)` вычисляется в `#f`

### 4. List

Аналогично, пишем функции, которые есть в [test_list.cpp](./test_list.cpp).

## Обработка ошибок

Ваш интерпретатор должен различать 3 вида ошибок:

1. `SyntaxError`: ошибки синтаксиса. Возникают, когда программа не соответствует формальному синтаксису языка. Или когда программа неправильно использует особые формы.

2. `NameError`: ошибки обращения к неопределённым переменным.

3. `RuntimeError`: ошибки времени исполнения. К этим ошибкам относятся все остальные ошибки, которые могут возникнуть во время выполнения программы. Например: неправильное количество аргументов передано в функцию, неправильный тип аргумента.

**При этом на любых входных данных никаких других исключений из вашего интерпретатора выбрасываться не должно.**

В этой подзадаче мы проверяем ваши решения с помощью еще одного [fuzzing-теста](./tests/test_fuzzing_2.cpp). В нем на вход интерпретатору подаются случайные последовательности токенов, а все вышеупомянутые исключения игнорируются. Задача интерпретатора в данном тесте - не выбросить никаких других объектов и не упасть по другим причинам. Для отладки укажите директиву `#define SCHEME_FUZZING_2_PRINT_REQUESTS` в файле scheme.h.

## Подсказки

### Уровни абстракции

Обратите внимание, что вычисление должно принимать на вход AST (выход парсера) и возвращать также AST. Сериализацию AST в строчку нужно производить после вычисления. Это будет особенно важно в дальнейшем при решении подзадачи advanced - и если вы сейчас сделаете неправильно, то потом будете многое переделывать.

Сериализацию проще всего реализовать рекурсивно аналогично парсингу.

### Как организовать процесс вычисления

* Постарайтесь избежать наличия в коде if-ов на каждый тип функции, так как их довольно много. Вспомните пример с обработчиками событий, который разбирался на прошлом семинаре. В нем мы пришли к выводу, что если интерфейс всех классов в дереве наследования одинаковый и не меняется часто, то виртуальные функции предпочтительнее, чем явная диспетчеризация в коде (с помощью `dynamic_cast` или `enum`-ов). Используйте такой дизайн в вашем решении.

* Напомним, что число вычисляется в себя, а пара и список - нет, они должны являться аргументами функций.

```scheme
$ 1
> 1

$ (1 2)
> RuntimeError

$ (1 . 2)
> RuntimeError
```

* Символ удобно сделать вычисляемым в некий объект функции, в который потом можно было бы передать аргументы и получить
  результат.

* В реализации вам может быть удобно работать со списком как с вектором, для этого можно написать хелпер, трансформирующий одно представление в другое.

* Функции часто делают похожие вещи, например, валидируют число и типы своих аргументов. Постарайтесь избежать дублирования кода, создавайте необходимые хелперы.

### Тестирование

* В basic запускаются в том числе тесты на парсер и токенайзер - они устроены также, как и в прошлых подзадачах.

* Тесты в этой подзадаче (и в двух последующих) используют вспомогательный [класс](./tests/scheme_test.h) `SchemeTest`. Из него вы поймете, какой интерфейс должен предоставлять ваш интерпретатор и как должны называться типы ошибок.

* Дополнительно собирается таргет `scheme_basic_repl`, который состоит из [repl/main.cpp](./repl/main.cpp) и ваших файлов с решениями. Он не участвует в тестировании на сервере, но может быть полезен вам при отладке - вы можете написать в нем свой REPL.