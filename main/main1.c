#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

// Define the GPIO pin to blink
#define BLINK_GPIO 12

void app_main(void)
{
// Reset the pin to a known state
    gpio_reset_pin(BLINK_GPIO);
    
    // Set the GPIO pin as a push/pull output
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
// Infinite loop
    while(1) {
        /* Set GPIO level low (turn LED off) */
        gpio_set_level(BLINK_GPIO, 0);
        
        // Wait for 1000 milliseconds (1 second)
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        /* Set GPIO level high (turn LED on) */
        gpio_set_level(BLINK_GPIO, 1);

        // Wait for 1000 milliseconds (1 second)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}