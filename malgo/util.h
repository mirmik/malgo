#ifndef MALGO_UTILCLASSES_H
#define MALGO_UTILCLASSES_H

#include <cmath>

namespace malgo
{
	template <typename A> constexpr A max(A a, A b) { return a > b ? a : b; } 
	template <typename A> constexpr A min(A a, A b) { return a < b ? a : b; }
	template <typename A> constexpr void swap(A& a, A& b) { A tmp = a; a = b; b = tmp; }  
	template <typename A> constexpr A abs(A a) { return a >= 0 ? a : -a; }

	template <class A> struct traits;
	template <class A> using type_t = typename traits<A>::type;
	template <class A> using alloc_t = typename traits<A>::alloc;
	template <class A> using vector_t = typename traits<A>::vector;
	template <class A> using matrix_t = typename traits<A>::matrix;

	template <class V>
	struct vector_keeper
	{
		typename traits<V>::alloc alloc;

		V& self_cast() { return *static_cast<V*>(this); }

		void create(size_t n)
		{
			auto& self = self_cast();
			self._data = alloc.allocate(n);
			self._size = n;
		}
	};

	struct rowmajor {};
	struct colmajor {};

	template <class M> struct matrix_keeper
	{
		typename traits<M>::alloc alloc;

		M& self_cast() { return *static_cast<M*>(this); }

		void create(int size1, int size2)
		{
			auto& self = self_cast();
			self._data = alloc.allocate(size1 * size2);
			self._size1 = size1;
			self._size2 = size2;
		}
	};



	namespace detail
	{
		// Function objects for selecting between alternatives
		struct min    { template<class A, class B> constexpr auto operator() (A a, B b) const -> typename std::remove_reference < decltype(a < b ? a : b) >::type { return a < b ? a : b; } };
		struct max    { template<class A, class B> constexpr auto operator() (A a, B b) const -> typename std::remove_reference < decltype(a < b ? b : a) >::type { return a < b ? b : a; } };
		struct clamp  { template<class A, class B, class C> constexpr auto operator() (A a, B b, C c) const -> typename std::remove_reference < decltype(a < b ? b : a < c ? a : c) >::type { return a < b ? b : a < c ? a : c; } };
		struct select { template<class A, class B, class C> constexpr auto operator() (A a, B b, C c) const -> typename std::remove_reference < decltype(a ? b : c) >::type             { return a ? b : c; } };
		struct lerp   { template<class A, class B, class C> constexpr auto operator() (A a, B b, C c) const -> decltype(a * (1 - c) + b*c)                                               { return a * (1 - c) + b * c; } };

		// Function objects for applying operators
		struct op_pos { template<class A> constexpr auto operator() (A a) const -> decltype(+a) { return +a; } };
		struct op_neg { template<class A> constexpr auto operator() (A a) const -> decltype(-a) { return -a; } };
		struct op_not { template<class A> constexpr auto operator() (A a) const -> decltype(!a) { return !a; } };
		struct op_cmp { template<class A> constexpr auto operator() (A a) const -> decltype(~(a)) { return ~a; } };
		struct op_mul { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a * b)  { return a * b; } };
		struct op_div { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a / b)  { return a / b; } };
		struct op_mod { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a % b)  { return a % b; } };
		struct op_add { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a + b)  { return a + b; } };
		struct op_sub { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a - b)  { return a - b; } };
		struct op_lsh { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a << b) { return a << b; } };
		struct op_rsh { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a >> b) { return a >> b; } };
		struct op_lt  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a < b)  { return a < b; } };
		struct op_gt  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a > b)  { return a > b; } };
		struct op_le  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a <= b) { return a <= b; } };
		struct op_ge  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a >= b) { return a >= b; } };
		struct op_eq  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a == b) { return a == b; } };
		struct op_ne  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a != b) { return a != b; } };
		struct op_int { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a & b)  { return a & b; } };
		struct op_xor { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a ^ b)  { return a ^ b; } };
		struct op_un  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a | b)  { return a | b; } };
		struct op_and { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a && b) { return a && b; } };
		struct op_or  { template<class A, class B> constexpr auto operator() (A a, B b) const -> decltype(a || b) { return a || b; } };

		// Function objects for applying standard library math functions
		struct std_abs      { template<class A> auto operator() (A a) const -> decltype(std::abs  (a)) { return std::abs  (a); } };
		struct std_floor    { template<class A> auto operator() (A a) const -> decltype(std::floor(a)) { return std::floor(a); } };
		struct std_ceil     { template<class A> auto operator() (A a) const -> decltype(std::ceil (a)) { return std::ceil (a); } };
		struct std_exp      { template<class A> auto operator() (A a) const -> decltype(std::exp  (a)) { return std::exp  (a); } };
		struct std_log      { template<class A> auto operator() (A a) const -> decltype(std::log  (a)) { return std::log  (a); } };
		struct std_log10    { template<class A> auto operator() (A a) const -> decltype(std::log10(a)) { return std::log10(a); } };
		struct std_sqrt     { template<class A> auto operator() (A a) const -> decltype(std::sqrt (a)) { return std::sqrt (a); } };
		struct std_sin      { template<class A> auto operator() (A a) const -> decltype(std::sin  (a)) { return std::sin  (a); } };
		struct std_cos      { template<class A> auto operator() (A a) const -> decltype(std::cos  (a)) { return std::cos  (a); } };
		struct std_tan      { template<class A> auto operator() (A a) const -> decltype(std::tan  (a)) { return std::tan  (a); } };
		struct std_asin     { template<class A> auto operator() (A a) const -> decltype(std::asin (a)) { return std::asin (a); } };
		struct std_acos     { template<class A> auto operator() (A a) const -> decltype(std::acos (a)) { return std::acos (a); } };
		struct std_atan     { template<class A> auto operator() (A a) const -> decltype(std::atan (a)) { return std::atan (a); } };
		struct std_sinh     { template<class A> auto operator() (A a) const -> decltype(std::sinh (a)) { return std::sinh (a); } };
		struct std_cosh     { template<class A> auto operator() (A a) const -> decltype(std::cosh (a)) { return std::cosh (a); } };
		struct std_tanh     { template<class A> auto operator() (A a) const -> decltype(std::tanh (a)) { return std::tanh (a); } };
		struct std_round    { template<class A> auto operator() (A a) const -> decltype(std::round(a)) { return std::round(a); } };
		struct std_fmod     { template<class A, class B> auto operator() (A a, B b) const -> decltype(std::fmod    (a, b)) { return std::fmod    (a, b); } };
		struct std_pow      { template<class A, class B> auto operator() (A a, B b) const -> decltype(std::pow     (a, b)) { return std::pow     (a, b); } };
		struct std_atan2    { template<class A, class B> auto operator() (A a, B b) const -> decltype(std::atan2   (a, b)) { return std::atan2   (a, b); } };
		struct std_copysign { template<class A, class B> auto operator() (A a, B b) const -> decltype(std::copysign(a, b)) { return std::copysign(a, b); } };
	}
}

#endif