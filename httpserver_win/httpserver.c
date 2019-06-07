#include <winsock2.h>
#include <ws2tcpip.h>
#include <winerror.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>

/* #define TASK_INFO(task, size, used) printf("sizeof(void*):%d task:%p stack size:%d stack used:%d\n", (int)sizeof(void *), (task), (int)(size), (int)(used)) */
/* #define TASK_ASSERT(expr)   do { if (!(expr)) {printf("ERROR: stack overflow!!\n"); abort();} } while(0) */

#include "../lib/atask.h"

struct iocp_evt_s
{
    event_t event;
    OVERLAPPED overlapped;
    ULONG_PTR key;
    DWORD numberOfBytes;
    DWORD dwError;
};

static HANDLE s_iocp = NULL;
static SOCKET s_sock = -1;

#ifndef WSAID_ACCEPTEX
#define WSAID_ACCEPTEX {0xb5367df1,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92}}
#endif

#ifndef WSAID_GETACCEPTEXSOCKADDRS
#define WSAID_GETACCEPTEXSOCKADDRS { 0xb5367df2,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92} }
#endif


typedef BOOL (WINAPI *MY_LPFN_ACCEPTEX)(SOCKET, SOCKET, PVOID, DWORD, DWORD, DWORD, LPDWORD, LPOVERLAPPED);
typedef VOID (WINAPI *MY_LPFN_GETACCEPTEXSOCKADDRS) (PVOID, DWORD, DWORD, DWORD, struct sockaddr **, LPINT, struct sockaddr **, LPINT);

MY_LPFN_ACCEPTEX lpfnAcceptEx = NULL;
MY_LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs = NULL;

/* IOCP event loop */
/* IOCP事件循环 */
void iocp_atask_run(HANDLE iocp)
{
    DWORD dwError;
    DWORD numberOfBytes;
    ULONG_PTR key;
    OVERLAPPED *pOverlapped;
    time_nclk_t due, now;
    time_ms_t timeout;
    struct iocp_evt_s *iocp_evt;

    while (1)
    {
        due = el_schedule();

        /* calculate the timeout */
        /* 计算超时 */
        now = time_nclk_get();
        timeout = due < now ? 0 : time_nclk_to_us(due - now) / 1000;
        timeout = timeout > UINT32_MAX ? INFINITE : timeout;

        /* If IOCP exists, use IOCP timeout, 
           otherwise use sleep instead */
        /* 若IOCP存在，则使用IOCP超时，否则使用Sleep替代 */
        if (iocp != NULL)
        {
            /* Get completed events in the IOCP queue until fully get. */
            /* 获取IOCP队列中已完成的事件，直到完全获取 */
            while (1)
            {
                dwError = GetQueuedCompletionStatus(iocp, &numberOfBytes, &key, &pOverlapped, (DWORD)timeout) ? 0 : GetLastError();

                /* Post to the event loop if an IOCP event is get. */
                /* 若获取到IOCP事件，则提交至事件循环 */
                if (pOverlapped != NULL)
                {
                    iocp_evt = container_of(pOverlapped, struct iocp_evt_s, overlapped);

                    iocp_evt->key = key;
                    iocp_evt->numberOfBytes = numberOfBytes;
                    iocp_evt->dwError = dwError;

                    el_event_post(&iocp_evt->event);
                }
                else
                {
                    break;
                }

                timeout = 0;
            };
        }
        else
        {
            Sleep((DWORD)timeout);
        }
    }
}

/* Http client data receive timeout */
/* Http客户端数据接收超时 */
#define HTTP_CLIENT_RECV_TIMEOUT_MS         5000

/* Http client request task stack size */
/* Http客户端请求任务的栈大小 */
#define HTTP_CLIENT_REQUST_TASK_STACK_SIZE  (256 + 3072)

/* Maximum number of Http clients */
/* Http客户端最大数量 */
#define HTTP_CLIENT_MAX_NUMS                30000

#define SERVER_STRING "Server: libatask httpd 1.0\r\n"

struct http_task_with_stack_s
{
    task_t task;
    event_t free_ev;
    uint8_t stack[HTTP_CLIENT_REQUST_TASK_STACK_SIZE];
};

static uint8_t http_client_tasks_slab_buff[HTTP_CLIENT_REQUST_TASK_STACK_SIZE * HTTP_CLIENT_MAX_NUMS + sizeof(slab_t)];
static slab_t *http_client_tasks_slab = NULL;

/* Get data from the client */
/* 从客户端获取数据 */
void http_client_data_get(task_t *task, 
                            event_t *ev, 
                            SOCKET _cli_sock, 
                            void *_buf, 
                            uint32_t _buf_size,
                            uint32_t *_recv_size,
                            time_ms_t _recv_timeout)
{
    uint8_t *bpd = TASK_BPD(task);
    struct vars
    {
        SOCKET cli_sock;
        timer_event_t timer;
        struct iocp_evt_s iocp_ev;
        WSABUF wsa_buf;
        uint32_t *recv_size;
    } *vars = (struct vars *)task_asyn_vars_get(task, sizeof(*vars));
    DWORD flags;
    DWORD dwError;

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(2);

    /* Save parameters */
    /* 保存参数 */
    vars->cli_sock = _cli_sock;
    vars->recv_size = _recv_size;

    /* Initialize the timer */
    /* 初始化定时器 */
    timer_init_inherit(&vars->timer, &task->event);

    /* Initialize IOCP event */
    /* 初始化IOCP事件 */
    memset(&vars->iocp_ev, 0, sizeof(vars->iocp_ev));
    event_init_inherit(&vars->iocp_ev.event, &task->event);

    vars->wsa_buf.buf = _buf;
    vars->wsa_buf.len = _buf_size;
    flags = 0;
    /* Start WSARecv opreate */
    /* 启动WSARecv操作 */
    if (SOCKET_ERROR == WSARecv(vars->cli_sock, 
                    &vars->wsa_buf, 
                    1,
                    NULL,
                    &flags, 
                    &vars->iocp_ev.overlapped, NULL))
    {
        dwError = WSAGetLastError();
        if (dwError != ERROR_IO_PENDING)
        {
            task->ret_val.u32 = dwError;
            /* The coroutine end */
            /* 协程结束 */
            bpd_break;
        }
    }

    /* Enable receive timeout */
    /* 开启接收超时 */
    el_timer_start_ms(&vars->timer, _recv_timeout);

    /* Suspend this coroutine and wait an event. */
    /* 挂起协程，等待事件 */
    bpd_yield(1);

    /* If it is a timeout event, end the request. */
    /* 若为超时事件，则结束请求 */
    if (ev == &vars->timer.event)
    {
        /* 若IOCP事件已就绪，但未到达 */
        if (el_event_is_ready(&vars->iocp_ev.event))
        {
            /* Cancel the event from the event loop */
            /* 从事件循环中取消该事件 */
            el_event_cancel(&vars->iocp_ev.event);
        }
        else
        {
            /* Cancel IOCP request */
            /* 取消IOCP请求 */
            if (FALSE == CancelIo((HANDLE)vars->cli_sock))
            {
                printf("Warnnig: CancelIo Failed, Error:%u\n", (uint32_t)GetLastError());
            }

            /* Wait IOCP event return an error */
            /* 等待IOCP事件返回错误 */
            bpd_yield(2);

            *vars->recv_size = 0;
            task->ret_val.u32 = WAIT_TIMEOUT;
            /* The coroutine end */
            /* 协程结束 */
            bpd_break;
        }
    }
    /* If it is IOCP event, stop the timer */
    /* 若是IOCP事件，停止定时器 */
    else
    {
        el_timer_stop(&vars->timer);
    }

    task->ret_val.u32 = vars->iocp_ev.dwError;
    *vars->recv_size = vars->iocp_ev.numberOfBytes;

    /* coroutine end */
    /* 协程结束 */
    bpd_end();

    /* asynchronous return */
    /* 异步返回 */
    task_asyn_return(task);
}

int mystrcasecmp(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 != *s2
          && tolower(*s1) != tolower(*s2))
        {
            break;
        }

        s1++;
        s2++;
    }

    return (signed char)*s1 - (signed char)*s2;
}

static const char http_501_rsp_text[] = 
"HTTP/1.0 501 Method Not Implemented\r\n"
SERVER_STRING
"Content-Type: text/html\r\n"
"\r\n"
"<html><head><title>Method Not Implemented\r\n"
"</title></head>\r\n"
"<body><p>HTTP request method not supported.\r\n"
"</p></body></html>\r\n";

static const char http_404_rsp_text[] = 
"HTTP/1.0 404 NOT FOUND\r\n"
SERVER_STRING
"Content-Type: text/html\r\n"
"\r\n"
"<html><head><title>Not Found\r\n"
"</title></head>\r\n"
"<body><p>The service could not respond to the path you requested.\r\n"
"</p></body></html>\r\n";

static const char http_ok_rsp_html[] = 
"HTTP/1.0 200 OK\r\n"
SERVER_STRING
"Content-Type: text/html\r\n"
"\r\n";

static const char http_ok_rsp_js[] = 
"HTTP/1.0 200 OK\r\n"
SERVER_STRING
"Content-Type: application/x-javascript\r\n"
"\r\n";

static const char http_ok_rsp_css[] = 
"HTTP/1.0 200 OK\r\n"
SERVER_STRING
"Content-Type: text/css\r\n"
"\r\n";

static const char http_ok_rsp_other[] = 
"HTTP/1.0 200 OK\r\n"
SERVER_STRING
"Content-Type: application/octet-stream\r\n"
"\r\n";

/* Http client send data */
/* http客户端发送数据 */
void http_client_send(task_t *task, event_t *ev, SOCKET _cli_sock, const void *_buf, uint32_t _buf_size)
{

    uint8_t *bpd = TASK_BPD(task);
    struct vars
    {
        SOCKET cli_sock;
        DWORD send_bytes;
        WSABUF wsa_buf;
        struct iocp_evt_s iocp_ev;
        char *buf;
        uint32_t buf_size;
    } *vars = (struct vars *)task_asyn_vars_get(task, sizeof(*vars));
    DWORD dwError;

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(1);

    /* Save parameters */
    /* 保存参数 */
    vars->buf = (char *)_buf;
    vars->buf_size = _buf_size;
    vars->cli_sock = _cli_sock;
    vars->send_bytes = 0;

    /* Initialize IOCP event */
    /* 初始化IOCP事件 */
    memset(&vars->iocp_ev, 0, sizeof(vars->iocp_ev));
    event_init_inherit(&vars->iocp_ev.event, &task->event);

    while (vars->send_bytes < vars->buf_size)
    {
        vars->wsa_buf.buf = vars->buf + vars->send_bytes;
        vars->wsa_buf.len = vars->buf_size - vars->send_bytes;
        /* Start WSASend opreate */
        /* 启动WSASend操作 */
        if (SOCKET_ERROR == WSASend(vars->cli_sock, 
                        &vars->wsa_buf, 
                        1,
                        NULL,
                        0, 
                        &vars->iocp_ev.overlapped, NULL))
        {
            dwError = WSAGetLastError();
            if (dwError != ERROR_IO_PENDING)
            {
                task->ret_val.u32 = vars->send_bytes;
                /* The coroutine end */
                /* 协程结束 */
                bpd_break;
            }
        }

        /* Suspend this coroutine and wait an event. */
        /* 挂起协程，等待事件 */
        bpd_yield(1);

        /* send failed */
        /* 发送失败 */
        if (vars->iocp_ev.dwError != 0 || vars->iocp_ev.numberOfBytes == 0)
        {
            task->ret_val.u32 = vars->send_bytes;
            /* The coroutine end */
            /* 协程结束 */
            bpd_break;
        }

        vars->send_bytes += vars->iocp_ev.numberOfBytes;
    }

    task->ret_val.u32 = vars->send_bytes;

    /* coroutine end */
    /* 协程结束 */
    bpd_end();

    /* asynchronous return */
    /* 异步返回 */
    task_asyn_return(task);
}

/* client requst processing task handler */
/* 客户端请求处理任务函数 */
void http_client_requst_task_handler(task_t *task, event_t *ev, SOCKET _cli_sock, const struct sockaddr_in *_client_addr)
{
    uint8_t *bpd = TASK_BPD(task);
    struct vars
    {
        SOCKET cli_sock;
        struct sockaddr_in client_addr;
        uint8_t buf[2048];
        char method[6];
        char url[400];
        char path[512];
        char *query_string;
        uint32_t data_size;
        uint8_t cgi;
        uint8_t bps;
        uint8_t header_end;
        uint32_t i;
        HANDLE hFile;
        struct iocp_evt_s read_iocp_evt;
        uint64_t offset;
    } *vars = (struct vars *)task_asyn_vars_get(task, sizeof(*vars));
    uint8_t *buf;
    uint8_t *buf_end;
    WIN32_FIND_DATAA findInfo;
    HANDLE hFind;
    const char *str;

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(14);

    /* Save incoming parameters to asynchronous variables */
    /* 保存传入的参数至异步变量 */
    vars->cli_sock = _cli_sock;
    vars->client_addr = *_client_addr;
    vars->header_end = 0;

    while (1)
    {
        /* Asynchronously call the http_client_data_get method to get client data with timeout */
        /* 异步调用http_client_data_get方法，带超时地获取客户端数据 */
        task_bpd_asyn_call(1, task, http_client_data_get, 
                            vars->cli_sock, 
                            vars->buf,
                            sizeof(vars->buf),
                            &vars->data_size,
                            HTTP_CLIENT_RECV_TIMEOUT_MS);
        if (task->ret_val.u32 != 0 || vars->data_size == 0)
        {
            closesocket(vars->cli_sock);
            bpd_break;
        }

        /* Set buf and buf_end for parsing of the bpd block.
         * There is a line break in buf, 
         * then set the buf_end to line break postion.
         */
        /* 设置buf, buf_end以供bpd语句块解析。
         * buf中存在换行，则设置buf_end换行处
         */
        buf = vars->buf;
        for (buf_end = buf; buf_end < buf + vars->data_size; buf_end++)
        {
            if (*buf_end == '\r' || *buf_end == '\n')
            {
                break;
            }
        }

        /* bpd statement block begin */
        /* bpd语句块开始 */
        #define BP_STMT_DEFAULT         vars->bps
        #define BP_STMT_DEFAULT_PREFIX  parse_data_label
        bpd_stmt_begin(4)
        {
            /* Filter out the previous white space */
            /* 过滤掉前面的空白符 */
            do
            {
                /* Wait the buf is not empty.  */
                /* 等待buf非空 */
                bpd_stmt_yield_until(1, buf < buf_end);
            } while (isspace(*buf) && buf++);

            /* get method field */
            /* 获取method字段 */
            vars->i = 0;
            while (1)
            {
                /* Wait the buf is not empty.  */
                /* 等待buf非空 */
                bpd_stmt_yield_until(2, buf < buf_end);

                if (!isspace(*buf))
                {
                    if (vars->i < sizeof(vars->method) - 1)
                    {
                        vars->method[vars->i++] = *buf;
                    }

                    buf++;
                }
                else
                {
                    break;
                }
            }
            vars->method[vars->i] = 0;

            /* Filter out the previous white space */
            /* 过滤掉前面的空白符 */
            do
            {
                /* Wait the buf is not empty.  */
                /* 等待buf非空 */
                bpd_stmt_yield_until(3, buf < buf_end);
            } while (isspace(*buf) && buf++);

            /* get url field */
            /* 获取url字段 */
            vars->i = 0;
            while (1)
            {
                /* Wait the buf is not empty.  */
                /* 等待buf非空 */
                bpd_stmt_yield_until(4, buf < buf_end);

                if (!isspace(*buf))
                {
                    if (vars->i < sizeof(vars->url) - 1)
                    {
                        vars->url[vars->i++] = *buf;
                    }

                    buf++;
                }
                else
                {
                    break;
                }
            }
            vars->url[vars->i] = 0;
        }
        /* bpd statement block end */
        /* bpd语句块结束 */
        bpd_stmt_end();
        #undef BP_STMT_DEFAULT
        #undef BP_STMT_DEFAULT_PREFIX

        /* In one line, the method and url are not parsed, 
           http request error */
        /* 在一行中，未解析出method和url，http请求错误 */
        if (buf_end != vars->buf + vars->data_size
          && vars->bps != 0)
        {
            closesocket(vars->cli_sock);
            bpd_break;
        }

        /* parse successfully */
        /* 解析成功 */
        if (vars->bps == BP_STMT_INIT_VAL)
        {
            break;
        }
    }

    /* Read the full header */
    /* 读取整个header */
    while (1)
    {
        /* Check if there is a header end identifier in the previous buf */
        /* 检查上一个buf是否存在header结尾标识 */
        buf = buf_end;
        buf_end = vars->buf + vars->data_size;

        /* bpd statement block begin */
        /* bpd语句块开始 */
        #define BP_STMT_DEFAULT         vars->bps
        #define BP_STMT_DEFAULT_PREFIX  find_header_end
        bpd_stmt_begin(3)
        {
            while (1)
            {
                bpd_stmt_yield_until(1, buf < buf_end);
                if (*buf == '\n')
                {
                    buf++;
                    bpd_stmt_yield_until(2, buf < buf_end);
                    if (*buf == '\r')
                    {
                        buf++;
                        bpd_stmt_yield_until(3, buf < buf_end);
                    }

                    /* successfully */
                    /* 成功 */
                    if (*buf == '\n')
                    {
                        bpd_stmt_break;
                    }
                }
                else
                {
                    buf++;
                }
            }
        }
        /* bpd statement block end */
        /* bpd语句块结束 */
        bpd_stmt_end();
        #undef BP_STMT_DEFAULT
        #undef BP_STMT_DEFAULT_PREFIX

        /* Read full header finished */
        /* 读取整个头部结束 */
        if (vars->bps == BP_STMT_INIT_VAL)
        {
            break;
        }

        /* Asynchronously call the http_client_data_get method to get client data with timeout */
        /* 异步调用http_client_data_get方法，带超时地获取客户端数据 */
        task_bpd_asyn_call(2, task, http_client_data_get, 
                            vars->cli_sock, 
                            vars->buf,
                            sizeof(vars->buf),
                            &vars->data_size,
                            HTTP_CLIENT_RECV_TIMEOUT_MS);
        if (task->ret_val.u32 != 0 || vars->data_size == 0)
        {
            closesocket(vars->cli_sock);
            bpd_break;
        }

        buf_end = vars->buf;
    }

    /* Does not support non-GET and non-POST methods */
    /* 不支持非GET和非POST方法 */
    if (mystrcasecmp(vars->method, "GET") != 0 
     && mystrcasecmp(vars->method, "POST") != 0)
    {
        task_bpd_asyn_call(3, task, http_client_send,
                            vars->cli_sock,
                            http_501_rsp_text,
                            sizeof(http_501_rsp_text) - 1);

        goto client_close;
    }

    /* Processing URL and parameter information */
    /* 处理URL及参数信息 */
    if (mystrcasecmp(vars->method, "GET") == 0)
    {
        vars->query_string = vars->url;

        while ((*vars->query_string != '?') && (*vars->query_string != '\0'))
        {
            vars->query_string++;
        }
        if (*vars->query_string == '?')
        {
            vars->cgi = 1;
            *vars->query_string = '\0';
            vars->query_string++;
        }
    }

    /* Generate Path */
    /* 生成Path */
    sprintf(vars->path, "wwwroot%s", vars->url);
    if (vars->path[strlen(vars->path) - 1] == '/')
    {
        strcat(vars->path, "index.html");
    }

    /* Determine if the file exists */
    /* 判断文件是否存在 */
    hFind = FindFirstFileA(vars->path, &findInfo);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        task_bpd_asyn_call(4, task, http_client_send,
                            vars->cli_sock,
                            http_404_rsp_text,
                            sizeof(http_404_rsp_text) - 1);

        goto client_close;
    }
    FindClose(hFind);

    /* Is a directory */
    /* 是一个目录 */
    if (findInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        strcat(vars->path, "/index.html");

        hFind = FindFirstFileA(vars->path, &findInfo);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            task_bpd_asyn_call(5, task, http_client_send,
                                vars->cli_sock,
                                http_404_rsp_text,
                                sizeof(http_404_rsp_text) - 1);

            goto client_close;
        }
        FindClose(hFind);
    }

    /* Open File */
    /* 打开文件 */
    vars->hFile = CreateFileA(vars->path, 
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    NULL);
    if (vars->hFile == INVALID_HANDLE_VALUE)
    {
        task_bpd_asyn_call(6, task, http_client_send,
                            vars->cli_sock,
                            http_404_rsp_text,
                            sizeof(http_404_rsp_text) - 1);

        goto client_close;
    }
    
    /* Add HANDLE to IOCP */
    /* 添加HANDLE到IOCP */
    if (NULL == CreateIoCompletionPort(vars->hFile, s_iocp, 0, 0))
    {
        task_bpd_asyn_call(7, task, http_client_send,
                            vars->cli_sock,
                            http_404_rsp_text,
                            sizeof(http_404_rsp_text) - 1);

        CloseHandle(vars->hFile);
        goto client_close;
    }

    /* 发送HTTP头 */
    /* Send Http header */
    str = strrchr(vars->path, '.');
    if (str == NULL)
    {
        task_bpd_asyn_call(8, task, http_client_send,
                            vars->cli_sock,
                            http_ok_rsp_other,
                            sizeof(http_ok_rsp_other) - 1);
    }
    else if (mystrcasecmp(str, ".htm") == 0 || mystrcasecmp(str, ".html") == 0)
    {
        task_bpd_asyn_call(9, task, http_client_send,
                            vars->cli_sock,
                            http_ok_rsp_html,
                            sizeof(http_ok_rsp_html) - 1);
    }
    else if (mystrcasecmp(str, ".js") == 0)
    {
        task_bpd_asyn_call(10, task, http_client_send,
                            vars->cli_sock,
                            http_ok_rsp_js,
                            sizeof(http_ok_rsp_js) - 1);
    }
    else if (mystrcasecmp(str, ".css") == 0)
    {
        task_bpd_asyn_call(11, task, http_client_send,
                            vars->cli_sock,
                            http_ok_rsp_css,
                            sizeof(http_ok_rsp_css) - 1);
    }
    else
    {
        task_bpd_asyn_call(12, task, http_client_send,
                            vars->cli_sock,
                            http_ok_rsp_other,
                            sizeof(http_ok_rsp_other) - 1);
    }

    /***Send file to client***/
    /***发送文件到客户端***/
    memset(&vars->read_iocp_evt, 0, sizeof(vars->read_iocp_evt));
    event_init_inherit(&vars->read_iocp_evt.event, &task->event);
    vars->offset = 0;

    while (1)
    {
        vars->read_iocp_evt.overlapped.Offset = vars->offset & 0xFFFFFFFF;
        vars->read_iocp_evt.overlapped.OffsetHigh = vars->offset >> 32;
        /* Read File */
        /* 读文件 */
        if (FALSE == ReadFile(vars->hFile, 
                        vars->buf, 
                        sizeof(vars->buf),
                        NULL,
                        &vars->read_iocp_evt.overlapped))
        {
            if (GetLastError() != ERROR_IO_PENDING)
            {
                CloseHandle(vars->hFile);
                goto client_close;
            }
        }

        bpd_yield(13);

        /* send to client*/
        /* 发送到客户端 */
        if (vars->read_iocp_evt.numberOfBytes != 0
         && (vars->read_iocp_evt.dwError == 0 || vars->read_iocp_evt.dwError == ERROR_HANDLE_EOF))
        {
            task_bpd_asyn_call(14, task, http_client_send,
                                vars->cli_sock,
                                vars->buf,
                                vars->read_iocp_evt.numberOfBytes);

            if (task->ret_val.u32 != vars->read_iocp_evt.numberOfBytes)
            {
                CloseHandle(vars->hFile);
                goto client_close;
            }
        }

        if (vars->read_iocp_evt.dwError == ERROR_HANDLE_EOF)
        {
            CloseHandle(vars->hFile);
            goto client_close;
        }

        vars->offset += vars->read_iocp_evt.numberOfBytes;
    }

client_close:
    closesocket(vars->cli_sock);

    /* coroutine end */
    /* 协程结束 */
    bpd_end();

    /* asynchronous return */
    /* 异步返回 */
    task_asyn_return(task);
}

static void on_task_end(void *task, event_t *ev)
{
    /* free the task */
    /* 释放task */
    slab_free(http_client_tasks_slab, task);
}

/* Accept task handler */
/* Accept任务函数 */
void http_accept_task_handler(task_t *task, event_t *ev)
{
    uint8_t *bpd = TASK_BPD(task);
    struct http_task_with_stack_s *task_with_stack;
    DWORD dwError;

    /* Get or assign asynchronous variables from the task stack.
       When the coroutine is yield, the local variables of the function will be destroyed. 
       While the asynchronous variables will still exist. */
    /* 从任务的栈中获取或分配异步变量。
       协程在挂起时，函数的局部变量将被销毁，而异步变量则依旧存在 */
    struct vars
    {
        struct iocp_evt_s accept_evt;
        char acceptex_out_buffer[(sizeof(struct sockaddr_in) + 16) * 2 + 16];
        SOCKET cli_sock;
        DWORD dwBytes;
        struct sockaddr_in *clientAddr, *localAddr;
        int clientAddrLen, localAddrLen;
        timer_event_t timer;
        slab_alloc_event_t alloc_ev;
    } *vars = (struct vars *)task_asyn_vars_get(task, sizeof(*vars));

    /* coroutine begin */
    /* 协程开始 */
    bpd_begin(3);

    /* empty accept_evt structure */
    /* 清零accept_evt结构体 */
    memset(&vars->accept_evt, 0, sizeof(vars->accept_evt));
    /* The accept_evt event is inherited from the task,
       This accpet_evt event will be process by this coroutine. */
    /* accept_evt的事件从task中继承，
       这样accpet_evt事件将由本协程处理 */
    event_init_inherit(&vars->accept_evt.event, &task->event);

    /* Initialize timer */
    /* 初始化timer */
    timer_init_inherit(&vars->timer, &task->event);

    /* Initialize slab alloc event */
    /* 初始化slab分配器事件 */
    slab_alloc_event_init_inherit(&vars->alloc_ev, &task->event);

    /* Initialize http client slab */
    /* 初始化http客户端slab */
    http_client_tasks_slab = slab_create(http_client_tasks_slab_buff, 
                                sizeof(http_client_tasks_slab_buff), 
                                sizeof(struct http_task_with_stack_s));

    while (1)
    {
        /* Create a socket for the new client */
        /* 为新客户端创建一个套接字 */
        vars->cli_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (vars->cli_sock == INVALID_SOCKET) {
            printf("Create accept socket failed with error: %u\n", WSAGetLastError());

            /* Retry after 500 milliseconds */
            /* 500毫秒后重试 */
            el_timer_start_ms(&vars->timer, 500);

            /* Suspend the coroutine and wait for the timer */
            /* 挂起协程，并等待定时器 */
            bpd_yield(1);
            continue;
        }

        /* Start AcceptEx, When AcceptEx overlapped I/O is completed, 
           The accept_evt event is triggered and re-enters the coroutine. */
        /* 开始AcceptEx，当AcceptEx重叠I/O完成时，
           将触发accept_evt事件，重新进入协程 */
        if (FALSE == lpfnAcceptEx(s_sock, 
                    vars->cli_sock, 
                    vars->acceptex_out_buffer, 
                    0, 
                    sizeof(struct sockaddr_in) + 16, 
                    sizeof(struct sockaddr_in) + 16, 
                    &vars->dwBytes, 
                    &vars->accept_evt.overlapped))
        {
            dwError = WSAGetLastError();
            if (dwError != ERROR_IO_PENDING)
            {
                printf("AcceptEx failed! error:%u\n", (uint32_t)dwError);

                closesocket(vars->cli_sock);
                continue;
            }
        }

        /* Suspend this coroutine and wait an event. */
        /* 挂起协程，并等待事件 */
        bpd_yield(2);

        /* Determine whether the operation is successful */
        /* 判断操作是否成功 */
        if (vars->accept_evt.dwError != 0)
        {
            printf("AcceptEx overlapped I/O failed! error:%u\n", (uint32_t)vars->accept_evt.dwError);

            closesocket(vars->cli_sock);
            continue;
        }

        /* Getting Client Information */
        /* 获取客户端信息 */
        lpfnGetAcceptExSockAddrs(vars->acceptex_out_buffer, 
                0, 
                sizeof(struct sockaddr_in),
                sizeof(struct sockaddr_in),
                (LPSOCKADDR *)&vars->localAddr,
                &vars->localAddrLen,
                (LPSOCKADDR *)&vars->clientAddr,
                &vars->clientAddrLen);

        /* add socket handle to iocp */
        /* 添加Socket句柄到IOCP */
        if (NULL == CreateIoCompletionPort((HANDLE)vars->cli_sock, s_iocp, 0, 0))
        {
            printf("Add socket handle failed! error:%d\n", (uint32_t)GetLastError());

            closesocket(vars->cli_sock);
            continue;
        }

        /* Alloc a task from the slab*/
        /* 从slab分配一个task */
        task_with_stack = (struct http_task_with_stack_s *)slab_alloc(http_client_tasks_slab);
        while (task_with_stack == NULL)
        {
            /* Waiting for slab to be available */
            /* 等待slab可用 */
            slab_wait(http_client_tasks_slab, &vars->alloc_ev);
            bpd_yield(3);

            task_with_stack = (struct http_task_with_stack_s *)vars->alloc_ev.mem_blk;
        }

        /* Initialize the http client request task */
        /* 初始化http客户端请求task */
        task_init(&task_with_stack->task, 
                    task_with_stack->stack, 
                    sizeof(task_with_stack->stack),
                    EVENT_PRIORITY(&task->event));

        /* The listening task ends and the used memory is released in on_task_end */
        /* 监听任务结束，并在on_task_end释放使用的内存 */
        event_init(&task_with_stack->free_ev, on_task_end, task_with_stack, MIDDLE_GROUP_PRIORITY);
        task_end_wait(&task_with_stack->task, &task_with_stack->free_ev);

        /* Process the client Http request */
        /* 处理该客户端Http请求 */
        task_start(&task_with_stack->task, 
                    http_client_requst_task_handler, 
                    vars->cli_sock,
                    vars->clientAddr);
    }

    /* coroutine end */
    /* 协程结束 */
    bpd_end();
}

int main()
{
    WSADATA wsaData;
    struct sockaddr_in server_addr;
    GUID GuidAcceptEx = WSAID_ACCEPTEX;
    GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
    DWORD dwBytes;
    TASK_DEFINE(http_accept_task, 280, MIDDLE_GROUP_PRIORITY);

    WSAStartup(MAKEWORD(2,2), &wsaData);

    /* Create IOCP port */
    /* 创建IOCP端口 */
    s_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (s_iocp == NULL)
    {
        printf("Create IOCP failed! error:%d\n", (uint32_t)GetLastError());

        WSACleanup();
        return -1;
    }

    /* Win Socket initialize */
    /* Win Socket初始化 */
    s_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s_sock == INVALID_SOCKET)
    {
        printf("Create Socket failed! error:%d\n", (uint32_t)GetLastError());

        WSACleanup();
        return -1;
    }

    /* add socket handle to iocp */
    /* 添加Socket句柄到IOCP */
    if (NULL == CreateIoCompletionPort((HANDLE)s_sock, s_iocp, 0, 0))
    {
        printf("Add socket handle failed! error:%d\n", (uint32_t)GetLastError());

        closesocket(s_sock);
        WSACleanup();
        return -1;
    }

    /* set listen address */
    /* 设置监听地址 */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);

    /* bind */
    /* 绑定 */
    if (SOCKET_ERROR == bind(s_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        printf("Bind server address failed! error:%d\n", WSAGetLastError());

        closesocket(s_sock);
        WSACleanup();
        return -1;
    }

    /* listen */
    /* 监听 */
    if (SOCKET_ERROR == listen(s_sock, SOMAXCONN))
    {
        printf("Listen server address failed! error:%d\n", WSAGetLastError());

        closesocket(s_sock);
        WSACleanup();
        return -1;
    }

    /* Load the AcceptEx function into memory using WSAIoctl.
     The WSAIoctl function is an extension of the ioctlsocket()
     function that can use overlapped I/O. The function's 3rd
     through 6th parameters are input and output buffers where
     we pass the pointer to our AcceptEx function. This is used
     so that we can call the AcceptEx function directly, rather
     than refer to the Mswsock.lib library. */
     /* 使用WSAIoctl将ACCEPTEX函数加载到内存中。
      WSAIoctl函数是ioctlsocket()函数的扩展，该函数可以使用重叠的I/O。
      该函数的第3到第6个参数是输入和输出缓冲区，我们将指针传递给ACCEPTEX函数。
      这样我们就可以直接调用ACCEPTEX函数，而不是引用mswsock.lib库。*/
    if (SOCKET_ERROR == WSAIoctl(s_sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
             &GuidAcceptEx, sizeof (GuidAcceptEx), 
             &lpfnAcceptEx, sizeof (lpfnAcceptEx), 
             &dwBytes, NULL, NULL))
    {
        printf("WSAIoctl failed with error: %u\n", WSAGetLastError());

        closesocket(s_sock);
        WSACleanup();
        return -1;
    }

    if (SOCKET_ERROR == WSAIoctl(s_sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
             &GuidGetAcceptExSockAddrs, sizeof (GuidGetAcceptExSockAddrs), 
             &lpfnGetAcceptExSockAddrs, sizeof (lpfnGetAcceptExSockAddrs), 
             &dwBytes, NULL, NULL))
    {
        printf("WSAIoctl failed with error: %u\n", WSAGetLastError());

        closesocket(s_sock);
        WSACleanup();
        return -1;
    }

    /* startup http accept task */
    /* 启动http accept任务 */
    task_start(&http_accept_task, http_accept_task_handler);

    /* Run the atask event loop with IOCP */
    /* 运行带IOCP的atask事件循环 */
    iocp_atask_run(s_iocp);

    closesocket(s_sock);
    WSACleanup();
    return 0;
}
