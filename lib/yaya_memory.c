//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory.h"
#include "yaya_memory_stats.h"

memory_stats_t *memory_stats_global = NULL;

#undef memory_stats_init
#undef memory_stats_free
#undef memory_stats_out
#undef memory_new
#undef memory_del

bool memory_stats_out(memory_stats_t *mem_stats, FILE* out)
{
    /*Проверка, что указатель не NULL*/
    if(out == NULL){
        return false;
    }

    /*Проверка, что указатель не NULL*/
    if(mem_stats == NULL) {
        return false;
    }

    fprintf(out, "Request :% 10" PRIiMAX "; ", mem_stats->memory_request);
    fprintf(out, "NEW  :% 10" PRIiMAX "; ",    mem_stats->memory_call_new);
    fprintf(out, "\n");
    fprintf(out, "Produce :% 10" PRIiMAX "; ", mem_stats->memory_produce);
    fprintf(out, "RES  :% 10" PRIiMAX "; ",    mem_stats->memory_call_res);
    fprintf(out, "\n");
    fprintf(out, "Overhead:% 10" PRIiMAX "; ", mem_stats->memory_produce - mem_stats->memory_request);
    fprintf(out, "DEL  :% 10" PRIiMAX "; ",    mem_stats->memory_call_del);
    fprintf(out, "\n");
    fprintf(out, "Release :% 10" PRIiMAX "; ", mem_stats->memory_release);
    fprintf(out, "USAGE:% 10" PRIiMAX "; ",    mem_stats->memory_produce - mem_stats->memory_release);
    fprintf(out, "\n");

    fflush(out);
    return true;
}

bool memory_stats_init(memory_stats_t **mem_stats)
{
    if(mem_stats == NULL){
        return false;
    }

    if(*mem_stats == NULL){
        *mem_stats = (memory_stats_t*)malloc(sizeof(memory_stats_t));
        if(*mem_stats == NULL){
            return false;
        }

        memset(*mem_stats, 0, malloc_usable_size(*mem_stats));
        return true;
    }

    return false;
}

bool memory_stats_free(memory_stats_t **mem_stats)
{
    if(mem_stats != NULL) {
        free(*mem_stats);
        *mem_stats = NULL;
        return true;
    }
    return false;
}

bool memory_new_s(memory_stats_t *mem_stats, void **ptr, void **old_ptr, const size_t count, const size_t size)
{
    /*Проверка, что возвращать есть куда*/
    if(ptr == NULL) {
        return false;
    }

    /*Проверка, что запрошено не нулевой размер памяти*/
    const size_t new_size_request = (count * size);
    if(new_size_request  == 0){
        return false;
    }

    /*Указатели под структуру памяти*/
    memory_t *mem_old = NULL;
    memory_t *mem_new = NULL;

    /*Если память не инициализирована, то указатель на предыдущую память NULL*/
    if(old_ptr == NULL || *old_ptr == NULL) {
        /*Выделение памяти на хранение информации и указателя*/
        mem_new = malloc(new_size_request  + offsetof(memory_t, memory_ptr));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL) {
            return false;
        }

        size_t produce = malloc_usable_size(mem_new);

        /*Зануление всего выделенного*/
        memset(mem_new, 0x00, produce);

        /*Установка значения сверх выделенного*/
        memset(mem_new->memory_ptr + new_size_request , YAYA_MEMORY_VALUE_AFTER_MEM, produce - new_size_request - offsetof(memory_t, memory_ptr));

        /*Сохранение информации о количестве памяти*/
        mem_new->memory_request = new_size_request ;
        mem_new->memory_produce = produce;
        mem_new->memory_canary  = YAYA_MEMORY_VALUE_CANARY;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_new++;
            mem_stats->memory_request += mem_new->memory_request;
            mem_stats->memory_produce += mem_new->memory_produce;
        }
    }
    /*Если память инициализированна, то указатель на предыдущую память не NULL*/
    else
    {
        /*Помещаем указатель со смещением*/
        mem_old = (*old_ptr) - offsetof(memory_t, memory_ptr);

        /*Проверяем корректность структуры*/
        if(mem_old->memory_canary != YAYA_MEMORY_VALUE_CANARY){
            return false;
        }

        /*Запоминаем сколько было выделено*/
        size_t old_size = mem_old->memory_request;

        /*Перераспределяем память*/
        mem_new = realloc(mem_old, new_size_request  + offsetof(memory_t, memory_ptr));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL) {
            return false;
        }

        /*Запоминаем сколько выделено*/
        size_t new_size = malloc_usable_size(mem_new);

        /*Вычисление разницы*/
        intptr_t diff = (intptr_t)(new_size_request ) - (intptr_t)(old_size);

        /*Зануление хвоста нового выделенного*/
        if(diff > 0) {
            memset(mem_new->memory_ptr + old_size, 0x00, (size_t)(diff));
        }

        /*Установка значения сверх выделенного*/
        if(diff < 0) {
            memset(mem_new->memory_ptr + new_size_request , YAYA_MEMORY_VALUE_AFTER_MEM, new_size - new_size_request  - offsetof(memory_t, memory_ptr));
        }

        /*Сохранение информации о количестве запрощеной памяти*/
        mem_new->memory_request = new_size_request ;
        mem_new->memory_produce = new_size;

        /*Уничтожаем старый указатель*/
        *old_ptr = NULL;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

        /*Сохранение статистики*/
        if(mem_stats != NULL) {
            mem_stats->memory_call_res++;
            mem_stats->memory_request += mem_new->memory_request;
            mem_stats->memory_produce += mem_new->memory_produce;
        }
    }
    return true;
}

bool memory_del_s(memory_stats_t *mem_stats, void **ptr)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL) {
        return false;
    }

    if(*ptr == NULL) {
        return false;
    }

    /*Указатели под структуру памяти*/
    memory_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = *ptr - offsetof(memory_t, memory_ptr);

    /*Сохранение статистики*/
    if(mem_stats != NULL){
        mem_stats->memory_call_del++;
        mem_stats->memory_release += mem->memory_produce;
    }

#if YAYA_MEMORY_FILL_BEFORE_FREE
    volatile size_t   volsize = mem->memory_produce;
    volatile uint8_t* volptr  = (uint8_t*)(mem);
    while (volsize--) {
        *(volptr++) = 0x00;
    }
#endif

    free(mem);
    mem = NULL;
    *ptr = NULL;

    return true;
}

bool memory_new(void **ptr, void **old_ptr, const size_t count, const size_t size)
{
    return memory_new_s(NULL, ptr, old_ptr, count, size);
}

bool memory_del(void **ptr)
{
    return memory_del_s(NULL, ptr);
}

bool memory_zero(void *ptr)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Указатель под структуру памяти*/
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(memory_t, memory_ptr);

    /*Проверяем корректность структуры*/
    if(mem_info->memory_canary != YAYA_MEMORY_VALUE_CANARY){
        return false;
    }

    /*Сохраняем значения*/
    volatile size_t   volsize = mem_info->memory_request;
    volatile uint8_t* volptr  = mem_info->memory_ptr;

    /*Заполняем нулями*/
    while (volsize--) {
        *volptr++ = 0x00;
    }

    return true;
}

bool memory_fill(void *ptr, uint8_t value)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Указатель под структуру памяти*/
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(memory_t, memory_ptr);

    /*Проверяем корректность структуры*/
    if(mem_info->memory_canary != YAYA_MEMORY_VALUE_CANARY){
        return false;
    }

    /*Сохраняем значения*/
    volatile size_t  volsize = mem_info->memory_request;
    volatile uint8_t *volptr = mem_info->memory_ptr;

    /*Заполняем значением*/
    while (volsize--){
        *volptr++ = value;
    }

    return true;
}

size_t memory_size(void *ptr)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return 0;
    }

    /*Указатель под структуру памяти*/
    memory_t *mem_info = NULL;

    /*Помещаем указатель со смещением*/
    mem_info = ptr - offsetof(memory_t, memory_ptr);

    /*Проверяем корректность структуры*/
    if(mem_info->memory_canary != YAYA_MEMORY_VALUE_CANARY){
        return 0;
    }

    /*Возвращаем размер запрошеной памяти*/
    return mem_info->memory_request;
}

bool memory_swap(void *swapA, void *swapB, size_t size)
{
    /*Проверка, что указатели не NULL*/
    if(swapA == NULL){
        return false;
    }
    if(swapB == NULL){
        return false;
    }
    if(size == 0){
        return false;
    }

    char tempX[size];
    memcpy(tempX, swapB, size);
    memcpy(swapB, swapA, size);
    memcpy(swapA, tempX, size);

    return true;
}

size_t memory_step(void* ptr_beg, void* ptr_end, size_t size)
{
    /*Проверка, что указатели не NULL*/
    if(ptr_beg == NULL){
        return 0;
    }
    if(ptr_end == NULL){
        return 0;
    }
    if(size == 0){
        return 0;
    }

    ptrdiff_t dist = (ptrdiff_t)(ptr_end) - (ptrdiff_t)(ptr_beg);

    if(dist % (ptrdiff_t)(size) != 0){
        return 0;
    }

    return (size_t)(dist / (ptrdiff_t)(size));
}

bool memory_shuf(void *base, size_t count, size_t size, unsigned int seed, memory_func_seed_t set_seed, memory_func_rand_t get_rand)
{
    /*Проверка, что указатели не NULL*/
    if(base == NULL){
        return false;
    }
    if(count == 0){
        return false;
    }
    if(size == 0){
        return false;
    }
    if(set_seed == NULL){
        set_seed = srand;
    }
    if(get_rand == NULL){
        get_rand = rand;
    }

    set_seed(seed);

    for(size_t i = 1; i < count; i++){
        size_t rndx =  ((((size_t)get_rand()) % (count - i)));
        memory_swap(base + i * size, base + rndx * size, size);
    }
    for(size_t i = 0; i < count; i++){
        size_t rnda = (((size_t)get_rand()) % (count)) ;
        size_t rndb = (((size_t)get_rand()) % (count)) ;
        memory_swap(base + rnda * size, base + rndb * size, size);
    }
    return true;
}

bool memory_sort(void *base, size_t count, size_t size, memory_func_comp_t compare)
{
    /*Проверка, что указатели не NULL*/
    if(base == NULL){
        return false;
    }
    if(compare == NULL){
        return false;
    }
    if(count == 0){
        return false;
    }
    if(size == 0){
        return false;
    }

    qsort(base, count, size, compare);

    return true;
}

bool memory_bsearch(void **search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare)
{
    /*Проверка, что указатели не NULL*/
    if(search_res == NULL){
        return false;
    }
    if(key == NULL){
        return false;
    }
    if(base == NULL){
        return false;
    }
    if(compare == NULL){
        return false;
    }
    if(size == 0){
        return false;
    }

    *search_res = bsearch(key, base, count, size, compare);
    if(*search_res != NULL){
        return true;
    }
    *search_res = NULL;
    return false;
}

bool memory_rsearch(void** search_res, void *key, void *base, size_t count, size_t size, memory_func_comp_t compare)
{
    /*Проверка, что указатели не NULL*/
    if(search_res == NULL){
        return false;
    }
    if(key == NULL){
        return false;
    }
    if(base == NULL){
        return false;
    }
    if(compare == NULL){
        return false;
    }
    if(size == 0){
        return false;
    }

    for(size_t i = 0; i < count; i++){
        if(compare(base + (i * size), key) == 0){
            *search_res =  base + (i * size);
            return true;
        }
    }
    *search_res = NULL;
    return false;
}

bool memory_dump(FILE* out, void *ptr, size_t len, uintmax_t catbyte, uintmax_t column)
{
    /*Проверка, что указатель не NULL*/
    if(out == NULL){
        return false;
    }

    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Проверка, что числа есть степень 2*/
    if(catbyte == 0 || (catbyte & (catbyte - 1)) != 0){
        return false;
    }
    if(column == 0 || (column & (column - 1)) != 0){
        return false;
    }

    if(len == 0) {
        memory_t *mem = ptr - offsetof(memory_t, memory_ptr);

        /*Проверяем корректность структуры*/
        if(mem->memory_canary != YAYA_MEMORY_VALUE_CANARY){
            return false;
        }

        ptr = mem->memory_ptr;
        len = mem->memory_request;
    }

    uintmax_t const col1 = (1 + 2 + sizeof(uintptr_t) * 2 + 1);
    uintmax_t const col2 = (((column * catbyte) * 2) + column) + 1;

    /*Шапка*/
    {
        fprintf(out, "╭");
        for(uintmax_t i = 0; i < col1; i++){
            fprintf(out, "-");
        }
        fprintf(out, "┬");
        for(uintmax_t i = 0; i < col2; i++){
            fprintf(out, "-");
        }
        fprintf(out, "╮");
        fprintf(out, "\n");
        fprintf(out, "| Len: %8" PRIuMAX " byte | ", len);
        for(uintmax_t i = 0; i < column; i++){
            for(uintmax_t j = 0; j < catbyte; j++){
                fprintf(out, "%02" PRIXMAX "", i*catbyte+j);
            }
            fprintf(out, " ");
        }
        fprintf(out, "|");
        fprintf(out, "\n");
        fprintf(out, "├");
        for(uintmax_t i = 0; i < col1; i++){
            fprintf(out, "-");
        }
        fprintf(out, "┼");
        for(uintmax_t i = 0; i < col2; i++){
            fprintf(out, "-");
        }
        fprintf(out, "┤");
        fprintf(out, "\n");
    }

    /*Тело*/
    {
        const uint8_t mod_bit = 0x10;
        uint8_t *nadr = ptr - (((uintptr_t)ptr % (column * catbyte)) % mod_bit);

        uintmax_t pos = 0;
        while(pos < len){
            if(pos == 0){
                fprintf(out, "| 0x%0*" PRIXPTR " | ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)nadr);
            }else{
                fprintf(out, "| 0x%0*" PRIXPTR " | ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)ptr + pos);
            }

            for(uintmax_t i = 0; i < column; i++){
                uintmax_t ind = 0;

                if(pos == 0){
                    for(ind = 0; ind < catbyte; ind++){
                        if(&nadr[i * catbyte + ind] == ptr){
                            goto L1;
                        }
                        fprintf(out, "..");
                    }
                    fprintf(out, " ");
                    continue;
                }
L1:
                for(uintmax_t k = ind; k < catbyte; k++){
                    if(pos < len){
                        fprintf(out, "%02" PRIx8 "", ((uint8_t*)ptr)[pos]);
                        pos++;

                        if(pos == len){
                            continue;
                        }
                    }else{
                        fprintf(out, "..");
                    }
                }
                fprintf(out, " ");
            }
            fprintf(out, "|");
            fprintf(out, "\n");
        }
    }

    /*Подвал*/
    {
        fprintf(out, "╰");
        for(uintmax_t i = 0; i < col1; i++){
            fprintf(out, "-");
        }
        fprintf(out, "┴");
        for(uintmax_t i = 0; i < col2; i++){
            fprintf(out, "-");
        }
        fprintf(out, "╯");
    }
    fprintf(out, "\n");

    fflush(out);
    return true;
}

static uint64_t bit_sequence(void *ptr, uintmax_t offset, uintmax_t len)
{
    uint8_t* bytes      = (uint8_t*)ptr;
    uint64_t result     = 0;
    uint32_t result32_1 = 0;
    uint32_t result32_2 = 0;

    if(len == 0){
        return 0;
    }

#define CU(x) (x##U)

    if(len <= CU(16)){
        for (uint64_t i = 0U; i < len; i++) {
            result |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        return result;
    }
    if(len <= CU(32)){
        for (uint64_t i = 0U; i < len; i++) {
            result32_1 |= ((uint64_t)(bytes[(offset + i) / CU(8)] >> ((offset + i) % CU(8))) & CU(1)) << i;
        }
        result = result32_1;
        return result;
    }
    if(len <= CU(64)){
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

bool memory_look(FILE* out, void *ptr, size_t struct_count, size_t struct_size, intmax_t bit_len_list[])
{
    /*Проверка, что указатель не NULL*/
    if(out == NULL){
        return false;
    }

    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        return false;
    }

    /* Проверка и отладочная информация */
    {
        /* Объявление вычислимых констант */
        uintmax_t list_bit_sum   = 0;

        /* Вычисление констант */
        for(uintmax_t i = 0; 0 == 0; i++) {
            if(bit_len_list[i] == 0){
                break;
            }
            if(bit_len_list[i] > 0){
                list_bit_sum += (uintmax_t)(+bit_len_list[i]);
            }else{
                list_bit_sum += (uintmax_t)(-bit_len_list[i]);
            }
        }

        /* Проверка, что переданый размер структуры и массив отступов совпадают */
        if((struct_size * __CHAR_BIT__) != list_bit_sum) {
            fprintf(out, "ERROR: (struct_size * __CHAR_BIT__) != |list_bit_sum| \n");
            return false;
        }
    }

    /* Таблица */
    {
        intmax_t col1 = (1 + 2 + (int)(sizeof(uintptr_t) * 2) + 1);
        intmax_t col2 = 1;
        intmax_t list_count = 0;
        intmax_t bits_count = 0;

        for(intmax_t i = 0; ; i++){
            if(bit_len_list[i] == 0){
                list_count = i;
                break;
            }
            if(bit_len_list[i] < 0){
                continue;
            }
            bits_count += bit_len_list[i];
            col2 += bit_len_list[i] / 4 + 1;
            if(bit_len_list[i] < 4 || bit_len_list[i] % 4 > 0){
                col2 += 1;
            }
        }

        /* Шапка */
        {
            fprintf(out, "╭");
            for(intmax_t i = 0; i < col1; i++){
                fprintf(out, "-");
            }
            fprintf(out, "┬");
            for(intmax_t i = 0; i < col2; i++){
                fprintf(out, "-");
            }
            fprintf(out, "╮");
            fprintf(out, "\n");
            fprintf(out, "| S: %5" PRIuMAX "/%-5"PRIuMAX" bit | ", bits_count, struct_size * __CHAR_BIT__);
            for(intmax_t i = 0; i < list_count; i++){
                if(bit_len_list[i] < 0){
                    continue;
                }
                fprintf(out, "%*"PRIiMAX" ", (int)((bit_len_list[i] / 4) + ((bit_len_list[i] < 4 || bit_len_list[i] % 4 > 0) ? 1 : 0)), bit_len_list[i]);
            }
            fprintf(out, "|");
            fprintf(out, "\n");
            fprintf(out, "├");
            for(intmax_t i = 0; i < col1; i++){
                fprintf(out, "-");
            }
            fprintf(out, "┼");
            for(intmax_t i = 0; i < col2; i++){
                fprintf(out, "-");
            }
            fprintf(out, "┤");
            fprintf(out, "\n");
        }
        /* Тело */
        {
            for(uintmax_t i = 0; i < struct_count; i++){
                fprintf(out, "| 0x%0*" PRIXPTR " | ", (int)(sizeof(uintptr_t) * 2), (uintptr_t)(ptr) + (i * (uintptr_t)(struct_size)));
                intmax_t offset = 0;
                for(intmax_t k = 0; k < list_count; k++){
                    if(bit_len_list[k] > 0){
                        uintmax_t res = bit_sequence(ptr, (uintptr_t)(struct_size * __CHAR_BIT__ * i) + (uintptr_t)(offset), (uintmax_t)(bit_len_list[k]));
                        fprintf(out, "%0*" PRIxMAX " ", (int)(bit_len_list[k] / 4 + ((bit_len_list[k] < 4 || bit_len_list[k] % 4 > 0) ? 1 : 0)), res);
                        offset += bit_len_list[k];
                    }else{
                        offset += -bit_len_list[k];
                    }
                }
                fprintf(out, "|\n");
            }
        }
        /*Подвал*/
        {
            fprintf(out, "╰");
            for(intmax_t i = 0; i < col1; i++){
                fprintf(out, "-");
            }
            fprintf(out, "┴");
            for(intmax_t i = 0; i < col2; i++){
                fprintf(out, "-");
            }
            fprintf(out, "╯");
        }
        fprintf(out, "\n");
    }

    fflush(out);
    return true;
}
