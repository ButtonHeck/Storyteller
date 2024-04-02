#pragma once

#define STRTLR_M_CONCAT_(x, y) x ## y
#define STRTLR_M_CONCAT(x, y) STRTLR_M_CONCAT_(x, y)
#define STRTLR_M_STRINGIFY_(x) #x
#define STRTLR_M_STRINGIFY(x) STRTLR_M_STRINGIFY_(x)

#define STRTLR_M_NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, TOTAL, ...) TOTAL
#define STRTLR_M_NUM_ARGS(...) STRTLR_M_NUM_ARGS_(__VA_ARGS__, 12_, 11_, 10_, 9_, 8_, 7_, 6_, 5_, 4_, 3_, 2_, 1_)
#define STRTLR_M_DISPATCH_VA(macro, ...) STRTLR_M_CONCAT(macro, STRTLR_M_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__) 
