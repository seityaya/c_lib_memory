//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.10
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory.h"

#define printf(...)     printf(__VA_ARGS__); fflush(stdout);

void test_param() {
    printf("test_param\n");
    void *ptr = NULL;
    void *ptr_old = NULL;
    void *ptr_new = NULL;
    void *ptr_tmp = NULL;

    {
        ptr = NULL;

        if(memory_new(&ptr, &ptr, 1, sizeof(char)))
        {
            printf("A 1/3 OK\n");
        }else{
            printf("ER\n");
        }

        if(memory_del(&ptr))
        {
            printf("A 2/3 OK\n");
        }else{
            printf("ER\n");
        }

        if(ptr == NULL)
        {
            printf("A 3/3 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        ptr_old = NULL;
        ptr_new = NULL;

        if(memory_new(&ptr_old, NULL, 1, sizeof(char)))
        {
            printf("B 1/4 OK\n");
        }else{
            printf("ER\n");
        }

        if(memory_new(&ptr_new, &ptr_old, 10, sizeof(char)))
        {
            printf("B 2/4 OK\n");
        }else{
            printf("ER\n");
        }

        if(ptr_old == NULL)
        {
            printf("B 3/4 OK\n");
        }else{
            printf("ER\n");
        }

        if(memory_del(&ptr_new))
        {
            printf("B 4/4 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        ptr = NULL;

        if(!memory_new(&ptr, &ptr, 0, 0))
        {
            printf("C 1/5 OK\n");
        }else{
            printf("ER\n");
        }

        if(!memory_new(&ptr, &ptr, 1, 0))
        {
            printf("C 2/5 OK\n");
        }else{
            printf("ER\n");
        }

        if(!memory_new(&ptr, &ptr, 0, sizeof(char)))
        {
            printf("C 3/5 OK\n");
        }else{
            printf("ER\n");
        }

        if(!memory_new(NULL, &ptr, 1, sizeof(char)))
        {
            printf("C 4/5 OK\n");
        }else{
            printf("ER\n");
        }

        if(memory_new(&ptr, ptr, 1, sizeof(char)))
        {
            printf("C 5/5 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        if(!memory_new(NULL, NULL, 0, 0))
        {
            printf("D 1/2 OK\n");
        }else{
            printf("ER\n");
        }

        if(!memory_del(NULL))
        {
            printf("D 2/2 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        ptr_old = NULL;
        ptr_new = NULL;
        ptr_tmp = NULL;

        if(memory_new(&ptr_old, NULL, 1, sizeof(char)))
        {
            printf("E 1/8 OK\n");
        }else{
            printf("ER\n");
        }

        ptr_tmp = ptr_old;
        if(memory_new(&ptr_new, &ptr_tmp, 50, sizeof(char)))
        {
            printf("E 2/8 OK\n");
        }else{
            printf("ER\n");
        }

        if(ptr_old == ptr_new)
        {
            printf("E 3/8 OK\n");
            printf("        ptr_old %p\n", ptr_old);
            printf("        ptr_new %p\n", ptr_new);
        }else{
            printf("ER\n");
        }

        if(memory_new(&ptr_tmp, NULL, 100, sizeof(char)))
        {
            printf("E 4/8 OK\n");
        }else{
            printf("ER\n");
        }

        if(memory_new(&ptr_new, &ptr_new, 100, sizeof(char)))
        {
            printf("E 5/8 OK\n");
        }else{
            printf("ER\n");
        }

        if(ptr_old != ptr_new)
        {
            printf("E 6/8 OK\n");
            printf("        ptr_old %p\n", ptr_old);
            printf("        ptr_new %p\n", ptr_new);
        }else{
            printf("ER\n");
        }

        if(memory_del(&ptr_tmp))
        {
            printf("E 7/8 OK\n");
        }else{
            printf("ER\n");
        }

        if(memory_del(&ptr_new))
        {
            printf("E 8/8 OK\n");
        }else{
            printf("ER\n");
        }
    }

    printf("\n");
    printf("\n");
    fflush(stdout);
}

void test_dump(){
    bool test_memory_fill(void *ptr){
        /*Проверка, что указатель не NULL*/
        if(ptr == NULL){
            printf("ERROR *POINTER\n"); fflush(stdout);
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

    printf("test_dump\n");

    void *ptr = NULL;

    memory_new(&ptr, &ptr, 16, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    memory_new(&ptr, &ptr, 17, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    memory_new(&ptr, &ptr, 42, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    test_memory_fill(ptr);
    memory_dump(stdout, ptr, 0, 1, 16);

    memory_new(&ptr, &ptr, 35, sizeof(char));
    memory_dump(stdout, ptr, 0, 1, 16);

    test_memory_fill(ptr);
    memory_dump(stdout, ptr, 0, 1, 16);

    printf("\n");

    memory_new(&ptr, &ptr, 33, sizeof(char));
    test_memory_fill(ptr);

    memory_dump(stdout, ptr, 0, 1, 4);
    memory_dump(stdout, ptr, 0, 4, 2);
    memory_dump(stdout, ptr, 0, 4, 2);
    memory_dump(stdout, ptr, 0, 1, 8);
    memory_dump(stdout, ptr, 0, 1, 16);
    memory_dump(stdout, ptr, 0, 1, 32);
    memory_dump(stdout, ptr, 0, 1, 64);

    void *ptr_save = ptr;
    memory_del(&ptr);
    memory_dump(stdout, ptr_save, 33, 1, 64);

    printf("\n");
    fflush(stdout);
}

void test_look(){
    printf("test_look\n");

    typedef struct S {
        uint8_t  x1:3;
        uint8_t  x2:1;
        uint8_t  x3:4;
        uint16_t a;
        int8_t   b;
        uint16_t c;
        uint8_t  d;
        int32_t  e;
        uint32_t f1:21;
        uint32_t f2:11;
        void*    p;
    } S;

    int a = 0;
    printf("%p\n", &a);

    S t[] = {
        {7, 1, 10, 5,  -1,       3,   1,   0, 17, 15, (int*)(3)},
        {7, 1, 10, 5,  -1,       2,   1,   1, 17, 15,        &a},
        {7, 1, 10, 5,  -1,       1,   1,  -1, 17, 15,        &t},
        {7, 1, 10, 5, 0-1,       0, 255,   2, 17, 15,        &t},
        {7, 1, 10, 5,   0,  0xFFFF,   0,  -2, 17, 15,   &t[0].a}
    };

    memory_dump(stdout, t, sizeof(S) * 5, 1, 16);

    memory_look(stdout, &t, 5, sizeof(S), ({ (intmax_t[]) { 3, 1, 4, 8, 16, 8, 8, 16, 8, 24, 32, 21, 11, 32, sizeof(void*) * __CHAR_BIT__, 0}; }) );
    memory_look(stdout, &t, 5, sizeof(S), memory_bit_len_list(3, 1, 4, -8, 16, 8, 8, 16, 8, -24, 32, 21, 11, 32, 64));


    void *ptr = NULL;

    memory_new(&ptr, &ptr, 17, sizeof(char));
    memory_fill(ptr, 0xff);
    memory_look(stdout, ptr, 1, memory_size(ptr), memory_bit_len_list(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
    memory_del(&ptr);

    printf("\n");
    fflush(stdout);
}

void test_swap() {
    printf("test_swap\n");

    {
        int8_t x = 0;
        int8_t y = 1;

        memory_swap(&x, &y, sizeof(int8_t));

        if(  1 == x &&  0 == y){
            printf("01 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        int16_t x = -1;
        int16_t y = +1;

        memory_swap(&x, &y, sizeof(int16_t));

        if( +1 == x && -1 == y){
            printf("02 OK\n");
        }else{
            printf("ER\n");
        }
    }

    {
        int32_t x = -136446856;
        int32_t y = +978321345;

        memory_swap(&x, &y, sizeof(int32_t));

        if( +978321345 ==  x && -136446856 == y){
            printf("03 OK\n");
        }else{
            printf("ER\n");
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

    memory_new((void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));

    for(int8_t i = 0; i < count_mas; i++){
        mas[i] = i;
    }

    for(int8_t i = 0; i < count_mas; i++){
        printf("%"PRIi8 " ", mas[i]);
    }
    printf("\n");

    int32_t count_shuf = 0;
    for(int8_t a = 0; a < count_test; a++){
        for(int8_t i = 0; i < count_mas; i++){
            mas[i] = i;
        }

        memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), (uint)(a + 1), (void(*)(unsigned int))srand, (int(*)(void)) rand);


        for(int8_t i = 0; i < count_mas; i++){
            if(mas[i] != i){
                count_shuf++;
            }
        }

        for(int8_t i = 0; i < count_mas; i++){
            printf("%"PRIi8 " ", mas[i]);
        }
        printf("\n");
    }

    float mean = ((float)(count_shuf) / (float)(count_mas * count_test));

    if(mean > 0.8){
        printf("OK\n");
    }else{
        printf("ER\n");
    }

    memory_del((void**)(&mas));

    printf("\n");
    fflush(stdout);
}

static int comp (const int8_t *i, const int8_t *j) {
    return *i - *j;
}

void test_sort() {
    printf("test_sort\n");

    const int8_t count_mas = 10;
    int8_t *mas = {0};

    memory_new((void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));

    for(int8_t i = 0; i < count_mas; i++){
        mas[i] = i;
    }

    memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), 1, (void(*)(unsigned int))srand, (int(*)(void)) rand);
    memory_sort(mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp));

    int8_t count_sort = 0;
    for(int8_t i = 0; i < count_mas; i++){
        if(mas[i] == i){
            count_sort++;
        }
    }

    if(count_mas == count_sort){
        printf("OK\n");
    }else{
        printf("ER\n");
    }

    memory_del((void**)(&mas));

    printf("\n");
    fflush(stdout);
}

void test_search() {
    printf("test_search\n");

    const int8_t count_mas = 10;

    int8_t *mas = NULL;
    memory_new((void**)(&mas), NULL, (size_t)(count_mas), sizeof(int8_t));

    for(int8_t i = 0; i < count_mas; i++){
        mas[i] = i;
    }

    uint8_t value = 11;
    int8_t *serch_r = NULL;
    int8_t *serch_b = NULL;

    if(!memory_rsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("1 OK\n");
    }else{
        printf("ER\n");
    }

    if(!memory_bsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("2 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_r == NULL && serch_b == NULL){
        printf("3 OK\n");
    }else{
        printf("ER\n");
    }

    value = 9;
    if(memory_rsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("4 OK\n");
    }else{
        printf("ER\n");
    }
    if(memory_bsearch((void**)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("5 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_r == &mas[value] && serch_b == &mas[value]){
        printf("6 OK\n");
    }else{
        printf("ER\n");
    }

    memory_shuf(mas, (size_t)(count_mas), sizeof(int8_t), 1, NULL, NULL);


    for(int8_t i = 0; i < count_mas; i++){
        printf("%" PRIi8 " ", mas[i]);
    }
    printf("\n");
    int8_t position = 8;

    if(memory_rsearch((void**)&serch_r, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("7 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_r == &mas[position]){
        printf("8 OK\n");
    }else{
        printf("ER\n");
    }

    if(memory_bsearch((void**)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("8 OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_b == &mas[position]){
        printf("9 OK\n");
    }else{
        printf("ER\n");
    }

    memory_sort(mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp));

    if(memory_bsearch((void**)&serch_b, &value, mas, (size_t)(count_mas), sizeof(int8_t), (memory_func_comp_t)(comp)))
    {
        printf("A OK\n");
    }else{
        printf("ER\n");
    }

    if(serch_b == &mas[value]){
        printf("B OK\n");
    }else{
        printf("ER\n");
    }

    if(memory_step(mas, serch_b, sizeof(int8_t)) == value){
        printf("C OK\n");
    }else{
        printf("ER\n");
    }

    memory_del((void**)(&mas));

    printf("\n");
    fflush(stdout);
}
