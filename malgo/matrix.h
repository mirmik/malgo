#ifndef MALGO_MATRIX_H
#define MALGO_MATRIX_H

#include <malgo/util.h>
#include <malgo/vector.h>

namespace malgo
{

	template <class T, class A> struct compact_const_iterator1;

	template <class T, class A> struct compact_iterator1 {
		T* 			_data;
		int 		_size;

		compact_iterator1 operator++() { _data += _size; return *this; };
		compact_iterator1 operator++(int) { auto ret = *this; _data += _size; return ret; };

		vecview<T, A> operator*() { return {_data, _size}; }

		bool operator==(const compact_iterator1& oth) { return oth._data == _data; }
		bool operator!=(const compact_iterator1& oth) { return oth._data != _data; }
		bool operator==(const compact_const_iterator1<T,A>& oth) { return oth._data == _data; }
		bool operator!=(const compact_const_iterator1<T,A>& oth) { return oth._data != _data; }
	};

	template <class T, class A> struct compact_const_iterator1 {
		const T*	_data;
		int 		_size;

		compact_const_iterator1 operator++() { _data += _size; return *this; };
		compact_const_iterator1 operator++(int) { auto ret = *this; _data += _size; return ret; };

		const_vecview<T, A> operator*() { return {_data, _size}; }

		bool operator==(const compact_iterator1<T,A>& oth) { return oth._data == _data; }
		bool operator!=(const compact_iterator1<T,A>& oth) { return oth._data != _data; }
		bool operator==(const compact_const_iterator1& oth) { return oth._data == _data; }
		bool operator!=(const compact_const_iterator1& oth) { return oth._data != _data; }
	};

	template <class T, class O, class A> class matrix;
	
	template <class T, class O, class A> struct traits<matrix<T, O, A>> 
	{ 
		using type = T; 
		using alloc = A; 
		using order = O; 
		using iterator1 = compact_iterator1<T,A>;
		using const_iterator1 = compact_const_iterator1<T,A>;
	};

	template <class M>
	struct matrix_root
	{
		using 						type = 		typename traits<M>::type;
		using 						alloc = 	typename traits<M>::alloc;

		M& 							self_cast() 				{ return *static_cast<M*>(this); }
		const M& 					self_cast() const 			{ return *static_cast<const M*>(this); }
		int 						size1() const 				{ return self_cast().size1(); }
		int 						size2() const 				{ return self_cast().size2(); }
		type* 						data()						{ return self_cast().data(); }
		const type* 				data() const				{ return self_cast().data(); }
		vecview<type, alloc> 		operator[](int i) 			{ return self_cast()[i]; }
		const vecview<type, alloc> 	operator[](int i) const 	{ return self_cast()[i]; }

		typename traits<M>::iterator1 				begin1() 		{ return self_cast().begin1(); };
		typename traits<M>::const_iterator1 		begin1() const 	{ return self_cast().begin1(); };
		typename traits<M>::iterator1 		 const 	end1() 			{ return self_cast().end1(); };
		typename traits<M>::const_iterator1 const 	end1() const 	{ return self_cast().end1(); };
	};

	template <class M>
	struct compact_matrix : public matrix_root<M>
	{
		using 							type = 		typename traits<M>::type;
		using 							alloc = 	typename traits<M>::alloc;
		using 							parent = 	matrix_root<compact_matrix<M>>;
		type* 							_data;
		int 							_size1;
		int 							_size2;
		int 							size1() const				{ return _size1; }
		int 							size2() const				{ return _size2; }
		type*							data()						{ return _data; }
		const type* 					data() const				{ return _data; }
		vecview<type, alloc>	 		operator[](int i) 			{ return { _data + i * _size2, _size2}; }
		const vecview<type, alloc>		operator[](int i) const 	{ return { _data + i * _size2, _size2 }; }

		compact_iterator1<type_t<M>, alloc_t<M>> 				begin1() 		{ return { _data, _size2 }; };
		compact_const_iterator1<type_t<M>, alloc_t<M>> 			begin1() const 	{ return { _data, _size2 }; };
		compact_iterator1<type_t<M>, alloc_t<M>> const 			end1() 			{ return { _data + _size1 * _size2, _size2 }; };
		compact_const_iterator1<type_t<M>, alloc_t<M>> const 	end1() const 	{ return { _data + _size1 * _size2, _size2 }; };
	};

	template <class T, class O = rowmajor, class A = std::allocator<T>>
	struct matrix : public compact_matrix<matrix<T, O, A>>, public matrix_keeper<matrix<T, O, A>>
	{
		using parent = compact_matrix<matrix<T, O, A>>;
		using keeper = matrix_keeper<matrix<T, O, A>>;
		matrix() {}
		matrix(int size1, int size2) { keeper::create(size1, size2); }
		matrix(const std::initializer_list<std::initializer_list<T>>& list) { keeper::create(list.size(), list.begin()->size()); T* ptr = parent::_data; for (const auto& c : list) { for (const auto& v : c) { *ptr++ = v; } } }
	};

	template <class A, class B> bool straight_compare(const A& a, const B& b) { auto ait = a.begin1(); auto bit = b.begin1(); auto aend = a.end1(); while (ait != aend) { auto r = compare(*ait++, *bit++); if (r != 0) return r; } return 0; }

	template <class OA, class OB> struct order_variant_impl;

	template <> struct order_variant_impl<rowmajor, rowmajor>
	{
		template<class A, class B> static bool op_eq(const A& a, const B& b) { return straight_compare(a, b) == 0; }
	};

	template <> struct order_variant_impl<rowmajor, colmajor>
	{
		template<class A, class B> static bool op_eq(const A& a, const B& b) { return true; }
	};

	template <> struct order_variant_impl<colmajor, rowmajor>
	{
		template<class A, class B> static bool op_eq(const A& a, const B& b) { return true; }
	};

	template <> struct order_variant_impl<colmajor, colmajor>
	{
		template<class A, class B> static bool op_eq(const A& a, const B& b) { return true; }
	};

	template<class A, class B> struct orders_variant : public order_variant_impl<typename traits<A>::order, typename traits<B>::order> {};

	template <class A, class B> bool operator == (const matrix_root<A>& a, const matrix_root<B>& b) { return orders_variant<A, B>::op_eq(a, b); }
}

template<class C, class M> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::matrix_root<M> & m)
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