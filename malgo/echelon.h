#ifndef MALGO_ECHELON_H
#define MALGO_ECHELON_H

namespace malgo
{
	int echelonsize(int n) { return (n * (n + 1)) >> 1; }

	template <class T, class A> struct traits<echelon_view<T, A>>
	{
		using type = T;
		using alloc = A;
	};

	template <class T, class A> struct traits<echelon<T, A>>
	{
		using type = T;
		using alloc = A;
	};

	template<class E>
	struct eroot
	{
		const E& 	self_cast() const 	{ return *static_cast<const E*>(this); }
		E& 			self_cast() 		{ return *static_cast<E*>(this); }
		type_t<E>& 	elem(int i) 		{ return self_cast().elem(i); }
		const type_t<E>& 	elem(int i)  const		{ return self_cast().elem(i); }
		type_t<E>* 	data() 				{ return self_cast().data(); }
		const type_t<E>* 	data() const				{ return self_cast().data(); }
		int 		size() const 		{ return self_cast().size(); }
		int 		dim() const 		{ return self_cast().dim(); }
	};

	template<class T, class A = std::allocator<T>>
	struct echelon_view : public eroot<echelon_view<T, A>>
	{
		T* _data = nullptr;
		int _size = 0;
		int _dim = 0;

		echelon_view() {}
		echelon_view(T* data, int dim) : _data(data), _dim(dim), _size(echelonsize(dim)) {}

		T& elem(int i) 	{ return _data[i]; }
		const T& elem(int i) const	{ return _data[i]; }
		T* data() 		{ return _data; }
		const T* data() const		{ return _data; }
		int 		size() const 		{ return _size; }
		int 		dim() const 		{ return _dim; }
	};

	template<class T, class A = std::allocator<T>>
	struct echelon : public eroot<echelon<T, A>>, public vector_keeper<echelon<T, A>>
	{
		T* _data = nullptr;
		int _size = 0;
		int _dim = 0;

		T& 			elem(int i) 		{ return _data[i]; }
		const T& 	elem(int i) const	{ return _data[i]; }
		T*			data()	 			{ return _data; }
		const T*			data() const	 			{ return _data; }
		int 		size() const 		{ return _size; }
		int 		dim() const 		{ return _dim; }

		using parent = eroot<echelon<T, A>>;
		using keeper = vector_keeper<echelon<T, A>>;
		echelon(int dim) { _dim = dim; keeper::create(echelonsize(dim)); }
	};
}

template<class C, class M> std::basic_ostream<C> &
operator << (std::basic_ostream<C> & out, const malgo::eroot<M> & m)
{
	int k = 0;

	out << '{';
	for (int i = 0; i < m.dim() - 1; ++i)
	{
		out << '{';
		for (int j = i; j < m.dim() - 1; ++j)
		{
			out << m.elem(k++) << ',';
		}
		out << m.elem(k++) << '}' << ',';
	}
	out << '{' << m.elem(k) << '}' << '}';
}

#endif