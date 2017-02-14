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
#define LORA_GPIO_TEST2  GPIO_TO_PIN(3,14)//PD14
#define LORA_GPIO_TEST3  GPIO_TO_PIN(3,13)//PD13

#define LORA_GPIO_LED1  GPIO_TO_PIN(1,4)//PB4
#define LORA_GPIO_LED2  GPIO_TO_PIN(9,2)//PJ2
#define LORA_GPIO_LED3  GPIO_TO_PIN(9,4)//PJ4

#define LORA_RF_RESET  GPIO_TO_PIN(1,5)//PB5


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

    gpio_enable_irq(LORA_GPIO_TEST3, GPIO_FALLING);
    gpio_irq_timed_wait(LORA_GPIO_TEST3, &value, 1000);
    printf("%d\n", value);
	return ret;
err:
    return -1;
}
