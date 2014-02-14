#pragma once

#ifdef _DEBUG
#include <iostream>
#endif

#ifdef _DEBUG
#define __DEBUG_OUT(str)           \
    std::cout << (str) << std::endl
#else
#define __DEBUG_OUT(str)
#endif

#ifdef _DEBUG
#define __DEBUG_OUT_NO_FLUSH(str)  \
    std::cout << (str)
#else
#define __DEBUG_OUT_NO_FLUSH(str)
#endif

#ifdef _DEBUG
#define __DEBUG_OUT_DO_FLUSH          \
    std::cout << std::endl
#else
#define __DEBUG_OUT_DO_FLUSH
#endif

#ifdef _DEBUG
#define __DEBUG_ERR(str)           \
    std::cerr << (str) << std::endl
#else
#define __DEBUG_ERR(str)
#endif
