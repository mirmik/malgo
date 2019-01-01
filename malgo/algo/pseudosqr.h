#ifndef MALGO_PSEUDOSQR_H
#define MALGO_PSEUDOSQR_H

#include <malgo/util.h>

namespace malgo 
{
	template <class A, class E> 
	void pseudosqr(const mroot<A>& a, eroot<E>& e) 
	{
		assert(a.size1() == e.dim());

		type_t<E>* it = e.data();
		for(int i = 0; i < a.size1(); ++i) { 
			for(int j = i; j < a.size1(); ++j) 
			{
				*it++ = dot(a[i], a[j]);
			}
		}
	}
}

#endif