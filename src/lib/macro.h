#ifndef DAV_LIB_AUX_H
#define DAV_LIB_AUX_H

#define MXT_STRINGIFY_IMPL(a) #a
#define MXT_STRINGIFY(a) MXT_STRINGIFY_IMPL(a)

#define MXT_COUNTARGS_(a1_, a2_, a3_, a4_, a5_, a6_, a7_, a8_, a9_, a10_, a11_, a12_, a13_, a14_, a15_, a16_, a17_, a18_, a19_, N, ...) N
#define MXT_COUNTARGS(...) MXT_COUNTARGS_(__VA_ARGS__, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define MXT_CONCAT_(a, b) a##b
#define MXT_CONCAT(a, b) MXT_CONCAT_(a, b)

#define MXT_OVERLOAD(m, ...) MXT_CONCAT(m, MXT_COUNTARGS(__VA_ARGS__))(__VA_ARGS__)

/*************************************
 * Variadic Macros
 * Usage:
 *
 * #define MACRO_3(a, b, c) // something
 * #define MACRO_2(a, b)    MACRO_3(a, b, default_c)
 * #define MACRO_1(a)       MACRO_2(a, default_b)
 * #define MACRO_0          MACRO_1(default_a)
 * #define MACRO(...)       MXT_OVERLOAD(MACRO_, __VA_ARGS__)
 *
 *************************************/

#endif

