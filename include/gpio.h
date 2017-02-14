/* libgpio
 *
 * Copyright 2012 Manuel Traut <544088192@qq.com>
 *
 * LGPL licensed
 */

#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))

typedef enum _gpio_direction {
	GPIO_IN,
	GPIO_OUT,
} gpio_direction_e;

typedef enum _gpio_value {
	GPIO_LOW = 0,
	GPIO_HIGH = 1,
} gpio_value_e;

typedef enum _gpio_irq_mode {
	GPIO_NONE,
	GPIO_RISING,
	GPIO_FALLING,
	GPIO_BOTH,
} gpio_irq_mode_e;
/* lorawan use */
/**
 * @brief 设置gpio口为输入模式
 *
 * @param gpio
 *
 * @return 
 */
int gpio_direction_input(unsigned int gpio);
/**
 * @brief 设置gpio口为输出模式
 *
 * @param gpio
 * @param value
 *
 * @return 
 */
int gpio_direction_output(unsigned int gpio, int value);

/**
 * @brief 设置gpio口的值
 *
 * @param pin
 * @param value
 *
 * @return 
 */
int gpio_set_value (unsigned int gpio, int value);
/**
 * @brief  获取gpio的值
 *
 * @param gpio
 *
 * @return 
 */
int gpio_get_value (unsigned int gpio);

/**
 * @brief 导出gpio到用户空间
 *
 * @param gpio
 *
 * @return 
 */
int gpio_export(unsigned gpio);
/**
 * @brief 撤销导出到用户空间的gpio
 */
void gpio_unexport(unsigned gpio); 
/**
 * @brief 
 *
 * @param gpio
 * @param m
 *
 * @return 
 */
int gpio_enable_irq (unsigned int gpio, gpio_irq_mode_e m);
/**
 * @brief 
 *
 * @param gpio
 * @param value
 *
 * @return 
 */
int gpio_irq_wait (unsigned int gpio, int *value);
/**
 * @brief 
 *
 * @param pin
 * @param value
 * @param timeout_ms
 *
 * @return 
 */
int gpio_irq_timed_wait (unsigned int gpio, int *value, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif
