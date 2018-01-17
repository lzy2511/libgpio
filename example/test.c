/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, jz.
 *       Filename:  test.c
 *
 *    Description:  
 *         Others:
 *
 *        Version:  1.0
 *        Created:  2017-01-15 14:24:38
 *       Revision:  none
 *       Compiler:  arm-linux-gcc
 *
 *         Author:  Joy (Joy), 
 *   Organization:  jz
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <gpio.h>

#define LORA_GPIO_TEST1  GPIO_TO_PIN(3,15)//PD15  
#define LORA_GPIO_TEST2  GPIO_TO_PIN(8,14)//PI14
#define LORA_GPIO_TEST3  GPIO_TO_PIN(8,15)//PI15 

#define LORA_GPIO_LED1  GPIO_TO_PIN(1,4)//PB4
#define LORA_GPIO_LED2  GPIO_TO_PIN(9,2)//PJ2
#define LORA_GPIO_LED3  GPIO_TO_PIN(9,4)//PJ4

#define LORA_RF_RESET  GPIO_TO_PIN(1,5)//PB5

void GPIO_Hander1(void *arg)
{
    printf("irq: pin%d!\n", *((int *)arg));
}

int main (int argc, char **argv)
{
    int ret = 0;
    int value = -1;
    
    if (gpio_export(LORA_GPIO_TEST1) < 0)
    {
        goto err;
    }
    if (gpio_export(LORA_GPIO_TEST2) < 0)
    {
        goto err;
    }
    if (gpio_export(LORA_GPIO_TEST3) < 0)
    {
        goto err;
    }

    gpio_direction_output(LORA_GPIO_TEST1, GPIO_LOW);
    gpio_direction_input(LORA_GPIO_TEST2);
    gpio_set_value(LORA_GPIO_TEST1, GPIO_HIGH);
    value = gpio_get_value(LORA_GPIO_TEST2);

    if (gpio_export(LORA_GPIO_TEST2) < 0)
    {
        goto err;
    }

    if (gpio_export(LORA_GPIO_TEST3) < 0)
    {
        goto err;
    }

    while(1)
    {
        printf("\n\n\n");
        printf("-------test irq pin%d:\n", LORA_GPIO_TEST2);
        gpio_irq_disable();
        gpio_irq_register(LORA_GPIO_TEST2, GPIO_BOTH, GPIO_Hander1);
        gpio_irq_enable();
        
        sleep(5);

        printf("\n\n\n");
        printf("-------test irq pin%d:\n", LORA_GPIO_TEST3);
        gpio_irq_disable();
        gpio_irq_register(LORA_GPIO_TEST3, GPIO_BOTH, GPIO_Hander1);
        gpio_irq_enable();
        
        sleep(5);

        printf("\n\n\n");
        printf("-------test irq pin%d and irq pin%d:\n", LORA_GPIO_TEST2, LORA_GPIO_TEST3);
        gpio_irq_disable();
        gpio_irq_register(LORA_GPIO_TEST2, GPIO_BOTH, GPIO_Hander1);
        gpio_irq_register(LORA_GPIO_TEST3, GPIO_BOTH, GPIO_Hander1);
        gpio_irq_enable();
        
        sleep(5);
    }
	return ret;
err:
    return -1;
}
