idf_component_register(SRCS "tftpd.c" "vfs.c" "main.c"
                    $ENV{IDF_PATH}/components/lwip/lwip/src/apps/tftp/tftp_server.c
                    INCLUDE_DIRS ".")
