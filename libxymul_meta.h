#ifndef LIBXYMUL_META_H
#define LIBXYMUL_META_H

#include <tuple>

#include "libxymul_meta.h"
#include "libxymul_meta.h"
#include "libxymul_meta.h"
#include "libxymul_meta.h"
#include "libxymul_meta.h"
#include "libxymul_meta.h"
#include "libxymul_meta.h"

namespace xym {
  namespace meta {
    template<typename ...Ts>
    struct linked_list;

    namespace __impl {
      template<typename ...Ts>
      struct separate_first;

      template<typename In, typename ...Ts>
      struct separate_first<linked_list<>, In, Ts...>
      {
        using new_container = linked_list<Ts...>;
      };

      template<typename In, typename T, typename ...Ts>
      struct separate_first<linked_list<Ts...>, In, T, Ts...>
        : std::conditional_t<
          std::is_same_v<In, typename linked_list<Ts...>::element_type>,
          separate_first<linked_list<>, In, Ts...>,
          separate_first<linked_list<Ts...>, In, Ts...>
      >
      {
        using base_type = std::conditional_t<
                          std::is_same_v<In, typename linked_list<Ts...>::element_type>,
                          separate_first<linked_list<>, In, Ts...>,
                          separate_first<linked_list<Ts...>, In, Ts...>>;

        using new_container = typename base_type::new_container;
      };

      template<typename C, typename T, typename ...Ts>
      struct append;


      template<typename T, typename ...Ts>
      struct append<linked_list<Ts...>, T, Ts...>
      {
        using new_container = linked_list<Ts..., T>;
      };

      template<typename C, typename... Ts>
      struct minus;

      template<typename T, typename ...Ts>
      struct minus<linked_list<>, T, Ts...>
      {
        using new_container = typename append<linked_list<>, T>::new_container;
      };

      template<typename ...Os, typename T, typename ...Ts>
      struct minus<linked_list<Os...>, T, Ts...> : std::conditional_t<
        sizeof...(Os) == sizeof...(Ts),
        minus<linked_list<>, Ts...>,
        minus<linked_list<Os...>, Ts...>
      >
      {
        using base_type = std::conditional_t<
                          sizeof...(Os) == sizeof...(Ts),
                          minus<linked_list<>, Ts...>,
                          minus<linked_list<Os...>, Ts...>
                        >;

        using new_container = typename append<typename base_type::new_container, T>::new_container;
      };

      template<typename C1, typename C2, typename T, typename ...Ts>
      struct merge;

      template<typename ...Os, typename T, typename ...Ts>
      struct merge<linked_list<Os...>, linked_list<Ts...>, T>
      {
        using new_container = linked_list<Os..., T, Ts...>;
      };

    };


    template<>
    struct linked_list<>
    {
    };

    template<typename T, typename ...Ts>
    struct linked_list<T, Ts...> : linked_list<Ts...>
    {
      using element_type = T;
    };

    template<typename T, typename In, typename Container>
    struct insert_first;

    template<typename T, typename In, typename ...Ts>
    struct insert_first<T, In, linked_list<Ts...>>
    {
      using new_container = typename __impl::merge<
        typename __impl::separate_first<linked_list<Ts...>, In>::new_container,
        typename __impl::minus<typename __impl::separate_first<linked_list<Ts...>, In>::new_container, Ts...>::new_container,
        T
      >::new_container;
    };

  }
}

#endif //LIBXYMUL_META_H
