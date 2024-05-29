#include "bsp_led.h"
#include "bsp.h"




static void Delay(int16_t count);

volatile uint32_t led_k,led_i;


void LED_Power_Key_On(void)
{
    LED_POWER_KEY_SetLow() ;


}
void LED_Power_Key_Off(void)
{
	LED_POWER_KEY_SetHigh() ;
}

//mos 
void LED_Mode_Key_On(void)
{
  LED_MODE_KEY_SetHigh();	

}
void LED_Mode_Key_Off(void)
{

	LED_MODE_KEY_SetLow();


}
//LED_NET

void LED_Net_On(void)
{
   LED_WIFI_ICON_ON();

}
void LED_Net_Off(void)
{
	LED_WIFI_ICON_OFF() ;
	
}




static void Delay(int16_t count)
{
   
    if(count ==0 || count <0){
       return;
    
    }
    else{
    while(count){

       count--;
   }
   }

}


void Power_On_Led_Init(void)
{

  
	//WIFI ICON LED
	LED_WIFI_ICON_ON()  ;             

	//PTC ICON LED
	LED_PTC_ICON_ON() ;               


	//KILL ICON LED
	LED_KILL_ICON_ON()  ;             


	//ULTRSONIC ICO LED
	LED_RAT_ICON_ON()  ;           


}

void Power_Off_Led(void)
{

//WIFI ICON LED
	LED_WIFI_ICON_OFF()	;			  

	//PTC ICON LED
	LED_PTC_ICON_OFF() ; 			  


	//KILL ICON LED
	LED_KILL_ICON_OFF()	;			  


	//ULTRSONIC ICO LED
	LED_RAT_ICON_OFF()  ;		   




}

/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*
*
**************************************************************/
void Breath_Led(void)
{
   {
    static uint32_t i,j,n,m;
    led_k++;

	if(led_k<2001){
        i=0;
        j=0;
	    n=0;
		m++;
		LED_Power_Key_On();
		Delay(m++);
	   LED_Power_Key_Off();
	   Delay(8000-m);

    }
    if(led_k>1999 && led_k <4001){
        j++;
		LED_Power_Key_Off();
		Delay(1000+j);
        LED_Power_Key_On();
        Delay(4000-j);
		
        

    }
    else if(led_k>3999 && led_k <6001){
		n++;

	   LED_Power_Key_On();
	   Delay(1000+n); //WT.EDIT 2024.04.28
	
       LED_Power_Key_Off();
	   Delay(8000-n);
      

	}
    else if(led_k>5999){
//        led_k = 0;//led_k =30000;
//        led_i=0;
//		led_k=0;
//	    m=0;
//		i=0;
		
       i++; 
      if(i<4000){
	  	  if(i< 100){
	  	  	LED_Power_Key_On();
	      	Delay(100-i); //WT.EDIT 2024.04.28
	  	  }
          LED_Power_Key_Off();
		  Delay(1000+i); //WT.EDIT 2024.04.28
      }
      else{
	  	led_k = 0;
        led_i=0;
		led_k=0;
	    m=0;
        
        
      }
	}
	

}
	

}




