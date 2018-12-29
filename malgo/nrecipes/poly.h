#ifndef MALGO_POLY_H
#define MALGO_POLY_H

#include <malgo/vector.h>
#include <nos/print.h>

namespace malgo
{

	/*template <typename>
	struct Poly
	{
		VecDoub &c;
		Poly(VecDoub &cc) : c(cc) {}
		Doub operator() (Doub x)
		{
			int j;
			Doub p = c[j = c.size() - 1];

			while (j > 0) p = p * x + c[--j];

			return p;
		}
	};*/

	/*void ddpoly(VecDoub_I &c, const Doub x, VecDoub_O &pd)
	{
		int nnd, j, i, nc = c.size() - 1, nd = pd.size() - 1;
		Doub cnst = 1.0;
		pd[0] = c[nc];

		for (j = 1; j < nd + 1; j++)
			pd[j] = 0.0;

		for (i = nc - 1; i >= 0; i--)
		{
			nnd = (nd < (nc - i) ? nd : nc - i);

			for (j = nnd; j > 0; j--)
				pd[j] = pd[j] * x + pd[j - 1];

			pd[0] = pd[0] * x + c[i];
		}

		for (i = 2; i < nd + 1; i++)
		{
			cnst *= i;
			pd[i] *= cnst;
		}
	}*/

	/// Деление полиномов. Старшая степень справа.
	/// @param u - делимое
	/// @param v - делитель
	/// @param q - частное
	/// @param r - остаток
	template <class U, class V, class Q, class R>
	void poldiv(const vector_root<U> &u, const vector_root<V> &v, vector_root<Q> &q, vector_root<R> &r)
	{
		int k; 
		int j;
		int n = u.size() - 1;	//Старший индекс вектора u
		int nv = v.size() - 1;	//Старший индекс вектора v

		//Отбрасываем ведущие нули полинома v.
		//Важно знать положение старшего значимого элемента v для вычисления частного.
		while (nv >= 0 && v[nv] == 0.) 
			nv--;

		if (nv < 0) throw("poldiv divide by zero polynomial");

		//Инициализируем выходные вектора.
		//r - Рабочая область. Тут хранится остаток от деления на итерациях.
		//q - Для записи частного. Инициализируем нулями, т.к. он может быть длиннее реального частного.
		r = u;
		q.reset(u.size(), 0.);

		//Длина вектора q совпадает с разностью длин полинома u и полинома v.
		//Если v длиннее u, этот цикл никогда не будет исполнен.
		for (k = n - nv; k >= 0; k--)
		{
			//Каждый следующий элемент частного будет равен частному головной части рабочего вектора r и делителя v
			q[k] = r[nv + k] / v[nv];

			//Подбиваем остаток.
			for (j = nv + k - 1; j >= k; j--)
				r[j] -= q[k] * v[j - k];
		}

		//Перезаписываем нулями все ведущие коэффициенты. Зачем??? 
		//Для улучшения точности. Остаток никак не может быть больше делителя. 
		//Значит тут все нули, что бы нам не насчитал алгоритм.
		for (j = nv; j <= n; j++) r[j] = 0.0;
	}
	
	/*struct Ratfn
	{
		VecDoub cofs;
		int nn, dd;

		Ratfn(VecDoub_I &num, VecDoub_I &den) : cofs(num.size() + den.size() - 1),
			nn(num.size()), dd(den.size())
		{
			int j;

			for (j = 0; j < nn; j++)
				cofs[j] = num[j] / den[0];

			for (j = 1; j < dd; j++)
				cofs[j + nn - 1] = den[j] / den[0];
		}

		Ratfn(VecDoub_I &coffs, const int n, const int d)
			: cofs(coffs), nn(n), dd(d) {}

		Doub operator() (Doub x) const
		{
			int j;
			Doub sumn = 0., sumd = 0.;

			for (j = nn - 1; j >= 0; j--)
				sumn = sumn * x + cofs[j];

			for (j = nn + dd - 2; j >= nn; j--)
				sumd = sumd * x + cofs[j];

			return sumn / (1.0 + x * sumd);
		}
	};*/
}

#endif