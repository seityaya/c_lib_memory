// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2025 Seityagiya Terlekchi. All rights reserved.

#include "inttypes.h"
#include "malloc.h"
#include "stdint.h"

#if !defined(YAYA_MEMORY_STATS_LOCAL) && !defined(YAYA_MEMORY_STATS_GLOBAL)
#define YAYA_MEMORY_STATS_GLOBAL 1
#endif

#include "yaya_memory_stats.h"

/*==================================================================================================================================================*/

#undef memory_req
#undef memory_ret

#undef memory_stats_init
#undef memory_stats_free
#undef memory_stats_out

/*==================================================================================================================================================*/

#if YAYA_MEMORY_STATS_GLOBAL == 1
memory_stats_t memory_stats_global = {0};
#endif

bool memory_stats_out(memory_stats_t *mem_stats, FILE *out) {
    /*Проверка, что указатель не NULL*/
    if (mem_stats == NULL) {
        return false;
    }

    /*Проверка, что указатель не NULL*/
    if (out == NULL) {
        return false;
    }

    fprintf(out, "Request :% 10" PRIiMAX "; NEW  :% 10" PRIiMAX ";\n", mem_stats->memory_request, mem_stats->memory_call_new);
    fprintf(out, "Produce :% 10" PRIiMAX "; RES  :% 10" PRIiMAX ";\n", mem_stats->memory_produce, mem_stats->memory_call_res);
    fprintf(out, "Overhead:% 10" PRIiMAX "; DEL  :% 10" PRIiMAX ";\n", mem_stats->memory_produce - mem_stats->memory_request, mem_stats->memory_call_del);
    fprintf(out, "Release :% 10" PRIiMAX "; USAGE:% 10" PRIiMAX ";\n", mem_stats->memory_produce - mem_stats->memory_release, mem_stats->memory_release);

    fflush(out);
    return true;
}

bool memory_stats_init(memory_stats_t **mem_stats) {
    /*Проверка, что указатель не NULL*/
    if (mem_stats == NULL) {
        return false;
    }
    /*Проверка, что указатель никуда не указывает*/
    if (*mem_stats != NULL) {
        return false;
    }

    *mem_stats = (memory_stats_t *)calloc(1, sizeof(memory_stats_t));
    if (*mem_stats == NULL) {
        return false;
    }

    return true;
}

bool memory_stats_free(memory_stats_t **mem_stats) {
    /*Проверка, что указатель не NULL*/
    if (mem_stats == NULL) {
        return false;
    }
    /*Проверка, что указатель куда-то указывает*/
    if (*mem_stats == NULL) {
        return false;
    }

    free(*mem_stats);
    *mem_stats = NULL;
    return true;
}

/*==================================================================================================================================================*/

bool memory_req_s(memory_stats_t *mem_stats, void **ptr, size_t count, size_t size) {
    /*Проверка, что указатели не NULL*/
    if (ptr == NULL) {
        return false;
    }

    bool new = true;
    if (*ptr != NULL) {
        new = false;
    }

    if (memory_req(ptr, count, size)) {
        /*Указатели под структуру памяти*/
        memory_t *mem = NULL;

        /*Помещаем указатель со смещением*/
        mem = (memory_t *)((char *)(*ptr) - offsetof(memory_t, memory_ptr));

        /*Сохранение статистики*/
        if (mem_stats != NULL) {
            if (new) {
                mem_stats->memory_call_new++;
            } else {
                mem_stats->memory_call_res++;
            }
            mem_stats->memory_request += mem->memory_request;
            mem_stats->memory_produce += mem->memory_produce;
        }
        return true;
    }
    return false;
}

bool memory_ret_s(memory_stats_t *mem_stats, void **ptr) {
    /*Проверка, что указатели не NULL*/
    if (ptr == NULL) {
        return false;
    }

    if (*ptr == NULL) {
        return false;
    }

    /*Указатели под структуру памяти*/
    memory_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = (memory_t *)((char *)(*ptr) - offsetof(memory_t, memory_ptr));

    size_t memory_release = mem->memory_request;

    if (memory_ret(ptr)) {
        /*Сохранение статистики*/
        if (mem_stats != NULL) {
            mem_stats->memory_call_del++;
            mem_stats->memory_release += memory_release;
        }
        return true;
    }
    return false;
}
