#pragma once

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"

#include "lwip/apps/tftp_server.h"

err_t init_tftpd();
