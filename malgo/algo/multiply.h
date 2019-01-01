#ifndef MALGO_ALGO_MULTIPLY_H
#define MALGO_ALGO_MULTIPLY_H

#include <malgo/util.h>
#include <nos/trace.h>
#include <nos/dprint.h>
#include <nos/print.h>

namespace malgo
{
	template<class A, class B, class C> void multiply(const vector_root<A>& a, const B& b, vector_root<C>& c);
	template<class A, class B, class C> void multiply(const matrix_root<A>& a, const vector_root<B>& b, vector_root<C>& c);
	template<class A, class B, class C> void multiply(const matrix_root<A>& a, const matrix_root<B>& b, matrix_root<C>& c);
	template<class A, class B>			auto multiply(const matrix_root<A>& a, const matrix_root<B>& b) -> matrix<decltype(type_t<A>() * type_t<B>()), alloc_t<A>>;
}

#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <malgo/algo/elementwise.h>

#include <assert.h>

namespace malgo
{
	template<class A, class B, class C>
	void multiply(const vector_root<A>& a, const B& b, vector_root<C>& c)
	{
		assert(a.size() == c.size());

		for (int i = 0; i < a.size(); ++i) c[i] = a[i] * b;
	}

	template<class A, class B, class C>
	void accumulate_multiply(const vector_root<A>& a, const B& b, vector_root<C>& c)
	{
		assert(a.size() == c.size());

		for (int i = 0; i < a.size(); ++i) c[i] += a[i] * b;
	}

	template<class A, class B, class C>
	void multiply(const matrix_root<A>& a, const vector_root<B>& b, vector_root<C>& c)
	{
		assert(a.size1() == b.size());

		for (int i = 0; i < a.size1(); ++i) accumulate_multiply(a[i], b[i], c);
	}
	template<class A, class B, class C>
	void multiply(const matrix_root<A>& a, const matrix_root<B>& b, matrix_root<C>& c)
	{
		assert(b.size1() == c.size1());

		for (int i = 0; i < b.size1(); ++i)
		{
			multiply(a, b[i], malgo::force(c[i]));
		}
	}

	template <class A, class B>
	auto multiply(const matrix_root<A>& a, const matrix_root<B>& b) -> matrix<decltype(type_t<A>() * type_t<B>()), alloc_t<A>>
	{
		matrix<decltype(type_t<A>() * type_t<B>()), alloc_t<A>> c(b.size1(), a.size2());
		multiply(a, b, c);
		return c;
	}
}

#endif