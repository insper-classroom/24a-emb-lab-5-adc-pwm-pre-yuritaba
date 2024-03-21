#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#define WINDOW_SIZE 5 // Tamanho da janela para a média móvel

void process_task(void *p) {
    int window[WINDOW_SIZE] = {0}; // Janela para armazenar as últimas 5 amostras
    int sum = 0; // Soma das amostras na janela
    int data = 0; // Dado atual recebido da fila
    int index = 0; // Índice para inserir a próxima amostra na janela

    while (true) {
        if (xQueueReceive(xQueueData, &data, 100)) {
            sum -= window[index]; // Subtrai o valor mais antigo da soma
            window[index] = data; // Atualiza a janela com o novo valor
            sum += data; // Adiciona o novo valor à soma

            // Calcula a média móvel e imprime o resultado
            int moving_average = sum / WINDOW_SIZE;
            printf("%d\n", moving_average);

            index = (index + 1) % WINDOW_SIZE; // Atualiza o índice para o próximo valor, rotacionando a janela

            // Deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
