/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#ifndef __BASE_DEFS_H__
#define __BASE_DEFS_H__

#ifdef __cplusplus 
extern "C" { 
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*******************************************
 * Compiler keyword definition
 * 编译器关键字定义
 *******************************************
 */

/* each compiler inline keyword definition */
/* 各编译器inline关键字定义 */
#if (defined(__ARMCC_VERSION) || defined(_MSC_VER)) && \
    !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif

/* each compiler asm keyword definition */
/* 各编译器asm关键字定义 */
#if defined(__GNUC__) && !defined(__asm)
#define __asm            __asm__
#endif /* __ASM */

/***********************************************
 * Basic data type definition
 * 基本数据类型定义
 ***********************************************
 */

/*
 * using stdint.h and stdbool.h
 * 使用stdint.h与stdbool.h
 */

/***************************************************
 * Common symbols and function definitions
 * 常用符号及功能定义
 ***************************************************
 */

/* calculate relative offsets of members by type, member name */
/* 通过类型、成员名计算成员的相对偏移 */
#ifndef offset_of
#define offset_of(type, member) (((const uint8_t *)&((type *)0)->member) - (const uint8_t *)0)
#endif

/* convert pointer to integer */
/* 将指针转换为整数 */
#ifndef ptr2uint
#define ptr2uint(ptr)  ((const uint8_t *)(ptr) - (const uint8_t *)0)
#endif

/* convert integer to pointer */
/* 将整数转换为指针 */
#ifndef uint2ptr
#define uint2ptr(i) ((void *)((uint8_t *)0 + (i)))
#endif

/* calculate relative offsets of members by pointers, member name */
/* 通过指针、成员名计算成员的相对偏移 */
#ifndef poffset_of
#define poffset_of(eptr, member) (((const uint8_t *)&(eptr)->member) - (const uint8_t *)(eptr))
#endif

/* calculate the address of the external structure by type, member name, and member pointer */
/* 通过类型、成员名、成员指针计算外部结构的地址 */
#ifndef container_of
#define container_of(mptr, type, member) ((type *)((const uint8_t *)(mptr) - (const uint8_t *)offset_of(type, member)))
#endif

/* calculate the address of the external structure by external structure pointers, member names, and member pointers */
/* 通过外部结构指针、成员名、成员指针计算外部结构的地址 */
#ifndef pcontainer_of
#define pcontainer_of(mptr, eptr, member) ((void *)((const uint8_t *)(mptr) - (const uint8_t *)poffset_of(eptr, member)))
#endif

/* unsigned 8, 16, 32, 64 bit integer maximum definition */
/* 无符号8、16、32、64位整数最大值定义 */
#undef UINT8_MAX
#undef UINT16_MAX
#undef UINT32_MAX
#undef UINT64_MAX

#define UINT8_MAX   0xFF
#define UINT16_MAX  0xFFFF
#define UINT32_MAX  0xFFFFFFFFu
#define UINT64_MAX  0xFFFFFFFFFFFFFFFFull

/* signed 8, 16, 32, 64 bit integer maximum definition */
/* 有符号8、16、32、64位整数最大值定义 */
#undef INT8_MAX
#undef INT16_MAX
#undef INT32_MAX
#undef INT64_MAX

#define INT8_MAX    0x7F
#define INT16_MAX   0x7FFF
#define INT32_MAX   0x7FFFFFFF
#define INT64_MAX   0x7FFFFFFFFFFFFFFFll

/* signed 8, 16, 32, 64 bit integer minimum definition */
/* 有符号8、16、32、64位整数最小值定义 */
#undef INT8_MIN
#undef INT16_MIN
#undef INT32_MIN
#undef INT64_MIN

#define INT8_MIN    (-INT8_MAX - 1)
#define INT16_MIN   (-INT16_MAX - 1)
#define INT32_MIN   (-INT32_MAX - 1)
#define INT64_MIN   (-INT64_MAX - 1ll)

/* returns the minimum value of two numbers
 * this macro must ensure that a, b remains unchanged when invoked
 */
/* 返回两个数的最小值
 * 此宏须保证a，b在调用时保持不变
 */
#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

/* returns the maximum value of two numbers
 * this macro must ensure that a, b remains unchanged when invoked
 */
/* 返回两个数的最大值
 * 此宏须保证a，b在调用时保持不变
 */
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* returns an 2 aligned upward integer */
/* 返回以2向上对齐的整数 */
#ifndef ALIGN2_UP
#define ALIGN2_UP(i)   (((i) + 1) & ~1)
#endif

/* returns an 2 aligned downward integer */
/* 返回以2向下对齐的整数 */
#ifndef ALIGN2_DOWN
#define ALIGN2_DOWN(i)   ((i) & ~1)
#endif

/* returns an 4 aligned upward integer */
/* 返回以4向上对齐的整数  */
#ifndef ALIGN4_UP
#define ALIGN4_UP(i)   (((i) + 3) & ~3)
#endif

/* returns an 4 aligned downward integer */
/* 返回以4向下对齐的整数  */
#ifndef ALIGN4_DOWN
#define ALIGN4_DOWN(i)   ((i) & ~3)
#endif

/* returns an 8 aligned upward integer */
/* 返回以8向上对齐的整数  */
#ifndef ALIGN8_UP
#define ALIGN8_UP(i)   (((i) + 7) & ~7)
#endif

/* returns an 8 aligned downward integer */
/* 返回以8向下对齐的整数  */
#ifndef ALIGN8_DOWN
#define ALIGN8_DOWN(i)   ((i) & ~7)
#endif

/* 返回CPU向上对齐的整数 */
#ifndef ALIGN_UP
#define ALIGN_UP(i)     (((i) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))
#endif

/* 返回CPU向下对齐的整数 */
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(i)   ((i) & ~(sizeof(void *) - 1))
#endif

/* returns an N aligned upward integer */
/* 返回以N向上对齐的整数  */
#ifndef ALIGNN_UP
#define ALIGNN_UP(N, i) (((((i) + (N) - 1) / N)) * N)
#endif

/* returns an N aligned downward integer */
/* 返回以N向下对齐的整数  */
#ifndef ALIGNN_DOWN
#define ALIGNN_DOWN(N, i) (((i) / N) * N)
#endif

/* returns the length of the array */
/* 返回数组的长度 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(*(arr)) ? (sizeof(arr) / sizeof(*(arr))) : 0)
#endif

/* null pointer */
/* 空指针 */
#ifndef NULL
#define NULL 0
#endif

#ifdef CONFIG_NULL_CB
/* empty callback function */
/* 空回调函数 */
extern void CONFIG_NULL_CB(void);
#define NULL_CB CONFIG_NULL_CB
#endif


/***************************************************
 * common function definition
 * 常用功能定义
 ***************************************************
 */

/*****************************************
 *@brief: memory copy
 *
 *@param: dst   destination address of dst memory copy
 *@param: src   source address of src memory copy
 *@param: size  copy memory size
 *@return: void* target address
 *****************************************/
/*****************************************
 *@简要：内存拷贝
 *
 *@参数 dst   内存拷贝的目标地址
 *@参数 src   内存拷贝的源地址
 *@参数 size  拷贝内存的大小
 *@返回值 void* 目标地址
 *****************************************/
static inline void *memcpy_spare(void *_dst, const void *_src, size_t size)
{
    struct __mem16 {
        uint8_t _mem[16];
    };
    struct __mem32 {
        uint8_t _mem32[32];
    };
    #define __MEM1(p)   (*((uint8_t *)(void *)(p)))
    #define __MEM2(p)  (*((uint16_t *)(void *)(p)))
    #define __MEM4(p)  (*((uint32_t *)(void *)(p)))
    #define __MEM8(p)  (*((uint64_t *)(void *)(p)))
    #define __MEM16(p) (*((struct __mem16 *)(void *)(p)))
    #define __MEM32(p) (*((struct __mem32 *)(void *)(p)))
    #define __CPU_SIZE (sizeof(void *))
    #define __CPU_MOD_SIZE (__CPU_SIZE - 1)

    size_t dst = (size_t)_dst;
    size_t src = (size_t)_src;
    size_t cpy_end = dst + size;;

    /* The source and target memory are not aligned, then copy in bytes */
    /* 源与目标内存未对齐，则按字节拷贝 */
    if ((((dst ^ src) & __CPU_MOD_SIZE) != 0) || size < __CPU_SIZE)
    {
        while (dst < cpy_end)
        {
            __MEM1(dst) = __MEM1(src);
            dst++;
            src++;
        }
    }
    else
    {
        while (dst < cpy_end)
        {
            if ((dst & __CPU_MOD_SIZE) == 0 && (dst + 8) <= cpy_end)
            {
                while (dst + 32 <= cpy_end)
                {
                    __MEM32(dst) = __MEM32(src);
                    dst += 32;
                    src += 32;
                }
                if (dst + 16 <= cpy_end)
                {
                    __MEM16(dst) = __MEM16(src);
                    dst += 16;
                    src += 16;
                }
                if (dst + 8 <= cpy_end)
                {
                    __MEM8(dst) = __MEM8(src);
                    dst += 8;
                    src += 8;
                }
            }
            if ((dst & 3) == 0 && (dst + 4) <= cpy_end)
            {
                __MEM4(dst) = __MEM4(src);
                dst += 4;
                src += 4;
            }
            else if ((dst & 1) == 0 && (dst + 2) <= cpy_end)
            {
                __MEM2(dst) = __MEM2(src);
                dst += 2;
                src += 2;
            }
            else
            {
                __MEM1(dst) = __MEM1(src);
                dst++;
                src++;
            }
        }
    }

    return _dst;

    #undef __MEM1
    #undef __MEM2
    #undef __MEM4
    #undef __MEM8
    #undef __MEM16
    #undef __MEM32
    #undef __CPU_MOD_SIZE
}


/*****************************************
 *@brief: set all bytes of size-size memory to the value of ch
 *
 *@param dst destination address for memory copy settings
 *@param ch content of memory byte unit
 *@param size sets the size of the memory
 *@return void* target memory address
 ******************************************/
/*****************************************
 *@简要：将size大小内存所有字节设置为ch的值
 *
 *@参数 mem   被填充的内存地址
 *@参数 ch    内存字节单元的内容
 *@参数 size  填充的大小
 *@返回值 void* 被填充的内存地址
 *****************************************/
static inline void *memset_spare(void *_mem, uint8_t ch, size_t size)
{
    union __mem16 
    {
        struct
        {
            uint32_t m4[2];
        } m8[2];
        uint64_t i8[2];
    };
    struct __mem32 
    {
        union __mem16 m16[2];
    };
    #define __MEM1(p)   (*((uint8_t *)(void *)(p)))
    #define __MEM4(p)  (*((uint32_t *)(void *)(p)))
    #define __MEM8(p)  (*((uint64_t *)(void *)(p)))
    #define __MEM16(p) (*((union __mem16 *)(void *)(p)))
    #define __MEM32(p) (*((struct __mem32 *)(void *)(p)))
    #define __CPU_SIZE  (sizeof(void *))
    #define __CPU_MOD_SIZE (sizeof(void *) - 1)

    struct __mem32 set32;

    size_t mem = (size_t)_mem;
    size_t set_end = mem + size;

    if (size < __CPU_SIZE)
    {
        while (mem < set_end)
        {
            __MEM1(mem) = ch;
            mem++;
        }
    }
    else
    {
        set32.m16[0].m8[0].m4[0] = 0x01010101 * ch;
        set32.m16[0].m8[0].m4[1] = set32.m16[0].m8[0].m4[0];
        set32.m16[0].m8[1] = set32.m16[0].m8[0];
        set32.m16[1] = set32.m16[0];

        while (mem < set_end)
        {
            if ((mem & __CPU_MOD_SIZE) == 0 && (mem + 8) <= set_end)
            {
                while (mem + 32 <= set_end)
                {
                    __MEM32(mem) = set32;
                    mem += 32;
                }
                if (mem + 16 <= set_end)
                {
                    __MEM16(mem) = set32.m16[0];
                    mem += 16;
                }
                if (mem + 8 <= set_end)
                {
                    __MEM8(mem) = set32.m16[0].i8[0];
                    mem += 8;
                }
            }
            if ((mem & 3) == 0 && (mem + 4) <= set_end)
            {
                __MEM4(mem) = set32.m16[0].m8[0].m4[0];
                mem += 4;
            }
            else
            {
                __MEM1(mem) = ch;
                mem++;
            }
        }
    }

    return _mem;

    #undef __MEM1
    #undef __MEM2
    #undef __MEM4
    #undef __MEM8
    #undef __MEM16
    #undef __MEM32
    #undef __CPU_MOD_SIZE
}


/*****************************************
 *@brief: comparing memory m1, m2 size before the size of bytes
 *
 *@param m1     memory m1
 *@param m2     memory m2
 *@param size   the length to be compared
 *@return int32_t
 *@return > 0 then m1 > m2
 *@return < 0 then m1 < m2
 *@return 0 then m1 == m2
 *****************************************/
/*****************************************
 *@简要：比较内存m1,m2前size字节大小
 *
 *@参数 m1    内存m1
 *@参数 m2    内存m2
 *@参数 size  要比较的长度
 *@返回值 int32_t
 *@返回 > 0 则m1 > m2
 *@返回 < 0 则m1 < m2
 *@返回 0 则m1 == m2
 *****************************************/
static inline int32_t memcmp_spare(const void *_m1, const void *_m2,
        size_t size)
{
    #define __MEM1(p)  (*((int8_t *)(void *)(p)))
    #define __MEM2(p)  (*((int16_t *)(void *)(p)))
    #define __MEM4(p)  (*((int32_t *)(void *)(p)))
    #define __MEM8(p)  (*((int64_t *)(void *)(p)))
    #define __CPU_SIZE  (sizeof(void *))
    #define __CPU_MOD_SIZE (__CPU_SIZE - 1)

    size_t m1 = (size_t)_m1;
    size_t m2 = (size_t)_m2;
    size_t cmp_end = m1 + size;

    /* The source and target memory are not aligned, then compare in bytes */
    /* 源与目标内存未对齐，则按字节比较 */
    if ((((m1 ^ m2) & __CPU_MOD_SIZE) != 0) || size < 4)
    {
        while (m1 < cmp_end)
        {
            if (__MEM1(m1) != __MEM1(m2))
            {
                return __MEM1(m1) - __MEM1(m2);
            }
            m1++;
            m2++;
        }
    }
    else
    {
        while (m1 < cmp_end)
        {
            if ((m1 & __CPU_MOD_SIZE) == 0)
            {
                while ((m1 + 8) <= cmp_end)
                {
                    if (__MEM8(m1) != __MEM8(m2))
                    {
                        if (__MEM4(m1) != __MEM4(m2))
                        {
                            return __MEM4(m1) - __MEM4(m2);
                        }
                        return __MEM4(m1 + 4) - __MEM4(m2 + 4);
                    }
                    m1 += 8;
                    m2 += 8;
                }
            }
            if ((m1 & 3) == 0 && (m1 + 4) <= cmp_end)
            {
                if (__MEM4(m1) != __MEM4(m2))
                {
                    return __MEM4(m1) - __MEM4(m2);
                }
                m1 += 4;
                m2 += 4;
            }
            else if ((m1 & 1) == 0 && (m1 + 2) <= cmp_end)
            {
                if (__MEM2(m1) != __MEM2(m2))
                {
                    return __MEM2(m1) - __MEM2(m2);
                }
                m1 += 2;
                m2 += 2;
            }
            else
            {
                if (__MEM1(m1) != __MEM1(m2))
                {
                    return __MEM1(m1) - __MEM1(m2);
                }
                m1++;
                m2++;
            }
        }
    }

    #undef __MEM1
    #undef __MEM2
    #undef __MEM4
    #undef __MEM8
    #undef __MEM16
    #undef __MEM32
    #undef __CPU_MOD_SIZE

    return 0;
}

/*****************************************
 *@brief: calculate the length of the string
 *
 *@param str        string
 *@return int32_t   string length
 *****************************************/
/*****************************************
 *@简要：计算字符串长度
 *
 *@参数 str       字符串
 *@返回值 int32_t 字符串长度
 *****************************************/
static inline size_t strlen_spare(const char *str)
{
    const char *old = str;

    while (*str++);

    return (str - old - 1);
}

/*****************************************
 *@brief: string copy
 *
 *@param dst  target string
 *@param str  source string
 *@return int32_t length of copy (length does not contain \0)
 *****************************************/
/*****************************************
 *@简要：字符串拷贝
 *
 *@参数 dst 目标字符串
 *@参数 str 源字符串
 *@返回值 int32_t 拷贝的长度（长度不包含\0）
 *****************************************/
static inline size_t strcpy_spare(char *dst, const char *src)
{
    char *old = dst;

    while ((*dst++ = *src++) != '\0');

    return (dst - old - 1);
}

/*****************************************
 *@brief: string security copy
 *
 *@param dst    target string
 *@param size   maximum copy length
 *@param str    source string
 *@return int32_t length of copy (length does not contain \0)
 *****************************************/
/*****************************************
 *@简要：字符串安全拷贝
 *
 *@参数 dst 目标字符串
 *@参数 size 最大拷贝长度
 *@参数 str 源字符串
 *@返回值 int32_t 拷贝的长度（长度不包含\0）
 *****************************************/
static inline size_t strncpy_spare(char *dst, const char *src, size_t size)
{
    char *dst_end = dst + size;
    char *dst_old = dst;

    while (dst < dst_end)
    {
        if ((*dst++ = *src++) == '\0')
        {
            return dst - dst_old - 1;
        }
    }

    return size;
}

/*****************************************
 *@brief: string comparison
 *
 *@param s1     string s1
 *@param s2     string s2
 *@return int32_t
 *@return > 0 then s1 > s2
 *@return < 0 then s1 < s2
 *@return 0 then s1 == s2
 *****************************************/
/*****************************************
 *@简要：字符串比较
 *
 *@参数 s1 字符串s1
 *@参数 s2 字符串s2
 *@返回值 int32_t
 *@返回 > 0 则s1 > s2
 *@返回 < 0 则s1 < s2
 *@返回 0 则s1 == s2
 *****************************************/
static inline int32_t strcmp_spare(const char *s1, const char *s2)
{
    while (*s1 == *s2)
    {
        if (*s1 == 0)
        {
            return 0;
        }
        s1++;
        s2++;
    }

    return *(int8_t *)s1 - *(int8_t *)s2;
}

/*****************************************
 *@brief: string security comparison
 *
 *@param s1       string s1
 *@param s2       string s2
 *@param size     maximum comparison length
 *@return int32_t
 *@return > 0 then s1 > s2
 *@return < 0 then s1 < s2
 *@return 0 then s1 == s2
 *****************************************/
/*****************************************
 *@简要：字符串安全比较
 *
 *@参数 s1 字符串s1
 *@参数 s2 字符串s2
 *@参数 size 最大比较长度
 *@返回值 int32_t
 *@返回 > 0 则s1 > s2
 *@返回 < 0 则s1 < s2
 *@返回 0 则s1 == s2
 *****************************************/
static inline int32_t strncmp_spare(const char *s1, const char *s2,
        size_t size)
{
    const char *s1_end = s1 + size;

    while (s1 < s1_end && *s1 == *s2)
    {
        if (*s1 == 0)
        {
            return 0;
        }
        s1++;
        s2++;
    }

    return (int32_t) *(int8_t *)s1 - (int32_t) *(int8_t *)s2;
}

#ifdef __cplusplus 
} 
#endif 
#endif /* __BASE_DEFS_H__ */
