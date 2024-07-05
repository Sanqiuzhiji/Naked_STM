
#include "main.h"
#include "spi.h"
#include "stdio.h"

 uint8_t TX_ADDRESSChen[TX_ADR_WIDTH]={0xB3, 0xC2, 0x00, 0x00, 0x00}; 
 uint8_t TX_ADDRESSJia[TX_ADR_WIDTH]={0xBC, 0xD6, 0x00, 0x00, 0x00}; 
 uint8_t TX_ADDRESSTang[TX_ADR_WIDTH]={0xCC, 0xC6, 0x00, 0x00, 0x00}; 
 uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x11, 0x22, 0x33, 0x44, 0x55};

//NRF24L01 ��������
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
//��ʼ��24L01��IO��
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

//��װspi��д����
unsigned char nRF24_SPI_Send_Byte(unsigned char txdata)
{
	unsigned char rxdata;
	HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 0x10);
	return(rxdata);							// return read unsigned char
}

//ͨ��SPIд�Ĵ���
unsigned char NRF24L01_Write_Reg(unsigned char regaddr,unsigned char data)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                    //ʹ��SPI����
	status =nRF24_SPI_Send_Byte(regaddr); //���ͼĴ�����
	nRF24_SPI_Send_Byte(data);            //д��Ĵ�����ֵ
	Set_NRF24L01_CSN;                    //��ֹSPI����
	return(status);       		         //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
unsigned char NRF24L01_Read_Reg(unsigned char regaddr)
{
	unsigned char reg_val;
	Clr_NRF24L01_CSN;                //ʹ��SPI����
	nRF24_SPI_Send_Byte(regaddr);     //���ͼĴ�����
	reg_val=nRF24_SPI_Send_Byte(0XFF);//��ȡ�Ĵ�������
	Set_NRF24L01_CSN;                //��ֹSPI����
	return(reg_val);                 //����״ֵ̬
}
//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
unsigned char NRF24L01_Read_Buf(unsigned char regaddr,unsigned char *pBuf,unsigned char datalen)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                     //ʹ��SPI����
	status=nRF24_SPI_Send_Byte(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	//for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//��������
	HAL_SPI_Receive(&hspi1, pBuf, datalen, 0x10);
	Set_NRF24L01_CSN;                     //�ر�SPI����
	return status;                        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
unsigned char NRF24L01_Write_Buf(unsigned char regaddr, unsigned char *pBuf, unsigned char datalen)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                                    //ʹ��SPI����
	status = nRF24_SPI_Send_Byte(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	//for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI_ReadWriteByte(*pBuf++); //д������
	HAL_SPI_Transmit(&hspi1, pBuf, datalen, 0x10);
	Set_NRF24L01_CSN;                                    //�ر�SPI����
	return status;                                       //���ض�����״ֵ̬
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

// ͨ������ͨ��0������Ӧ��
void NRF24L01_TX_Mode( uint8_t *TX_ADDRESS)
{														 
	Clr_NRF24L01_CE;	    
  	NRF24L01_Write_Buf(nRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	Set_NRF24L01_CE;//CEΪ��,10us����������
}
// ���ý���ͨ��1��������
void NRF24L01_RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P1,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
 	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x02);    	//ʹ��ͨ��1���Զ�Ӧ��    
 	NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x02);	//ʹ��ͨ��1�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);	    	//����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RX_PW_P1,RX_PLOAD_WIDTH);//ѡ��ͨ��1����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0f);	//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG, 0x0f);		//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	Set_NRF24L01_CE; //CEΪ��,�������ģʽ 
}		

//���ý���ͨ��0��������
//void NRF24L01_RX_Mode(void)
//{
//	NRF24L01_CE=0;	  
//  	NRF24L01_Write_Buf(nRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
//	  
  	// NRF24L01_Write_Reg(nRF_WRITE_REG+EN_AA,0x01);    	//ʹ��ͨ��0���Զ�Ӧ��    
  	// NRF24L01_Write_Reg(nRF_WRITE_REG+EN_RXADDR,0x01);	//ʹ��ͨ��0�Ľ��յ�ַ  	 
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_CH,0);	    	//����RFͨ��Ƶ��		  
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��1����Ч���ݿ�� 	    
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+RF_SETUP,0x0f);	//����TX�������,0db����,2Mbps,���������濪��   
//  	NRF24L01_Write_Reg(nRF_WRITE_REG+CONFIG, 0x0f);		//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
//  	NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
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
