#include "bsp.h"


static void mode_longkey_settimer(void);

uint16_t mode_key_counter;

uint8_t ptc_fun_led_init_flg , plasma_fun_led_init_flag, rat_fun_led_init_flag;



/******************************************************************************
	*
	*Function Name:static void Key_Speical_Power_Fun_Handler(void)
	*Funcion: speical of power key function
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Key_Speical_Power_Fun_Handler(void)
{
	static uint8_t  power_on_off ;
	//be pressed long time key of function that link tencent cloud funtion 
   
	
	 while(pro_t.key_power_be_pressed_flag==1){

	   
         if(POWER_KEY_VALUE() ==KEY_DOWN &&  pro_t.gPower_On == power_on){
		 	  iwdg_feed();
              if(ptc_error_state()==0 && fan_error_state()==0){
         


		 if(pro_t.gTimer_pro_power_key_adjust >2){
			//power_key_counter=0;

			pro_t.key_power_be_pressed_flag =0;
			pro_t.gTimer_pro_wifi_led =0;
            pro_t.wifi_led_fast_blink_flag=1;
			wifi_t.gTimer_linking_tencent_duration=0;
			//WIFI CONNCETOR process
			wifi_t.esp8266_login_cloud_success =0;
			wifi_t.runCommand_order_lable=wifi_link_tencent_cloud;
			wifi_t.wifi_config_net_lable= wifi_set_restor;
			wifi_t.power_on_login_tencent_cloud_flag=0;
			wifi_t.link_tencent_step_counter=0;
			wifi_t.gTimer_linking_tencent_duration=0; //166s -2分7秒
			buzzer_sound();
			wifi_FastLed_Blink_Init();
			

		 }
			
			 
        }
	    }
        else if(POWER_KEY_VALUE() ==KEY_UP){
			
           pro_t.key_power_be_pressed_flag=0;
		  
            power_on_off = power_on_off ^ 0x01;

			if(pro_t.gPower_On == power_on) power_on_off =0;
			else power_on_off =1;

		  if(power_on_off==1){
		   // __disable_irq();
			pro_t.gPower_On = power_on;   
            pro_t.long_key_flag =0;
            pro_t.run_process_step=0;
			//pro_t.buzzer_sound_flag = 1;

			  LED_Mode_Key_On();
			  LED_Power_Key_On();
			  Power_On_Led_Init();
			  TFT_BACKLIGHT_ON();
			
			buzzer_sound();
			
			// __enable_fiq();
		   

		
			
		  }
		  else {
			 
			//__disable_irq();
			 pro_t.power_off_flag=1;
			 pro_t.long_key_flag =0;
			pro_t.run_process_step=0xff;
			pro_t.gPower_On = power_off;   
			// pro_t.buzzer_sound_flag = 1;
			    TFT_BACKLIGHT_OFF();
				LED_Mode_Key_Off();
			    LED_Power_Key_Off();
               
				
			buzzer_sound();
			
		  }
			  
			}
		 }
}
		
/******************************************************************************
	*
	*Function Name:static void Key_Speical_Mode_Fun_Handler(void)
	*Funcion: speical of mode key fun
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Key_Speical_Mode_Fun_Handler(void)
{

  static uint8_t mode_long_key ;
   while(pro_t.mode_key_pressed_flag==1){   // && fan_error_state() ==0 && ptc_error_state() ==0)){
   if(MODE_KEY_VALUE() ==KEY_DOWN){
      
              pro_t.mode_key_counter++;
							wifi_t.get_rx_beijing_time_enable=0;
              iwdg_feed();
              Wifi_Fast_Led_Blink();
			
			       
   	if((pro_t.gTimer_pro_mode_key_adjust>2 ||pro_t.mode_key_counter >2053503) && fan_error_state() ==0 && ptc_error_state() ==0 ){//if(mode_key_counter> 900000){
		
			gctl_t.mode_long_key_flag=1;
	    pro_t.gTimer_pro_set_long_key_tims=0;
		pro_t.mode_key_pressed_flag =0;
		pro_t.mode_key_longdoning = 1;
       

	    Wifi_Fast_Led_Blink();
	
		iwdg_feed();

		pro_t.gTimer_pro_mode_key_adjust=0;
		
	
		pro_t.mode_key_run_proc_item=mode_key_set_timer_value;
		pro_t.disp_works_timer_timing_mode_item=timer_set_time; //set timer mode enable,
	   gctl_t.gTimer_ctl_disp_works_time_second=0;


		TFT_Disp_Set_TimerTime_Init();
		 pro_t.gTimer_pro_set_long_key_tims=0;
		//pro_t.buzzer_sound_flag =1;
		mode_long_key =1;
		
		Buzzer_KeySound();
		break;
       }
			

   }
   else if(MODE_KEY_VALUE() ==KEY_UP &&  pro_t.mode_key_longdoning ==1){
	  mode_long_key ++ ;
	   pro_t.mode_key_longdoning =0;

      wifi_t.get_rx_beijing_time_enable=0;
      pro_t.mode_key_pressed_flag=0;
      pro_t.gTimer_pro_set_long_key_tims=0;
     
	  Wifi_Fast_Led_Blink();

	  break;


   }
   else if(MODE_KEY_VALUE() ==KEY_UP &&   pro_t.mode_key_longdoning ==0 ){
   	     //  HAL_Delay(10);
		 //  Wifi_Fast_Led_Blink();

      if(MODE_KEY_VALUE() ==KEY_UP){
        Wifi_Fast_Led_Blink();
		pro_t.mode_key_pressed_flag =0;
		gctl_t.mode_long_key_flag=0;
	    
		
		
        pro_t.gTimer_pro_mode_key_be_select=0;
		pro_t.mode_key_run_proc_item = mode_key_select;
	    iwdg_feed();
	     gctl_t.select_main_fun_numbers++; // 0,1,2
		 if(gctl_t.select_main_fun_numbers > 3){
			  gctl_t.select_main_fun_numbers = 1;
		 }
		
		gctl_t.memory_confimr_key_done = 1;
        pro_t.gTimer_pro_mode_key_be_select = 0; //counter starts after 4 seconds ,cancel this function
		gctl_t.gTimer_ctl_select_led =0;
		pro_t.buzzer_sound_flag =1;

		pro_t.gTimer_pro_set_long_key_tims =9;
	
		//Buzzer_KeySound();
      	}
	   	
	   	}
	 
	}

    if( pro_t.gTimer_pro_set_long_key_tims> 0 &&  mode_long_key ==1){

	     mode_long_key ++;
		 pro_t.mode_key_pressed_flag=0;
	     pro_t.mode_key_longdoning = 0;
      

	}

    
	  Mode_Key_Config_Fun_Handler();

      Wifi_Fast_Led_Blink();
   
		

}

/******************************************************************************
	*
	*Function Name:static void Mode_Key_Config_Fun_Handler(void)
	*Funcion: speical of mode key fun
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Mode_Key_Config_Fun_Handler(void)
{
    if(pro_t.key_works_timer_timing_state()==mode_key_select){
		Wifi_Fast_Led_Blink();

       if(pro_t.gTimer_pro_mode_key_be_select < 4){ //exit of rule

				Mode_Key_Select_Fun();
				
             }
			 else{
                gctl_t.memory_confimr_key_done = 0;
                pro_t.mode_key_run_proc_item = 0xff; //
         
              
			    gctl_t.select_main_fun_numbers--; //return back the first confirm item 
				if(gctl_t.select_main_fun_numbers == 0){
					gctl_t.select_main_fun_numbers = 5;
				}
			
				//ModeKey_Select_Default_LedOnOff();
                Device_Action_Led_OnOff_Handler();
			 }
             

		  

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
void Key_Interrup_Handler(void)
{

while( pro_t.gKey_value_add ==1){


       if(ADD_KEY_VALUE()==KEY_UP){
			pro_t.gKey_value_add=0;

		    Buzzer_KeySound();
			ADD_Key_Fun();

       }
		   
			
        
	 }

	 while(pro_t.gKey_value_dec ==1){

       if(DEC_KEY_VALUE()==KEY_UP){
           pro_t.gKey_value_dec=0;
		   Buzzer_KeySound();
			DEC_Key_Fun();
		    

       }
     }

}
/******************************************************************************
	*
	*Function Name:static void ModeKey_Select_Default_LedOnOff(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void ModeKey_Select_Default_LedOnOff(void)
{
    if(ptc_fun_led_init_flg > 0){
	    ptc_fun_led_init_flg=0;
		if(ptc_state()== 1){


		    LED_PTC_ICON_ON();

		}
		else{

		   LED_PTC_ICON_OFF();
		}

	}

	if(plasma_fun_led_init_flag > 0){
		plasma_fun_led_init_flag =0;
		if(plasma_state() == 1){
                  
			LED_KILL_ICON_ON();
		}
		else{
			//  Plasma_Off();
		   LED_KILL_ICON_OFF();

		}
	}

	if(rat_fun_led_init_flag > 0){
		rat_fun_led_init_flag =0;
		 if(ultrasonic_state()==1){
			LED_RAT_ICON_ON();

		}
		else{
    	LED_RAT_ICON_OFF();

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
void ADD_Key_Fun(void)
{

 static uint8_t disp_temp_value,add_timer;

 if(gctl_t.mode_long_key_flag ==1 && pro_t.gTimer_pro_set_long_key_tims< 3){
 
	  pro_t.mode_key_run_proc_item = mode_key_set_timer_value;
 
 
  }
 
  if(power_on_state()==power_on){

	if(gctl_t.ptc_warning ==0 && ptc_error_state() ==0){


        iwdg_feed();
		switch(pro_t.mode_key_run_proc_item){

		case 0xff:
            

		case mode_key_set_temp: //set temperature value add number
			     
			     if(gctl_t.mode_long_key_flag==0){
			 
					// pro_t.buzzer_sound_flag = 1;//Buzzer_KeySound();
					gctl_t.gSet_temperature_value ++;
					if( gctl_t.gSet_temperature_value < 20)gctl_t.gSet_temperature_value=20;
					
		            if(gctl_t.gSet_temperature_value > 40) gctl_t.gSet_temperature_value= 20;

		             pro_t.gTimer_pro_set_tem_value_blink =0;
					 gctl_t.gSet_temperature_value_item = set_temp_value_item;

		        
					disp_temp_value =1;

			    }
				else{
					 // pro_t.buzzer_sound_flag = 1;
				   // pro_t.gTimer_pro_mode_long_key=0;
				   pro_t.gTimer_pro_set_long_key_tims=0;
				
					
					gctl_t.gSet_timer_minutes=0;
					gctl_t.gSet_timer_hours ++ ;//disp_t.disp_timer_time_hours++ ;//pro_t.dispTime_minutes = pro_t.dispTime_minutes + 60;
					if(gctl_t.gSet_timer_hours  > 24){ //if(pro_t.dispTime_minutes > 59){

					gctl_t.gSet_timer_hours =0;//pro_t.dispTime_hours =0;


			}
			

		   pro_t.mode_key_run_proc_item = mode_key_set_timer_value;
		  
			
		    add_timer =1;



				}
				

		break;

		case mode_key_set_timer_value://3
		  
           // pro_t.buzzer_sound_flag = 1;
		   // pro_t.gTimer_pro_mode_long_key=0;
		   pro_t.gTimer_pro_set_long_key_tims=0;
		
			
			gctl_t.gSet_timer_minutes=0;
			gctl_t.gSet_timer_hours ++ ;//disp_t.disp_timer_time_hours++ ;//pro_t.dispTime_minutes = pro_t.dispTime_minutes + 60;
			if(gctl_t.gSet_timer_hours  > 24){ //if(pro_t.dispTime_minutes > 59){

			gctl_t.gSet_timer_hours =0;//pro_t.dispTime_hours =0;


			}
			

		   pro_t.mode_key_run_proc_item = mode_key_set_timer_value;
		  
			
		    add_timer =1;

		break;

		

		case mode_key_select:
			
			//pro_t.buzzer_sound_flag = 1;
	        pro_t.gTimer_pro_temp_delay=0;
		   
            Mode_Key_Confirm_Fun();
		    Device_Action_Led_OnOff_Handler();
		
			pro_t.mode_key_run_proc_item = mode_key_set_temp;
			
			 pro_t.gTimer_pro_set_long_key_tims=7;
		
			  pro_t.gTimer_pro_temp_delay=70; //at once run ptc of compare temperature value .
           
			// Buzzer_KeySound();
			
			
		
        break; 
		

		}	
	}
	
    if(disp_temp_value ==1){
		disp_temp_value =2;
		 pro_t.mode_key_run_proc_item = mode_key_set_temp;
		gctl_t.local_set_temp_value = 1;
    	TFT_Disp_Temp_SwitchSet_Value(0,gctl_t.gSet_temperature_value);
	    gctl_t.gTimer_ctl_publish_set_temperature_value = 0;
		if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

		    if(ptc_state()==0){
				gctl_t.ptc_flag =1;
				Ptc_On();
				LED_PTC_ICON_ON();
		    }

		}
		else{
		   if(ptc_state()==1){
			   gctl_t.ptc_flag =0;
			   Ptc_Off();
			   LED_PTC_ICON_OFF();

		   }


		}

    }
	if(add_timer==1){
		add_timer =0;
        mode_longkey_settimer();

	}
	}
   
}


/************************************************************************
	*
	*Function Name: static void DEC_Key_Fun(void)
	*Function : KEY OF decrease
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
void DEC_Key_Fun(void)
{
    static uint8_t disp_temp_value,dec_timer;
    
	if(gctl_t.mode_long_key_flag ==1 && pro_t.gTimer_pro_set_long_key_tims< 3){
	
		 pro_t.mode_key_run_proc_item = mode_key_set_timer_value;
	
	
	 }
	

    if(power_on_state() ==power_on){
		
	   	if(gctl_t.ptc_warning ==0 && ptc_error_state() ==0 ){
			iwdg_feed();
	   	
	     switch(pro_t.mode_key_run_proc_item ){

		   case 0xff:
           

		   case mode_key_set_temp:  //default tempearture value 
	  
	              if(gctl_t.mode_long_key_flag ==0){
			 	
		      //  pro_t.buzzer_sound_flag = 1;// Buzzer_KeySound();
				 gctl_t.gSet_temperature_value--;
				if( gctl_t.gSet_temperature_value<20)  gctl_t.gSet_temperature_value=40;
		        if( gctl_t.gSet_temperature_value >40) gctl_t.gSet_temperature_value=40;
	   
				 pro_t.gTimer_pro_set_tem_value_blink =0;
				 gctl_t.gSet_temperature_value_item = set_temp_value_item;

	             disp_temp_value =1;

	              }
				  else{

						   //  pro_t.buzzer_sound_flag = 1;
			          
			
					    pro_t.gTimer_pro_set_long_key_tims=0;
					
					
						gctl_t.gSet_timer_minutes=0;
						gctl_t.gSet_timer_hours --;//disp_t.disp_timer_time_hours -- ;//pro_t.dispTime_minutes = pro_t.dispTime_minutes - 1;
						if(gctl_t.gSet_timer_hours  < 0){//if(pro_t.dispTime_minutes < 0){

						    gctl_t.gSet_timer_hours  =24;//pro_t.dispTime_hours --;
							
							
						}
				
				     pro_t.mode_key_run_proc_item = mode_key_set_timer_value;
				       dec_timer =1;
				}
			        
			 
			        
			break;

			case mode_key_set_timer_value: //timer timing set "decrease -down"
			       
			  //  pro_t.buzzer_sound_flag = 1;
	          
	
			    pro_t.gTimer_pro_set_long_key_tims=0;
			
			
				gctl_t.gSet_timer_minutes=0;
				gctl_t.gSet_timer_hours --;//disp_t.disp_timer_time_hours -- ;//pro_t.dispTime_minutes = pro_t.dispTime_minutes - 1;
				if(gctl_t.gSet_timer_hours  < 0){//if(pro_t.dispTime_minutes < 0){

				    gctl_t.gSet_timer_hours  =24;//pro_t.dispTime_hours --;
					
					
				}
		
		     pro_t.mode_key_run_proc_item = mode_key_set_timer_value;
		       dec_timer =1;
				
				
			 	
			//TFT_Disp_Set_TimerTime(0);
			break;

			 case mode_key_select:
			 
			 //	pro_t.buzzer_sound_flag = 1;
		    //	 Buzzer_KeySound();
			
				Device_Action_Led_OnOff_Handler();
				Mode_Key_Confirm_Fun();

			
				pro_t.mode_key_run_proc_item = mode_key_set_temp;


				pro_t.gTimer_pro_set_long_key_tims=7;

				pro_t.gTimer_pro_temp_delay=100; //at once run ptc of compare temperature value .
			 

             break;


	    	}
	   	 }
		
		if(disp_temp_value ==1){
		    disp_temp_value =2;
			gctl_t.local_set_temp_value =1;
		    pro_t.mode_key_run_proc_item = mode_key_set_temp;
    		TFT_Disp_Temp_SwitchSet_Value(0,gctl_t.gSet_temperature_value);
		    gctl_t.gTimer_ctl_publish_set_temperature_value = 0;
		
			if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

		    	if(ptc_state()==0){
					gctl_t.ptc_flag =1;
					Ptc_On();
					LED_PTC_ICON_ON();
		    	}

			}
			else{
		   		if(ptc_state()==1){
                    gctl_t.ptc_flag = 0;
			   		Ptc_Off();
			   		LED_PTC_ICON_OFF();
				}
			}
			
    	}

		 if(dec_timer ==1){
		 	dec_timer=0;
	      mode_longkey_settimer();
		 }
          
  
       }
	   
}
       
/*****************************************************************************
 * 
 * Function Name: void Mode_Key_Select_Fun(void)
 * Function:   This function is used to select the working mode of the device.
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/
void Mode_Key_Select_Fun(void)
{
   switch(gctl_t.select_main_fun_numbers){

      case ptc_fun:

		
		ptc_fun_led_init_flg++;
		if(ptc_state()== 1){


		LED_PTC_ICON_OFF();//LED_PTC_ICON_ON();

		}
		else{

		LED_PTC_ICON_ON();//LED_PTC_ICON_OFF();


		}

		if(wifi_link_net_state()==1){

		LED_WIFI_ICON_ON();
		}
		else{

		LED_WIFI_ICON_OFF();

		}

   	      
   
        

  led_blik: if(plasma_state() == 1){
                   // Plasma_On();
				LED_KILL_ICON_ON();
			}
			else{
			//  Plasma_Off();
			LED_KILL_ICON_OFF();

			}

			if(ultrasonic_state()==1){

			// Ultrasonic_Pwm_Output();
			LED_RAT_ICON_ON();

			}
			else{

			// Ultrasonic_Pwm_Stop();
			LED_RAT_ICON_OFF();
		}
			
			
	    if(gctl_t.gTimer_ctl_select_led < 20){ //30x10ms=300ms
 		      LED_PTC_ICON_ON()  ;  
		  }
		  else if(gctl_t.gTimer_ctl_select_led > 19 && gctl_t.gTimer_ctl_select_led < 41){
 		     LED_PTC_ICON_OFF() ; 

		  }
		  else{
		  	gctl_t.gTimer_ctl_select_led=0;
			if(pro_t.mode_key_run_proc_item==mode_key_select)
			   goto led_blik;
			

		  }

        
		

      break;

	  case plasma_fun:
	  	//KILL ICON LED

		 plasma_fun_led_init_flag ++;

		 if(plasma_state() == 1){
                   // Plasma_On();
			LED_KILL_ICON_OFF();//LED_KILL_ICON_ON();
		}
		else{
			//  Plasma_Off();
		   LED_KILL_ICON_ON();//LED_KILL_ICON_OFF();

		}

		
	 if(wifi_link_net_state()==1){

	 LED_WIFI_ICON_ON();
	 }
	 else{

	 LED_WIFI_ICON_OFF();

	 }


		
	 led_blik2:   if(ptc_state()== 1){

	
		LED_PTC_ICON_ON();

		}
		else{
	
		LED_PTC_ICON_OFF();


		}

		if(ultrasonic_state()==1){

	
		LED_RAT_ICON_ON();

		}
		else{

		
		LED_RAT_ICON_OFF();

		}

     if(gctl_t.gTimer_ctl_select_led < 20){ //30x10ms=300ms

		    
				LED_KILL_ICON_ON() ;   
         	 }
			 else if(gctl_t.gTimer_ctl_select_led > 19 && gctl_t.gTimer_ctl_select_led < 41){
				LED_KILL_ICON_OFF() ;
			 }
			 else{
			 	gctl_t.gTimer_ctl_select_led=0;
				if(pro_t.mode_key_run_proc_item==mode_key_select)
				goto led_blik2;

			 }
	  	
		

	  break;

	  case rat_fun:

	  //ULTRSONIC ICO LED
	  rat_fun_led_init_flag++;

	  if(ultrasonic_state()==1){

	
		LED_RAT_ICON_OFF();//LED_RAT_ICON_ON();

		}
		else{
			
			LED_RAT_ICON_ON();//LED_RAT_ICON_OFF();

		}

		
      if(wifi_link_net_state()==1){

		LED_WIFI_ICON_ON();
	  }
	  else{

		LED_WIFI_ICON_OFF();

	  }
	 

	led_blink3:   if(ptc_state()== 1){
	
	   
		LED_PTC_ICON_ON();
	
	 }
	 else{
	   
	   LED_PTC_ICON_OFF();
	}
	
	if(plasma_state() == 1){
		
		  LED_KILL_ICON_ON();
	  }
	  else{
	   
		 LED_KILL_ICON_OFF();
	
	  }
	
	 


	if(gctl_t.gTimer_ctl_select_led < 20){ //30x10ms=300ms
			LED_RAT_ICON_ON(); 
	 }
	 else if(gctl_t.gTimer_ctl_select_led > 19 && gctl_t.gTimer_ctl_select_led < 41){	
		   LED_RAT_ICON_OFF();
	  }
	  else{
		   gctl_t.gTimer_ctl_select_led=0;
		   if(pro_t.mode_key_run_proc_item==mode_key_select)
		   goto led_blink3;

		}

	   
	  break;
	  	
	  	

   }

}

/**************************************************************************
 * 
 * Function Name: void Mode_Key_Confirm_Fun(void)
 * Function : "+" and "-" of key as confirm of key be used to 
 * Input Ref:NO
 * Return Ref:NO
 * 
**************************************************************************/
void Mode_Key_Confirm_Fun(void)
{
   switch(gctl_t.select_main_fun_numbers){

      case ptc_fun:
           if(gctl_t.memory_confimr_key_done ==1){
            if(ptc_state() == 0){
				LED_PTC_ICON_ON(); 
			    Ptc_On();
				gctl_t.ptc_flag = 1;
				pro_t.add_or_dec_is_cofirm_key_flag =0;
				
		    }
			else{
				pro_t.add_or_dec_is_cofirm_key_flag =1;
				LED_PTC_ICON_OFF() ;
				Ptc_Off();
				gctl_t.ptc_flag = 0;
				 
				
            }

			gctl_t.select_main_fun_numbers--;
			if(gctl_t.select_main_fun_numbers==0)gctl_t.select_main_fun_numbers=4;
		    gctl_t.memory_confimr_key_done = 0;
          }
	  break;

	  case plasma_fun:
	  	   if(gctl_t.memory_confimr_key_done ==1){

		 if(plasma_state() == 0){
			gctl_t.plasma_flag=1;
			LED_KILL_ICON_ON() ;
			Plasma_On();
		}
     	else{
		  gctl_t.plasma_flag=0;
		  LED_KILL_ICON_OFF() ;
		  Plasma_Off();
		}
		gctl_t.select_main_fun_numbers--;
			if(gctl_t.select_main_fun_numbers==0)gctl_t.select_main_fun_numbers=5;
		gctl_t.memory_confimr_key_done = 0;
	  	   }
	  break;

	  case rat_fun: //ball cat-black
       if(gctl_t.memory_confimr_key_done ==1){
	   if(ultrasonic_state() ==0){ //30x10ms=300ms
	   	    gctl_t.ultrasonic_flag=1;
			LED_RAT_ICON_ON(); 
		    Ultrasonic_Pwm_Output();
			
			
	   	}
		else{	
		   gctl_t.ultrasonic_flag=0;
		   LED_RAT_ICON_OFF();
		   Ultrasonic_Pwm_Stop();
		  
				
		}
	  gctl_t.select_main_fun_numbers--;
	  if(gctl_t.select_main_fun_numbers==0)gctl_t.select_main_fun_numbers=6;
	   gctl_t.memory_confimr_key_done = 0;
       }
	   break;

	 }
   
}


static void mode_longkey_settimer(void)
{

   TFT_Disp_Onley_Set_TimerTime_Value();
       

}



