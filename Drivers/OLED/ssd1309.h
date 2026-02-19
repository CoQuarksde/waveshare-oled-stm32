#ifndef SSD1309_H
#define SSD1309_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file ssd1309.h
 * @brief Minimal SSD1309 OLED driver interface for STM32 HAL (SPI)
 *
 * This module provides a minimal hardware abstraction layer for controlling
 * a SSD1309-based 128x64 OLED display over SPI using STM32 HAL.
 *
 * Target hardware:
 * Waveshare 1.51" Transparent OLED (SSD1309 compatible controller)
 *
 * Communication:
 * - SPI (MOSI, SCK)
 * - GPIO: CS, DC, RST
 *
 * Design constraints:
 * - No dynamic memory allocation
 * - No RTOS dependency
 * - Static framebuffer (1 KB)
 */

#include "stm32g4xx_hal.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @def SSD1309_WIDTH
 * @brief SSD1309 display width in pixels
 */
#define SSD1309_WIDTH   128

/**
 * @def SSD1309_HEIGHT
 * @brief SSD1309 display height in pixels
 */
#define SSD1309_HEIGHT  64

/**
 * @def SSD1309_FB_SIZE
 * @brief SSD1309 framebuffer size in bytes
 *
 * Calculation:
 * 128 x 64 pixels / 8 bits per byte = 1024 bytes
 */
#define SSD1309_FB_SIZE (SSD1309_WIDTH * SSD1309_HEIGHT / 8)

/**
 * @struct ssd1309_t
 * @brief SSD1309 driver context structure
 *
 * Holds hardware interface handles and internal framebuffer.
 */
typedef struct
{
    /**
     * @brief Pointer to SPI HAL handle used for communication
     */
    SPI_HandleTypeDef *hspi;

    /**
     * @brief Chip Select (CS) GPIO port and pin
     */
    GPIO_TypeDef *cs_port;
    uint16_t      cs_pin;

    /**
     * @brief Data/Command (DC) GPIO port and pin
     */
    GPIO_TypeDef *dc_port;
    uint16_t      dc_pin;

    /**
     * @brief Reset (RST) GPIO port and pin
     */
    GPIO_TypeDef *rst_port;
    uint16_t      rst_pin;

    /**
     * @brief Internal framebuffer (page addressing mode)
     *
     * Size: 1024 bytes
     * Layout: Horizontal addressing, 8 vertical pixels per byte
     */
    uint8_t framebuffer[SSD1309_FB_SIZE];

} ssd1309_t;

/**
 * @brief Initialize SSD1309 display driver context
 *
 * Initializes internal driver state and prepares the display
 * for command/data transmission over SPI.
 *
 * @param dev Pointer to SSD1309 driver context
 * @return HAL status code
 */
HAL_StatusTypeDef ssd1309_init(ssd1309_t *dev);

/**
 * @brief Perform hardware reset of SSD1309 controller
 *
 * Toggles the RST line according to SSD1309 timing requirements.
 *
 * @param dev Pointer to SSD1309 driver context
 */
void ssd1309_reset(ssd1309_t *dev);

/**
 * @brief Send single command byte to SSD1309
 *
 * DC line is driven LOW during command transmission.
 *
 * @param dev Pointer to SSD1309 driver context
 * @param cmd Command byte
 * @return HAL status code
 */
HAL_StatusTypeDef ssd1309_write_cmd(ssd1309_t *dev, uint8_t cmd);

/**
 * @brief Send data buffer to SSD1309
 *
 * DC line is driven HIGH during data transmission.
 *
 * @param dev Pointer to SSD1309 driver context
 * @param data Pointer to data buffer
 * @param size Number of bytes to transmit
 * @return HAL status code
 */
HAL_StatusTypeDef ssd1309_write_data(ssd1309_t *dev, const uint8_t *data, size_t size);

/**
 * @brief Update OLED display from internal framebuffer
 *
 * Transfers full framebuffer content (1024 bytes)
 * to display RAM via SPI.
 *
 * @param dev Pointer to SSD1309 driver context
 * @return HAL status code
 */
HAL_StatusTypeDef ssd1309_update(ssd1309_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* SSD1309_H */
