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
			using vecout = vecview<T,A>;
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
	struct vroot
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

		V& 						operator = (const vroot& u) 		{ return self_cast() = u; }
		template<class U> V& 	operator = (const vroot<U>& u) 	{ return self_cast() = u; }
	};

	template <class V>
	struct compact_vroot : public vroot<V>
	{
		using parent = vroot<V>;
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

		operator vecview<type_t<V>, alloc_t<V>>() { return {_data, _size}; }

		template <class W> V& operator = (const vroot<W>& oth) 	{ if ((void*)&oth == (void*)this) return (V&)*this; std::copy(oth.begin(), oth.end(), _data); return (V&)*this; }

		compact_vroot() 					{}
		compact_vroot(type* data, int size) : _data(data), _size(size)	{}
	};

	template <class V>
	struct uncompact_vroot : public vroot<V>
	{
		using parent = vroot<V>;
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
	
		uncompact_vroot() 																		{}
		uncompact_vroot(type* data, int size, int step) : _data(data), _size(size), _step(step)	{}
	};

	template <class T, class A = std::allocator<T>>
	struct vecview : public compact_vroot<vecview<T, A>>
	{
		using parent = compact_vroot<vecview<T, A>>;
		using parent::operator=;
		vecview(T* data, int size) : parent(data, size) {}
	};

	template <class T, class A = std::allocator<T>>
	struct uncompact_vecview : public uncompact_vroot<uncompact_vecview<T, A>>
	{
		using parent = uncompact_vroot<uncompact_vecview<T, A>>;
		uncompact_vecview(T* data, int size, int step) : parent(data, size, step) {}
	};

	template <class T, class A = std::allocator<T>>
	struct vector : public compact_vroot<vector<T, A>>, public vector_keeper<vector<T, A>>
	{
		using parent = compact_vroot<vector<T, A>>;
		using keeper = vector_keeper<vector<T, A>>;
		vector()										{};
		vector(vector&& oth)							: parent(oth._data, oth._size) { oth._size = 0; oth._data = nullptr; };
		vector(int size)								{ keeper::create(size); }
		vector(const T* data, int size)					{ keeper::create(size); std::copy(data, data+size, parent::_data); }
		vector(const std::initializer_list<T>& list)	{ keeper::create(list.size()); std::copy(list.begin(), list.end(), parent::_data); }
		vector(const vector& oth) { keeper::create(oth.size()); T* ptr = parent::_data; const T*mptr = oth.data(); for (int i = 0; i < parent::size(); ++i) *ptr++ = *mptr++; }
		void reset(int size, const T& val)				{ keeper::invalidate(); keeper::create(size); for (auto& ref : *this) ref = val; }

		vector& operator = (const vector& oth) 								{ if (&oth == this) return *this; keeper::resize(oth.size()); std::copy(oth.begin(), oth.end(), parent::_data); return *this; }
		vector& operator = (vector&& oth) 									{ parent::_data = oth._data; parent::_size = oth._size; oth._data = nullptr; oth._size = 0; return *this; }
		template <class V> vector& operator = (const vroot<V>& oth) 	{ if (&oth == this) return *this; keeper::resize(oth.size()); std::copy(oth.begin(), oth.end(), parent::_data); return *this; }
		template <class V> vector& operator = (vroot<V>&& oth) 		{ parent::_data = oth._data; parent::_size = oth._size; oth._data = nullptr; oth._size = 0; return *this; }
	};

	template<class F, class A, class B> using vxv_apply_t = vector<ret_t<F, type_t<A>, type_t<B>>, alloc_t<A>>;
	template<class F, class A, class B> using sxv_apply_t = vector<ret_t<F, A, type_t<B>>, alloc_t<A>>;
	template<class F, class A, class B> using vxs_apply_t = vector<ret_t<F, type_t<A>, B>, alloc_t<A>>;
	template<class F, class A, class B> vxv_apply_t<F,A,B> elementwise(F&& f, const vroot<A>& a, const vroot<B>& b) 	{ vxv_apply_t<F,A,B> res(a.size()); for (int i = 0; i < a.size(); ++i) res[i] = f(a[i], b[i]); 	return res; }
	template<class F, class A, class B> sxv_apply_t<F,A,B> sxv_elementwise(F&& f, const A& a, const vroot<B>& b) 		{ sxv_apply_t<F,A,B> res(b.size()); for (int i = 0; i < b.size(); ++i) res[i] = f(a, 	 b[i]); return res; }
	template<class F, class A, class B> vxs_apply_t<F,A,B> vxs_elementwise(F&& f, const vroot<A>& a, const B& b) 		{ vxs_apply_t<F,A,B> res(a.size()); for (int i = 0; i < a.size(); ++i) res[i] = f(a[i], b); 	return res; }

	//template<class F, class A, class B> A& self_elementwise(F&& f, vroot<A>& a, const vroot<B>& b) 	{ for (int i = 0; i < a.size(); ++i) a[i] = f(a[i], b[i]); 	return res; }
	
	//Lexicographic compare
	template<class V, class W> compare_t<V,W> compare(const vroot<V>& a, const vroot<W>& b) { if (a.size() != b.size()) return a.size() - b.size(); for (int i = 0; i < a.size(); ++i) if (a[i] != b[i]) return a[i] - b[i]; return 0; }	
	template<class A, class B> auto operator == (const vroot<A> & a, const vroot<B> & b) -> decltype(compare(a,b) == 0) { return compare(a,b) == 0; }
	template<class A, class B> auto operator != (const vroot<A> & a, const vroot<B> & b) -> decltype(compare(a,b) != 0) { return compare(a,b) != 0; }
	template<class A, class B> auto operator <  (const vroot<A> & a, const vroot<B> & b) -> decltype(compare(a,b) <  0) { return compare(a,b) <  0; }
	template<class A, class B> auto operator >  (const vroot<A> & a, const vroot<B> & b) -> decltype(compare(a,b) >  0) { return compare(a,b) >  0; }
	template<class A, class B> auto operator <= (const vroot<A> & a, const vroot<B> & b) -> decltype(compare(a,b) <= 0) { return compare(a,b) <= 0; }
	template<class A, class B> auto operator >= (const vroot<A> & a, const vroot<B> & b) -> decltype(compare(a,b) >= 0) { return compare(a,b) >= 0; }

	template<class F, class V, class A> A fold(F f, A a, const vroot<V> & b) { for (const auto& r : b) { a = f(a, r); } return a; }

	template<class V, class W> vxv_apply_t<detail::op_add,V,W> operator + (const vroot<V>& a, const vroot<W>& b) { return elementwise(detail::op_add{}, a, b); }
	template<class V, class W> vxv_apply_t<detail::op_sub,V,W> operator - (const vroot<V>& a, const vroot<W>& b) { return elementwise(detail::op_sub{}, a, b); }
	template<class V, class W> vxv_apply_t<detail::op_mul,V,W> operator * (const vroot<V>& a, const vroot<W>& b) { return elementwise(detail::op_mul{}, a, b); }
	template<class V, class W> sxv_apply_t<detail::op_mul,V,W> operator * (const V& a, const vroot<W>& b) { return sxv_elementwise(detail::op_mul{}, a, b); }
	template<class V, class W> vxv_apply_t<detail::op_div,V,W> operator / (const vroot<V>& a, const vroot<W>& b) { return elementwise(detail::op_div{}, a, b); }

	template<class V, class S> vxs_apply_t<detail::op_mul,V,S> operator * (const vroot<V>& a, S b) { return vxs_elementwise(detail::op_mul{}, a, b); }
	template<class V, class S> vxs_apply_t<detail::op_div,V,S> operator / (const vroot<V>& a, S b) { return vxs_elementwise(detail::op_div{}, a, b); }

	//template<class V, class W> V& operator += (vroot& self, const vroot<W>& b) { return self_elementwise(detail::op_sum{}, a, b); }

	// Reduction functions on vectors
	template<class V> bool any (const vroot<V> & a) { return fold(detail::op_or{}, false, a); }
	template<class V> bool all (const vroot<V> & a) { return fold(detail::op_and{}, true, a); }
	template<class V> typename traits<V>::type sum    (const vroot<V> & a) { return fold(detail::op_add{}, typename traits<V>::type(0), a); }
	template<class V> typename traits<V>::type product(const vroot<V> & a) { return fold(detail::op_mul{}, typename traits<V>::type(1), a); }
	template<class V> typename traits<V>::type minelem(const vroot<V> & a) { return fold(detail::min{}, a[0], a); }
	template<class V> typename traits<V>::type maxelem(const vroot<V> & a) { return fold(detail::max{}, a[0], a); }

	template<class V, class W> typename traits<V>::type dot(const vroot<V>& a, const vroot<W>& b) 	{ return sum(a * b); }
	template<class V> typename traits<V>::type 			length2(const vroot<V> & a) 						{ return dot(a, a); }
	template<class V> typename traits<V>::type 			length(const vroot<V> & a) 						{ return std::sqrt(length2(a)); }

    template<class V> 
    vector_t<V> normalize(const vroot<V>& a) 
    { 
    	return a / length(a); 
    }

    template<class V> 
    void self_normalize(vroot<V>& a) 
    { 
    	auto l = length(a);
    	for (auto& r : a) r /= l;  
    }
}

template<class C, class V> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::vroot<V> & v)
{
	out << '{';
	for (int i = 0; i < v.size() - 1; ++i)
		out << v[i] << ',';
	out << v[v.size() - 1] << '}';
	return out;
}

#endif