#include "sys.h"
#include "usart.h"	
#include "upcom.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[USART_REC_LEN]; 
u8 USART4_RX_BUF[USART_REC_LEN];
u8 USART5_RX_BUF[USART_REC_LEN];

u8 BT_BUF[BT_BUF_LEN];
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

void uart2_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
    
    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
    
    //USART2_RX	  GPIOA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10     
    
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 

}

//uart4 tx ��PC10
void uart4_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��USART2��GPIOCʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.2
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PC11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC.11  
    
    //UART4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�    
    USART_Init(UART4, &USART_InitStructure); //��ʼ������2
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���2 

}


void uart5_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��USART2��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��USART2��GPIOAʱ��
    
    //UART5_TX   GPIOC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC12

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD2 
    
    //UART4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�    
    USART_Init(UART5, &USART_InitStructure); //��ʼ������2
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���2 


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
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1      

}

u8 msgFlag=0;

void sendComAnalyzeEvent(void)
{
    msgFlag = NEW_MSG;     
}

u8 connectionStatus = 0;

void checkBTMsg(u8 res){
    if((USART_RX_STA&0x8000)==0)//����δ���
	{
	    if(USART_RX_STA&0x4000)//���յ���0x0d
	    {
		    if(res!=0x0d){
                USART_RX_STA=0;//���մ���,���¿�ʼ
            }else{
                USART_RX_STA|=0x8000;	//��������� 
                //connectionStatus = CONNECTED;
            }
        }else{ //��û�յ�0X0D			
		    if(res==0x0a){
                USART_RX_STA|=0x4000;
            }else{
			    BT_BUF[USART_RX_STA&0X3FFF]=res ;
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1)){
                    USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
                } 
			}		 
		}
    }   	

}

u8 transFlagU5 = TRANS_NONE;
u8 usartRecieveCounterU5 = 0;

void UART5_IRQHandler(void){
    u8 res;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){  //�����ж�
        res = USART_ReceiveData(UART5); 	//��ȡ���յ�������   
        
        //������յ�����0xFF
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
        //����յ���0xFE            
        }else if(res == 0xFE){
            switch (transFlagU5){
                case TRANS_SENDING:
                    transFlagU5 = TRANS_READYEND;
                    break;
                case TRANS_READYEND:
                    transFlagU5 = TRANS_END;
                
                    //���ͱ�־, ��ʼ����
                    sendComAnalyzeEvent();
                    break;
                default:
                   //���У����������254, Ҳ��Ҫȥ������.
                   transFlagU5 = TRANS_SENDING; 
            }
            
            USART5_RX_BUF[usartRecieveCounterU5] = res;
            usartRecieveCounterU5++;
                   
        }else{                     
            //����յ��ķ�0xFF 0xFE, ���̽�״̬���⵽�����е�״̬.
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
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){  //�����ж�
        res = USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������   
        //checkBTMsg(res);
        
        //������յ�����0xFF
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
        //����յ���0xFE            
        }else if(res == 0xFE){
            switch (transFlagU4){
                case TRANS_SENDING:
                    transFlagU4 = TRANS_READYEND;
                    break;
                case TRANS_READYEND:
                    transFlagU4 = TRANS_END;
                
                    //���ͱ�־, ��ʼ����
                    sendComAnalyzeEvent();
                    break;
                default:
                   //���У����������254, Ҳ��Ҫȥ������.
                   transFlagU4 = TRANS_SENDING; 
            }
            
            USART4_RX_BUF[usartRecieveCounterU4] = res;
            usartRecieveCounterU4++;
                   
        }else{                     
            //����յ��ķ�0xFF 0xFE, ���̽�״̬���⵽�����е�״̬.
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
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  //�����ж�
        res = USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������   
        //checkBTMsg(res);
        
        //������յ�����0xFF
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
        //����յ���0xFE            
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
                   //���У����������254, Ҳ��Ҫȥ������.
                   transFlagU2 = TRANS_SENDING; 
            }
            
            USART2_RX_BUF[usartRecieveCounterU2] = res;
            usartRecieveCounterU2++;
                   
        }else{                     
            //����յ��ķ�0xFF 0xFE, ���̽�״̬���⵽�����е�״̬.
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
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){  //�����ж�
        res = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������   
        //checkBTMsg(res);
        
        //������յ�����0xFF
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
        //����յ���0xFE            
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
                   //���У����������254, Ҳ��Ҫȥ������.
                   transFlag = TRANS_SENDING; 
            }
            
            USART_RX_BUF[usartRecieveCounter] = res;
            usartRecieveCounter++;
                   
        }else{           
            
            //����յ��ķ�0xFF0xFE, ���̽�״̬���⵽�����е�״̬.
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

void USART1_IRQHandler_back(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	

