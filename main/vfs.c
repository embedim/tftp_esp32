#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "sdkconfig.h"
#include "vfs.h"

static const char *TAG = "vfs";
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

void init_mount_fs(){
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
        .use_one_fat = false,
    };

    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(BASE_PATH, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }

    size_t total = 0, free_bytes = 0;
    esp_vfs_fat_info(BASE_PATH, &total, &free_bytes);
    ESP_LOGI(TAG, "FAT FS: total: %d, free: %d", total, free_bytes);

}

