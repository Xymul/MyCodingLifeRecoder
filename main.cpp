#include <iostream>
#include <typeinfo>

#include "xym_concepts.h"

using namespace xymlib;
using namespace xymlib::_meta;

int main()
{
  // 理想：pred_impl<type<A> && type<B>>::value
  // 现实：
  std::cout << decltype(type_of<int> && type_of<double>)::value<std::is_trivial>;
  // 由简化繁了属于是

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
