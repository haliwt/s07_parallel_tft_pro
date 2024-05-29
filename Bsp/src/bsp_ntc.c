#include "bsp.h"


#define Zero_Degree           5828    
#define ADC_Sample_Times      10

#define COMPENSATION_VALUE    0

#define NTC_RES_F3425         1

touchpad_t ctl_t;




uint16_t *pArray[23];
uint8_t find_out_temperature_value;
uint8_t temp_uint16_t_vlue;
uint8_t length_simple;

uint16_t temp_variable_value_1;

uint8_t  temp_degree;
uint8_t temp_decimal_point;
uint16_t ntc_voltage_value;
uint8_t array_subscript;

uint8_t disp_temp_degree;




static uint16_t Read_NTC_Temperature_Voltage(void);

static int8_t  Binary_Search(const uint8_t *array ,uint8_t key,uint8_t length);




static void Read_Ntc_Decimal_Point_Numbers(void);

static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint16_t key,uint8_t length);

//static void Display_Speicial_Temperature_Value(uint8_t temp);


uint8_t search_key;

//std voltage
static const uint16_t R10K_NTC[]={

    2993,2977,2960,2942,2923,  //-20 ~ -16
    2904,2884,2863,2842,2819,  //-15 ~ -11
    2796,2773,2748,2723,2697,  //-10 ~ -6
    2671,2644,2616,2587,2558,   //-5 ~ -1
    2528,2497,2466,2434,2402,	//0 ~ 4
    2369,2335,2301,2267,2232,	//5 ~ 9
    2197,2162,2126,2090,2054,	//10 ~ 14
    2017,1980,1944,1907,1870,	//15 ~ 19
    1833,1796,1759,1722,1686,	//20 ~ 24
    1650,1613,1577,1542,1506,	//25 ~ 29
    1471,1437,1402,1369,1335,	//30 ~ 34
    1302,1270,1238,1206,1175,	//35 ~ 39
    1145,1115,1085,1057,1028,	//40 ~ 44
    1001,974,947,921,896,		//45 ~ 49
    871,847,825,800,777,		//50 ~ 54
    756,734,713,693,673,		//55 ~ 59
    654,635,617,599,582,		//60 ~ 64
    565,549,533,518,503,		//65 ~ 69
    488,474,460,447,434, 		//70 ~ 74
    422,409,398,386,375,		//75 ~ 79
    364,354						//80 ~ 81
    
    
    
};
//0 degree to 80 degree
static const uint16_t R10K_NTC_81[]={

    2528,2497,2466,2434,2402,	//0 ~ 4
    2369,2335,2301,2267,2232,	//5 ~ 9
    2197,2162,2126,2090,2054,	//10 ~ 14
    2017,1980,1944,1907,1870,	//15 ~ 19
    1833,1796,1759,1722,1686,	//20 ~ 24
    1650,1613,1577,1542,1506,	//25 ~ 29
    1471,1437,1402,1369,1335,	//30 ~ 34
    1302,1270,1238,1206,1175,	//35 ~ 39
    1145,1115,1085,1057,1028,	//40 ~ 44
    1001,974,947,921,896,		//45 ~ 49
    871,847,825,800,777,		//50 ~ 54
    756,734,713,693,673,		//55 ~ 59
    654,635,617,599,582,		//60 ~ 64
    565,549,533,518,503,		//65 ~ 69
    488,474,460,447,434, 		//70 ~ 74
    422,409,398,386,375,		//75 ~ 79
    364,354						//80 ~ 81
    
};




typedef enum{

   degree_zero = 0,
   degree_one = 1,
   degree_two = 2,
   degree_three = 3,
   degree_four = 4,
   degree_five =5,
   degree_six = 6,
   degree_seven = 7,
   degree_eight = 8,
   degree_nine = 9,
   degree_ten = 10,
   degree_eleven = 11,
   degree_twelve = 12,
   degree_thirteen =13,
   degree_fourteen = 14,
   degree_fiveteen =15,
   degree_sixteen =16,
   degree_seventeen =17,
   degree_eighteen = 18,
   degree_nineteen = 19,
   degree_twenty = 20,
   degree_twenty_one = 21,
   degree_twenty_two =22
   
   


}degree_state;


int8_t left_point =0;
int8_t right_point ;
int8_t length = sizeof(R10K_NTC_81)/(sizeof(R10K_NTC_81[0]));
int8_t mid_value;
//拆分成23个数组，mid =11
static const uint16_t R10K_0_0[2]={2558,2528};
static const uint16_t R10K_1_4[4]={2497,2466,2434,2402};
//NTC RES F3950  new calculation 

static const uint16_t R10K_5_7[3]={2382,2348,2314};
static const uint16_t R10K_8_10[3]={2280,2245,2210,};
static const uint16_t R10K_11_13[3]={2174,2138,2102};

static const uint16_t R10K_14_15[3]={2065,2028};
static const uint16_t R10K_16_18[3]={1991,1954,1917};

static const uint16_t R10K_19_21[3]={1880,1843,1806};
static const uint16_t R10K_22_23[2]={1768,1731};
static const uint16_t R10K_24_26[3]={1694,1658,1622};

static const uint16_t R10K_27_29[3]={1586,1551,1516};
static const uint16_t R10K_30_32[3]={1481,1447,1413};
static const uint16_t R10K_33_35[3]={1379,1346,1313};
static const uint16_t R10K_36_38[3]={1281,1249,1218};
static const uint16_t R10K_39_41[3]={1187,1157,1127};

static const uint16_t R10K_42_45[4]={1098,1069,1041,1013};

static const uint16_t R10K_46_49[4]={986,960,934,909};
static const uint16_t R10K_50_53[4]={884,860,836,813};

static const uint16_t R10K_54_58[5]={790,768,747,726,706};
static const uint16_t R10K_59_63[5]={686,667,648,629,612};

static const uint16_t R10K_64_70[7]={594,577,561,545,529,514,500};

static const uint16_t R10K_71_76[6]={485,472,458,445,432,420};
///


static const uint8_t R10K_Init_20_75_simple[15]={

	18,17,16,15,14,
	13,12,11,10,9,
	8,7,6,5,4
};



#if 0
static const uint8_t R10K_Init_0_81_simple[23]={

    25,24,23,22,21,
	20,19,18,17,16,
	15,14,13,12,11,
	10,9,8,7,6,
	5,4,3
};
#endif 




static uint16_t Get_Adc_Channel(uint32_t ch,uint32_t rank) ;

static uint16_t Get_Adc_Average(uint32_t ch,uint32_t,uint8_t times);

static void Judge_PTC_Temperature_Value(uint16_t adc_ptc);

static void display_ntc_temp_value(uint8_t disp);

static void Display_Speicial_Temperature_Value(uint8_t temp);

uint16_t ptc_res_temp_voltage;

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
*****************************************************************/
static uint16_t Get_Adc_Channel(uint32_t ch,uint32_t rank)   
{
    ADC_ChannelConfTypeDef adc_channel;

	 /* ÅäÖÃ¶ÔÓ¦ADCÍ¨µÀ */
   
    adc_channel.Channel = ch;               /* ÉèÖÃADCX¶ÔÍ¨µÀch */
    adc_channel.Rank = rank;                /* ÉèÖÃ²ÉÑùÐòÁÐ */
	//adc_channel.Offset = 0;
    adc_channel.SamplingTime = ADC_SAMPLETIME_3CYCLES;;       /* ÉèÖÃ²ÉÑùÊ±¼ä */
           
    HAL_ADC_ConfigChannel(&hadc1,&adc_channel);        //Í¨µÀÅäÖÃ
	
    HAL_ADC_Start(&hadc1);                               //start ADC transmit
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
*****************************************************************/
static uint16_t Get_Adc_Average(uint32_t ch,uint32_t rank,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Channel(ch,rank);
		delay_ms(1);
	}
	return temp_val/times;
} 

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
*****************************************************************/
void Get_Ntc_Resistance_Temperature_Voltage(void)
{
    uint8_t length_simple ;
	uint16_t adcx, temp_vlue;
	adcx = Get_Adc_Average(ADC_CHANNEL_1,1,30);

    ptc_res_temp_voltage  =(uint16_t)((adcx * 3300)/4096); //amplification 100 ,3.11V -> 311

    temp_vlue= ptc_res_temp_voltage /100;
	length_simple = 15; //sizeof(R10K_Init_0_81_simple)/sizeof(R10K_Init_0_81_simple[0]);
    
   	 disp_temp_degree = Binary_Search(R10K_Init_20_75_simple,temp_vlue,length_simple);

	Display_Speicial_Temperature_Value(disp_temp_degree);
	display_ntc_temp_value(ctl_t.temperature_value);
    
}


/*************************************************************************
	*
	*Functin Name: static int8_t  Binary_Search(uint8_t *R10K_NTC_81 ,uint8_t key)
	*Funtion: binary search arithmetic
	*
	*
	*
*************************************************************************/
static int8_t  Binary_Search(const uint8_t *array ,uint8_t key,uint8_t length)
{
     left_point =0 ;
	right_point = length -1;
   while(left_point <= right_point){

        mid_value = (left_point + right_point)/2;

        if(key == array[mid_value]){

		    return   mid_value;

		}
        else if(array[mid_value] >   key ){ //right ->big number

             left_point = mid_value +1;
			

		}
		else if(array[mid_value] < key){ //left -> small number

          right_point = mid_value -1;
		   

       }
	} 
   
	return -1;
} 

/******************************************************************************
	*
	*Function Name: static void Display_Speicial_Temperature_Value(uint8_t temp)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
******************************************************************************/
static void Display_Speicial_Temperature_Value(uint8_t temp)
{
  // static uint8_t array_subscript;

   switch(temp){

    case degree_zero: //zero degree  20 degree

         // zero_d =2;
        array_subscript =  Calculate_Display_Temperature_Value(R10K_20,ctl_t.ntc_voltage_value,3);
      
       switch(array_subscript){

		 case 0:
		 	ctl_t.temperature_value = 20 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 1:
		 	ctl_t.temperature_value = 20 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	ctl_t.temperature_value = 20+ ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;


        }

   break;


   case degree_one: //zero degree  21~23 degree

         // zero_d =2;
        array_subscript =  Calculate_Display_Temperature_Value(R10K_21_23,ctl_t.ntc_voltage_value,3);
      
       switch(array_subscript){

		 case 0:
		 	ctl_t.temperature_value = 21 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 1:
		 	ctl_t.temperature_value = 22 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	ctl_t.temperature_value = 23+ ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;


        }

   break;

   case degree_two: //24~26 degree
   	   array_subscript =  Calculate_Display_Temperature_Value(R10K_24_26,ctl_t.ntc_voltage_value,3);
	   
	   switch(array_subscript){

		 case 0:
		 	ctl_t.temperature_value = 24 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;

		 break;

		 case 1:
		 	ctl_t.temperature_value = 25 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	ctl_t.temperature_value = 26 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		

        }

   break;

   
   case degree_three : //27~~30 degree

         array_subscript =  Calculate_Display_Temperature_Value(R10K_27_30,ctl_t.ntc_voltage_value,4);
		 // HAL_Delay(5);
  		
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 27 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 28 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 2:
			  ctl_t.temperature_value = 29 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 3:
			  ctl_t.temperature_value = 30 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  
   
		  }

   break;
   
   case degree_four: //31~~33 degree

         array_subscript =  Calculate_Display_Temperature_Value(R10K_31_33,ctl_t.ntc_voltage_value,3);
			// HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 31 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 32 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 33 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;

   case degree_five : //34 À~~37 degree
		 array_subscript =  Calculate_Display_Temperature_Value(R10K_34_37,ctl_t.ntc_voltage_value,4);
		
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 34 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 35 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 36 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ctl_t.temperature_value = 37 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }
   break;
   
   case degree_six: //38 ```41 degree
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_38_41,ctl_t.ntc_voltage_value,4);
		 // HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 38 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 39 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 40 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ctl_t.temperature_value = 41 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_seven ://42 ````44 degree
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_42_44,ctl_t.ntc_voltage_value,3);
		  

		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 42 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 43 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ctl_t.temperature_value = 44 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
   
		  }

   break;

  
   case degree_eight : //45````49 degree
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_45_49,ctl_t.ntc_voltage_value,5);
		  //HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 45 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 46+ ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 47 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ctl_t.temperature_value = 48 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ctl_t.temperature_value = 49 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;

   
   case degree_nine :  //50````53 degree
   	    array_subscript =  Calculate_Display_Temperature_Value(R10K_50_53,ctl_t.ntc_voltage_value,4);
		  //HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 50 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 51 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 52 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ctl_t.temperature_value = 53 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_ten : //54````58 degree
   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_54_58,ctl_t.ntc_voltage_value,5);
	     // HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 54 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 55 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 56 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ctl_t.temperature_value = 57 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ctl_t.temperature_value = 58 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case  degree_eleven ://59````64 degree
   		 array_subscript =  Calculate_Display_Temperature_Value(R10K_59_64,ctl_t.ntc_voltage_value,6);
		 // HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 59 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 60 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 61 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ctl_t.temperature_value = 62 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ctl_t.temperature_value = 63 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ctl_t.temperature_value = 64 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_twelve : //65````71 degree
   	   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_65_71,ctl_t.ntc_voltage_value,7);
	    //   HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 65 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 66 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 67 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ctl_t.temperature_value = 68 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ctl_t.temperature_value = 69 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ctl_t.temperature_value = 70 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 6:
			  ctl_t.temperature_value = 71 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   
   
		  }

   break;

   case degree_thirteen : //72````78 degree
   		 array_subscript =  Calculate_Display_Temperature_Value(R10K_72_78,ctl_t.ntc_voltage_value,7);
		//  HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 72 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 73 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 74 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 3:
			  ctl_t.temperature_value = 75 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ctl_t.temperature_value = 76 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ctl_t.temperature_value = 77 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 6:
			  ctl_t.temperature_value = 78 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_fourteen : //79~85 degree
       array_subscript =  Calculate_Display_Temperature_Value(R10K_79_85,ctl_t.ntc_voltage_value,7);
	     //  HAL_Delay(5);
		 switch(array_subscript){
   
		   case 0:
			  ctl_t.temperature_value = 79 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  ctl_t.temperature_value = 80 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  ctl_t.temperature_value = 81 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		    case 3:
			  ctl_t.temperature_value = 82 + ctl_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 4:
			  ctl_t.temperature_value = 83 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 5:
			  ctl_t.temperature_value = 84 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;

		   case 6:
			  ctl_t.temperature_value = 85 + ctl_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;

   }
}
/******************************************************************************
	*
	*Function Name: static uint8_t Calculate_Display_Temperature_Value(const uin16_t *pt,uint8_t length)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
******************************************************************************/
static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint16_t key,uint8_t length)
{
      uint8_t i;
	
	
	  uint16_t temp_temperature_value;
      for(i=0;i<length;i++){

	   		 if(i==0){
               if(*(pt+0) < key){

			    if(key- *(pt+0) >=13){

				   ctl_t.temperature_rectify_value = -1;
				  

				}
                else{
					ctl_t.temperature_rectify_value =0;
                }
				
				    temp_decimal_point = key - *(pt+i);

			        temp_decimal_point = temp_decimal_point +9;

	   		       ctl_t.temperature_decimal_point_value =  temp_decimal_point/10  ;

               }

			   temp_temperature_value =  i;
			   return temp_temperature_value ;

            }
	 	    else if(*(pt+i) <  key && (*(pt+i+1) < key)){ //high temperature degree is number is smaller

                temp_temperature_value =  i;
				return temp_temperature_value ;
				 
            }
		     else if(i==(length-1)){

               if(*(pt+i) >  key){


			   if((*(pt+i) - key) >=13){
                 ctl_t.temperature_rectify_value =1;
				 
               }
			   else
		      	ctl_t.temperature_rectify_value =0;

			    temp_decimal_point = *(pt+i)-key; //小数点

		        temp_decimal_point = temp_decimal_point +9;


   		       ctl_t.temperature_decimal_point_value =  temp_decimal_point/10 ;

                temp_temperature_value  = i;

				return temp_temperature_value ;
			   	

               }

		 }
		
	  }
	  return 0;

	 
	  
}
  
  
/***********************************************************************************************
	*
	*Function Name:void TFT_Display_WorksTime(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
*************************************************************************************************/
static void display_ntc_temp_value(uint8_t disp)
{


static uint8_t timer_decade_hours,timer_unit_hours;


 timer_decade_hours =disp /10;
 timer_unit_hours =  disp % 10;

 //timer_decade_minutes= gctl_t.gSet_timer_minutes / 10;
 //timer_unit_minutes = gctl_t.gSet_timer_minutes % 10;

    

//      TFT_Disp_Pic_Warnign_Words(100,120,disp,0);//“高”
//	  TFT_Disp_Pic_Warnign_Words(136,120,disp,1);//“温”
//	  TFT_Disp_Pic_Warnign_Words(172,120,disp,2);//“警”
//	  TFT_Disp_Pic_Warnign_Words(208,120,disp,3);//“告”


	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,120,0,timer_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(136,120,0,timer_unit_hours);
}

  
  
  

