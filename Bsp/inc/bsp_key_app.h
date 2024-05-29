#ifndef __BSP_KEY_APP_H
#define __BSP_KEY_APP_H
#include "main.h"




void Key_Speical_Power_Fun_Handler(void);
void Key_Speical_Mode_Fun_Handler(void);
void Mode_Key_Config_Fun_Handler(void);
void ADD_Key_Fun(void);
void DEC_Key_Fun(void);

void Mode_Key_Select_Fun(void);

void Mode_Key_Confirm_Fun(void);


void Key_Speical_Mode_Fun_NoInt_Handler(uint8_t data);


void Key_Interrup_Handler(void);

void ModeKey_Select_Default_LedOnOff(void);



#endif 

