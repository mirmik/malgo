#ifndef MALGO_SVD_H
#define MALGO_SVD_H

#include <limits>
#include <malgo/matrix.h>


//Attention Row Order.

namespace malgo
{
	template <typename M>
	struct SVD
	{
		int m, n;
		matrix_t<M> u, v;
		vector_t<M> w;
		type_t<M> eps, tsh;

		SVD(mroot<M> &a) : m(a.size1()), n(a.size2()), u(a), v(n, n), w(n)
		{
			eps = std::numeric_limits<type_t<M>>::epsilon();
			decompose();
			nos::print_matrix(u);
			reorder();
			tsh = 0.5 * sqrt(m + n + 1.) * w[0] * eps;
		}

		template<class A, class B> void solve(const vroot<A> &b, vroot<B> &x, type_t<M> thresh = -1.);
		void solve(const mroot<M> &b, mroot<M> &x, type_t<M> thresh = -1.);

		int 			rank(type_t<M> thresh = -1.);
		int 			nullity(type_t<M> thresh = -1.);
		matrix_t<M> 	range(type_t<M> thresh = -1.);
		matrix_t<M> 	nullspace(type_t<M> thresh = -1.);

		type_t<M> inv_condition()
		{
			return (w[0] <= 0. || w[n - 1] <= 0.) ? 0. : w[n - 1] / w[0];
		}

		void decompose();
		void reorder();
		type_t<M> pythag(const type_t<M> a, const type_t<M> b);
	};

	template<class M>
	template<class A, class B>
	void SVD<M>::solve(const vroot<A> &b, vroot<B> &x, type_t<M> thresh)
	{
		int i, j, jj;
		type_t<M> s;
		if (b.size() != m || x.size() != n) throw("SVD::solve bad sizes");
		vector_t<M> tmp(n);
		tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(m + n + 1.) * w[0] * eps);
		for (j = 0; j < n; j++)
		{
			s = 0.0;
			if (w[j] > tsh)
			{
				for (i = 0; i < m; i++) s += u[i][j] * b[i];
				s /= w[j];
			}
			tmp[j] = s;
		}
		for (j = 0; j < n; j++)
		{
			s = 0.0;
			for (jj = 0; jj < n; jj++) s += v[j][jj] * tmp[jj];
			x[j] = s;
		}
	}

	template <typename M>
	void SVD<M>::solve(const mroot<M> &b, mroot<M> &x, type_t<M> thresh)
	{
		int i, j, p = b.ncols();
		if (b.nrows() != m || x.nrows() != n || x.ncols() != p)
			throw("SVD::solve bad sizes");
		vector_t<M> xx(n), bcol(m);
		for (j = 0; j < p; j++)
		{
			for (i = 0; i < m; i++) bcol[i] = b[i][j];
			solve(bcol, xx, thresh);
			for (i = 0; i < n; i++) x[i][j] = xx[i];
		}
	}

	template <typename M>
	int SVD<M>::rank(type_t<M> thresh)
	{
		int j, nr = 0;
		tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(m + n + 1.) * w[0] * eps);
		for (j = 0; j < n; j++) if (w[j] > tsh) nr++;
		return nr;
	}

	template <typename M>
	int SVD<M>::nullity(type_t<M> thresh)
	{
		int j, nn = 0;
		tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(m + n + 1.) * w[0] * eps);
		for (j = 0; j < n; j++) if (w[j] <= tsh) nn++;
		return nn;
	}

	template <typename M>
	matrix_t<M> SVD<M>::range(type_t<M> thresh)
	{
		int i, j, nr = 0;
		matrix_t<M> rnge(m, rank(thresh));
		for (j = 0; j < n; j++)
		{
			if (w[j] > tsh)
			{
				for (i = 0; i < m; i++) rnge[i][nr] = u[i][j];
				nr++;
			}
		}
		return rnge;
	}

	template <typename M>
	matrix_t<M> SVD<M>::nullspace(type_t<M> thresh)
	{
		int j, jj, nn = 0;
		matrix_t<M> nullsp(n, nullity(thresh));
		for (j = 0; j < n; j++)
		{
			if (w[j] <= tsh)
			{
				for (jj = 0; jj < n; jj++) nullsp[jj][nn] = v[jj][j];
				nn++;
			}
		}
		return nullsp;
	}

	template <typename M>
	void SVD<M>::decompose()
	{
		bool flag;
		int i, its, j, jj, k, l, nm;
		type_t<M> anorm, c, f, g, h, s, scale, x, y, z;
		vector_t<M> rv1(n);
		g = scale = anorm = 0.0;
		for (i = 0; i < n; i++)
		{
			l = i + 2;
			rv1[i] = scale * g;
			g = s = scale = 0.0;
			if (i < m)
			{
				for (k = i; k < m; k++) scale += abs(u[k][i]);
				if (scale != 0.0)
				{
					for (k = i; k < m; k++)
					{
						u[k][i] /= scale;
						s += u[k][i] * u[k][i];
					}
					f = u[i][i];
					g = -malgo::sign(sqrt(s), f);
					h = f * g - s;
					u[i][i] = f - g;
					for (j = l - 1; j < n; j++)
					{
						for (s = 0.0, k = i; k < m; k++) s += u[k][i] * u[k][j];
						f = s / h;
						for (k = i; k < m; k++) u[k][j] += f * u[k][i];
					}
					for (k = i; k < m; k++) u[k][i] *= scale;
				}
			}
			w[i] = scale * g;
			g = s = scale = 0.0;
			if (i + 1 <= m && i + 1 != n)
			{
				for (k = l - 1; k < n; k++) scale += abs(u[i][k]);
				if (scale != 0.0)
				{
					for (k = l - 1; k < n; k++)
					{
						u[i][k] /= scale;
						s += u[i][k] * u[i][k];
					}
					f = u[i][l - 1];
					g = -malgo::sign(sqrt(s), f);
					h = f * g - s;
					u[i][l - 1] = f - g;
					for (k = l - 1; k < n; k++) rv1[k] = u[i][k] / h;
					for (j = l - 1; j < m; j++)
					{
						for (s = 0.0, k = l - 1; k < n; k++) s += u[j][k] * u[i][k];
						for (k = l - 1; k < n; k++) u[j][k] += s * rv1[k];
					}
					for (k = l - 1; k < n; k++) u[i][k] *= scale;
				}
			}
			anorm = malgo::max(anorm, (abs(w[i]) + abs(rv1[i])));
		}
		for (i = n - 1; i >= 0; i--)
		{
			if (i < n - 1)
			{
				if (g != 0.0)
				{
					for (j = l; j < n; j++)
						v[j][i] = (u[i][j] / u[i][l]) / g;
					for (j = l; j < n; j++)
					{
						for (s = 0.0, k = l; k < n; k++) s += u[i][k] * v[k][j];
						for (k = l; k < n; k++) v[k][j] += s * v[k][i];
					}
				}
				for (j = l; j < n; j++) v[i][j] = v[j][i] = 0.0;
			}
			v[i][i] = 1.0;
			g = rv1[i];
			l = i;
		}
		for (i = malgo::min(m, n) - 1; i >= 0; i--)
		{
			l = i + 1;
			g = w[i];
			for (j = l; j < n; j++) u[i][j] = 0.0;
			if (g != 0.0)
			{
				g = 1.0 / g;
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < m; k++) s += u[k][i] * u[k][j];
					f = (s / u[i][i]) * g;
					for (k = i; k < m; k++) u[k][j] += f * u[k][i];
				}
				for (j = i; j < m; j++) u[j][i] *= g;
			}
			else for (j = i; j < m; j++) u[j][i] = 0.0;
			++u[i][i];
		}
		for (k = n - 1; k >= 0; k--)
		{
			for (its = 0; its < 30; its++)
			{
				flag = true;
				for (l = k; l >= 0; l--)
				{
					nm = l - 1;
					if (l == 0 || abs(rv1[l]) <= eps * anorm)
					{
						flag = false;
						break;
					}
					if (abs(w[nm]) <= eps * anorm) break;
				}
				if (flag)
				{
					c = 0.0;
					s = 1.0;
					for (i = l; i < k + 1; i++)
					{
						f = s * rv1[i];
						rv1[i] = c * rv1[i];
						if (abs(f) <= eps * anorm) break;
						g = w[i];
						h = pythag(f, g);
						w[i] = h;
						h = 1.0 / h;
						c = g * h;
						s = -f * h;
						for (j = 0; j < m; j++)
						{
							y = u[j][nm];
							z = u[j][i];
							u[j][nm] = y * c + z * s;
							u[j][i] = z * c - y * s;
						}
					}
				}
				z = w[k];
				if (l == k)
				{
					if (z < 0.0)
					{
						w[k] = -z;
						for (j = 0; j < n; j++) v[j][k] = -v[j][k];
					}
					break;
				}
				if (its == 29) throw("no convergence in 30 svdcmp iterations");
				x = w[l];
				nm = k - 1;
				y = w[nm];
				g = rv1[nm];
				h = rv1[k];
				f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
				g = pythag(f, 1.0);
				f = ((x - z) * (x + z) + h * ((y / (f + malgo::sign(g, f))) - h)) / x;
				c = s = 1.0;
				for (j = l; j <= nm; j++)
				{
					i = j + 1;
					g = rv1[i];
					y = w[i];
					h = s * g;
					g = c * g;
					z = pythag(f, h);
					rv1[j] = z;
					c = f / z;
					s = h / z;
					f = x * c + g * s;
					g = g * c - x * s;
					h = y * s;
					y *= c;
					for (jj = 0; jj < n; jj++)
					{
						x = v[jj][j];
						z = v[jj][i];
						v[jj][j] = x * c + z * s;
						v[jj][i] = z * c - x * s;
					}
					z = pythag(f, h);
					w[j] = z;
					if (z)
					{
						z = 1.0 / z;
						c = f * z;
						s = h * z;
					}
					f = c * g + s * y;
					x = c * y - s * g;
					for (jj = 0; jj < m; jj++)
					{
						y = u[jj][j];
						z = u[jj][i];
						u[jj][j] = y * c + z * s;
						u[jj][i] = z * c - y * s;
					}
				}
				rv1[l] = 0.0;
				rv1[k] = f;
				w[k] = x;
			}
		}
	}

	template <typename M>
	void SVD<M>::reorder()
	{
		int i, j, k, s, inc = 1;
		type_t<M> sw;
		vector_t<M> su(m), sv(n);
		do { inc *= 3; inc++; }
		while (inc <= n);
		do
		{
			inc /= 3;
			for (i = inc; i < n; i++)
			{
				sw = w[i];
				for (k = 0; k < m; k++) su[k] = u[k][i];
				for (k = 0; k < n; k++) sv[k] = v[k][i];
				j = i;
				while (w[j - inc] < sw)
				{
					w[j] = w[j - inc];
					for (k = 0; k < m; k++) u[k][j] = u[k][j - inc];
					for (k = 0; k < n; k++) v[k][j] = v[k][j - inc];
					j -= inc;
					if (j < inc) break;
				}
				w[j] = sw;
				for (k = 0; k < m; k++) u[k][j] = su[k];
				for (k = 0; k < n; k++) v[k][j] = sv[k];

			}
		}
		while (inc > 1);
		for (k = 0; k < n; k++)
		{
			s = 0;
			for (i = 0; i < m; i++) if (u[i][k] < 0.) s++;
			for (j = 0; j < n; j++) if (v[j][k] < 0.) s++;
			if (s > (m + n) / 2)
			{
				for (i = 0; i < m; i++) u[i][k] = -u[i][k];
				for (j = 0; j < n; j++) v[j][k] = -v[j][k];
			}
		}
	}

	template <typename M>
	type_t<M> SVD<M>::pythag(const type_t<M> a, const type_t<M> b)
	{
		type_t<M> absa = abs(a), absb = abs(b);
		return (absa > absb ? absa * sqrt(1.0 + malgo::sqr(absb / absa)) :
		        (absb == 0.0 ? 0.0 : absb * sqrt(1.0 + malgo::sqr(absa / absb))));
	}
}

#endif