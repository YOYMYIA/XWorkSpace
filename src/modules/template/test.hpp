#ifndef __TEMPLATE_TEST_H__
#define __TEMPLATE_TEST_H__

#include "tools/DebugLog.h"

template<typename T>
T add(const T& a, const T& b)
{
    DBG_LOGD("hello, add!");
    return a + b;
}

#endif
