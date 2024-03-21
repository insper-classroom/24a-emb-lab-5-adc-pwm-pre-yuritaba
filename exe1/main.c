#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/adc.h"

void adc_1_task(void *p) {
    adc_init();
    adc_gpio_init(27); // Inicializa o GPIO 27 para ADC
    adc_gpio_init(26); // Inicializa o GPIO 26 para ADC

    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    const float conversion_factor = 3.3f / (1 << 12);

    uint16_t result;
    while (1) {
        adc_select_input(1); // Seleciona a entrada do ADC 1 (GPIO27)
        result = adc_read();
        printf("voltage 1: %f V\n", result * conversion_factor);

        // Configuração para o ADC 0 (GP26)
        adc_select_input(0); // Seleciona a entrada do ADC 0 (GPIO26)
        result = adc_read(); // Lê o valor do ADC 0
        printf("voltage 2: %f V\n", result * conversion_factor); // Imprime o valor do ADC 0

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

int main() {
    stdio_init_all();
    printf("Start RTOS \n");
    adc_init();

    xTaskCreate(adc_1_task, "LED_Task 1", 4095, NULL, 1, NULL);
    vTaskStartScheduler();

    while (true) {
    }
}
