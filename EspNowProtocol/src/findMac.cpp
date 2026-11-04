
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "ESP32_MAC";

extern "C" void app_main(void)
{
    // 1. Initializare NVS
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    // 2. Initializare interfata de retea
    ESP_ERROR_CHECK(esp_netif_init());

    // 3. Initializare event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 4. Initializare Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 5. Configurare Station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // 6. Pornire Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());

    // 7. Citire MAC
    uint8_t mac[6];

    ESP_ERROR_CHECK(
        esp_wifi_get_mac(WIFI_IF_STA, mac)
    );

    // 8. Afisare MAC
    ESP_LOGI(TAG, "Adresa MAC este:");

    ESP_LOGI(TAG,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2],
        mac[3], mac[4], mac[5]
    );

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
