// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_WRAPPER_H
#define YAYA_MEMORY_WRAPPER_H

#include "yaya_memory.h"

#if YAYA_MEMORY_STATS_USE && !YAYA_MEMORY_STATS_GLOBAL
#define memory_req(ptr, count, size)                           memory_req((void**)(ptr), (size_t)(count), (size_t)(size))
#define memory_ret(ptr)                                        memory_ret((void**)(ptr))
#else
#define memory_req(stats, ptr, count, size)                    memory_req_s((stats), (void**)(ptr), (size_t)(count), (size_t)(size))
#define memory_exc(stats, ptr, count, size)                    memory_exs_s((stats), (void**)(ptr), (size_t)(count), (size_t)(size))
#define memory_ret(stats, ptr)                                 memory_ret_s((stats), (void**)(ptr))
#endif

#define memory_zero(ptr)                                       memory_zero((void*)(ptr))
#define memory_size(ptr)                                       memory_size((void*)(ptr))
#define memory_fill(ptr, value)                                memory_fill((void*)(ptr), (uint8_t)(value))

#define memory_step(ptr_beg, ptr_end, size)                    memory_step((void*)(ptr_beg), (void*)(ptr_end), (size_t)(size))
#define memory_swap(ptr_1, ptr_2, size)                        memory_swap((void*)(ptr_1), (void*)(ptr_2), (size_t)(size))
#define memory_shuf(ptr, count, size, seed)                    memory_shuf((void*)(ptr), (size_t)(count), (size_t)(size), (seed), (srand), (rand))
#define memory_sort(ptr, count, size, func_comp)               memory_sort((void*)(ptr), (size_t)(count), (size_t)(size), (memory_func_comp_t)(func_comp))

#define memory_bsearch(res, key, base, count, size, func_comp) memory_bsearch((void**)(res), (void*)(key), (void*)(base), (size_t)(count), (size_t)(size), (memory_func_comp_t)(func_comp))
#define memory_rsearch(res, key, base, count, size, func_comp) memory_rsearch((void**)(res), (void*)(key), (void*)(base), (size_t)(count), (size_t)(size), (memory_func_comp_t)(func_comp))
#define memory_dump(ptr)                                       memory_dump((void*)(ptr), 0, 1, 16)
#define memory_look(ptr, count, size, bit_len_list)            memory_look((void*)(ptr), (size_t)(count), (size_t)(size), memory_bit_len_list((bit_len_list)))

#endif /*YAYA_MEMORY_WRAPPER_H*/
