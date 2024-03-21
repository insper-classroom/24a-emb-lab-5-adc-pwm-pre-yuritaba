#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pwm.h"

const int PWM_0_PIN = 4;
const int PWM_1_PIN = 6; // Assumindo que o pino do LED G é 5, ajuste conforme necessário

void led_task(void *p) {
    // Configuração para o LED R
    gpio_set_function(PWM_0_PIN, GPIO_FUNC_PWM);
    uint slice_num_r = pwm_gpio_to_slice_num(PWM_0_PIN);
    pwm_set_clkdiv(slice_num_r, 125);
    pwm_set_wrap(slice_num_r, 100);
    pwm_set_chan_level(slice_num_r, PWM_CHAN_A, 80);
    pwm_set_enabled(slice_num_r, true);

    // Configuração para o LED G
    gpio_set_function(PWM_1_PIN, GPIO_FUNC_PWM);
    uint slice_num_g = pwm_gpio_to_slice_num(PWM_1_PIN);
    // Assume que o LED G está no mesmo slice e usa o mesmo clkdiv e wrap
    pwm_set_chan_level(slice_num_g, PWM_CHAN_B, 20); // Duty cycle de 20%
    pwm_set_enabled(slice_num_g, true);

    while (true) {
    }
}

int main() {
    stdio_init_all();
    printf("Start RTOS \n");

    xTaskCreate(led_task, "LED_Task 1", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
