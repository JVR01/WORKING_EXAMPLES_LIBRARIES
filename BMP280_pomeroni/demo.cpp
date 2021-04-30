#include "pico/stdlib.h"
#include "stdio.h"

#include "bme280.hpp"

using namespace pimoroni;
BME280 bme280;

int main() {
  //setup_default_uart();
  stdio_init_all();
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  if(!bme280.init()) {//default SDA, SCL -> 4, 5 GPIO
    while(true) {
    printf("BME280 Failed to init!\n");
    sleep_ms(200);
    //return 1;
    }
  }

  while(true) {
    bme280.update_sensor();

    float temperature = bme280.get_temperature();
    float pressure = bme280.get_pressure();
    float humidity = bme280.get_humidity();

    printf("Temperature: %f\n", temperature);
    printf("Pressure: %f\n", pressure);
    printf("Humidity: %f\n", humidity);

    gpio_put(PICO_DEFAULT_LED_PIN, true);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    sleep_ms(500);
  }

  return 0;
}
