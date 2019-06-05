#include "sys.h"
#include "usart.h"	
#include "upcom.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[USART_REC_LEN]; 
u8 USART4_RX_BUF[USART_REC_LEN];
u8 USART5_RX_BUF[USART_REC_LEN];

u8 BT_BUF[BT_BUF_LEN];
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

void uart2_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
    
    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
    
    //USART2_RX	  GPIOA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10     
    
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART2, ENABLE);                    //使能串口2 

}

//uart4 tx 是PC10
void uart4_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能USART2，GPIOC时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.2
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PC11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.11  
    
    //UART4 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断    
    USART_Init(UART4, &USART_InitStructure); //初始化串口2
    USART_Cmd(UART4, ENABLE);                    //使能串口2 

}


void uart5_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能USART2，GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//使能USART2，GPIOA时钟
    
    //UART5_TX   GPIOC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC12

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD2 
    
    //UART4 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断    
    USART_Init(UART5, &USART_InitStructure); //初始化串口2
    USART_Cmd(UART5, ENABLE);                    //使能串口2 


}

u8 txBuffer[TX_MAX_LENGTH];
u16 txBufferLength;

void cleanTxBuffer(){
    u16 i;
    for(i = 0; i < TX_MAX_LENGTH; i++){
        txBuffer[i]=0;    
    }
}

void cpStr2TxBuffer(const char *str){

    u16 i;
    cleanTxBuffer(); 
    for(i = 0; i < txBufferLength - 1; i++){
        txBuffer[i]=str[i];
    }
    
}

void uart2Send(const char *str,u8 length){
    u8 TxCounter = 0;
   
    while(TxCounter < length){
        USART_SendData(USART2,str[TxCounter++]);
        
        while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){
        //just wait
        }     
    }
    TxCounter = 0;
}

void uart4Send(const char *str,u8 length){
    u8 TxCounter = 0;
       
    while(TxCounter < length){
        USART_SendData(UART4,str[TxCounter++]);
        
        while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET){
        //just wait
        }     
    }
    TxCounter = 0;

}

void uart5Send(const char *str,u8 length){
    u8 TxCounter = 0;
     
    while(TxCounter < length){
        USART_SendData(UART5,str[TxCounter++]);
        
        while(USART_GetFlagStatus(UART5,USART_FLAG_TXE)==RESET){
        //just wait
        }     
    }
    TxCounter = 0;

}

void btSend(const char *str,u8 length){
    u8 TxCounter = 0;
   
    //cpStr2TxBuffer(str); 
    
    while(TxCounter < length){
        USART_SendData(USART1,str[TxCounter++]);
        
        while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){
        //just wait
        }     
    }
    TxCounter = 0;        
}


void uart1_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
    //USART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART1, ENABLE);                    //使能串口1      

}

u8 msgFlag=0;

void sendComAnalyzeEvent(void)
{
    msgFlag = NEW_MSG;     
}

u8 connectionStatus = 0;

void checkBTMsg(u8 res){
    if((USART_RX_STA&0x8000)==0)//接收未完成
	{
	    if(USART_RX_STA&0x4000)//接收到了0x0d
	    {
		    if(res!=0x0d){
                USART_RX_STA=0;//接收错误,重新开始
            }else{
                USART_RX_STA|=0x8000;	//接收完成了 
                //connectionStatus = CONNECTED;
            }
        }else{ //还没收到0X0D			
		    if(res==0x0a){
                USART_RX_STA|=0x4000;
            }else{
			    BT_BUF[USART_RX_STA&0X3FFF]=res ;
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1)){
                    USART_RX_STA=0;//接收数据错误,重新开始接收	 
                } 
			}		 
		}
    }   	

}

u8 transFlagU5 = TRANS_NONE;
u8 usartRecieveCounterU5 = 0;

void UART5_IRQHandler(void){
    u8 res;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){  //接收中断
        res = USART_ReceiveData(UART5); 	//读取接收到的数据   
        
        //如果接收到的是0xFF
        if(res == 0xFF){
            switch(transFlagU5){
                case TRANS_NONE:
                    usartRecieveCounterU5 = 0;
                    transFlagU5 = TRANS_HALFSTART;
                    break;
                case TRANS_HALFSTART:
                    transFlagU5 = TRANS_SENDING;
                    break;
                //case TRANS_SENDING:
                    //transFlag = TRANS_READYEND;
                    //break;                                                    
            }
            USART5_RX_BUF[usartRecieveCounterU5] = res;
            usartRecieveCounterU5++;
        //如果收到了0xFE            
        }else if(res == 0xFE){
            switch (transFlagU5){
                case TRANS_SENDING:
                    transFlagU5 = TRANS_READYEND;
                    break;
                case TRANS_READYEND:
                    transFlagU5 = TRANS_END;
                
                    //发送标志, 开始分析
                    sendComAnalyzeEvent();
                    break;
                default:
                   //如果校验数正好是254, 也需要去掉误判.
                   transFlagU5 = TRANS_SENDING; 
            }
            
            USART5_RX_BUF[usartRecieveCounterU5] = res;
            usartRecieveCounterU5++;
                   
        }else{                     
            //如果收到的非0xFF 0xFE, 即刻将状态机扳到发送中的状态.
            if(transFlagU5 == TRANS_SENDING){
                USART5_RX_BUF[usartRecieveCounterU5] = res;
                usartRecieveCounterU5++;                
            }else{
                //checkBTMsg(res);
            }
            //transFlag = TRANS_SENDING;           
        }     
        //USART_RX_BUF[usartRecieveCounter] = res;          
    }
}


u8 transFlagU4 = TRANS_NONE;
u8 usartRecieveCounterU4 = 0;

void UART4_IRQHandler(void){
    u8 res;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){  //接收中断
        res = USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据   
        //checkBTMsg(res);
        
        //如果接收到的是0xFF
        if(res == 0xFF){
            switch(transFlagU4){
                case TRANS_NONE:
                    usartRecieveCounterU4 = 0;
                    transFlagU4 = TRANS_HALFSTART;
                    break;
                case TRANS_HALFSTART:
                    transFlagU4 = TRANS_SENDING;
                    break;
                //case TRANS_SENDING:
                    //transFlag = TRANS_READYEND;
                    //break;                                                    
            }
            USART4_RX_BUF[usartRecieveCounterU4] = res;
            usartRecieveCounterU4++;
        //如果收到了0xFE            
        }else if(res == 0xFE){
            switch (transFlagU4){
                case TRANS_SENDING:
                    transFlagU4 = TRANS_READYEND;
                    break;
                case TRANS_READYEND:
                    transFlagU4 = TRANS_END;
                
                    //发送标志, 开始分析
                    sendComAnalyzeEvent();
                    break;
                default:
                   //如果校验数正好是254, 也需要去掉误判.
                   transFlagU4 = TRANS_SENDING; 
            }
            
            USART4_RX_BUF[usartRecieveCounterU4] = res;
            usartRecieveCounterU4++;
                   
        }else{                     
            //如果收到的非0xFF 0xFE, 即刻将状态机扳到发送中的状态.
            if(transFlagU4 == TRANS_SENDING){
                USART4_RX_BUF[usartRecieveCounterU4] = res;
                usartRecieveCounterU4++;                
            }else{
                //checkBTMsg(res);
            }
            //transFlag = TRANS_SENDING;           
        }     
        //USART_RX_BUF[usartRecieveCounter] = res;          
    }
}


u8 transFlagU2 = TRANS_NONE;
u8 usartRecieveCounterU2 = 0;

void USART2_IRQHandler(void){
    u8 res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  //接收中断
        res = USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据   
        //checkBTMsg(res);
        
        //如果接收到的是0xFF
        if(res == 0xFF){
            switch(transFlagU2){
                case TRANS_NONE:
                    usartRecieveCounterU2 = 0;
                    transFlagU2 = TRANS_HALFSTART;
                    break;
                case TRANS_HALFSTART:
                    transFlagU2 = TRANS_SENDING;
                    break;
                //case TRANS_SENDING:
                    //transFlag = TRANS_READYEND;
                    //break;                                                    
            }
            USART2_RX_BUF[usartRecieveCounterU2] = res;
            usartRecieveCounterU2++;
        //如果收到了0xFE            
        }else if(res == 0xFE){
            switch (transFlagU2){
                case TRANS_SENDING:
                    transFlagU2 = TRANS_READYEND;
                    break;
                case TRANS_READYEND:
                    transFlagU2 = TRANS_END;
                    //comAnalyze();
                    sendComAnalyzeEvent();
                    break;
                default:
                   //如果校验数正好是254, 也需要去掉误判.
                   transFlagU2 = TRANS_SENDING; 
            }
            
            USART2_RX_BUF[usartRecieveCounterU2] = res;
            usartRecieveCounterU2++;
                   
        }else{                     
            //如果收到的非0xFF 0xFE, 即刻将状态机扳到发送中的状态.
            if(transFlagU2 == TRANS_SENDING){
                USART2_RX_BUF[usartRecieveCounterU2] = res;
                usartRecieveCounterU2++;                
            }else{
                //checkBTMsg(res);
            }
            //transFlag = TRANS_SENDING;           
        }     
        //USART_RX_BUF[usartRecieveCounter] = res;          
    }
}

u8 transFlag = TRANS_NONE;
u8 usartRecieveCounter = 0;

void USART1_IRQHandler(void){
    u8 res;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){  //接收中断
        res = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据   
        //checkBTMsg(res);
        
        //如果接收到的是0xFF
        if(res == 0xFF){
            switch(transFlag){
                case TRANS_NONE:
                    usartRecieveCounter = 0;
                    transFlag = TRANS_HALFSTART;
                    break;
                case TRANS_HALFSTART:
                    transFlag = TRANS_SENDING;
                    break;
                //case TRANS_SENDING:
                    //transFlag = TRANS_READYEND;
                    //break;                                                    
            }
            USART_RX_BUF[usartRecieveCounter] = res;
            usartRecieveCounter++;
        //如果收到了0xFE            
        }else if(res == 0xFE){
            switch (transFlag){
                case TRANS_SENDING:
                    transFlag = TRANS_READYEND;
                    break;
                case TRANS_READYEND:
                    transFlag = TRANS_END;
                    //comAnalyze();
                    sendComAnalyzeEvent();
                    break;
                default:
                   //如果校验数正好是254, 也需要去掉误判.
                   transFlag = TRANS_SENDING; 
            }
            
            USART_RX_BUF[usartRecieveCounter] = res;
            usartRecieveCounter++;
                   
        }else{           
            
            //如果收到的非0xFF0xFE, 即刻将状态机扳到发送中的状态.
            if(transFlag == TRANS_SENDING){
                USART_RX_BUF[usartRecieveCounter] = res;
                usartRecieveCounter++;                
            }else{
                checkBTMsg(res);
            }
            //transFlag = TRANS_SENDING;           
        } 
    
        //USART_RX_BUF[usartRecieveCounter] = res;  
        
    }
}

void USART1_IRQHandler_back(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

