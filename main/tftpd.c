#include "tftpd.h"
#include "vfs.h"

static const char *TAG = "tftpd";

static FILE *fd;
struct tftp_context tftpd = {0};
static int file_size = 0;
static char *base_path = BASE_PATH;

void* tftp_open(const char* fname, const char* mode, u8_t write)
{
    ESP_LOGD(TAG, "Get file name '%s', RW:%d, mode %s" , fname , write, mode);

    char path[256];
    sprintf(path, "%s/%s", base_path, fname);

    fd = fopen(path, write ? "wb+":"rb");
    if (fd == NULL) {
        ESP_LOGE(TAG, "Failed to open file for '%s'", write?"wb+":"rb");
        return 0;
    }
    file_size = 0;
    return (void *)fd;
}

void tftp_close(void* handle)
{
    ESP_LOGD(TAG, "file size: %d", file_size );
    fclose(handle);
}

int tftp_read(void* handle, void* buf, int bytes){
    size_t written = fread(buf, sizeof(char), bytes, handle);
    file_size += written;
    return written;
}

int tftp_write(void* handle, struct pbuf* p)
{
    size_t written = fwrite(p->payload, sizeof(char), p->len, handle);
    file_size += written;
    return written;
}

err_t init_tftpd(){
    ESP_LOGI(TAG, "Init tftpd");

    tftpd.open   = tftp_open;
    tftpd.close  = tftp_close;
    tftpd.read   = tftp_read;
    tftpd.write  = tftp_write;

    err_t err = tftp_init(&tftpd);
    if (err != ERR_OK) {
        ESP_LOGE(TAG, "Failed init tftp (%s)", esp_err_to_name(err));
    }

    return err;
}