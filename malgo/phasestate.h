#ifndef MALGO_PHASE_STATE_H
#define MALGO_PHASE_STATE_H

#include <malgo/matrix.h>
#include <malgo/algo/multiply.h>

namespace malgo 
{
	template <class T>
	struct transfer_function 
	{
		template <class M> using mrr = const malgo::mroot<M>&;
		
		malgo::matrix<T> a, b, c, d;

		template <class A, class B, class C, class D> 
		transfer_function(mrr<A> _a, mrr<B> _b, mrr<C> _c, mrr<D> _d, double step) 
			: a(_a), b(_b), c(_c), d(_d) 
		{ 
			auto g_dim = b.size1();
			auto x_dim = b.size2();
			auto y_dim = c.size2();
		
			assert(a.size1() == x_dim);
			assert(a.size2() == x_dim);
			assert(b.size1() == g_dim);
			assert(b.size2() == x_dim);
			assert(c.size1() == x_dim);
			assert(c.size2() == y_dim);
			assert(d.size1() == g_dim);
			assert(d.size2() == y_dim);

			discretization(step);
		}

		void discretization(double step) 
		{
			auto ide_a = identity<T>(a.size1());
			auto inv_a = inverse(a);

			a = exponent(a * step);
			b = multiply(inv_a, multiply(ide_a - a, b));
		}
	};
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const malgo::transfer_function<T>& w) 
{
	os << "a: " << w.a << std::endl;
	os << "b: " << w.b << std::endl;
	os << "c: " << w.c << std::endl;
	os << "d: " << w.d << std::endl;
}

#endif