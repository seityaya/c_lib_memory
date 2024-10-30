// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_MEMORY_WRAPPER_H
#define YAYA_MEMORY_WRAPPER_H

#include "yaya_memory.h"

#undef memory_req
#undef memory_ret

#if YAYA_MEMORY_STATS_LOCAL == 0 && YAYA_MEMORY_STATS_GLOBAL == 0
#   define memory_req(ptr, count, size)                        memory_req  (         (void**)(ptr), (size_t)(count), (size_t)(size))
#   define memory_ret(ptr)                                     memory_ret  (         (void**)(ptr))
#   define memory_req_s(stats, ptr, count, size)               memory_req_s((stats), (void**)(ptr), (size_t)(count), (size_t)(size))
#   define memory_ret_s(stats, ptr)                            memory_ret_s((stats), (void**)(ptr))
#else
#   if YAYA_MEMORY_STATS_LOCAL == 1
#       define memory_req(stats, ptr, count, size)             memory_req_s((stats),              (void**)(ptr), (size_t)(count), (size_t)(size))
#       define memory_ret(stats, ptr)                          memory_ret_s((stats),              (void**)(ptr))
#   endif /*YAYA_MEMORY_STATS_LOCAL*/
#   if YAYA_MEMORY_STATS_GLOBAL == 1
#       define memory_req(ptr, count, size)                    memory_req_s(&memory_stats_global, (void**)(ptr), (size_t)(count), (size_t)(size))
#       define memory_ret(ptr)                                 memory_ret_s(&memory_stats_global, (void**)(ptr))
#   endif /*YAYA_MEMORY_STATS_GLOBAL*/
#endif /*YAYA_MEMORY_STATS_USE*/

#define memory_chk(ptr)                                        memory_chk ((void*)(ptr))
#define memory_zero(ptr)                                       memory_zero((void*)(ptr))
#define memory_size(ptr)                                       memory_size((void*)(ptr))
#define memory_fill(ptr, value)                                memory_fill((void*)(ptr), (uint8_t)(value))

#define memory_step(ptr_beg, ptr_end, size)                    memory_step((void*)(ptr_beg), (void*)(ptr_end), (size_t)(size))
#define memory_swap(ptr_1, ptr_2, size)                        memory_swap((void*)(ptr_1), (void*)(ptr_2), (size_t)(size))
#define memory_shuf(ptr, count, size, seed)                    memory_shuf((void*)(ptr), (size_t)(count), (size_t)(size), (seed), (srand), (rand))
#define memory_sort(ptr, count, size, func_comp)               memory_sort((void*)(ptr), (size_t)(count), (size_t)(size), (memory_func_comp_t)(func_comp))

#define memory_bsearch(res, key, base, count, size, func_comp) memory_bsearch((void**)(res), (void*)(key), (void*)(base), (size_t)(count), (size_t)(size), (memory_func_comp_t)(func_comp))
#define memory_rsearch(res, key, base, count, size, func_comp) memory_rsearch((void**)(res), (void*)(key), (void*)(base), (size_t)(count), (size_t)(size), (memory_func_comp_t)(func_comp))
#define memory_dump(out, ptr, len, catbyte, column_mod2)       memory_dump((out), (void*)(ptr), (size_t)(len), catbyte, column_mod2)
#define memory_look(out, ptr, count, size, bit_len_list)       memory_look((out), (void*)(ptr), (size_t)(count), (size_t)(size), bit_len_list)

#endif /*YAYA_MEMORY_WRAPPER_H*/
