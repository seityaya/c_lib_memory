//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.10
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

#define YAYA_MEMORY_STATS_GLOBAL 1
#include "yaya_memory_stats.h"

void test_param_stats_global() {
    printf("test_param_stats_global\n");

    if(memory_stats_init()){
        printf("00 OK\n");
    }else{
        printf("ER\n");
    }

    if(!memory_new(NULL, NULL, 0, 0))
    {
        printf("01 OK\n");
    }else{
        printf("ER\n");
    }


    void *ptr = NULL;


    if(!memory_new(&ptr, NULL, 0, 0))
    {
        printf("02 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(&ptr, ptr, 0, 0))
    {
        printf("03 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(NULL, NULL, 1, sizeof(char)))
    {
        printf("04 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(NULL, ptr, 1, sizeof(char)))
    {
        printf("05 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_new(ptr, ptr, 1, sizeof(char)))
    {
        printf("06 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_del(ptr))
    {
        printf("07 OK\n");
    }else{
        printf("ER\n");
    }


    if(!memory_del(&ptr))
    {
        printf("08 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_new(&ptr, &ptr, 1, sizeof(char)))
    {
        printf("09 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_del(&ptr))
    {
        printf("10 OK\n");
    }else{
        printf("ER\n");
    }

    if(!memory_del(&ptr))
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


    if(memory_stats_out(stdout))
    {
        printf("13 OK\n");
    }else{
        printf("ER\n");
    }


    if(memory_stats_free()){
        printf("14 OK\n");
    }else{
        printf("ER\n");
    }

    if(memory_stats_global == NULL){
        printf("15 OK\n");
    }else{
        printf("ER\n");
    }

    printf("\n");
    printf("\n");
    fflush(stdout);
}
