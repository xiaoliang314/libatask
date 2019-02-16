/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#include "../lib/atask.h"
#include <windows.h>

static time_nclk_t sys_clk = 0;

/* get the current time, unit is number of clocks */
/* 获取当前时间时钟数 */
time_nclk_t time_nclk_get(void)
{
    return sys_clk += 16;
}


/* get the current time, unit is millisecond */
/* 获取当前时间微秒数 */
time_us_t time_us_get(void)
{
    /* 2GHz */
    return time_nclk_get() / 2048;
}


/* convert the clocks to microseconds */
/* 将时钟数转为微秒 */
time_us_t time_nclk_to_us(time_nclk_t time_nclk)
{
    return (time_us_t)time_nclk / 2048;
}


/* convert the microseconds to clocks */
/* 将微秒转为时钟数 */
time_nclk_t time_us_to_nclk(time_us_t time_us)
{
    return time_us * 2048;
}

void el_schedule_prepare(void)
{
}
