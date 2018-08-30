#ifndef _OLED_H_
#define _OLED_H_

#define OLED_I2C                         I2C2
#define OLED_I2C_CLK                     RCC_APB1Periph_I2C2

#define OLED_I2C_SCL_PIN                 GPIO_Pin_13                  /* PB.13 */
#define OLED_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define OLED_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define OLED_I2C_SCL_SOURCE              GPIO_PinSource13
#define OLED_I2C_SCL_AF                  GPIO_AF_5

#define OLED_I2C_SDA_PIN                 GPIO_Pin_14                  /* PB.14 */
#define OLED_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define OLED_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define OLED_I2C_SDA_SOURCE              GPIO_PinSource14
#define OLED_I2C_SDA_AF                  GPIO_AF_5


#define OLED_I2C_ADDR  	0x78

#define OLED_I2C_TIMING     0x20303E5D
#endif