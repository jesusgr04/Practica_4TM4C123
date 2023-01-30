#include "lib/include.h"
extern void Configurar_GPIO(void)
//Configura puerto A para los 6 botones como interrupciones externas 

{
    //Se habilita el puerto  F
    SYSCTL->RCGCGPIO |=(1<<5);  //pueto F
    GPIOF->LOCK= 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIOF->CR = 0x1F;           // allow changes to PF4-0
    //Desabilita la función analogica del puerto, para funcionar como GPIO pag. 687
    GPIOF->AMSEL = 0x00; 
    //Indicar que los pines trabajan como GPIO on PA7-0
    GPIOF->PCTL = 0x00000000;   
    //Para indicar que pines son esntradas y cuales son salida 0 -> entrada, 1 -> salida 
    GPIOF->DIR = 0x0E; // PA2 - PA7 son entradas 
    //Se desabilita la función alternativa  PA7-0
    GPIOF->AFSEL = 0x00;   
    GPIOF->PUR = 0x11;// PA5 - PA0
    //Se habilitan los pines como digitales PA5-PA0
    GPIOF->DEN = 0x1F;     

    
}

extern void Delay(void)
{
  unsigned long volatile time;
  time = 1600000;
  while(time)
  {
		time--;
  }
}





