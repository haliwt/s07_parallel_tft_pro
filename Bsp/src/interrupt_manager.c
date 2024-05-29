#include "interrupt_manager.h"
#include "bsp.h"

uint8_t voice_inputBuf[1];


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void USART_Cmd_Error_Handler(void)
{
    uint32_t temp;   
	HAL_DMA_StateTypeDef flag_dma1_tx_state;
	   DMA_HandleTypeDef hdma_spi1_tx;
	   DMA_HandleTypeDef hdma_spi1_rx;
	  
       if(gctl_t.gTimer_ctl_usart1_error > 15 ){ //9
			
			gctl_t.gTimer_ctl_usart1_error=0;
			  __HAL_UART_CLEAR_OREFLAG(&huart2);
	        
	          temp = USART2->DR;
	     
	         HAL_UART_Receive_IT(&huart2,voice_inputBuf,1);//UART receive data interrupt 1 byte
			 // UART_Start_Receive_IT(&huart1,inputBuf,1);
			
	  }

	 if(gctl_t.gTimer_ctl_usart2_error >4){
	  	gctl_t.gTimer_ctl_usart2_error=0;

           __HAL_UART_CLEAR_OREFLAG(&huart1);
	 

        

          temp = USART1->DR;
		 
		    HAL_UART_Receive_IT(&huart1,wifi_t.usart1_dataBuf,1);
		
     }

	 if(gctl_t.gTimer_ctl_dma_state >34){
           gctl_t.gTimer_ctl_dma_state =0;
		   flag_dma1_tx_state = HAL_DMA_GetState(&hdma_spi1_tx);
	      // flag_dma1_rx_state = HAL_DMA_GetState(&hdma_spi1_rx);
	 
		   if(flag_dma1_tx_state ==HAL_DMA_STATE_ERROR){
		   	   TFT_BACKLIGHT_OFF();
  			   TFT_LCD_Init();
		   	   Update_DHT11_Value();
		       TFT_Display_Handler();
			   TFT_BACKLIGHT_ON();
		   }
	 }
}
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t state_uart1;
    
   
    //wifi usart1 --wifi 
    if(huart->Instance==USART1)
    {
           
	  if(wifi_t.linking_tencent_cloud_doing  ==1){ //link tencent netware of URL

			wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
			wifi_t.wifi_uart_counter++;

			if(*wifi_t.usart1_dataBuf==0X0A) // 0x0A = "\n"
			{
				//wifi_t.usart2_rx_flag = 1;
				Wifi_Rx_Link_Net_InputInfo_Handler();
				wifi_t.wifi_uart_counter=0;
			}

	      } 
		  else{

		         if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
					wifi_t.wifi_uart_counter++;
				}
				else if(wifi_t.get_rx_auto_repeat_net_enable ==1){

					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart1_dataBuf[0];
					wifi_t.wifi_uart_counter++;

					if(*wifi_t.usart1_dataBuf==0X0A) // 0x0A = "\n"
					{
						
						Wifi_Rx_Auto_Link_Net_Handler();
						wifi_t.wifi_uart_counter=0;
					}


				}
				else{
					Subscribe_Rx_Interrupt_Handler();

				}
	      }
	 
	  
//	__HAL_UART_CLEAR_NEFLAG(&huart1);
//	__HAL_UART_CLEAR_FEFLAG(&huart1);
	__HAL_UART_CLEAR_OREFLAG(&huart1);
//	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	//__HAL_UART_CLEAR_TXFECF(&huart2);
	 HAL_UART_Receive_IT(&huart1,wifi_t.usart1_dataBuf,1);
     
	}


  if(huart->Instance==USART2){

		switch(state_uart1)
		{
		case 0:  //#0
			if(voice_inputBuf[0]==0xA5){  //hex :4D - "M" -fixed mainboard
				state_uart1=1; //=1
              
			}
			else{
				state_uart1=0; //=1


			}
			break;
		case 1: //#1
			if(voice_inputBuf[0]==0xFA) //hex : 41 -'A'  -fixed master
			{
				state_uart1=2; 
			}
			else
				state_uart1=0; 
			break;

	   case 2:
	      if(voice_inputBuf[0]==0) //hex : 41 -'A'	-fixed master
		   {
			   state_uart1=3; 
		   }
		   else{
			  state_uart1=0; 
		   }


	   break;

	   case 3:
	      if(voice_inputBuf[0]==0x81) //hex : 41 -'A'	-fixed master
		   {
			  
			   state_uart1=4; 
		   }
		   else{
			  state_uart1=0; 
			 }


	   break;

	   case 4:

	    if(voice_inputBuf[0]==0x01){
		 	 
			 v_t.voice_wakeword_enable=1;
			 v_t.RxBuf[0]= voice_inputBuf[0];
			 v_t.gTimer_voice_time_counter_start =0;
		      state_uart1=0;
		    
		 }
		 else if(v_t.voice_wakeword_enable==1){
		  
	      if(voice_inputBuf[0] >0 && voice_inputBuf[0] < 0x40) //指令Command 
		  {
              v_t.RxBuf[0]=voice_inputBuf[0]; //voice data4 + data6
			 
			   state_uart1=5;


		  }
	      }
		  else{

		   state_uart1=0; 
		

		
		  }
		 

	  break;

	   case 5:
	    
	   if(voice_inputBuf[0]==0x00) //hex : 41 -'A' -fixed master
		{
		  state_uart1=6; 
		}
		else
			state_uart1=0; 
	   

	   break;

	   case 6:
       if(v_t.voice_wakeword_enable ==1){

		 
	      if(voice_inputBuf[0] >0x21 && voice_inputBuf[0] < 0x60) //hex : 41 -'A'	-fixed master
		  {

			   v_t.RxBuf[1]=voice_inputBuf[0];
			   v_t.gTimer_voice_time_counter_start =0;
			   v_t.voice_power_on_cmd = 1;
			   state_uart1=0; 
		   }
	     }
		 else{
           // VOICE_OUTPUT_SOUND_DISABLE();
			 state_uart1=0; 
			 

		 }
		
	   break;

	   case 7:
	   	 
	     if(voice_inputBuf[0]==0xFB) //hex : 41 -'A'	-fixed master
		  {
            
             
			   state_uart1=0; 

		  }
		  else{

			  state_uart1=0; 

		  }
		  
	   
		 
     break;

	  case 8:
	      if(voice_inputBuf[0]==0xFB) //hex : 41 -'A'	-fixed master
		   {
			state_uart1=0; 

		  }
		   state_uart1=0; 
		break;
		  
	  case 9:
	      if(voice_inputBuf[0]==0xFB) //hex : 41 -'A'	-fixed master
		   {
			
			state_uart1=0; 

		  }
		   state_uart1=0; 
		break;

	 
	  }

	HAL_UART_Receive_IT(&huart2,voice_inputBuf,1);//UART receive data interrupt 1 byte

	}
	
}

  
 

//	__HAL_UART_CLEAR_NEFLAG(&huart1);
//	__HAL_UART_CLEAR_FEFLAG(&huart1);
//	__HAL_UART_CLEAR_OREFLAG(&huart1);
//	__HAL_UART_CLEAR_TXFECF(&huart1);

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timer timing 1ms 
	*           sys_clk = 64MHz
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t tm0;
  static uint8_t tm1,tm2;
 

    
   if(htim->Instance==TIM10){
    
    tm0++;  //1ms
    tm1++;
   
    pro_t.gTimer_pro_wifi_fast_led++;

	if(tm1 > 9){
		tm1=0;
		gctl_t.gTimer_ctl_select_led++;
	    pro_t.gTimer_pro_detect_key_ms =1;
	}
	
	
	if(tm0>999){ //1000 *1ms = 1000ms = 1s
		tm0=0;
		tm2++;


	
	 pro_t.gTimer_pro_temp_delay++;
	

	 pro_t.gTimer_pro_feed_dog++;
	
     pro_t.gTimer_pro_display_dht11_temp++;
	
	 pro_t.gTimer_pro_fan++;  //fan continuce counter 60s
	 /*******************************************/
	  pro_t.gTimer_pro_time_split_symbol++;
	  pro_t.gTimer_pro_wifi_led++;

	  pro_t.gTimer_pro_timer_mode_times++;
	  pro_t.gTimer_pro_mode_key_be_select++;
	  pro_t.gTimer_pro_set_tem_value_blink++;
	 

	  pro_t.gTimer_pro_mode_key_adjust ++;
	  pro_t.gTimer_pro_power_key_adjust++;
	  pro_t.gTimer_pro_ptc_delay_time++;
	 
	
	
	  pro_t.gTimer_pro_action_publis++;
	  pro_t.gTimer_pro_display_dht11_hum++;
	  pro_t.gTimer_pro_update_dht11_data++;
	  pro_t.gTimer_pro_wifi_dht11_temp_hum ++;
	  pro_t.gTimer_pro_pub_set_timer ++;
	  pro_t.gTimer_pro_action_publis_main_fun++;
	  pro_t.gTimer_pro_set_long_key_tims++;
	  pro_t.gTimer_pro_update_tft_icon++;
	 

	  //gctl_t 
	   gctl_t.gTimer_ctl_set_timer_time_senconds++;
	   gctl_t.gTimer_ctl_ptc_adc_times++;
	   gctl_t.gTimer_ctl_fan_adc_times ++;
	   gctl_t.gTimer_ctl_disp_works_time_second++;
	   gctl_t.gTimer_ctl_warning_time++;
	   gctl_t.gTimer_ctl_usart1_error++; 
	   gctl_t.gTimer_ctl_usart2_error++; 
	   gctl_t.gTimer_ctl_dma_state++ ;
	   gctl_t.gTimer_ctl_publish_set_temperature_value++;
	   gctl_t.gTimer_ctl_set_timer_value++;
	   
	
	  //wifi counter 
	 
	  wifi_t.gTimer_login_tencent_times++;
	  wifi_t.gTimer_publish_dht11++;
	  wifi_t.gTimer_auto_detected_net_state_times++;
	  wifi_t.gTimer_get_beijing_time++;
	  wifi_t.gTimer_read_beijing_time++;
	  wifi_t.gTimer_wifi_power_on_detect++;
	  wifi_t.gTimer_wifi_counter_link_beijing_times++;
	  wifi_t.gTimer_wifi_rx_error++;
	
	  wifi_t.gTimer_linking_tencent_duration++;
	  wifi_t.gTimer_power_first_link_tencent++;
	  wifi_t.gTimer_wifi_pub_power_off++;
	  wifi_t.gTimer_main_pro_times++;
	  wifi_t.gTimer_wifi_sub_power_off++;
	  
	 

	  //voice sound 
	
	   v_t.gTimer_voice_time_counter_start++;
	  	

	  

	  if(tm2 > 59){//1 minute
          tm2 =0;
		  gctl_t.gTimer_ctl_total_continue_time++;
	  }
	 
	  
	 
   }
  }
}

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart2) //voice  sound send 
	{
		v_t.transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}

//	if(huart== &huart2){
//
//       usart2_transOngoingFlag =0;
//
//	}

}






