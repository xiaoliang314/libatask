# libatask
**libatask(Asynchronous multitasking library)是一个由纯C语言编写的基于事件循环由多事件驱动的高性能协程库。它将事件与协程进行了结合，还原了协程最本质的用途。libatask资源占用极低，仅需56B的RAM，以及不到1K的CODE，即使最苛刻单片机环境中也能完美运行。**
### 事件与事件循环
---
#### libatask是一个完全由事件驱动运行的框架，事件主要包含了下面4个元素:
* **一个单向循环链表节点** | 当把事件封装成请求的时候，可以使用先进先出队列(libatask自带)创建一个请求队列，利用事件的链表节点可以很方便地将请求进行排队。同时libatask的事件循环队列也使用了该节点。因此，**只有一个事件从其他的队列之中取出的时候才能被提交到事件循环之中。**

* **事件的处理函数指针** | 当事件被处理时，可以调用事件的处理函数对事件进行处理。

* **事件处理函数的上下文指针** | 事件在被创建时可以指定事件携带的上下文，它将作为事件处理函数的第一个参数，libatask不对该字段进行任何处理。

* **事件的优先级** | 事件支持0-255的优先级，优先级高的事件进入事件循环之后，可以被优先处理。同等级的事件按先进先出顺序处理。

#### libatask的事件使用顺序如下：
1. 使用event_init函数或者event_init_inherit对刚创建的事件进行初始化，事件初始化只需要一次，后续可反复使用。
2. 对于已初始化的事件，可以使用el_event_post触发事件调用。
3. 事件在被调用时，事件循环将释放对事件的所有引用，此时事件链表节点将为移除状态。
4. 事件被加入到事件队列后，使用el_event_is_ready将返回true，使用el_event_cancel将事件从事件队列中取消。

#### libatask事件循环主要实现以下功能：
1. 事件按优先级处理
2. 防止事件被递归调用，引发递归重入的问题
3. 实现了定时器功能(需要移植时间获取函数)

#### libatask(事件循环)移植
1. 移植获取时间和时间转换的4个函数
2. 移植事件循环准备调度的函数(该函数用于通知外层用户框架，事件循环中存在可被调度的事件)

## 协程
### 轻量级协程
libatask提供了一个极轻量级的协程，该协程甚至不依赖于libatask库，只需要包含文件bp.h即可。
协程的使用方式如下：
1. 定义一个uint8_t \*bpd的一个指针变量，该指针指向用于保存协程断点位置的变量。如：`uint8_t *bpd = &user_data->bp;`

2. 在协程代码块开始的位置使用bpd_begin(N)，结束的位置使用bpd_end()，一个函数只能有一个协程。 注：N是一个纯数字，从0到255，N表示了协程代码块中存在的断点个数。

3. 使用bpd_yield(N) ret_val，对协程执行挂起操作。原函数则执行了return ret_val操作。N表示协程的第N个断点号(纯数字)，同时，协程内的断点号必须从0到bpd_begin中的N连续。bpd_yield记录当前返回的位置，记录值N保存在bpd指针指向的变量中。原函数在下次调用时将返回当前执行位置。

4. 值得注意的是，原函数的局部变量，在执行bpd_yield之后消失，因为bpd_yield调用的是return，因此，需要使用的变量应该保存在原函数外部，并通过参数传递进来。

用法示例:
```
    uint8_t *bpd = &user->bp;
    uint a;

    bpd_begin(2);

    /* code */
    a = 100;

    bpd_yield(1);

    /* !!此时a的值已经消失 */
    /* code */

    bpd_yield(2);

    bpd_end();
```
### libatask的协程
libatask在轻量级协程的基础上实现了一个在bpd_yield之后不丢失变量的方法。并且实现异步调用等功能。<br/>
libatask协程的函数原型为一个特殊的事件回调函数，即：`void task_func(task_t *task, event_t *ev ...)`
其中...表示用户自定义的参数，如下函数都是合法的libatask协程函数。
```
    void task1(task_t *task, event_t *ev, void *arg1, int arg2, float arg3);
    void task1(task_t *task, event_t *ev, void *arg1, struct tm arg2, int arg3, float arg4);
```
#### libatask协程的使用方式如下：
1. 使用task_init初始化一个task_t的协程任务，并指定协程任务的栈buffer和栈大小

2. 创建协程函数原型如：`void task_func(task_t *task, event_t *ev ...)`

3. 协程函数中使用TASK_BPD宏获取协程的断点变量。`uint8_t *bpd = TASK_BPD(task);`

4. 从task的栈中分配当前协程需要在yield之后保存的变量。使用方式如下：
```
    uint8_t *bpd = TASK_BPD(task);
    struct vars
    {
        int a;
        float b;
        event_t c;
        ...
    } *vars = (struct vars *)task_asyn_vars_get(task, sizeof(*vars));

    bpd_begin(1);

    bpd_yield(1);

    /* 通过vars->可以获取保存在task栈中的变量 */
    printf("%d %f\n", vars->a, vars->b);

    bpd_end();
```

5. 使用task_start(task, task_func, arg1, arg2, ...)启动一个协程。arg1以及之后的参数为协程函数原型event_t *ev之后的自定义的参数。注：协程第一调用时传递的参数仅在bpd_begin() 到 bpd_yield(1)之间有效，因此，如需后续使用这些参数，需要在bpd_yield(1)之前将其保存在task的栈中。

[示例](demo/main.c)<br/>
#### libatask事件继承
在libatask的task_t中，存在一个task->event事件成员。task->event在使用task_start时已经初始化完成。因此，协程中的事件可以直接使用event_init_inherit(myevent, &task->event)，对myevent进行继承初始化。myevent事件触发后，将自动进入当前协程。
#### libatask多事件驱动
libatask的协程挂起后的恢复是由事件触发的，并且支持多事件同时触发。示例如下：
```
    bpd_begin(1);

    /* 事件使用继承的方式初始化，继承至协程的event */
    timer_init_inherit(&vars->timer, &task->event);
    event_init_inherit(&vars->io_ev, &task->event);

    /* 同时开启了I/O事件与Timer事件 */
    io_read(&vars->io_ev);
    el_timer_start_ms(&vars->timer, 3000);

    /* 协程挂起，并等待任意一个事件的到来 */
    bpd_yield(1);

    /* 判断到来的事件是哪一个事件 */
    if (ev == &vars->timer)
    {
        /* 超时了，取消I/O操作 */
        io_cancel(&vars->io_ev);
    }
    else
    {
        /* I/O正常到达，取消超时功能 */
        el_timer_stop(&vars->timer);
    }

    bpd_end();
```
#### libatask异步调用
libatask支持异步调用子协程，使用task_bpd_asyn_call(N, task, func, arg1, arg2, ...)即可，N为断点号。func为子协程函数，原型为void func(task_t *task, event_t *ev, type1 arg1, type2 arg2, ...)，子协程函数需要返回父协程时，需要使用task_asyn_return(task)即可。<br/>
[示例](demo/main.c)<br/>
顶层协程可以选择使用task_asyn_return或不使用task_asyn_return，子协程应该使用task_asyn_return，否则无法返回父协程。
<br/>**注：异步调用时必须确保当前协程没有正在等待的事件，否则等待的事件返回将破坏task的栈。**
#### 极限递归
由于libatask协程使用自定义的栈空间，因此，只要task的栈空间足够大，可以递归任意多的次数。libatask一次异步调用至少需要消耗12B-16B空间（没有异步变量），一个4G的栈空间可以递归调用2.6亿次。2M的栈空间最多也可递归13万次。

## 数据结构
### 单向循环链表
libatask自带了一个单向循环链表，该链表拥有以下特性：
* 链表与链表节点都由一个指针构成，空间占用sizeof(void *)
* 链表节点API使用slist_del_next与slist_inset_next完成链表的删除与插入操作。
* 链表节点支持使用slist\_is\_del判断节点是否被删除（不在队列中）
* 链表支持slist\_foreach\_系列宏进行遍历操作，并且使用slist\_foreach\_\*\*\_safe宏配合slist\_foreach\_safe\_\*\_next函数，可在递归中安全的插入删除当前链表的元素。

<br/>[使用介绍](lib/slist.h)

### FIFO与LIFO
FIFO与LIFO是基于slist实现的先进先出与后进先出的队列。
### FIFO、LIFO以及slist与事件的关系
事件中的链表节点可以作为fifo或lifo中的一个节点。方便对事件进行队列操作。事件的节点可以使用EVENT_NODE(event)取得。

## 定时器
libatask自带了一个定时器功能，API如下：
* 使用timer\_init或者timer\_init\_inherit对定时器进行初始化
* 使用el\_timer\_start\_*对启动定时器

[示例](demo/main.c)

## 信号量
libatask实现了一个基于事件的信号量功能，使用sem_init初始化一个信号量，使用sem_give于sem_take增加或减少信号量。信号量一般配合协程使用。
<br/>使用示例如下：
```
    /* 这是协程等待任务开始信号的实现 */
    while (user->state == STATE_STOP)
    {
        sem_take(&user->sem, &user->task.event);
        bpd_yield(1);
    }
```
```

    /* 唤醒处 */
    user->state = STATE_RUN;
    sem_give(&user->sem, NULL);
```
假如协程未等待信号量，则使用sem\_give(&user->sem, NULL)不做任何操作，若协程正在等待该信号量，则sem_give将唤醒协程。

## SLAB内存池
libatask实现了一个块式无碎片的内存池分配功能，可基于事件实现内存不足时的等待功能。
<br/>API如下：
* 使用slab_create(buff, buff_size, blk_size)创建一个slab，返回创建好的内存池，buff为内存池的基地址，buff为内存池的大小，blk_size为内存池中元素块的大小。
* 使用slab_alloc从slab中分配一个块，块的大小为blk_size，slab空间不足时将返回NULL
* 使用slab_free释放一个块到slab中
* 使用slab_wait等待内存池有可用的内存，内存可用时，将分配内存，并触发事件。slab内存等待的按事件的优先级进行内存分配。

[示例](httpserver_win/httpserver.c)
