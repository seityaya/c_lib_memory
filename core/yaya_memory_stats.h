// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2025 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_STATS_H
#define YAYA_MEMORY_STATS_H

#include "yaya_memory.h"

/**
 * Включить подсчет статистики использования памяти
 */
#ifndef YAYA_MEMORY_STATS_LOCAL
#   define YAYA_MEMORY_STATS_LOCAL 0
#endif /*YAYA_MEMORY_STATS_USE*/

/**
 * Для подсчета статистики использовать глобальную переменную
 */
#ifndef YAYA_MEMORY_STATS_GLOBAL
#   define YAYA_MEMORY_STATS_GLOBAL 0
#endif /*YAYA_MEMORY_STATS_GLOBAL*/

#if YAYA_MEMORY_STATS_LOCAL == 1 && YAYA_MEMORY_STATS_GLOBAL == 1
#   error Define YAYA_MEMORY_STATS_LOCAL == 1 or YAYA_MEMORY_STATS_GLOBAL == 1
#endif

#if YAYA_MEMORY_STATS_LOCAL == 0 && YAYA_MEMORY_STATS_GLOBAL == 0
#   error Define YAYA_MEMORY_STATS_LOCAL == 0 or YAYA_MEMORY_STATS_GLOBAL == 0
#endif

#if (YAYA_MEMORY_STATS_LOCAL == 0) != (YAYA_MEMORY_STATS_GLOBAL == 0)
typedef struct memory_stats {
    size_t memory_request;   // запросил
    size_t memory_produce;   // выдали
    size_t memory_release;   // освободил
    size_t memory_call_new;  // фактически выдано
    size_t memory_call_res;  // фактически перераспределено
    size_t memory_call_del;  // фактически удалено
} memory_stats_t;

bool memory_stats_init(memory_stats_t **mem_stats);
bool memory_stats_free(memory_stats_t **mem_stats);
bool memory_stats_out(memory_stats_t *mem_stats, FILE *out);

bool memory_req_s(memory_stats_t *mem_stats, void **ptr, size_t count, size_t size);
bool memory_ret_s(memory_stats_t *mem_stats, void **ptr);

#if YAYA_MEMORY_STATS_LOCAL == 1
#   define memory_req(stats, ptr, count, size) memory_req_s((stats), (ptr), (count), (size))
#   define memory_ret(stats, ptr)              memory_ret_s((stats), (ptr))
#endif

#if YAYA_MEMORY_STATS_GLOBAL == 1
    extern memory_stats_t memory_stats_global;

#   define memory_stats_init()                 (true)
#   define memory_stats_free()                 (true)
#   define memory_stats_out(out)               memory_stats_out(&memory_stats_global, out)

#   define memory_req(ptr, count, size)        memory_req_s((&memory_stats_global), (ptr), (count), (size))
#   define memory_ret(ptr)                     memory_ret_s((&memory_stats_global), (ptr))
#endif

#endif

#endif /*YAYA_MEMORY_STATS_H*/
