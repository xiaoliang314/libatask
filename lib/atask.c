/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */
#include "atask.h"

el_t dflt_el = EL_STATIC_INIT(dflt_el);

const uint8_t priority_ready_bitmap[(1 << READY_GROUP_COUNT)] =
{
/*     000, 001, 010, 011, 100, 101, 110, 111 */
/* 0 */0xFF, 0,   1,   1,   2,   2,   2,   2,
/* 1 */3,    3,   3,   3,   3,   3,   3,   3
};

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE

static inline void el_schedule_prepare_no_recursion(void)
{
    if (dflt_el.recursion_schedule == 0)
    {
        el_schedule_prepare();
    }
}
#endif /* CONFIG_EL_HAVE_SCHEDULE_PREPARE */

/* post event to event loop */
/* 提交事件到事件循环 */
bool el_event_post(event_t *e)
{
    uint8_t ready_group = e->priority >> READY_GROUP_PRIORITY_SHIFT;
    uint8_t el_old_have_event;

    /* The event node must be in an idle state */
    /* 事件节点必须处于空闲状态 */
    if (slist_node_is_del(EVENT_NODE(e)))
    {
        /* Add events to the corresponding event group */
        /* 添加事件到相应的事件组 */
        event_fifo_priority_push(&dflt_el.ready_groups[ready_group], e);

        /* set to ready state */
        /* 设置为就绪态 */
        e->is_ready = 1;

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE
        el_old_have_event = el_have_imm_event();
#endif

        /* Update event group ready map */
        /* 更新事件组就绪图 */
        dflt_el.ready_map |= (1 << ready_group);

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE
        if (!el_old_have_event)
        {
            el_schedule_prepare_no_recursion();
        }
#endif

        return true;
    }

    return false;
}

/* Cancel the event in event loop */
/* 取消事件循环中的事件 */
bool el_event_cancel(event_t *e)
{
    uint8_t ready_group = e->priority >> READY_GROUP_PRIORITY_SHIFT;
    fifo_t *ready_q = &dflt_el.ready_groups[ready_group];

    /* Node is not idle that delete this from the event group */
    /* 节点非空闲状态则从事件组中删除 */
    if (!slist_node_is_del(EVENT_NODE(e))
     && fifo_del_node(ready_q, EVENT_NODE(e)))
    {
        /* Update the ready map if the event group is empty */
        /* 若事件组为空，则更新就绪图 */
        if (fifo_is_empty(ready_q))
        {
            dflt_el.ready_map &= ~(1 << ready_group);
        }

        e->is_ready = 0;

        return true;
    }

    return false;
}


/* Reset the priority of posted events */
/* 重新设置已提交的事件的优先级 */
bool el_event_reset_priority(event_t *e, uint8_t new_priority)
{
    uint8_t cur_group = e->priority >> READY_GROUP_PRIORITY_SHIFT;
    uint8_t new_group = new_priority >> READY_GROUP_PRIORITY_SHIFT;
    fifo_t *cur_ready_q = &dflt_el.ready_groups[cur_group];
    fifo_t *new_ready_q = &dflt_el.ready_groups[new_group];

    if (slist_node_is_del(EVENT_NODE(e)))
    {
        return false;
    }

    if (cur_group == new_group)
    {
        return event_fifo_reset_priority(cur_ready_q, e, new_priority);
    }
    else
    {
        if (fifo_del_node(cur_ready_q, EVENT_NODE(e)))
        {
            if (fifo_is_empty(cur_ready_q))
            {
                dflt_el.ready_map &= ~(1 << cur_group);
            }

            event_fifo_priority_push(new_ready_q, e);
            dflt_el.ready_map &= ~(1 << new_group);

            return true;
        }

        return false;
    }
}


/* Inline version event scheduling */
/* 内联版事件调度 */
static inline void _el_event_schedule(void)
{
    uint8_t ready_group;
    event_t *e;
    fifo_t *ready_q;

    if (dflt_el.ready_map)
    {
        ready_group = priority_ready_bitmap[dflt_el.ready_map];
        ready_q = &dflt_el.ready_groups[ready_group];

        e = event_fifo_priority_pop(ready_q);
        if (fifo_is_empty(ready_q))
        {
            dflt_el.ready_map &= ~(1 << ready_group);
        }

        e->is_ready = 0;
        e->callback(e->context, e);
    }
}


/* Inline version timer scheduling */
/* 内联版定时器调度 */
static inline void _el_timer_schedule(void)
{
    time_nclk_t nclk_now;
    timer_event_t *timer;
    slist_node_t *cur_node;
    slist_node_t *prev_node;
    slist_node_t *safe_node;

    if (dflt_el.timers_have && dflt_el.due <= (nclk_now = time_nclk_get()))
    {
        dflt_el.timers_have  = 0;

        slist_foreach_record_prev_safe(FIFO_LIST(&dflt_el.timers), cur_node, prev_node, safe_node)
        {
            timer = TIMER_OF_NODE(cur_node);

            if (timer->due <= nclk_now)
            {
                fifo_foreach_safe_del_next(&dflt_el.timers, prev_node, &safe_node);

                el_event_post(TIMER_EVENT(timer));
            }
            else
            {
                dflt_el.timers_have = 1;
                dflt_el.due = timer->due;

                break;
            }
        }
    }
}


/* Kernel scheduling once */
/* 事件循环调度一次 */
time_nclk_t el_schedule(void)
{
    int max_schedule_events = EL_ONCE_SCHEDULE_MAX_EVENT_COUNT;

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE
    dflt_el.recursion_schedule++;
#endif

    _el_timer_schedule();

    while (el_have_imm_event() && max_schedule_events--)
    {
        _el_event_schedule();
    }

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE
    dflt_el.recursion_schedule--;
#endif

    return el_have_imm_event() ? 0 : el_timer_recent_due_get();
}


/* start timer */
/* 启动定时器 */
bool el_timer_start_due(timer_event_t *timer, time_nclk_t due)
{
    timer_event_t *find;
    slist_node_t *prev_node;
    slist_node_t *cur_node;

    if (!slist_node_is_del(TIMER_NODE(timer)))
    {
        return false;
    }

    timer->due = due;

    find = TIMER_OF_NODE(FIFO_TAIL(&dflt_el.timers));
    if (fifo_is_empty(&dflt_el.timers)
     || find->due <= timer->due)
     {
         fifo_push(&dflt_el.timers, TIMER_NODE(timer));
     }
     else
     {
         slist_foreach_record_prev(FIFO_LIST(&dflt_el.timers), cur_node, prev_node)
         {
             find = TIMER_OF_NODE(cur_node);
             if (find->due > timer->due)
             {
                 fifo_node_insert_next(&dflt_el.timers, prev_node, TIMER_NODE(timer));
                 break;
             }
         }
     }

     if (!dflt_el.timers_have || timer->due < dflt_el.due)
     {
        dflt_el.due = timer->due;
        dflt_el.timers_have = 1;

#ifdef CONFIG_EL_HAVE_SCHEDULE_PREPARE
         if (!el_have_imm_event())
         {
             el_schedule_prepare_no_recursion();
         }
#endif
     }

     return true;
}


/* stop timer */
/* 停止定时器 */
bool el_timer_stop(timer_event_t *timer)
{
    if (slist_node_is_del(TIMER_NODE(timer)))
    {
        return false;
    }

    if (el_event_is_ready(TIMER_EVENT(timer)))
    {
        return el_event_cancel(TIMER_EVENT(timer));
    }
    else if (fifo_del_node(&dflt_el.timers, TIMER_NODE(timer)))
    {
        if (fifo_is_empty(&dflt_el.timers))
        {
            dflt_el.timers_have = 0;
        }
        else
        {
            dflt_el.due = TIMER_OF_NODE(FIFO_TOP(&dflt_el.timers))->due;
        }

        return true;
    }

    return false;
}


/* Trigger timer immediately */
/* 立即触发定时器 */
bool el_timer_trigger(timer_event_t *timer)
{
    if (el_event_is_ready(TIMER_EVENT(timer)))
    {
        return true;
    }
    else
    {
        if (!slist_node_is_del(TIMER_NODE(timer)))
        {
            if (!fifo_del_node(&dflt_el.timers, TIMER_NODE(timer)))
            {
                return false;
            }

            if (fifo_is_empty(&dflt_el.timers))
            {
                dflt_el.timers_have = 0;
            }
            else
            {
                dflt_el.due = TIMER_OF_NODE(FIFO_TOP(&dflt_el.timers))->due;
            }
        }

        timer->due = 0;
        return el_event_post(TIMER_EVENT(timer));
    }
}
