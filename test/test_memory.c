// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory.h"

void test_param() {
    printf("test_param\n");

    void *ptr = NULL;
    void *ptr_tmp = NULL;

    {
        ptr = NULL;

        if (memory_req(&ptr, 1, sizeof(char))) {
            printf("A-1 OK\n");
        } else {
            printf("A-1 ERROR\n");
        }

        if (ptr != NULL) {
            printf("A-2 OK\n");
        } else {
            printf("A-2 ERROR\n");
        }

        if (memory_ret(&ptr)) {
            printf("A-3 OK\n");
        } else {
            printf("A-3 ERROR\n");
        }

        if (ptr == NULL) {
            printf("A-4 OK\n");
        } else {
            printf("A-4 ERROR\n");
        }
    }

    {
        ptr = NULL;
        ptr_tmp = NULL;

        if (memory_req(&ptr, 1, sizeof(char))) {
            printf("B-1 OK\n");
        } else {
            printf("B-1 ERROR\n");
        }

        ptr_tmp = ptr;

        if (memory_req(&ptr, 2, sizeof(char))) {
            printf("B-2 OK\n");
        } else {
            printf("B-2 ERROR\n");
        }

        if (ptr == ptr_tmp) {
            printf("B-3 OK\n");
        } else {
            printf("B-3 ERROR\n");
        }

        if (memory_ret(&ptr)) {
            printf("B-4 OK\n");
        } else {
            printf("B-4 ERROR\n");
        }

        if (!memory_ret(&ptr)) {
            printf("B-5 OK\n");
        } else {
            printf("B-5 ERROR\n");
        }

        if (ptr == NULL) {
            printf("B-6 OK\n");
        } else {
            printf("B-6 ERROR\n");
        }
    }

    {
        if (!memory_req(NULL, 0, 0)) {
            printf("C-1 OK\n");
        } else {
            printf("C-1 ERROR\n");
        }

        if (!memory_ret(NULL)) {
            printf("C-2 OK\n");
        } else {
            printf("C-2 ERROR\n");
        }
    }

    {
        ptr = NULL;

        if (!memory_req(&ptr, 0, 0)) {
            printf("D-1 OK\n");
        } else {
            printf("D-1 ERROR\n");
        }
        memory_ret(&ptr);

        if (!memory_req(&ptr, 1, 0)) {
            printf("D-2 OK\n");
        } else {
            printf("D-2 ERROR\n");
        }
        memory_ret(&ptr);

        if (!memory_req(NULL, 1, sizeof(char))) {
            printf("D-3 OK\n");
        } else {
            printf("D-3 ERROR\n");
        }
        memory_ret(&ptr);

        if (memory_req(&ptr, 0, sizeof(char))) {
            printf("D-4 OK\n");
        } else {
            printf("D-4 ERROR\n");
        }
        memory_ret(&ptr);
    }

    {
        if (!memory_req(NULL, 0, 0)) {
            printf("E-1 OK\n");
        } else {
            printf("E-1 ERROR\n");
        }

        if (!memory_req(NULL, 1, 0)) {
            printf("E-2 OK\n");
        } else {
            printf("E-2 ERROR\n");
        }

        if (!memory_req(NULL, 0, 1)) {
            printf("E-3 OK\n");
        } else {
            printf("E-3 ERROR\n");
        }

        if (!memory_req(NULL, 1, 1)) {
            printf("E-4 OK\n");
        } else {
            printf("E-4 ERROR\n");
        }

        if (!memory_req(&ptr, 0, 0)) {
            printf("E-5 OK\n");
        } else {
            printf("E-5 ERROR\n");
        }

        if (memory_req(&ptr, 0, sizeof(char))) {
            printf("E-6 OK\n");
        } else {
            printf("E-6 ERROR\n");
        }

        if (memory_req(&ptr, 1, sizeof(char))) {
            printf("E-7 OK\n");
        } else {
            printf("E-7 ERROR\n");
        }
    }

    {
        ptr = NULL;

        if (memory_req(&ptr, 1, sizeof(char))) {
            printf("F-8 OK\n");
        } else {
            printf("F-8 ERROR\n");
        }

        if (memory_req(&ptr, 2, sizeof(char))) {
            printf("F-9 OK\n");
        } else {
            printf("F-9 ERROR\n");
        }

        if (memory_req(&ptr, 1, sizeof(char))) {
            printf("F-1 OK\n");
        } else {
            printf("F-1 ERROR\n");
        }

        if (memory_ret(&ptr)) {
            printf("F-4 OK\n");
        } else {
            printf("F-4 ERROR\n");
        }
    }

    {
        ptr_tmp = NULL;

        if (memory_req(&ptr, 1, sizeof(char))) {
            printf("I-1 OK\n");
        } else {
            printf("I-1 ERROR\n");
        }

        ptr_tmp = ptr;
        if (memory_req(&ptr, 50, sizeof(char))) {
            printf("I-2 OK\n");
        } else {
            printf("I-2 ERROR\n");
        }

        if (ptr == ptr_tmp) {
            printf("I-3 OK\n");
        } else {
            printf("I-3 ERROR\n");
            printf("ptr     %p\n", ptr);
            printf("ptr_tmp %p\n", ptr_tmp);
        }

        if (memory_ret(&ptr)) {
            printf("I-4 OK\n");
        } else {
            printf("I-4 ERROR\n");
        }
    }

    printf("\n");
    printf("\n");
    fflush(stdout);
}

static bool test_memory_fill(void *ptr) {
    /*Проверка, что указатель не NULL*/
    if (ptr == NULL) {
        printf("ERROR *POINTER\n");
        fflush(stdout);
        return false;
    }

    /*Указатели под структуру памяти*/
    memory_t *mem = NULL;

    /*Помещаем указатель со смещением*/
    mem = ptr - offsetof(memory_t, memory_ptr);

    size_t produce = malloc_usable_size(mem);

    /*Заполнение памяти*/
    memset(mem->memory_ptr, 0x11, produce - offsetof(memory_t, memory_ptr));
    memset(mem->memory_ptr, 0xff, 1);
    memset(&mem->memory_ptr[mem->memory_request - 1], 0xff, 1);
    memset(&mem->memory_ptr[mem->memory_request], 0x88, produce - mem->memory_request - offsetof(memory_t, memory_ptr));

    return true;
}

void test_dump() {

    printf("test_dump\n");

    void *ptr = NULL;

    memory_req(&ptr, 16, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    memory_req(&ptr, 17, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    memory_req(&ptr, 42, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    test_memory_fill(ptr);
    memory_dump(stdout, ptr, 0, 1, 16);

    memory_req(&ptr, 35, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    test_memory_fill(ptr);
    memory_dump(stdout, ptr, 0, 1, 16);

    printf("\n");

    memory_req(&ptr, 33, sizeof(char));
    test_memory_fill(ptr);

    memory_dump(stdout, ptr, 0, 1, 4);
    memory_dump(stdout, ptr, 0, 4, 2);
    memory_dump(stdout, ptr, 0, 4, 2);
    memory_dump(stdout, ptr, 0, 1, 8);
    memory_dump(stdout, ptr, 0, 1, 16);
    memory_dump(stdout, ptr, 0, 1, 32);
    memory_dump(stdout, ptr, 0, 1, 64);

    void *ptr_save = ptr;
    memory_ret(&ptr);
    memory_dump(stdout, ptr_save, 33, 1, 64);

    printf("\n");
    fflush(stdout);
}

void test_look() {
    printf("test_look\n");

    typedef struct S {
        uint8_t x1 : 3;
        uint8_t x2 : 1;
        uint8_t x3 : 4;
        uint16_t a;
        int8_t b;
        uint16_t c;
        uint8_t d;
        int32_t e;
        uint32_t f1 : 21;
        uint32_t f2 : 11;
        void *p;
    } S;

    int a = 0;
    printf("%p\n", &a);

    S t[] = {{7, 1, 10, 5, -1, 3, 1, 0, 17, 15, (int *)(3)},
             {7, 1, 10, 5, -1, 2, 1, 1, 17, 15, &a},
             {7, 1, 10, 5, -1, 1, 1, -1, 17, 15, &t},
             {7, 1, 10, 5, 0 - 1, 0, 255, 2, 17, 15, &t},
             {7, 1, 10, 5, 0, 0xFFFF, 0, -2, 17, 15, &t[0].a}};

    memory_dump(stdout, t, sizeof(S) * 5, 1, 16);

    memory_look(stdout, &t, 5, sizeof(S), ({ (intmax_t[]){3, 1, 4, 8, 16, 8, 8, 16, 8, 24, 32, 21, 11, 32, sizeof(void *) * __CHAR_BIT__, 0}; }));
    memory_look(stdout, &t, 5, sizeof(S), memory_bit_len_list(3, 1, 4, -8, 16, 8, 8, 16, 8, -24, 32, 21, 11, 32, 64));

    void *ptr = NULL;

    memory_req(&ptr, 17, sizeof(char));
    memory_fill(ptr, 0xff);
    memory_look(stdout, ptr, 1, memory_size(ptr), memory_bit_len_list(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
    memory_ret(&ptr);

    printf("\n");
    fflush(stdout);
}

void test_swap() {
    printf("test_swap\n");

    {
        int8_t x = 0;
        int8_t y = 1;

        memory_swap(&x, &y, sizeof(int8_t));

        if (1 == x && 0 == y) {
            printf("01 OK\n");
        } else {
            printf("ERROR\n");
        }
    }

    {
        int16_t x = -1;
        int16_t y = +1;

        memory_swap(&x, &y, sizeof(int16_t));

        if (+1 == x && -1 == y) {
            printf("02 OK\n");
        } else {
            printf("ERROR\n");
        }
    }

    {
        int32_t x = -136446856;
        int32_t y = +978321345;

        memory_swap(&x, &y, sizeof(int32_t));

        if (+978321345 == x && -136446856 == y) {
            printf("03 OK\n");
        } else {
            printf("ERROR\n");
        }
    }

    printf("\n");
    fflush(stdout);
}

void test_shuf() {
    printf("test_shuf\n");
    const int8_t count_mas = 10;
    const int8_t count_test = 127;
    int8_t *mas = NULL;

    memory_req((void **)(&mas), (size_t)(count_mas), sizeof(int8_t));

    for (int8_t i = 0; i < count_mas; i++) {
        mas[i] = i;
    }

    for (int8_t i = 0; i < count_mas; i++) {
        printf("%" PRIi8 " ", mas[i]);
    }
    printf("\n");

    int32_t count_shuf = 0;
    for (int8_t a = 0; a < count_test; a++) {
        for (int8_t i = 0; i < count_mas; i++) {
            mas[i] = i;
        }

        memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), (uint)(a + 1), (void (*)(unsigned int))srand, (int (*)(void))rand);

        for (int8_t i = 0; i < count_mas; i++) {
            if (mas[i] != i) {
                count_shuf++;
            }
        }

        for (int8_t i = 0; i < count_mas; i++) {
            printf("%" PRIi8 " ", mas[i]);
        }
        printf("\n");
    }

    float mean = ((float)(count_shuf) / (float)(count_mas * count_test));

    if (mean > 0.8) {
        printf("OK\n");
    } else {
        printf("ERROR\n");
    }

    memory_ret((void **)(&mas));

    printf("\n");
    fflush(stdout);
}

static int comp(const int8_t *i, const int8_t *j) { return *i - *j; }

void test_sort() {
    printf("test_sort\n");

    const int8_t count_mas = 10;
    int8_t *mas = {0};

    memory_req((void **)(&mas), (size_t)(count_mas), sizeof(int8_t));

    for (int8_t i = 0; i < count_mas; i++) {
        mas[i] = i;
    }

    memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), 1, (void (*)(unsigned int))srand, (int (*)(void))rand);
    memory_sort(mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp));

    int8_t count_sort = 0;
    for (int8_t i = 0; i < count_mas; i++) {
        if (mas[i] == i) {
            count_sort++;
        }
    }

    if (count_mas == count_sort) {
        printf("OK\n");
    } else {
        printf("ERROR\n");
    }

    memory_ret((void **)(&mas));

    printf("\n");
    fflush(stdout);
}

void test_search() {
    printf("test_search\n");

    const int8_t count_mas = 10;

    int8_t *mas = NULL;
    memory_req((void **)(&mas), (size_t)(count_mas), sizeof(int8_t));

    for (int8_t i = 0; i < count_mas; i++) {
        mas[i] = i;
    }

    uint8_t value = 11;
    int8_t *serch_r = NULL;
    int8_t *serch_b = NULL;

    if (!memory_rsearch((void **)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("1 OK\n");
    } else {
        printf("ERROR\n");
    }

    if (!memory_bsearch((void **)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("2 OK\n");
    } else {
        printf("ERROR\n");
    }

    if (serch_r == NULL && serch_b == NULL) {
        printf("3 OK\n");
    } else {
        printf("ERROR\n");
    }

    value = 9;
    if (memory_rsearch((void **)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("4 OK\n");
    } else {
        printf("ERROR\n");
    }
    if (memory_bsearch((void **)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("5 OK\n");
    } else {
        printf("ERROR\n");
    }

    if (serch_r == &mas[value] && serch_b == &mas[value]) {
        printf("6 OK\n");
    } else {
        printf("ERROR\n");
    }

    memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), 1, NULL, NULL);

    for (int8_t i = 0; i < count_mas; i++) {
        printf("%" PRIi8 " ", mas[i]);
    }
    printf("\n");
    int8_t position = 8;

    if (memory_rsearch((void **)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("7 OK\n");
    } else {
        printf("ERROR\n");
    }

    if (serch_r == &mas[position]) {
        printf("8 OK\n");
    } else {
        printf("ERROR\n");
    }

    if (memory_bsearch((void **)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("8 OK\n");
    } else {
        printf("ERROR\n");
    }

    if (serch_b == &mas[position]) {
        printf("9 OK\n");
    } else {
        printf("ERROR\n");
    }

    memory_sort(mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp));

    if (memory_bsearch((void **)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp))) {
        printf("A OK\n");
    } else {
        printf("ERROR\n");
    }

    if (serch_b == &mas[value]) {
        printf("B OK\n");
    } else {
        printf("ERROR\n");
    }

    if (memory_step(mas, serch_b, sizeof(int8_t)) == value) {
        printf("C OK\n");
    } else {
        printf("ERROR\n");
    }

    memory_ret((void **)&mas);

    printf("\n");
    fflush(stdout);
}
