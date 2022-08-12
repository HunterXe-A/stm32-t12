#include "oledfont.h"
#include "oled.h"

#define OLED_ADD 0x78 

 /**
  * @brief  oled_cmd����OLEDд������
  * @param  I2C_Command���������
  * @retval ��
  */
static void oled_cmd(uint8_t i2c_cmd)
{
    uint8_t *cmd;
    cmd = &i2c_cmd;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0x00, 
                      I2C_MEMADD_SIZE_8BIT, cmd, 1,
                      100);
}

 /**
  * @brief  oled_data����OLEDд������
  * @param  I2C_Data������
  * @retval ��
  */
static void oled_data(uint8_t i2c_data)
{
    uint8_t *data;
    data = &i2c_data;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0x40, 
                      I2C_MEMADD_SIZE_8BIT, data, 1,
                      100);
}

 /**
  * @brief  oled_setPos�����ù��
  * @param  x,���xλ��
	*					y�����yλ��
  * @retval ��
  */
static void oled_setPos(uint8_t x, uint8_t y) 
{
    oled_cmd(0xb0+y);
	oled_cmd(((x&0xf0)>>4)|0x10);
	oled_cmd((x&0x0f)|0x01);
}

 /**
  * @brief  oled_full�����������Ļ
  * @param  data:      Ҫ��������
  * @retval ��
  */
void oled_full(uint8_t data)//0xff
{
    uint8_t m,n;
	for(m=0;m<8;m++)
	{
		oled_cmd(0xb0+m);		//page0-page1
		oled_cmd(0x00);		//low column start address
		oled_cmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				oled_data(data);
			}
	}
} 

 /**
  * @brief  oled_cls������
  * @param  ��
	* @retval ��
  */
void oled_cls(void)//����
{
	oled_full(0x00);
}

 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
	* @retval ��
  */
void oled_on(void)
{
	oled_cmd(0X8D);  //���õ�ɱ�
	oled_cmd(0X14);  //������ɱ�
	oled_cmd(0XAF);  //OLED����
}

 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * @param  ��
	* @retval ��
  */
void oled_off(void)
{
	oled_cmd(0X8D);  //���õ�ɱ�
	oled_cmd(0X10);  //�رյ�ɱ�
	oled_cmd(0XAE);  //OLED����
}

/**
  * @brief  oled_init����ʼ��OLED
  * @param  ��
  * @retval ��
  */
void oled_init(void)
{
    HAL_Delay(100);
	
	oled_cmd(0xAE); //display off
	oled_cmd(0x20);	//Set Memory Addressing Mode	
	oled_cmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	oled_cmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	oled_cmd(0xc8);	//Set COM Output Scan Direction
	oled_cmd(0x00); //---set low column address
	oled_cmd(0x10); //---set high column address
	oled_cmd(0x40); //--set start line address
	oled_cmd(0x81); //--set contrast control register
	oled_cmd(0xff); //����?���̡�?�� 0x00~0xff
	oled_cmd(0xa1); //--set segment re-map 0 to 127
	oled_cmd(0xa6); //--set normal display
	oled_cmd(0xa8); //--set multiplex ratio(1 to 64)
	oled_cmd(0x3F); //
	oled_cmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	oled_cmd(0xd3); //-set display offset
	oled_cmd(0x00); //-not offset
	oled_cmd(0xd5); //--set display clock divide ratio/oscillator frequency
	oled_cmd(0xf0); //--set divide ratio
	oled_cmd(0xd9); //--set pre-charge period
	oled_cmd(0x22); //
	oled_cmd(0xda); //--set com pins hardware configuration
	oled_cmd(0x12);
	oled_cmd(0xdb); //--set vcomh
	oled_cmd(0x20); //0x20,0.77xVcc
	oled_cmd(0x8d); //--set DC-DC enable
	oled_cmd(0x14); //
	oled_cmd(0xaf); //--turn on oled panel
    
    oled_full(0x00);
}

 /**
  * @brief  oled_display_char����ʾ�ַ�
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
	*					chr:- Ҫ��ʾ���ַ�; 
	*					Char_Size : �ַ���С(16/8)
	* @retval ��
  */
void oled_display_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size == 16)
		{
			oled_setPos(x,y);	
			for(i=0;i<8;i++)
			oled_data(F8X16[c*16+i]);
			oled_setPos(x,y+1);
			for(i=0;i<8;i++)
			oled_data(F8X16[c*16+i+8]);
		}
		else 
		{	
			oled_setPos(x,y);
			for(i=0;i<6;i++)
			oled_data(F6x8[c][i]);
		}
}

 /**
  * @brief  oled_show_string����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
	*					ch[] :- Ҫ��ʾ���ַ���; 
	*					TextSize : �ַ���С(1:6*8 ; 2:8*16)
	* @retval ��
  */
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize)
{
    uint8_t c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				oled_setPos(x,y);
				for(i=0;i<6;i++)
					oled_data(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				oled_setPos(x,y);
				for(i=0;i<8;i++)
					oled_data(F8X16[c*16+i]);
				oled_setPos(x,y+1);
				for(i=0;i<8;i++)
					oled_data(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

 /**
  * @brief  oled_showCN����ʾcodetab.h�еĺ���,16*16����
  * @param  x,y: ��ʼ������(x:0~127, y:0~7); 
	*					N:������codetab.h�е�����
	* @retval ��
  */
void oled_showCN(unsigned char x, unsigned char y, unsigned char CN[],unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	oled_setPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		oled_data(CN[adder]);
		adder += 1;
	}
	oled_setPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		oled_data(CN[adder]);
		adder += 1;
	}
}

 /**
  * @brief  oled_drawBMP����ʾBMPλͼ
  * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
	*					x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
	* @retval ��
  */
void oled_drawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		oled_setPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			oled_data(BMP[j++]);
		}
	}
}

//m^n
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

 /**
  * @brief  oled_showNum����ʾ����
  * @param  x,y: ��ʼ������(x:0~127, y:0~7); 
			num��Ҫ��ʾ������
			len��num��ʮ����λ��
			sizel�����������С 12/16/24
  * @retval ��
  */
void oled_showNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1)
{
	uint8_t t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(temp==0)
		{
			oled_display_char(x+(size1/2)*t,y,'0',size1);
		}
		else 
		{
			oled_display_char(x+(size1/2)*t,y,temp+'0',size1);
		}
  }
}

 /**
  * @brief  show_title����ʾ����
  * @param  ��
  * @retval ��
  */
void show_title(void)
{
	uint32_t i;
	
	oled_cls();
	for(i=0; i<=10; i++)  
	{
		if(i<=4)
			oled_showCN(20+i*16,1,Title,i);  //�ڵ�һ����ʾǰ����ַ���ȫ����ѧ����
		else
			oled_showCN(10+(i-5)*16,4,Title,i);  //�ڵڶ�����ʾ�������ַ���������ƴ�����
	}
}
