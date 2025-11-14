#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO           22
#define I2C_MASTER_SDA_IO           19
#define I2C_MASTER_PORT             I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000

static const char *TAG = "I2C_SCAN";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting I2C Scanner...");

    // 1. Create config
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };

    i2c_master_bus_handle_t bus_handle;

    // 2. Create I2C bus
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    // 3. Scan all addresses
    for (int addr = 1; addr < 127; addr++) {
        i2c_master_dev_handle_t dev_handle;
        i2c_device_config_t dev_cfg = {
            .device_address = addr,
            .scl_speed_hz = I2C_MASTER_FREQ_HZ,
        };

        esp_err_t add_result = i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);

        if (add_result == ESP_OK) {
            esp_err_t ret = i2c_master_probe(bus_handle, addr, 1000);

            if (ret == ESP_OK) {
                ESP_LOGI(TAG, "I2C device found at 0x%02X", addr);
            }

            i2c_master_bus_rm_device(dev_handle);
        }
    }

    ESP_LOGI(TAG, "Scan Completed.");
}
