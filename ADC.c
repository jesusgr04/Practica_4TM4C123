#include "lib/include.h"
extern void Configura_Reg_ADC(void)
{
/* jhgr
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

    GPIOE->DIR &= ~(1<<2) ;// PIN E2 
    GPIOE->DIR &= ~(1<<1) ; // PIN E1
    GPIOB->DIR &= ~(1<<4) ; //PIN B4
    GPIOD->DIR &= ~(1<<3) ;// PIN D3
    GPIOD->DIR &= ~(1<<1) ;// PIN D1
    GPIOD->DIR &= ~(1<<0) ;// PIN D0

    //(GPIOAFSEL) pag.672  trabajan con función alternativa 
    GPIOB->AFSEL= (1<<4) ;// PIN B4
    GPIOE->AFSEL = (1<<1) | (1<<2); //PE1 PE2
    GPIOD->AFSEL = (1<<3) | (1<<1) | (1<<0); //PD3, PD1 y PD0
    //(GPIODEN) pag.683 desabilita el modo digital de los pines porque son analogicos
    GPIOB->DEN = (0<<4) ;// PIN B4
    GPIOE->DEN = (0<<1) | (0<<2); //PE1 PE2
    GPIOD->DEN = (0<<3) | (0<<1) | (0<<0); //PD3, PD1 y PD0
    //Pag 688 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1351, Se indica la función alternativa de los pines
    GPIOB->PCTL = GPIOB->PCTL & (0xFF0FFFFF); // PONER EN 0 PIN 5 
    GPIOE->PCTL = GPIOE->PCTL & (0xFFFFF00F); //Poner 0 en los bits de los pines 1, 2 
    GPIOD->PCTL = GPIOD->PCTL & (0xFFFF0F00); //Poner 0 en los bits de los pines 0, 1,3
    //(GPIOAMSEL) pag.687 habilitar analogico los pines de los canales
    GPIOB->AMSEL = (1<<4) ;// PIN B4
    GPIOE->AMSEL = (1<<2) | (1<<1); //PE1 PE2
    GPIOD->AMSEL = (1<<3) | (1<<1) | (1<<0); //PD3, PD1 y PD0
    //Pag 891 El registro (ADCPC) establece la velocidad de conversión por segundo
    //1Mms  SE pone 7 en binario porque trabaja con la max. velocidad que es 1 millon
    ADC0->PC = (1<<2)|(1<<1)|(1<<0);
    ADC1->PC = (1<<2)|(1<<1)|(1<<0);
    //Pag 841 configurar la prioridad de los secuenciadores 
    ADC0->SSPRI = 0x3102; //
    ADC1->SSPRI = 0x1023; //
    //Pag 821 control de la activación de los secuenciadores se desactivan los 4 secuenciadores 
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); 
    ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    //Pag 834 Este registro (ADCEMUX) selecciona el evento que activa la conversión 
    // Secuenciador 1 y 3 se le  da un valor de 4 para configurar con interrupciones
    ADC0->EMUX  = (0x0000); 
    ADC1->EMUX  = (0x0000); 
    //Pag 867 se definen entradas analógicas con el canal y secuenciador seleccionado,
    // Se establece que canal estará en cada mux por lo tanto en cada secuenciador y en cada modulo
    ADC0->SSMUX1 = 0x0246; // Los canales 1,2, 4 se definen para el mux 1 del secuenciador 1 en el modulo 0
    ADC1->SSMUX2 = 0x00A1; // Los canales 6,7 y 10  se definen para el mux 2 del secuecniador 3 en el modulo 1
    ADC1->SSMUX3 = 0x0007;
    //pag 868 Se configura el bit de control de muestreo y la interrupción
    // secuenciador 1        mux 0            mux 1            mux 2      es donde se establecen los canales en la configuración del SSMUX1 -> linea 70 
    ADC0->SSCTL1 = 0X00000644;// Para establecer configuración de muestro y finalizar muestro acorde a la pag. 868
    // secuenciador 3        mux 0            mux 1            mux 2  
    ADC1->SSCTL2 = 0X00000064;
    ADC1->SSCTL3 = 0X00000006;
    // Pag 825 Enable ADC Interrupt - Interrumpe la conversión del secuenciador a usar 
    // Al secuenciador 1 y 3  se le asigna un 1 para indicar que manda la señal pura al controlador de interrupciones
    ADC0->IM |= (1<<1); 
    ADC1->IM |= (1<<2);
    ADC1->IM |= (1<<3);
    //Pag 821 (ADCACTSS) Este registro controla la activación de los secuenciadores utilizados 
    // se habilita el secuanciador 1 y 3
    ADC0->ACTSS = (0<<3) | (0<<2) | (1<<1) | (0<<0);
    ADC1->ACTSS = (1<<3) | (1<<2) | (0<<1) | (0<<0); 
    //se inicializa el secuenciador 1 y 3   cuando se configura el adc en modo de procesador
    ADC0->PSSI |= (0<<1); 
    ADC1->PSSI |= (0<<2)|(0<<3); 

    ADC0->ISC = 2; 
    ADC1->ISC = 12;
}
//adquisición 
extern void ADC0_InSeq1(uint16_t Result[6])
{
       ADC0->PSSI = 0x00000002;
       while((ADC0->RIS&0x02)==0){}; // espera al convertidor
       Result[0] = ADC0->SSFIFO1&0xFFF; // leer  el resultado almacenado en la pila
       Result[1] = ADC0->SSFIFO1&0xFFF;
       Result[2] = ADC0->SSFIFO1&0xFFF;
       ADC0->ISC = 0x0002; 
}

extern void ADC1_InSeq2(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000004;
       while((ADC1->RIS&0x04)==0){}; // esperamos  el  convertidor
       Result[3] = ADC1->SSFIFO2&0xFFF; 
       Result[4] = ADC1->SSFIFO2&0xFFF; //en FIFO esta el valor de conversión
       ADC1->ISC = 0x0004;  //conversion finalizada, limpiamos la conversión
}

extern void ADC1_InSeq3(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000008;  //se habilita el modo de configuración y luego espera al convertidor
       while((ADC1->RIS&0x08)==0){}; 
       Result[5] = ADC1->SSFIFO3&0xFFF; 
       ADC1->ISC = 0x0008;
}

