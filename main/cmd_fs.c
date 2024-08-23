#include <stdio.h>
#include <string.h>
#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include <dirent.h>
#include <sys/errno.h>

#include "vfs.h"

static const char *TAG = "fs";

static int do_ls_cmd(int argc, char **argv)
{
    int ret;
    DIR *dir;
    struct dirent *d;
    char *file_path;

    ret = asprintf(&file_path, "%s", BASE_PATH);

    if (ret < 0) {
        ESP_LOGE(TAG, "failed to asprintf errno=%d", errno);
        return -1;
    }

    dir = opendir(file_path);
    if (!dir) {
        ESP_LOGE(TAG, "failed to opendir %s errno=%d", file_path, errno);
        free(file_path);
        return -1;
    }

    do {
        d = readdir(dir);
        if (!d) {
            break;
        }
        printf("/%s\n",  d->d_name);

    } while (1);

    closedir(dir);
    free(file_path);

    return 0;
}


static struct {
    struct arg_str *file;
    struct arg_end *end;
} del_args;

static int do_del_cmd(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **)&del_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, del_args.end, argv[0]);
        return 1;
    }

    ESP_LOGI(TAG, "try del %s", del_args.file->sval[0]);

    char *file_path;
    int ret = asprintf(&file_path, "%s/%s", BASE_PATH, del_args.file->sval[0]);

    remove(file_path);
    free(file_path);
    return 0;
}

void register_cmd_fs(void)
{
    const esp_console_cmd_t ls_cmd = {
        .command = "ls",
        .help = "List directory contents",
        .hint = NULL,
        .func = &do_ls_cmd,
        .argtable = NULL
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&ls_cmd));


    del_args.file = arg_str1(NULL, NULL, "<filename>", "wth");
    del_args.end = arg_end(1);

    const esp_console_cmd_t del_cmd = {
        .command = "del",
        .help = "del file",
        .hint = NULL,
        .func = &do_del_cmd,
        .argtable = &del_args
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&del_cmd));

}
