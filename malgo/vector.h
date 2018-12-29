#ifndef MALGO_VECTOR_H
#define MALGO_VECTOR_H

#include <initializer_list>
#include <memory>

#include <malgo/util.h>

namespace malgo
{
	template <class T, class A> class vector;
	template <class T, class A> class vecview;
	template <class T, class A> class const_vecview;
	template <class T, class A> class uncompact_vecview;
	template <class T, class A> class const_uncompact_vecview;

	template <class T, class A> struct traits<vector<T,A>> { 
			using type = T;
			using alloc = A;
			using iterator = T*; 
			using const_iterator = const T*; 
			using vector = malgo::vector<T,A>;
	};

	template <class T, class A> struct traits<vecview<T,A>> { 
			using type = T;
			using alloc = A;
			using iterator = T*; 
			using const_iterator = const T*; 
			using vector = malgo::vector<T,A>;
	};

	template <class T, class A> struct traits<const_vecview<T,A>> { 
			using type = T;
			using alloc = A;
			using iterator = T*; 
			using const_iterator = const T*; 
			using vector = malgo::vector<T,A>;
	};
	
	template <class T, class A> struct traits<uncompact_vecview<T,A>> { 
			using type = T;
			using alloc = A;
			using iterator = T*; 
			using const_iterator = const T*; 
			using vector = malgo::vector<T,A>;
	};

	template <class T, class A> struct traits<const_uncompact_vecview<T,A>> { 
			using type = T;
			using alloc = A;
			using iterator = T*; 
			using const_iterator = const T*; 
			using vector = malgo::vector<T,A>;
	};

	template <class V>
	struct const_vector_root
	{
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;
		
		const V& 				self_cast() const 			{ return *static_cast<const V*>(this); }
		int 					size() const 				{ return self_cast().size(); }
		const type* 			data() const				{ return self_cast().data(); }
		const type& 			operator[](int i) const 	{ return self_cast()[i]; }
		const_iterator 			begin() const 				{ return self_cast().begin(); };
		const_iterator const 	end() const 				{ return self_cast().end(); };
	};

	template <class V>
	struct vector_root : public const_vector_root<V>
	{
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;
		
		V& 						self_cast() 				{ return *static_cast<V*>(this); }
		type* 					data()						{ return self_cast().data(); }
		type& 					operator[](int i) 			{ return self_cast()[i]; }
		const type& 			operator[](int i) const 	{ return self_cast()[i]; }
		iterator 				begin() 		{ return self_cast().begin(); };
		iterator const 			end() 			{ return self_cast().end(); };
	};

	template <class V>
	struct compact_vector : public vector_root<V>
	{
		using parent = vector_root<V>;
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;

		type* 					_data;
		int 					_size;
		int 					size() const				{ return _size; }
		type* 					data()						{ return _data; }
		const type* 			data() const				{ return _data; }
		type& 					operator[](int i) 			{ return _data[i]; }
		const type& 			operator[](int i) const 	{ return _data[i]; }

		iterator 				begin() 					{ return _data; }
		const_iterator 			begin() const 				{ return _data; }
		const iterator 			end() 						{ return _data + _size; }
		const const_iterator 	end() const 				{ return _data + _size; }

		compact_vector() 												{}
		compact_vector(type* data, int size) : _data(data), _size(size)	{}
	};

	template <class V>
	struct const_compact_vector : public vector_root<V>
	{
		using parent = vector_root<V>;
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;

		const type*				_data;
		int 					_size;
		int 					size() const				{ return _size; }
		const type* 			data() const				{ return _data; }
		const type& 			operator[](int i) const 	{ return _data[i]; }
		const_iterator 			begin() const 				{ return _data; }
		const const_iterator 	end() const 				{ return _data + _size; }

		const_compact_vector() 												{}
		const_compact_vector(const type* data, int size) : _data(data), _size(size)	{}
	};

	template <class V>
	struct uncompact_vector : public vector_root<V>
	{
		using parent = vector_root<V>;
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;
		type* 	_data;
		int 	_size;
		int 	_step;
		int 					size() const				{ return _size; }
		type* 					data()						{ return _data; }
		const type* 			data() const				{ return _data; }
		type& 					operator[](int i) 			{ return _data[i*_step]; }
		const type& 			operator[](int i) const 	{ return _data[i*_step]; }

		iterator 				begin() 					{ return {_data, _step}; }
		const_iterator 			begin() const 				{ return {_data, _step}; }
		const iterator 			end() 						{ return {_data + _size, _step}; }
		const const_iterator 	end() const 				{ return {_data + _size, _step}; }
	
		uncompact_vector() 																			{}
		uncompact_vector(type* data, int size, int step) : _data(data), _size(size), _step(step)	{}
	};

	template <class V>
	struct const_uncompact_vector : public vector_root<V>
	{
		using parent = vector_root<V>;
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;
		const type* 	_data;
		int 			_size;
		int 			_step;
		int 					size() const				{ return _size; }
		const type* 			data() const				{ return _data; }
		const type& 			operator[](int i) const 	{ return _data[i*_step]; }
		const_iterator 			begin() const 				{ return {_data, _step}; }
		const const_iterator 	end() const 				{ return {_data + _size, _step}; }
	
		const_uncompact_vector() 																			{}
		const_uncompact_vector(const type* data, int size, int step) : _data(data), _size(size), _step(step)	{}
	};

	template <class T, class A = std::allocator<T>>
	struct vecview : public compact_vector<vecview<T, A>>
	{
		using parent = compact_vector<vecview<T, A>>;
		vecview(T* data, int size) : parent(data, size) {}
	};

	template <class T, class A = std::allocator<T>>
	struct const_vecview : public const_compact_vector<const_vecview<T, A>>
	{
		using parent = const_compact_vector<const_vecview<T, A>>;
		const_vecview(const T* data, int size) : parent(data, size) {}
	};

	template <class T, class A = std::allocator<T>>
	struct uncompact_vecview : public uncompact_vector<uncompact_vecview<T, A>>
	{
		using parent = uncompact_vector<uncompact_vecview<T, A>>;
		uncompact_vecview(T* data, int size, int step) : parent(data, size, step) {}
	};

	template <class T, class A = std::allocator<T>>
	struct const_uncompact_vecview : public const_uncompact_vector<const_uncompact_vecview<T, A>>
	{
		using parent = const_uncompact_vector<const_uncompact_vecview<T, A>>;
		const_uncompact_vecview(const T* data, int size, int step) : parent(data, size, step) {}
	};

	template <class T, class A = std::allocator<T>>
	struct vector : public compact_vector<vector<T, A>>, public vector_keeper<vector<T, A>>
	{
		using parent = compact_vector<vector<T, A>>;
		using keeper = vector_keeper<vector<T, A>>;
		vector()										{};
		vector(int size)								{ keeper::create(size); }
		vector(const T* data, int size)					{ keeper::create(size); std::copy(data, data+size, parent::_data); }
		vector(const std::initializer_list<T>& list)	{ keeper::create(list.size()); std::copy(list.begin(), list.end(), parent::_data); }
	};

	template<class V, class W> int compare(const vector_root<V>& a, const vector_root<W>& b) { if (a.size() != b.size()) return a.size() - b.size(); for (int i = 0; i < a.size(); ++i) if (a[i] != b[i]) return a[i] - b[i]; return 0; }	

	template<class A, class B> auto operator == (const vector_root<A> & a, const vector_root<B> & b) -> decltype(compare(a,b) == 0) { return compare(a,b) == 0; }
	template<class A, class B> auto operator != (const vector_root<A> & a, const vector_root<B> & b) -> decltype(compare(a,b) != 0) { return compare(a,b) != 0; }
	template<class A, class B> auto operator <  (const vector_root<A> & a, const vector_root<B> & b) -> decltype(compare(a,b) <  0) { return compare(a,b) <  0; }
	template<class A, class B> auto operator >  (const vector_root<A> & a, const vector_root<B> & b) -> decltype(compare(a,b) >  0) { return compare(a,b) >  0; }
	template<class A, class B> auto operator <= (const vector_root<A> & a, const vector_root<B> & b) -> decltype(compare(a,b) <= 0) { return compare(a,b) <= 0; }
	template<class A, class B> auto operator >= (const vector_root<A> & a, const vector_root<B> & b) -> decltype(compare(a,b) >= 0) { return compare(a,b) >= 0; }

	template<class V, class W, class F> typename V::vector elementwise_do(const vector_root<V>& a, const vector_root<W>& b, F&& f) { typename V::vector res(a.size()); for (int i = 0; i < a.size(); ++i) res[i] = f(a[i], b[i]); return res; }
	template<class V, class A, class F> A fold(F f, A a, const vector_root<V> & b) { for (const auto& r : b) { a = f(a, r); } return a; }

	template<class V, class W> typename traits<V>::vector operator + (const vector_root<V>& a, const vector_root<W>& b) { return elementwise_do(a, b, detail::op_add{}); }
	template<class V, class W> typename traits<V>::vector operator - (const vector_root<V>& a, const vector_root<W>& b) { return elementwise_do(a, b, detail::op_sub{}); }
	template<class V, class W> typename traits<V>::vector operator * (const vector_root<V>& a, const vector_root<W>& b) { return elementwise_do(a, b, detail::op_mul{}); }
	template<class V, class W> typename traits<V>::vector operator / (const vector_root<V>& a, const vector_root<W>& b) { return elementwise_do(a, b, detail::op_div{}); }

	// Reduction functions on vectors
	template<class V> bool any (const vector_root<V> & a) { return fold(detail::op_or{}, false, a); }
	template<class V> bool all (const vector_root<V> & a) { return fold(detail::op_and{}, true, a); }
	template<class V> typename traits<V>::type sum    (const vector_root<V> & a) { return fold(detail::op_add{}, typename traits<V>::type(0), a); }
	template<class V> typename traits<V>::type product(const vector_root<V> & a) { return fold(detail::op_mul{}, typename traits<V>::type(1), a); }
	template<class V> typename traits<V>::type minelem(const vector_root<V> & a) { return fold(detail::min{}, a[0], a); }
	template<class V> typename traits<V>::type maxelem(const vector_root<V> & a) { return fold(detail::max{}, a[0], a); }

	template<class V, class W> typename traits<V>::type dot(const vector_root<V>& a, const vector_root<W>& b) 	{ return sum(a * b); }
	template<class V> typename traits<V>::type 			length2(const vector_root<V> & a) 							{ return dot(a, a); }
	template<class V> typename traits<V>::type 			length(const vector_root<V> & a) 							{ return std::sqrt(length2(a)); }
}

template<class C, class V> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::vector_root<V> & v)
{
	out << '{';
	for (int i = 0; i < v.size() - 1; ++i)
		out << v[i] << ',';
	out << v[v.size() - 1] << '}';
}

#endif