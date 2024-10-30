// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_H
#define YAYA_MEMORY_H

#include "inttypes.h"
#include "malloc.h"
#include "stdalign.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/**
 * Очищать область памяти перед освобождением
 */
#ifndef YAYA_MEMORY_FILL_BEFORE_FREE
#   define YAYA_MEMORY_FILL_BEFORE_FREE 1
#endif /*YAYA_MEMORY_FILL_BEFORE_FREE*/

/**
 * Заполнять память за пределами запрошеной области
 * ОПАСНАЯ НАСТРОЙКА!
 * DANGEROUS SETTING!
 */
#ifndef YAYA_MEMORY_FILL_AFTER_MEM
#   define YAYA_MEMORY_FILL_AFTER_MEM   0
#endif /*YAYA_MEMORY_FILL_AFTER_MEM*/

/**
 * Специальное значение для заполнения за пределами запрошеной области
 */
#ifndef YAYA_MEMORY_VALUE_AFTER_MEM
#   define YAYA_MEMORY_VALUE_AFTER_MEM  0x00
#endif /*YAYA_MEMORY_VALUE_AFTER_MEM*/

/**
 * Специальное значение для определения начала структуры
 */
#ifndef YAYA_MEMORY_VALUE_CANARY
#   define YAYA_MEMORY_VALUE_CANARY     0x7968574635241320
#endif /*YAYA_MEMORY_VALUE_CANARY*/

typedef struct memory_t {
    size_t memory_canary_beg;               // начальная канарейка
    size_t memory_request;                  // запросили
    size_t memory_produce;                  // выдали
    alignas(max_align_t) char memory_ptr[]; // указатель на начало
/*  size_t memory_canary_end;               // коечная канарейка */ //TODO
} memory_t;

typedef int  (*memory_func_comp_t)(const void *, const void *);
typedef void (*memory_func_seed_t)(unsigned int);
typedef int  (*memory_func_rand_t)(void);

bool    memory_req(void **ptr, size_t count, size_t size);
bool    memory_ret(void **ptr);

bool    memory_chk (void *ptr);
bool    memory_zero(void *ptr);
bool    memory_fill(void *ptr, uint8_t value);
size_t  memory_size(void *ptr);
bool    memory_dump(FILE* out, void *ptr, size_t len, uint64_t catbyte, uint64_t column_mod2);
bool    memory_swap(void *x, void *y, size_t size);
size_t  memory_step(void *ptr_beg, void *ptr_end, size_t size);
bool    memory_shuf(void *base, size_t count, size_t size, unsigned int seed, memory_func_seed_t set_seed, memory_func_rand_t get_rand);
bool    memory_sort(void *base, size_t count, size_t size, memory_func_comp_t compare);
bool    memory_bsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare);
bool    memory_rsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare);
bool    memory_look(FILE* out, void *ptr, size_t struct_count, size_t struct_size, int64_t bit_len_list[]);

#define memory_bit_len_list(...) (int64_t[]){__VA_ARGS__, 0}

/*==================================================================================================================================================*/

/**
 * Включить подсчет статистики использования памяти
 */
#ifndef YAYA_MEMORY_STATS_LOCAL
#   define YAYA_MEMORY_STATS_LOCAL       0
#endif /*YAYA_MEMORY_STATS_USE*/

/**
 * Для подсчета статистики использовать глобальную переменную
 */
#ifndef YAYA_MEMORY_STATS_GLOBAL
#    define YAYA_MEMORY_STATS_GLOBAL     0
#endif /*YAYA_MEMORY_STATS_GLOBAL*/

#if YAYA_MEMORY_STATS_LOCAL == 1 && YAYA_MEMORY_STATS_GLOBAL == 1
#error YAYA_MEMORY_STATS_LOCAL == 1 or YAYA_MEMORY_STATS_GLOBAL == 1
#endif

typedef struct memory_stats {
    size_t memory_request;  // запросил
    size_t memory_produce;  // выдали
    size_t memory_release;  // освободил
    size_t memory_call_new; // фактически выдано
    size_t memory_call_res; // фактически перераспределено
    size_t memory_call_del; // фактически удалено
} memory_stats_t;

bool memory_stats_init(memory_stats_t **mem_stats);
bool memory_stats_free(memory_stats_t **mem_stats);
bool memory_stats_out (memory_stats_t *mem_stats, FILE *out);

bool memory_req_s(memory_stats_t *mem_stats, void **ptr, size_t count, size_t size);
bool memory_ret_s(memory_stats_t *mem_stats, void **ptr);

#if YAYA_MEMORY_STATS_GLOBAL == 1
extern memory_stats_t memory_stats_global;

#define memory_stats_init()                    (true)
#define memory_stats_free()                    (true)
#define memory_stats_out(out)                  memory_stats_out(&memory_stats_global, out)

#define memory_req(ptr, count, size)           memory_req_s(&memory_stats_global, ptr, count, size)
#define memory_ret(ptr)                        memory_ret_s(&memory_stats_global, ptr)
#endif

#endif /*YAYA_MEMORY_H*/
