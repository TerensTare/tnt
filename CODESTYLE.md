# About
This file explains the coding style used on writing the TnT engine. If you want to contribute to the engine, please read and follow this guide to increase the readability of your code. If you use clang-format for formatting files, or your editor supports `.editorconfig`, you can skip all the parts related to formatting. The layout of this file is based on [https://gist.github.com/derofim/df604f2bf65a506223464e3ffd96a78a](https://gist.github.com/derofim/df604f2bf65a506223464e3ffd96a78a).


# Naming schemes
The following shortcuts are used in this section to denote naming schemes:

- CamelCased: The name starts with a capital letter, and has a capital letter for each new word, with no underscores.
- camelCased: Like CamelCase, but with a lower-case first letter
- under_scored: The name uses only lower-case letters, with words separated by underscores. (yes, I realize that under_scored should be underscored, because it's just one word).
- ALL_CAPITALS: All capital letters, with words separated by underscores.


## Bracket Style
C++ uses curly brackets for: defining a new scope, `namespace`, `class`/`struct`/`enum`/`union`, function/lambda, array, (direct/copy)-list-initialization, or an `if`/`else`/`for`/`while`/`switch` (pretty much everywhere). On the engine, the opening curly bracket are placed one line after the "head" of the entity (except of lambdas, where the bracket is on the same line as the lambda arguments), however, this is left as a decision to the contributor.


## Files
- Use `CamelCase` for the file names.
- Store header files on an `include` directory and source files on a `src` directory. Store module files on `modules` directory.
- The `.hpp` extension is preferred for header files, because it is C++ specific. The `.cpp` extension is preferred for source files.
- `#ifdef`/`#endif` is used as an include guard, but `#pragma once` is also acceptable since most of C++ compilers support it.
- `#include` should be avoided as much as possible on header files.
- If you want to use C++20 modules:
    - The `.ixx` extension is recommended for the module interface and `.cxx` is preferred for the module implementation unit.
    - `import` is preferred over `#include` to be used at least on source files/implementation unit.


## Forward declarations
Forward declarations should be avoided unless the header file containing the data definition is costly to be `#includ`-ed.
Keep in mind that forward declarations affect the order of `#include`-s (but not that of `import`-s). Also forward declarations are not allowed by the standard on the C++ modules.


## Order of #include-s
The engine uses this order/scheme of `#include`-s:
1. C/C++ standard headers.
2. 3rd-party libraries/system specific header files.
3. TnT's header files.
An empty newline is preferred on separating these categories, but this paragraph's rules don't matter/are optional as long as there is no forward declaration on the `#include`d files.


## Namespaces
Place the shared/global code on `namespace tnt` if it is engine-related code. If the code is related to a specific independent feature (ex. a reflection system), you can add a new `namespace` inside `tnt` (ex. `tnt::mirror`). On that case, `inline namespace` is allowed.
On header files, this rules should be followed strictly:
- No `using namespace` or `using a::b`.
- Place implementation code on `namespace tnt::YourNamespaceEmptyIfNone::detail`.


## Functions
- It is recommended that, for smaller build times, the implementation of public functions/member functions should be done on the source file that corresponds to the header file where they are declared (ex. functions from `someFile.hpp` should have their implementation on `someFile.cpp`). If the implementation is written on the header file (ex. templated functions, small functions), the functions should be marked `inline`.
- `static` (member) functions are allowed, but not preferred.
- Lambdas are preferred to be used when the function is short (up to 10 lines).
- Use `inline`/`noexcept`/`constexpr`/`explicit`/`const` where possible.
    - `inline` for functions with in-place implementation.
    - `noexcept` for functions that do not throw exceptions. Conditional `noexcept` is also recommended when necessary.
    - `constexpr` for functions to be used both on compile-time and run-time.
    - `explicit` for single-argument constructors (not copy or move) and conversion operators.
    - `const` for getters/non-modifying functions.
- Use attributes where possible.


## Variables
- Avoid global and `static` (member) variables.
- Initialize local variables in their declaration.
- Avoid declaring variables inside a `for`/`while`/`do-while` loop for performance reasons.
- Use `const&`/`const` for variables that are not meant to be modified. (ex. for-each loop or function arguments).
- Prefer to initialize member variables in place or on the member initializer list of the constructor. (see [this](https://stackoverflow.com/a/4589256/9807802)). ex:

``` c++
class MyClass
{
public:
explicit MyClass(int b)
    : a{b} // recommended if a depends on the constructor's arguments.
    {
        a = b; // not recommended.
    }


private:
    int a{10}; // recommended
};
```

## struct vs class
- On the engine, `struct` is usually used for types with only member variables and constructors (no member functions) or for types with `public` members. However, it is up to the contributor to use their favourite.


## Order of member declaration
The preferred order is as follows:
- `using`-declarations
- `static-assert`
- `enum`/`class`/`struct`/`union`
- `public` constructors + destructor (if any)
- `public` functions (if any)
- `public` members (if any)
The same goes for `protected` (if any) and the for `private` (if any).


## Comments
Comments should be used for:
- Writing the Doxygen-style documentation of public functions and variables. (at least `@brief`, `@param` (if any) and `@return` (if not `void`) for functions). `///` is preferred on this case.
- Explaining the code where magic numbers are used (please use `constexpr` variables instead of these). Check [this](https://en.wikipedia.org/wiki/Magic_number_%28programming%29) to learn about magic numbers and [src/imgui/ImGui.cpp](./src/imgui/ImGui.cpp#L685) for some examples of commenting out on these cases.


## Macros
- Use `constexpr` variables instead of macros that store a constant.
- Use `template` functions (or just functions) instead of function-like macros.

## auto
- `auto` is preferred to be used when the type of the variable reduces readability (ex. `MyCustomHashMap<MyCustomKeyType, MyCustomValueType>::iterator myVariable{ /* ... */ };` can be replaced with `auto myVariable{ /* ... */};`).
- NOTE: Be carefull to mark brace-initialised-list `auto` variables as `const&` or to use the following syntax:
``` c++
auto myVar = int{10}; // myVar is int
```
and not:
``` c++
auto myVar = {10}; // myVar is std::initializer_list<int>
```


## Exceptions, RTTI
- Exceptions are only allowed (but not advised) on utilities (like `tnt::Excepted<T>`).
- RTTI (``) is used only on the old ECS (which exists only for compatibility) and is totally prohibited to be used anywhere.


## Smart pointers
- Smart pointers are allowed, but not recommended. Please try to handle memory yourself (use references, call `operator delete`/`free()` for every `operator new`/`malloc()` call etc).


## Strings
`char const*` is the preferred type. Use `std::string_view` as a function argument and you want to pass a `std::string` to it, `char const*` otherwise. Use `std::pmr::string` to avoid memory allocations on the heap.


## Naming
- Use `camelCase` or `under_scored` for variables.
- Add enum name as a prefix to enum's memebers (unless it is an `enum class`).
- Use `CamelCase` or `under_scored` for `class`/`struct` names.
- Use `camelCase` for member function names with `CamelCase`-style `class` names and `under_scored` for member function names with `under_scored`-style `class` names.
- Use `CamelCase` for template types and `under_scored` for concepts.


## Indenting
Use 4 spaces to format the files. No whitespaces at the end of a line. Prefer to format with spaces instead of tabs.


## Other general stuff
- Prefer `using` over `typedef` or macros.
- Use UTF-8 for file encoding.
- If there are unfinished stuff, add a "TODO" comment on after the `#include`s or a `@todo` line on the doxygen description of the function/class/etc.
- If you have a boolean compounded by several boolean expressions connected with `&&` or `||`, you can separate it into different lines for better readability, but make sure the operator ends the line. Ex:
``` c++
// this is better
if(someLongBooleanExpressionNumber1 &&
    someLongBooleanExpressionNumber2)

// this is still ok
if(someLongBooleanExpressionNumber1
    && someLongBooleanExpressionNumber2)

// this is not
if(someLongBooleanExpressionNumber1 && someLongBooleanExpressionNumber2)
```
- Don't use `else` after a `return` inside an `if`. Ex:
``` c++
// good
int f1() noexcept
{
    if(cond)
    {
        return 0;
    }
    return 1;
}

// good, but else is redundant
int f2() noexcept
{
    if(cond)
    {
        return 0;
    }
    else
    {
        return 1;
    }
```

- Always add a space around assignment operators (`operator =`).
- When declaring template parameters, prefer `typename` over `class`.
- Don't use "p" or "ptr" as a prefix for pointers.
- Use english language on the code and comments.