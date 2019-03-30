/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#include "../lib/atask.h"
#include <time.h>

/* get the current time, unit is number of clocks */
/* 获取当前时间时钟数 */
time_nclk_t time_nclk_get(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    return ((time_nclk_t)tp.tv_sec * 1000000000) + tp.tv_nsec;
}


/* get the current time, unit is millisecond */
/* 获取当前时间微秒数 */
time_us_t time_us_get(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    return ((time_nclk_t)tp.tv_sec * 1000000) + (tp.tv_nsec / 1000);
}


/* convert the clocks to microseconds */
/* 将时钟数转为微秒 */
time_us_t time_nclk_to_us(time_nclk_t time_nclk)
{
    return time_nclk / 1000;
}


/* convert the microseconds to clocks */
/* 将微秒转为时钟数 */
time_nclk_t time_us_to_nclk(time_us_t time_us)
{
    return time_us * 1000;
}

void el_schedule_prepare(void)
{
}
