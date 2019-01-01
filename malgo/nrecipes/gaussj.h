#ifndef MALGO_GAUSSJ_H
#define MALGO_GAUSSJ_H

#include <malgo/matrix.h>

namespace malgo
{
	template <typename A, typename B>
	int gaussj(mroot<A>& a, mroot<B>& b)
	{
		int 				i, icol, irow, j, k, l, ll, n = a.size1(), m = b.size2();
		type_t<A> 			big, dum, pivinv;
		vector_t<A> 		indxc(n), indxr(n), ipiv(n);
		
		for (j = 0; j < n; j++) ipiv[j] = 0;
		for (i = 0; i < n; i++)
		{
			big = 0.0;
			for (j = 0; j < n; j++)
				if (ipiv[j] != 1)
					for (k = 0; k < n; k++)
					{
						if (ipiv[k] == 0)
						{
							if (abs(a(j,k)) >= big)
							{
								big = malgo::abs(a(j,k));
								irow = j;
								icol = k;
							}
						}
					}
			++(ipiv[icol]);
			if (irow != icol)
			{
				for (l = 0; l < n; l++) malgo::swap(a(irow,l), a(icol,l));
				for (l = 0; l < m; l++) malgo::swap(b(irow,l), b(icol,l));
			}
			indxr[i] = irow;
			indxc[i] = icol;
			if (a(icol,icol) == 0.0) {
				//throw("gaussj: Singular Matrix";
				return -1;
			}

			pivinv = 1.0 / a(icol,icol);
			a(icol,icol) = 1.0;
			for (l = 0; l < n; l++) a(icol,l) *= pivinv;
			for (l = 0; l < m; l++) b(icol,l) *= pivinv;
			for (ll = 0; ll < n; ll++)
				if (ll != icol)
				{
					dum = a(ll,icol);
					a(ll,icol) = 0.0;
					for (l = 0; l < n; l++) a(ll,l) -= a(icol,l) * dum;
					for (l = 0; l < m; l++) b(ll,l) -= b(icol,l) * dum;
				}
		}
		for (l = n - 1; l >= 0; l--)
		{
			if (indxr[l] != indxc[l])
				for (k = 0; k < n; k++)
					malgo::swap(a(k,indxr[l]), a(k,indxc[l]));
		}

		return 0;
	}

	template<typename V>
	int gaussj(mroot<V>& a)
	{
		matrix_t<V> b(a.size1(), a.size1());
		return gaussj(a, b);
	}

}

#endif