#ifndef MALGO_INVERSE_H
#define MALGO_INVERSE_H

//gaussj from numerical recipes

#include <malgo/util.h>

namespace malgo
{
	template <class A, class B> void 		inverse(const mroot<A>& c, mroot<A>& b);
	template <class A> 			matrix_t<A> inverse(const mroot<A>& c, int* singular);
}

#include <malgo/matrix.h>

namespace malgo
{
	template <class A, class C> int inverse(const mroot<A>& a, mroot<C>& c)
	{
		int n = c.size1();
		int pivrow;		// keeps track of current pivot row
		int k, i, j;	// k: overall index along diagonal; i: row index; j: col index
		int pivrows[n]; // keeps track of rows swaps to undo at end
		type_t<A> tmp;	// used for finding max value and making column swaps

		assert(c.size1() == c.size2());
		assert(a.size1() == a.size2());
		assert(c.size1() == a.size1());

		for (i = 0 ; i < n; i++)
			for (j = 0 ; j < n; j++)
				c(i,j) = a(i,j);

		for (k = 0; k < n; k++)
		{
			// find pivot row, the row with biggest entry in current column
			tmp = 0;
			for (i = k; i < n; i++)
			{
				if (malgo::abs(c(i,k)) >= tmp)	// 'Avoid using other functions inside abs()?'
				{
					tmp = abs(c(i,k));
					pivrow = i;
				}
			}

			// check for singular matrix
			if (c(pivrow,k) == 0.0f)
			{
				return -1;
			}

			// Execute pivot (row swap) if needed
			if (pivrow != k)
			{
				// swap row k with pivrow
				for (j = 0; j < n; j++)
				{
					tmp = c(k,j);
					c(k,j) = c(pivrow,j);
					c(pivrow,j) = tmp;
				}
			}
			pivrows[k] = pivrow;	// record row swap (even if no swap happened)

			tmp = 1.0f / c(k,k);	// invert pivot element
			c(k,k) = 1.0f;		// This element of input matrix becomes result matrix

			// Perform row reduction (divide every element by pivot)
			for (j = 0; j < n; j++)
			{
				c(k,j) = c(k,j) * tmp;
			}

			// Now eliminate all other entries in this column
			for (i = 0; i < n; i++)
			{
				if (i != k)
				{
					tmp = c(i,k);
					c(i,k) = 0.0f; // The other place where in matrix becomes result mat
					for (j = 0; j < n; j++)
					{
						c(i,j) = c(i,j) - c(k,j) * tmp;
					}
				}
			}
		}

		// Done, now need to undo pivot row swaps by doing column swaps in reverse order
		for (k = n - 1; k >= 0; k--)
		{
			if (pivrows[k] != k)
			{
				for (i = 0; i < n; i++)
				{
					tmp = c(i,k);
					c(i,k) = c(i,pivrows[k]);
					c(i,pivrows[k]) = tmp;
				}
			}
		}
		return 0;
	}

	template<typename V>
	matrix_t<V> inverse(mroot<V>& c, int* is_singular = nullptr)
	{
		assert(c.size1() == c.size2());
		
		matrix_t<V> b(c.size1(), c.size1());

		int error = inverse(c, b);
		if (is_singular) 
			*is_singular = error;
		
		return b;
	}
}

#endif