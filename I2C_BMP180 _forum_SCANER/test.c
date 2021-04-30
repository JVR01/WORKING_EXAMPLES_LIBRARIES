#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

static int bme280_addr = 0x76;
static uint8_t bme280_id_reg = 0xd0;

#define I2C_PORT i2c0
// cpp library https://github.com/pimoroni/pimoroni-pico/blob/patch-bme280-breakouts-dev/drivers/bme280/bme280.cpp

static int read_byte(int addr, uint8_t reg, uint8_t *byte) {
  int rc = i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
  if (rc != 1) {
    printf("i2c write error (%d)\n", rc);
    return rc;
  }

  rc = i2c_read_blocking(I2C_PORT, addr, byte, 1, false);
  if (rc != 1) {
    printf("i2c read error (%d)\n", rc);
    return rc;
  }

  return 0;
}

int main() {
  stdio_init_all();

  // I2C0 on GPIO4 (SDA) and GPIO5 (SCL) running at 400kHz.
  i2c_init(I2C_PORT, 100 * 1000);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);

  while (1) {
    uint8_t bme280_id;
    int rc = read_byte(bme280_addr, bme280_id_reg, &bme280_id);
    if (rc == 0) {
      printf("BME280 ID: 0x%x\n", bme280_id);
    }

    sleep_ms(1000);
  }

  return 0;
}
