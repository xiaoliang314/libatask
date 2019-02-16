/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#ifndef __VA_NARGS_H__
#define __VA_NARGS_H__

#undef VA_NARGS
#undef __VA_NARGS

#define VA_NARGS_EXPAND(x)  x

/*********************************************************
 *@brief: 
 ***macro for counting the number of parameter list parameters
 *
 *@parameter:
 *[...]: parameter list
 *
 *@return: number of parameters in the parameter list
 *********************************************************/
/*********************************************************
 *@简要：
 ***用于计算参数列表参数个数的宏
 *
 *@参数：
 *[...]：参数列表
 *
 *@返回：参数列表中的参数个数
 **********************************************************/
#define VA_NARGS(...)    VA_NARGS_EXPAND(__VA_NARGS(0, ##__VA_ARGS__,\
                                    50,  49,  48, \
                                    47,  46,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32, \
                                    31,  30,  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16, \
                                    15,  14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2,   1,   0))

#define __VA_NARGS(   _0,   _1,   _2,   _3,   _4,   _5,   _6,   _7,   _8,   _9,  _10,  _11,  _12,  _13,  _14,  _15, \
                     _16,  _17,  _18,  _19,  _20,  _21,  _22,  _23,  _24,  _25,  _26,  _27,  _28,  _29,  _30,  _31, \
                     _32,  _33,  _34,  _35,  _36,  _37,  _38,  _39,  _40,  _41,  _42,  _43,  _44,  _45,  _46,  _47, \
                     _48,  _49,  _50, N, ...) N

/*********************************************************
 *@brief: 
 ***first expand the two macros completely, then link
 *
 *@parameter:
 *[x]: first macro
 *[y]: second macro
 *
 *@return: linked macro
 *********************************************************/
/*********************************************************
 *@简要：
 ***先将两个宏完全展开，再进行链接
 *
 *@参数：
 *[x]：第一个宏
 *[y]：第二个宏
 *
 *@返回：链接之后的宏
 **********************************************************/
#define MACRO_CAT_2(x, y)  __MACRO_CAT_2(x, y)
#define __MACRO_CAT_2(x, y) x##y

/*********************************************************
 *@brief: 
 ***the corresponding function is automatically called according to the number of parameters,
 ***assume ** is the name of the function by input,
 ***the **0 function will be called on 0 parameters,
 ***the **1 function will be called on 1 parameters,
 *
 *@parameter:
 *[func]: the prefix of the called function
 *[...]: "func" parameter list
 *********************************************************/
/*********************************************************
 *@简要：
 ***根据参数个数自动调用相应的函数，
 **假设**为传入的函数名
 ***0个参数时将调用**0函数，
 ***1个参数时将调用**1函数
 *
 *@参数：
 *[func]：被调用的函数的前缀
 *[...]: func的参数列表
 **********************************************************/
#define VA_ARGS_FUNC(func, ...) VA_NARGS_EXPAND(MACRO_CAT_2(func, VA_NARGS(__VA_ARGS__))(__VA_ARGS__))

#endif /* __VA_NARGS_H__ */
