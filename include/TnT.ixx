// This file imports all TnT module partitions and serves as the main module for TnT.

#if __cplusplus >= 201704L
module;

export module tnt;

export import tnt : logger;
#else
inline static constexpr bool v = []() {
    constexpr bool value{false};
    static_assert(value, "File TnT.ixx contents are available only with C++20");
    return false;
}();
#endif