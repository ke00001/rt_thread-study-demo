#include "board.h"
#include<rtthread.h>
#include<rtdevice.h>

/*
 * 在使用 RT-Thread 提供的消息队列函数的时候，需要了解以下几点：
    1. 使用 rt_mq_recv()、 rt_mq_send()、 rt_mq_delete()等这些函数之前应先创建需消息
            队列，并根据队列句柄进行操作。

    2. 队列读取采用的是先进先出（ FIFO）模式，会首先读取出首先存储在队列中的数
            据。当然也有例外， RT-Thread 给我们提供了另一个函数，可以发送紧急消息的，
            那么读取的时候就会读取到紧急消息的数据。

    3. 必须要我们定义一个存储读取出来的数据的地方，并且把存储数据的起始地址传
            递给 rt_mq_recv()函数，否则，将发生地址非法的错误。

    4. 接收消息队列中的消息是拷贝的方式，读取消息时候定义的地址必须保证能存放
            下即将读取消息的大小。
*/
#define DBG_TAG "rt_mq_demo"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "rt_demo.h"
#ifdef RT_MQ_DEMO

static rt_mq_t test_mq = RT_NULL; //必须定义成全局变量
static rt_thread_t test_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;


static void test_mq_thread_entry(void *par)
{

    rt_err_t err = RT_EOK;
    uint32_t recv_data = 1;

    test_mq = rt_mq_create("test_mq", 40, 20, RT_IPC_FLAG_FIFO);
    if(test_mq != RT_NULL)
    {
        rt_kprintf("消息队列创建成功\r\n");
    }
    while(1)
    {
        err = rt_mq_recv(test_mq,&recv_data, 4, RT_WAITING_FOREVER);
        if(err == RT_EOK)
        {
            rt_kprintf("消息接收成功 data: %d\r\n",recv_data);
        }
        rt_thread_mdelay(20);

    }
}
static void send_thread_entry(void *par)
{
    rt_err_t err = RT_EOK;
    uint32_t senf_data_1 = 1;
    uint32_t senf_data_2 = 2;

    while(1)
    {
        rt_thread_mdelay(2000);
        err = rt_mq_send(test_mq, &senf_data_1, 4);
        if(err != RT_EOK)
        {
            rt_kprintf("消息1发送失败\r\n");
        }
        rt_thread_mdelay(2000);
        err = rt_mq_send(test_mq, &senf_data_2, 4);
        if(err != RT_EOK)
        {
            rt_kprintf("消息2发送失败\r\n");
        }


    }

}

void rt_mq_demo_thread_entry(void *par)
{

    test_thread = rt_thread_create("test_mq", test_mq_thread_entry, RT_NULL, 512, 5, 20);
    if (test_thread != RT_NULL)
        rt_thread_startup(test_thread);
    else rt_kprintf("test_thread err");

   send_thread = rt_thread_create("send_mq", send_thread_entry, RT_NULL, 512, 4, 20);
    if (send_thread != RT_NULL)
        rt_thread_startup(send_thread);
    else rt_kprintf("send_thread err");

}
#endif

