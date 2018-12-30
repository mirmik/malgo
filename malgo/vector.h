#ifndef MALGO_VECTOR_H
#define MALGO_VECTOR_H

#include <initializer_list>
#include <memory>

#include <malgo/util.h>

#include <nos/trace.h>

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
	
	template <class T, class A> struct traits<uncompact_vecview<T,A>> { 
			using type = T;
			using alloc = A;
			using iterator = T*; 
			using const_iterator = const T*; 
			using vector = malgo::vector<T,A>;
	};

	template <class V>
	struct vector_root
	{
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;
		
		const V& 			self_cast() const 				{ return *static_cast<const V*>(this); }
		const type* 		data() const					{ return self_cast().data(); }
		int 				size() const 					{ return self_cast().size(); }
		const type& 		operator[](int i) const 		{ return self_cast()[i]; }
		
		V& 					self_cast() 					{ return *static_cast<V*>(this); }
		type* 				data()							{ return self_cast().data(); }
		type& 				operator[](int i) 				{ return self_cast()[i]; }
		void 				reset(int n, const type& val)	{ return self_cast().reset(n, val); }

		const_iterator 			begin() const 				{ return self_cast().begin(); };
		const_iterator const 	end() const 				{ return self_cast().end(); };
		iterator 				begin() 					{ return self_cast().begin(); };
		iterator const 			end() 						{ return self_cast().end(); };

		V& 						operator = (const vector_root& u) 		{ return self_cast() = u; }
		template<class U> V& 	operator = (const vector_root<U>& u) 	{ return self_cast() = u; }
	};

	template <class V>
	struct compact_vector_root : public vector_root<V>
	{
		using parent = vector_root<V>;
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;

		type* 		_data = nullptr;
		int 		_size = 0;
		int 					size() const				{ return _size; }
		type* 					data()						{ return _data; }
		const type* 			data() const				{ return _data; }
		type& 					operator[](int i) 			{ return _data[i]; }
		const type& 			operator[](int i) const 	{ return _data[i]; }

		iterator 				begin() 					{ return _data; }
		const_iterator 			begin() const 				{ return _data; }
		const iterator 			end() 						{ return _data + _size; }
		const const_iterator 	end() const 				{ return _data + _size; }

		compact_vector_root() 													{}
		compact_vector_root(type* data, int size) : _data(data), _size(size)	{}
	};

	template <class V>
	struct uncompact_vector_root : public vector_root<V>
	{
		using parent = vector_root<V>;
		using type = typename traits<V>::type;
		using iterator = typename traits<V>::iterator;
		using const_iterator = typename traits<V>::const_iterator;
		type* 	_data = nullptr;
		int 	_size = 0;
		int 	_step = 0;
		int 					size() const				{ return _size; }
		type* 					data()						{ return _data; }
		const type* 			data() const				{ return _data; }
		type& 					operator[](int i) 			{ return _data[i*_step]; }
		const type& 			operator[](int i) const 	{ return _data[i*_step]; }

		iterator 				begin() 					{ return {_data, _step}; }
		const_iterator 			begin() const 				{ return {_data, _step}; }
		const iterator 			end() 						{ return {_data + _size, _step}; }
		const const_iterator 	end() const 				{ return {_data + _size, _step}; }
	
		uncompact_vector_root() 																		{}
		uncompact_vector_root(type* data, int size, int step) : _data(data), _size(size), _step(step)	{}
	};

	template <class T, class A = std::allocator<T>>
	struct vecview : public compact_vector_root<vecview<T, A>>
	{
		using parent = compact_vector_root<vecview<T, A>>;
		vecview(T* data, int size) : parent(data, size) {}
	};

	template <class T, class A = std::allocator<T>>
	struct uncompact_vecview : public uncompact_vector_root<uncompact_vecview<T, A>>
	{
		using parent = uncompact_vector_root<uncompact_vecview<T, A>>;
		uncompact_vecview(T* data, int size, int step) : parent(data, size, step) {}
	};

	template <class T, class A = std::allocator<T>>
	struct vector : public compact_vector_root<vector<T, A>>, public vector_keeper<vector<T, A>>
	{
		using parent = compact_vector_root<vector<T, A>>;
		using keeper = vector_keeper<vector<T, A>>;
		vector()										{};
		vector(int size)								{ keeper::create(size); }
		vector(const T* data, int size)					{ keeper::create(size); std::copy(data, data+size, parent::_data); }
		vector(const std::initializer_list<T>& list)	{ keeper::create(list.size()); std::copy(list.begin(), list.end(), parent::_data); }
		void reset(int size, const T& val)				{ keeper::invalidate(); keeper::create(size); for (auto& ref : *this) ref = val; }

		vector& operator = (const vector& oth) 								{ if (&oth == this) return *this; keeper::resize(oth.size()); std::copy(oth.begin(), oth.end(), parent::_data); return *this; }
		vector& operator = (vector&& oth) 									{ parent::_data = oth._data; parent::_size = oth._size; oth._data = nullptr; oth._size = 0; return *this; }
		template <class V> vector& operator = (const vector_root<V>& oth) 	{ if (&oth == this) return *this; keeper::resize(oth.size()); std::copy(oth.begin(), oth.end(), parent::_data); return *this; }
		template <class V> vector& operator = (vector_root<V>&& oth) 		{ parent::_data = oth._data; parent::_size = oth._size; oth._data = nullptr; oth._size = 0; return *this; }
	};

	//Lexicographic compare
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