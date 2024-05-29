#include "bsp.h"


static uint8_t key_works_timer_timing_fun(void);

static uint8_t disp_works_timer_timing_fun(void);


void TimerTiming_Init(void)
{

    Key_Works_Timer_Timing_Handler(key_works_timer_timing_fun);

   Dis_pWorks_Timer_Timing_Handler(disp_works_timer_timing_fun);
}


//static void TFT_Donnot_Set_Timer_Time(void);

void TimeTimer_Pro_Handler(void)
{
 
  
  switch(pro_t.disp_works_timer_timing_mode_item){


   case works_time://1

		if(gctl_t.gTimer_ctl_disp_works_time_second > 59 ){
			TFT_Display_WorksTime();
		}

		//TFT_Donnot_Set_Timer_Time();

	break;

	case timer_time: //01 ->timer_time 
		if(gctl_t.gTimer_ctl_set_timer_time_senconds >59 ){

		
			gctl_t.gTimer_ctl_set_timer_time_senconds =0;

			gctl_t.gSet_timer_minutes --;

			if(gctl_t.gSet_timer_minutes <0){
			gctl_t.gSet_timer_minutes =59;
			gctl_t.gSet_timer_hours --;
			}

			
			if(gctl_t.gSet_timer_hours < 0){

			pro_t.gPower_On = power_off;
			pro_t.power_off_flag=1;
			pro_t.run_process_step=0xff;
			 TFT_BACKLIGHT_OFF();
				LED_Mode_Key_Off();
			    LED_Power_Key_Off();

			}
			TFT_Disp_Set_TimerTime(0);
		}
		
		TFT_DonnotDisp_Works_Time();

		
	break;

	

	 case timer_set_time://02

		if(pro_t.gTimer_pro_set_long_key_tims > 2){
            pro_t.gTimer_pro_set_long_key_tims =0;  
			gctl_t.mode_long_key_flag =0;  
		
			if(gctl_t.gSet_timer_hours >0 ){
           
				pro_t.disp_works_timer_timing_mode_item= timer_time;
				pro_t.mode_key_run_proc_item =0xff;
				gctl_t.gTimer_ctl_set_timer_time_senconds =0;
			
				gctl_t.gSet_timer_minutes =0;
				//TFT_Disp_Chinese_Timer_96_24(100,150);
				 TFT_Disp_Chinese_Timer_23_23(TIMER_X0,TIMER_Y,0);
				   TFT_Disp_Chinese_Timer_23_23(TIMER_X1,TIMER_Y,1);
				   TFT_Disp_Chinese_Timer_23_23(TIMER_X2,TIMER_Y,2);
				   TFT_Disp_Chinese_Timer_23_23(TIMER_X3,TIMER_Y,3);
				    TFT_Only_Disp_Set_Timer_Blink();
					HAL_Delay(100);
					TFT_Disp_Onley_Set_TimerTime_Value();
					
					
			}
			else{
				
		  
				if(wifi_link_net_state()==1 && gctl_t.beijing_time_flag==1){
					pro_t.mode_key_run_proc_item =0xff;
					pro_t.disp_works_timer_timing_mode_item= works_time;//pro_t.disp_works_timer_timing_mode_item
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X0,TIMER_Y,0);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X1,TIMER_Y,1);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X2,TIMER_Y,2);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X3,TIMER_Y,3);
					TFT_Display_WorksTime();

				}
				else{

					pro_t.mode_key_run_proc_item =0xff;
					pro_t.disp_works_timer_timing_mode_item= works_time;//pro_t.disp_works_timer_timing_mode_item
					//TFT_Disp_Chinese_WorksTime_96_24(100,150);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X0,TIMER_Y,0);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X1,TIMER_Y,1);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X2,TIMER_Y,2);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X3,TIMER_Y,3);
					TFT_Display_WorksTime();
			}
			}
			
		}
	   break;
	}

}
/**********************************************************************************************************
    **
    *Function Name:TFT_Donnot_Set_Timer_Time();
    *Function : 记录设置的定时时间，
    *Input Ref:
    *Return Ref:NO
    *
*********************************************************************************************************/
#if 0
static void TFT_Donnot_Set_Timer_Time(void)
{
    if(pro_t.disp_works_timer_timing_mode_item== timer_time ){

	if(gctl_t.gTimer_ctl_set_timer_time_senconds >59){
		gctl_t.gTimer_ctl_set_timer_time_senconds =0;

		gctl_t.gSet_timer_minutes --;

		if(gctl_t.gSet_timer_minutes <0){
		gctl_t.gSet_timer_minutes =59;
		gctl_t.gSet_timer_hours --;


		}
		if(gctl_t.gSet_timer_hours < 0){

			pro_t.gPower_On = power_off;

		}

	}

   }

}
#endif

void Display_Precise_Works_Time(void)
{
	  if(pro_t.disp_works_timer_timing_mode_item==timer_time){
	  	TFT_DonnotDisp_Works_Time();
	  }
	  else{
	     if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
				  TFT_Display_Precise_WorksTime();
			}


	  }


}

/********************************************************************************
 * 
 * Function Name: void Power_Off_Retain_Beijing_Time(void)
 * Function:  get beijing of timing 
 * Input Ref: 
 * Return Ref:
 * 
*********************************************************************************/
void Power_Off_Retain_Beijing_Time(void)
{
  if(wifi_link_net_state()==1){
	if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
		 gctl_t.gTimer_ctl_disp_works_time_second =0;
		   
				 gctl_t.disp_works_minutes++;
                  if(gctl_t.disp_works_minutes>59){
					   gctl_t.disp_works_minutes=0;
					   gctl_t.disp_works_hours++;
					   if(gctl_t.disp_works_hours > 23){
						   gctl_t.disp_works_hours=0;
					   }
			     }
			
   }
 }
}

	
/**********************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void Key_Works_Timer_Timing_Handler(uint8_t(*worktiming_handler)(void))
{
     pro_t.key_works_timer_timing_state = worktiming_handler;

}


static uint8_t key_works_timer_timing_fun(void)
{

     if(pro_t.mode_key_run_proc_item==mode_key_set_temp)return mode_key_set_temp;
	 else if(pro_t.mode_key_run_proc_item==mode_key_select)return mode_key_select;
	 else if(pro_t.mode_key_run_proc_item==mode_key_confirm)return mode_key_confirm;
     else if(pro_t.mode_key_run_proc_item==mode_key_set_timer_value)return mode_key_set_timer_value;
	 else 
	 	return  mode_key_set_temp;

	 

}

void Dis_pWorks_Timer_Timing_Handler(uint8_t(*disp_worktimer_handler)(void))
{

    pro_t.disp_works_timer_timing_state = disp_worktimer_handler;

}




static uint8_t disp_works_timer_timing_fun(void)
{

  if(pro_t.disp_works_timer_timing_mode_item==works_time)return works_time ;
  else if(pro_t.disp_works_timer_timing_mode_item==timer_time)return timer_time ;
  else if(pro_t.disp_works_timer_timing_mode_item==timer_set_time)return timer_set_time ;

}




