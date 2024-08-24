//
// Created by Xymul on 2024/8/22.
//

#ifndef XYMLIB_C__20_XYM_CONCEPTS_H
#define XYMLIB_C__20_XYM_CONCEPTS_H

#include <utility>

namespace xymlib {

  namespace _meta {
    template<typename = void>
    struct do_nothing
    {
    };

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
    {
    };

    template<typename T, typename ...Tr>
    struct type_tuple<T, Tr...> : type_tuple<Tr...>
    {
      using base_type = type_tuple<Tr...>;
      using type = T;
    };

    template<typename Tuple, std::size_t N>
    struct get_tuple_type
    {
      using type = get_tuple_type<typename Tuple::base_type, N - 1>::type;
    };

    template<typename Tuple>
    struct get_tuple_type<Tuple, 0>
    {
      using type = Tuple;
    };

    template<typename T>
    struct type_of_t;

    template<std::size_t N, typename ...Ts>
    struct type_conjunction;

    template<std::size_t N, typename ...Ts>
    struct type_disjunction;

    template<typename ...Ts>
    struct apply;

    /**
     * 对类型合取以指定方式使用运算，其等价于：<br/>
     *
     * <code>Apply\<type1\>::value && Apply\<type2\>::value && Apply\<type3\>::value && ...</code>
     * */
    template<template<class> typename Apply, std::size_t N, typename ...Ts>
    struct apply<Apply<type_conjunction<N, Ts...>>, type_conjunction<N, Ts...>>
      : apply<Apply<type_conjunction<N - 1, Ts...>>, type_conjunction<N - 1, Ts...>>
    {
      using base_type = apply<Apply<type_conjunction<N - 1, Ts...>>, type_conjunction<N - 1, Ts...>>;

      // N-1保证不会遇到tuple<>，而且会运算两遍type_tuple<0>，但是对结果无影响。
      static constexpr auto value = Apply<
        typename get_tuple_type<
          typename type_conjunction<N, Ts...>::type_list, N - 1>::type::type::type>::value && base_type::value;
      // get_tuple_type::type -> tuple_type
      // tuple_type::type -> type_of_t<T>
      // type_of_t<T>::type -> T
    };

    template<template<class> typename Apply, typename ...Ts>
    struct apply<Apply<type_conjunction<0, Ts...>>, type_conjunction<0, Ts...>>
    {
      static constexpr auto value = Apply<
        typename get_tuple_type<
          typename type_conjunction<0, Ts...>::type_list, 0>::type::type::type>::value;
    };

    template<template<class> typename Apply, std::size_t N, typename ...Ts>
    struct apply<Apply<type_disjunction<N, Ts...>>, type_disjunction<N, Ts...>>
      : apply<Apply<type_disjunction<N - 1, Ts...>>, type_disjunction<N - 1, Ts...>>
    {
      using base_type = apply<Apply<type_disjunction<N - 1, Ts...>>, type_disjunction<N - 1, Ts...>>;

      static constexpr auto value = Apply<
        typename get_tuple_type<
          typename type_disjunction<N, Ts...>::type_list, N - 1>::type::type::type>::value || base_type::value;
    };

    template<template<class> typename Apply, typename ...Ts>
    struct apply<Apply<type_disjunction<0, Ts...>>, type_disjunction<0, Ts...>>
    {
      static constexpr auto value = Apply<
        typename get_tuple_type<
          typename type_disjunction<0, Ts...>::type_list, 0>::type::type::type>::value;
    };

    template<template<class, class> typename Apply, typename T, std::size_t N, typename ...Ts>
    struct apply<Apply<T, type_conjunction<N, Ts...>>, type_conjunction<N, Ts...>>
      : apply<Apply<T, type_conjunction<N - 1, Ts...>>, type_conjunction<N - 1, Ts...>>
    {
      using base_type = apply<Apply<T, type_conjunction<N - 1, Ts...>>, type_conjunction<N - 1, Ts...>>;

      static constexpr auto value = Apply<
        T,
        typename get_tuple_type<
          typename type_conjunction<N, Ts...>::type_list, N - 1>::type::type::type>::value && base_type::value;
    };

    template<template<class, class> typename Apply, typename T, typename ...Ts>
    struct apply<Apply<T, type_conjunction<0, Ts...>>, type_conjunction<0, Ts...>>
    {
      static constexpr auto value = Apply<
        T,
        typename get_tuple_type<
          typename type_disjunction<0, Ts...>::type_list, 0>::type::type::type>::value;
    };

    template<template<class, class> typename Apply, typename T, std::size_t N, typename ...Ts>
    struct apply<Apply<T, type_disjunction<N, Ts...>>, type_disjunction<N, Ts...>>
      : apply<Apply<T, type_disjunction<N - 1, Ts...>>, type_disjunction<N - 1, Ts...>>
    {
      using base_type = apply<Apply<T, type_disjunction<N - 1, Ts...>>, type_disjunction<N - 1, Ts...>>;

      static constexpr auto value = Apply<
        T,
        typename get_tuple_type<
          typename type_disjunction<N, Ts...>::type_list, N - 1>::type::type::type>::value || base_type::value;
    };

    template<template<class, class> typename Apply, typename T, typename ...Ts>
    struct apply<Apply<T, type_disjunction<0, Ts...>>, type_disjunction<0, Ts...>>
    {
      static constexpr auto value = Apply<
        T,
        typename get_tuple_type<
          typename type_disjunction<0, Ts...>::type_list, 0>::type::type::type>::value;
    };

    template<template<class, class> typename Apply, typename T, std::size_t N, typename ...Ts>
    constexpr auto value_of(type_conjunction<N, Ts...>)
    {
      return apply<Apply<T, type_conjunction<N, Ts...>>, type_conjunction<N, Ts...>>::value;
    }

    template<template<class, class> typename Apply, typename T, std::size_t N, typename ...Ts>
    constexpr auto value_of(type_disjunction<N, Ts...>)
    {
      return apply<Apply<T, type_disjunction<N, Ts...>>, type_disjunction<N, Ts...>>::value;
    }

    template<template<class> typename Apply, std::size_t N, typename ...Ts>
    constexpr auto value_of(type_conjunction<N, Ts...>)
    {
      return type_conjunction<N, Ts...>::template value<Apply>;
    }

    template<template<class> typename Apply, std::size_t N, typename ...Ts>
    constexpr auto value_of(type_disjunction<N, Ts...>)
    {
      return type_disjunction<N, Ts...>::template value<Apply>;
    }

    /**
     * 对类型以指定运算进行合取
     * <br/>
     * <code>N=1</code>时<code>this_type::value</code>与<code>type_disjunction::value</code>相同
     *
     * @tparam N 所要合取的类型的数量
     * @tparam Ts 所要合取的所有类型
     * */
    template<std::size_t N, typename ...Ts>
    struct type_conjunction
    {

      using this_type = type_conjunction<N, Ts...>;
      using type_list = type_tuple<Ts...>;

      static constexpr std::size_t numbers = N;

      template<template<class> typename Apply>
      static constexpr auto value = apply<Apply<type_conjunction<N, Ts...>>, type_conjunction<N, Ts...>>::value;

      template<template<class> typename Apply>
      constexpr auto get_value()
      {
        return std::remove_cvref_t<decltype(*this)>::template value<Apply>;
      }

      template<std::size_t N_other, typename ...Ts_other>
      constexpr auto operator&&(type_conjunction<N_other, Ts_other...>)
      {
        return type_conjunction<N + N_other, Ts..., Ts_other...>{};
      }

      // (A && B && C) || (D || E || F) == (A && B && C)
      // (A && B && C) || (D) == (A && B && C)
      template<std::size_t N_other, typename ...Ts_other>
      constexpr auto operator||(type_disjunction<N_other, Ts_other...>)
      {
        return this_type{};
      }

      template<typename T>
      constexpr auto operator&&(type_of_t<T>)
      {
        return type_conjunction<N + 1, Ts..., type_of_t<T>>{};
      }
    };

    /**
     * 对类型以指定运算进行析取
     * <br/>
     * <code>N=1</code>时<code>this_type::value</code>与<code>type_conjunction::value</code>相同
     *
     * @tparam N 所要析取的类型的数量
     * @tparam Ts 所要析取的所有类型
     * */
    template<std::size_t N, typename ...Ts>
    struct type_disjunction
    {
      using this_type = type_disjunction<N, Ts...>;

      static constexpr std::size_t numbers = N;
      using type_list = type_tuple<Ts...>;

      /**
       * @tparam Apply 对每个类型的运算方式
       * */
      template<template<class> typename Apply>
      static constexpr auto value = apply<Apply<type_disjunction<N, Ts...>>, type_disjunction<N, Ts...>>::value;

      template<template<class> typename Apply>
      constexpr auto get_value()
      {
        return std::remove_cvref_t<decltype(*this)>::template value<Apply>;
      }

      template<std::size_t N_other, typename ...Ts_other>
      constexpr auto operator||(type_disjunction<N_other, Ts_other...>)
      {
        return type_disjunction<N + N_other, Ts..., Ts_other...>{};
      }

      // (A || B || C) || (D && E && F) == (D && E && F)
      // (A || B || C) || (D) == (A || B || C || D)
      template<std::size_t N_other, typename ...Ts_other>
      constexpr auto operator||(type_conjunction<N_other, Ts_other...>)
      {
        if constexpr (N_other > 1) {
          return type_conjunction<N_other, Ts_other...>{};
        } else {
          return type_disjunction<N_other + N_other, Ts..., Ts_other...>{};
        }
      }

      template<typename T>
      constexpr auto operator||(type_of_t<T>)
      {
        return type_disjunction<N + 1, Ts..., type_of_t<T>>{};
      }
    };

    template<typename T>
    struct type_of_t
    {
      using this_type = type_of_t<T>;

      using type = T;

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

    _meta::value_of<std::is_same, decltype(T::xym_serialize_impl::template name<N>())>(
      _meta::type_of<char> || _meta::type_of<const char*> || _meta::type_of<char* const>
    );
//    _meta::same_or<
//      decltype(T::xym_serialize_impl::template name<N>()),
//      char*, const char*, char* const
//    >::value;
  };
}

#endif //XYMLIB_C__20_XYM_CONCEPTS_H
