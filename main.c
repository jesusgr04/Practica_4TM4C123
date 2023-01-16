#include "lib/include.h"
int main(void)
{
    char valor1[4]; //Para enviar los valores del ADC en carácteres, es de 4 porque son cuatro digitos de 4096 
    char valor2[4];
    char valor3[4];
    char valor4[4];
    char valor5[4];
    char valor6[4];

    Configurar_PLL(_20MHZ);  // velocidad de reloj
    Configura_Reg_ADC();
    Configurar_UART0();
    Configurar_GPIO();

    while(1)
    {   
    }
}

