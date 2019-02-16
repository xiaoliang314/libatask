/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */
#include "../lib/atask.h"
#include <stdio.h>
#include <windows.h>

/* task test 3 */
/* task测试3 */
static void task_test_3(task_t *task, event_t *ev, time_ms_t interval, time_ms_t total)
{
    /* Use the bp variable of the "task" in the coroutine */
    /* 协程使用task的bp变量 */
    uint8_t *bpd = TASK_BPD(task);

    /* Get and assign asynchronous variables from the stack of the "task" */
    /* 从task的栈中获取和分配异步变量 */
    struct
    {
        time_ms_t interval;
        time_ms_t total;
        time_ms_t i;
        timer_event_t timer;
    } *vars = task_asyn_vars_get(task, sizeof(*vars));

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(2);

    /* Save the called parameters to the asynchronous variables before the coroutine yield */
    /* 在协程yield之前保存调用的参数到异步变量之中 */
    vars->interval = interval;
    vars->total = total;

    /* 
     * Initialize timer with the event in "task" 
     * because the function has initialized the event in "task" 
     * before being called asynchronously, 
     * so, the timer can inherit the event of the "task" to initialize.
     */
    /* 
     * 使用task中的event初始化timer，
     * 因为在task被异步调用之前，
     * 已经对task中的event进行了初始化，
     * 因此timer可以继承task的event进行初始化
     */
    timer_init_inherit(&vars->timer, &task->event);

    /* flashing "total" milliseconds at intervals of "interval"  */
    /* 以interval的间隔闪烁total毫秒 */
    for (vars->i = 0; vars->i < vars->total; vars->i += vars->interval * 2)
    {
        /* LED ON */
        /* 打开LED */
        printf("%s : LED ON i:%d total:%d\n", __FUNCTION__, (int)vars->i, (int)vars->total);

        /* Start the timer of "interval" time */
        /* 启动"interval"时间的定时器 */
        el_timer_start_ms(&vars->timer, vars->interval);
        bpd_yield(1);

        /* LED OFF */
        /* 关闭LED */
        printf("%s : LED OFF i:%d total:%d\n", __FUNCTION__, (int)vars->i + (int)vars->interval, (int)vars->total);

        /* Start the timer of "interval" time */
        /* 启动"interval"时间的定时器 */
        el_timer_start_ms(&vars->timer, vars->interval);
        bpd_yield(2);
    }

    /* coroutine end */
    /* 协程结束 */
    bpd_end();

    /* Asynchronous return */
    /* 异步返回 */
    task_asyn_return(task);
}


/* task test 2 */
/* task测试2 */
static void task_test_2(task_t *task, event_t *ev)
{
    /* Use the bp variable of the "task" in the coroutine */
    /* 协程使用task的bp变量 */
    uint8_t *bpd = TASK_BPD(task);

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(2);

    /* Asynchronously call task_test_3 and waits for a return */
    /* 异步调用task_test_3并等待返回 */
    task_bpd_asyn_call(1, task, task_test_3, 2000, 50000);

    /* Asynchronously call task_test_3 and waits for a return */
    /* 异步调用task_test_3并等待返回 */
    task_bpd_asyn_call(2, task, task_test_3, 3000, 80000);

    /* coroutine end */
    /* 协程结束 */
    bpd_end();

    /* Asynchronous return */
    /* 异步返回 */
    task_asyn_return(task);
}


/* task function test */
/* task功能测试 */
static void task_test(task_t *task, event_t *ev)
{
    /* Use the bp variable of the "task" in the coroutine */
    /* 协程使用task的bp变量 */
    uint8_t *bpd = TASK_BPD(task);

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(1);

    /* Asynchronously call task_test_2 and waits for a return */
    /* 异步调用task_test_2并等待返回 */
    task_bpd_asyn_call(1, task, task_test_2);

    /* coroutine end */
    /* 协程结束 */
    bpd_end();
}


int main()
{
    time_ms_t timeout;

    /*
     * Define a "test" task with a stack size of 128 bytes 
     * and an event priority of LOWER_GROUP_PRIORITY,
     * the task function is task_test
     */
    /* 
     * 定义一个test任务，栈大小128字节，事件优先级为LOWER_GROUP_PRIORITY，
     * 任务函数为task_test
     */
    TASK_DEFINE(test, 128, LOWER_GROUP_PRIORITY);

    /* Start the task */
    /* 启动任务 */
    task_start(&test, task_test);

    /* Running the atask kernel */
    /* 运行atask内核 */
    while (1)
    {
        el_schedule();

        if (!el_have_imm_event())
        {
            timeout = time_nclk_to_us(el_timer_recent_due_get() - time_nclk_get()) / 1000;
            Sleep(timeout);
        }
    }
}
