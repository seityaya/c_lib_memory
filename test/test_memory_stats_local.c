//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.10
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#define YAYA_MEMORY_STATS_GLOBAL 0
#include "yaya_memory_stats.h"

void test_param_stats_local() {
    printf("test_param_stats_local\n");

    memory_stats_t* memory_stats = NULL;

    if(memory_stats_init(&memory_stats)){
        printf("00 OK\n");
    }else{
        printf("ER\n");
    }

    if(!memory_new(memory_stats, NULL, NULL, 0, 0))
    {
        printf("01 OK\n");
    }else{
        printf("ER\n");
    }


    void *ptr = NULL;


    if(!memory_new(memory_stats, &ptr, NULL, 0, 0))
    {
        printf("02 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(memory_stats, &ptr, ptr, 0, 0))
    {
        printf("03 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(memory_stats, NULL, NULL, 1, sizeof(char)))
    {
        printf("04 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(memory_stats, NULL, ptr, 1, sizeof(char)))
    {
        printf("05 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(memory_stats, ptr, ptr, 1, sizeof(char)))
    {
        printf("06 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_del(memory_stats, ptr))
    {
        printf("07 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_del(memory_stats, &ptr))
    {
        printf("08 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_new(memory_stats, &ptr, &ptr, 1, sizeof(char)))
    {
        printf("09 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_del(memory_stats, &ptr))
    {
        printf("10 OK\n");
    }else{
        printf("ER\n");
    }

    if(!memory_del(memory_stats, &ptr))
    {
        printf("11 OK\n");
    }else{
        printf("ER\n");
    }


    if(ptr == NULL){
        printf("12 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_stats_out(memory_stats, stdout))
    {
        printf("13 OK\n");
    }else{
        printf("ER\n");
    }

    if(memory_stats_free(&memory_stats)){
        printf("14 OK\n");
    }else{
        printf("ER\n");
    }

    if(memory_stats == NULL){
        printf("15 OK\n");
    }else{
        printf("ER\n");
    }

    printf("\n");
    printf("\n");
    fflush(stdout);
}
