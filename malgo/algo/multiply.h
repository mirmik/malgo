#ifndef MALGO_ALGO_MULTIPLY_H
#define MALGO_ALGO_MULTIPLY_H

#include <malgo/util.h>
#include <nos/trace.h>
<<<<<<< HEAD
#include <igris/dprint.h>
=======
//#include <nos/dprint.h>
>>>>>>> 241965d2f3adc3334a8b896e29af03de85042cc6
#include <nos/print.h>

namespace malgo
{
	template<class A, class B, class C> void multiply(const vroot<A>& a, const B& b, vroot<C>& c);
	template<class A, class B, class C> void multiply(const mroot<A>& a, const vroot<B>& b, vroot<C>& c);
	template<class A, class B, class C> void multiply(const mroot<A>& a, const mroot<B>& b, mroot<C>& c);
	template<class A, class B>			auto multiply(const mroot<A>& a, const mroot<B>& b) -> matrix<decltype(type_t<A>() * type_t<B>()), alloc_t<A>>;
}

#include <malgo/vector.h>
#include <malgo/matrix.h>

#include <assert.h>

namespace malgo
{
	template<class A, class B, class C>
	void multiply(const vroot<A>& a, const B& b, vroot<C>& c)
	{
		assert(a.size() == c.size());
		for (int i = 0; i < a.size(); ++i) c[i] = a[i] * b;
	}

	template<class A, class B, class C>
	void accumulate_multiply(const vroot<A>& a, const B& b, vroot<C>& c)
	{
		assert(a.size() == c.size());
		for (int i = 0; i < a.size(); ++i) c[i] += a[i] * b;
	}

	template<class A, class B, class C>
	void multiply(const mroot<A>& a, const vroot<B>& b, vroot<C>& c)
	{
		assert(a.size1() == b.size());
		for (int i = 0; i < a.size1(); ++i) accumulate_multiply(a[i], b[i], c);
	}

	template<class A, class B, class C>
	void multiply(const mroot<A>& a, const mroot<B>& b, mroot<C>& c)
	{
		assert(b.size1() == c.size1());
		for (int i = 0; i < b.size1(); ++i) multiply(a, b[i], malgo::force(c[i]));
	}

	template <class A, class B>
	auto multiply(const mroot<A>& a, const mroot<B>& b) -> matrix<decltype(type_t<A>() * type_t<B>()), alloc_t<A>>
	{
		matrix<decltype(type_t<A>() * type_t<B>()), alloc_t<A>> c(b.size1(), a.size2());
		multiply(a, b, c);
		return c;
	}
}

#endif