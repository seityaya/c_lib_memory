//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.10
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_STATS_H
#define YAYA_MEMORY_STATS_H

/**
 * Включить подсчет статистики использования памяти глобально
 */
#ifndef YAYA_MEMORY_STATS_USE
#   define YAYA_MEMORY_STATS_USE            1
/**
 * Для подсчета статистики использовать глобальную переменную
 */
#   ifndef YAYA_MEMORY_STATS_GLOBAL
#       define YAYA_MEMORY_STATS_GLOBAL     0
#   endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

#include "yaya_memory.h"

typedef struct memory_stats {
    size_t memory_request;  //запросил
    size_t memory_produce;  //выдали
    size_t memory_release;  //освободил
    size_t memory_call_new; //фактически выдано
    size_t memory_call_res; //фактически перераспределено
    size_t memory_call_del; //фактически удалено
} memory_stats_t;

bool memory_stats_init(memory_stats_t** mem_stats);
bool memory_stats_free(memory_stats_t** mem_stats);
bool memory_stats_out (memory_stats_t*  mem_stats, FILE* out);

bool memory_new_s(memory_stats_t *mem_stats, void **ptr, void **old_ptr, const size_t count, const size_t size);
bool memory_del_s(memory_stats_t *mem_stats, void **ptr);

#if YAYA_MEMORY_STATS_GLOBAL == 1
extern memory_stats_t *memory_stats_global;

#define memory_stats_init()        memory_stats_init(&memory_stats_global)
#define memory_stats_free()        memory_stats_free(&memory_stats_global)
#define memory_stats_out(out)      memory_stats_out ( memory_stats_global, out)

#define memory_new(N, O, C, S)     memory_new_s(memory_stats_global, N, O, C, S)
#define memory_del(N)              memory_del_s(memory_stats_global, N)
#else
#define memory_new(I, N, O, C, S)  memory_new_s(I, N, O, C, S)
#define memory_del(I, N)           memory_del_s(I, N)
#endif

#endif /*YAYA_MEMORY_STATS_H*/
