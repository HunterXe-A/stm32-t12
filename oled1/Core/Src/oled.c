#include "oledfont.h"
#include "oled.h"

#define OLED_ADD 0x78 

 /**
  * @brief  oled_cmd，向OLED写入命令
  * @param  I2C_Command：命令代码
  * @retval 无
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
  * @brief  oled_data，向OLED写入数据
  * @param  I2C_Data：数据
  * @retval 无
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
  * @brief  oled_setPos，设置光标
  * @param  x,光标x位置
	*					y，光标y位置
  * @retval 无
  */
static void oled_setPos(uint8_t x, uint8_t y) 
{
    oled_cmd(0xb0+y);
	oled_cmd(((x&0xf0)>>4)|0x10);
	oled_cmd((x&0x0f)|0x01);
}

 /**
  * @brief  oled_full，填充整个屏幕
  * @param  data:      要填充的数据
  * @retval 无
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
  * @brief  oled_cls，清屏
  * @param  无
	* @retval 无
  */
void oled_cls(void)//清屏
{
	oled_full(0x00);
}

 /**
  * @brief  OLED_ON，将OLED从休眠中唤醒
  * @param  无
	* @retval 无
  */
void oled_on(void)
{
	oled_cmd(0X8D);  //设置电荷泵
	oled_cmd(0X14);  //开启电荷泵
	oled_cmd(0XAF);  //OLED唤醒
}

 /**
  * @brief  OLED_OFF，让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  * @param  无
	* @retval 无
  */
void oled_off(void)
{
	oled_cmd(0X8D);  //设置电荷泵
	oled_cmd(0X10);  //关闭电荷泵
	oled_cmd(0XAE);  //OLED休眠
}

/**
  * @brief  oled_init，初始化OLED
  * @param  无
  * @retval 无
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
	oled_cmd(0xff); //áá?èμ÷?ú 0x00~0xff
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
  * @brief  oled_display_char，显示字符
  * @param  x,y : 起始点坐标(x:0~127, y:0~7);
	*					chr:- 要显示的字符; 
	*					Char_Size : 字符大小(16/8)
	* @retval 无
  */
void oled_display_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
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
  * @brief  oled_show_string，显示codetab.h中的ASCII字符,有6*8和8*16可选择
  * @param  x,y : 起始点坐标(x:0~127, y:0~7);
	*					ch[] :- 要显示的字符串; 
	*					TextSize : 字符大小(1:6*8 ; 2:8*16)
	* @retval 无
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
  * @brief  oled_showCN，显示codetab.h中的汉字,16*16点阵
  * @param  x,y: 起始点坐标(x:0~127, y:0~7); 
	*					N:汉字在codetab.h中的索引
	* @retval 无
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
  * @brief  oled_drawBMP，显示BMP位图
  * @param  x0,y0 :起始点坐标(x0:0~127, y0:0~7);
	*					x1,y1 : 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
	* @retval 无
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
  * @brief  oled_showNum，显示数字
  * @param  x,y: 起始点坐标(x:0~127, y:0~7); 
			num：要显示的数字
			len：num的十进制位数
			sizel：数字字体大小 12/16/24
  * @retval 无
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
  * @brief  show_title，显示标题
  * @param  无
  * @retval 无
  */
void show_title(void)
{
	uint32_t i;
	
	oled_cls();
	for(i=0; i<=10; i++)  
	{
		if(i<=4)
			oled_showCN(20+i*16,1,Title,i);  //在第一行显示前五个字符“全国大学生”
		else
			oled_showCN(10+(i-5)*16,4,Title,i);  //在第二行显示后六个字符“电子设计大赛”
	}
}
