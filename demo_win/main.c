/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */
#include "../lib/atask.h"
#include <stdio.h>
#include <windows.h>

/* Asynchronous function 1 */
/* 异步函数1 */
static void async_func1(task_t *task, event_t *ev, time_ms_t interval, time_ms_t timeout)
{
    /* Defines the bpd pointer required for the coroutine (asynchronous function) */
    /* 定义协程（异步函数）所需的bpd指针 */
    uint8_t *bpd = TASK_BPD(task);

    /* Define asynchronous variables for coroutines (asynchronous functions) */
    /* 定义协程（异步函数）的异步变量 */
    struct vars
    {
        /* Define a timer */
        /* 定义一个定时器 */
        timer_event_t timer;

        /* Total execution time */
        /* 总执行时间 */
        time_ms_t total;

        /* Execution time*/
        /* 已执行时间*/
        time_ms_t current;

        /* execution interval */
        /* 执行间隔 */
        time_ms_t interval;

        /* binds the address of the asynchronous variable to the stack space of the current task */
        /* 将异步变量的地址绑定到当前任务的栈空间中 */
    } *vars = task_asyn_vars_get(task, sizeof(*vars));

    /* Coroutine start */
    /* 开始协程 */
    bpd_begin(2);

    /* can be used to initialize other events directly using the event in the task.
     * Guarantee that the current coroutine can be restored after these events are returned */
    /* 可直接使用task中的event对其他事件进行初始化，
     * 保证这些事件返回之后可恢复当前协程 */
    timer_init_inherit(&vars->timer, &task->event);

    /* Save the parameters to the asynchronous variable before yield, so that it can still be used after yield */
    /* 在yield之前先将参数保存到异步变量中，使之yield之后仍能使用 */
    vars->total = timeout;
    vars->interval = interval;
    vars->current = 0;

    for (vars->current = 0; 
        vars->current < vars->total;
        vars->current += vars->interval * 2)
    {
        /* delay interval ms */
        /* 延时interval ms */
        el_timer_start_ms(&vars->timer, vars->interval);
        bpd_yield(1);

        printf("LED ON\n");

        /* delay interval ms */
        /* 延时interval ms */
        el_timer_start_ms(&vars->timer, vars->interval);
        bpd_yield(2);

        printf("LED OFF\n");
    }

    /* Coroutine end */
    /* 结束协程 */
    bpd_end();

    /* Coroutine (asynchronous function) returns to the asynchronous caller */
    /* 协程（异步函数）返回到异步调用者 */
    task_asyn_return(task);
}

/* Asynchronous function 2 */
/* 异步函数2 */
static void async_func2(task_t *task, event_t *ev)
{
    uint8_t *bpd = TASK_BPD(task);

    bpd_begin(2);

    while (1)
    {
        /* Asynchronous call asynchronous function 1 Parameters: 1000,8000 */
        /* 异步调用异步函数1 参数：1000，8000*/
        task_bpd_asyn_call(1, task, async_func1, 1000, 8000);

        /* Asynchronous call asynchronous function 2 Parameters: 2000,10000*/
        /* 异步调用异步函数1 参数：2000，10000*/
        task_bpd_asyn_call(2, task, async_func1, 2000, 10000);
    }

    bpd_end();
}

/* Task end callback */
/* 任务结束回调 */
static void on_task_end(void *ctx, event_t *ev)
{
    printf("%s\n", (const char *)ctx);
}

/* Separate timer callback */
/* 单独的定时器回调 */
static void on_timer(void *ctx, event_t *ev)
{
    printf("%s\n", (const char *)ctx);

    el_timer_start_ms((timer_event_t*)ev, 1000);
}

int main()
{
    event_t task_end_ev;
    timer_event_t timer;

    time_nclk_t due, now;
    time_ms_t timeout;

    /*
     * Define some task with a stack size of 128 bytes 
     * and an event priority of LOWER_GROUP_PRIORITY
     */
    /* 
     * 定义几个任务，栈大小128字节，事件优先级为LOWER_GROUP_PRIORITY
     */
    TASK_DEFINE(task1, 128, LOWER_GROUP_PRIORITY);
    TASK_DEFINE(task2, 128, LOWER_GROUP_PRIORITY);
    TASK_DEFINE(task3, 128, LOWER_GROUP_PRIORITY);

    /* Start asynchronous function (task) 2 */
    /* 启动异步函数（任务）2 */
    task_start(&task1, async_func2);

    /* Start multiple asynchronous functions (tasks) 2 */
    /* 开启多个异步函数（任务）2 */
    task_start(&task2, async_func2);

    /* Initialize an event */
    /* 初始化一个事件 */
    event_init(&task_end_ev, on_task_end, "Async function 1 end!\n", LOWER_GROUP_PRIORITY);
    /* Set to task3 to end the notification event */
    /* 设置为task3结束通知事件 */
    task_end_wait(&task3, &task_end_ev);

    /* Start asynchronous function with parameters (task) 1 */
    /* 带参数启动异步函数（任务）1 */
    task_start(&task3, async_func1, 5000, 15000);

    /* Initialize timer */
    /* 初始化定时器 */
    timer_init(&timer, on_timer, "This is a timer event!\n", LOWER_GROUP_PRIORITY);
    /* Start a separate timer event */
    /* 启动单独的定时器事件 */
    el_timer_start_ms(&timer, 1500);

    /* Running the atask kernel */
    /* 运行atask内核 */
    while (1)
    {
        due = el_schedule();

        /* calculate timeout */
        /* 计算超时 */
        now = time_nclk_get();
        timeout = due < now ? 0 : time_nclk_to_us(due - now) / 1000;
        timeout = timeout > UINT32_MAX ? INFINITE : timeout;

        Sleep((DWORD)timeout);
    }
}
