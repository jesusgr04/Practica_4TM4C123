#include "lib/include.h"
extern void Configurar_GPIO(void)
//Configura puerto A para los 6 botones como interrupciones externas 

{
    //Se habilita el puerto  A
    SYSCTL->RCGCGPIO |=(1<<0);  //pueto A
    //Desabilita la función analogica del puerto, para funcionar como GPIO pag. 687
    GPIOA->AMSEL = 0x00; 
    //Indicar que los pines trabajan como GPIO on PA7-0
    GPIOA->PCTL = 0x00000000;   
    //Para indicar que pines son esntradas y cuales son salida 0 -> entrada, 1 -> salida 
    GPIOA->DIR = (0<<2) | (0<<3) | (0<<4) | (0<<5) | (0<<6) | (0<<7); // PA2 - PA7 son entradas 
    //Se desabilita la función alternativa  PA7-0
    GPIOA->AFSEL = 0x00;   
    GPIOA->PUR = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);// PA5 - PA0
    //Se habilitan los pines como digitales PA5-PA0
    GPIOA->DEN = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);     
    //pag. 667 Se limpian los pines a los que se asocian las interrupciones para poderlas configurar 
    GPIOA -> IM |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //pag. 664 Configurar que sea sensible al borde 
     GPIOA -> IS |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //pag. 665 registro para indicar que el IEV va a configurar que flancos detectar
    GPIOA -> IBE |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //pag. 666 Indicar con que flanco trabaja 
    GPIOA -> IEV |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5); //es con flanco de subida
    //pag. 668 establecer que no ha ocurrido ninguna interrupción en esos pines (edo inicial)
    GPIOA -> RIS |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //PAG. 667 La interrupción del pin es enviada al controlador de interrup.
    GPIOA -> IM |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);
    //pag. 104 Configurar la interrupción correspondiente al puerto A -> int 0
    // int. 3       2      1    0
    //    [4n+3] [4n+2] [4n+1] [4n] ---> [4n] = 0(valor de la int) : n=0
    //      FF     FF     FF    00    (los 00 indica lo que se le suma)
    NVIC -> IP[0] = (NVIC ->IP[0]&0xFFFFFF00) | 0x00000002; //0 es el numero de prioridad (n)
    NVIC -> ISER[0] = 0x00000001; //se pone 1 en la interrupción 0
}


extern void GPIOA_INT_ISR(void)
{
   
}





