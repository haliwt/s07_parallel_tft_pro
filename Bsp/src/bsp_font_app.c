#include "bsp.h"

/********************************************************************
*
* name         : void TFT_ShowChar_576(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
* description  : 绘制字符串，可设置前景色和背景色
				 字体宽度，高度，字符，字符颜色，背景颜色
* Input        : 
* Return       : 
*
********************************************************************/
#if NORMAL_DISPLAY
void TFT_Disp_Numbers_Pic_413(uint16_t x,uint16_t y,uint8_t bc,uint8_t num)
{
    uint16_t temp, t, tbit;
    uint16_t x0=x;
	uint8_t mode;
	mode =0;
    
	static uint16_t color;

	for(t = 0; t < 413; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font_pic_num[num][t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80){
				
				color =BLACK; // WHITE;

			}
			else if(0 == mode){
				if(bc==0)
				  color = WHITE;//BLACK;
				else
				  color = BLACK;

			}
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 52){ //英文数字是中文宽度的一半 110/2=45
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  
  
	
}
#else 
void TFT_MainDisp_Numbers_Pic_354(uint16_t x,uint16_t y,uint8_t bc,uint8_t num)
{
    uint16_t temp, t, tbit;
    uint16_t x0=x;
	uint8_t mode;
	mode =0;
    
	static uint16_t color;

	for(t = 0; t < 354; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font_main_pic_num[num][t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80){
				
				color =BLACK; // WHITE;

			}
			else if(0 == mode){
				if(bc==0)
				  color = WHITE;//BLACK;
				else
				  color = BLACK;

			}
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 43){ //英文数字是中文宽度的一半 110/2=45
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  
  
	
}

#endif 
/**************************************************************************************************************
*
*Function Name:void TFT_display_char32_32_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color)
*Function: 显示16x16的汉字(不带背景颜色，镂空)
*		  显示规则：一行一行显示，首先显示第一行的前八位，
*		  然后显示后八位，显示完成之后显示第二行，
*Input Ref:address:图片数据地址
*			startX：X起始坐标
*			startY：Y起始坐标
*			color：字体显示颜色
*Return Ref:NO
*
**************************************************************************************************************/
void TFT_display_char32_32_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color)
{

	uint16_t column;
	uint8_t tm=0;
	uint8_t temp;
	uint16_t y = 0;
	for(column = 0; column < 128; column++) //2个字 1个字是16 
	{
		temp =* address;
	   for(tm = 8; tm > 0; tm--)//for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm, startY+ y ,color);
				 TFT_DrawPoint(startX+tm, startY+ y ,color);
			}
			
			temp >>= 1;
		}
		address++;
		temp =* address;
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+8, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
//		if(column>0 && column%2 == 0)//如果开启字体的高读会压缩到之前的一半
		y++;
		address++;
	}	

	

}

/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Chinese_Timer_23_23(uint16_t x,uint16_t y,uint8_t num)
	*Function: "定时时间"
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Chinese_Timer_23_23(uint16_t x,uint16_t y,uint8_t num)
{
    uint16_t temp, t, tbit;
    uint16_t x0=x;
   
	static uint16_t color;

	for(t = 0; t < 69; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font2323_chinese_timer_time[num][t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
		    if(temp & 0x80)	color = BLACK;//WHITE;
			else color = WHITE;
			
			

			
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 22){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}

/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Chinese_BeiJing_Time_23_23(uint16_t x,uint16_t y,uint8_t num)
	*Function: "北京时间"
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Chinese_BeiJing_Time_23_23(uint16_t x,uint16_t y,uint8_t num)
{
    uint16_t temp, t, tbit;
    uint16_t x0=x;
   
	static uint16_t color;

	for(t = 0; t < 69; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font2323_chinese_beijing_time[num][t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
		    if(temp & 0x80)	color = BLACK;//WHITE;
			else color = WHITE;
			
			

			
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 22){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}

/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Chinese_WorkTime_23_23(uint16_t x,uint16_t y,uint8_t num)
	*Function: "工作时间"
	*Input Ref: x, y ,bc-0 display ,bc=1 -> don't display 
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Chinese_WorkTime_23_23(uint16_t x,uint16_t y,uint8_t num)
{
    uint16_t temp, t, tbit;
    uint16_t x0=x;
   
	static uint16_t color;

	for(t = 0; t < 69; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font2323_chinese_works_time[num][t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
		    if(temp & 0x80)	color = BLACK;//WHITE;
			else color = WHITE;
			
			

			
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 22){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}

/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Temp_24_24_onBlack(uint16_t x,uint16_t y,uint8_t num)
	*Function:
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
#if NORMAL_DISPLAY
void TFT_Disp_Temp_Symbol_24_24(uint16_t x,uint16_t y)
{
    uint16_t temp, t, tbit,mode;
    uint16_t x0=x;
    mode =0;
	static uint16_t color;

	for(t = 0; t < 72; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font2424_temp_symbol[t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80)	color = WHITE;
			else if(0 == mode)	color = BLACK;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 24){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}
#else 
void TFT_Disp_Temp_Symbol_16_15(uint16_t x,uint16_t y)
{
    static uint8_t mode ;
	uint16_t temp, t, tbit;
    uint16_t x0=x;
  
	static uint16_t color;

	for(t = 0; t < 30; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font16x15_temp_symbol[t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			if(temp & 0x80)	color = BLACK;
			else if(0 == mode)	color = WHITE;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 16){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}

#endif 
/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Temp_24_24_onBlack(uint16_t x,uint16_t y,uint8_t num)
	*Function:
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
#if NORMAL_DISPLAY
void TFT_Disp_Humidity_Symbol_24_24(uint16_t x,uint16_t y)
{
    uint16_t temp, t, tbit,mode;
    uint16_t x0=x;
    mode =0;
	static uint16_t color;

	for(t = 0; t < 72; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font2424_humidity_symbol[t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80)	color = WHITE;
			else if(0 == mode)	color = BLACK;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 24){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}
#else  
void TFT_Disp_Humidity_Symbol_16_16(uint16_t x,uint16_t y)
{
    uint16_t temp, t, tbit,mode;
    uint16_t x0=x;
    mode =0;
	static uint16_t color;

	for(t = 0; t < 32; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font1616_humidity_symbol[t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80)	color = BLACK;//WHITE;
			else if(0 == mode)	color = WHITE;//BLACK;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 16){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}
#endif 
/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Temp_24_24_onBlack(uint16_t x,uint16_t y,uint8_t num)
	*Function:
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(uint16_t x,uint16_t y,uint8_t sel,uint8_t num)
{
    
	  
		uint8_t temp, t,tbit;
		uint8_t x0=x;
	   
		static uint16_t color;
		
	
		//for(t = 0; t < 144; t++)	/*遍历打印所有像素点到LCD */
		for(t = 0; t < 116; t++)
		{	
		
			//temp = font4848_works_time_value[num][t]; 
			temp = font_pic_time_value[num][t];
			
			
			for(tbit = 0; tbit < 8; tbit++) /* 打印一个像素点到液晶 */
			{	
				
				if(temp & 0x80){ //display of backgroud color is "black" picture of words is "black"->0x00, background is "white" ->0xff
					
					color =BLACK; // WHITE;
	
				}
				else if(sel==0){ //display of words of color is "white"
					
					  color = WHITE;//BLACK;
					
               }
			   else color = BLACK; //don't display words ,all color is backgroud black.
				
				TFT_DrawPoint(x, y,color);
				
				temp <<= 1; 		
				//y++; // 垂直扫描
				x++;//水平扫描
	
				if(x >= LCD_Width){
						pro_t.lcd_over_width_flag =1;
						return; /* 超区域了 */
	
				}
				
				if((x - x0) == 26){//
					x = x0;
					y++;
					
					if(y >= LCD_Height){
					pro_t.lcd_over_height_flag=1;
					return; 	/* 超区域了 */
	
					 }
	 
					break;
				}
			}	 
		}  

}
/****************************************************************************************************
	*
	*Function Name:void TFT_Disp_Pic_Warnign_Words(uint16_t x,uint16_t y,uint8_t sel,uint8_t num)
	*Function: display set up timer timing value how meny value 
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************************/
void TFT_Disp_Pic_Warnign_Words(uint16_t x,uint16_t y,uint8_t sel,uint8_t num)
{


	uint8_t temp, t,tbit;
	uint8_t x0=x;

	static uint16_t color;


	//for(t = 0; t < 144; t++)	/*遍历打印所有像素点到LCD */
	for(t = 0; t < 116; t++)
	{	

	temp = font_pic_warning_words[num][t];


	for(tbit = 0; tbit < 8; tbit++) /* 打印一个像素点到液晶 */
	{

	if(sel ==1){

	   color = BLACK;


	}
	else if(temp & 0x80){

		color =BLACK; // picture of words ,words is "black"->0x00, background is "white" ->0xff

	}
	else if(sel==0){ //words of color display 
		
	   //color = WHITE;//BLACK;  this is words display = 'white' 
	   color = YELLOW;  //chines is words display = "RED"
    }
	

	TFT_DrawPoint(x, y,color);

	temp <<= 1; 		
	//y++; // 垂直扫描
	x++;//水平扫描

	if(x >= LCD_Width){
	pro_t.lcd_over_width_flag =1;
	return; /* 超区域了 */

	}

	if((x - x0) == 26){//
		x = x0;
		y++;

		if(y >= LCD_Height){
		pro_t.lcd_over_height_flag=1;
		return; 	/* 超区域了 */

		}

		break;
		}
	  }	 
	}  


}

/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Time_Split_Symbol(void)
	*Function:
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Time_Split_Symbol(uint16_t x,uint16_t y,uint8_t dat)
{
    uint16_t temp, t, tbit,mode;
    uint16_t x0=x;
    mode =0;
	static uint16_t color;

	for(t = 0; t < 144; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		//temp = font4848_works_time_value[10][t]; 
		  temp = font_colon_symbol[0][t];
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80)	{
				if(dat==0)
				   color = WHITE;
				else
				   color = BLACK;

			}
			else if(0 == mode)	color = BLACK;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 24){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}


/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Chinese_Temp_36_18(uint16_t x,uint16_t y)
	*Function: "温度"
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Chinese_Temp_36_18(uint16_t x,uint16_t y)
{
    uint16_t temp, t, tbit,mode;
    uint16_t x0=x;
    mode =0;
	static uint16_t color;

	for(t = 0; t < 90; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font3918_chinese_temp[t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80)	color = BLACK;//WHITE;
			else if(0 == mode)	color = WHITE;//BLACK;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 36){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}


/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Chinese_Humidity_39_18(uint16_t x,uint16_t y,uint8_t num)
	*Function:
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Chinese_Humidity_39_18(uint16_t x,uint16_t y,uint8_t num)
{
    uint16_t temp, t, tbit,mode;
    uint16_t x0=x;
    mode =0;
	static uint16_t color;

	for(t = 0; t < 90; t++)	/*遍历打印所有像素点到LCD */
	{   
	
		temp = font39x18_chinese_humidity[t]; 
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			
			
			if(temp & 0x80)	color = BLACK;//WHITE;
			else if(0 == mode)	color = WHITE;//BLACK;
			else color = BLACK;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 39){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}


#if 0
void TFT_Disp_Fan_RunIcon(uint16_t x,uint16_t y)
{

	uint16_t temp, t, tbit;
    uint16_t x0=x;
   
	static uint16_t color;

	for(t = 0; t < 9360; t++)	/*遍历打印所有像素点到LCD */
	{   
	    
		  temp = fan_font_390_240[t]; 
		
		
		
		for(tbit = 0; tbit < 8; tbit++)	/* 打印一个像素点到液晶 */
		{	
			if(temp & 0x80)	color = BLACK;//WHITE; //words is "white",backgroud is "black"
			else color = WHITE;
			TFT_DrawPoint(x, y,color );
			
			temp <<= 1;			
			//y++; // 垂直扫描
			x++;//水平扫描

			if(x >= LCD_Width){
                    pro_t.lcd_over_width_flag =1;
					return;	/* 超区域了 */

			}
			
			if((x - x0) == 309){
				x = x0;
				y++;
				
			    if(y >= LCD_Height){
				pro_t.lcd_over_height_flag=1;
				return;		/* 超区域了 */

			     }
 
				break;
			}
		}  	 
	}  

}
#endif 