/*
 * Copyright 2022 Emiliano Gonzalez (egonzalez . hiperion @ gmail . com))
 * * Project Site: https://github.com/hiperiondev/ladder-parser *
 *
 * This is based on other projects *
 *     Wren Programming Language: https://github.com/wren-lang/wren
 *     Espressif IoT Development Framework: https://github.com/espressif/esp-idf
 *
 *     please contact their authors for more information.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include <esp_timer.h>

#include "wren.h"

static void writeFn(WrenVM *vm, const char *text) {
    printf("%s", text);
}

void errorFn(WrenVM *vm, WrenErrorType errorType, const char *module, const int line, const char *msg) {
    switch (errorType) {
        case WREN_ERROR_COMPILE: {
            printf("[%s line %d] [Error] %s\n", module, line, msg);
        }
            break;
        case WREN_ERROR_STACK_TRACE: {
            printf("[%s line %d] in %s\n", module, line, msg);
        }
            break;
        case WREN_ERROR_RUNTIME: {
            printf("[Runtime Error] %s\n", msg);
        }
            break;
    }
}

void wren_task(void *pvParameter) {
    char ptrTaskList[250];

    while (1) {
        WrenConfiguration config;
        wrenInitConfiguration(&config);
        config.writeFn = &writeFn;
        config.errorFn = &errorFn;
        WrenVM *vm = wrenNewVM(&config);

        const char *module = "main";
        const char *script = "System.print(\"I am running in a VM!\")";

        WrenInterpretResult result = wrenInterpret(vm, module, script);

        switch (result) {
            case WREN_RESULT_COMPILE_ERROR: {
                printf("Compile Error!\n");
            }
                break;
            case WREN_RESULT_RUNTIME_ERROR: {
                printf("Runtime Error!\n");
            }
                break;
            case WREN_RESULT_SUCCESS: {
                printf("Success!\n\n");
            }
                break;
        }



        vTaskList(ptrTaskList);
        printf("**************************************************\n");
        printf("Task          State    Prio    Stack   Num\n");
        printf("**************************************************\n");
        printf("%s", ptrTaskList);
        printf("**************************************************\n");
        printf("esp_reset_reason: %d\n", esp_reset_reason());
        printf("esp_get_free_heap_size: %d\n", esp_get_free_heap_size());
        printf("esp_get_minimum_free_heap_size: %d\n", esp_get_minimum_free_heap_size());
        printf("**************************************************\n\n\n");

        printf("wrenFreeVM\n\n");
        wrenFreeVM(vm);

        vTaskList(ptrTaskList);
        printf("**************************************************\n");
        printf("Task          State    Prio    Stack   Num\n");
        printf("**************************************************\n");
        printf("%s", ptrTaskList);
        printf("**************************************************\n");
        printf("esp_reset_reason: %d\n", esp_reset_reason());
        printf("esp_get_free_heap_size: %d\n", esp_get_free_heap_size());
        printf("esp_get_minimum_free_heap_size: %d\n", esp_get_minimum_free_heap_size());
        printf("**************************************************\n\n\n");

        vTaskDelay(3000 / portTICK_RATE_MS);
    }
}

void app_main(void) {
    xTaskCreatePinnedToCore(
            wren_task,
            "WREN Task",
            32000,
            NULL,
            10,
            NULL,
            1
            );
}
