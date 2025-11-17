#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_SDA_PIN      21
#define I2C_SCL_PIN      22
#define I2C_PORT         I2C_NUM_0
#define I2C_FREQ_HZ      100000   // PCF8574 supports 100kHz

static const char *TAG = "PCF8574";

// ---------------------------------------------------
// Initialize I2C Bus
// ---------------------------------------------------
i2c_master_bus_handle_t init_i2c()
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_PORT,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };

    i2c_master_bus_handle_t bus_handle;
    esp_err_t ret= (i2c_new_master_bus(&bus_cfg, &bus_handle));
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C Bus Initialization Error: %s", esp_err_to_name(ret));
        return NULL;
    }
    else {      
    ESP_LOGI(TAG, "I2C Bus Initialized");
    }   
    return bus_handle;
}

// ---------------------------------------------------
// Add PCF8574 device
// ---------------------------------------------------
i2c_master_dev_handle_t pcf8574_add(i2c_master_bus_handle_t bus, uint8_t address)
{
    i2c_device_config_t dev_cfg = {
        .device_address = address,
        .scl_speed_hz = I2C_FREQ_HZ,
    };

    i2c_master_dev_handle_t dev_handle;
    esp_err_t ret= (i2c_master_bus_add_device(bus, &dev_cfg, &dev_handle));

    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "PCF8574 Add Device Error: %s", esp_err_to_name(ret));
        return NULL;
    }
    else {
    ESP_LOGI(TAG, "PCF8574 added at 0x%02X", address);
    
    }
    return dev_handle;
}

// ---------------------------------------------------
// Write 1 byte to PCF8574
// ---------------------------------------------------
void pcf8574_write(i2c_master_dev_handle_t dev, uint8_t value)
{
    // ESP_ERROR_CHECK(i2c_master_transmit(dev, &value, 1, 100));
    esp_err_t ret = i2c_master_transmit(dev, &value, 1, 100);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "PCF8574 Write Error: %s", esp_err_to_name(ret));
    }
    else {
    ESP_LOGI(TAG, "PCF8574 Write: 0x%02X", value);
    }
}

// ---------------------------------------------------
// Read 1 byte from PCF8574
// ---------------------------------------------------
uint8_t pcf8574_read(i2c_master_dev_handle_t dev)
{
    uint8_t data = 0;
    // ESP_ERROR_CHECK(i2c_master_receive(dev, &data, 1, 100));
    esp_err_t ret = i2c_master_receive(dev, &data, 1, 100);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "PCF8574 Read Error: %s", esp_err_to_name(ret));
        return 0;
    }
    else {
        ESP_LOGI(TAG, "PCF8574 Read: 0x%02X", data);
    return data;
    }
}

// ---------------------------------------------------
// Application Main
// ---------------------------------------------------
void app_main(void)
{
    ESP_LOGI(TAG, "Starting PCF8574 Example...");

    // 1. Initialize I2C Bus
    i2c_master_bus_handle_t bus = init_i2c();

    // 2. Add PCF8574 (change address if needed)
    // Default address = 0x20 → 0b0100 A2 A1 A0
    i2c_master_dev_handle_t pcf = pcf8574_add(bus, 0x26);

    // 3. Write to PCF8574
    // Example: Turn ON P0, P1, P2 → write LOW (0 means output LOW)
    pcf8574_write(pcf, 0b11111100);

    // 4. Read Inputs
    while (1) {
        uint8_t input = pcf8574_read(pcf);
        printf("PCF Input Pins = 0x%02X\n", input);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
