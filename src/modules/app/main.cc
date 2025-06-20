#include <iostream>
#include "../tools/DebugLog.h"
#include "../template/test.hpp"
#include "../template/mystl/type_traits.hpp"

int main() {
    std::cout << "Hello, XWorkSpace!" << std::endl;
    std::cout << add(123 , 134) << std::endl;
    myTpl::pair<int, float> st;
    DBG_LOGD("hello, XWorkSpace!") ;
    return 0;
}
