#include "bsp.h"

uint8_t sub_send_power_on_times;
uint8_t get_rx_beijing_time_enable;


uint8_t beijing_step;
uint8_t auto_det_flag;


static void MainBoard_Self_Inspection_PowerOn_Fun(void);
static void RunWifi_Command_Handler(void);
static void auto_repeat_link_netware_fun(void);
static void auto_repeat_init_link_net(void);
static void auto_repeat_tencnet_net(void);

/********************************************************************************
	*
	*Functin Name:void WIFI_Process_Handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void WIFI_Process_Handler(void)
{
  	MainBoard_Self_Inspection_PowerOn_Fun();
    RunWifi_Command_Handler();
    if(wifi_t.get_rx_beijing_time_enable==0){
     Tencent_Cloud_Rx_Handler();
	 Json_Parse_Command_Fun();
    }
}
/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
static void MainBoard_Self_Inspection_PowerOn_Fun(void)
{
    static uint8_t counter;
		if(wifi_t.power_on_linkwifi==0 && wifi_link_net_state()==0 && wifi_t.rx_error_codes_flag==0){
		

		
		InitWifiModule_Hardware();//InitWifiModule();
        SmartPhone_TryToLink_TencentCloud();
		

		if(counter ==0){
			counter++;
          wifi_t.gTimer_wifi_pub_power_off =0;
       }
       
    }

	if(wifi_t.gTimer_wifi_pub_power_off > 20  && wifi_link_net_state()==0 && counter==1){
		counter++;
		wifi_t.power_on_linkwifi++;

   }


	if((wifi_link_net_state()==1 && counter==1)){
		counter++;
			
			wifi_t.gTimer_auto_detected_net_state_times=0;
	        wifi_t.rx_error_codes_flag=0;
			wifi_t.get_rx_beijing_time_enable=0;
			wifi_t.rx_error_codes_flag=0;
		    if(pro_t.gPower_On == power_on){
				wifi_t.power_on_linkwifi++;
				wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data; //0x01
		    }
			else{
				if(wifi_t.power_off_step==1){

					wifi_t.power_on_linkwifi++;
                    
               
				}
				else{
					wifi_t.power_on_linkwifi++;
				
					wifi_t.gTimer_wifi_pub_power_off =6;


				}


			}
			
	    }


	if(wifi_t.rx_error_codes_flag==1 && wifi_t.rx_setup_hardware_counter< 6 && wifi_link_net_state()==0 && wifi_t.gTimer_wifi_rx_error >6){
		wifi_t.gTimer_auto_detected_net_state_times=0;
		wifi_t.gTimer_get_beijing_time =0;
		wifi_t.gTimer_wifi_rx_error =0;
        WIFI_IC_DISABLE();
		HAL_Delay(1000);
		Key_Speical_Power_Fun_Handler();
	    HAL_Delay(1000);
		Key_Speical_Power_Fun_Handler();
		WIFI_IC_ENABLE();
        HAL_Delay(1000);
		Key_Speical_Power_Fun_Handler();

		wifi_t.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
		wifi_t.wifi_uart_counter=0;
		wifi_t.soft_ap_config_flag =0;
		
 
	
		
       HAL_UART_Transmit(&huart1, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
	  
	   

	   HAL_Delay(1000);
	   Key_Speical_Power_Fun_Handler();
	  
	   HAL_Delay(1000);
	   Key_Speical_Power_Fun_Handler();
	 
	  
	  wifi_t.rx_setup_hardware_counter++;
		

	}


	if(wifi_link_net_state()==1 && wifi_t.rx_setup_hardware_counter < 8 ){

            wifi_t.rx_setup_hardware_counter=9;
			wifi_t.rx_error_codes_flag=0;
			wifi_t.power_on_thefirst_times =0;
			pro_t.gTimer_pro_action_publis=0;
			pro_t.gTimer_pro_pub_set_timer = 0;
			wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;//04
	   	 

	}

	
}
/********************************************************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *Input Ref:NO
   *Return Ref:NO
   *
*********************************************************************************/
static void RunWifi_Command_Handler(void)
{
  
  
   static uint8_t link_net_flag; 

   
 	switch(wifi_t.runCommand_order_lable){

   case wifi_link_tencent_cloud: //01

	if(power_on_state() == power_on){
 
		Wifi_Fast_Led_Blink(); 
		Wifi_SoftAP_Config_Handler();
		Wifi_Fast_Led_Blink();
		SmartPhone_LinkTencent_Cloud();
		Wifi_Fast_Led_Blink();
		//wifi_t.auto_linknet_flag =0;

	}
	
	if(wifi_link_net_state()==0){
		if(wifi_t.gTimer_linking_tencent_duration < 166 ){
			Wifi_Fast_Led_Blink();
		   wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
		}
		else if(wifi_link_net_state()==0 && wifi_t.gTimer_linking_tencent_duration >166){
		  //auto link wifi net 
		  	wifi_t.gTimer_linking_tencent_duration=0;
		
			wifi_t.esp8266_login_cloud_success =0;
			wifi_t.rx_error_codes_flag= 1;
			wifi_t.rx_setup_hardware_counter=0;
			wifi_t.gTimer_auto_detected_net_state_times=0;
			wifi_t.gTimer_wifi_rx_error = 0;
			wifi_t.get_rx_beijing_time_enable=0;

			wifi_t.runCommand_order_lable = wifi_null;
		   
		 }

		
		

		}


	if(wifi_link_net_state()==1){

	wifi_t.get_rx_beijing_time_enable=0;
	wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;

	wifi_t.gTimer_auto_detected_net_state_times =0;
	wifi_t.gTimer_linking_tencent_duration=0;
	// MqttData_Publish_SetOpen(0x01);
	   wifi_t.gTimer_get_beijing_time =0;
	  wifi_t.gTimer_publish_dht11=0;        
    }
	break;


	case wifi_tencent_publish_init_data://02

       if(wifi_t.gTimer_publish_dht11 >0){
	 
	       MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
		   wifi_t.gTimer_publish_dht11=0;
		   wifi_t.gTimer_get_beijing_time =0;
	       wifi_t.runCommand_order_lable = wifi_subscriber_form_tencent_data;

       }


    break;

			
     case wifi_subscriber_form_tencent_data: //03

	   if( wifi_t.gTimer_publish_dht11 >4){

	      Subscriber_Data_FromCloud_Handler();
		 wifi_t.gTimer_publish_dht11=0;
	     wifi_t.gTimer_get_beijing_time =0;
	     wifi_t.get_rx_auto_repeat_net_enable=0;
		 wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	   }


	break;



	case wifi_publish_update_tencent_cloud_data://06

	if(wifi_t.gTimer_get_beijing_time > 180 && (wifi_link_net_state()==1 && wifi_t.get_rx_auto_repeat_net_enable==0)){
        
			wifi_t.gTimer_get_beijing_time=0;
		    wifi_t.link_beijing_times_flag =1;
	
	 }

	if(wifi_t.link_beijing_times_flag ==1){
	   wifi_t.link_beijing_times_flag =0;
	   wifi_t.three_times_link_beijing++ ;
	  

	   if(wifi_t.three_times_link_beijing >0 && wifi_t.three_times_link_beijing <4){
		

	    wifi_t.beijing_time_flag =1;
	   	wifi_t.set_beijing_time_flag =1;
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		wifi_t.runCommand_order_lable= wifi_get_beijing_time;
       }
	   else{
        if(wifi_t.three_times_link_beijing % 2 == 0){
			wifi_t.beijing_time_flag =1;
			wifi_t.set_beijing_time_flag =1;
			wifi_t.gTimer_auto_detected_net_state_times =0;
		
			wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		
			wifi_t.runCommand_order_lable= wifi_get_beijing_time;

        }
		else{
			wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;


		}

	}
	}
	else{

      wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;

	}

	if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61 && beijing_step ==3){

		beijing_step ++;
		if(wifi_t.receive_beijing_time==0){

			 wifi_t.receive_beijing_time++;

	     }
		else{
			gctl_t.disp_works_hours   = wifi_t.real_hours;

			gctl_t.disp_works_minutes = wifi_t.real_minutes;

			gctl_t.gTimer_ctl_disp_works_time_second= wifi_t.real_seconds;

			gctl_t.beijing_time_flag = 1; //WT.2024.04.25

			if(pro_t.disp_works_timer_timing_state()==works_time){
				TFT_Display_WorksTime_Voice();

			}

		}
	}

	

	break;

	case wifi_auto_to_link_cloud:
	
	if(wifi_t.gTimer_auto_detected_net_state_times > 180){

		wifi_t.gTimer_auto_detected_net_state_times=0;


		wifi_t.wifi_uart_counter=0; //clear USART2 counter is zero
		if(wifi_t.auto_link_login_tencent_cloud_flag==0){ //link net is OK
			wifi_t.get_rx_auto_repeat_net_enable=1;
			Wifi_Link_Tencent_Net_State();
			wifi_t.gTimer_publish_dht11=0;

		}
		else{ //link net is NG
			//  Wifi_Reconnect_Tencent_Net();
			wifi_t.get_rx_auto_repeat_net_enable=0;
			wifi_t.power_on_login_tencent_cloud_flag =0;
			wifi_t.gTimer_publish_dht11=1;
			auto_det_flag=1 ;



		}

		wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;

	}
	else{

		wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;

	}


	break;

	case wifi_auto_repeat_link_cloud://09

	if(wifi_t.gTimer_publish_dht11>0){
		
	

	switch(wifi_t.auto_link_login_tencent_cloud_flag){


	case 0:  //link net is ok
	    Wifi_Rx_Auto_Link_Net_Handler();
		wifi_t.get_rx_auto_repeat_net_enable=0;
		wifi_t.gTimer_publish_dht11=0;
		wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;//wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;
		 wifi_t.gTimer_auto_detected_net_state_times=0;  

	break;

	case 1:  //don't link to tencet cloud net.
   
		auto_repeat_link_netware_fun();
		wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;
	    wifi_t.gTimer_auto_detected_net_state_times=0;  


	break;

	}

	}
	
    wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
	break;


	case wifi_get_beijing_time://8


	wifi_t.linking_tencent_cloud_doing =0;
    if(wifi_t.set_beijing_time_flag ==1){   //&& wifi_t.gTimer_beijing_time>1){
		wifi_t.set_beijing_time_flag ++;
		wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
		wifi_t.wifi_uart_counter=0;
		Get_BeiJing_Time_Cmd();
		HAL_Delay(100);
		wifi_t.gTimer_read_beijing_time=0;
		beijing_step =1;
		wifi_t.gTimer_wifi_counter_link_beijing_times = 0;

	}

	//if(wifi_t.gTimer_read_beijing_time > 2 && wifi_t.gTimer_read_beijing_time < 4){
	if(wifi_t.gTimer_read_beijing_time > 0 && beijing_step ==1){//
		beijing_step =2;
		wifi_t.beijing_time_flag =0;
		Get_Beijing_Time();
		HAL_Delay(100);
		wifi_t.gTimer_read_beijing_time=0;


	}

	if(beijing_step ==2 && wifi_t.gTimer_read_beijing_time > 2){

		
		wifi_t.beijing_time_flag = 1;

		wifi_t.real_hours = (wifi_t.wifi_data[134]-0x30)*10 + wifi_t.wifi_data[135]-0x30;
		wifi_t.real_minutes =(wifi_t.wifi_data[137]-0x30)*10 + wifi_t.wifi_data[138]-0x30;
		wifi_t.real_seconds = (wifi_t.wifi_data[140]-0x30)*10 + wifi_t.wifi_data[141]-0x30;

		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times



		wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		beijing_step =3;
		
	}

	
	if(beijing_step== 3){
	    wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	}


   break;


   case wifi_null:

   if(wifi_link_net_state()==1){

	wifi_t.get_rx_beijing_time_enable=0;
	wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;

	wifi_t.gTimer_auto_detected_net_state_times =0;
	wifi_t.gTimer_linking_tencent_duration=0;
	// MqttData_Publish_SetOpen(0x01);
	   wifi_t.gTimer_get_beijing_time =0;
	  wifi_t.gTimer_publish_dht11=0;  
	  wifi_t.rx_error_codes_flag= 0;
    }


   break;

	   
	  default:

	   break;
	 
  }

  if(wifi_t.gTimer_wifi_counter_link_beijing_times > 20 && wifi_t.get_rx_beijing_time_enable==1){

      wifi_t.gTimer_wifi_counter_link_beijing_times=0;
	  wifi_t.get_rx_beijing_time_enable=0;


  }
  
}
 
/********************************************************************************
	*
	*Functin Name:static void auto_repeat_link_netware_fun(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
static void auto_repeat_link_netware_fun(void)
{

  
  if(auto_det_flag==1){
        //Buzzer_KeySound();
      
		//InitWifiModule_Hardware();//InitWifiModule();
        auto_repeat_init_link_net();
        //SmartPhone_TryToLink_TencentCloud();
        auto_repeat_tencnet_net();
  }

    if(wifi_link_net_state()==1){

		  wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;//04
		  auto_det_flag=0;
            
		  wifi_t.gTimer_auto_detected_net_state_times=0;  
	 }

	
	
   
}


void Wifi_Rx_Auto_Link_Net_Handler(void)
{

   
	   strncpy((char *)wifi_t.auto_det_data, (const char *)wifi_t.wifi_data,150);
	   wifi_t.get_rx_auto_repeat_net_enable =1;

       if(strstr((const char*)wifi_t.auto_det_data,"QTTSTATE:0")){

			 wifi_t.esp8266_login_cloud_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =1;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"+TCMQTTRECONNECTING")){

			 wifi_t.esp8266_login_cloud_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =1;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"ONNECTING")){

			 wifi_t.esp8266_login_cloud_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =1;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"QTTSTATE:1")){

		   wifi_t.esp8266_login_cloud_success= 1;
		   wifi_t.auto_link_login_tencent_cloud_flag =0;


       }
       
         wifi_t.wifi_uart_counter=0;
		
         wifi_t.gTimer_auto_detected_net_state_times=0;
		
    
}


static void auto_repeat_init_link_net(void)
{
	    WIFI_IC_DISABLE();
		HAL_Delay(1000);
		//HAL_Delay(1000);
		//HAL_Delay(1000);
		WIFI_IC_ENABLE();
		//at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		HAL_Delay(1000);
}

static void auto_repeat_tencnet_net(void)
{
            wifi_t.linking_tencent_cloud_doing =1;
          
			wifi_t.wifi_uart_counter=0;
	        wifi_t.soft_ap_config_flag =0;
	        HAL_UART_Transmit(&huart1, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
		    HAL_Delay(1000);
			HAL_Delay(1000);
			HAL_Delay(1000);
		
}

