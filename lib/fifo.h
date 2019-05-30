/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#ifndef __LWES_FIFO_H__
#define __LWES_FIFO_H__

#include "slist.h"


/*********************************************************
 *@type description:
 *
 *[fifo_t]: first-in-first-out queue
 *********************************************************
 *@类型说明：
 *
 *[fifo_t]：先进先出队列
 *********************************************************/
typedef struct fifo_s
{
    /* single linked list for implementing first-in-first-out queues */
    /* 用于实现先进先出队列的单链表 */
    slist_t list;

    /* recording the tail node of the linked list */
    /* 记录着链表的尾节点 */
    slist_node_t *tail;
} fifo_t;


/************************************************************
 *@brief:
 ***first-in-first-out queue structure static initialization
 *
 *@Usage:
 ***fifo_t fifo = FIFO_STATIC_INIT(fifo);
 *
 *@parameter:
 *[head]: first-in-first-out queue variable name, non-address
 *************************************************************/
/************************************************************
 *@简介：
 ***先进先出队列结构体静态初始化
 *
 *@用法：
 ***fifo_t fifo = FIFO_STATIC_INIT(fifo);
 *
 *@参数：
 *[head]：先进先出队列变量名，非地址
 *************************************************************/
#define FIFO_STATIC_INIT(fifo) {SLIST_STATIC_INIT(fifo.list), &fifo.list}

/************************************************************
 *@brief:
 ***first-in-first-out queue structure static initialization
 *
 *@Usage:
 ***fifo_t fifo = FIFO_STATIC_INIT(fifo);
 *
 *@parameter:
 *[head]: first-in-first-out queue variable name, non-address
 *************************************************************/
/************************************************************
 *@简介：
 ***先进先出队列结构体静态初始化
 *
 *@用法：
 ***fifo_t fifo = FIFO_STATIC_INIT(fifo);
 *
 *@参数：
 *[head]：先进先出队列变量名，非地址
 *************************************************************/
#define FIFO_STATIC_INIT(fifo) {SLIST_STATIC_INIT(fifo.list), &fifo.list}


/************************************************************
 *@brief:
 ***Get the single circular linked list in fifo for traversing fifo
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the single circular linked list in fifo
 *************************************************************/
/************************************************************
 *@简介：
 ***获取fifo中的单循环链表，用于遍历fifo
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回：先进先出队列的单循环链表
 *************************************************************/
#define FIFO_LIST(fifo)      (&(fifo)->list)


/************************************************************
 *@brief:
 ***Get the linked list in fifo for traversing fifo
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the linked list in fifo
 *************************************************************/
/************************************************************
 *@简介：
 ***获取fifo中的链表，用于遍历fifo
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回：先进先出队列的链表
 *************************************************************/
#define FIFO_OF_LIST(slist)      (container_of(slist, fifo_t, list))

/************************************************************
 *@brief:
 ***Get the single circular linked list in fifo for traversing fifo
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the single circular linked list in fifo
 *************************************************************/
/************************************************************
 *@简介：
 ***获取fifo中的单循环链表，用于遍历fifo
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回：先进先出队列的单循环链表
 *************************************************************/
#define FIFO_LIST(fifo)      (&(fifo)->list)


/************************************************************
 *@brief:
 ***Get the linked list in fifo for traversing fifo
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the linked list in fifo
 *************************************************************/
/************************************************************
 *@简介：
 ***获取fifo中的链表，用于遍历fifo
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回：先进先出队列的链表
 *************************************************************/
#define FIFO_OF_LIST(slist)      (container_of(slist, fifo_t, list))


/*********************************************************
 *@brief: 
 ***first-in-first-out queue initialization.
 *
 *@contract: 
 ***1. "fifo" is not null pointer
 ***2. cannot initialize the "fifo" being used
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***先进先出队列初始化
 *
 *@约定：
 ***1、fifo不是空指针
 ***2、不可对正在使用的fifo进行初始化
 *
 *@参数：
 *[fifo]：先进先出队列
 **********************************************************/
static inline void fifo_init(fifo_t *fifo)
{
    slist_init(&fifo->list);
    fifo->tail = SLIST_HEAD(&fifo->list);
}


/*********************************************************
 *@brief: 
 ***determine if the fifo queue is empty
 *
 *@contract: 
 ***1. fifo is not null pointer
 *
 *@parameter:
 *[fifo]: first-in-fist-out queue
 *
 *@return value:
 *[true]: first-in-first-out is empty
 *[false]: first-in-fist-out is not empty
 *********************************************************/
/*********************************************************
 *@简要：
 ***判断先进先出队列是否为空
 *
 *@约定：
 ***1、fifo不是空指针
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回值：
 *[true]：先进先出队列为空
 *[false]：先进先出队列非空
 **********************************************************/
#define fifo_is_empty(fifo) slist_is_empty(&(fifo)->list)


/*********************************************************
 *@brief: 
 ***first-in-first-out queue enqueue
 *
 *@contract: 
 ***1. fifo and node are not null pointer
 ***2. the "node" is the deleted node
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *[node]: the node that needs to be enqueued
 *********************************************************/
/*********************************************************
 *@简要：
 ***先进先出队列入队
 *
 *@约定：
 ***1、fifo与node不是空指针
 ***2、node为已删除的节点
 *
 *@参数：
 *[fifo]：先进先出队列
 *[node]：需要入队的节点
 **********************************************************/
static inline void fifo_push(fifo_t *fifo, slist_node_t *node)
{
    slist_node_insert_next(fifo->tail, node);
    fifo->tail = node;
}


/*********************************************************
 *@brief: 
 ***first-in-first-out queue dequeue
 *
 *@contract: 
 ***1. "fifo" is not null pointer
 ***2. "fifo" is not empty
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the node of dequeuing
 *********************************************************/
/*********************************************************
 *@简要：
 ***先进先出队列出队
 *
 *@约定：
 ***1、fifo不是空指针
 ***2、fifo非空
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回：出队的节点
 **********************************************************/
static inline slist_node_t* fifo_pop(fifo_t *fifo)
{
    slist_node_t *first_node;

    first_node = slist_node_del_next(SLIST_HEAD(&fifo->list));

    if (slist_is_empty(&fifo->list))
    {
        fifo->tail = SLIST_HEAD(&fifo->list);
    }

    return first_node;
}


/*********************************************************
 *@brief: 
 ***get the top node of first-in-fist-out queue
 *
 *@contract: 
 ***1. fifo is not null pointer
 ***2. fifo is not empty
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the top node of first-in-first-out queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***获取先进先出队列的顶部节点
 *
 *@约定：
 ***1、fifo不是空指针
 ***2、fifo非空
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回类型：
 *[slist_node_t *]：单循环链表的节点
 *
 *@返回值：先进先出队列的顶部节点
 **********************************************************/
#define FIFO_TOP(fifo)  SLIST_NODE_NEXT(SLIST_HEAD(&(fifo)->list))


/*********************************************************
 *@brief: 
 ***get the tail node of first-in-fist-out queue
 *
 *@contract: 
 ***1. fifo is not null pointer
 ***2. fifo is not empty
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *
 *@return: the tail node of first-in-first-out queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***获取先进先出队列的尾部节点
 *
 *@约定：
 ***1、fifo不是空指针
 ***2、fifo非空
 *
 *@参数：
 *[fifo]：先进先出队列
 *
 *@返回：先进先出队列的尾部节点
 **********************************************************/
#define FIFO_TAIL(fifo)   (fifo)->tail


/*********************************************************
 *@brief: 
 ***insert the next node at the node of the-first-in-first-out queue
 *
 *@contract: 
 ***1. "fifo", "node" and "next_node" are not null pointers
 ***2. "node" is the node in the first-in-first-out queue
 ***3. "next_node" is the deleted node
 *
 *@parameter:
 *[node]: "node" is the node in first-in-first-out queue
 *[next_node]: "next_node" is the node of be inserted
 *********************************************************/
/*********************************************************
 *@简要：
 ***在先进先出队列的节点处插入下一个节点
 *
 *@约定：
 ***1、fifo、node和next_node不是空指针
 ***2、node为先进先出队列中的节点
 ***3、next_node为已删除的节点
 *
 *@参数：
 *[node]：先进先出队列中的节点
 *[next_node]：需要插入的节点
 **********************************************************/
static inline void fifo_node_insert_next(fifo_t *fifo, slist_node_t *node, slist_node_t *next_node)
{
    slist_node_insert_next(node, next_node);
    if (node == fifo->tail)
    {
        fifo->tail = next_node;
    }
}


/*********************************************************
 *@brief: 
 ***remove the next node of the node in the-first-in-first-out queue,
 ***this function is usually used in the traversal process
 *
 *@contract: 
 ***1. "node" and "fifo" are not a null pointer
 ***2. "node" is in the first-in-first-out queue
 ***3. "node" is not the tail node of the first-in-first-out queue
 *
 *@parameter:
 *[node]: the node in first-in-first-out queue
 *
 *@return type:
 *[slist_node_t*]: the node of single circular linked list
 *
 *@return: the node of be deleted
 *********************************************************/
/*********************************************************
 *@简要：
 ***移除先进先出队列中节点的下一个节点，
 ***该函数通常在遍历过程中使用
 *
 *@约定：
 ***1、node与fifo不是空指针
 ***2、node处于先进先出队列中
 ***3、node不是先进先出队列的尾节点
 *
 *@参数：
 *[node]：先进先出队列中的节点
 *
 *@返回类型：
 *[slist_node_t*]：单循环链表的节点
 *
 *@返回：被移除的节点
 **********************************************************/
static inline slist_node_t* fifo_node_del_next(fifo_t *fifo, slist_node_t *node)
{
    slist_node_t *next_node;

    next_node = slist_node_del_next(node);
    if (next_node == fifo->tail)
    {
        fifo->tail = node;
    }

    return next_node;
}


/*********************************************************
 *@brief: 
 ***remove node from first-in-first-out queue
 *
 *@parameter:
 *[fifo]: first-in-first-out queue
 *[node]: removed node
 *
 *@return value:
 *[true]: successfully removed this node from the queue
 *[false]: this node is not in this queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***从先进先出队列中移除节点
 *
 *@参数：
 *[fifo]：先进先出队列
 *[node]: 被移除的节点
 *
 *@返回值：
 *[true]：成功从队列中移除这个节点
 *[false]：这个节点不在这个队列之中
 **********************************************************/
static inline bool fifo_del_node(fifo_t *fifo, slist_node_t *node)
{
    slist_node_t *prev_node;
    slist_node_t *find_node;

    /* traverses the list,
     * looking for the previous position of the node */
    /* 遍历列表，寻找node的前一个位置 */
    slist_foreach_record_prev(&fifo->list, find_node, prev_node)
    {
        if (find_node == node)
        {
            slist_node_del_next(prev_node);

            /* If the current element is a tail element,
             * update the tail */
            /* 若当前元素为队尾元素，则更新队尾 */
            if (find_node == fifo->tail)
            {
                fifo->tail = prev_node;
            }

            return true;
        }
    }

    return false;
}


/*********************************************************
 *@brief: 
 ***safely insert the node to the FIFO queue
 *
 *@contract:
 ***1. "node" is the node in the FIFO queue
 ***2. next_node is the deleted node
 *
 *@parameter:
 *[fifo]: FIFO queue
 *[node]: the node of the operation
 *[next_node]: the next node to insert
 *[safe_node]: safe node used during traversal
 *********************************************************/
/*********************************************************
 *@简要：
 ***安全地将节点插入到先进先出队列
 *
 *@约定：
 ***1、node为先进先出队列中的节点
 ***2、next_node为已删除的节点
 *
 *@参数：
 *[fifo]：先进先出队列
 *[node]: 操作的节点
 *[next_node]: 要插入的下一个节点
 *[safe_node]: 遍历过程中使用的安全节点
 **********************************************************/
static inline void fifo_node_insert_next_safe(fifo_t *fifo, slist_node_t *node, slist_node_t *next_node, slist_node_t **safe_node)
{
    slist_node_insert_next_safe(node, next_node, safe_node);
    if (node == fifo->tail)
    {
        fifo->tail = next_node;
    }
}


/*********************************************************
 *@brief: 
 ***safely remove the next node from the FIFO queue
 *
 *@contract:
 ***1. "node" is the node in the FIFO queue
 ***2. "node" is not the fifo tail node
 *
 *@parameter:
 *[fifo]: FIFO queue
 *[node]: the node of the operation
 *[safe_node]: safe node used during traversal
 *
 *@return value: the next node to be removed
 *********************************************************/
/*********************************************************
 *@简要：
 ***从先进先出队列中安全移除下一个节点
 *
 *@约定：
 ***1、node为先进先出队列中的节点
 ***2、node不是尾节点
 *
 *@参数：
 *[fifo]：先进先出队列
 *[node]: 操作的节点
 *[safe_node]: 遍历过程中使用的安全节点
 *
 *@返回值: 被移除的下一个节点
 **********************************************************/
static inline slist_node_t *fifo_node_del_next_safe(fifo_t *fifo, slist_node_t *node, slist_node_t **safe_node)
{
    slist_node_t *next_node;

    next_node = slist_node_del_next_safe(node, safe_node);
    if (next_node == fifo->tail)
    {
        fifo->tail = node;
    }

    return next_node;
}


/*********************************************************
 *@brief: 
 ***safely remove the node from the FIFO queue
 *
 *@parameter:
 *[fifo]: FIFO queue
 *[node]: removed node
 *[safe_node]: safe node used during traversal
 *
 *@return value:
 *[true]: successfully removed this node from the queue
 *[false]: this node is not in this queue
 *********************************************************/
/*********************************************************
 *@简要：
 ***从先进先出队列中安全移除节点
 *
 *@参数：
 *[fifo]：先进先出队列
 *[node]: 被移除的节点
 *[safe_node]: 遍历过程中使用的安全节点
 *
 *@返回值:
 *[true]: 成功从先进先出队列中移除节点
 *[false]: 当前节点不在先进先出队列之中
 **********************************************************/
static inline bool fifo_del_node_safe(fifo_t *fifo, slist_node_t *node, slist_node_t **safe_node)
{
    slist_node_t *find_node, *prev_node;

    slist_foreach_record_prev(FIFO_LIST(fifo), find_node, prev_node)
    {
        if (find_node == node)
        {
            slist_node_del_next_safe(prev_node, safe_node);

            if (node == fifo->tail)
            {
                fifo->tail = prev_node;
            }

            return true;
        }
    }

    return false;
}


/*********************************************************
 *@brief: 
 ***transfer all nodes of the FIFO queue to the end of the receive queue,
 ***after the transfer is completed, the original FIFO queue becomes empty
 *
 *@parameter:
 *[fifo]: FIFO queue that was transferred
 *[recv_fifo]: FIFO queue for receiving nodes
 *********************************************************/
/*********************************************************
 *@简要：
 ***将先进先出队列所有节点转移至接收队列的尾部，
 ***转移完成后，原先进先出队列变成空
 *
 *@参数：
 *[fifo]：被转移的先进先出队列
 *[recv_fifo]: 接收节点的先进先出队列
 **********************************************************/
static inline void fifo_nodes_transfer_to(fifo_t *fifo, fifo_t *recv_fifo)
{
    if (!fifo_is_empty(fifo))
    {
        recv_fifo->tail->next = fifo->list.next;
        fifo->tail->next = &recv_fifo->list;
        recv_fifo->tail = fifo->tail;

        fifo_init(fifo);
    }
}

#endif /* __LWES_FIFO_H__ */
