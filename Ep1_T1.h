#ifndef EP2_T1_H
#define EP2_T1_H
#include <type_traits>

namespace ep2 {
  template<typename OriginType, typename ReplacedType, typename ReplacerType>
  struct replace_type;

  // fixme: 不能正确替换指针和引用类型
  template<typename OriginType, typename ReplacedType, typename ReplacerType>
  struct replace_type
  {
    using type = typename std::conditional<std::is_same<OriginType, ReplacedType>::value, ReplacerType, OriginType>::type;
  };

  /*
   * TODO
   * c,x均为引用，向下传递c,x的引用性，否则只传递x的引用性
   * c,x均为指针，向下传递保留c,x的指针级数，否则只保留x的指针级数
   * c,x均为数组，向下传递保留c,x的数组类型，否则只保留x的数组类型
   * XXX 这种方案可能有多余的实例化
   */

  template<typename ReturnType, typename ReplacedType, typename ReplacerType, typename... ArgsType>
  struct replace_type<ReturnType(ArgsType...), ReplacedType, ReplacerType> // fixme: 无法正确处理以函数类型为返回值和参数的类型
  {
    // todo: 使用replace_type_t而非conditional_t 可行性待验证
    using type = typename std::conditional<std::is_same<ReturnType, ReplacedType>::value, ReplacerType, ReturnType>::type(
      typename std::conditional<std::is_same<ArgsType, ReplacedType>::value, ReplacerType, ArgsType>::type...
      );
  };

  template<typename OriginType, typename ReplacedType, typename ReplacerType>
  struct replace_type<OriginType[], ReplacedType, ReplacerType> : replace_type<OriginType, ReplacedType, ReplacerType>
  {
    using base_type = replace_type<OriginType, ReplacedType, ReplacerType>;
    using type = typename base_type::type[];
  };

  template<typename OriginType, typename ReplacedType, typename ReplacerType, std::size_t N>
  struct replace_type<OriginType[N], ReplacedType, ReplacerType> : replace_type<OriginType, ReplacedType, ReplacerType>
  {
    using base_type = replace_type<OriginType, ReplacedType, ReplacerType>;
    using type = typename base_type::type[N];
  };

  template<typename OriginType, typename ReplacedType, typename ReplacerType>
  struct replace_type<OriginType&, ReplacedType, ReplacerType>: replace_type<OriginType, ReplacedType, ReplacerType>
  {
    using base_type = replace_type<OriginType, ReplacedType, ReplacerType>;
    using type = typename base_type::type&;
  };

  template<typename OriginType, typename ReplacedType, typename ReplacerType>
  struct replace_type<OriginType*, ReplacedType, ReplacerType>: replace_type<OriginType, ReplacedType, ReplacerType>
  {
    using base_type = replace_type<OriginType, ReplacedType, ReplacerType>;
    using type = typename base_type::type*;
  };

  template<typename OriginType, typename ReplacedType, typename ReplacerType>
  struct replace_type<OriginType&&, ReplacedType, ReplacerType>: replace_type<OriginType, ReplacedType, ReplacerType>
  {
    using base_type = replace_type<OriginType, ReplacedType, ReplacerType>;
    using type = typename base_type::type&&;
  };
}

#endif //EP2_T1_H
