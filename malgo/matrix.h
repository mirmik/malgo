#ifndef MALGO_MATRIX_H
#define MALGO_MATRIX_H

#include <malgo/util.h>
#include <malgo/vector.h>
#include <malgo/algo/multiply.h>
#include <malgo/algo/inverse.h>
#include <malgo/raw/basealgo.h>

#include <nos/trace.h>

#include <linalg.h>

namespace malgo
{
	template <class M> struct matrix_iterator1;
	template <class M> struct matrix_iterator2;
	template <class M> struct const_matrix_iterator1;
	template <class M> struct const_matrix_iterator2;

	template <class M> struct matrix_iterator
	{
		M& m;
		int pos1;
		int pos2;

		matrix_iterator(M& _m, int _pos1, int _pos2) : m(_m), pos1(_pos1), pos2(_pos2) {}

		type_t<M> operator*() { return m(pos1, pos2); }
		const type_t<M> operator*() const { return m(pos1, pos2); }

		matrix_iterator2<M> begin2() 	{ return {m, pos1, pos2}; }
		matrix_iterator2<M> end2() 	{ return {m, pos1, m.size2()}; }

		bool operator==(const matrix_iterator& oth) { return pos1 == oth.pos1 && pos2 == oth.pos2; }
		bool operator!=(const matrix_iterator& oth) { return pos1 != oth.pos1 || pos2 != oth.pos2;; }
	};


	template <class M> struct matrix_iterator1 : public matrix_iterator<M>
	{
		matrix_iterator1(M& _m, int _pos1, int _pos2) : matrix_iterator<M>(_m, _pos1, _pos2) {}
		matrix_iterator1 operator++() { ++matrix_iterator<M>::pos1; return *this; };
		matrix_iterator1 operator++(int) { auto ret = *this; ++matrix_iterator<M>::pos1; return ret; };
	};

	template <class M> struct const_matrix_iterator1 : public matrix_iterator1<M>
	{
		const_matrix_iterator1(M& _m, int _pos1, int _pos2) : matrix_iterator1<M>(_m, _pos1, _pos2) {}
	};

	template <class M> struct matrix_iterator2 : public matrix_iterator<M>
	{
		matrix_iterator2(M& _m, int _pos1, int _pos2) : matrix_iterator<M>(_m, _pos1, _pos2) {}
		matrix_iterator2 operator++() { ++matrix_iterator<M>::pos2; return *this; };
		matrix_iterator2 operator++(int) { auto ret = *this; ++matrix_iterator<M>::pos2; return ret; };
	};

	template <class M> struct const_matrix_iterator2 : public matrix_iterator2<M>
	{
		const_matrix_iterator2(M& _m, int _pos1, int _pos2) : matrix_iterator2<M>(_m, _pos1, _pos2) {}
	};

	template <class T, class A> class matrix;
	template <class T, class A> class matrix_view;
	template <class T, class A> class const_matrix_view;

	template <class T, class A> struct traits<matrix<T, A>>
	{
		using type 					= T;
		using alloc 				= A;
		using matrix 				= malgo::matrix<T,A>;
		using const_matrix_view 	= malgo::const_matrix_view<T,A>;
		using vector 				= malgo::vector<T,A>;
		using iterator 				= T*;
		using const_iterator 		= const T*;
		using iterator1 			= matrix_iterator1<matrix>;
		using const_iterator1 		= const_matrix_iterator1<matrix>;
		using iterator2 			= matrix_iterator2<matrix>;
		using const_iterator2 		= const_matrix_iterator2<matrix>;
		using matout = matrix_view<T,A>;
	};

	template <class T, class A> struct traits<matrix_view<T, A>>
	{
		using type 					= T;
		using alloc 				= A;
		using matrix 				= malgo::matrix<T,A>;
		using matrix_view 			= malgo::matrix_view<T,A>;
		using const_matrix_view 	= malgo::const_matrix_view<T,A>;
		using vector 				= malgo::vector<T,A>;
		using iterator 				= T*;
		using const_iterator 		= const T*;
		using iterator1 			= matrix_iterator1<matrix>;
		using const_iterator1 		= const_matrix_iterator1<matrix>;
		using iterator2 			= matrix_iterator2<matrix>;
		using const_iterator2 		= const_matrix_iterator2<matrix>;
	};

	template <class M>
	struct mroot
	{
		using 						type = 		typename traits<M>::type;
		using 						alloc = 	typename traits<M>::alloc;

		M& 							self_cast() 					{ return *static_cast<M*>(this); }
		const M& 					self_cast() const 				{ return *static_cast<const M*>(this); }
		type& 						elem(int i)						{ return self_cast().elem(i); }
		const type& 				elem(int i) const				{ return self_cast().elem(i); }
		int 						size() const 					{ return self_cast().size(); }
		int 						size1() const 					{ return self_cast().size1(); }
		int 						size2() const 					{ return self_cast().size2(); }
		type* 						data()							{ return self_cast().data(); }
		const type* 				data() const					{ return self_cast().data(); }
		vecview<type, alloc> 		operator[](int i) 				{ return self_cast()[i]; }
		const vecview<type, alloc> 	operator[](int i) const 		{ return self_cast()[i]; }
		type&				 		operator()(int i, int j) 		{ return self_cast()(i, j); }
		const type&			 		operator()(int i, int j) const 	{ return self_cast()(i, j); }


		typename traits<M>::iterator 				begin() 		{ return self_cast().begin(); };
		typename traits<M>::iterator const 			end() 			{ return self_cast().end(); };
		typename traits<M>::iterator1 				begin1() 		{ return self_cast().begin1(); };
		typename traits<M>::iterator1 const 		end1() 			{ return self_cast().end1(); };
		typename traits<M>::iterator2 				begin2() 		{ return self_cast().begin2(); };
		typename traits<M>::iterator2 const 		end2() 			{ return self_cast().end2(); };
	};

	template <class M>
	struct compact_matrix : public mroot<M>
	{
		using 							type = 		typename traits<M>::type;
		using 							alloc = 	typename traits<M>::alloc;
		using 							parent = 	mroot<compact_matrix<M>>;
		type* 							_data;
		int 							_size1;
		int 							_size2;
		int 							size() const 					{ return _size1 * _size2; }
		int 							size1() const					{ return _size1; }
		int 							size2() const					{ return _size2; }
		type*							data()							{ return _data; }
		const type* 					data() const					{ return _data; }
		type&							elem(int i)						{ return _data[i]; }
		const type&						elem(int i)	const				{ return _data[i]; }
		vecview<type, alloc>	 		operator[](int i) 				{ return { _data + i * _size2, _size2 }; }
		const vecview<type, alloc>		operator[](int i) const 		{ return { _data + i * _size2, _size2 }; }
		type&							operator()(int i, int j)		{ return _data[i * _size2 + j]; }
		const type&						operator()(int i, int j) const	{ return _data[i * _size2 + j]; }

		type* 								begin() 		{ return _data; }
		const type* 						begin() const 	{ return _data; }
		type* const							end() 			{ return _data + _size1 * _size2; }
		const type* const					end() const 	{ return _data + _size1 * _size2; }

		matrix_iterator1<M> 				begin1() 		{ return {static_cast<M&>(*this), 0, 0}; }
		const_matrix_iterator1<M> 			begin1() const 	{ return {static_cast<M&>(*this), 0, 0}; }
		matrix_iterator1<M> const 			end1() 			{ return {static_cast<M&>(*this), _size1, 0}; }
		const_matrix_iterator1<M> const 	end1() const 	{ return {static_cast<M&>(*this), _size1, 0}; }

		matrix_iterator2<M> 				begin2() 		{ return {static_cast<M&>(*this), 0, 0}; }
		const_matrix_iterator2<M> 			begin2() const 	{ return {static_cast<M&>(*this), 0, 0}; }
		matrix_iterator2<M> const 			end2() 			{ return {static_cast<M&>(*this), 0, _size2 }; }
		const_matrix_iterator2<M> const 	end2() const 	{ return {static_cast<M&>(*this), 0, _size2 }; }

		operator matrix_view<type_t<M>, alloc_t<M>>() { return {_data, _size1, _size2}; }
	};

	template <class T, class A = std::allocator<T>>
	struct matrix : public compact_matrix<matrix<T, A>>, public matrix_keeper<matrix<T, A>>
	{
		using parent = compact_matrix<matrix<T, A>>;
		using keeper = matrix_keeper<matrix<T, A>>;
		matrix() {}
		matrix(int size1, int size2) { keeper::create(size1, size2); }
		matrix(const std::initializer_list<std::initializer_list<T>>& list) { keeper::create(list.size(), list.begin()->size()); T* ptr = parent::_data; for (const auto& c : list) { for (const auto& v : c) { *ptr++ = v; } } }
		matrix(const matrix& oth) { keeper::create(oth.size1(), oth.size2()); T* ptr = parent::_data; const T*mptr = oth.data(); for (int i = 0; i < parent::size(); ++i) *ptr++ = *mptr++; }
		template<class M> matrix(const mroot<M>& oth) { keeper::create(oth.size1(), oth.size2()); T* ptr = parent::_data; const T*mptr = oth.data(); for (int i = 0; i < parent::size(); ++i) *ptr++ = *mptr++; }
	
		template<int N> 
		matrix(const std::vector<linalg::vec<T,N>>& oth) 
		{ 
			keeper::create(oth.size(), N); 
			T* ptr = parent::data();

			for (unsigned int i = 0; i < oth.size(); ++i) 
				for (int j = 0; j < N; ++j)
					*ptr++ = oth[i][j];

			//T* ptr = parent::_data; 
			//const T*mptr = oth.data(); 
			//for (int i = 0; i < parent::size(); ++i) *ptr++ = *mptr++; 
		}
		
	};

	template <class T, class A = std::allocator<T>>
	struct matrix_view : public compact_matrix<matrix_view<T, A>>
	{
		using parent = compact_matrix<matrix_view<T, A>>;
		matrix_view(T* data, int size1, int size2) : parent(data, size1, size2) {};
		template <class W> matrix_view(mroot<W>& mat) : parent(mat.data(), mat.size1(), mat.size2()) {};
	};

	template<class F, class A, class B> using mxm_apply_t = matrix<ret_t<F, type_t<A>, type_t<B>>, alloc_t<A>>;
	template<class F, class A, class B> using sxm_apply_t = matrix<ret_t<F, A, type_t<B>>, alloc_t<A>>;
	template<class F, class A, class B> using mxs_apply_t = matrix<ret_t<F, type_t<A>, B>, alloc_t<A>>;
	template<class F, class A, class B> mxm_apply_t<F,A,B> elementwise(F&& f, const mroot<A>& a, const mroot<B>& b) { mxm_apply_t<F,A,B> res(a.size1(), b.size2()); for (int i = 0; i < a.size(); ++i) res.elem(i) = f(a.elem(i), b.elem(i)); return res; }
	template<class F, class A, class B> sxm_apply_t<F,A,B> sxm_elementwise(F&& f, const A& a, const mroot<B>& b) 	{ sxm_apply_t<F,A,B> res(b.size1(), b.size2()); for (int i = 0; i < b.size(); ++i) res.elem(i) = f(a, 	 b.elem(i)); 	return res; }
	template<class F, class A, class B> mxs_apply_t<F,A,B> mxs_elementwise(F&& f, const mroot<A>& a, const B& b) 	{ mxs_apply_t<F,A,B> res(a.size1(), a.size2()); for (int i = 0; i < a.size(); ++i) res.elem(i) = f(a.elem(i), 	 b); 	return res; }

	//Lexicographic compare
	template<class A, class B> int compare(const mroot<A>& a, const mroot<B>& b) 	{ if (a.size() != b.size()) return a.size() - b.size(); auto ait = a.begin(); auto bit = b.begin(); auto aend = a.end(); while (ait != aend) { auto r = *ait++ - *bit++; if (r != 0) return r; } return 0; }
	template<class A, class B> bool operator == (const mroot<A>& a, const mroot<B>& b) { return compare(a, b) == 0; }
	template<class A, class B> bool operator != (const mroot<A>& a, const mroot<B>& b) { return compare(a, b) != 0; }
	template<class A, class B> bool operator <  (const mroot<A>& a, const mroot<B>& b) { return compare(a, b) <  0; }
	template<class A, class B> bool operator >  (const mroot<A>& a, const mroot<B>& b) { return compare(a, b) >  0; }
	template<class A, class B> bool operator <= (const mroot<A>& a, const mroot<B>& b) { return compare(a, b) <= 0; }
	template<class A, class B> bool operator >= (const mroot<A>& a, const mroot<B>& b) { return compare(a, b) >= 0; }

	//Algebraic
	template<class A, class B> mxm_apply_t<detail::op_add,A,B> operator + (const mroot<A>& a, const mroot<B>& b) { return elementwise(detail::op_add{}, a, b); }
	template<class A, class B> mxm_apply_t<detail::op_sub,A,B> operator - (const mroot<A>& a, const mroot<B>& b) { return elementwise(detail::op_sub{}, a, b); }
	template<class A, class B> mxs_apply_t<detail::op_mul,A,B> operator * (const mroot<A>& a, B b) { return mxs_elementwise(detail::op_mul{}, a, b); }
	//template<class A, class B> mxm_apply_t<detail::op_mul,A,B> operator * (const mroot<A>& a, const mroot<B>& b) { return elementwise(detail::op_mul{}, a, b); }
	//template<class A, class B> mxm_apply_t<detail::op_div,A,B> operator / (const mroot<A>& a, const mroot<B>& b) { return elementwise(detail::op_div{}, a, b); }

	//Basealgo
	template<class M> matrix_t<M> transpose(const compact_matrix<M>& a) 	{ matrix_t<M> res(a.size2(), a.size1()); malgo::raw::transpose(a.data(), a.size1(), a.size2(), res.data()); return res; } 
	//template<class M> matrix_t<M> inverse(const compact_matrix<M>& a) 		{ assert(a.size1() == a.size2()); matrix_t<M> res(a.size1(), a.size1()); malgo::raw::square_matrix_inverse(a.data(), a.size1(), res.data()); return res; } 
	template<class M> matrix_t<M> exponent(const compact_matrix<M>& a) 	{ assert(a.size1() == a.size2()); matrix_t<M> res(a.size1(), a.size1()); malgo::raw::square_matrix_exponent(a.data(), a.size1(), res.data()); return res; } 

	template<class T, class A = std::allocator<T>> matrix<T,A> diag(const std::initializer_list<T>& lst) { matrix<T,A> ret(lst.size(), lst.size()); const auto* data = lst.begin(); for (int i = 0; i < lst.size(); ++i) ret(i,i) = data[i]; return ret; } 

	template<class T> matrix<T> identity(int num) { matrix<T> ret(num,num); for (int i = 0; i < num; ++i) for (int j = 0; j < num; ++j) ret(i,j) = i==j ? 1 : 0; return ret; }
}


template<class C, class M> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::mroot<M> & m)
{
	out << '{';
	for (int i = 0; i < m.size1() - 1; ++i)
	{
		out << '{';
		for (int j = 0; j < m.size2() - 1; ++j)
		{
			out << m[i][j] << ',';
		}
		out << m[i][m.size2() - 1] << '}' << ',';
	}
	out << '{';
	for (int j = 0; j < m.size2() - 1; ++j)
	{
		out << m[m.size1() - 1][j] << ',';
	}
	out << m[m.size1() - 1][m.size2() - 1] << '}' << '}';
}

#endif