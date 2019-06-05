/*
 * Copyright (C) 2018-2019 xiaoliang<1296283984@qq.com>.
 */

#ifndef __LIB_ATASK_H__
#define __LIB_ATASK_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "defs.h"
#include "slist.h"
#include "fifo.h"
#include "lifo.h"
#include "va_nargs.h"
#include "bp.h"
#include "bp_stmt.h"

/*********************************************************
 *@description: 
 ***When libatask has schedulable events, 
 ***the outer framework should prepare the libatask scheduling process.
 ***
 ***If libatask is embedded in other frameworks, 
 ***after the external framework submits events to libatask or runs a timer, 
 ***the external framework should execute el_schedule to schedule libatask events 
 ***at a suitable time in the future. These frameworks may be: GUI frameworks, etc.
 ***
 ***Scheduling preparation process is as follows:
 ***1. provide and implement the el_schedule_prepare interface
 ***2. el_schedule_prepare interface is implemented as follows:
 ***  1) Determine whether there is an event in the event loop that can be dispatched immediately. 
 ***     If it exists, the external framework should prepare the scheduling process immediately.
 ***  2) If there is no event in the event loop, 
 ***     check the most recent expiration time of the current event loop timer, 
 ***     and schedule the scheduling process at the specified expiration time.
 ***
 ***If you use libatask as the outermost framework, 
 ***you do not need to configure this feature. 
 *********************************************************
 *@说明：
 ***当libatask有可调度事件时，需要外层框架准备libatask的调度过程。
 ***
 ***如果libatask被嵌入其他的框架之中，在外部框架在向libatask提交事件
 ***或者运行定时器后，外部框架应该在未来的合适的时机执行el_schedule对
 ***libatask的事件进行调度。这些框架如：GUI框架等
 ***
 ***调度的准备过程如下：
 ***1、提供并实现el_schedule_prepare接口
 ***2、el_schedule_prepare接口实现方式如下：
 ***  1)判断事件循环中是否存在可被立即调度的事件，若存在则应该让
 ***    外部框架立即安排一次调度过程
 ***  2)若事件循环中不存在被调度的事件，则检查当前事件循环定时器
 ***    的最近到期时间，在指定的到期时间安排一次调度过程
 ***
 ***若使用libatask作为最外部框架，则不需要配置此功能
 *********************************************************/
#define CONFIG_EL_HAVE_SCHEDULE_PREPARE


/*********************************************************
 *@description: 
 ***The maximum number of events that can be processed by el_schdule at a time.
 ***This value is a trade-off between external events and 
 ***local backlog events when the local backlog events and external events is high.
 ***Taking 4 as an example, it means:
 ***Process 4 local backlog events first 
 ***and then check external events (ie el_schedule returns)
 *********************************************************
 *@说明：
 ***一次el_schdule调用最多可处理的事件数量，
 ***此数值是积压的未处理事件与外部事件都高负载情况下，
 ***对外部事件与本地积压事件调度权衡比。
 ***以4为例，则代表：
 ***先处理4个本地积压事件再检查一次外部事件（即el_schedule返回）
 ***
 *********************************************************/
#define EL_ONCE_SCHEDULE_MAX_EVENT_COUNT   4


/*********************************************************
 *@type description:
 *
 *[event_t]: event type, the basic unit of asynchronous scheduling in atask,
 ***         with 0-255 priorities.
 ***         The event contains the context of the callback function and 
 ***         the callback function at the time of the trigger,
 ***         contains a byte of event ID and a byte of event state
 *********************************************************
 *@类型说明：
 *
 *[event_t]：事件类型，atask中异步调度的基本单位，拥有0-255个优先级，
 ***         事件包含了触发时的回调函数与回调函数的上下文
 *********************************************************/
typedef struct event_s
{
    /* event node */
    /* 事件节点 */
    slist_node_t node;

    /* event context */
    /* 事件上下文 */
    void *context;

    /* event callback */
    /* 事件回调函数 */
    void (*callback)(void *context, struct event_s *e);

    /* event priority */
    /* 事件优先级 */
    uint8_t priority;

    /* is ready */
    /* 是否就绪 */
    uint8_t is_ready;

    /* unused field*/
    /* 未使用字段 */
    uint8_t unused[2];
} event_t;


/************************************************************
 *@brief:
 ***event structure static initialization
 *
 *@Usage:
 ***event_t event = EVENT_STATIC_INIT(event);
 *
 *@parameter:
 *[event]: event variable name, non-address
 *[callback]: event callback function (valid when called with 2 arguments)
 *[ctx]: callback context of the event (valid when called with 3 arguments)
 *[priority]: priority of the event (valid when called with 4 arguments)
 *[id]: ID of the event (valid when called with 5 arguments)
 *[state]: state of the event (valid when called with 6 arguments)
 *************************************************************/
/************************************************************
 *@简介：
 ***事件结构体静态初始化
 *
 *@用法：
 ***event_t event = EVENT_STATIC_INIT(event);
 *
 *@参数：
 *[event]：事件变量名，非地址
 *[callback]：事件回调函数
 *[ctx]：事件的回调上下文
 *[priority]：事件的优先级
 *************************************************************/
#define EVENT_STATIC_INIT(event, callback, ctx, priority) \
{                                               \
    SLIST_NODE_STATIC_INIT((event).node),       \
    (ctx),                                      \
    (callback),                                 \
    (priority), 0, {0}                          \
}


/* event callback type definition */
/* 事件回调类型定义 */
typedef void (*event_cb)(void *, event_t *);


/* empty event callback function */
/* 空事件回调函数 */
#define EVENT_NULL_CB   ((event_cb)NULL_CB)

/*********************************************************
 *@description: 
 ***event related member variable reference
 *********************************************************
 *@说明：
 ***事件相关成员变量引用
 *********************************************************/
/* the node of event */
/* 事件的节点 */
#define EVENT_NODE(event)   ((slist_node_t *)(event))

/* the event of node */
/* 节点的事件 */
#define EVENT_OF_NODE(_node) ((event_t *)(_node))

/* the context of event */
/* 事件的上下文 */
#define EVENT_CONTEXT(event) ((event)->context)

/* the callback of event */
/* 事件的回调 */
#define EVENT_CALLBACK(event) ((event)->callback)

/* the priority of event */
/* 事件的优先级 */
#define EVENT_PRIORITY(event) ((event)->priority)

/************************************************************
 *@brief:
 ***event initialization
 *
 *@parameter:
 *[event]: event pointer of be initialized
 *[callback]: event callback function (valid when called with 2 arguments)
 *[ctx]: callback context of the event (valid when called with 3 arguments)
 *[priority]: priority of the event (valid when called with 4 arguments)
 *[id]: ID of the event (valid when called with 5 arguments)
 *[state]: state of the event (valid when called with 6 arguments)
 *************************************************************/
/************************************************************
 *@简介：
 ***事件初始化
 *
 *@参数：
 *[event]：被初始化的事件指针
 *[callback]：事件回调函数
 *[ctx]：事件的回调上下文
 *[priority]：事件的优先级
 *************************************************************/
static inline void event_init(event_t *event,
                                event_cb ecb,
                                void *ctx,
                                uint8_t priority)
{
    event->priority = priority;
    event->is_ready = 0;
    event->context = ctx;
    event->callback = ecb ? ecb : EVENT_NULL_CB;
    slist_node_init(&event->node);
}


/*****************************************
 *@brief:
 ***Event inheritance initialization,
 ***will inherit the parent event callback function,
 ***context, priority
 * 
 *@param
 *[event]: event pointer of be initialized
 *[parent]: parent event
 *[id]: ID of event (valid when called with 3 arguments)
 *[state]: state of event (valid when called with 4 arguments)
 *****************************************/
/*****************************************
 *@简要：
 ***事件继承初始化，将继承父事件的回调函数，上下文，优先级
 *
 *@参数
 *[event]：被初始化的事件指针
 *[parent]：父事件
 *****************************************/
static inline void event_init_inherit(event_t *event, const event_t *parent)
{
    event->priority = parent->priority;
    event->is_ready = 0;
    event->context = parent->context;
    event->callback = parent->callback ? parent->callback : EVENT_NULL_CB;
    slist_node_init(&event->node);
}


/*********************************************************
 *@brief: 
 ***event queue, enqueue by priority
 *
 *@contract: 
 ***1. "epfifo" is the event queue
 ***2. "event" node is deleted
 ***3. "epfifo" and "event" are not null pointers
 *
 *@parameter:
 *[epfifo]: event queue
 *[event]: event of be enqueued
 *********************************************************/
/*********************************************************
 *@简要：
 ***事件队列，按优先级入队
 *
 *@约定：
 ***1、epfifo是事件队列
 ***2、event节点处于删除状态
 ***3、epfifo与event不是空指针
 *
 *@参数：
 *[epfifo]：事件队列
 *[event]：需要入队的事件
 **********************************************************/
static inline void event_fifo_priority_push(fifo_t *epfifo, event_t *event)
{
    event_t *insert_pos = EVENT_OF_NODE(FIFO_TAIL(epfifo));
    slist_node_t *prev_node;

    /* If the queue is empty or the current event priority is lower than
     the queue tail priority, insert the event to the tail */
    /* 如果队列空或者当前事件优先级低于队尾优先级，则将事件插入到尾部 */
    if (fifo_is_empty(epfifo) || EVENT_PRIORITY(event) <= EVENT_PRIORITY(insert_pos))
    {
        fifo_push(epfifo, EVENT_NODE(event));
    }
    else
    {
        /* find the inserted position by priority from the queue list */
        /* 从头按优先级查找插入的位置 */
        slist_foreach_entry_record_prev(FIFO_LIST(epfifo), insert_pos, node, prev_node)
        {
            if (EVENT_PRIORITY(event) > EVENT_PRIORITY(insert_pos))
            {
                fifo_node_insert_next(epfifo, prev_node, EVENT_NODE(event));
                break;
            }
        }
    }
}


/*********************************************************
 *@brief: 
 ***event queue, dequeue by priority
 *
 *@contract: 
 ***1. "epfifo" is not a null pointer
 ***2. "epfifo" is the event queue
 ***3. "epfifo" is not empty
 *
 *@parameter:
 *[epfifo]: event queue
 *
 *@return value:
 *[event]: the highest priority event in the queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***事件队列，按优先级出队
 *
 *@约定：
 ***1、epfifo不是空指针
 ***2、epfifo是事件队列
 ***3、epfifo不为空
 *
 *@参数：
 *[epfifo]：事件队列
 *
 *@返回值：
 *[event]：队列中优先级最高的事件
 **********************************************************/
static inline event_t *event_fifo_priority_pop(fifo_t *epfifo)
{
    return container_of(fifo_pop(epfifo), event_t, node);
}


/*********************************************************
 *@brief: 
 ***modify the priority of event in the event queue
 *
 *@contract: 
 ***1. "epfifo" and "event" are not null pointers
 *
 *@parameter:
 *[epfifo]: event queue
 *[event]: the event of need to modify priority
 *[new_priority]: new event priority
 *
 *@return value:
 *[true]: successfully modified
 *[false]: the event is not in "epfifo"
 *********************************************************/
/*********************************************************
 *@简要：
 ***修改处于事件队列中事件的优先级
 *
 *@约定：
 ***1、epfifo与event不是空指针
 *
 *@参数：
 *[epfifo]：事件队列
 *[event]：要修改优先级的事件
 *[new_priority]：新的优先级
 *
 *@返回值：
 *[true]：修改成功
 *[false]：事件不在epfifo之中
 **********************************************************/
static inline bool event_fifo_reset_priority(fifo_t *epfifo, event_t *event, uint8_t new_priority)
{
    event_t *find_event = 0;
    slist_node_t *prev_node;
    slist_node_t *safe_node;
    slist_node_t *insert_pos;
    uint8_t is_find_priority = 0;
    uint8_t is_find_event = 0;

    /* special case, if the event is at the end of the queue
     * and the priority is lower than the current priority,
     * the priority is directly modified. */
    /* 特殊情况，若事件处于队列尾部且优先级比当前优先级小，
     * 则直接修改优先级 */
    if (FIFO_TAIL(epfifo) == EVENT_NODE(event)
     && new_priority <= EVENT_PRIORITY(event))
    {
        EVENT_PRIORITY(event) = new_priority;

        return true;
    }

    /* when the new priority insertion position is not found,
       it is inserted into the tail of the queue by default */
    /* 当新优先级插入位置未找到时，默认插入到队列尾部 */
    insert_pos = FIFO_TAIL(epfifo);

    /* finding the position of the event in the queue,
     * while recording the insertion position of the new priority */
    /* 从头查找事件在队列中的位置，同时记录新优先级的插入位置 */
    slist_foreach_entry_record_prev_safe(FIFO_LIST(epfifo), find_event, node, prev_node, safe_node)
    {
        /* priority position is found, set flags and record */
        /* 优先级位置找到，设置flags并记录 */
        if (!is_find_priority && new_priority > EVENT_PRIORITY(find_event))
        {
            insert_pos = prev_node;

            is_find_priority = 1;
        }

        /* event is found, set flags and remove */
        /* 事件被找到，设置flags并移除 */
        if (find_event == event)
        {
            fifo_node_del_next_safe(epfifo, prev_node, &safe_node);

            is_find_event = 1;
        }

        /* both found, break */
        /* 都找到，则返回 */
        if (is_find_priority && is_find_event)
        {
            break;
        }
    }

    /* the event is not in the queue */
    /* 事件不在队列之中 */
    if (is_find_event == 0)
    {
        return false;
    }

    /* modify the priority and insert it into a new position */
    /* 修改优先级，并插入到新位置 */
    EVENT_PRIORITY(event) = new_priority;
    fifo_node_insert_next(epfifo, insert_pos, EVENT_NODE(event));

    return true;
}


/* time unit of nanosecond */
/* 纳秒为单位的时间 */
typedef uint64_t time_ns_t;

/* time unit of microseconds */
/* 微秒为单位的时间 */
typedef uint64_t time_us_t;

/* time unit of millisecond */
/* 毫秒为单位的时间 */
typedef uint64_t time_ms_t;

/* time unit of seconds */
/* 秒为单位的时间 */
typedef uint32_t time_sec_t;

/* number of timer interrupts */
/* 定时器中断个数 */
typedef uint32_t time_tick_t;

/* timer counter value type */
/* 定时器计数器值类型  */
typedef uint32_t time_cntr_t;

/* time unit of number of clocks */
/* 以时钟数为单位的时间 */
typedef uint64_t time_nclk_t;

/* get the current time, unit is number of clocks */
/* 获取当前时间时钟数 */
extern time_nclk_t time_nclk_get(void);

/* get the current time, unit is millisecond */
/* 获取当前时间微秒数 */
extern time_us_t time_us_get(void);

/* convert the clocks to microseconds */
/* 将时钟数转为微秒 */
extern time_us_t time_nclk_to_us(time_nclk_t time_nclk);

/* convert the microseconds to clocks */
/* 将微秒转为时钟数 */
extern time_nclk_t time_us_to_nclk(time_us_t time_us);


enum
{
    /* highest priority group */
    /* 最高优先级组 */
    HIGHEST_GROUP_PRIORITY = 0xc0,

    /* high priority group */
    /* 高优先级组 */
    HIGH_GROUP_PRIORITY = 0x80,

    /* middle priority group */
    /* 中优先级 */
    MIDDLE_GROUP_PRIORITY = 0x40,

    /* lower priority group */
    /* 低优先级组 */
    LOWER_GROUP_PRIORITY = 0x00,

    /* the count of priority group */
    /* 优先级组个数 */
    READY_GROUP_COUNT = 4
};

/* priority bit field definition */
/* 优先级位字段定义 */
#define READY_SUB_PRIORITY_MASK 0x3F
#define READY_GROUP_PRIORITY_MASK 0xC0
#define READY_GROUP_PRIORITY_SHIFT 6

/* The maximum number of events scheduled at once */
/* 一次调度的最大事件个数 */
#ifndef EL_ONCE_SCHEDULE_MAX_EVENT_COUNT
#define EL_ONCE_SCHEDULE_MAX_EVENT_COUNT   4
#endif /* EL_ONCE_SCHEDULE_MAX_EVENT_COUNT */

typedef struct el_s
{
    /* event ready queue group */
    /* 事件就绪队列组 */
    fifo_t ready_groups[READY_GROUP_COUNT];

    /* timers queue */
    /* 定时器队列 */
    fifo_t timers;

    /* event loop timer time of due */
    /* 事件循环定时器到期时间 */
    time_nclk_t due;

    /* event group ready status bitmap */
    /* 事件组就绪状态bitmap */
    uint8_t ready_map;

    /* timer queue has element flag */
    /* 定时器队列有元素标志位 */
    uint8_t timers_have;

    /* recursion schedule count */
    uint16_t recursion_schedule;
} el_t;


/************************************************************
*@brief:
***event loop data structure static initialization
*
*@parameter:
*[el]: event loop variable name, non-address
*************************************************************/
/************************************************************
*@简介：
***事件循环数据结构体静态初始化
*
*@参数：
*[el]：事件循环变量名，非地址
*************************************************************/
#define EL_STATIC_INIT(el)                              \
{                                                       \
    {                                                   \
        FIFO_STATIC_INIT((el).ready_groups[0]),         \
        FIFO_STATIC_INIT((el).ready_groups[1]),         \
        FIFO_STATIC_INIT((el).ready_groups[2]),         \
        FIFO_STATIC_INIT((el).ready_groups[3])          \
    },                                                  \
    FIFO_STATIC_INIT((el).timers),                      \
    0,                                                  \
    0,0,0                                               \
}


/*********************************************************
*@type description:
*
*[timer_event_t]: Timer events, inherited from events,
***are used to generate timeouts.
*********************************************************
*@类型说明：
*
*[timer_event_t]：定时器事件，继承于事件，用于产生超时事件
*********************************************************/
typedef struct timer_event_s
{
    event_t event;
    time_nclk_t due;
} timer_event_t;

/************************************************************
*@brief:
***timer structure static initialization
*
*@Usage:
***timer_event_t timer = TIMER_EVENT_STATIC_INIT(timer, callback, ctx);
*
*@parameter:
*[timer]: timer variable name, non-address
*[callback]: event callback function (valid when called with 2 arguments)
*[ctx]: callback context of the event (valid when called with 3 arguments)
*[priority]: priority of the event (valid when called with 4 arguments)
*[id]: ID of the event (valid when called with 5 arguments)
*[state]: state of the event (valid when called with 6 arguments)
*************************************************************/
/************************************************************
*@简介：
***定时器结构体静态初始化
*
*@用法：
***event_t event = TIMER_EVENT_STATIC_INIT(timer, callback, ctx);
*
*@参数：
*[timer]：定时器变量名，非地址
*[callback]：事件回调函数
*[ctx]：事件的回调上下文
*[priority]：事件的优先级
*************************************************************/
#define TIMER_EVENT_STATIC_INIT(timer, callback, ctx, priority)     \
{                                                                   \
    EVENT_STATIC_INIT((timer).event, (callback), (ctx), (priority)),\
    0                                                               \
}

#define timer_init(timer, callback, ctx, priority)                  \
    do                                                              \
    {                                                               \
        event_init(&(timer)->event, (callback), (ctx), (priority)); \
        (timer)->due = 0;                                           \
    } while (0)

#define timer_init_inherit(timer, parent_ev)                        \
    do                                                              \
    {                                                               \
        event_init_inherit(&(timer)->event, (parent_ev));           \
        (timer)->due = 0;                                           \
    } while (0);


/*********************************************************
*@description:
***timer related member variable conversion
*********************************************************
*@说明：
***定时器相关成员变量转换
*********************************************************/
#define TIMER_EVENT(timer)  ((event_t *)(timer))

#define TIMER_OF_EVENT(_event)  ((timer_event_t *)(_event))

#define TIMER_NODE(timer) EVENT_NODE(TIMER_EVENT(timer))

#define TIMER_OF_NODE(node) TIMER_OF_EVENT(EVENT_OF_NODE(node))


/*********************************************************
*@description:
***global variable declaration
*********************************************************
*@说明：
***全局变量声明
*********************************************************/

/* default event loop object */
/* 默认事件循环对象 */
extern el_t dflt_el;

/* highest priority ready group bitmap */
/* 最高优先级就绪组位图 */
extern const uint8_t priority_ready_bitmap[];


/*********************************************************
*@description:
***API declaration
*********************************************************
*@说明：
***API声明
*********************************************************/

/*********************************************************
*@brief:
***Post an event to the event loop
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[e]: the event of be posted
*
*@return value:
*[true]: Successfully posted
*[false]: The event node is in the queue or reference state
*********************************************************/
/*********************************************************
*@简要：
***向事件循环提交一个事件
*
*@约定：
***不能使用空指针
*
*@参数：
*[e]：被提交的事件
*
*@返回值：
*[true]：提交成功
*[false]：事件节点处于队列之中或者引用状态
**********************************************************/
bool el_event_post(event_t *e);


/*********************************************************
*@description:
***API declaration
*********************************************************
*@说明：
***API声明
*********************************************************/

/*********************************************************
*@brief:
***Synchronously call this event
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[e]: the event of be posted
*********************************************************/
/*********************************************************
*@简要：
***同步调用触发这个事件
*
*@约定：
***不能使用空指针
*
*@参数：
*[e]：被提交的事件
**********************************************************/
static inline void el_event_sync_post(event_t *e)
{
    e->callback(e->context, e);
}


/*********************************************************
*@brief:
***Synchronously call this event with arguments
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[e]: the event of be call
*[ecb_type]: 
****The function type (signature) of the event callback, 
****the callback function parameter list must be in the form of (void *, event_t *, ...)
*[...]: The third and subsequent parameters of the event callback function
*********************************************************/
/*********************************************************
*@简要：
***带参数的方式同步触发这个事件
*
*@约定：
***不能使用空指针
*
*@参数：
*[e]：被调用的事件
*[ecb_type]: 
***事件回调的函数类型（签名）
***回调函数参数列表必须形如(void *, event_t *, ...)格式
*[...]: 传入事件回调函数的第3个及之后的参数
**********************************************************/
#define el_event_sync_post_with_args(e, ecb_type, ...) \
    ((ecb_type)(e)->callback)((e)->context, (e), ##__VA_ARGS__)


/*********************************************************
*@brief:
***Cancel an event in the event loop
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[e]: the event of be cancelled
*
*@return value:
*[true]: Successfully cancelled
*[false]: The event is not in the event loop
*********************************************************/
/*********************************************************
*@简要：
***取消事件循环中的一个事件
*
*@约定：
***不能使用空指针
*
*@参数：
*[e]：被提交的事件
*
*@返回值：
*[true]：提交成功
*[false]：事件节点处于队列之中或者引用状态
**********************************************************/
bool el_event_cancel(event_t *e);


/*********************************************************
*@brief:
***Reset the priority of posted events
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[e]: the event of posted
*[new_priority]: new priority
*
*@return value:
*[true]: Set up success
*[false]: The event was not posted
*********************************************************/
/*********************************************************
*@简要：
***重新设置已提交的事件的优先级
*
*@约定：
***不能使用空指针
*
*@参数：
*[e]：已提交的事件
*[new_priority]: 新优先级
*
*@返回值：
*[true]：设置成功
*[false]：事件未被提交
**********************************************************/
bool el_event_reset_priority(event_t *e, uint8_t new_priority);


/*********************************************************
*@brief:
***Check timers, 
***and schedule the highest priority events in the event loop event list
*********************************************************/
/*********************************************************
*@简要：
***检查定时器、并调度事件循环事件列表中优先级最高的事件
**********************************************************/
time_nclk_t el_schedule(void);


/*********************************************************
*@brief:
***Specify expiration time and start timer
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer of be started
*[due]: the time of timer expires
*
*@return value:
*[true]: Successful startup
*[false]: The timer has started or the node is in the queue
*********************************************************/
/*********************************************************
*@简要：
***指定到期时间，并开启定时器
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：被开启的定时器
*[due]: 定时器到期的时间
*
*@返回值：
*[true]：启动成功
*[false]：定时器已启动或节点处于队列之中
**********************************************************/
bool el_timer_start_due(timer_event_t *timer, time_nclk_t due);


/*********************************************************
*@brief:
***Stop timer
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer of be stoped
*
*@return value:
*[true]: Successful stoped
*[false]: Timer not started
*********************************************************/
/*********************************************************
*@简要：
***停止定时器
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：被停止的定时器
*
*@返回值：
*[true]：成功停止
*[false]：定时器未启动
**********************************************************/
bool el_timer_stop(timer_event_t *timer);


/*********************************************************
*@brief:
***Trigger timer event immediately
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer of be triggered
*
*@return value:
*[true]: Successfully triggered, started timing operation will be invalid
*[false]: Timer nodes are in other queues (not event loop event list and event loop timer list)
*********************************************************/
/*********************************************************
*@简要：
***立即触发定时器事件
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：被触发的定时器
*
*@返回值：
*[true]：触发成功、已启动的定时操作将失效
*[false]：定时器节点处于其他队列之中(非事件循环事件列表和事件循环定时器列表)
**********************************************************/
bool el_timer_trigger(timer_event_t *timer);


/*********************************************************
*@description:
***API declaration with implementation
*********************************************************
*@说明：
***API声明带实现
*********************************************************/


/*********************************************************
*@brief:
***Check if the event is ready (will be triggered)
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[e]: an event
*
*@return value:
*[true]: ready
*[false]: not ready
*********************************************************/
/*********************************************************
*@简要：
***检查事件是否已经就绪(将被触发)
*
*@约定：
***不能使用空指针
*
*@参数：
*[e]：事件
*
*@返回值：
*[true]：已就绪
*[false]：未就绪
**********************************************************/
static inline bool el_event_is_ready(event_t *e)
{
    return e->is_ready != 0;
}


/*********************************************************
*@brief:
***Get the highest scheduling priority in the event loop
*
*@return: Current highest scheduling priority in the event loop
*********************************************************/
/*********************************************************
*@简要：
***获取事件循环中当前的最高调度优先级
*
*@返回：事件循环中当前的最高调度优先级
**********************************************************/
static inline uint8_t el_highest_ready_priority_get(void)
{
    uint8_t highest_priority = 0;
    uint8_t ready_group;

    if (dflt_el.ready_map)
    {
        ready_group = priority_ready_bitmap[dflt_el.ready_map];

        highest_priority =
            EVENT_OF_NODE(FIFO_TOP(&dflt_el.ready_groups[ready_group]))->priority;
    }

    return highest_priority;
}


/*********************************************************
*@brief:
***Returns the time of timer expires in the event loop
*@return value:
*[0xFFFFFFFFFFFFFFFFUL]: Never expire
*[other]：Expiration time
*********************************************************/
/*********************************************************
*@简要：
***返回事件循环中定时器到期的时间
*@返回值：
*[0xFFFFFFFFFFFFFFFFUL]: 永不到期
*[其他]：到期的时间
**********************************************************/
static inline time_nclk_t el_timer_recent_due_get(void)
{
    return dflt_el.timers_have ? dflt_el.due : 0xFFFFFFFFFFFFFFFFUL;
}


/*********************************************************
*@brief:
***Check if there is a timer in the event loop
*@return value:
*[true]: There are timers in the event loop
*[false]: There is no timer in the event loop
*********************************************************/
/*********************************************************
*@简要：
***检查事件循环中是否存在定时器
*@返回值：
*[true]：事件循环中有定时器
*[false]：事件循环中没有定时器
**********************************************************/
static inline bool el_have_timers(void)
{
    return dflt_el.timers_have;
}


/*********************************************************
*@brief:
***Check if there are any events in the event loop
***that can be scheduled immediately.
***Will check the event loop event list for events
*
*@return value:
*[true]: There are events that can be scheduled immediately
*[false]: There are no events that can be scheduled immediately
*********************************************************/
/*********************************************************
*@简要：
***检查事件循环中是否有能被立即调度的事件
***将检查事件循环事件列表是否存在事件
*
*@返回值：
*[true]：存在能被立即调度的事件
*[false]：不存在能被立即调度的事件
**********************************************************/
static inline uint16_t el_have_imm_event(void)
{
    return dflt_el.ready_map;
}


/*********************************************************
*@brief:
***Get the expiration time in the timer
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer
*
*@return: the expiration time in the timer
*********************************************************/
/*********************************************************
*@简要：
***获取定时器中的到期时间
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：定时器
*
*@返回：定时器到期的时间
**********************************************************/
static inline time_nclk_t timer_due_get(timer_event_t *timer)
{
    return timer->due;
}


/*********************************************************
*@brief:
***Start timer, unit: microsecond
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer
*[timeout]: microsecond timeout
*
*@return value:
*[true]: Successful startup
*[false]: The timer has started or the node is in another queue
*********************************************************/
/*********************************************************
*@简要：
***启动定时器，单位：微秒
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：定时器
*[timeout]: 微秒超时时间
*
*@返回值：
*[true]：启动成功
*[false]: 定时器已启动或节点处于其他队列中
**********************************************************/
static inline bool el_timer_start_us(timer_event_t *timer, time_us_t timeout)
{
    time_nclk_t due;

    due = time_nclk_get() + time_us_to_nclk(timeout);

    return el_timer_start_due(timer, due);
}


/*********************************************************
*@brief:
***Start timer, unit: millisecond
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer
*[timeout]: millisecond timeout
*
*@return value:
*[true]: Successful startup
*[false]: The timer has started or the node is in another queue
*********************************************************/
/*********************************************************
*@简要：
***启动定时器，单位：毫秒
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：定时器
*[timeout]: 毫秒超时时间
*
*@返回值：
*[true]：启动成功
*[false]: 定时器已启动或节点处于其他队列中
**********************************************************/
static inline bool el_timer_start_ms(timer_event_t *timer, time_ms_t timeout)
{
    time_nclk_t due;

    due = time_nclk_get() + time_us_to_nclk(timeout * 1000);

    return el_timer_start_due(timer, due);
}


/*********************************************************
*@brief:
***Start timer, unit: clock cycle
*
*@contract:
***Cannot use null pointer
*
*@parameter:
*[timer]: the timer
*[timeout]: clock cycle timeout
*
*@return value:
*[true]: Successful startup
*[false]: The timer has started or the node is in another queue
*********************************************************/
/*********************************************************
*@简要：
***启动定时器，单位：时钟周期
*
*@约定：
***不能使用空指针
*
*@参数：
*[timer]：定时器
*[timeout]: 时钟周期超时时间
*
*@返回值：
*[true]：启动成功
*[false]: 定时器已启动或节点处于其他队列中
**********************************************************/
static inline bool el_timer_start_nclk(timer_event_t *timer, time_nclk_t timeout)
{
    time_nclk_t due;

    due = time_nclk_get() + timeout;

    return el_timer_start_due(timer, due);
}


/*********************************************************
*@brief:
***Timer remaining time acquisition, unit: clock cycle
*
*@contract:
***1. Cannot use null pointer
***2. timer was started
*
*@parameter:
*[timer]: the timer
*
*@return value:
*[0]: be expired
*[other]: Remaining expiration time
*********************************************************/
/*********************************************************
*@简要：
***定时器剩余时间获取，单位：时钟周期
*
*@约定：
***1、不能使用空指针
***2、定时器已启动
*
*@参数：
*[timer]：定时器
*
*@返回值：
*[0]：已到期
*[其他]：剩余的到期时间
**********************************************************/
static inline time_nclk_t el_timer_remaining_nclk_get(timer_event_t *timer)
{
    time_nclk_t now_nclk;

    if (slist_node_is_del(TIMER_NODE(timer))
        || timer->due < (now_nclk = time_nclk_get()))
    {
        return 0;
    }

    return timer->due - now_nclk;
}


/*********************************************************
*@brief:
***Timer remaining time acquisition, unit: microsecond
*
*@contract:
***1. Cannot use null pointer
***2. timer was started
*
*@parameter:
*[timer]: the timer
*
*@return value:
*[0]: be expired
*[other]: Remaining expiration time
*********************************************************/
/*********************************************************
*@简要：
***定时器剩余时间获取，单位：微秒
*
*@约定：
***1、不能使用空指针
***2、定时器已启动
*
*@参数：
*[timer]：定时器
*
*@返回值：
*[0]：已到期
*[其他]：剩余的到期时间
**********************************************************/
static inline time_us_t el_timer_remaining_us_get(timer_event_t *timer)
{
    time_nclk_t remaining_nclk = el_timer_remaining_nclk_get(timer);

    return time_nclk_to_us(remaining_nclk);
}


/*********************************************************
*@brief:
***Timer remaining time acquisition, unit: millisecond
*
*@contract:
***1. Cannot use null pointer
***2. timer was started
*
*@parameter:
*[timer]: the timer
*
*@return value:
*[0]: be expired
*[other]: Remaining expiration time
*********************************************************/
/*********************************************************
*@简要：
***定时器剩余时间获取，单位：毫秒
*
*@约定：
***1、不能使用空指针
***2、定时器已启动
*
*@参数：
*[timer]：定时器
*
*@返回值：
*[0]：已到期
*[其他]：剩余的到期时间
**********************************************************/
static inline time_ms_t el_timer_remaining_ms_get(timer_event_t *timer)
{
    time_nclk_t remaining_nclk = el_timer_remaining_nclk_get(timer);

    return time_nclk_to_us(remaining_nclk) / 1000;
}


/*********************************************************
 *@type description:
 *
 *[sem_t]: Semaphore
 *********************************************************
 *@类型说明：
 *
 *[sem_t]：信号量
 *********************************************************/
typedef struct sem_s
{
    fifo_t  give_q;
    fifo_t  take_q;
    int32_t limit;
    int32_t cnt;
} sem_t;


/************************************************************
 *@brief:
 ***Semaphore data structure static initialization
 *
 *@parameter:
 *[sem]: Initialized semaphore, non-pointer
 *[cnt]: Current number of semaphores (valid for three parameters)
 *[limit]: Maximum number of semaphores (valid for two parameters)
 *************************************************************/
/************************************************************
 *@简介：
 ***信号量数据结构静态初始化
 *
 *@参数：
 *[sem]：初始化的信号量，非指针
 *[cnt]：当前信号量个数（三个参数时有效）
 *[limit]：信号量最大个数（两个参数时有效）
 *************************************************************/
#define SEM_STATIC_INIT(sem, ...)  VA_ARGS_FUNC(SEM_STATIC_INIT_, sem, ##__VA_ARGS__)

#define SEM_STATIC_INIT_1(sem)              \
    SEM_STATIC_INIT_2(sem, 1)

#define SEM_STATIC_INIT_2(sem, limit)       \
    SEM_STATIC_INIT_3(sem, 0, limit)

#define SEM_STATIC_INIT_3(sem, cnt, limit)  \
{                                           \
    FIFO_STATIC_INIT((sem).give_q),         \
    FIFO_STATIC_INIT((sem).take_q),         \
    (limit),                                \
    (cnt)                                   \
}


/************************************************************
 *@brief:
 ***Semaphore data structure initialization
 *
 *@parameter:
 *[sem]: Initialized semaphore
 *[cnt]: Current number of semaphores (valid for three parameters)
 *[limit]: Maximum number of semaphores (valid for two parameters)
 *************************************************************/
/************************************************************
 *@简介：
 ***信号量数据结构初始化
 *
 *@参数：
 *[sem]：初始化的信号量
 *[cnt]：当前信号量个数（三个参数时有效）
 *[limit]：信号量最大个数（两个参数时有效）
 *************************************************************/
#define sem_init(sem, ...)  VA_ARGS_FUNC(sem_init_, (sem), ##__VA_ARGS__)

#define sem_init_1(sem)         \
    sem_init_2(sem, 1)

#define sem_init_2(sem, limit)  \
    sem_init_3(sem, 0, limit)

static inline void sem_init_3(sem_t *sem, int32_t cnt, int32_t limit)
{
    fifo_init(&sem->give_q);
    fifo_init(&sem->take_q);
    sem->limit = limit;
    sem->cnt = cnt;
}


/*********************************************************
 *@brief: 
 ***take semaphore, semaphore count minus 1
 *
 *@contract: 
 ***1. sem not is null pointer
 *
 *@parameter:
 *[sem]: semaphore
 *[event]: the event of notification success, can be NULL
 *
 *@return value:
 *[true]: If event is not NULL and the event is not referenced, 
 ***sem_take returns true and triggers an event event when the semaphore decrease successfully. 
 ***If the event is NULL, the semaphore decrease successfully and returns true.
 *
 *[false]: The event is not NULL, and when the event is referenced, it returns false. 
 ***the event is NULL, and returns false when the semaphore fails to decrease.
 *********************************************************/
/*********************************************************
 *@简要：
 ***取信号量，信号量计数值减1
 * 
 *@约定：
 ***1、sem不能为空指针
 *
 *@参数：
 *[sem]：信号量
 *[event]：通知操作成功的事件，可以为NULL
 *
 *@返回值：
 *[true]：若event不为NULL，且event未被引用则sem_take返回true，
 ***并在信号量减少成功时触发事件事件。
 ***若event为NULL，则信号量减少成功返回true
 *
 *[false]: event不为NULL，且event被引用时，返回false，
 ***event为NULL，且信号减少失败时返回false
 **********************************************************/
static inline bool sem_take(sem_t *sem, event_t *event)
{
    if (event && !slist_node_is_del(EVENT_NODE(event)))
    {
        return false;
    }

    if (sem->cnt > 0)
    {
        if (!fifo_is_empty(&sem->give_q))
        {
            el_event_post(event_fifo_priority_pop(&sem->give_q));
        }
        else
        {
            sem->cnt--;
        }

        if (event)
        {
            el_event_post(event);
        }

        return true;
    }
    else
    {
        if (event)
        {
            event_fifo_priority_push(&sem->take_q, event);

            return true;
        }

        return false;
    }
}


/*********************************************************
 *@brief: 
 ***give semaphore, semaphore count add 1
 *
 *@contract: 
 ***1. sem not is null pointer
 *
 *@parameter:
 *[sem]: semaphore
 *[event]: the event of notification success, can be NULL
 *
 *@return value:
 *[true]: If event is not NULL and the event is not referenced, 
 ***sem_take returns true and triggers an event event when the semaphore increases successfully. 
 ***If the event is NULL, the semaphore increases successfully and returns true.
 *
 *[false]: The event is not NULL, and when the event is referenced, it returns false. 
 ***the event is NULL, and returns false when the semaphore fails to increase.
 *********************************************************/
/*********************************************************
 *@简要：
 ***给信号量，信号量计数值加1
 * 
 *@约定：
 ***1、sem不能为空指针
 *
 *@参数：
 *[sem]：信号量
 *[event]：通知操作成功的事件，可以为NULL
 *
 *@返回值：
 *[true]：若event不为NULL，且event未被引用则sem_take返回true，
 ***并在信号量增加成功时触发事件事件。
 ***若event为NULL，则信号量增加成功返回true
 *
 *[false]: event不为NULL，且event被引用时，返回false，
 ***event为NULL，且信号量增加失败时返回false
 **********************************************************/
static inline bool sem_give(sem_t *sem, event_t *event)
{
    if (event && !slist_node_is_del(EVENT_NODE(event)))
    {
        return false;
    }

    if (sem->cnt < sem->limit)
    {
        if (!fifo_is_empty(&sem->take_q))
        {
            el_event_post(event_fifo_priority_pop(&sem->take_q));
        }
        else
        {
            sem->cnt++;
        }

        if (event)
        {
            el_event_post(event);
        }

        return true;
    }
    else
    {
        if (event)
        {
            event_fifo_priority_push(&sem->give_q, event);

            return true;
        }

        return false;
    }
}


/*********************************************************
 *@brief: 
 ***Cancel the event of sem_give
 *
 *@contract: 
 ***Cannot use null pointer
 *
 *@parameter:
 *[sem]: semaphore
 *[event]: the event used in sem_give
 *
 *@return value:
 *[true]: cancel success
 *[false]: cancel failed
 *********************************************************/
/*********************************************************
 *@简要：
 ***取消sem_give的事件
 *
 *@约定：
 ***不能使用空指针
 *
 *@参数：
 *[sem]：信号量
 *[event]: sem_give中使用的事件
 *
 *@返回值：
 *[true]：取消成功
 *[false]：取消失败
 **********************************************************/
static inline bool sem_give_cancel(sem_t *sem, event_t *event)
{
    if (slist_node_is_del(EVENT_NODE(event)))
    {
        return false;
    }

    if (el_event_is_ready(event))
    {
        return el_event_cancel(event);
    }
    else
    {
        return fifo_del_node(&sem->give_q, EVENT_NODE(event));
    }
}


/*********************************************************
 *@brief: 
 ***Cancel the event of sem_take
 *
 *@contract: 
 ***Cannot use null pointer
 *
 *@parameter:
 *[sem]: semaphore
 *[event]: the event used in sem_take
 *
 *@return value:
 *[true]: cancel success
 *[false]: cancel failed
 *********************************************************/
/*********************************************************
 *@简要：
 ***取消sem_take的事件
 *
 *@约定：
 ***不能使用空指针
 *
 *@参数：
 *[sem]：信号量
 *[event]: sem_take中使用的事件
 *
 *@返回值：
 *[true]：取消成功
 *[false]：取消失败
 **********************************************************/
static inline bool sem_take_cancel(sem_t *sem, event_t *event)
{
    if (slist_node_is_del(EVENT_NODE(event)))
    {
        return false;
    }

    if (el_event_is_ready(event))
    {
        return el_event_cancel(event);
    }
    else
    {
        return fifo_del_node(&sem->take_q, EVENT_NODE(event));
    }
}


/* slab allocator definition */
/* slab分配器定义 */
typedef struct slab_s
{
    /* block size */
    /* 块大小 */
    uint32_t    blk_size;

    /* number of block */
    /* 块数 */
    uint32_t    blk_nums;

    /* number of used block */
    /* 已使用的块数 */
    uint32_t    nums_used;

    /* record the original buff pointer */
    /* 记录原始的buff指针 */
    uint8_t     *buff;

    /* slab available notification queue */
    /* slab可用通知队列 */
    fifo_t    notify_q;

    /* free block list */
    /* 空闲块链表 */
    slist_t free_list;
} slab_t;


/* slab allocate event */
/* slab分配事件 */
typedef struct slab_alloc_event_s
{
    event_t event;
    void *mem_blk;
} slab_alloc_event_t;


/************************************************************
 *@brief:
 ***slab event structure static initialization
 *
 *@Usage:
 ***slab_alloc_event_t slab_event
 ***     = SLAB_ALLOC_EVENT_STATIC_INIT(slab_event, callback, ctx);
 *
 *@parameter:
 *[slab_event]: slab event variable name, non-address
 *[callback]: event callback function (valid when called with 2 arguments)
 *[ctx]: callback context of the event (valid when called with 3 arguments)
 *[priority]: priority of the event (valid when called with 4 arguments)
 *[id]: ID of the event (valid when called with 5 arguments)
 *[state]: state of the event (valid when called with 6 arguments)
 *************************************************************/
/************************************************************
 *@简介：
 ***分配器事件结构体静态初始化
 *
 *@用法：
 ***slab_alloc_event_t slab_event
 ***     = SLAB_ALLOC_EVENT_STATIC_INIT(slab_event, callback, ctx);
 *
 *@参数：
 *[slab_event]：分配器事件变量名，非地址
 *[callback]：事件回调函数
 *[ctx]：事件的回调上下文
 *[priority]：事件的优先级
 *************************************************************/
#define SLAB_ALLOC_EVENT_STATIC_INIT(slab_event, ecb, ctx, priority)    \
{                                                                       \
    EVENT_STATIC_INIT((slab_event).event, (ecb), (ctx), (priority)),    \
    NULL                                                                \
}

#define slab_alloc_event_init(slab_event, ecb, ctx, priority)       \
    do                                                              \
    {                                                               \
        event_init(&(slab_event)->event, (ecb), (ctx), (priority)); \
        (slab_event)->mem_blk = NULL;                               \
    } while (0)

#define slab_alloc_event_init_inherit(slab_event, parent)           \
    do                                                              \
    {                                                               \
        event_init_inherit(&(slab_event)->event, (parent));         \
        (slab_event)->mem_blk = NULL;                               \
    } while (0)


#define SLAB_ALLOC_EVENT_EVENT(slab_event)  (&(slab_event)->event)

#define SLAB_ALLOC_EVENT_NODE(slab_event)   EVENT_NODE(&(slab_event)->event)

#define SLAB_ALLOC_EVENT_OF_EVENT(_event) container_of(_event, slab_alloc_event_t, event)

#define SLAB_ALLOC_EVENT_OF_NODE(node) container_of(EVENT_OF_NODE(node), slab_alloc_event_t, event)

/*********************************************
 *@brief：get the original buffer pointer of slab
 *
 *@parameter:
 *[slab]: slab allocator
 *
 *@return: the original buff of slab
 *********************************************
 */
/*********************************************
 *@简要：获取slab原始的buffer指针
 *
 *@参数
 *[slab] slab分配器
 *
 *@返回：slab的原始的buff
 *********************************************
 */
static inline void *slab_buff_get(slab_t *slab)
{
    return slab->buff;
}


/*********************************************
 *@brief：get the total number of slab
 *
 *@parameter: 
 *[slab] slab allocator
 *
 *@return: the total number of slab
 *********************************************
 */
/*********************************************
 *@简要：获取slab的总块数
 *
 *@参数：
 *[slab] slab分配器
 *
 *@返回：slab总块数
 *********************************************
 */
static inline uint32_t slab_blk_nums_get(slab_t *slab)
{
    return slab->blk_nums;
}


/*********************************************
 *@brief: get the number of blocks used by slab
 *
 *@parameter:
 *[slab] slab allocator
 *
 *@return: the number of blocks used by slab
 *********************************************
 */
/*********************************************
 *@简要：获取slab已使用的块数
 *
 *@参数：
 *[slab] slab分配器
 *
 *@返回：slab已使用的块数
 *********************************************
 */
static inline uint32_t slab_used_get(slab_t *slab)
{
    return slab->nums_used;
}


/*********************************************
 *@brief: get the number of blocks unused by slab
 *
 *@parameter:
 *[slab] slab allocator
 *
 *@return: the number of blocks unused by slab
 *********************************************
 */
/*********************************************
 *@简要：获取slab未使用的块数
 *
 *@参数：
 *[slab] slab分配器
 *
 *@返回：slab未使用的块数
 *********************************************
 */
static inline uint32_t slab_unused_get(slab_t *slab)
{
    return slab->blk_nums - slab->nums_used;
}

/*********************************************
 *@brief: get the size of the slab block
 *
 *@parameter:
 *[slab] slab allocator
 *
 *@return: the size of the slab block
 *********************************************
 */
/*********************************************
 *@简要：获取slab块的大小
 *
 *@参数：
 *[slab] slab分配器
 *
 *@返回：slab块的大小
 *********************************************
 */
static inline uint32_t slab_blk_size_get(slab_t *slab)
{
    return slab->blk_size;
}


/*********************************************
 *@brief: create a slab allocator using buffer initialization
 *
 *@parameter:
 *[buff] buffer
 *[blk_nums] total number of blocks
 *[blk_size] the size of each block
 *
 *@return: slab allocator
 *********************************************
 */
/*********************************************
 *@简要：使用buffer初始化创建一个slab分配器
 *
 *@参数：
 *[buff]     buffer
 *[blk_nums] 总块数
 *[blk_size] 每块的大小
 *
 *@返回：slab分配器
 *********************************************
 */
static inline slab_t *slab_create(void *buff, uint32_t buf_size, uint32_t blk_size)
{
    uint8_t *align_buff;
    slab_t *slab;
    uint32_t size;
    uint8_t *free_node;
    uint32_t i;

    size = buf_size;
    align_buff = (uint8_t *)ALIGN_UP((size_t)buff);
    slab = (slab_t *)align_buff;

    /* ensure sufficient space */
    /* 确保空间足够 */
    if (size >= align_buff - (uint8_t *)buff + sizeof(*slab))
    {
        /* calculate the available size */
        /* 计算可用的size */
        size -= (uint32_t)(align_buff - (uint8_t *)buff + sizeof(*slab));

        /* blk_size cpu-byte alignment */
        /* blk_size CPU字节对齐 */
        slab->blk_size = (uint32_t)ALIGN_UP(blk_size);

        /* calculate the number of total blocks */
        /* 重新计算总的块数 */
        slab->blk_nums = size / slab->blk_size;

        slab->nums_used = 0;

        /* record buff */
        /* 记录buff */
        slab->buff = (uint8_t *)buff; /* for C++ */

        fifo_init(&slab->notify_q);

        /* initialize the free list */
        /* 初始化空闲块链表 */
        slist_init((slist_t *)&slab->free_list);

        /* generates a free block list */
        /* 生成空闲块链表 */
        free_node = align_buff + sizeof(*slab);
        for (i = 0; i < slab->blk_nums; i++)
        {
            slist_node_insert_next(SLIST_HEAD(&slab->free_list), (slist_node_t *)free_node);
            free_node += slab->blk_size;
        }

        return slab;
    }

    return NULL;
}


/*********************************************
 *@brief: the slab allocator allocate memory block
 *
 *@param:
 *[slab] slab allocator
 *
 *@return: memory block
 *********************************************
 */
/*********************************************
 *@简要：slab分配器分配内存块
 *
 *@参数：
 *[slab] slab分配器
 *
 *@返回： 内存块
 *********************************************
 */
static inline void *slab_alloc(slab_t *slab)
{
    /* cannot allocate memory when the queue is empty */
    /* 当队列为空时，不能进行内存分配 */
    if (slist_is_empty(&slab->free_list))
    {
        return NULL;
    }

    slab->nums_used++;

    /* take a free node */
    /* 取出一个空闲节点，并返回 */
    return (void *)slist_node_del_next(&slab->free_list);
}


/*********************************************
 *@brief: the slab allocator free memory block
 *
 *@parameter:
 *[slab] slab allocator
 *[mem] slab block
 *
 *@return: N/A
 *********************************************
 */
/*********************************************
 *@简要：slab分配器释放内存块
 *
 *@参数：
 *[slab] slab分配器
 *[mem] 内存块
 *
 *@返回：无
 *********************************************
 */
static inline void slab_free(slab_t *slab, void *mem)
{
    slab_alloc_event_t *alloc_event;

    /* notification waiter slab is available */
    /* 通知等待者slab已可用 */
    if (!fifo_is_empty(&slab->notify_q))
    {
        alloc_event = SLAB_ALLOC_EVENT_OF_EVENT(event_fifo_priority_pop(&slab->notify_q));
        alloc_event->mem_blk = mem;
        el_event_post(&alloc_event->event);
    }
    else
    {
        /* insert the node into the queue */
        /* 将节点插入队列 */
        slist_node_insert_next(SLIST_HEAD(&slab->free_list), (slist_node_t *)mem);

        slab->nums_used--;
    }
}


/*********************************************
 *@brief: wait for the slab allocator to have a memory block available
 *
 *@param:
 *[slab] slab allocator
 *
 *@return:
 *[true] the slab allocator adds events successfully
 *[false] events are in other queues and cannot be enqueue
 *********************************************
 */
/*********************************************
 *@简要：等待slab分配器有内存块可用
 *
 *@参数：
 *[slab] slab分配器
 *
 *@返回：
 *[true] slab分配器添加事件成功
 *[false] 事件以处于其他队列中，不能入队
 *********************************************
 */
static inline bool slab_wait(slab_t *slab, slab_alloc_event_t *alloc_event)
{
    /* 将事件添加到等待列表 */
    if (slist_node_is_del(SLAB_ALLOC_EVENT_NODE(alloc_event)))
    {
        event_fifo_priority_push(&slab->notify_q, SLAB_ALLOC_EVENT_EVENT(alloc_event));

        /* 若有内存可用，则唤醒等待队列中的一个事件 */
        if (!slist_is_empty(&slab->free_list))
        {
            alloc_event = SLAB_ALLOC_EVENT_OF_EVENT(event_fifo_priority_pop(&slab->notify_q));
            alloc_event->mem_blk = slist_node_del_next(SLIST_HEAD(&slab->free_list));
            el_event_post(&alloc_event->event);
        }

        return true;
    }

    return false;
}


/*********************************************
 *@brief: Cancel the event waiting for the slab allocator
 *
 *@contract: slab_wait_cancel can only cancel events waiting by slab_wait
 * 
 *@param:
 *[slab] slab allocator
 *[alloc_event] allocator event
 *
 *@return:
 *[true] Cancel the slab allocator event successfully
 *[false] Slab allocator event is not in the wait queue
 *********************************************
 */
/*********************************************
 *@简要：取消等待slab分配器的事件
 *
 *@约定：slab_wait_cancel只能取消由slab_wait等待的事件
 * 
 *@参数：
 *[slab] slab分配器
 *[alloc_event] 分配器事件
 *
 *@返回：
 *[true] 取消slab分配器事件成功
 *[false] slab分配器事件不处于等待队列中
 *********************************************
 */
static inline bool slab_wait_cancel(slab_t *slab, slab_alloc_event_t *alloc_event)
{
    if (el_event_is_ready(&alloc_event->event))
    {
        slab_free(slab, alloc_event->mem_blk);
        alloc_event->mem_blk = NULL;

        return el_event_cancel(&alloc_event->event);
    }

    return fifo_del_node(&slab->notify_q, EVENT_NODE(&alloc_event->event));
}

#ifndef TASK_ASSERT
#define TASK_ASSERT(expr)
#endif /* TASK_ASSERT */

#ifndef TASK_INFO
#define TASK_INFO(task, stack_size, stack_used)
#endif /* TASK_INFO */

/*********************************************************
 *@type description:
 *
 *[struct task_cur_ctx_s]：task current context information
 *[struct task_stack_s]：task current stack pointer information
 *[task_t]：task data structure
 *********************************************************
 *@类型说明：
 *
 *[struct task_cur_ctx_s]：task当前的上下文信息
 *[struct task_stack_s]：task当前栈指针信息
 *[task_t]：task数据结构
 *********************************************************/
struct task_cur_ctx_s
{
    uint32_t stack_used;
    uint8_t yield_state;
    uint8_t bp;
};

struct task_stack_s
{
    uint8_t *start;
    uint8_t *end;
    uint8_t *cur;
};

typedef struct task_s
{
    event_t event;
    struct task_stack_s stack;
    struct task_cur_ctx_s cur_ctx;
    union 
    {
        void *ptr;
        uint32_t u32;
        int32_t  s32;
    } ret_val;
    lifo_t task_end_notify_q;
} task_t;


/*********************************************************
 *@type description:
 *
 *[task_asyn_routine_t]: task function and asynchronous function type
 *********************************************************
 *@类型说明：
 *
 *[task_asyn_routine_t]：任务函数与异步函数类型
 *********************************************************/
typedef void (*task_asyn_routine_t)(struct task_s *, event_t *);


/* TASK bpd pointer, Used by bpd coroutine */
/* TASK的bpd指针，由bpd协程使用 */
#define TASK_BPD(task)  (&((task_t *)(task))->cur_ctx.bp)


/************************************************************
 *@brief:
 ***Task data structure static initialization
 *
 *@parameter:
 *[task]: Task variable name, non-pointer
 *[task_func]: Task function
 *[stack]：Task stack
 *[stack_size]：Stack size
 *[priority]：Priority of task events
 *************************************************************/
/************************************************************
 *@简介：
 ***任务数据结构静态初始化
 *
 *@参数：
 *[task]：任务的变量名，非指针
 *[task_func]：任务函数
 *[stack]：任务的栈
 *[stack_size]：栈的大小
 *[priority]：任务事件的优先级
 *************************************************************/
#define TASK_STATIC_INIT(task, stack, stack_size, priority)                     \
{                                                                               \
    EVENT_STATIC_INIT((task).event, (event_cb)NULL_CB, &task, (priority)),      \
    {                                                                           \
        (stack),                                                                \
        (uint8_t *)(stack) + (stack_size),                                      \
        (stack),                                                                \
    },                                                                          \
    {0, 0, BP_INIT_VAL},                                                        \
    {0},                                                                        \
    LIFO_STATIC_INIT((task).task_end_notify_q)                                  \
}


/************************************************************
 *@brief:
 ***Define a task object, and initialize
 *
 *@parameter:
 *[_name]：The name of the task object
 *[task_func]：Task function
 *[stack_size]：Stack size
 *[priority]：Priority of task events
 *************************************************************/
/************************************************************
 *@简介：
 ***定义一个任务对象，并初始化
 *
 *@参数：
 *[_name]：任务对象的名字
 *[task_func]：任务函数
 *[stack_size]：任务栈的大小
 *[priority]：任务事件的优先级
 *************************************************************/
#define TASK_DEFINE(_name, stack_size, priority)                    \
    uint8_t _name##_stack_buf[stack_size];                          \
    task_t  _name = TASK_STATIC_INIT(_name,                         \
                                     _name##_stack_buf,             \
                                     (stack_size),                  \
                                     (priority))


/************************************************************
 *@brief:
 ***Define a task object using static modifiers, and initialize
 *
 *@parameter:
 *[_name]：The name of the task object
 *[task_func]：Task function
 *[stack_size]：Stack size
 *[priority]：Priority of task events
 *************************************************************/
/************************************************************
 *@简介：
 ***使用static修饰定义一个任务对象，并初始化
 *
 *@参数：
 *[_name]：任务对象的名字
 *[task_func]：任务函数
 *[stack_size]：任务栈的大小
 *[priority]：任务事件的优先级
 *************************************************************/
#define TASK_DEFINE_STATIC(_name, stack_size, priority)             \
    static uint8_t _name##_stack_buf[stack_size];                   \
    static task_t  _name = TASK_STATIC_INIT(_name,                  \
                                            _name##_stack_buf,      \
                                            (stack_size),           \
                                            (priority))

/* The size of task context information to be saved when making an asynchronous call */
/* 进行异步调用时需要保存的任务上下文信息大小 */
#define TASK_STACK_CTX_SIZE (sizeof(struct task_cur_ctx_s) + sizeof(event_cb))

/* Simple macro, return a - b to ensure greater than or equal to 0 */
/* 简单的宏，返回a - b保证大于等于0 */
#define _SUB_BEZ(a, b)  ((a) > (b) ? ((a) - (b)) : 0)


/************************************************************
 *@brief:
 ***Task data structure dynamic initialization,
 ***if the task has been statically initialized, 
 ***you don't have to use this function.
 *
 *@parameter:
 *[task]: Task variable name, non-pointer
 *[task_func]: Task function
 *[stack]：Task stack
 *[stack_size]：Stack size
 *[priority]：Priority of task events
 *************************************************************/
/************************************************************
 *@简介：
 ***任务数据结构动态初始化，若task已静态初始化，则不必使用该函数
 *
 *@参数：
 *[task]：任务的变量名，非指针
 *[task_func]：任务函数
 *[stack]：任务的栈
 *[stack_size]：栈的大小
 *[priority]：任务事件的优先级
 *************************************************************/
static inline void task_init(task_t *task, void *stack, size_t stack_size, uint8_t priority)
{
    event_init(&task->event, (event_cb)NULL_CB, task, priority);

    task->stack.start = (uint8_t *)ALIGN_UP((size_t)stack);
    task->stack.end = task->stack.start + _SUB_BEZ(stack_size, (size_t)(task->stack.start - (uint8_t *)stack));
    task->stack.cur = task->stack.start;

    task->ret_val.ptr = NULL;

    task->cur_ctx.stack_used = 0;
    task->cur_ctx.bp = BP_INIT_VAL;
    task->cur_ctx.yield_state = 0;
    lifo_init(&task->task_end_notify_q);
}


/************************************************************
 *@brief:
 ***start task
 *
 *@parameter:
 *[task]: Task object, cannot be empty
 *************************************************************/
/************************************************************
 *@简介：
 ***启动任务
 *
 *@参数：
 *[task]：任务对象，不能为空
 *[task_func]: 任务函数，任务函数应该被声明为 void task_func(task_t *task, event_t *ev, 参数1, 参数2..)格式
 *[...]: 任务函数的第3个及之后的参数
 *************************************************************/
#define task_start(task, task_func, ...)                        \
    do {                                                        \
        EVENT_CALLBACK(&(task)->event) = (event_cb)(task_func); \
        (task_func)((task), NULL, ##__VA_ARGS__);               \
    } while (0)

/*********************************************************
 *@brief: 
 ***Get the asynchronous variables in the current task stack.
 ***
 ***task will generate the operation of allocating memory 
 ***when it is called for the first time. 
 ***After bpd_yield causes bp to not be 0,
 ***get again, return to the previous memory
 *
 *@contract: 
 ***Cannot use null pointer
 *
 *@parameter:
 *[task]: task object
 *[vars_size]: The size of the asynchronous variables
 *
 *@return: Memory address of an asynchronous variables
 *********************************************************/
/*********************************************************
 *@简要：
 ***获取当前task堆栈中的异步变量
 ***
 ***task在首次调用时，将产生分配内存的操作，
 ***bpd_yield导致bp不为0后，再次获取，则返回以前的内存
 *
 *@约定：
 ***不能使用空指针
 *
 *@参数：
 *[task]：任务对象
 *[vars_size]：异步变量的大小
 *
 *@返回：异步变量的内存地址
 **********************************************************/
static inline void *task_asyn_vars_get(task_t *task, size_t vars_size)
{
    if (task->cur_ctx.bp == BP_INIT_VAL)
    {
        size_t alloc_size = ALIGN_UP(vars_size);

        TASK_INFO((task), (task->stack.end - task->stack.start), (task->stack.cur + alloc_size - task->stack.start));
        TASK_ASSERT(task->stack.cur + alloc_size <= task->stack.end);

        task->cur_ctx.stack_used = alloc_size;
    }

    return task->stack.cur;
}


/*********************************************************
 *@brief: 
 ***The function called asynchronously returns to the caller
 ***
 ***Any function called by task_bpd_asyn_call should use this method to return to the caller
 *
 *@contract: 
 ***Cannot use null pointer
 *
 *@parameter:
 *[task]: task object
 *********************************************************/
/*********************************************************
 *@简要：
 ***被异步调用的函数返回到调用者
 ***
 ***任何由task_bpd_asyn_call调用的函数，应当使用此方法返回到调用者
 *
 *@约定：
 ***不能使用空指针
 *
 *@参数：
 *[task]：任务对象
 **********************************************************/
static inline void task_asyn_return(task_t *task)
{
    uint8_t yield_state;

    if (task->stack.cur >= task->stack.start + TASK_STACK_CTX_SIZE)
    {
        /* Record yielded state */
        /* 记下是否yield过状态 */
        yield_state = task->cur_ctx.yield_state;
        /* Restore caller context information and event callbacks */
        /* 恢复调用者上下文信息及事件回调 */
        task->stack.cur -= TASK_STACK_CTX_SIZE;
        task->cur_ctx = *(struct task_cur_ctx_s *)task->stack.cur;
        EVENT_CALLBACK(&task->event) = *(event_cb *)(task->stack.cur + sizeof(struct task_cur_ctx_s));
        task->stack.cur -= task->cur_ctx.stack_used;

        TASK_ASSERT(task->stack.cur >= task->stack.start && task->stack.cur <= task->stack.end);

        if (yield_state)
        {
            /* Immediate return to the caller */
            /* 立即返回调用者 */
            el_event_sync_post(&task->event);
        }
    }
    /* task end */
    /* 任务结束 */
    else
    {
        task->stack.cur = task->stack.start;
        task->ret_val.ptr = NULL;
        task->cur_ctx.stack_used = 0;
        task->cur_ctx.bp = BP_INIT_VAL;
        task->cur_ctx.yield_state = 0;
        EVENT_CALLBACK(&(task)->event) = (event_cb)NULL_CB;

        while (!lifo_is_empty(&task->task_end_notify_q))
        {
            /* Asynchronous post events can avoid errors caused by freeing task in callbacks */
            /* 异步提交事件可避免在回调中释放task而引起错误 */
            el_event_post(EVENT_OF_NODE(lifo_pop(&task->task_end_notify_q)));
        }
    }
}


/*********************************************************
 *@brief: 
 ***Determine if the coroutine task is end
 *
 *@return:
 *[true]: task is end
 *[false]: task not end
 *********************************************************/
/*********************************************************
 *@简要：
 ***判断协程任务是否结束
 *
 *@返回：
 *[true]：任务已结束
 *[false]: 任务未结束
 **********************************************************/
static inline bool task_is_end(task_t *task)
{
    return EVENT_CALLBACK(&(task)->event) == (event_cb)NULL_CB;
}


/*********************************************************
 *@brief: 
 ***Add a listen event for the end action of the coroutine task,
 ***and trigger the listen event when the coroutine task ends.
 *
 *@contract: 
 ***The task_asyn_return must be called at 
 ***the end of the topmost coroutine (started by task_start)
 *
 *@parameter:
 *[task]: task object
 *[task_end_notify_ev]: Event for listening to coroutine exit
 *
 *@return:
 *[true]: Add a listen event successfully
 *[false]: Listening events are in the reference state or in the queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***为协程任务的结束动作添加一个监听事件，协程任务结束时将触发
 ***监听的事件
 *
 *@约定：
 ***1、最上层的协程(由task_start启动)结束时必须调用task_asyn_return
 *
 *@参数：
 *[task]：任务对象
 *[task_end_notify_ev]：用于监听协程退出的事件
 *
 *@返回值：
 *[true]：添加监听事件成功
 *[false]：监听事件处于引用状态或者队列中
 **********************************************************/
static inline bool task_end_wait(task_t *task, event_t *task_end_notify_ev)
{
    if (!slist_node_is_del(EVENT_NODE(task_end_notify_ev)))
    {
        return false;
    }

    lifo_push(&task->task_end_notify_q, EVENT_NODE(task_end_notify_ev));

    return true;
}


/*********************************************************
 *@brief: 
 ***Cancel the event listener for task_end_wait
 *
 *@parameter:
 *[task]: task object
 *[task_end_notify_ev]: The event used by task_end_wait
 *
 *@return:
 *[true]: Cancel the listen event successfully
 *[false]: The listened event is no in the listen queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***取消task_end_wait的事件监听
 *
 *@参数：
 *[task]：任务对象
 *[task_end_notify_ev]：传入task_end_wait的事件
 *
 *@返回值：
 *[true]：取消监听事件成功
 *[false]：监听的事件已经不在监听队列之中
 **********************************************************/
static inline bool task_end_wait_cancel(task_t *task, event_t *task_end_notify_ev)
{
    if (slist_node_is_del(EVENT_NODE(task_end_notify_ev)))
    {
        return false;
    }

    if (el_event_is_ready(task_end_notify_ev))
    {
        return el_event_cancel(task_end_notify_ev);
    }
    else
    {
        return lifo_del_node(&task->task_end_notify_q, EVENT_NODE(task_end_notify_ev));
    }
}


/************************************************************
 *@brief:
 ***Saves the current context to the stack and initializes new context information
 ***
 ***This function is used by task_bpd_asyn_call and should not be used directly.
 *
 *@parameter:
 *[task]: task object
 *[afunc]: Called asynchronous function
 *************************************************************/
/************************************************************
 *@简介：
 ***将当前上下文保存到栈中，并初始化新的上下文信息
 ***
 ***此函数由task_bpd_asyn_call使用，不应该被直接使用
 *
 *@参数：
 *[task]：任务对象
 *[afunc]：被调用的异步函数
 *************************************************************/
static inline void task_asyn_call_prepare(task_t *task, task_asyn_routine_t afunc)
{
    TASK_ASSERT(task->stack.cur + task->cur_ctx.stack_used + TASK_STACK_CTX_SIZE <= task->stack.end);

    /* Save current context information and event callback */
    /* 保存当前上下文信息以及事件回调 */
    task->stack.cur += task->cur_ctx.stack_used;
    *(struct task_cur_ctx_s *)task->stack.cur = task->cur_ctx;
    *(event_cb *)(task->stack.cur + sizeof(struct task_cur_ctx_s)) = EVENT_CALLBACK(&task->event);
    task->stack.cur += TASK_STACK_CTX_SIZE;

    /* Initialize new context information and event callback */
    /* 初始化新的上下文信息和事件回调 */
    task->cur_ctx.bp = BP_INIT_VAL;
    task->cur_ctx.stack_used = 0;
    task->cur_ctx.yield_state = 0;
    EVENT_CALLBACK(&task->event) = (event_cb)afunc;
}


/*********************************************************
 *@brief: 
 ***Asynchronously call an asynchronous function 
 ***and wait for the asynchronous function to return using task_asyn_return
 *
 *@contract: 
 ***Cannot use null pointer
 *
 *@parameter:
 *[bp_num]：For asynchronous return, bpd breakpoint number
 *[task]：Task object
 *
 *[afunc]：The asynchronous function being called, 
 ***the asynchronous function should be declared 
 ***as a format such as void func(task_t *task, event_t *ev, parameter 1, parameter 2..)
 *
 *[...]：The second argument after the called asynchronous function
 *********************************************************/
/*********************************************************
 *@简要：
 ***异步地调用异步函数，并等待异步函数使用task_asyn_return返回
 *
 *@约定：
 ***不能使用空指针
 *
 *@参数：
 *[bp_num]：用于异步返回的，bpd断点号
 *[task]：任务对象
 *[afunc]：被调用的异步函数，异步函数应该被声明为 void func(task_t *task, event_t *ev, 参数1, 参数2..)等格式
 *[...]：被调用的异步函数的第二个之后的参数
 **********************************************************/
#define task_bpd_asyn_call(bp_num, task, afunc, ...)                \
    do {                                                            \
        bpd_set(bp_num);                                            \
        *(void **)&bpd = (void *)task->stack.cur;                   \
        task_asyn_call_prepare((task), (task_asyn_routine_t)afunc); \
        afunc((task), (event_t *)NULL, ##__VA_ARGS__);              \
        if ((void *)bpd != (void *)task->stack.cur)                 \
        {                                                           \
            task->cur_ctx.yield_state = 1;                          \
            return ;                                                \
        }                                                           \
        bpd = TASK_BPD(task);                                       \
        bpd_restore_point(bp_num):;                                 \
    } while (0)


/********************************************************
 * atask dependent interface
 * atask依赖接口
 ********************************************************
 */

/* get the current time, unit is number of clocks */
/* 获取当前时间时钟数 */
extern time_nclk_t time_nclk_get(void);

/* get the current time, unit is millisecond */
/* 获取当前时间微秒数 */
extern time_us_t time_us_get(void);

/* convert the clocks to microseconds */
/* 将时钟数转为微秒 */
extern time_us_t time_nclk_to_us(time_nclk_t time_nclk);

/* convert the microseconds to clocks */
/* 将微秒转为时钟数 */
extern time_nclk_t time_us_to_nclk(time_us_t time_us);

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE

/*********************************************************
*@brief:
***Scheduling the preparation interface,
***scheduling event loop events at the right time,
***implemented by an external framework
***is generally implemented as:
***1. wake up scheduling when event loop timer expires
***2. when the event loop has an event pending, wake up scheduling immediately
*
*********************************************************/
/*********************************************************
*@简要：
***调度准备接口，在合适的时机进行事件循环事件调度，由外部框架实现，
***一般实现为：
***1、在事件循环定时器到期时唤醒调度
***2、事件循环有事件挂起时，立即唤醒调度
*
**********************************************************/
extern void el_schedule_prepare(void);

#endif /* CONFIG_EL_HAVE_SCHEDULE_PREPARE */

#ifdef __cplusplus 
}
#endif

#endif // __LIB_ATASK_H__
