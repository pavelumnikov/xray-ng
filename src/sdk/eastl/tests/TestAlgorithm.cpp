/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#if defined(_MSC_VER)
	// We have little choice but to disable this warning. See the FAQ for why.
	#pragma warning(disable: 4244) // conversion from '___' to '___', possible loss of data
#endif


#include <EASTL/algorithm.h>
#include <EASTL/functional.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/array.h>
#include <EASTL/deque.h>
#include <EASTL/list.h>
#include <EASTL/slist.h>
#include <EASTL/string.h>
#include <EASTL/set.h>
#include <EASTL/sort.h>
#include "ConceptImpls.h"
#include <EASTL/EAStdC/EAMemory.h>
#include "EASTLTest.h" // Put this after the above so that it doesn't block any warnings from the includes above.

namespace
{
	struct A
	{
		A(int n) : a(n){}
		int a;
	};
	struct LessStruct{ bool operator()(const A& a1, const A& a2){ return a1.a < a2.a; } };


	struct B
	{
		B(int n) : b(n){}
		int b;
	};
	inline bool LessFunction(const B& b1, const B& b2){ return b1.b < b2.b; }

    /// back_insert_iterator
    ///
    /// A back_insert_iterator is simply a class that acts like an iterator but when you 
    /// assign a value to it, it calls push_back on the container with the value.
    ///
    template <size_t N>
    class char_back_insert_iterator : public eastl::iterator<EASTL_ITC_NS::output_iterator_tag, void, void, void, void>
    {
    public:
        using this_type = char_back_insert_iterator<N>;
        using const_reference = char const&;

    protected:
        char* m_array_ptr;
        size_t m_current_iter;

    public:
        explicit char_back_insert_iterator(char* x)
            : m_array_ptr { x }
            , m_current_iter { 0 }
        {}

        char_back_insert_iterator& operator=(const_reference value)
        {
            EASTL_ASSERT(m_current_iter < N);
            m_array_ptr[m_current_iter++] = value; return *this;
        }

        char_back_insert_iterator& operator=(char&& value)
        {
            EASTL_ASSERT(m_current_iter < N);
            m_array_ptr[m_current_iter++] = value; return *this;
        }

        char_back_insert_iterator& operator*()
        {
            return *this;
        }

        char_back_insert_iterator& operator++()
        {
            return *this;
        } // This is by design.

        char_back_insert_iterator operator++(int)
        {
            return *this;
        } // This is by design.

    protected:
        void operator=(const this_type&)
        {} // Declared to avoid compiler warnings about inability to generate this function.
    };

    template <size_t N>
    inline char_back_insert_iterator<N>
        char_back_inserter(char* x)
    {
        return char_back_insert_iterator<N>(x);
    }
}

enum TestMinMaxEnum
{
	teX = 0, 
	teY = 3 
};


///////////////////////////////////////////////////////////////////////////////
// Greater
//
// A version of greater that uses operator < instead of operator >.
//
template <typename T>
struct Greater : public eastl::binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const { return (b < a); }
};


///////////////////////////////////////////////////////////////////////////////
// DivisibleBy
//
struct DivisibleBy
{
	int d;
	DivisibleBy(int n = 1) : d(n) {}
	bool operator()(int n) const { return ((n % d) == 0); }
};


///////////////////////////////////////////////////////////////////////////////
// TestObjectNegate
//
struct TestObjectNegate : public eastl::unary_function<TestObject, TestObject>
{
	TestObject operator()(const TestObject& a) const
		{ return TestObject(-a.mX); }
};

TEST_CASE("TestAlgorithm")
{
	using namespace eastl;
	TestObject::Reset();

	// const T& min(const T& a, const T& b);
	// const T& min(const T& a, const T& b, Compare compare)
	// const T& max(const T& a, const T& b);
	// const T& max(const T& a, const T& b, Compare compare)
	// const T& min_alt(const T& a, const T& b);
	// const T& min_alt(const T& a, const T& b, Compare compare)
	// const T& max_alt(const T& a, const T& b);
	// const T& max_alt(const T& a, const T& b, Compare compare)

	SECTION("Test min&max with LessStruct", "[struct A]")
	{
		A a1 { 1 }, a2 { 2 }, a3 { 3 };
		a3 = eastl::min(a1, a2, LessStruct());
		REQUIRE(a3.a == 1);
		a3 = eastl::max(a1, a2, LessStruct());
		REQUIRE(a3.a == 2);
	}

	SECTION("Test min&max with boolean func LessStruct", "[struct B]")
	{
		B b1 { 1 }, b2 { 2 }, b3 { 3 };
		b3 = eastl::min(b2, b1, LessFunction);
		REQUIRE(b3.b == 1);
		b3 = eastl::max(b2, b1, LessFunction);
		REQUIRE(b3.b == 2);
	}

	SECTION("Test min&max with TestObject", "[class TestObject]")
	{
		TestObject t1 { 1 }, t2 { 2 }, t3 { 3 };
		t3 = eastl::min(t2, t1);
		REQUIRE(t3.mX == 1);
		t3 = eastl::max(t2, t1);
		REQUIRE(t3.mX == 2);
	}

	SECTION("Test min&max with triple int")
	{
		int32_t i1, i2 { -1 }, i3 { 1 };
		i1 = eastl::min(i2, i3);
		REQUIRE(i1 == -1);
		i1 = eastl::min(i3, i2);
		REQUIRE(i1 == -1);
		i1 = eastl::max(i2, i3);
		REQUIRE(i1 == 1);
		i1 = eastl::max(i3, i2);
		REQUIRE(i1 == 1);
	}

	SECTION("Test min&max with triple volatile int")
	{
		int32_t i1;
		const volatile int32_t i2cv { -1 }, i3cv { 1 };
		i1 = eastl::min(i2cv, i3cv);
		REQUIRE(i1 == -1);
		i1 = eastl::min(i3cv, i2cv);
		REQUIRE(i1 == -1);
		i1 = eastl::max(i2cv, i3cv);
		REQUIRE(i1 == 1);
		i1 = eastl::max(i3cv, i2cv);
		REQUIRE(i1 == 1);
	}

	SECTION("Test min&max with triple float")
	{
		float f1, f2(-1), f3(1);
		f1 = eastl::min(f2, f3);
		REQUIRE(f1 == -1);
		f1 = eastl::min(f3, f2);
		REQUIRE(f1 == -1);
		f1 = eastl::max(f2, f3);
		REQUIRE(f1 == 1);
		f1 = eastl::max(f3, f2);
		REQUIRE(f1 == 1);
	}

	SECTION("Test min&max with triple double")
	{
		double d1, d2(-1), d3(1);
		d1 = eastl::min(d2, d3);
		REQUIRE(d1 == -1);
		d1 = eastl::min(d3, d2);
		REQUIRE(d1 == -1);
		d1 = eastl::max(d2, d3);
		REQUIRE(d1 == 1);
		d1 = eastl::max(d3, d2);
		REQUIRE(d1 == 1);
	}

	SECTION("Test min with void*")
	{
		double d2(-1), d3(1);
		void* p1, * p2 = &d2, * p3 = &d3;
		p1 = eastl::min(p2, p3);
		REQUIRE((uintptr_t)p1 == eastl::min((uintptr_t)p2, (uintptr_t)p3));
	}

	SECTION("Test min with double*")
	{
		double d2(-1), d3(1);
		double* pd1, * pd2 = &d2, * pd3 = &d3;
		pd1 = eastl::min(pd2, pd3);
		REQUIRE((uintptr_t)pd1 == eastl::min((uintptr_t)pd2, (uintptr_t)pd3));
	}

	SECTION("Test min&max with std::initializer_list<int>")
	{
		REQUIRE(eastl::min({ 3, 1, 2 }) == 1);
		REQUIRE(eastl::max({ 3, 1, 2 }) == 3);
	}

	SECTION("Test min")
	{
		SECTION("char: min")
		{
			REQUIRE(eastl::min((char)1, (char)1) == 1);
			REQUIRE(eastl::min((char)1, (char)2) == 1);
			REQUIRE(eastl::min((char)2, (char)1) == 1);
		}

		SECTION("signed char: min")
		{
			REQUIRE(eastl::min((signed char)1, (signed char)1) == 1);
			REQUIRE(eastl::min((signed char)1, (signed char)2) == 1);
			REQUIRE(eastl::min((signed char)2, (signed char)1) == 1);
		}

		SECTION("unsigned char: min")
		{
			REQUIRE(eastl::min((unsigned char)1, (unsigned char)1) == 1);
			REQUIRE(eastl::min((unsigned char)1, (unsigned char)2) == 1);
			REQUIRE(eastl::min((unsigned char)2, (unsigned char)1) == 1);
		}

		SECTION("signed short: min")
		{
			REQUIRE(eastl::min((signed short)1, (signed short)1) == 1);
			REQUIRE(eastl::min((signed short)1, (signed short)2) == 1);
			REQUIRE(eastl::min((signed short)2, (signed short)1) == 1);
		}

		SECTION("unsigned short: min")
		{
			REQUIRE(eastl::min((unsigned short)1, (unsigned short)1) == 1);
			REQUIRE(eastl::min((unsigned short)1, (unsigned short)2) == 1);
			REQUIRE(eastl::min((unsigned short)2, (unsigned short)1) == 1);
		}

		SECTION("signed int: min")
		{
			REQUIRE(eastl::min((signed int)1, (signed int)1) == 1);
			REQUIRE(eastl::min((signed int)1, (signed int)2) == 1);
			REQUIRE(eastl::min((signed int)2, (signed int)1) == 1);
		}

		SECTION("unsigned int: min")
		{
			REQUIRE(eastl::min((unsigned int)1, (unsigned int)1) == 1);
			REQUIRE(eastl::min((unsigned int)1, (unsigned int)2) == 1);
			REQUIRE(eastl::min((unsigned int)2, (unsigned int)1) == 1);
		}

		SECTION("signed long: min")
		{
			REQUIRE(eastl::min((signed long)1, (signed long)1) == 1);
			REQUIRE(eastl::min((signed long)1, (signed long)2) == 1);
			REQUIRE(eastl::min((signed long)2, (signed long)1) == 1);
		}

		SECTION("unsigned long: min")
		{
			REQUIRE(eastl::min((unsigned long)1, (unsigned long)1) == 1);
			REQUIRE(eastl::min((unsigned long)1, (unsigned long)2) == 1);
			REQUIRE(eastl::min((unsigned long)2, (unsigned long)1) == 1);
		}

		SECTION("signed long long: min")
		{
			REQUIRE(eastl::min((signed long long)1, (signed long long)1) == 1);
			REQUIRE(eastl::min((signed long long)1, (signed long long)2) == 1);
			REQUIRE(eastl::min((signed long long)2, (signed long long)1) == 1);
		}

		SECTION("unsigned long long: min")
		{
			REQUIRE(eastl::min((unsigned long long)1, (unsigned long long)1) == 1);
			REQUIRE(eastl::min((unsigned long long)1, (unsigned long long)2) == 1);
			REQUIRE(eastl::min((unsigned long long)2, (unsigned long long)1) == 1);
		}

		SECTION("float: min")
		{
			REQUIRE(eastl::min((float)1, (float)1) == 1);
			REQUIRE(eastl::min((float)1, (float)2) == 1);
			REQUIRE(eastl::min((float)2, (float)1) == 1);
		}

		SECTION("double: min")
		{
			REQUIRE(eastl::min((double)1, (double)1) == 1);
			REQUIRE(eastl::min((double)1, (double)2) == 1);
			REQUIRE(eastl::min((double)2, (double)1) == 1);
		}

		SECTION("long double: min")
		{
			REQUIRE(eastl::min((long double)1, (long double)1) == 1);
			REQUIRE(eastl::min((long double)1, (long double)2) == 1);
			REQUIRE(eastl::min((long double)2, (long double)1) == 1);
		}
	}

	// Test max specializations

	SECTION("Test max")
	{
		SECTION("char: max")
		{
			REQUIRE(max((char)1, (char)1) == 1);
			REQUIRE(max((char)1, (char)2) == 2);
			REQUIRE(max((char)2, (char)1) == 2);
		}

		SECTION("signed char: max")
		{
			REQUIRE(max((signed char)1, (signed char)1) == 1);
			REQUIRE(max((signed char)1, (signed char)2) == 2);
			REQUIRE(max((signed char)2, (signed char)1) == 2);
		}

		SECTION("unsigned char: max")
		{
			REQUIRE(max((unsigned char)1, (unsigned char)1) == 1);
			REQUIRE(max((unsigned char)1, (unsigned char)2) == 2);
			REQUIRE(max((unsigned char)2, (unsigned char)1) == 2);
		}

		SECTION("signed short: max")
		{
			REQUIRE(max((signed short)1, (signed short)1) == 1);
			REQUIRE(max((signed short)1, (signed short)2) == 2);
			REQUIRE(max((signed short)2, (signed short)1) == 2);
		}

		SECTION("unsigned short: max")
		{
			REQUIRE(max((unsigned short)1, (unsigned short)1) == 1);
			REQUIRE(max((unsigned short)1, (unsigned short)2) == 2);
			REQUIRE(max((unsigned short)2, (unsigned short)1) == 2);
		}

		SECTION("signed int: max")
		{
			REQUIRE(max((signed int)1, (signed int)1) == 1);
			REQUIRE(max((signed int)1, (signed int)2) == 2);
			REQUIRE(max((signed int)2, (signed int)1) == 2);
		}

		SECTION("unsigned int: max")
		{
			REQUIRE(max((unsigned int)1, (unsigned int)1) == 1);
			REQUIRE(max((unsigned int)1, (unsigned int)2) == 2);
			REQUIRE(max((unsigned int)2, (unsigned int)1) == 2);
		}

		SECTION("signed long: max")
		{
			REQUIRE(max((signed long)1, (signed long)1) == 1);
			REQUIRE(max((signed long)1, (signed long)2) == 2);
			REQUIRE(max((signed long)2, (signed long)1) == 2);
		}

		SECTION("unsigned long: max")
		{
			REQUIRE(max((unsigned long)1, (unsigned long)1) == 1);
			REQUIRE(max((unsigned long)1, (unsigned long)2) == 2);
			REQUIRE(max((unsigned long)2, (unsigned long)1) == 2);
		}

		SECTION("signed long long: max")
		{
			REQUIRE(max((signed long long)1, (signed long long)1) == 1);
			REQUIRE(max((signed long long)1, (signed long long)2) == 2);
			REQUIRE(max((signed long long)2, (signed long long)1) == 2);
		}

		SECTION("unsigned long long: max")
		{
			REQUIRE(max((unsigned long long)1, (unsigned long long)1) == 1);
			REQUIRE(max((unsigned long long)1, (unsigned long long)2) == 2);
			REQUIRE(max((unsigned long long)2, (unsigned long long)1) == 2);
		}

		SECTION("float: max")
		{
			REQUIRE(max((float)1, (float)1) == 1);
			REQUIRE(max((float)1, (float)2) == 2);
			REQUIRE(max((float)2, (float)1) == 2);
		}

		SECTION("double: max")
		{
			REQUIRE(max((double)1, (double)1) == 1);
			REQUIRE(max((double)1, (double)2) == 2);
			REQUIRE(max((double)2, (double)1) == 2);
		}

		SECTION("long double: max")
		{
			REQUIRE(max((long double)1, (long double)1) == 1);
			REQUIRE(max((long double)1, (long double)2) == 2);
			REQUIRE(max((long double)2, (long double)1) == 2);
		}
	}

	// Test min_alt specializations

	SECTION("Test min_alt")
	{
		SECTION("char: min_alt")
		{
			REQUIRE(min_alt((char)1, (char)1) == 1);
			REQUIRE(min_alt((char)1, (char)2) == 1);
			REQUIRE(min_alt((char)2, (char)1) == 1);
		}

		SECTION("signed char: min_alt")
		{
			REQUIRE(min_alt((signed char)1, (signed char)1) == 1);
			REQUIRE(min_alt((signed char)1, (signed char)2) == 1);
			REQUIRE(min_alt((signed char)2, (signed char)1) == 1);
		}

		SECTION("unsigned char: min_alt")
		{
			REQUIRE(min_alt((unsigned char)1, (unsigned char)1) == 1);
			REQUIRE(min_alt((unsigned char)1, (unsigned char)2) == 1);
			REQUIRE(min_alt((unsigned char)2, (unsigned char)1) == 1);
		}

		SECTION("signed int: min_alt")
		{
			REQUIRE(min_alt((signed int)1, (signed int)1) == 1);
			REQUIRE(min_alt((signed int)1, (signed int)2) == 1);
			REQUIRE(min_alt((signed int)2, (signed int)1) == 1);
		}

		SECTION("unsigned int: min_alt")
		{
			REQUIRE(min_alt((unsigned int)1, (unsigned int)1) == 1);
			REQUIRE(min_alt((unsigned int)1, (unsigned int)2) == 1);
			REQUIRE(min_alt((unsigned int)2, (unsigned int)1) == 1);
		}

		SECTION("signed long: min_alt")
		{
			REQUIRE(min_alt((signed long)1, (signed long)1) == 1);
			REQUIRE(min_alt((signed long)1, (signed long)2) == 1);
			REQUIRE(min_alt((signed long)2, (signed long)1) == 1);
		}

		SECTION("unsigned long: min_alt")
		{
			REQUIRE(min_alt((unsigned long)1, (unsigned long)1) == 1);
			REQUIRE(min_alt((unsigned long)1, (unsigned long)2) == 1);
			REQUIRE(min_alt((unsigned long)2, (unsigned long)1) == 1);
		}

		SECTION("signed long long: min_alt")
		{
			REQUIRE(min_alt((signed long long)1, (signed long long)1) == 1);
			REQUIRE(min_alt((signed long long)1, (signed long long)2) == 1);
			REQUIRE(min_alt((signed long long)2, (signed long long)1) == 1);
		}

		SECTION("unsigned long long: min_alt")
		{
			REQUIRE(min_alt((unsigned long long)1, (unsigned long long)1) == 1);
			REQUIRE(min_alt((unsigned long long)1, (unsigned long long)2) == 1);
			REQUIRE(min_alt((unsigned long long)2, (unsigned long long)1) == 1);
		}

		SECTION("float: min_alt")
		{
			REQUIRE(min_alt((float)1, (float)1) == 1);
			REQUIRE(min_alt((float)1, (float)2) == 1);
			REQUIRE(min_alt((float)2, (float)1) == 1);
		}

		SECTION("double: min_alt")
		{
			REQUIRE(min_alt((double)1, (double)1) == 1);
			REQUIRE(min_alt((double)1, (double)2) == 1);
			REQUIRE(min_alt((double)2, (double)1) == 1);
		}

		SECTION("long double: min_alt")
		{
			REQUIRE(min_alt((long double)1, (long double)1) == 1);
			REQUIRE(min_alt((long double)1, (long double)2) == 1);
			REQUIRE(min_alt((long double)2, (long double)1) == 1);
		}
	}

	// Test max_alt specializations

	SECTION("Test max_alt")
	{
		SECTION("char: max_alt")
		{
			REQUIRE(max_alt((char)1, (char)1) == 1);
			REQUIRE(max_alt((char)1, (char)2) == 2);
			REQUIRE(max_alt((char)2, (char)1) == 2);
		}

		SECTION("unsigned char: max_alt")
		{
			REQUIRE(max_alt((unsigned char)1, (unsigned char)1) == 1);
			REQUIRE(max_alt((unsigned char)1, (unsigned char)2) == 2);
			REQUIRE(max_alt((unsigned char)2, (unsigned char)1) == 2);
		}

		SECTION("signed short: max_alt")
		{
			REQUIRE(max_alt((signed short)1, (signed short)1) == 1);
			REQUIRE(max_alt((signed short)1, (signed short)2) == 2);
			REQUIRE(max_alt((signed short)2, (signed short)1) == 2);
		}

		SECTION("unsigned short: max_alt")
		{
			REQUIRE(max_alt((unsigned short)1, (unsigned short)1) == 1);
			REQUIRE(max_alt((unsigned short)1, (unsigned short)2) == 2);
			REQUIRE(max_alt((unsigned short)2, (unsigned short)1) == 2);
		}

		SECTION("signed int: max_alt")
		{
			REQUIRE(max_alt((signed int)1, (signed int)1) == 1);
			REQUIRE(max_alt((signed int)1, (signed int)2) == 2);
			REQUIRE(max_alt((signed int)2, (signed int)1) == 2);
		}

		SECTION("unsigned int: max_alt")
		{
			REQUIRE(max_alt((unsigned int)1, (unsigned int)1) == 1);
			REQUIRE(max_alt((unsigned int)1, (unsigned int)2) == 2);
			REQUIRE(max_alt((unsigned int)2, (unsigned int)1) == 2);
		}

		SECTION("signed long: max_alt")
		{
			REQUIRE(max_alt((signed long)1, (signed long)1) == 1);
			REQUIRE(max_alt((signed long)1, (signed long)2) == 2);
			REQUIRE(max_alt((signed long)2, (signed long)1) == 2);
		}

		SECTION("unsigned long: max_alt")
		{
			REQUIRE(max_alt((unsigned long)1, (unsigned long)1) == 1);
			REQUIRE(max_alt((unsigned long)1, (unsigned long)2) == 2);
			REQUIRE(max_alt((unsigned long)2, (unsigned long)1) == 2);
		}

		SECTION("signed long long: max_alt")
		{
			REQUIRE(max_alt((signed long long)1, (signed long long)1) == 1);
			REQUIRE(max_alt((signed long long)1, (signed long long)2) == 2);
			REQUIRE(max_alt((signed long long)2, (signed long long)1) == 2);
		}

		SECTION("unsigned long long: max_alt")
		{
			REQUIRE(max_alt((unsigned long long)1, (unsigned long long)1) == 1);
			REQUIRE(max_alt((unsigned long long)1, (unsigned long long)2) == 2);
			REQUIRE(max_alt((unsigned long long)2, (unsigned long long)1) == 2);
		}

		SECTION("float: max_alt")
		{
			REQUIRE(max_alt((float)1, (float)1) == 1);
			REQUIRE(max_alt((float)1, (float)2) == 2);
			REQUIRE(max_alt((float)2, (float)1) == 2);
		}

		SECTION("double: max_alt")
		{
			REQUIRE(max_alt((double)1, (double)1) == 1);
			REQUIRE(max_alt((double)1, (double)2) == 2);
			REQUIRE(max_alt((double)2, (double)1) == 2);
		}

		SECTION("long double: max_alt")
		{
			REQUIRE(max_alt((long double)1, (long double)1) == 1);
			REQUIRE(max_alt((long double)1, (long double)2) == 2);
			REQUIRE(max_alt((long double)2, (long double)1) == 2);
		}
	}

	SECTION("Test min_alt&max_alt with LessStruct", "[struct A]")
	{
		A a1 { 1 }, a2 { 2 }, a3 { 3 };
		a3 = min_alt(a1, a2, LessStruct());
		REQUIRE(a3.a == 1);
		a3 = max_alt(a1, a2, LessStruct());
		REQUIRE(a3.a == 2);
	}

	SECTION("Test min_alt&max_alt with boolean func LessStruct", "[struct B]")
	{
		B b1 { 1 }, b2 { 2 }, b3 { 3 };
		b3 = min_alt(b2, b1, LessFunction);
		REQUIRE(b3.b == 1);
		b3 = max_alt(b2, b1, LessFunction);
		REQUIRE(b3.b == 2);
	}

	SECTION("Test min_alt&max_alt with TestObject", "[class TestObject]")
	{
		TestObject t1 { 1 }, t2 { 2 }, t3 { 3 };
		t3 = min_alt(t2, t1);
		REQUIRE(t3.mX == 1);
		t3 = max_alt(t2, t1);
		REQUIRE(t3.mX == 2);
	}

	SECTION("Test min_alt&max_alt with triple int")
	{
		int32_t i1, i2 { -1 }, i3 { 1 };
		i1 = min_alt(i2, i3);
		REQUIRE(i1 == -1);
		i1 = min_alt(i3, i2);
		REQUIRE(i1 == -1);
		i1 = max_alt(i2, i3);
		REQUIRE(i1 == 1);
		i1 = max_alt(i3, i2);
		REQUIRE(i1 == 1);
	}

	SECTION("Test min_alt&max_alt with triple float")
	{
		float f1, f2 { -1 }, f3 { 1 };
		f1 = min_alt(f2, f3);
		REQUIRE(f1 == -1);
		f1 = min_alt(f3, f2);
		REQUIRE(f1 == -1);
		f1 = max_alt(f2, f3);
		REQUIRE(f1 == 1);
		f1 = max_alt(f3, f2);
		REQUIRE(f1 == 1);
	}

	SECTION("Test min_alt&max_alt with triple double")
	{
		double d1, d2 { -1 }, d3 { 1 };
		d1 = min_alt(d2, d3);
		REQUIRE(d1 == -1);
		d1 = min_alt(d3, d2);
		REQUIRE(d1 == -1);
		d1 = max_alt(d2, d3);
		REQUIRE(d1 == 1);
		d1 = max_alt(d3, d2);
		REQUIRE(d1 == 1);
	}

	SECTION("Test min with enum")
	{
		// Make sure enums work
		static_assert(eastl::is_enum<TestMinMaxEnum>::value, "is_enum failure");
		REQUIRE(eastl::min(teX, teY) == teX);
	}

	SECTION("Test min with array", "[TestObject]")
	{
		// Make sure pointers work
		TestObject testObjectArray[2];
		REQUIRE(eastl::min(&testObjectArray[0], &testObjectArray[1]) == &testObjectArray[0]);
	}

	SECTION("Regression test max_alt for Microsoft warning C4347")
	{
		// Regression for Microsoft warning C4347 (http://msdn.microsoft.com/en-us/library/x7wb5te0.aspx)
		int32_t value = 17;
		int32_t result = eastl::max_alt<int32_t>(0, value);
		REQUIRE(result == 17);
	}

	SECTION("Regression test min_alt for Microsoft error C2666")
	{
		// Regression for Microsoft error C2666 (http://msdn.microsoft.com/en-us/library/dyafzty4%28v=vs.110%29.aspx)
		uint32_t value2a = 17;
		uint32_t value2b = 2;
		uint32_t result2 = eastl::min_alt<uint32_t>(value2a - value2b, 4); // error C2666: 'eastl::min_alt' : 12 overloads have similar conversions
		REQUIRE(result2 == 4);
	}

	SECTION("Regression test min_alt for volatile arguments + literals")
	{
		// Regression for volatile arguments + literals
		// This test is disabled until we come up with a solution for this. std::min gives the same result as below, so we aren't necessarily obligated to resolve this.
		// volatile uint32_t value3  = 17;
		// uint32_t result3 = eastl::min_alt<uint32_t>(value3, 4); // error C2664: 'const T &eastl::min_alt<unsigned int>(const T &,const T &)' : cannot convert parameter 1 from 'volatile uint32_t' to 'const unsigned int &'
		// EATEST_VERIFY(result3 == 4);
	}

	// ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
	// ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare compare)

	SECTION("Test min_element")
	{
		int intArray[] = { -5, 2, 1, 5, 4, 5 };
		TestObject toArray[] = { TestObject(7), TestObject(2), TestObject(8), TestObject(5), TestObject(4), TestObject(-12) };

		SECTION("min_element for array", "[int*]")
		{
			auto pInt = min_element(intArray, intArray + 6);
			REQUIRE((pInt && (*pInt == -5)) != false);
		}

		SECTION("min_element for array with Greater comparator", "[int*]")
		{
			auto pInt = min_element(intArray, intArray + 6, Greater<int>());
			REQUIRE((pInt && (*pInt == 5)) != false);
		}

		SECTION("min_element for array", "[TestObject*]")
		{
			auto pTO = min_element(toArray, toArray + 6);
			REQUIRE((pTO && (*pTO == TestObject(-12))) != false);
		}

		SECTION("min_element for array with Greater comparator", "[TestObject*]")
		{
			auto pTO = min_element(toArray, toArray + 6, Greater<TestObject>());
			REQUIRE((pTO && (*pTO == TestObject(8))) != false);
		}
	}

	// ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
	// ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare compare)

	SECTION("Test max_element")
	{
		int intArray[] = { -5, 2, 1, 5, 4, 5 };
		TestObject toArray[] = { TestObject(7), TestObject(2), TestObject(8), TestObject(5), TestObject(4), TestObject(-12) };

		SECTION("max_element for array", "[int*]")
		{
			auto pInt = max_element(intArray, intArray + 6);
			REQUIRE((pInt && (*pInt == 5)) != false);
		}

		SECTION("max_element for array with less comparator", "[int*]")
		{
			auto pInt = max_element(intArray, intArray + 6, less<int>());
			REQUIRE((pInt && (*pInt == 5)) != false);
		}

		SECTION("max_element for array", "[TestObject*]")
		{
			auto pTO = max_element(toArray, toArray + 6);
			REQUIRE((pTO && (*pTO == TestObject(8))) != false);
		}

		SECTION("max_element for array with Greater comparator", "[TestObject*]")
		{
			auto pTO = max_element(toArray, toArray + 6, less<TestObject>());
			REQUIRE((pTO && (*pTO == TestObject(8))) != false);
		}
	}

	// template <class ForwardIterator, class Compare>
	// eastl::pair<ForwardIterator, ForwardIterator> 
	// minmax_element(ForwardIterator first, ForwardIterator last)
	//
	// template <class ForwardIterator, class Compare>
	// eastl::pair<ForwardIterator, ForwardIterator> 
	// minmax_element(ForwardIterator first, ForwardIterator last, Compare compare)

	SECTION("Test minmax_element")
	{
		int intArray[] = { 5, -2, 1, 5, 6, 5 };

		SECTION("minmax_element for array", "[int*]")
		{
			eastl::pair<int*, int*> result = eastl::minmax_element(intArray, intArray + 6);
			REQUIRE(((*result.first == -2) && (*result.second == 6)) != false);
		}
	}

	// template <typename T> 
	// eastl::pair<const T&, const T&>
	// minmax(const T& a, const T& b)
	//
	// template <typename T, typename Compare> 
	// eastl::pair<const T&, const T&>
	// minmax(const T& a, const T& b, Compare comp)
	//
	// template <typename T>
	// eastl::pair<T, T>
	// minmax(std::initializer_list<T> ilist)
	//
	// template <typename T, class Compare>
	// eastl::pair<T, T>
	// minmax(std::initializer_list<T> ilist, Compare compare)

	SECTION("Test minmax")
	{
		// The VC++ compiler is broken in such a way that it can't compile the following without generating a warning: 
		//     warning C4413: 'eastl::pair<T1,T2>::first' : reference member is initialized to a temporary that doesn't persist after the constructor exits.
		// The Microsoft standard library definition of minmax doesn't generate this warning... because that minmax is broken and non-conforming. I think they
		// made it the way they did because of the aforementioned compiler bug.
		// Recent versions of clang seem to generate a warning of its own. To do: we need to address this.
		// GCC 4.8 for x86 has a compiler bug in optimized builds for this code, so we currently enable this for non-optimized builds only.

#if defined(EA_COMPILER_CPP11_ENABLED) && ((defined(EA_COMPILER_CLANG) && EA_COMPILER_VERSION < 302) || (defined(EA_COMPILER_GNUC) && (EA_COMPILER_VERSION >= 4007)) && !defined(__OPTIMIZE__))

		SECTION("int: minmax")
		{
			int i3 { 3 }, i2 { 2 };
			eastl::pair<const int&, const int&> resulti = eastl::minmax(i3, i2);
			EATEST_VERIFY_F((resulti.first == 2) && (resulti.second == 3), "minmax failure. %d %d", resulti.first, resulti.second);
		}

		SECTION("char: minmax")
		{
			char c3 { 3 }, c2 { 2 };
			eastl::pair<const char&, const char&> resultc = eastl::minmax(c3, c2);
			EATEST_VERIFY_F((resultc.first == 2) && (resultc.second == 3), "minmax failure. %d %d", (int)resultc.first, (int)resultc.second);
		}

		SECTION("float: minmax")
		{
			float f3 { 3.f }, f2 { 2.f };
			eastl::pair<const float&, const float&> resultf = eastl::minmax(f3, f2);
			EATEST_VERIFY_F((resultf.first == 2) && (resultf.second == 3), "minmax failure. %f %f", resultf.first, resultf.second);
		}

#endif // defined(EA_COMPILER_CPP11_ENABLED) && ((defined(EA_COMPILER_CLANG) && EA_COMPILER_VERSION < 302) || (defined(EA_COMPILER_GNUC) && (EA_COMPILER_VERSION >= 4007)) && !defined(__OPTIMIZE__))

		SECTION("initializer_list<int>: minmax")
		{
			eastl::pair<int, int> result3 = eastl::minmax({ 3, 2 });
			REQUIRE(((result3.first == 2) && (result3.second == 3)) != false);
		}
	}

	SECTION("Test clamp on different types")
	{
		REQUIRE(eastl::clamp(42, 1, 100) == 42);
		REQUIRE(eastl::clamp(-42, 1, 100) == 1);
		REQUIRE(eastl::clamp(420, 1, 100) == 100);
		REQUIRE(eastl::clamp(1, 1, 100) == 1);
		REQUIRE(eastl::clamp(100, 1, 100) == 100);

		REQUIRE(eastl::clamp(42.f, 1.f, 100.f, less<float>()) == 42.f);
		REQUIRE(eastl::clamp(-42.f, 1.f, 100.f, less<float>()) == 1.f);
		REQUIRE(eastl::clamp(420.f, 1.f, 100.f, less<float>()) == 100.f);
		REQUIRE(eastl::clamp(1.f, 1.f, 100.f, less<float>()) == 1.f);
		REQUIRE(eastl::clamp(100.f, 1.f, 100.f, less<float>()) == 100.f);

		REQUIRE(eastl::clamp(42., 1., 100., less<double>()) == 42.);
		REQUIRE(eastl::clamp(-42., 1., 100., less<double>()) == 1.);
		REQUIRE(eastl::clamp(420., 1., 100., less<double>()) == 100.);
		REQUIRE(eastl::clamp(1., 1., 100., less<double>()) == 1.);
		REQUIRE(eastl::clamp(100., 1., 100., less<double>()) == 100.);

		REQUIRE(eastl::clamp(A(42), A(1), A(100), LessStruct()).a == A(42).a);
		REQUIRE(eastl::clamp(A(-42), A(1), A(100), LessStruct()).a == A(1).a);
		REQUIRE(eastl::clamp(A(420), A(1), A(100), LessStruct()).a == A(100).a);
		REQUIRE(eastl::clamp(A(1), A(1), A(100), LessStruct()).a == A(1).a);
		REQUIRE(eastl::clamp(A(100), A(1), A(100), LessStruct()).a == A(100).a);
	}

	SECTION("Test all/any/none_of", "[vector<int>]")
	{
		int v[] { 2, 4, 6, 8 };

		SECTION("all_of test")
		{
			REQUIRE(eastl::all_of(eastl::begin(v), eastl::end(v), DivisibleBy(2)));
		}

		SECTION("any_of test")
		{
			REQUIRE(eastl::any_of(eastl::begin(v), eastl::end(v), DivisibleBy(3)));
		}

		SECTION("none_of test")
		{
			REQUIRE(eastl::none_of(eastl::begin(v), eastl::end(v), DivisibleBy(5)));
		}
	}

	// pair mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	// pair mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Predicate predicate)

	SECTION("Test mismatch algorithm")
	{
		int intArray1[] = { -5, 2, 1, 5, 4, 8888 };
		int intArray2[] = { -5, 2, 1, 5, 4, 9999 };
		int intArray3[] = { -5, 2, 1, 5, 4, 9999 };

		SECTION("mismatch: round 1")
		{
			eastl::pair<int*, int*> pairInt = mismatch(intArray1, intArray1, intArray2);
			REQUIRE(pairInt.first == intArray1 + 0);
			REQUIRE(pairInt.second == intArray2 + 0);
		}

		SECTION("mismatch: round 2")
		{
			eastl::pair<int*, int*> pairInt = mismatch(intArray1, intArray1 + 6, intArray2);
			REQUIRE(pairInt.first == intArray1 + 5);
			REQUIRE(pairInt.second == intArray2 + 5);
		}

		SECTION("mismatch: round 3")
		{
			eastl::pair<int*, int*> pairInt = mismatch(intArray2, intArray2 + 6, intArray3);
			REQUIRE(pairInt.first == intArray2 + 6);
			REQUIRE(pairInt.second == intArray3 + 6);
		}

		SECTION("mismatch: round 4")
		{
			eastl::pair<int*, int*> pairInt = mismatch(intArray1, intArray1, intArray2, equal_to<int>());
			REQUIRE(pairInt.first == intArray1 + 0);
			REQUIRE(pairInt.second == intArray2 + 0);
		}

		SECTION("mismatch: round 5")
		{
			eastl::pair<int*, int*> pairInt = mismatch(intArray1, intArray1 + 6, intArray2, equal_to<int>());
			REQUIRE(pairInt.first == intArray1 + 5);
			REQUIRE(pairInt.second == intArray2 + 5);
		}

		SECTION("mismatch: round 6")
		{
			eastl::pair<int*, int*> pairInt = mismatch(intArray2, intArray2 + 6, intArray3, equal_to<int>());
			REQUIRE(pairInt.first == intArray2 + 6);
			REQUIRE(pairInt.second == intArray3 + 6);
		}
	}

	// void swap(T& a, T& b)
	// void iter_swap(ForwardIterator1 a, ForwardIterator2 b)

	SECTION("Test: swap")
	{
		SECTION("swap on array", "[int*]")
		{
			int intArray[] = { -5, 2, 1, 5, 4, 5 };
			swap(intArray[0], intArray[4]);
			REQUIRE(VerifySequence(intArray, intArray + 6, int(), "swap", 4, 2, 1, 5, -5, 5, -1));
		}

		SECTION("iter_swap on array", "[int*]")
		{
			int intArray[] = { -5, 2, 1, 5, 4, 5 };
			iter_swap(intArray + 2, intArray + 3);
			REQUIRE(VerifySequence(intArray, intArray + 6, int(), "iter_swap", -5, 2, 5, 1, 4, 5, -1));
		}

		SECTION("swap on array", "[TestObject*]")
		{
			TestObject toArray[] =
			{
				TestObject { -5 },
				TestObject { 2 },
				TestObject { 1 },
				TestObject { 5 },
				TestObject { 4 },
				TestObject { 5 }
			};

			swap(toArray[0], toArray[4]);
			REQUIRE(toArray[0] == TestObject(4));
			REQUIRE(toArray[4] == TestObject(-5));
		}

		SECTION("iter_swap on array", "[TestObject*]")
		{
			TestObject toArray[] = 
			{ 
				TestObject { -5 },
				TestObject { 2 },
				TestObject { 1 },
				TestObject { 5 },
				TestObject { 4 },
				TestObject { 5 }
			};

			iter_swap(toArray + 2, toArray + 3);
			REQUIRE(toArray[2] == TestObject(5));
			REQUIRE(toArray[3] == TestObject(1));
		}
	}

	// ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)

	SECTION("Test: swap_ranges")
	{
		SECTION("swap_ranged on arrays", "[int*]")
		{
			int intArray1[] = { 3, 2, 6, 5, 4, 1 };
			int intArray2[] = { 0, 0, 0, 0, 0, 0 };

			swap_ranges(intArray1, intArray1 + 6, intArray2);
			REQUIRE(VerifySequence(intArray1, intArray1 + 6, int(), "swap_ranges", 0, 0, 0, 0, 0, 0, -1));
			REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "swap_ranges", 3, 2, 6, 5, 4, 1, -1));
		}

		SECTION("swap_ranges on arrays", "[TestObject*]")
		{
			TestObject toArray1[] =
			{
				TestObject { 3 },
				TestObject { 2 },
				TestObject { 6 },
				TestObject { 5 },
				TestObject { 4 },
				TestObject { 1 }
			};

			TestObject toArray2[] =
			{
				TestObject { 0 },
				TestObject { 0 },
				TestObject { 0 },
				TestObject { 0 },
				TestObject { 0 },
				TestObject { 0 }
			};

			swap_ranges(toArray1, toArray1 + 6, toArray2);
			REQUIRE(toArray1[0] == TestObject { 0 });
			REQUIRE(toArray1[5] == TestObject { 0 });
			REQUIRE(toArray2[0] == TestObject { 3 });
			REQUIRE(toArray2[5] == TestObject { 1 });
		}
	}

	// ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
	// ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate predicate)

	SECTION("Test: adjacent_find")
	{
		int intArray[] = { 3, 2, 5, 5, 4, 1 };
		TestObject toArray[] = 
		{ 
			TestObject { 3 },
			TestObject { 2 },
			TestObject { 5 },
			TestObject { 5 },
			TestObject { 4 },
			TestObject { 1 }
		};

		SECTION("adjacent_find on array", "[int*]")
		{
			auto pInt = adjacent_find(intArray + 0, intArray + 6);
			REQUIRE((pInt == (intArray + 2)) != false);
		}

		SECTION("adjacent_find on array (verify not found)", "[int*]")
		{
			auto pInt = adjacent_find(intArray + 3, intArray + 6);
			REQUIRE((pInt == (intArray + 6)) != false);
		}

		SECTION("adjacent_find on array", "[TestObject*]")
		{
			auto pTO = adjacent_find(toArray + 0, toArray + 6);
			REQUIRE(pTO == (toArray + 2));
		}

		SECTION("adjacent_find on array (verify not found)", "[TestObject*]")
		{
			auto pTO = adjacent_find(toArray + 3, toArray + 6);
			REQUIRE(pTO == (toArray + 6));
		}
	}

	// OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)

	SECTION("Test: move")
	{
		int intArray1[] = { 3, 2, 6, 5, 4, 1 };

		SECTION("move to other array (only the last one)", "[int*]")
		{
			int intArray2[] = { 0, 0, 0, 0, 0, 0 };
			move(intArray1, intArray1 + 0, intArray2);
			REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "move", 0, 0, 0, 0, 0, 0, -1));
		}

		SECTION("move to other array (all)", "[int*]")
		{
			int intArray2[] = { 0, 0, 0, 0, 0, 0 };
			move(intArray1, intArray1 + 6, intArray2);
			REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "move", 3, 2, 6, 5, 4, 1, -1));
		}

		SECTION("move to same array (copy over self)", "[int*]")
		{
			int intArraySame[] = { 3, 2, 6, 5, 4, 1 };
			move(intArraySame + 1, intArraySame + 6, intArraySame + 0); // Copy over self.
			REQUIRE(VerifySequence(intArraySame, intArraySame + 6, int(), "move", 2, 6, 5, 4, 1, 1, -1));
		}
	}

	// OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)

	SECTION("Test: copy")
	{
		int intArray1[] = { 3, 2, 6, 5, 4, 1 };

		SECTION("copy to other array (only the last one)", "[int*]")
		{
			int intArray2[] = { 0, 0, 0, 0, 0, 0 };
			copy(intArray1, intArray1 + 0, intArray2);
			REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "copy", 0, 0, 0, 0, 0, 0, -1));
		}

		SECTION("copy to other array (all)", "[int*]")
		{
			int intArray2[] = { 0, 0, 0, 0, 0, 0 };
			copy(intArray1, intArray1 + 6, intArray2);
			REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "copy", 3, 2, 6, 5, 4, 1, -1));
		}

		SECTION("copy to same array (copy over self)", "[int*]")
		{
			int intArraySame[] = { 3, 2, 6, 5, 4, 1 };
			copy(intArraySame + 1, intArraySame + 6, intArraySame + 0); // Copy over self.
			REQUIRE(VerifySequence(intArraySame, intArraySame + 6, int(), "copy", 2, 6, 5, 4, 1, 1, -1));
		}
	}

	// OutputIterator copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate predicate)

	SECTION("Test: copy_if")
	{
		int intArray1[] = { 9, 1, 9, 9, 9, 9, 1, 1, 9, 9 };

		SECTION("copy_if to array (one element)", "[int*]")
		{
			int intArray2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			copy_if(intArray1, intArray1 + 0, intArray2, bind2nd(equal_to<int>(), (int)1));
			REQUIRE(VerifySequence(intArray2, intArray2 + 10, int(), "copy_if", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1));
		}

        SECTION("copy_if to array (all elements)", "[int*]")
        {
            int intArray2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            copy_if(intArray1, intArray1 + 9, intArray2, bind2nd(equal_to<int>(), (int)1));
            REQUIRE(VerifySequence(intArray2, intArray2 + 10, int(), "copy_if", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, -1));
        }

        SECTION("copy_if to same array", "[int*]")
        {
            copy_if(intArray1 + 1, intArray1 + 9, intArray1 + 0, bind2nd(equal_to<int>(), (int)1)); // Copy over self.
            REQUIRE(VerifySequence(intArray1, intArray1 + 10, int(), "copy_if", 1, 1, 1, 9, 9, 9, 1, 1, 9, 9, -1));
        }
	}

    // OutputIterator copy_n(InputIterator first, Size count, OutputIterator result)

    SECTION("Test: copy_n")
    {
        const char in[] = "123456";
        
        SECTION("copy_n to other array (4 symbols)", "[char*]")
        {
            char out[5] = { 0 };
            const char test_in[] = { '1', '2', '3', '4', '\0' };
            eastl::copy_n(eastl::begin(in), 4, char_back_inserter<sizeof(out)>(out));
            REQUIRE(!strcmp(test_in, out));
        }

        SECTION("copy_n to other array (all symbols)", "[char*]")
        {
            char out[sizeof(in)] = { 0 };
            eastl::copy_n(eastl::begin(in), eastl::CharStrlen(in), char_back_inserter<sizeof(out)>(out));
            REQUIRE(!strcmp(in, out));
        }
    }

    // BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)

    SECTION("Test: copy_backward")
    {
        int intArray1[] = { 3, 2, 6, 5, 4, 1 };

        SECTION("copy_backward to array (only one)", "[int*]")
        {
            int intArray2[] = { 0, 0, 0, 0, 0, 0 };
            copy_backward(intArray1, intArray1 + 0, intArray2 + 0);
            REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "copy_backward", 0, 0, 0, 0, 0, 0, -1));
        }

        SECTION("copy_backward to array (all elements)", "[int*]")
        {
            int intArray2[] = { 0, 0, 0, 0, 0, 0 };
            copy_backward(intArray1, intArray1 + 6, intArray2 + 6);
            REQUIRE(VerifySequence(intArray2, intArray2 + 6, int(), "copy_backward", 3, 2, 6, 5, 4, 1, -1));
        }

        SECTION("copy_backward to same array (copy over self)", "[int*]")
        {
            copy_backward(intArray1, intArray1 + 5, intArray1 + 6); // 
            REQUIRE(VerifySequence(intArray1, intArray1 + 6, int(), "copy_backward", 3, 3, 2, 6, 5, 4, -1));
        }
    }
}
