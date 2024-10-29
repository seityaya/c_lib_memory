// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory.h"

void test_param_stats_local() {
    printf("test_param_stats_local\n");

    memory_stats_t* memory_stats = NULL;

    if (memory_stats_init(&memory_stats)) {
        printf("00 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (memory_req_s(memory_stats, NULL, 0, 0)) {
        printf("01 OK\n");
    } else {
        printf("EROOR\n");
    }

    void* ptr = NULL;

    if (!memory_req_s(memory_stats, &ptr, 0, 0)) {
        printf("02 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_req_s(memory_stats, &ptr, 0, 0)) {
        printf("03 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_req_s(memory_stats, NULL, 1, sizeof(char))) {
        printf("04 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_req_s(memory_stats, NULL, 1, sizeof(char))) {
        printf("05 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_req_s(memory_stats, ptr, 1, sizeof(char))) {
        printf("06 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_ret_s(memory_stats, ptr)) {
        printf("07 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_ret_s(memory_stats, &ptr)) {
        printf("08 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (memory_req_s(memory_stats, &ptr, 1, sizeof(char))) {
        printf("09 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (memory_ret_s(memory_stats, &ptr)) {
        printf("10 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (!memory_ret_s(memory_stats, &ptr)) {
        printf("11 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (ptr == NULL) {
        printf("12 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (memory_stats_out(memory_stats, stdout)) {
        printf("13 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (memory_stats_free(&memory_stats)) {
        printf("14 OK\n");
    } else {
        printf("EROOR\n");
    }

    if (memory_stats == NULL) {
        printf("15 OK\n");
    } else {
        printf("EROOR\n");
    }

    printf("\n");
    printf("\n");
    fflush(stdout);
}
