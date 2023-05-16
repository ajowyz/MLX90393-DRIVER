void SpiInit(void);
void MLX90393Measure(void);
void MLX90393Init(void);


uint16_t XDATA=0,YDATA=0,ZDATA=0;
uint8_t Mdata[6] = {0};
uint8_t StateReg = 0;

int main(void)
{ 
  SpiInit();
	
  MLX90393Init();	
	
  while(1)
  {	
	 MLX90393Measure();		
  }
}


uint8_t write_readonebyte(uint8_t senddata)
{
	uint16_t timeout = 0;
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET)
	{
		timeout++;
		if(timeout >= 200)
			return 0;	
	}
	SPI2->DR = senddata;
	timeout = 0;	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET)
	{
		timeout++;
		if(timeout >= 200)
			return 0;	
	}
	return SPI2->DR;
}



void SpiInit(void)
{
	SPI_InitTypeDef SPI_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//4.5Mbps, SPI_BaudRatePrescaler_4  8MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
  SPI_Cmd(SPI2, ENABLE);//SPI2
	
}

void MLX90393Measure(void)
{
//SM
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0x36);
	StateReg = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	
	
	
	delay_ms(2);
	
//RR
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0x46);
	StateReg = write_readonebyte(0x00);
	Mdata[0] = write_readonebyte(0x00);
	Mdata[1] = write_readonebyte(0x00);
	Mdata[2] = write_readonebyte(0x00);
	Mdata[3] = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	XDATA=(Mdata[0]<<8)+Mdata[1];
	YDATA=(Mdata[2]<<8)+Mdata[3];
	
	

}

void MLX90393Init(void)
{
//EX
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0x80);
	StateReg = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
//Rest
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0xf0);
	StateReg = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	delay_ms(2);	
	

//0X00,0X000C
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0x60);
	write_readonebyte(0x00);//Data[15:8]
	write_readonebyte(0x0c);//Data[7:0] 
	write_readonebyte(0x00);
	StateReg = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	

	
//0X01,0X0000
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0x60);
	write_readonebyte(0x00);
	write_readonebyte(0x00);
	write_readonebyte(0x04);
	StateReg = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	

	
//0X02,0X01E0
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	write_readonebyte(0x60);
	write_readonebyte(0x01);
	write_readonebyte(0xE0);
	write_readonebyte(0x08);
	StateReg = write_readonebyte(0x00);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);


////burst
//	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//	write_readonebyte(0x16);
//	StateReg = write_readonebyte(0x00);
//	GPIO_SetBits(GPIOB,GPIO_Pin_12);

}