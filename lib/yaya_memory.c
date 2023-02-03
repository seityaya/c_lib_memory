//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2022.12
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#include "inttypes.h"
#include "malloc.h"
#include "stdio.h"
#include "string.h"

#define USE_MEMORY_STATS 1
#include "yaya_memory.h"

#if USE_MEMORY_STATS
bool memory_stats_init(mem_stats_t **mem_stats){
    if(mem_stats == NULL){
        return false;
    }

    if(*mem_stats == NULL){
        *mem_stats = (mem_stats_t*)malloc(sizeof(mem_stats_t));
        if(*mem_stats == NULL){
            return false;
        }

        memset(*mem_stats, 0, malloc_usable_size(*mem_stats));
        return true;
    }

    return false;
}
bool memory_stats_free(mem_stats_t **mem_stats){
    if(mem_stats != NULL){
        free(*mem_stats);
        *mem_stats = NULL;
        return true;
    }
    return false;
}
bool memory_stats_show(mem_stats_t *mem_stats){
    if(mem_stats != NULL){
        printf("Request:% 10" PRIiMAX "; ",   mem_stats->memory_request);
        printf("Produce:% 10" PRIiMAX "; ",   mem_stats->memory_produce);
        printf("Overhead:% 10" PRIiMAX "; ",  mem_stats->memory_produce - mem_stats->memory_request);
        printf("Release:% 10" PRIiMAX "; ",   mem_stats->memory_release);
        printf("USAGE:% 10" PRIiMAX "; ", mem_stats->memory_produce - mem_stats->memory_release);
        printf("NEW:% 10" PRIiMAX "; ",   mem_stats->memory_call_new);
        printf("RES:% 10" PRIiMAX "; ",   mem_stats->memory_call_res);
        printf("DEL:% 10" PRIiMAX "; ",   mem_stats->memory_call_del);
        printf("\n");
        fflush(stdout);
        return true;
    }
    return false;
}
#endif

bool memory_new(mem_stats_t *mem_stats, void **ptr, void *old_ptr, const size_t new_size_len)
{
    /*Указатели под структуру памяти*/
    mem_info_t *mem_old = {0};
    mem_info_t *mem_new = {0};

    /*Проверка, что запрошено не нулевой размер памяти*/
    if(new_size_len == 0){
        return false;
    }

    /*Проверка, что возвращать есть куда*/
    if(ptr == NULL){
        return false;
    }

    /*Если память не инициализирована, то указатель на предыдущую память NULL*/
    if(old_ptr == NULL){
        /*Выделение памяти под запрос и на хранение информации и указателя*/
        mem_new = malloc(new_size_len + sizeof(mem_info_t));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL){
            return false;
        }

        size_t produce = malloc_usable_size(mem_new);

        /*Зануление всего выделеного*/
        memset(mem_new, 0x00, produce);

        /*Сохранение информации о количестве памяти*/
        mem_new->memory_request = (intptr_t)new_size_len;
        mem_new->memory_produce = (intptr_t)produce;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

#if USE_MEMORY_STATS
        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_new++;
            mem_stats->memory_request += mem_new->memory_request;
            mem_stats->memory_produce += mem_new->memory_produce;
        }
#endif
    }
    /*Если память инициализированна, то указатель на предыдущую память не NULL*/
    else
    {
        /*Помещаем указатель со смещением*/
        mem_old = old_ptr - offsetof(mem_info_t, memory_ptr);

        /*Запоминаем сколько было выделено и сколько запрошено*/
        intptr_t old_size_p = mem_old->memory_produce;
        intptr_t old_size_r = mem_old->memory_request;

        /*Перераспределяем память*/
        mem_new = realloc(mem_old, new_size_len + sizeof(mem_info_t));

        /*Проверка, что память выделилась*/
        if(mem_new == NULL){
            return false;
        }

        /*Запоминаем сколько выделено и сколько запрошено*/
        intptr_t new_size_p = (intptr_t)malloc_usable_size(mem_new);
        intptr_t new_size_r = (intptr_t)new_size_len;

        /*Вычисление разницы*/
        intptr_t diff_r = new_size_r - old_size_r;
        intptr_t diff_p = new_size_p - old_size_p;

        /*Зануление хвоста выделеного*/
        if(diff_r > 0){
            intptr_t diff = new_size_p - (intptr_t)offsetof(mem_info_t, memory_ptr) - old_size_r;
            memset(mem_new->memory_ptr + old_size_r, 0x00, diff);
        }

        if(diff_r < 0){
            intptr_t diff = new_size_p - (intptr_t)offsetof(mem_info_t, memory_ptr) - new_size_r;
            memset(mem_new->memory_ptr + new_size_r, 0x00, diff);
        }

        /*Сохранение информации о количестве запрощеной памяти*/
        mem_new->memory_produce = new_size_p;
        mem_new->memory_request = new_size_r;

        /*Возвращение указателя на память для пользователя*/
        *ptr = mem_new->memory_ptr;

#if USE_MEMORY_STATS
        /*Сохранение статистики*/
        if(mem_stats != NULL){
            mem_stats->memory_call_res++;
            mem_stats->memory_request += diff_r;
            mem_stats->memory_produce += diff_p;
        }
#endif
    }
    return true;
}

bool memory_del(mem_stats_t *mem_stats, void **ptr)
{
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    if(*ptr == NULL){
        return false;
    }

    /*Указатели под структуру памяти*/
    mem_info_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = *ptr - offsetof(mem_info_t, memory_ptr);

#if USE_MEMORY_STATS
    /*Сохранение статистики*/
    if(mem_stats != NULL){
        mem_stats->memory_call_del++;
        mem_stats->memory_release += mem->memory_produce;
    }
#endif

    volatile uintptr_t size = mem->memory_produce;
    volatile uint8_t  *p    = (uint8_t*)(mem);
    while (size--){
        *p++ = 0;
    }

    free(mem);
    mem = NULL;
    *ptr = NULL;

    return true;
}

bool memory_zero(void *ptr){
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return false;
    }

    /*Указатели под структуру памяти*/
    mem_info_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = ptr - offsetof(mem_info_t, memory_ptr);

    /*Сохраняем значения*/
    volatile intptr_t size = mem->memory_request;
    volatile uint8_t  *p    = mem->memory_ptr;

    /*Заполняем нулями*/
    while (size--){
        *p++ = 0;
    }

    return true;
}

size_t memory_size(void *ptr){
    /*Проверка, что указатели не NULL*/
    if(ptr == NULL){
        return 0;
    }

    mem_info_t *mem = ptr - offsetof(mem_info_t, memory_ptr);
    return mem->memory_request;
}

bool memory_dump(void *ptr, size_t len, uintmax_t catbyte, uintmax_t column){
    /*Проверка, что указатель не NULL*/
    if(ptr == NULL){
        return false;
    }
    if(catbyte == 0 || (catbyte & (catbyte - 1)) != 0){
        return false;
    }

    if(column == 0 || (column & (column - 1)) != 0){
        return false;
    }

    if(len == 0){
        mem_info_t *mem = NULL;
        mem = ptr - offsetof(mem_info_t, memory_ptr);
        ptr = mem->memory_ptr;
        len = mem->memory_request;
    }

    uintmax_t const col1 = (1 + 2 + 16 + 1);
    uintmax_t const col2 = (((column * catbyte) * 2) + column) + 1;

   /*Шапка*/
    {
        printf("╭");
        for(uintmax_t i = 0; i < col1; i++){
            printf("-");
        }
        printf("┬");
        for(uintmax_t i = 0; i < col2; i++){
            printf("-");
        }
        printf("╮");

        printf("\n");

        printf("| %5"PRIuMAX"   %5"PRIuMAX"%-5"PRIuMAX" | ", len, catbyte, column);
        for(uintmax_t i = 0; i < column; i++){
            for(uintmax_t j = 0; j < catbyte; j++){
                printf("%02"PRIXMAX"", i*catbyte+j);
            }
            printf(" ");
        }
        printf("|");

        printf("\n");

        printf("├");
        for(uintmax_t i = 0; i < col1; i++){
            printf("-");
        }
        printf("┼");
        for(uintmax_t i = 0; i < col2; i++){
            printf("-");
        }
        printf("┤");

        printf("\n");
    }

    /*Тело*/
    {
        uintptr_t madr = ((uintptr_t)ptr % (column * catbyte)) % 0x10;
        uint8_t *nadr = ptr - madr;

        for(uintmax_t m = 0; m < len; ){
            if(m == 0){
                printf("| 0x%016" PRIXPTR " | ", (uintptr_t)nadr);
            }else{
                printf("| 0x%016" PRIXPTR " | ", (uintptr_t)ptr + m);
            }
            for(uintmax_t i = 0; i < column; i++){
                uintmax_t j = 0;

                if(m == 0){
                    for(j = 0; j < catbyte; j++){
                        if(&nadr[i * catbyte + j] == ptr){
                            goto L1;
                        }
                        printf("..");
                    }
                    printf(" ");
                    continue;
L1: ;
                }

                if(m < len){
                    for(uintmax_t l = j; l < catbyte; l++){
                        printf("%02" PRIx8 "", ((uint8_t*)ptr)[m]);
                        m++;

                        if(m == len){
                            j = l+1;
                            goto L2;
                        }
                    }
                }else{
L2: ;
                    for(uintmax_t l = j; l < catbyte; l++){
                        printf("..");
                    }
                }

                printf(" ");
            }
            printf("|");
            printf("\n");
        }
    }

    /*Подвал*/
    {
        printf("╰");
        for(uintmax_t i = 0; i < col1; i++){
            printf("-");
        }
        printf("┴");
        for(uintmax_t i = 0; i < col2; i++){
            printf("-");
        }
        printf("╯");
    }

    printf("\n");

    fflush(stdout);
    return true;
}
