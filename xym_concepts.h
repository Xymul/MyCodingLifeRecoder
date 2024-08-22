//
// Created by Xymul on 2024/8/22.
//

#ifndef XYMLIB_C__20_XYM_CONCEPTS_H
#define XYMLIB_C__20_XYM_CONCEPTS_H

#include <utility>

namespace xymlib {

  namespace _meta {
    template<typename T, typename ...Ts>
    struct same_or
    {
      static constexpr bool value = (std::is_same_v<T, Ts> || ...);
    };

    template<typename T1, typename T2>
    struct same_or<T1, T2>
    {
      static constexpr bool value = std::is_same_v<T1, T2>;
    };

    template<typename T>
    struct same_or<T>
    {
      static_assert(false, "`same_or` cannot have just one template argument.");
    };

    template<typename ...Ts>
    struct type_tuple;

    template<>
    struct type_tuple<>
    {};

    template<typename T, typename ...Tr>
    struct type_tuple<T, Tr...> : type_tuple<Tr...>
    {
      using base_type = type_tuple<Tr...>;
      using type = T;
    };

    template<typename Tuple, std::size_t N>
    struct get_type_in_tuple
    {
      using type = get_type_in_tuple<typename Tuple::base_type, N - 1>::type;
    };

    template<typename Tuple>
    struct get_type_in_tuple<Tuple, 0>
    {
      using type = Tuple;
    };

    template<std::size_t N, typename ...Ts>
    struct type_conjunction;

    template<std::size_t N, typename ...Ts>
    struct type_disjunction;

    template<typename ...Ts>
    struct apply;

    template<template<class> typename Apply, std::size_t N, typename ...Ts>
    struct apply<Apply<type_conjunction<N, Ts...>>, type_conjunction<N, Ts...>>
      : apply<Apply<type_conjunction<N - 1, Ts...>>, type_conjunction<N - 1, Ts...>>
    {
      using base_type = apply<Apply<type_conjunction<N - 1, Ts...>>, type_conjunction<N - 1, Ts...>>;

      static constexpr auto value = Apply<
        get_type_in_tuple<
          typename type_conjunction<N, Ts...>::type_list, N>>::value && base_type::value;
    };

    template<template<class> typename Apply, typename ...Ts>
    struct apply<Apply<type_conjunction<0, Ts...>>, type_conjunction<0, Ts...>>
    {
      static constexpr auto value = Apply<
        get_type_in_tuple<
          typename type_conjunction<0, Ts...>::type_list, 0>>::value;
    };

    template<template<class> typename Apply, std::size_t N, typename ...Ts>
    struct apply<Apply<type_disjunction<N, Ts...>>, type_disjunction<N, Ts...>>
      : apply<Apply<type_disjunction<N - 1, Ts...>>, type_disjunction<N - 1, Ts...>>
    {
      using base_type = apply<Apply<type_disjunction<N - 1, Ts...>>, type_disjunction<N - 1, Ts...>>;

      static constexpr auto value = Apply<
        get_type_in_tuple<
          typename type_disjunction<N, Ts...>::type_list, N>>::value || base_type::value;
    };

    template<template<class> typename Apply, typename ...Ts>
    struct apply<Apply<type_disjunction<0, Ts...>>, type_disjunction<0, Ts...>>
    {
      static constexpr auto value = Apply<
        get_type_in_tuple<
          typename type_disjunction<0, Ts...>::type_list, 0>>::value;
    };

    template<std::size_t N, typename ...Ts>
    struct type_conjunction
    {
      static constexpr std::size_t numbers = N;
      using type_list = type_tuple<Ts...>;

      template<template<class> typename Apply>
      static constexpr auto value = apply<Apply<type_conjunction<N, Ts...>>, type_conjunction<N, Ts...>>::value;

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator||(type_disjunction<N_other, Tss...> other)
      {
        return type_disjunction<N, Tss...>::value || value<Apply>;
      }

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator||(type_conjunction<N_other, Tss...> other)
      {
        return type_conjunction<N, Tss...>::value || value<Apply>;
      }

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator&&(type_disjunction<N_other, Tss...> other)
      {
        return type_disjunction<N, Tss...>::value && value<Apply>;
      }

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator&&(type_conjunction<N_other, Tss...> other)
      {
        return type_conjunction<N, Tss...>::value && value<Apply>;
      }
    };

    template<std::size_t N, typename ...Ts>
    struct type_disjunction
    {
      static constexpr std::size_t numbers = N;
      using type_list = type_tuple<Ts...>;

      template<template<class> typename Apply>
      static constexpr auto value = apply<Apply<type_disjunction<N, Ts...>>, type_disjunction<N, Ts...>>::value;

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator||(type_disjunction<N_other, Tss...> other)
      {
        return type_disjunction<N, Tss...>::value || value<Apply>;
      }

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator||(type_conjunction<N_other, Tss...> other)
      {
        return type_conjunction<N, Tss...>::value || value<Apply>;
      }

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator&&(type_disjunction<N_other, Tss...> other)
      {
        return type_disjunction<N, Tss...>::value && value<Apply>;
      }

      template<template<class> typename Apply, std::size_t N_other, typename ...Tss>
      auto operator&&(type_conjunction<N_other, Tss...> other)
      {
        return type_conjunction<N, Tss...>::value && value<Apply>;
      }
    };

    template<typename T>
    struct type_of_t
    {
      using this_type = type_of_t<T>;

      template<typename U>
      constexpr type_conjunction<2, this_type, type_of_t<U>>
      operator&&(type_of_t<U> other)
      {
        return {};
      }

      template<typename U>
      constexpr type_disjunction<2, this_type, type_of_t<U>>
      operator||(type_of_t<U> other)
      {
        return {};
      }
    };

    template<typename T> type_of_t<T> type_of{};
  }

  template<typename F, typename ...Args>
  concept callable = requires(F func, Args&& ... args) {
    func(std::forward<Args&&>(args)...);
  };

  template<typename T, std::size_t N>
  concept serializable = requires() {
    T::xym_serializable;
    T::xym_serialize_impl;

    T::xym_serialize_impl::template get<N>();
    T::xym_serialize_impl::template name<N>();

    _meta::same_or<
      decltype(T::xym_serialize_impl::template name<N>()),
      char*, const char*, char* const
    >::value;
  }; // todo
}

#endif //XYMLIB_C__20_XYM_CONCEPTS_H
