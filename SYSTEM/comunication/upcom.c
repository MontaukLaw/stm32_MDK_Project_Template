#include "upcom.h"
#include "led.h"

extern u8 transFlag;
extern u8 usartRecieveCounter;

extern u8 SN[];
//查询序列号
void Get_ChipID(void)
{
    u32 ChipUniqueID[3];

    ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8); 
    ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); 
    ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0);  
    //有人说要注意大端小端...
    SN[6]=ChipUniqueID[0] >> 24;
    SN[7]=ChipUniqueID[0] >> 16;
    SN[8]=ChipUniqueID[0] >> 8;
    SN[9]=ChipUniqueID[0];
    SN[10]=ChipUniqueID[1] >> 24;
    SN[11]=ChipUniqueID[1] >> 16 ;
    SN[12]=ChipUniqueID[1] >> 8;
    SN[13]=ChipUniqueID[1];
    SN[14]=ChipUniqueID[2] >> 24;
    SN[15]=ChipUniqueID[2] >> 16;
    SN[16]=ChipUniqueID[2] >> 8;
    SN[17]=ChipUniqueID[2];
}



