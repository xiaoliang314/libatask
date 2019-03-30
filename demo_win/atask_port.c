/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#include "../lib/atask.h"
#include <windows.h>

static LARGE_INTEGER freq = { 0 };

/* get the current time, unit is number of clocks */
/* 获取当前时间时钟数 */
time_nclk_t time_nclk_get(void)
{
    LARGE_INTEGER time;

    QueryPerformanceCounter(&time);

    return (time_nclk_t)time.QuadPart;
}


/* get the current time, unit is millisecond */
/* 获取当前时间微秒数 */
time_us_t time_us_get(void)
{
    LARGE_INTEGER time;

    if (!freq.QuadPart)
    {
        QueryPerformanceFrequency(&freq);
    }

    QueryPerformanceCounter(&time);

    return (time_us_t)((double)time.QuadPart * 1000000 / (double)freq.QuadPart);
}


/* convert the clocks to microseconds */
/* 将时钟数转为微秒 */
time_us_t time_nclk_to_us(time_nclk_t time_nclk)
{
    if (!freq.QuadPart)
    {
        QueryPerformanceFrequency(&freq);
    }

    return (time_us_t)((double)time_nclk * 1000000 / (double)freq.QuadPart);
}


/* convert the microseconds to clocks */
/* 将微秒转为时钟数 */
time_nclk_t time_us_to_nclk(time_us_t time_us)
{
    if (!freq.QuadPart)
    {
        QueryPerformanceFrequency(&freq);
    }

    return (time_nclk_t)((double)time_us * freq.QuadPart / 1000000);
}

void el_schedule_prepare(void)
{
}
