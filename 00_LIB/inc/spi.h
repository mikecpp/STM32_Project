#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f7xx_hal.h"
#include "stm32f723_system.h"

typedef struct {
    int32_t (*spi_init)(uint8_t id);
    int32_t (*spi_write_read)(uint8_t id, uint8_t *pWrite, uint8_t *pRead, uint16_t len);
    int32_t (*spi_write)(uint8_t id, uint8_t *pData, uint16_t len);
} SPI_Driver_T; 

typedef enum {
    SPI_0,
    SPI_1,
    SPI_2
} SPI_ID;

int32_t spi_register_driver(SPI_Driver_T *spi_driver);
int32_t spi_init(uint8_t id);
int32_t spi_write_read(uint8_t id, uint8_t *pWrite, uint8_t *pRead, uint16_t len);
int32_t spi_write(uint8_t id, uint8_t *pData, uint16_t len);

#endif//__SPI_H__
