#ifndef DAV_LIB_TEST_UNITTEST_H
#define DAV_LIB_TEST_UNITTEST_H

#include <test/TestManager.h>
#include <test/UnitTest.h>

#define MXT_STRINGIFY_IMPL(a) #a
#define MXT_STRINGIFY(a) MXT_STRINGIFY_IMPL(a)

// # ---------------------------------------------------------------------------

#define BEGIN \
namespace TEST_CASE_NAME { \
struct Base : public ::lib::test::UnitTest { }; \
static ::lib::test::TestManager::Selector select_unit_##TEST_CASE_NAME(MXT_STRINGIFY(TEST_CASE_NAME)); \
template<typename T> struct Data { }; \
template<typename T> struct Before : public virtual Data<T> { }; \
template<typename T> struct After : public virtual Data<T> { };

// # ---------------------------------------------------------------------------

#define END \
}

// # ---------------------------------------------------------------------------

#define DATA \
ThereCanBeOnlyOne { }; \
template<> \
struct Data<Base>

// # ---------------------------------------------------------------------------

#define BEFORE \
template<> \
struct Before<Base> : public virtual Data<Base> \
{ \
	Before( ); \
}; \
Before<Base>::Before(void)

// # ---------------------------------------------------------------------------

#define AFTER \
template<> \
struct After<Base> : public virtual Data<Base> \
{ \
	virtual ~After( ); \
}; \
After<Base>::~After(void)

// # ---------------------------------------------------------------------------

#define TEST(test_name) \
struct test_name : public Before<Base>, public After<Base>, public Base \
{ \
	void test( ); \
	static void wrapper( ); \
}; \
void test_name::wrapper(void) \
{ \
	test_name t; \
	t.test(); \
} \
static ::lib::test::TestManager::Registrar register_##test_name(#test_name, &test_name::wrapper); \
void test_name::test(void)

// # ---------------------------------------------------------------------------

#endif

