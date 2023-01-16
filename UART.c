#include "lib/include.h"
//Configuración de UART0 que corresponde al PA0 y PA1  pag. 895
extern void Configurar_UART0(void)
{
    //P pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCUART  = (1<<0);   
    //  pag.340 Enable clock port A
    SYSCTL->RCGCGPIO |= (1<<0);     
    // pag.671 Enable alternate function
    GPIOA->AFSEL = (1<<1) | (1<<0); //PA0 y PA1
    // PA0-> U0Rx PA1-> U0Tx pag.1351 para conocer que valor poner
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // Habilitan los pines como digital pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0
    //Indicar entradas y salidas pag. 895 
            //      Rx(in) | Tx(out)
    //Desabilita-Limpia los registros pag. 918
    UART0->CTL = (0<<9) | (0<<8) | (0<<0); //bit 9 es de Tx, bit 8 de Rx, y 0 al uart en genral.
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 80*10^6 / (16 * 19200) = 260.4       yo: 80MHZ Baud-rate 19200
    UARTFBRD[DIVFRAC] = integer(0.4 * 64 + 0.5) = 27 */
    UART3->IBRD = 260;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915 */
    UART3->FBRD = 27;  /* (parte fraccionaria del calculo) */
    UART3->LCRH = (0x3<<5);  //Para que se tengan 8 bits, y |(1<<4); activar el bit de paridad pero solo con el uart0
    //  UART Clock Configuration(UARTCC) pag.939
    UART3->CC =(0<<0); /*PARA USAR EL RELOJ INTERNO*/
    //Disable UART3 UART Control (UARTCTL) pag.918
    UART3->CTL = (1<<0) | (1<<8) | (1<<9);   //se habilitan los pines de recepción, el pin 8 y 9 se deben prender juntos

}




