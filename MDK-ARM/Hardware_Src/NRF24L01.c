
#include "main.h"
#include "spi.h"
#include "stdio.h"

 uint8_t TX_ADDRESSChen[TX_ADR_WIDTH]={0xB3, 0xC2, 0x00, 0x00, 0x00}; 
 uint8_t TX_ADDRESSJia[TX_ADR_WIDTH]={0xBC, 0xD6, 0x00, 0x00, 0x00}; 
 uint8_t TX_ADDRESSTang[TX_ADR_WIDTH]={0xCC, 0xC6, 0x00, 0x00, 0x00}; 
 uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x11, 0x22, 0x33, 0x44, 0x55};

//NRF24L01 驱动函数
#define debug_out(fmt,args...)  Serial_Printf(fmt,##args)
//#define debug_out(fmt,args...) 
void delay_us(uint32_t n)
{
	unsigned char i;
	while(n--)
	{
		i = 8;
		while(i--);
	}
}
//初始化24L01的IO口
void NRF24L01_Init(void)
{
	//spi init
	//gpio init
	Clr_NRF24L01_CE;    // chip enable
	Set_NRF24L01_CSN;   // Spi disable
	while(NRF24L01_Check());
	Serial_SendByte(0xaa);
	delay_us(100);
}

//封装spi读写函数
unsigned char nRF24_SPI_Send_Byte(unsigned char txdata)
{
	unsigned char rxdata;
	HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 0x10);
	return(rxdata);							// return read unsigned char
}

//通过SPI写寄存器
unsigned char NRF24L01_Write_Reg(unsigned char regaddr,unsigned char data)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                    //使能SPI传输
	status =nRF24_SPI_Send_Byte(regaddr); //发送寄存器号
	nRF24_SPI_Send_Byte(data);            //写入寄存器的值
	Set_NRF24L01_CSN;                    //禁止SPI传输
	return(status);       		         //返回状态值
}
//读取SPI寄存器值 ，regaddr:要读的寄存器
unsigned char NRF24L01_Read_Reg(unsigned char regaddr)
{
	unsigned char reg_val;
	Clr_NRF24L01_CSN;                //使能SPI传输
	nRF24_SPI_Send_Byte(regaddr);     //发送寄存器号
	reg_val=nRF24_SPI_Send_Byte(0XFF);//读取寄存器内容
	Set_NRF24L01_CSN;                //禁止SPI传输
	return(reg_val);                 //返回状态值
}
//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
unsigned char NRF24L01_Read_Buf(unsigned char regaddr,unsigned char *pBuf,unsigned char datalen)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                     //使能SPI传输
	status=nRF24_SPI_Send_Byte(regaddr);   //发送寄存器值(位置),并读取状态值
	//for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//读出数据
	HAL_SPI_Receive(&hspi1, pBuf, datalen, 0x10);
	Set_NRF24L01_CSN;                     //关闭SPI传输
	return status;                        //返回读到的状态值
}
//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
unsigned char NRF24L01_Write_Buf(unsigned char regaddr, unsigned char *pBuf, unsigned char datalen)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                                    //使能SPI传输
	status = nRF24_SPI_Send_Byte(regaddr);                //发送寄存器值(位置),并读取状态值
	//for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI_ReadWriteByte(*pBuf++); //写入数据
	HAL_SPI_Transmit(&hspi1, pBuf, datalen, 0x10);
	Set_NRF24L01_CSN;                                    //关闭SPI传输
	return status;                                       //返回读到的状态值
}

uint8_t NRF24L01_GetRxBuf(uint8_t *Buf)
{
	uint8_t State;
	State = NRF24L01_Read_Reg(STATUS);  			
	NRF24L01_Write_Reg(nRF_WRITE_REG + STATUS, State);
	if(State & RX_OK)								
	{                                                       
		Set_NRF24L01_CE;											
		NRF24L01_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX, NOP);					
		Set_NRF24L01_CE;				
		delay_us(150);
		return 0; 
	}	   
	return 1;
}
uint8_t NRF24L01_SendTxBuf(uint8_t *Buf)
{
	uint8_t State;
	Clr_NRF24L01_CE;												
  	NRF24L01_Write_Buf(WR_TX_PLOAD, Buf, TX_PLOAD_WIDTH);	
 	Set_NRF24L01_CE;												
	while(READ_NRF24L01_IRQ==1);									
	State = NRF24L01_Read_Reg(STATUS);  					
	NRF24L01_Write_Reg(nRF_WRITE_REG + STATUS, State); 		
	if(State&MAX_TX)				 						
	{
		NRF24L01_Write_Reg(FLUSH_TX, NOP);					
		return MAX_TX; 
	}
	if(State & TX_OK)	
	{
		return TX_OK;
	}
	return NOP;	
}

uint8_t NRF24L01_Check(void)
{
	uint8_t check_in_buf[5] = {0x11 ,0x22, 0x33, 0x44, 0x55};
	uint8_t check_out_buf[5] = {0x00};
	HAL_GPIO_WritePin(NRF_SCK_GPIO_Port,NRF_SCK_Pin,GPIO_PIN_RESET);
	Set_NRF24L01_CSN;   
	Clr_NRF24L01_CE;	
	NRF24L01_Write_Buf(nRF_WRITE_REG + TX_ADDR, check_in_buf, 5);
	NRF24L01_Read_Buf(nRF_READ_REG + TX_ADDR, check_out_buf, 5);
	if((check_out_buf[0] == 0x11) && (check_out_buf[1] == 0x22) && (check_out_buf[2] == 0x33) && (check_out_buf[3] == 0x44) && (check_out_buf[4] == 0x55))
	{
		return 0;
	}
	else 	return 1;
}			

// 通过接收通道0来接收应答
void NRF24L01_TX_Mode( uint8_t *TX_ADDRESS)
{														 
	Clr_NRF24L01_CE;	    
  	NRF24L01_Write_Buf(nRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);       //设置RF通道为40
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	Set_NRF24L01_CE;//CE为高,10us后启动发送
}
// 利用接收通道1接收数据
void NRF24L01_RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P1,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	  
 	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x02);    	//使能通道1的自动应答    
 	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x02);	//使能通道1的接收地址  	 
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);	    	//设置RF通信频率		  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RX_PW_P1,RX_PLOAD_WIDTH);//选择通道1的有效数据宽度 	    
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0f);	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG, 0x0f);		//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	Set_NRF24L01_CE; //CE为高,进入接收模式 
}		

//利用接收通道0接收数据
//void NRF24L01_RX_Mode(void)
//{
//	NRF24L01_CE=0;	  
//  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
//	  
  	// NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x01);    	//使能通道0的自动应答    
  	// NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x01);	//使能通道0的接收地址  	 
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);	    	//设置RF通信频率		  
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道1的有效数据宽度 	    
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0f);	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG, 0x0f);		//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
//  	NRF24L01_CE = 1; //CE为高,进入接收模式 
//}	

void NRF24L01_SendBuf(uint8_t *Buf)
{
	Clr_NRF24L01_CE;									
	NRF24L01_Write_Reg(nRF_WRITE_REG + CONFIG, 0x0E);   
	Set_NRF24L01_CE;
	delay_us(15);
	NRF24L01_SendTxBuf(Buf);						
	Clr_NRF24L01_CE;
	NRF24L01_Write_Reg(nRF_WRITE_REG + CONFIG, 0x0F);
	Set_NRF24L01_CE;	
}

uint8_t NRF24L01_Get_Value_Flag(void)
{
	return !READ_NRF24L01_IRQ;
}
