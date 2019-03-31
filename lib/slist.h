/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

#ifndef __SLIST_H__
#define __SLIST_H__

#include "defs.h"


/*********************************************************
 *@type description:
 *
 *[slist_node_t]: node of single circular linked list
 *[slist_t]: single circular linked list
 *********************************************************
 *@类型说明：
 *
 *[slist_node_t]：单向循环链表的节点
 *[slist_t]：单向循环链表
 *********************************************************/
typedef struct slist_node_s
{
    /* next node */
    /* 下一个节点 */
    struct slist_node_s *next;
} slist_node_t, slist_t;



/************************************************************
 *@brief:
 ***single circular linked list structure static initialization
 *
 *@Usage:
 ***slist_t list = SLIST_STATIC_INIT(list);
 *
 *@parameter:
 *[head]: List variable name, non-address
 *************************************************************/
/************************************************************
 *@简介：
 ***单向循环链表结构体静态初始化
 *
 *@用法：
 ***slist_t list = SLIST_STATIC_INIT(list);
 *
 *@参数：
 *[head]：链表变量名，非地址
 *************************************************************/
#define SLIST_STATIC_INIT(head) { &head }


/************************************************************
 *@brief:
 ***node of single circular linked list structure static
 ***initialization
 *
 *@Usage:
 ***slist_node_t node = SLIST_NODE_STATIC_INIT(node);
 *
 *@parameter:
 *[node]: node variable name, non-address
 *************************************************************/
/************************************************************
 *@简介：
 ***单向循环链表节点结构体静态初始化
 *
 *@用法：
 ***slist_node_t node = SLIST_NODE_STATIC_INIT(node);
 *
 *@参数：
 *[node]：节点变量名，非地址
 *************************************************************/
#define SLIST_NODE_STATIC_INIT(node) { &node }


/************************************************************
 *@brief:
 ***Get the head node of the single circular linked list
 *
 *@parameter:
 *[slist]: single circular linked list
 *
 *@return: head node of the single circular linked list
 *************************************************************/
/************************************************************
 *@简介：
 ***获取链表的头节点
 *
 *@参数：
 *[slist]：单向循环链表
 *
 *@返回：单向循环链表的头节点
 *************************************************************/
#define SLIST_HEAD(slist)      ((slist_node_t *)(slist))


/************************************************************
 *@brief:
 ***get the next node of the linked list node
 *
 *@parameter:
 *[slist]: the node of single circular linked list
 *
 *@return: the next node of the linked list node
 *************************************************************/
/************************************************************
 *@简介：
 ***获取链表节点的下一个节点
 *
 *@参数：
 *[node]：单向循环链表的节点
 *
 *@返回：单向循环链表节点的下一个节点
 *************************************************************/
#define SLIST_NODE_NEXT(node)      ((node)->next)


/*********************************************************
 *@brief: 
 ***single circular linked list initialization
 *
 *@contract: 
 ***1. Parameter "slist" cannot be a null pointer
 ***2. Cannot initialize the slist being used
 *
 *@parameter:
 *[slist]: single circular linked list
 *********************************************************/
/*********************************************************
 *@简要：
 ***单向循环链表初始化
 *
 *@约定：
 ***1、参数slist不能为空指针
 ***2、不可对正在使用的slist进行初始化
 *
 *@参数：
 *[slist]：单向循环链表
 **********************************************************/
static inline void slist_init(slist_t *slist)
{
    slist->next = slist;
}


/*********************************************************
 *@brief: 
 ***node of single circular linked list initialization
 *
 *@contract: 
 ***1. Parameter "node" connot be a null pointer
 ***2. Cannot initialize the node being used
 *
 *@parameter:
 *[node]: node of single circular linked list
 *********************************************************/
/*********************************************************
 *@简要：
 ***初始化单向循环链表节点
 *
 *@约定：
 ***1、参数node不能为空指针
 ***2、不可对正在使用的node进行初始化
 *
 *@参数：
 *[node]：单向循环链表的节点
 **********************************************************/
static inline void slist_node_init(slist_node_t *node)
{
    node->next = node;
}


/*********************************************************
 *@brief: 
 ***Insert a node after the current node
 *
 *@contract: 
 ***1. "node" is in the linked list
 ***2. "next_node" is the deleted node
 ***3. "node" and "next_node" are not null pointers
 *
 *@parameter:
 *[node]: The node in the linked list
 *[next_node]: Inserted node
 *********************************************************/
/*********************************************************
 *@简要：
 ***在当前节点后面插入节点
 *
 *@约定：
 ***1、node处于链表之中
 ***2、next_node为已删除的节点
 ***3、node与next_node都不为空指针
 *
 *@参数：
 *[node]：链表中的节点
 *[next_node]：插入的节点
 **********************************************************/
static inline void slist_node_insert_next(slist_node_t *node,
                                        slist_node_t *next_node)
{
    next_node->next = node->next;
    node->next = next_node;
}


/*********************************************************
 *@brief: 
 ***Delete the next node of the current node
 *
 *@contract: 
 ***1. "node" is not the tail node in the linked list
 ***2. "node" is not a null pointer
 ***3. "node" is in the linked list
 *
 *@parameter:
 *[node]: the node in linked list
 *
 *@return: node of be deleted
 *********************************************************/
/*********************************************************
 *@简要：
 ***删除当前节点的下一个节点
 *
 *@约定：
 ***1、node不是链表中最后一个节点
 ***2、node不是空指针
 ***3、node处于链表中
 *
 *@参数：
 *[node]：链表中的节点
 *
 *@返回：被删除的节点
 **********************************************************/
static inline slist_node_t* slist_node_del_next(slist_node_t *node)
{
    slist_node_t *deleted_node = node->next;

    node->next = deleted_node->next;
    deleted_node->next = deleted_node;

    return deleted_node;
}


/*********************************************************
 *@brief: 
 ***determine if the single circular linked list is empty
 *
 *@contract: 
 ***1. slist is not a null pointer
 *
 *@parameter:
 *[slist]: single circular linked list
 *
 *@return value:
 *[ture]: single cycle linked list is empty
 *[false]: single cycle linked list is not empty
 *********************************************************/
/*********************************************************
 *@简要：
 ***判断单向循环链表是否为空
 *
 *@约定：
 ***1、slist不是空指针
 *
 *@参数：
 *[slist]：单向循环链表
 *
 *@返回值：
 *[ture]：单向循环链表是空链表
 *[false]：单向循环链表不是空链表
 **********************************************************/
static inline bool slist_is_empty(slist_t *slist)
{
    return slist->next == slist;
}


/*********************************************************
 *@brief: 
 ***determine if the node is deleted
 *
 *@contract: 
 ***1. node is not a null pointer
 *
 *@parameter:
 *[node]: the node of single circular linked list
 *
 *@return value:
 *[ture]: node has been deleted
 *[false]: node not deleted
 *********************************************************/
/*********************************************************
 *@简要：
 ***判断单向循环链表是否为空
 *
 *@约定：
 ***1、slist不是空指针
 *
 *@参数：
 *[node]：单向循环链表的节点
 *
 *@返回值：
 *[ture]：节点已被删除
 *[false]：节点未被删除
 **********************************************************/
static inline bool slist_node_is_del(slist_node_t *node)
{
    return node->next == node;
}

#define slist_node_is_ref(node)   (!slist_node_is_del(node))

/*********************************************************
 *@brief: 
 ***mark the node of deleted as referenced state,
 ***causing slist_node_is_del to return false
 *
 *@contract: 
 ***1. node is not null pointer
 *
 *@parameter:
 *[node]: node of be referenced
 *
 *@return value:
 *[true]：successfully marked
 *[false]：node is not deleted
 *********************************************************/
/*********************************************************
 *@简要：
 ***将已删除的节点标记为引用状态，使slist_node_is_del返回false
 *
 *@约定：
 ***1、node不是空指针
 *
 *@参数：
 *[node]：被引用的节点
 *
 *@返回值：
 *[true]：标记成功
 *[false]：节点未处于删除状态
 **********************************************************/
static inline slist_node_t *slist_node_ref(slist_node_t *node)
{
    if (node->next == node)
    {
        node->next = NULL;

        return node;
    }

    return NULL;
}


/*********************************************************
 *@brief: 
 ***mark the referenced node as unreferenced,
 ***making slist_node_is_del return true
 *
 *@contract: 
 ***1. node is not null pointer
 *
 *@parameter:
 *[node]: referenced node
 *
 *@return value:
 *[true]：successfully cancel referenced
 *[false]：node is not referenced
 *********************************************************/
/*********************************************************
 *@简要：
 ***将引用的节点标记为未引用状态，使slist_node_is_del返回true
 *
 *@约定：
 ***1、node不是空指针
 *
 *@参数：
 *[node]：已引用的节点
 *
 *@返回值：
 *[true]：标记成功
 *[false]：节点未处于删除状态
 **********************************************************/
static inline bool slist_node_unref(slist_node_t *node)
{
    if (node->next == NULL)
    {
        node->next = node;

        return true;
    }

    return false;
}


/*********************************************************
 *@brief: 
 ***Get its outer structure pointer by member pointer
 *
 *@parameter:
 *[type]: Type name of the outer structure
 *[member]: The member name of the member in the outer structure
 *[member_ptr]: The pointer of member in outer structure
 *
 *@return type:
 *[type *]: The pointer of type
 *
 *@return value: Its outer structure pointer by member pointer
 *********************************************************/
/*********************************************************
 *@简要：
 ***通过成员指针获取其外层结构体指针
 *
 *@参数：
 *[type]：外层结构体的类型名
 *[member]：成员在外层结构体中的成员名
 *[member_ptr]：结构体中成员的指针
 *
 *@返回类型：
 *[type *]：type的指针
 *
 *@返回值：成员指针所在的外层结构体指针
 **********************************************************/
#define slist_entry(type, member, member_ptr) container_of(member_ptr, type, member)


/*********************************************************
 *@brief: 
 ***Get the pointer of the outer structure of the member
 ***through any outer structure pointer and member pointer
 *
 *@parameter:
 *[eptr]: Type name of the outer structure
 *[member]: The member name of the member in the outer structure
 *[member_ptr]: The pointer of member in outer structure
 *
 *@return type:
 *[type *]: The pointer of type
 *
 *@return value: The outer structure pointer of member 
 *********************************************************/
/*********************************************************
 *@简要：
 ***通过任意的外层结构体指针和成员指针获取成员所在外层结构体的指针
 *
 *@参数：
 *[eptr]：任意的外层结构体指针
 *[member]：成员在外层结构体中的成员名
 *[member_ptr]：结构体中成员的指针
 *
 *@返回类型：
 *[eptr的类型]：eptr指针类型
 *
 *@返回值：成员指针所在的外层结构体指针
 **********************************************************/
#define slist_pentry(eptr, member, member_ptr) pcontainer_of(member_ptr, eptr, member)



/*********************************************************
 *@brief: 
 ***Use "node" to traverse the "slist",
 ***"node" is the node that is currently traversed
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. If you perform insert and delete operations on a node,
 ***   you cannot continue traversing.
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[node]: storing the node that is currently traversed
 *
 *@Usage:
 ***  slist_node_t *node;
 ***
 ***  slist_foreach(slist, node)
 ***  {
 ***      //processing code for each node
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用node遍历整个slist，node为当前被遍历的节点
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、若对节点执行插入与删除操作，则不可继续遍历。
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[node]：存放着当前被遍历的节点
 *
 *@用法：
 ***  slist_node_t *node;
 ***
 ***  slist_foreach(slist, node)
 ***  {
 ***      //对每个节点的处理代码
 ***  }
 **********************************************************/
#define slist_foreach(slist, node)                              \
    for ((node) = (slist)->next;                                \
        (node) != (slist);                                      \
        (node) = (node)->next)


/*********************************************************
 *@brief: 
 ***Use "node" to traverse the "slist",
 ***"node" is the node that is currently traversed
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. safely delete of linked list nodes requires slist_foreach_safe_del_next
 ***3. safely insert of linked list nodes requires slist_foreach_safe_insert_next
 ***4. in the case of safe insert and delete operations, you can continue traversing.
 ***5. don't do anything with safe_node
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[node]: storing the node that is currently traversed
 *[safe_node]: safe node required for insert and delete operations in linked list traversal
 *
 *@Usage:
 ***  slist_node_t *node;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_safe(slist, node, safe_node)
 ***  {
 ***      //processing code for each node
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用node遍历整个slist，node为当前被遍历的节点
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、链表节点的安全删除操作需使用slist_foreach_safe_del_next
 ***3、链表节点的安全插入操作需使用slist_foreach_safe_insert_next
 ***4、在使用安全插入与删除操作的情况下，则可以继续遍历。
 ***5、不可对safe_node做任何处理
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[node]：存放着当前被遍历的节点
 *[safe_node]：链表遍历中进行插入与删除操作所需的安全节点
 *
 *@用法：
 ***  slist_node_t *node;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_safe(slist, node, safe_node)
 ***  {
 ***      //对每个节点的处理代码
 ***  }
 **********************************************************/
#define slist_foreach_safe(slist, node, safe_node)              \
    for ((safe_node) = (slist)->next,                           \
         (node) = (safe_node);                                  \
         (node) != (slist);                                     \
         (safe_node) = (safe_node)->next,                       \
         (node) = (safe_node))


/*********************************************************
 *@brief: 
 ***Use "node" to traverse the "slist",
 ***"node" is the node that is currently traversed,
 ***"prev_node" is the previous node of "node".
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. If you perform insert and delete operations on a node,
 ***   you cannot continue traversing.
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[node]: storing the node that is currently traversed
 *[prev_node]: stores the previous node of the node that is currently traversed
 *
 *@Usage:
 ***  slist_node_t *node;
 ***  slist_node_t *prev_node;
 ***
 ***  slist_foreach_record_prev(slist, node, prev_node)
 ***  {
 ***      if (...) //certain conditions
 ***      {
 ***          //remove node by prev_node
 ***          slist_node_del_next(prev_node);
 ***
 ***          //you must exit traversal after performing delete and insert
 ***          break;
 ***      }
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用node遍历整个slist，node为当前被遍历的节点，
 ***prev_node为node的上一个节点
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、若对节点执行插入与删除操作，则不可继续遍历。
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[node]：存放着当前被遍历的节点
 *[prev_node]: 存放着当前被遍历节点的上一个节点
 *
 *@用法：
 ***  slist_node_t *node;
 ***  slist_node_t *prev_node;
 ***
 ***  slist_foreach_record_prev(slist, node, prev_node)
 ***  {
 ***      if (...) //某些条件
 ***      {
 ***          //通过prev_node移除node
 ***          slist_node_del_next(prev_node);
 ***
 ***          //在执行删除与插入后必须退出遍历
 ***          break;
 ***      }
 ***  }
 **********************************************************/
#define slist_foreach_record_prev(slist, node, prev_node)       \
    for ((node) = (slist)->next,                                \
         (prev_node) = (slist);                                 \
         (node) != (slist);                                     \
         (prev_node) = (node),                                  \
         (node) = (node)->next)


/*********************************************************
 *@brief: 
 ***Use "node" to traverse the "slist",
 ***"node" is the node that is currently traversed,
 ***"prev_node" is the previous node of "node".
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. safely delete of linked list nodes requires slist_foreach_safe_del_next
 ***3. safely insert of linked list nodes requires slist_foreach_safe_insert_next
 ***4. in the case of safe insert and delete operations, you can continue traversing.
 ***5. don't do anything with safe_node
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[node]: storing the node that is currently traversed
 *[prev_node]: stores the previous node of the node that is currently traversed
 *[safe_node]: safe node required for insert and delete operations in linked list traversal
 *
 *@Usage:
 ***  slist_node_t *node;
 ***  slist_node_t *prev_node;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_record_prev_safe(slist, node, prev_node, safe_node)
 ***  {
 ***      if (...) //certain conditions
 ***      {
 ***          //safely remove "node" in traversal by "prev_node"
 ***          slist_foreach_safe_del_next(prev_node, &safe_node);
 ***
 ***          //continue to traverse after performing delete and insert
 ***          //break;
 ***      }
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用node遍历整个slist，node为当前被遍历的节点，
 ***prev_node为被遍历节点的上一个节点
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、链表节点的安全删除操作需使用slist_foreach_safe_del_next
 ***3、链表节点的安全插入操作需使用slist_foreach_safe_insert_next
 ***4、在使用安全插入与删除操作的情况下，则可以继续遍历。
 ***5、不可对safe_node做任何处理
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[node]：存放着当前被遍历的节点
 *[prev_node]: 存放着当前被遍历节点的上一个节点
 *[safe_node]：链表遍历中进行插入与删除操作所需的安全节点
 *
 *@用法：
 ***  slist_node_t *node;
 ***  slist_node_t *prev_node;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_record_prev_safe(slist, node, prev_node, safe_node)
 ***  {
 ***      if (...) //某些条件
 ***      {
 ***          //通过prev_node在遍历中安全移除node
 ***          slist_foreach_safe_del_next(prev_node, &safe_node);
 ***
 ***          //在插入与删除之后，可以继续遍历
 ***          //break;
 ***      }
 ***  }
 **********************************************************/
#define slist_foreach_record_prev_safe(slist, node, prev_node, safe_node)       \
    for ((prev_node) = (slist),                                                 \
         (safe_node) = (slist)->next,                                           \
         (node) = (safe_node);                                                  \
         (node) != (slist);                                                     \
         (prev_node) = (safe_node),                                             \
         (safe_node) = (safe_node)->next,                                       \
         (node) = (safe_node))


/*********************************************************
 *@brief: 
 ***Use "entry" to traverse the "slist",
 ***"entry" is the object that is currently traversed
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. If you perform insert and delete operations on a node,
 ***   you cannot continue traversing.
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[entry]: stores the outer structure pointer of the node currently being traversed
 *[member]: the member name of the linked list node in the outer structure
 *
 *@Usage:
 ***  student_t *student;
 ***
 ***  slist_foreach(class_list, student, class_list_node)
 ***  {
 ***      //reward all student with score of over 95
 ***      if (student->score >= 95)
 ***      {
 ***           //reward this student with $100
 ***           reward_money(student, 100);
 ***      }
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用entry遍历整个slist，entry为当前被遍历的对象
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、若对节点执行插入与删除操作，则不可继续遍历。
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[entry]：存放着当前被遍历节点的外层结构体指针
 *[member]: 链表节点在外层结构体中的成员名
 *
 *@用法：
 ***  student_t *student;
 ***
 ***  slist_foreach(class_list, student, class_list_node)
 ***  {
 ***      //奖励分数超过95的学生
 ***      if (student->score >= 95)
 ***      {
 ***           //奖励这个学生100元
 ***           reward_money(student, 100);
 ***      }
 ***  }
 **********************************************************/
#define slist_foreach_entry(slist, entry, member)                               \
    for (*(void **)(&entry) = pcontainer_of((slist)->next, (entry), member);    \
        &(entry)->member != (slist);                                            \
        *(void **)(&entry) = pcontainer_of((entry)->member.next, (entry), member))


/*********************************************************
 *@brief: 
 ***Use "entry" to traverse the "slist",
 ***"entry" is the object that is currently traversed
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. safely delete of linked list nodes requires slist_foreach_safe_del_next
 ***3. safely insert of linked list nodes requires slist_foreach_safe_insert_next
 ***4. in the case of safe insert and delete operations, you can continue traversing.
 ***5. don't do anything with safe_node
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[entry]: stores the outer structure pointer of the node currently being traversed
 *[member]: the member name of the linked list node in the outer structure
 *[safe_node]: safe node required for insert and delete operations in linked list traversal
 *
 *@Usage:
 ***  student_t *student;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_entry_safe(class_list, student, class_list_node, safe_node)
 ***  {
 ***      //reward all student with score of over 95
 ***      if (student->score >= 95)
 ***      {
 ***           //reward this student with $100
 ***           reward_money(student, 100);
 ***      }
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用entry遍历整个slist，entry为当前被遍历的对象
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、链表节点的安全删除操作需使用slist_foreach_safe_del_next
 ***3、链表节点的安全插入操作需使用slist_foreach_safe_insert_next
 ***4、在使用安全插入与删除操作的情况下，则可以继续遍历。
 ***5、不可对safe_node做任何处理
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[entry]：存放着当前被遍历节点的外层结构体指针
 *[member]：链表节点在外层结构体中的成员名
 *[safe_node]：链表遍历中进行插入与删除操作所需的安全节点
 *
 *@用法：
 ***  student_t *student;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_entry_safe(class_list, student, class_list_node, safe_node)
 ***  {
 ***      //奖励分数超过95的学生
 ***      if (student->score >= 95)
 ***      {
 ***           //奖励这个学生100元
 ***           reward_money(student, 100);
 ***      }
 ***  }
 **********************************************************/
#define slist_foreach_entry_safe(slist, entry, member, safe_node)               \
    for ((safe_node) = (slist)->next,                                           \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member);      \
         &(entry)->member != (slist);                                           \
         (safe_node) = (safe_node)->next,                                       \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member))


/*********************************************************
 *@brief: 
 ***Use "entry" to traverse the "slist",
 ***"entry" is the object that is currently traversed
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. If you perform insert and delete operations on a node,
 ***   you cannot continue traversing.
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[entry]: stores the outer structure pointer of the node currently being traversed
 *[member]: the member name of the linked list node in the outer structure
 *[prev_node]: stores the previous node of the node that is currently traversed
 *
 *@Usage:
 ***  student_t *student;
 ***  slist_node_t *prev_node;
 ***
 ***  slist_foreach_entry_record_prev(class_list, student, class_list_node, prev_node)
 ***  {
 ***      //remove Jack from the class
 ***      if (strcmp(student->name, "Jack") == 0)
 ***      {
 ***          //remove the current node by prev_node
 ***          slist_node_del_next(prev_node);
 ***          
 ***          //after inserting or deleting, you must exit the traversal
 ***          break;
 ***      }
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用entry遍历整个slist，entry为当前被遍历的对象
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、若对节点执行插入与删除操作，则不可继续遍历。
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[entry]：存放着当前被遍历节点的外层结构体指针
 *[member]: 链表节点在外层结构体中的成员名
 *[prev_node]：存放着当前被遍历节点的上一个节点
 *
 *@用法：
 ***  student_t *student;
 ***  slist_node_t *prev_node;
 ***
 ***  slist_foreach_entry_record_prev(class_list, student, class_list_node, prev_node)
 ***  {
 ***      //将小明移除班级
 ***      if (strcmp(student->name, "小明") == 0)
 ***      {
 ***          //通过prev_node移除当前节点
 ***          slist_node_del_next(prev_node);
 ***          
 ***          //在插入或删除后，必须退出遍历
 ***          break;
 ***      }
 ***  }
 **********************************************************/
#define slist_foreach_entry_record_prev(slist, entry, member, prev_node)                \
    for (*(void **)(&entry) = pcontainer_of((slist)->next, (entry), member),            \
         (prev_node) = (slist);                                                         \
         &(entry)->member != (slist);                                                   \
         (prev_node) = &(entry)->member,                                                \
         *(void **)(&entry) = pcontainer_of((entry)->member.next, (entry), member))


/*********************************************************
 *@brief: 
 ***Use "entry" to traverse the "slist",
 ***"entry" is the object that is currently traversed
 *
 *@contract: 
 ***1. "slist" is not null pointer
 ***2. safely delete of linked list nodes requires slist_foreach_safe_del_next
 ***3. safely insert of linked list nodes requires slist_foreach_safe_insert_next
 ***4. in the case of safe insert and delete operations, you can continue traversing.
 ***5. don't do anything with safe_node
 *
 *@parameter:
 *[slist]: the single circular linked list of be traversed
 *[entry]: stores the outer structure pointer of the node currently being traversed
 *[member]: the member name of the linked list node in the outer structure
 *[prev_node]: stores the previous node of the node that is currently traversed
 *[safe_node]: safe node required for insert and delete operations in linked list traversal
 *
 *@Usage:
 ***  student_t *student;
 ***  slist_node_t *prev_node;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_entry_record_prev_safe(class_list, student, class_list_node, prev_node, safe_node)
 ***  {
 ***      //remove students who have made more than three mistakes from the class
 ***      if (student->mistake_count >= 3)
 ***      {
 ***          //remove the current node by prev_node
 ***          slist_foreach_safe_del_next(prev_node, &safe_node);
 ***          
 ***          //after inserting or deleting, you can continue to traverse
 ***          //break;
 ***      }
 ***  }
 *********************************************************/
/*********************************************************
 *@简要：
 ***使用entry遍历整个slist，entry为当前被遍历的对象
 *
 *@约定：
 ***1、slist不能为空指针
 ***2、链表节点的安全删除操作需使用slist_foreach_safe_del_next
 ***3、链表节点的安全插入操作需使用slist_foreach_safe_insert_next
 ***4、在使用安全插入与删除操作的情况下，则可以继续遍历。
 ***5、不可对safe_node做任何处理
 *
 *@参数：
 *[slist]：被遍历的单向循环链表
 *[entry]：存放着当前被遍历节点的外层结构体指针
 *[member]: 链表节点在外层结构体中的成员名
 *[prev_node]：存放着当前被遍历节点的上一个节点
 *[safe_node]：链表遍历中进行插入与删除操作所需的安全节点
 *
 *@用法：
 ***  student_t *student;
 ***  slist_node_t *prev_node;
 ***  slist_node_t *safe_node;
 ***
 ***  slist_foreach_entry_record_prev_safe(class_list, student, class_list_node, prev_node, safe_node)
 ***  {
 ***      //将犯错次数超过三次的学生移除班级
 ***      if (student->mistake_count >= 3)
 ***      {
 ***          //通过prev_node移除当前节点
 ***          slist_foreach_safe_del_next(prev_node, &safe_node);
 ***
 ***          //在插入或删除后，可以继续遍历
 ***          //break;
 ***      }
 ***  }
 **********************************************************/
#define slist_foreach_entry_record_prev_safe(slist, entry, member, prev_node, safe_node)\
    for ((prev_node) = (slist),                                                         \
         (safe_node) = (slist)->next,                                                   \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member);              \
         &(entry)->member != (slist);                                                   \
         (prev_node) = (safe_node),                                                     \
         (safe_node) = (safe_node)->next,                                               \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member))


/* traversal from the "form",
 * the same function as slist_foreach */
/* 从form开始遍历，功能与slist_foreach相同 */
#define slist_foreach_from(from, slist, node)                   \
    for ((node) = (from)->next;                                 \
        (node) != (slist);                                      \
        (node) = (node)->next)


/* traversal from the "form",
 * the same function as slist_foreach_safe */
/* 从form开始遍历，功能与slist_foreach_safe相同 */
#define slist_foreach_from_safe(from, slist, node, safe_node)   \
    for ((safe_node) = (from)->next,                            \
         (node) = (safe_node);                                  \
         (node) != (slist);                                     \
         (safe_node) = (safe_node)->next,                       \
         (node) = (safe_node))


/* traversal from the "form",
 * the same function as slist_foreach_record_prev */
/* 从form开始遍历，功能与slist_foreach_record_prev相同 */
#define slist_foreach_from_record_prev(from, slist, node, prev_node)        \
    for ((prev_node) = (from),                                              \
         (node) = (from)->next;                                             \
         (node) != (slist);                                                 \
         (prev_node) = (node),                                              \
         (node) = (node)->next)


/* traversal from the "form",
 * the same function as slist_foreach_record_prev_safe */
/* 从form开始遍历，功能与slist_foreach_record_prev_safe相同 */
#define slist_foreach_from_record_prev_safe(from, slist, node, prev_node, safe_node)    \
    for ((prev_node) = (from),                                                          \
         (safe_node) = (from)->next,                                                    \
         (node) = (safe_node);                                                          \
         (node) != (slist);                                                             \
         (prev_node) = (safe_node),                                                     \
         (safe_node) = (safe_node)->next,                                               \
         (node) = (safe_node))


/* traversal from the "form_entry",
 * the same function as slist_foreach_entry */
/* 从form_entry开始遍历，功能与slist_foreach_entry相同 */
#define slist_foreach_from_entry(form_entry, slist, entry, member)              \
    for (*(void **)(&entry) = pcontainer_of((form_entry)->member.next, (entry), member);   \
        &(entry)->member != (slist);                                            \
        *(void **)(&entry) = pcontainer_of((entry)->member.next, (entry), member))


/* traversal from the "form_entry",
 * the same function as slist_foreach_entry_safe */
/* 从form_entry开始遍历，功能与slist_foreach_entry_safe相同 */
#define slist_foreach_form_entry_safe(form_entry, slist, entry, member, safe_node)          \
    for ((safe_node) = (form_entry)->member.next,                                           \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member);                  \
         &(entry)->member != (slist);                                                       \
         (safe_node) = (safe_node)->next,                                                   \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member))


/* traversal from the "form_entry",
 * the same function as slist_foreach_entry_record_prev */
/* 从form_entry开始遍历，功能与slist_foreach_entry_record_prev相同 */
#define slist_foreach_form_entry_record_prev(form_entry, slist, entry, member, prev_node)   \
    for ((prev_node) = &(form_entry)->member,                                               \
         *(void **)(&entry) = pcontainer_of((form_entry)->member.next, (entry), member);    \
         &(entry)->member != (slist);                                                       \
         (prev_node) = &(entry)->member,                                                    \
         *(void **)(&entry) = pcontainer_of((entry)->member.next, (entry), member))


/* traversal from the "form_entry",
 * the same function as slist_foreach_entry_record_prev_safe */
/* 从form_entry开始遍历，功能与slist_foreach_entry_record_prev_safe相同 */
#define slist_foreach_form_entry_record_prev_safe(form_entry, slist, entry, member, prev_node, safe_node)\
    for ((prev_node) = &(form_entry)->member,                                           \
         (safe_node) = (form_entry)->member.next,                                       \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member);              \
         &(entry)->member != (slist);                                                   \
         (prev_node) = (safe_node),                                                     \
         (safe_node) = (safe_node)->next,                                               \
         *(void **)(&entry) = pcontainer_of((safe_node), (entry), member))


/*********************************************************
 *@brief: 
 ***this function can perform a safe delete operation on 
 ***the linked list node in the macro of the slist_foreach_**safe series,
 ***and keep the linked list continue traversal.
 *
 *@contract: 
 ***1. "prev_node" and "safe_node" are not null pointers
 ***2. "prev_node" is the node of be traversed linked list
 ***3. "safe_node" is the "safe_node" of be traversed linked list
 *
 *@parameter:
 *[prev_node]: previous node of the be deleted node
 *[safe_node]: "safe_node" is "safe_node" of  the linked list being traversed
 *
 *@return type:
 *[slist_node_t *]: node of single circular linked list
 *
 *@return: node of be deleted
 *********************************************************/
/*********************************************************
 *@简要：
 ***该函数可以在slist_foreach_**safe系列的宏中，对链表节点执行安全删除操作，
 ***并保持链表正常遍历
 *
 *@约定：
 ***1、prev_node和safe_node不是空指针
 ***2、prev_node为被遍历链表的节点
 ***3、safe_node为被遍历链表的safe_node
 *
 *@参数：
 *[prev_node]：被删除节点的前一个节点
 *[safe_node]：安全遍历时使用的safe_node
 *
 *@返回：被删除的节点
 **********************************************************/
static inline slist_node_t* slist_foreach_safe_del_next(slist_node_t *prev_node, slist_node_t **safe_node)
{
    slist_node_t *node = prev_node->next;

    if (*safe_node == node)
    {
        *safe_node = prev_node;
    }

    prev_node->next = node->next;
    node->next = node;

    return node;
}


/*********************************************************
 *@brief: 
 ***this function can perform a safe insert operation on 
 ***the linked list node in the macro of the slist_foreach_**safe series,
 ***and keep the linked list continue traversal.
 *
 *@contract: 
 ***1. "prev_node", "safe_node" and "node" are not null pointers
 ***2. "prev_node" is the node of be traversed linked list
 ***3. the "node" is node of deleted
 ***4. "safe_node" is the "safe_node" of be traversed linked list
 *
 *@parameter:
 *[prev_node]: previous node of the be insert node
 *[node]: the node to be inserted
 *[safe_node]: "safe_node" is "safe_node" of  the linked list being traversed
 *********************************************************/
/*********************************************************
 *@简要：
 ***该函数可以在slist_foreach_**safe系列的宏中，对链表节点执行安全删除操作，
 ***并保持链表正常遍历
 *
 *@约定：
 ***1、prev_node和safe_node不是空指针
 ***2、prev_node为被遍历链表的节点
 ***3、node为已删除的节点
 ***4、safe_node为被遍历链表的safe_node
 *
 *@参数：
 *[prev_node]：被删除节点的前一个节点
 *[node]：需要插入的节点
 *[safe_node]：安全遍历时使用的safe_node
 **********************************************************/
static inline void slist_foreach_safe_insert_next(slist_node_t *prev_node,
                                                  slist_node_t *node,
                                                  slist_node_t **safe_node)
{
    if (*safe_node == prev_node)
    {
        *safe_node = node;
    }

    node->next = prev_node->next;
    prev_node->next = node;
}


/* delete the "node" node in the single linked list */
/* 删除单链表中的node节点 */
/*********************************************************
 *@brief: 
 ***delete the "node" node in single circular linked list
 *
 *@contract: 
 ***1. "slist" and "node" are not null pointers
 *
 *@parameter:
 *[slist]: single circular linked list
 *[node]: node of be deleted
 *
 *@return value:
 *[true]: successfully removed node from "slist"
 *[false]: "node" is not in "slist"
 *********************************************************/
/*********************************************************
 *@简要：
 ***删除单循环链表中的node节点
 *
 *@约定：
 ***1、slist与node不是空指针
 *
 *@参数：
 *[slist]：单循环链表
 *[node]：要删除的节点
 *
 *@返回值：
 *[true]：成功从slist中删除节点
 *[false]：node不在slist之中
 **********************************************************/
static inline bool slist_del_node(slist_t *slist, slist_node_t *node)
{
    slist_node_t *prev_node;
    slist_node_t *find_node;

    slist_foreach_record_prev(slist, find_node, prev_node)
    {
        if (find_node == node)
        {
            slist_node_del_next(prev_node);
            return true;
        }
    }

    return false;
}

#endif /* __SLIST_H__ */
