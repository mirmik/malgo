#ifndef MALGO_VECTOR_H
#define MALGO_VECTOR_H

#include <initializer_list>
#include <memory>

#include <malgo/utilclasses.h>

namespace malgo
{
	template <class T, class A, class V>
	struct vector_root
	{
		using traits = vector_traits<V>;

		V& 					self_cast() 				{ return *static_cast<V*>(this); }
		const V& 			self_cast() const 			{ return *static_cast<const V*>(this); }
		int 				size() const 				{ return self_cast().size(); }
		T& 					operator[](int i) 			{ return self_cast()[i]; }
		const T& 			operator[](int i) const 	{ return self_cast()[i]; }

		typename traits::iterator 				begin() 		{ return self_cast().begin(); };
		typename traits::const_iterator 		begin() const 	{ return self_cast().begin(); };
		typename traits::iterator 		const 	end() 			{ return self_cast().end(); };
		typename traits::const_iterator const 	end() const 	{ return self_cast().end(); };
	};

	template <class T, class A, class V>
	struct compact_vector : public vector_root<T, A, compact_vector<T,A,V>>
	{
		using parent = vector_root<T, A, compact_vector<T,A,V>>;
		using iterator = T*;

		T* 					_data;
		int 				_size;
		int 				size() const				{ return _size; }
		T& 					operator[](int i) 			{ return _data[i]; }
		const T& 			operator[](int i) const 	{ return _data[i]; }

		T* 					begin() 					{ return _data; }
		const T* 			begin() const 				{ return _data; }
		T* 			const 	end() 						{ return _data + _size; }
		const T* 	const 	end() const 				{ return _data + _size; }

		compact_vector() 												{}
		compact_vector(T* data, int size) : _data(data), _size(size)	{}
 	};

	template <class T, class A, class V>
	struct uncompact_vector : public vector_root<T, A, uncompact_vector<T,A,V>>
	{
		using parent = vector_root<T, A, uncompact_vector<T,A,V>>;
		T* 		_data;
		int 	_size;
		int 	_step;
	};

	template <class T, class A = std::allocator<T>>
	struct vecview : public compact_vector<T, A, vecview<T, A>>
	{
		using parent = compact_vector<T, A, vecview<T, A>>;
		vecview(T* data, int size) : parent(data, size) {}
	};

	template <class T, class A = std::allocator<T>>
	struct uncompact_vecview : public uncompact_vector<T, A, uncompact_vecview<T, A>>
	{
		using parent = compact_vector<T, A, uncompact_vecview<T, A>>;
	};

	template <class T, class A = std::allocator<T>>
	struct vector : public compact_vector<T, A, vector<T, A>>, public vector_keeper<T, A, vector<T, A>>
	{
		using parent = compact_vector<T, A, vector<T, A>>;
		using keeper = vector_keeper<T, A, vector<T, A>>;
		vector()										{};
		vector(int size)								{ keeper::create(size); }
		vector(const T* data, int size)					{ keeper::create(size); std::copy(data, data+size, parent::_data); }
		vector(const std::initializer_list<T>&);
	};

	template<class T, class Z, class V, class W, class F> vector<T,Z> elementwise_do(const vector_root<T,Z,V>& a, const vector_root<T,Z,W>& b, F&& f) { vector<T,Z> res(a.size()); for (int i = 0; i < a.size(); ++i) res[i] = f(a[i], b[i]); return res; }
	template<class F, class Z, class V, class A, class B> constexpr A fold(F f, A a, const vector_root<B,Z,V> & b) { for (const auto& r : b) { a = f(a, r); } return a; }

	template<class T, class Z, class V, class W> vector<T,Z> operator + (const vector_root<T,Z,V>& a, const vector_root<T,Z,W>& b) { return elementwise_do(a, b, detail::op_add{}); }
	template<class T, class Z, class V, class W> vector<T,Z> operator - (const vector_root<T,Z,V>& a, const vector_root<T,Z,W>& b) { return elementwise_do(a, b, detail::op_sub{}); }
	template<class T, class Z, class V, class W> vector<T,Z> operator * (const vector_root<T,Z,V>& a, const vector_root<T,Z,W>& b) { return elementwise_do(a, b, detail::op_mul{}); }
	template<class T, class Z, class V, class W> vector<T,Z> operator / (const vector_root<T,Z,V>& a, const vector_root<T,Z,W>& b) { return elementwise_do(a, b, detail::op_div{}); }

	// Reduction functions on vectors
	template<class T, class Z, class V> constexpr bool any (const vector_root<T,Z,V> & a) { return fold(detail::op_or{}, false, a); }
	template<class T, class Z, class V> constexpr bool all (const vector_root<T,Z,V> & a) { return fold(detail::op_and{}, true, a); }
	template<class T, class Z, class V> constexpr T sum    (const vector_root<T,Z,V> & a) { return fold(detail::op_add{}, T(0), a); }
	template<class T, class Z, class V> constexpr T product(const vector_root<T,Z,V> & a) { return fold(detail::op_mul{}, T(1), a); }
	template<class T, class Z, class V> constexpr T minelem(const vector_root<T,Z,V> & a) { return fold(detail::min{}, a[0], a); }
	template<class T, class Z, class V> constexpr T maxelem(const vector_root<T,Z,V> & a) { return fold(detail::max{}, a[0], a); }

	template<class T, class Z, class V, class W> T 	dot(const vector_root<T,Z,V>& a, const vector_root<T,Z,W>& b) 	{ return sum(a * b); }
	template<class T, class Z, class V> T 			length2(const vector_root<T,Z,V> & a) 							{ return dot(a, a); }
	template<class T, class Z, class V> T 			length(const vector_root<T,Z,V> & a) 							{ return std::sqrt(length2(a)); }
}

template<class C, class T, class A, class V> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::vector_root<T,A,V> & v)
{
	out << '{';
	for (int i = 0; i < v.size() - 1; ++i)
		out << v[i] << ',';
	out << v[v.size() - 1] << '}';
}

#endif