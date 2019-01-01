#ifndef MALGO_ALGO_SVD_H
#define MALGO_ALGO_SVD_H

#include <malgo/util.h>

namespace malgo
{
	template<class E, class D, class L>
	void svd(const eroot<E>& e, vroot<D>& d, eroot<L>& l)
	{
		auto eit = e.data();
		auto lit = l.data();

		type_t<E> tmp[e.dim()];

		for (int col = 0; col < e.dim(); ++col) 
		{
			// copy data to tmp
			for (int i = 0; i < e.dim() - col; ++i) 
			{
				tmp[i] = eit[i];
			}

			// применяю предыдущие столбцы
			for (int i = 0; i < e.dim() - col; ++i) 
			{
				//todo
			}

			//Вычисляю d
			d[col] = tmp[0] >= 0 ? 1 : -1;
			type_t<E> lcur = sqrt(*eit++ * d[col]); 			
			*lit++ = lcur;

			//Решаю уравнение
			for (int i = 0; i < e.dim() - col - 1; ++i) 
			{
				*lit++ = *eit / lcur * d[col];
			}

			eit += e.dim() - col;
		}

		/*for (int i = 0; i < e.dim(); ++i) {
			d[i] = *eit >= 0 ? 1 : -1; 
			for (int j = i; j < e.dim(); ++j)
			{

			}
		}*/
	}
}

#endif