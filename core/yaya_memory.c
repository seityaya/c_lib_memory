// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2025 Seityagiya Terlekchi. All rights reserved.

#include "inttypes.h"
#include "malloc.h"
#include "stdalign.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "yaya_memory.h"

/*==================================================================================================================================================*/

bool memory_req(void **ptr, size_t count, size_t size) {
    /*Проверка, что возвращать есть куда*/
    if (ptr == NULL) {
        return false;
    }
    /*Размер элемента не может быть равен 0*/
    if (size == 0) {
        return false;
    }

    /*Вычисление количества запрошеной памяти*/
    const size_t new_size_request = count * size;

    /*Указатель под структуру памяти*/
    memory_t *mem_new = NULL;

    /*Если память не инициализированна, то указатель на предыдущую память NULL*/
    if (*ptr == NULL) {
        /*Вычисление размера необходимой памяти*/
        size_t new_size = 0;
        new_size += new_size_request;
        new_size += offsetof(memory_t, memory_ptr);
#if YAYA_MEMORY_USING_CANARY_END == 1
        new_size += sizeof(memory_canary_t);
#endif

        /*Выделение памяти на хранение информации и указателя*/
        mem_new = calloc(1, new_size);

        /*Проверка, что память выделилась*/
        if (mem_new == NULL) {
            return false;
        }

        /*Запоминаем сколько всего выделено*/
        size_t produce = malloc_usable_size(mem_new);

#if YAYA_MEMORY_FILL_AFTER_MEM == 1
        /*Установка значения сверх выделенного*/
        memset(mem_new->memory_ptr + new_size_request, YAYA_MEMORY_VALUE_AFTER_MEM, produce - new_size_request - offsetof(memory_t, memory_ptr));
#endif

        /*Сохранение информации о количестве памяти*/
        mem_new->memory_request = new_size_request;
        mem_new->memory_produce = produce;

#if YAYA_MEMORY_USING_CANARY_BEG == 1
        /*Устанавливаем значение начальной канарейки*/
        mem_new->memory_canary_beg = YAYA_MEMORY_VALUE_CANARY;
#endif
#if YAYA_MEMORY_USING_CANARY_END == 1
        /*Устанавливаем значение конечной канарейки*/
        memory_canary_t *canary_end = (memory_canary_t *)(&mem_new->memory_ptr[mem_new->memory_request]);
        *canary_end          = YAYA_MEMORY_VALUE_CANARY;
#endif

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;
    } else {
        /*Указатели под структуру памяти*/
        memory_t *mem_old = NULL;
        memory_t *mem_new = NULL;

        /*Помещаем указатель со смещением*/
        mem_old = (memory_t*)((char*)(*ptr) - offsetof(memory_t, memory_ptr));

#if YAYA_MEMORY_USING_CANARY_BEG == 1
        /*Проверяем корректность структуры*/
        if (mem_old->memory_canary_beg != YAYA_MEMORY_VALUE_CANARY) {
            return false;
        }
#endif
#if YAYA_MEMORY_USING_CANARY_END == 1
        /*Проверяем корректность структуры*/
        memory_canary_t *canary_end = (memory_canary_t *)(&mem_old->memory_ptr[mem_old->memory_request]);
        if (*canary_end != YAYA_MEMORY_VALUE_CANARY) {
            return false;
        }
#endif

        /*Запоминаем сколько было выделено*/
        size_t old_size_request = mem_old->memory_request;

        /*Вычисление размера необходимой памяти*/
        size_t new_size = 0;
        new_size += new_size_request;
        new_size += offsetof(memory_t, memory_ptr);
#if YAYA_MEMORY_USING_CANARY_END == 1
        new_size += sizeof(memory_canary_t);
#endif

        /*Перераспределяем память*/
        mem_new = realloc(mem_old, new_size);

        /*Проверка, что память выделилась*/
        if (mem_new == NULL) {
            return false;
        }

        /*Запоминаем сколько всего выделено*/
        size_t produce = malloc_usable_size(mem_new);

        /*Вычисление разницы*/
        intptr_t diff = (intptr_t)(new_size_request) - (intptr_t)(old_size_request);

        /*Зануление хвоста нового выделенного пространства*/
        if (diff > 0) {
            memset(mem_new->memory_ptr + old_size_request, 0x00, (size_t)(diff));
        }

#if YAYA_MEMORY_FILL_AFTER_MEM == 1
        /*Установка значения сверх выделенного*/
        if (diff < 0) {
            memset(mem_new->memory_ptr + new_size_request, YAYA_MEMORY_VALUE_AFTER_MEM, produce - new_size_request - offsetof(memory_t, memory_ptr));
        }
#endif

        /*Сохранение информации о количестве запрощеной памяти*/
        mem_new->memory_request = new_size_request;
        mem_new->memory_produce = produce;

#if YAYA_MEMORY_USING_CANARY_END == 1
        /*Устанавливаем значение конечной канарейки*/
        canary_end  = (memory_canary_t *)(&mem_new->memory_ptr[mem_new->memory_request]);
        *canary_end = YAYA_MEMORY_VALUE_CANARY;
#endif

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;
    }

    return true;
}

bool memory_ret(void **ptr) {
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
    mem = (memory_t*)((char*)(*ptr) - offsetof(memory_t, memory_ptr));

#if YAYA_MEMORY_FILL_BEFORE_FREE
    volatile size_t volsize = mem->memory_request;
    volatile uint8_t *volptr = (uint8_t *)(mem);
    while (volsize--) {
        *(volptr++) = 0x00;
    }
#endif

    free(mem);
    mem = NULL;
    *ptr = NULL;

    return true;
}

/*==================================================================================================================================================*/

bool memory_chk(void *ptr) {
    /*Проверка, что указатели не NULL*/
    if (ptr == NULL) {
        return false;
    }

    /*Указатель под структуру памяти*/
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = (memory_t*)((char*)(ptr) - offsetof(memory_t, memory_ptr));

#if YAYA_MEMORY_USING_CANARY_BEG == 1
    /*Проверяем корректность памяти по начальной канарейке*/
    if (mem_info->memory_canary_beg != YAYA_MEMORY_VALUE_CANARY) {
        return false;
    }
#endif
#if YAYA_MEMORY_USING_CANARY_END == 1
    /*Проверяем корректность памяти по конечной канарейке*/
    memory_canary_t *canary_end = (memory_canary_t *)(&mem_info->memory_ptr[mem_info->memory_request]);
    if (*canary_end != YAYA_MEMORY_VALUE_CANARY) {
        return false;
    }
#endif
    return true;
}

static memory_t* memory_begin(void *ptr) {
    if(!memory_chk(ptr)){
        return NULL;
    }
    return (memory_t*)((char*)(ptr) - offsetof(memory_t, memory_ptr));
}

bool memory_zero(void *ptr) {
    /*Указатель под структуру памяти*/
    memory_t *mem_info = memory_begin(ptr);

    /*Проверка, что указатели не NULL*/
    if (mem_info == NULL) {
        return false;
    }

    /*Сохраняем значения*/
    volatile size_t volsize = mem_info->memory_request;
    volatile char *volptr = mem_info->memory_ptr;

    /*Заполняем нулями*/
    while (volsize--) {
        *volptr++ = 0x00;
    }

    return true;
}

bool memory_fill(void *ptr, uint8_t value) {
    /*Указатель под структуру памяти*/
    memory_t *mem_info = memory_begin(ptr);

    /*Проверка, что указатели не NULL*/
    if (mem_info == NULL) {
        return false;
    }

    /*Сохраняем значения*/
    volatile size_t volsize = mem_info->memory_request;
    volatile char *volptr = mem_info->memory_ptr;

    /*Заполняем значением*/
    while (volsize--) {
        *volptr++ = value;
    }

    return true;
}

size_t memory_size(void *ptr) {
    /*Указатель под структуру памяти*/
    memory_t *mem_info = memory_begin(ptr);

    /*Проверка, что указатели не NULL*/
    if (mem_info == NULL) {
        return false;
    }

    /*Возвращаем размер запрошеной памяти*/
    return mem_info->memory_request;
}

/*==================================================================================================================================================*/

bool memory_swap(void *swapA, void *swapB, size_t size) {
    /*Проверка, что указатели не NULL*/
    if (swapA == NULL) {
        return false;
    }
    if (swapB == NULL) {
        return false;
    }
    if (size == 0) {
        return false;
    }

    char tempX[size];
    memcpy(tempX, swapB, size);
    memcpy(swapB, swapA, size);
    memcpy(swapA, tempX, size);

    return true;
}

size_t memory_step(void *ptr_beg, void *ptr_end, size_t size) {
    /*Проверка, что указатели не NULL*/
    if (ptr_beg == NULL) {
        return 0;
    }
    if (ptr_end == NULL) {
        return 0;
    }
    if (size == 0) {
        return 0;
    }

    ptrdiff_t dist = (ptrdiff_t)(ptr_end) - (ptrdiff_t)(ptr_beg);

    if (dist % (ptrdiff_t)(size) != 0) {
        return 0;
    }

    return (size_t)(dist / (ptrdiff_t)(size));
}

bool memory_shuf(void *base, size_t count, size_t size, unsigned int seed, memory_func_seed_t set_seed, memory_func_rand_t get_rand) {
    /*Проверка, что указатели не NULL*/
    if (base == NULL) {
        return false;
    }
    if (count == 0) {
        return false;
    }
    if (size == 0) {
        return false;
    }
    if (set_seed == NULL) {
        set_seed = srand;
    }
    if (get_rand == NULL) {
        get_rand = rand;
    }

    set_seed(seed);

    for (size_t i = 1; i < count; i++) {
        size_t rndx = ((((size_t)get_rand()) % (count - i)));
        memory_swap((char*)(base) + i * size, (char*)(base) + rndx * size, size);
    }
    for (size_t i = 0; i < count; i++) {
        size_t rnda = (((size_t)get_rand()) % (count));
        size_t rndb = (((size_t)get_rand()) % (count));
        memory_swap((char*)(base) + rnda * size, (char*)(base) + rndb * size, size);
    }
    return true;
}

bool memory_sort(void *base, size_t count, size_t size, memory_func_comp_t compare) {
    /*Проверка, что указатели не NULL*/
    if (base == NULL) {
        return false;
    }
    if (compare == NULL) {
        return false;
    }
    if (count == 0) {
        return false;
    }
    if (size == 0) {
        return false;
    }

    qsort(base, count, size, compare);

    return true;
}

bool memory_bsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare) {
    /*Проверка, что указатели не NULL*/
    if (search_res == NULL) {
        return false;
    }
    if (key == NULL) {
        return false;
    }
    if (base == NULL) {
        return false;
    }
    if (compare == NULL) {
        return false;
    }
    if (size == 0) {
        return false;
    }

    *search_res = bsearch(key, base, count, size, compare);
    if (*search_res != NULL) {
        return true;
    }
    *search_res = NULL;
    return false;
}

bool memory_rsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare) {
    /*Проверка, что указатели не NULL*/
    if (search_res == NULL) {
        return false;
    }
    if (key == NULL) {
        return false;
    }
    if (base == NULL) {
        return false;
    }
    if (compare == NULL) {
        return false;
    }
    if (size == 0) {
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        if (compare((char*)(base) + (i * size), key) == 0) {
            *search_res = (char*)(base) + (i * size);
            return true;
        }
    }
    *search_res = NULL;
    return false;
}

bool memory_dump(FILE *out, void *ptr, size_t len, uintmax_t catbyte, uintmax_t column) {
    /*Проверка, что указатель не NULL*/
    if (out == NULL) {
        return false;
    }

    /*Проверка, что указатель не NULL*/
    if (ptr == NULL) {
        return false;
    }

    /*Проверка, что числа есть степень 2*/
    if (catbyte == 0 || (catbyte & (catbyte - 1)) != 0) {
        return false;
    }
    if (column == 0 || (column & (column - 1)) != 0) {
        return false;
    }

    if (len == 0) {
        /*Указатель под структуру памяти*/
        memory_t *mem_info = memory_begin(ptr);

        /*Проверка, что указатели не NULL*/
        if (mem_info == NULL) {
            return false;
        }

        ptr = mem_info->memory_ptr;
        len = mem_info->memory_request;
    }

    uintmax_t const col1 = (1 + 2 + sizeof(uintptr_t) * 2 + 1);
    uintmax_t const col2 = (((column * catbyte) * 2) + column) + 1;

    /*Шапка*/
    {
        fprintf(out, "╭");
        for (uintmax_t i = 0; i < col1; i++) {
            fprintf(out, "─");
        }
        fprintf(out, "┬");
        for (uintmax_t i = 0; i < col2; i++) {
            fprintf(out, "─");
        }
        fprintf(out, "╮");
        fprintf(out, "\n");
        fprintf(out, "│ Len: %8" PRIuMAX " byte │ ", len);
        for (uintmax_t i = 0; i < column; i++) {
            for (uintmax_t j = 0; j < catbyte; j++) {
                fprintf(out, "%02" PRIXMAX "", i * catbyte + j);
            }
            fprintf(out, " ");
        }
        fprintf(out, "│");
        fprintf(out, "\n");
        fprintf(out, "├");
        for (uintmax_t i = 0; i < col1; i++) {
            fprintf(out, "─");
        }
        fprintf(out, "┼");
        for (uintmax_t i = 0; i < col2; i++) {
            fprintf(out, "─");
        }
        fprintf(out, "┤");
        fprintf(out, "\n");
    }

    /*Тело*/
    {
        const uint8_t mod_bit = 0x10;
        uint8_t *nadr = (uint8_t*)((char*)(ptr) - (((uintptr_t)ptr % (column * catbyte)) % mod_bit));

        uintmax_t pos = 0;
        while (pos < len) {
            if (pos == 0) {
                fprintf(out, "│ 0x%0*" PRIXPTR " │ ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)nadr);
            } else {
                fprintf(out, "│ 0x%0*" PRIXPTR " │ ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)ptr + pos);
            }

            for (uintmax_t i = 0; i < column; i++) {
                uintmax_t ind = 0;

                if (pos == 0) {
                    for (ind = 0; ind < catbyte; ind++) {
                        if (&nadr[i * catbyte + ind] == ptr) {
                            goto L1;
                        }
                        fprintf(out, "..");
                    }
                    fprintf(out, " ");
                    continue;
                }
            L1:
                for (uintmax_t k = ind; k < catbyte; k++) {
                    if (pos < len) {
                        fprintf(out, "%02" PRIx8 "", ((uint8_t *)ptr)[pos]);
                        pos++;
                    } else {
                        fprintf(out, "..");
                    }
                }
                fprintf(out, " ");
            }
            fprintf(out, "│");
            fprintf(out, "\n");
        }
    }

    /*Подвал*/
    {
        fprintf(out, "╰");
        for (uintmax_t i = 0; i < col1; i++) {
            fprintf(out, "─");
        }
        fprintf(out, "┴");
        for (uintmax_t i = 0; i < col2; i++) {
            fprintf(out, "─");
        }
        fprintf(out, "╯");
    }
    fprintf(out, "\n");

    fflush(out);
    return true;
}

static uint64_t bit_sequence(void *ptr, uintmax_t offset, uintmax_t len) {
    uint8_t *bytes = (uint8_t *)ptr;
    uint64_t result = 0;
    uint32_t result32_1 = 0;
    uint32_t result32_2 = 0;

    if (len == 0) {
        return 0;
    }

#define CU(x) (x##U)
    if (len <= CU(16)) {
        for (uint64_t i = 0U; i < len; i++) {
            result |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        return result;
    }
    if (len <= CU(32)) {
        for (uint64_t i = 0U; i < len; i++) {
            result32_1 |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        result = result32_1;
        return result;
    }
    if (len <= CU(64)) {
        for (uint64_t i = 0U; i < CU(32); i++) {
            result32_1 |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        for (uint64_t i = 0U; i < CU(64) - (len - CU(32)); i++) {
            result32_2 |= ((uint64_t)(bytes[(offset + CU(32) + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        result = (uint64_t)result32_1 | ((uint64_t)result32_2) << CU(32);
        return result;
    }
#undef CU
    return 0;
}

bool memory_look(FILE *out, void *ptr, size_t struct_count, size_t struct_size, intmax_t bit_len_list[]) {
    /*Проверка, что указатель не NULL*/
    if (out == NULL) {
        return false;
    }

    /*Проверка, что указатель не NULL*/
    if (ptr == NULL) {
        return false;
    }

    /* Проверка и отладочная информация */
    {
        /* Объявление вычислимых констант */
        uintmax_t list_bit_sum = 0;

        /* Вычисление констант */
        for (uintmax_t i = 0; 0 == 0; i++) {
            if (bit_len_list[i] == 0) {
                break;
            }
            if (bit_len_list[i] > 0) {
                list_bit_sum += (uintmax_t)(+bit_len_list[i]);
            } else {
                list_bit_sum += (uintmax_t)(-bit_len_list[i]);
            }
        }

        /* Проверка, что переданый размер структуры и массив отступов совпадают */
        if ((struct_size * __CHAR_BIT__) != list_bit_sum) {
            fprintf(out,
                    "ERROR: (struct_size * __CHAR_BIT__)[%" PRIuMAX "] != |list_bit_sum|[%" PRIuMAX "] \n",
                    (struct_size * __CHAR_BIT__),
                    list_bit_sum);
            return false;
        }
    }

    /* Таблица */
    {
        intmax_t col1 = (1 + 2 + (int)(sizeof(uintptr_t) * 2) + 1);
        intmax_t col2 = 1;
        intmax_t list_count = 0;
        intmax_t bits_count = 0;

        for (intmax_t i = 0;; i++) {
            if (bit_len_list[i] == 0) {
                list_count = i;
                break;
            }
            if (bit_len_list[i] < 0) {
                continue;
            }
            bits_count += bit_len_list[i];
            col2 += bit_len_list[i] / 4 + 1;
            if (bit_len_list[i] < 4 || bit_len_list[i] % 4 > 0) {
                col2 += 1;
            }
        }

        /* Шапка */
        {
            /* Крышка */
            fprintf(out, "╭");
            for (intmax_t i = 0; i < col1; i++) {
                fprintf(out, "─");
            }
            fprintf(out, "┬");
            for (intmax_t i = 0; i < col2; i++) {
                fprintf(out, "─");
            }
            fprintf(out, "╮");
            fprintf(out, "\n");

            /* Подписи */
            fprintf(out, "│ S: %5" PRIuMAX "/%-5" PRIuMAX " bit │ ", bits_count, struct_size * __CHAR_BIT__);
            for (intmax_t i = 0; i < list_count; i++) {
                if (bit_len_list[i] < 0) {
                    continue;
                }
                fprintf(out,
                        "%*" PRIiMAX " ",
                        (int)((bit_len_list[i] / 4) + ((bit_len_list[i] < 4 || bit_len_list[i] % 4 > 0) ? 1 : 0)),
                        bit_len_list[i]);
            }
            fprintf(out, "│");
            fprintf(out, "\n");

            /* Балка */
            fprintf(out, "├");
            for (intmax_t i = 0; i < col1; i++) {
                fprintf(out, "─");
            }
            fprintf(out, "┼");
            for (intmax_t i = 0; i < col2; i++) {
                fprintf(out, "─");
            }
            fprintf(out, "┤");
            fprintf(out, "\n");
        }

        /* Тело */
        {
            for (uintmax_t i = 0; i < struct_count; i++) {
                fprintf(out, "│ 0x%0*" PRIXPTR " │ ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)(ptr) + (i * (uintptr_t)(struct_size)));
                intmax_t offset = 0;
                for (intmax_t k = 0; k < list_count; k++) {
                    if (bit_len_list[k] > 0) {
                        uintmax_t res =
                            bit_sequence(ptr, (uintptr_t)(struct_size * __CHAR_BIT__ * i) + (uintptr_t)(offset), (uintmax_t)(bit_len_list[k]));
                        fprintf(out, "%0*" PRIxMAX " ", (int)(bit_len_list[k] / 4 + ((bit_len_list[k] < 4 || bit_len_list[k] % 4 > 0) ? 1 : 0)), res);
                        offset += bit_len_list[k];
                    } else {
                        offset += -bit_len_list[k];
                    }
                }
                fprintf(out, "│\n");
            }
        }

        /*Подвал*/
        {
            /* Дно */
            fprintf(out, "╰");
            for (intmax_t i = 0; i < col1; i++) {
                fprintf(out, "─");
            }
            fprintf(out, "┴");
            for (intmax_t i = 0; i < col2; i++) {
                fprintf(out, "─");
            }
            fprintf(out, "╯");
        }
        fprintf(out, "\n");
    }

    fflush(out);
    return true;
}
