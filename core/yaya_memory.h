// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2025 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_H
#define YAYA_MEMORY_H

#include "stdalign.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"

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
#if YAYA_MEMORY_FILL_AFTER_MEM
#ifndef YAYA_MEMORY_VALUE_AFTER_MEM
#   define YAYA_MEMORY_VALUE_AFTER_MEM 0x00
#endif /*YAYA_MEMORY_VALUE_AFTER_MEM*/
#endif

/**
 * Использовать начальную канарейку
 */
#ifndef YAYA_MEMORY_USING_CANARY_BEG
#   define YAYA_MEMORY_USING_CANARY_BEG 1
#endif /*YAYA_MEMORY_USING_CANARY_BEG*/

/**
 * Использовать конечную канарейку
 */
#ifndef YAYA_MEMORY_USING_CANARY_END
#   define YAYA_MEMORY_USING_CANARY_END 1 /*TODO*/
#endif /*YAYA_MEMORY_USING_CANARY_END*/

/**
 * Специальное значение для определения начала структуры
 */
#if YAYA_MEMORY_USING_CANARY_BEG || YAYA_MEMORY_USING_CANARY_END
#ifndef YAYA_MEMORY_VALUE_CANARY
#   define YAYA_MEMORY_VALUE_CANARY     0x7968574635241320
#endif /*YAYA_MEMORY_VALUE_CANARY*/
#endif

#if YAYA_MEMORY_USING_CANARY_BEG || YAYA_MEMORY_USING_CANARY_END
typedef size_t memory_canary_t;
#endif

typedef struct memory_t {
#if YAYA_MEMORY_USING_CANARY_BEG
    memory_canary_t memory_canary_beg;      // начальная канарейка
#endif
    size_t memory_request;                  // запросили
    size_t memory_produce;                  // выдали
    alignas(max_align_t) char memory_ptr[]; // указатель на начало
#if YAYA_MEMORY_USING_CANARY_END
//  memory_canary_t memory_canary_end;      // конечная канарейка /*TODO*/
#endif
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

#endif /*YAYA_MEMORY_H*/
