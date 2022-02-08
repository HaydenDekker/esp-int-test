/*
 *
	This tests the GPIO interrupt latency using a HLI
	that directly sets the register

*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"

#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_0)
#define GPIO_INPUT_IO_0     17
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0)

#define INTR_NUM 31
#define CPU_CORE 1

volatile int intCounter = 0;
volatile int regVal = 0 ;

static void gpio_task_example(void* arg)
{
    // setup
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

	//interrupt of rising edge
	io_conf.intr_type = GPIO_INTR_POSEDGE;
	//bit mask of the pins, use GPIO4/5 here
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_down_en = 1;
	gpio_config(&io_conf);

	gpio_set_level(GPIO_OUTPUT_IO_0, 0);

	ESP_INTR_DISABLE(INTR_NUM);
	intr_matrix_set(CPU_CORE, ETS_GPIO_INTR_SOURCE, INTR_NUM);
	ESP_INTR_ENABLE(INTR_NUM);

	while(1){

		printf("Task waiting for interrupts, count is %i\n", intCounter);
		printf("GPIO Reg value = %X", regVal);
		vTaskDelay(200);

	}



}

void app_main(void)
{
    printf("This tests the interrupt latency, plug in a fungen to pin 17 and observe on pin 4!\n");

    xTaskCreatePinnedToCore(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL, 1);

}
