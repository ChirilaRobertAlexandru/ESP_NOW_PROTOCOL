
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "ESP32_MAC";

void app_main(void)
{
    // Initializare memorie NVS
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    // Initializare interfata de retea
    ESP_ERROR_CHECK(esp_netif_init());

    // Initializare sistem de evenimente
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initializare Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Activare mod Station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Pornire Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());

    // Citire adresa MAC
    uint8_t mac[6];

    ESP_ERROR_CHECK(
        esp_wifi_get_mac(WIFI_IF_STA, mac)
    );

    // Afisare adresa MAC
    ESP_LOGI(TAG, "Adresa MAC ESP32:");

    ESP_LOGI(TAG,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2],
        mac[3], mac[4], mac[5]
    );

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}