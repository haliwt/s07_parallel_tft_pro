#include "bsp.h"

PRO_T pro_t;

uint8_t led_blink_times;
uint8_t update_step;
uint8_t fan_continuce_run_flag;


//static void TFT_Pocess_Command_Handler(void);

static void Power_On_Fun(void);
static void Power_Off_Fun(void);
//static void Key_Interrup_Handler(void);
void bsp_Init(void);





/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
   pro_t.buzzer_sound_flag=1;

   pro_t.mode_key_run_proc_item=0xff;
   wifi_t.gTimer_wifi_pub_power_off=60;
   VOICE_OUTPUT_SOUND_ENABLE();

   TimerTiming_Init();

 
  
}
/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{

	if(pro_t.power_on_first ==0){
      
	 
	
	    Update_DHT11_Value();
	   
        TFT_Display_Handler();
		 pro_t.power_on_first =1;
		 
		

	}
	
	/* --- 喂狗 */
      iwdg_feed();

    

    TFT_Disp_Timer_Split_Symbol();
    Wifi_Fast_Led_Blink();
	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();
	if(pro_t.gPower_On == power_on && pro_t.gTimer_pro_update_tft_icon > 16){
	  pro_t.gTimer_pro_update_tft_icon =0;
	   TFT_Display_Update_Handler();

	}

	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
}
/*
*********************************************************************************************************
*
*	函 数 名: void TFT_Process_Handler(void)
*	功能说明: TFT显示的内容		 
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
void TFT_Process_Handler(void)
{
   
   static uint8_t fan_continuce_flag,power_off_wifi_login;
	if(pro_t.buzzer_sound_flag ==1 && pro_t.power_on_first==1){
		pro_t.buzzer_sound_flag=0;
		Buzzer_KeySound();
	}
	


	switch(pro_t.gPower_On){
	
	case power_on:

	  
	  TFT_Pocess_Command_Handler();
		
	break;

	case power_off:

	if(pro_t.power_off_flag == 1){
		pro_t.power_off_flag =0;
	    wifi_t.power_off_step=1; 
	    wifi_t.gTimer_wifi_pub_power_off=0;	
		
		wifi_t.repeat_login_tencent_cloud_init_ref=0;
	    fan_continuce_flag =1;
		power_off_wifi_login=0;
	
		wifi_t.power_on_thefirst_times=0;
		pro_t.gTimer_pro_fan =0;
		wifi_t.gTimer_wifi_pub_power_off=0;	
		wifi_t.gTimer_wifi_sub_power_off=0;
		gctl_t.gTimer_ctl_disp_works_time_second=0;
		pro_t.run_process_step=0;
		wifi_t.power_on_linkwifi=0;
		wifi_t.runCommand_order_lable= 0xff;
       // TFT_BACKLIGHT_OFF();
	
		Power_Off_Fun();
		Device_NoAction_Power_Off();
		Power_Off_Led();
		
	   wifi_t.three_times_link_beijing=0;
		wifi_t.smartphone_app_power_on_flag=0;
		
	}
	if(wifi_link_net_state() ==1 && wifi_t.gTimer_wifi_pub_power_off >2  && power_off_wifi_login==0 ){
		wifi_t.gTimer_wifi_pub_power_off=0;
		power_off_wifi_login++;
		MqttData_Publish_PowerOff_Ref();
		wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	     
		 
		  
	}
	if(wifi_link_net_state() ==1  && wifi_t.gTimer_wifi_sub_power_off > 4 && power_off_wifi_login==1){
		
        power_off_wifi_login++;
		wifi_t.gTimer_wifi_sub_power_off=0;
        Subscriber_Data_FromCloud_Handler();
	  
	
    }
    wifi_t.receive_beijing_time=0;
    wifi_t.gTimer_get_beijing_time=0;
	if(fan_continuce_flag ==1){

	    if(pro_t.gTimer_pro_fan <61){
            Fan_Run(); 
			

		}
		else{
			fan_continuce_flag++;

            Fan_Stop();
		    
		    
		}
      

	}


	Power_Off_Retain_Beijing_Time();
	if(wifi_t.power_on_thefirst_times>0)
		wifi_t.power_on_thefirst_times=0;
	if(wifi_t.wifi_config_net_lable >0)
		wifi_t.wifi_config_net_lable =0;
	if(wifi_t.smartphone_app_power_on_flag==1)
   		 wifi_t.smartphone_app_power_on_flag=0; //手机定时关机和开机，设置参数的标志位
	
	Breath_Led();
	
	break;


	}
	
}




/******************************************************************************
	*
	*Function Name:void TFT_Pocess_Command_Handler(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void TFT_Pocess_Command_Handler(void)
{
  
  
	if(power_on_state() == power_on){
  
    switch(pro_t.run_process_step){


	 case 0:
	  
	    if(wifi_link_net_state() ==1){
		    
		    TFT_Display_WorksTime_Voice();
	    }
		else{
           TFT_Display_PowerOn_WorksTime_Init();
		}
	    
		Power_On_Fun();
	    Fan_Run();
		Device_Action_No_Wifi_Power_On_Handler();

		//TFT_BACKLIGHT_ON();


		pro_t.run_process_step=pro_disp_dht11_value;
		pro_t.gTimer_pro_ptc_delay_time=0;
		pro_t.gTimer_pro_update_dht11_data=30;
		pro_t.gTimer_pro_display_dht11_temp=30; //at once display dht11 value
		pro_t.gTimer_pro_display_dht11_hum=40;
		gctl_t.gTimer_ctl_dma_state =0;
		pro_t.gTimer_pro_action_publis=0;
		wifi_t.gTimer_get_beijing_time=0;
		wifi_t.receive_beijing_time=0;
		wifi_t.three_times_link_beijing=0;
		wifi_t.get_rx_beijing_time_enable=0;
		

       
		
	 break;

	 case pro_disp_dht11_value: //1 //display works time + "temperature value " + "humidity value"

	   Wifi_Fast_Led_Blink();

	  
	   if(pro_t.gTimer_pro_update_dht11_data > 8){

		   pro_t.gTimer_pro_update_dht11_data= 0;

		    Update_DHT11_Value();


	   }

	   if(pro_t.gTimer_pro_display_dht11_temp > 10 &&  pro_t.wifi_led_fast_blink_flag==0){
	   	   pro_t.gTimer_pro_display_dht11_temp=0;

	        
         
		    if( gctl_t.gSet_temperature_value_item != set_temp_value_item){
			  // TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
			   TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);
			  
            }
			
	        if(v_t.voice_set_temperature_value_flag==2){
				v_t.voice_set_temperature_value_flag++;

               TFT_Disp_Voice_Temp_Value(0,gctl_t.dht11_temp_value);


			}
			

	   }

	   if(pro_t.gTimer_pro_display_dht11_hum > 16 &&  pro_t.wifi_led_fast_blink_flag==0 ){

		   pro_t.gTimer_pro_display_dht11_hum=0;

           TFT_Disp_Only_Humidity_Numbers(gctl_t.dht11_hum_value);
	   
		  // TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);
		}

	 


	   if(pro_t.gTimer_pro_wifi_dht11_temp_hum > 38 && wifi_link_net_state() ==1){
		   pro_t.gTimer_pro_wifi_dht11_temp_hum=0;

			Update_Dht11_Totencent_Value();

	   }

	   Display_Precise_Works_Time();

	 pro_t.run_process_step=pro_run_main_fun;
	   
	case pro_run_main_fun: //02
	
   
	  Wifi_Fast_Led_Blink();

	   Fan_Pro_Handler();
	  Display_Precise_Works_Time();
	   pro_t.run_process_step=pro_disp_works_time;
	 break;

	 case pro_disp_works_time: //display works times and timer timing .

		Wifi_Fast_Led_Blink();
	
	
	    TimeTimer_Pro_Handler();
			

		pro_t.run_process_step=pro_set_temperature;


    case pro_set_temperature:

       Wifi_Fast_Led_Blink();
	   Temperature_Ptc_Pro_Handler();
		
        Display_Precise_Works_Time();
      pro_t.run_process_step=pro_disp_wifi_led;

	break;

	case pro_disp_wifi_led: //4
	      Wifi_Fast_Led_Blink();
	
		  if(wifi_link_net_state() ==0){
	
             switch(pro_t.wifi_led_fast_blink_flag){

			 case 1:
				Wifi_Fast_Led_Blink();

			 	pro_t.run_process_step=1;
             break;

             case 0: //slowly of blink led 

			    if(pro_t.mode_key_run_proc_item != mode_key_select){
                
				if(pro_t.gTimer_pro_wifi_led > 1 && pro_t.gTimer_pro_wifi_led < 3){
					
					LED_WIFI_ICON_ON();
				}
				else if(pro_t.gTimer_pro_wifi_led > 2){

					pro_t.gTimer_pro_wifi_led=0;
					LED_WIFI_ICON_OFF();
				}
			    }

				ModeKey_Select_Default_LedOnOff();
			 break;

            }
		}
		else{

		  LED_WIFI_ICON_ON();
		  ModeKey_Select_Default_LedOnOff();

		}

	  if(wifi_link_net_state()==1){

		  if(pro_t.gTimer_pro_action_publis_main_fun > 3 ){
		    	pro_t.gTimer_pro_action_publis_main_fun=0;
		
		      	Device_Action_Publish_Handler();
		  }
        }

	   Display_Precise_Works_Time();
	  pro_t.run_process_step=pro_wifi_publish_init;
	 break; 
		  
      // handler of wifi 
	  case pro_wifi_publish_init: //7
		Wifi_Fast_Led_Blink();
	   if(wifi_t.linking_tencent_cloud_doing ==0){
	   if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.power_on_thefirst_times==0){
             wifi_t.power_on_thefirst_times++;
		     pro_t.gTimer_pro_action_publis=0;
		     MqttData_Publish_SetOpen(0x01);
		     HAL_Delay(350);

		}
		if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.power_on_thefirst_times==1 &&  pro_t.gTimer_pro_action_publis >2){
             wifi_t.power_on_thefirst_times++;
		     pro_t.gTimer_pro_action_publis=0;
		    MqttData_Publish_Update_Data();
		     HAL_Delay(350);

		}
	   	}
	    Display_Precise_Works_Time();
	     pro_t.run_process_step=pro_wifi_subscriber_init;

	 break;

	  case pro_wifi_subscriber_init:
		Wifi_Fast_Led_Blink();
	   if(wifi_t.linking_tencent_cloud_doing ==0){
	   if(wifi_link_net_state()==1 && wifi_t.power_on_thefirst_times==2 && pro_t.gTimer_pro_pub_set_timer > 4){
	   	 
	   	 pro_t.gTimer_pro_pub_set_timer=0;
		 wifi_t.power_on_thefirst_times++;

	     Subscriber_Data_FromCloud_Handler();
	     HAL_Delay(350);

	   }
	   if(wifi_t.power_on_thefirst_times==3){
	     	wifi_t.power_on_thefirst_times++;
		  	wifi_t.linking_tencent_cloud_doing=0;
			wifi_t.has_been_login_flag = 1;
			
			
			wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;//wifi_tencent_subscription_data;
			wifi_t.gTimer_auto_detected_net_state_times=0;
			wifi_t.gTimer_get_beijing_time =0;
       }
	   }

	    Display_Precise_Works_Time();
		pro_t.run_process_step=pro_disp_dht11_value;

	  break;

	  


    default:
    break;
   	}
   }
  
   
}
/************************************************************************
	*
	*Function Name: static void Power_On_Fun(void)
	*Function : power on
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
static void Power_On_Fun(void)
{
  //led on 
  
  //smart phone control power on 
  if(wifi_t.smartphone_app_power_on_flag==0){
	  gctl_t.ptc_flag = 1;
      gctl_t.plasma_flag = 1;
	  gctl_t.ultrasonic_flag =1;
	  gctl_t.gSet_temperature_value =40;
  }
  else{
      if(gctl_t.ptc_flag ==0){ //don't auto turn off heater plate.
      	  pro_t.add_or_dec_is_cofirm_key_flag =1;
		 
      }
	   LED_Mode_Key_On();
	   LED_Power_Key_On();
	   Power_On_Led_Init();
	   TFT_BACKLIGHT_ON();
	  
 }

   //timer timing
   gctl_t.mode_flag = works_time;
  
    pro_t.disp_works_timer_timing_mode_item=works_time;
	 gctl_t.gSet_timer_hours =0;
	 gctl_t.gSet_timer_minutes =0;

	 //mode key long times 
	  pro_t.mode_key_run_proc_item=0xff;
	 pro_t.long_key_flag =0;
	//works time
	gctl_t.gTimer_ctl_total_continue_time =0; //works total is two hours recoder.

    pro_t.gTimer_pro_display_dht11_temp = 30; //powe on display sensoe dht11 of value .
    //display beijing times 
    wifi_t.link_beijing_times_flag=0; //from initiatil value =0; //WT.2024.04.28
    
    if(wifi_link_net_state()==0){
		 gctl_t.disp_works_hours =0;
	     gctl_t.disp_works_minutes=0;
	     gctl_t.gTimer_ctl_disp_works_time_second=0;

   }
   else if(wifi_link_net_state()==1 && gctl_t.beijing_time_flag ==0){

	   gctl_t.disp_works_hours =0;
	   gctl_t.disp_works_minutes=0;
	   gctl_t.gTimer_ctl_disp_works_time_second=0;


   }

 }
     


static void Power_Off_Fun(void)
{

	if(wifi_t.smartphone_app_power_on_flag==1){
    TFT_BACKLIGHT_OFF();
	LED_Mode_Key_Off();
	LED_Power_Key_Off();

	}

	Power_Off_Led();
   gctl_t.mode_flag = 0;
   pro_t.gPower_On = power_off;
   pro_t.mode_key_run_proc_item=0xff;
   gctl_t.plasma_flag = 0;
   gctl_t.ultrasonic_flag =0;
   gctl_t.ptc_flag = 0;
   //timer timing flag
    pro_t.wifi_led_fast_blink_flag=0;
	gctl_t.gSet_timer_hours =0;



	if(wifi_link_net_state()==0){
		 gctl_t.disp_works_hours =0;
	     gctl_t.disp_works_minutes=0;
	     gctl_t.gTimer_ctl_disp_works_time_second=0;

   }
   else if(wifi_link_net_state()==1 && gctl_t.beijing_time_flag ==0){

	   gctl_t.disp_works_hours =0;
	   gctl_t.disp_works_minutes=0;
	   gctl_t.gTimer_ctl_disp_works_time_second=0;


   }
   if(gctl_t.ptc_warning== ptc_warning){

         gctl_t.ptc_warning=0;
	   display_high_temp_words(1); //do not display ptc error words 
      
		
   }
   else{

      gctl_t.ptc_warning=0;
   


   }

   if(ptc_error_state() == 1){

     
	 gctl_t.fan_warning =0;
   	
     Display_Fan_Notworking_Words(1); //do not display  fan error words

   }
   else{

     
        gctl_t.fan_warning =0;
   }
   
	
	
}
void power_off_fan_run(void)
{
	
    LED_Mode_Key_Off();
	Breath_Led();
	

}





/**********************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void Wifi_Fast_Led_Blink(void)
{
   if(pro_t.wifi_led_fast_blink_flag==1 && wifi_link_net_state()==0){
wifi_led: if(pro_t.gTimer_pro_wifi_led < 166){//2'06s

	if( pro_t.gTimer_pro_wifi_fast_led < 80 ){ //50ms

	         LED_WIFI_ICON_ON();
	}
	else if(pro_t.gTimer_pro_wifi_fast_led > 79 && pro_t.gTimer_pro_wifi_fast_led< 161){

        
		LED_WIFI_ICON_OFF();
	}
	else{

		pro_t.gTimer_pro_wifi_fast_led=0;
		goto wifi_led;

		
	  }
   }
   else{
	
	 pro_t.wifi_led_fast_blink_flag=0;
   }

  }
  
}

void wifi_FastLed_Blink_Init(void)
{
   uint8_t i;
   for(i=0;i< 10;i++){
   LED_WIFI_ICON_OFF();
   HAL_Delay(100);
   LED_WIFI_ICON_ON();
    HAL_Delay(100);

   }



}


