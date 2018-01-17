/* libgpio
 *
 * Copyright 2012 Manuel Traut <544088192@qq.com>, Yu! <1036164431@qq.com>
 *
 * LGPL licensed
 */

#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))

typedef void (*GPIO_HANDLER)(void *arg);

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

typedef struct _gpio_irq_info {
	unsigned int gpio;
    gpio_irq_mode_e m;
    int fd;
    GPIO_HANDLER hander;
    int flag;
} gpio_irq_info;

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
 * @brief 注册gpio中断(注册前应取消使能gpio中断，注册后使能gpio中断)
 *
 * @param pin
 * @param irq_mode
 * @param irq_hander
 *
 * @return 
 */
int gpio_irq_register(unsigned int gpio, gpio_irq_mode_e m, GPIO_HANDLER hander);

/**
 * @brief 撤销注册gpio中断
 *
 * @param pin
 *
 * @return 
 */
int gpio_irq_unregister(unsigned int gpio);

/**
 * @brief 使能gpio中断
 */
int gpio_irq_enable (void);

/**
 * @brief 撤销使能gpio中断
 */
int gpio_irq_disable (void);

#ifdef __cplusplus
}
#endif

#endif
