#ifndef MALGO_BASEALGO_H
#define MALGO_BASEALGO_H

#include <malgo/util.h>

namespace malgo
{
	namespace raw
	{
		template<class F, class A, class B, class C> 	void elementwise3(F&& f, int size, A a, B b, C c) 	{ while (size--) *c++ = f(*a++, *b++); }

		template<class A, class B, class C> 	void elwise_add(int size, A a, B b, C c) 	{ elementwise3(detail::op_add{}, size, a, b, c); }
		template<class A, class B, class C> 	void elwise_sub(int size, A a, B b, C c) 	{ elementwise3(detail::op_sub{}, size, a, b, c); }
		template<class A, class B, class C> 	void elwise_mul(int size, A a, B b, C c) 	{ elementwise3(detail::op_mul{}, size, a, b, c); }
		template<class A, class B, class C> 	void elwise_div(int size, A a, B b, C c) 	{ elementwise3(detail::op_div{}, size, a, b, c); }

		template <class A, class B> void transpose( A a, int m, int n, B b)
		{
			int i, j;
			for (i = 0; i < m; i++)
				for (j = 0; j < n; j++)
					*(b + m * j + i) = *a++;
		}
	}
}

#endif