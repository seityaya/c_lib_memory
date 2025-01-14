// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2025 Seityagiya Terlekchi. All rights reserved.

#define YAYA_MEMORY_STATS_GLOBAL 1
#include "yaya_memory_wrapper.h"

void test_param_stats_global(void) {
    printf("test_param_stats_global\n");

    if (memory_stats_init()) {
        printf("00 OK\n");
    } else {
        printf("00 EROOR\n");
    }

    if (!memory_req(NULL, 0, 0)) {
        printf("01 OK\n");
    } else {
        printf("01 EROOR\n");
    }

    void *ptr = NULL;

    if (!memory_req(&ptr, 0, 0)) {
        printf("02 OK\n");
    } else {
        printf("02 EROOR\n");
    }

    if (!memory_req(&ptr, 0, 0)) {
        printf("03 OK\n");
    } else {
        printf("03 EROOR\n");
    }

    if (!memory_req(NULL, 1, sizeof(char))) {
        printf("04 OK\n");
    } else {
        printf("04 EROOR\n");
    }

    if (!memory_req(NULL, 1, sizeof(char))) {
        printf("05 OK\n");
    } else {
        printf("05 EROOR\n");
    }

    if (!memory_req(ptr, 1, sizeof(char))) {
        printf("06 OK\n");
    } else {
        printf("06 EROOR\n");
    }

    if (!memory_ret(ptr)) {
        printf("07 OK\n");
    } else {
        printf("07 EROOR\n");
    }

    if (!memory_ret(&ptr)) {
        printf("08 OK\n");
    } else {
        printf("08 EROOR\n");
    }

    if (memory_req(&ptr, 1, sizeof(char))) {
        printf("09 OK\n");
    } else {
        printf("09 EROOR\n");
    }

    if (memory_ret(&ptr)) {
        printf("10 OK\n");
    } else {
        printf("10 EROOR\n");
    }

    if (!memory_ret(&ptr)) {
        printf("11 OK\n");
    } else {
        printf("11 EROOR\n");
    }

    if (ptr == NULL) {
        printf("12 OK\n");
    } else {
        printf("12 EROOR\n");
    }

    if (memory_stats_out(stdout)) {
        printf("13 OK\n");
    } else {
        printf("13 EROOR\n");
    }

    if (memory_stats_free()) {
        printf("14 OK\n");
    } else {
        printf("14 EROOR\n");
    }

    printf("\n");
    printf("\n");
    fflush(stdout);
}
