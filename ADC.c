#include "lib/include.h"
extern void Configura_Reg_ADC(void)
{
/*
    b) 2,4,6,7,10,1- 30MHZ 57600 -sec3, sec1
                                                    PIN - CANAL
                                                    PE2 - AIN1,
                                                    PE1 - AIN2,
                                                    PD3 - AIN4, 
                                                    PD1 - AIN6, 
                                                    PD0 - AIN7, 
                                                    PB4 - AIN10
                           pag. 104
    mod 0 -> secuenciador 1 -> Int 15
    mod 1 -> secuenciador  3 -> Int 51   
    */
    //Pag 352 para inicializar el modulo de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0)|(1<<1);  //Inicializar  del reloj del adc  (RCGCADC) se habilita el modulo 1 y 0
    //Pag 340 los puertos (RGCGPIO) Puertos base habilitación del reloj SE habilitan los puertos correspondientes a los canales del adc pg. 801 para canales y puerto
    //                     F     E      D       C      B     A     
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(1<<3)|(0<<2)|(0<<1)|(1<<0); //Port A -> UART, Port F -> leds, Port E y D para canales 
    //Pag 663 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida. Pag 801 para pines correspondientes a canales
    GPIOB_AHB->DIR = (0<<4) ;// PIN B4
    GPIOE_AHB->DIR = (0<<1) | (0<<2); //PE1 PE2
    GPIOD_AHB->DIR = (0<<3) | (0<<1) | (0<<0); //PD3, PD1 y PD0
    //(GPIOAFSEL) pag.672  trabajan con función alternativa 
    GPIOB_AHB->AFSEL= (0<<4) ;// PIN B4
    GPIOE_AHB->AFSEL = (0<<1) | (0<<2); //PE1 PE2
    GPIOD_AHB->AFSEL = (0<<3) | (0<<1) | (0<<0); //PD3, PD1 y PD0
    //(GPIODEN) pag.683 desabilita el modo digital de los pines porque son analogicos
    GPIOB_AHB->DEN = (0<<4) ;// PIN B4
    GPIOE_AHB->DEN = (0<<1) | (0<<2); //PE1 PE2
    GPIOD_AHB->DEN = (0<<3) | (0<<1) | (0<<0); //PD3, PD1 y PD0
    //Pag 688 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1351, Se indica la función alternativa de los pines
    GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0xFF0FFFFF); // PONER EN 0 PIN 5 
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFFFF00F); //Poner 0 en los bits de los pines 1, 2 
    GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0xFFFF0F00); //Poner 0 en los bits de los pines 0, 1,3
    //(GPIOAMSEL) pag.687 habilitar analogico los pines de los canales
    GPIOB_AHB->AMSEL = (0<<4) ;// PIN B4
    GPIOE_AHB->AMSEL = (0<<1) | (0<<2); //PE1 PE2
    GPIOD_AHB->AMSEL = (0<<3) | (0<<1) | (0<<0); //PD3, PD1 y PD0
    //Pag 891 El registro (ADCPC) establece la velocidad de conversión por segundo
    //1Mms  SE pone 7 en binario porque trabaja con la max. velocidad que es 1 millon
    ADC0->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);
    ADC1->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);
    //Pag 841 configurar la prioridad de los secuenciadores 
    ADC0->SSPRI = 0x1203; //
    ADC1->SSPRI = 0x0213; //
    //Pag 821 control de la activación de los secuenciadores se desactivan los 4 secuenciadores 
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); 
    ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    //Pag 834 Este registro (ADCEMUX) selecciona el evento que activa la conversión 
    // Secuenciador 1 y 3 se le  da un valor de 4 para configurar con interrupciones
    ADC0->EMUX  = (0x0000); 
    ADC1->EMUX  = (0x0000); 
    //Pag 867 se definen entradas analógicas con el canal y secuenciador seleccionado,
    // Se establece que canal estará en cada mux por lo tanto en cada secuenciador y en cada modulo
    ADC0->SSMUX1 = 0x0124; // Los canales 1,2, 4 se definen para el mux 1 del secuenciador 1 en el modulo 0
    ADC1->SSMUX2 = 0x067A; // Los canales 6,7 y 10  se definen para el mux 2 del secuecniador 3 en el modulo 1
    //pag 868 Se configura el bit de control de muestreo y la interrupción
    // secuenciador 1        mux 0            mux 1            mux 2      es donde se establecen los canales en la configuración del SSMUX1 -> linea 70 
    ADC0->SSCTL1 = (1<<2) | (1<<1)| (1<<6) | (1<<5) | (1<<10) | (1<<9); // Para establecer configuración de muestro y finalizar muestro acorde a la pag. 868
    // secuenciador 3        mux 0            mux 1            mux 2  
    ADC1->SSCTL3 = (1<<2) | (1<<1)| (1<<6) | (1<<5) | (1<<10) | (1<<9);
    // Pag 825 Enable ADC Interrupt - Interrumpe la conversión del secuenciador a usar 
    /* Unmask ADC0 sequence 2 interrupt*/ 
    // Al secuenciador 1 y 3  se le asigna un 1 para indicar que manda la señal pura al controlador de interrupciones
    ADC0->IM |= (1<<1); 
    ADC1->IM |= (1<<3); 
    //Pag 821 (ADCACTSS) Este registro controla la activación de los secuenciadores utilizados 
    // se habilita el secuanciador 1 y 3
    ADC0->ACTSS = (0<<3) | (0<<2) | (1<<1) | (0<<0);
    ADC1->ACTSS = (1<<3) | (0<<2) | (0<<1) | (0<<0); 
    //se inicializa el secuenciador 1 y 3   cuando se configura el adc en modo de procesador
    ADC0->PSSI |= (1<<1); 
    ADC1->PSSI |= (1<<3); 
}
