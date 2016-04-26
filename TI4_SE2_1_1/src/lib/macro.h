#ifndef DAV_LIB_AUX_H
#define DAV_LIB_AUX_H

// # ===========================================================================
// GARBAGE:
// # ===========================================================================

#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <errno.h>
#include <string.h>

//#define formatFILE(s) (s+31)
inline const char *formatFILE(const char *s)
{
	static const size_t l = 16; // == strlen("TI4_SE2_1_1/src/")
	const char *t = s;
	while(s && *s && memcmp(s, "TI4_SE2_1_1/src/", l)) ++s;
	return (s && *s) ? s + l : t;
}
//static const char *formatFILE(const char *s)
//{
//	return s + 31; // "Z:/git/.../src/" is cut off
//}

template<typename T>
void terminate(const char *s, int l, int e)
{
	std::stringstream ss;
	ss << "exception in " << (s + 31) << ":" << l << " with errno " << e << " (\"" << strerror(e) << "\")";
//	std::cout << ss.str();
	throw std::runtime_error(ss.str());
}

#define MXT_TODO_ERROR terminate<void>(__FILE__, __LINE__, errno)

// # ===========================================================================

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

#define MXT_PINPORT(port, pin) ((((uint32_t)(port))<<16)|((uint32_t)(pin)))
#define MXT_PORT(pp) ((uint16_t)((pp)>>16))
#define MXT_PINS(pp) ((uint8_t)((pp)&0xff))

#define MXT_SETBITS(v,m) ((v)|(m))
#define MXT_RESETBITS(v,m) ((v)&~(m))

#endif

