#ifndef MALGO_MATRIX_H
#define MALGO_MATRIX_H

#include <malgo/utilclasses.h>
#include <malgo/vector.h>

namespace malgo
{
	template <class T, class O, class A, class M>
	struct matrix_root
	{
		using traits = matrix_traits<M>;

		M& 					self_cast() 				{ return *static_cast<M*>(this); }
		const M& 			self_cast() const 			{ return *static_cast<const M*>(this); }
		int 				size1() const 				{ return self_cast().size1(); }
		int 				size2() const 				{ return self_cast().size2(); }
		vecview<T,A> 		operator[](int i) 			{ return self_cast()[i]; }
		const vecview<T,A> 	operator[](int i) const 	{ return self_cast()[i]; }

		/*typename traits::iterator 				begin() 		{ return self_cast().begin(); };
		typename traits::const_iterator 		begin() const 	{ return self_cast().begin(); };
		typename traits::iterator 		const 	end() 			{ return self_cast().end(); };
		typename traits::const_iterator const 	end() const 	{ return self_cast().end(); };*/
	};

	template <class T, class O, class A, class M>
	struct compact_matrix : public matrix_root<T, O, A, compact_matrix<T, O, A, M>>
	{
		T* _data;
		int _size1;
		int _size2;
		using parent = matrix_root<T, O, A, compact_matrix<T, O, A, M>>;
		int 				size1() const				{ return _size1; }
		int 				size2() const				{ return _size2; }
		vecview<T,A> 		operator[](int i) 			{ return { _data + i * _size2, _size2}; }
		const vecview<T,A>	operator[](int i) const 	{ return { _data + i * _size2, _size2 }; }
	};

	template <class T, class O = row_order, class A = std::allocator<T>>
	struct matrix : public compact_matrix<T, O, A, matrix<T, O, A>>, public matrix_keeper<T, O, A, matrix<T, O, A>>
	{
		using parent = compact_matrix<T, O, A, matrix<T, O, A>>;
		using keeper = matrix_keeper<T, O, A, matrix<T, O, A>>;
		matrix() {}
		matrix(int size1, int size2) { keeper::create(size1, size2); }
		matrix(const std::initializer_list<std::initializer_list<T>>& list) { keeper::create(list.size(), list.begin()->size()); T* ptr = parent::_data; for (const auto& c : list) { for (const auto& v : c) { *ptr++ = v; } } }
	};
}

template<class C, class T, class O, class A, class M> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::matrix_root<T, O, A, M> & m)
{
	out << '{';
	for (int i = 0; i < m.size1() - 1; ++i)
	{
		out << '{';
		for (int j = 0; j < m.size2() - 1; ++j)
		{
			out << m[i][j] << ',';
		}
		out << m[i][m.size2() - 1] << '}';
	}
	out << '{';
	for (int j = 0; j < m.size2() - 1; ++j)
	{
		out << m[m.size1() - 1][j] << ',';
	}
	out << m[m.size1() - 1][m.size2() - 1] << '}' << '}';
}

#endif