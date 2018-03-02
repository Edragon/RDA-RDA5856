代码说明：
1. UART2宏开关在target.def里面，UART2_MODEL，可以分别设置为uart2_at/uart2_hci/uart2_loop/uart2_none，其中uart2_none表示
关闭uart2功能。
2. bal_tasks.c第283行开始，定义了UART指令和相应5850消息的结构体，
预先加了几条，余下的可以按照现有的格式自行添加，另外，由于不同
模块的消息定义在不同的文件中，为了统一，在event.h里284行新增加
了UART消息的定义，所有的UART消息需要在这里添加，为了在APP的其他
模块里能处理这些消息，必须在原来的蓝牙消息case上面增加新增的UART
消息。例如
case UART_BT_VISIBLE://新增加的UART消息
case BT_VISIBLE:

3. 如果要使用模拟器，则需要在模拟器的event.h里也增加UART消息定义。不然
使用VC6.0编译会报错

4. 睡眠唤醒机制：在需要睡眠的地方调用hal_UartAllowSleep(HAL_UART_2, 1);，唤醒的地方调用hal_UartAllowSleep(HAL_UART_2, 0);
发送数据流程：发送前唤醒，发送启动timer后睡眠，代码中已经实现。
接受数据流程：外部设备发送数据前先用gpio唤醒uart2，然后发送数据，芯片处理完数据后启动timer进入睡眠，
需要配置TGT_GPIO_CONFIG_CFG中的uart2_wake为唤醒gpio。

5. 需要确认uart2的txd rxd的配置，对应于gpio(6,8)需要设置为AS_ALT_FUNC：
// GPIO(6)  // UART2_TXD, UART1_CD
#define TGT_HAL_GPIO_6_USED AS_ALT_FUNC
// GPIO(8)  // UART2_RXD, UART1_DTR
#define TGT_HAL_GPIO_8_USED AS_ALT_FUNC
在5855里，uart2可以分别映射到两组GPIO上，分别是GPIO_3_4 - GPIO_3_7和GPIO_6_0 - GPIO_6_3（包括流控）。用第一组GPIO时，uart2PinConfig
应当设置为1，用第二组时应当设置为2。