// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2024.10
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2024-2024 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory_wrapper.h"

#define ENUMERATION_TEXT_COUNT 5

typedef struct {
    int index;
    void *pointer;
} enumeration_text_t;

static enumeration_text_t *create(void) {
    enumeration_text_t *enumeration_text = {0};
    if(!memory_req(&enumeration_text, ENUMERATION_TEXT_COUNT, sizeof(enumeration_text_t))){
        return NULL;
    }
    return enumeration_text;
}

static void delete(enumeration_text_t *enumeration_text) {
    memory_ret(&enumeration_text);
}

static void fill(enumeration_text_t *enumeration_text){
    for(int i = 0; i < ENUMERATION_TEXT_COUNT; i++){
        enumeration_text[i].index = i;
        enumeration_text[i].pointer  = &enumeration_text[i];
    }
}

static void print(enumeration_text_t *enumeration_text){
    for(int i = 0; i < ENUMERATION_TEXT_COUNT; i++){
        printf("%d = %p\n", enumeration_text[i].index, enumeration_text[i].pointer);
    }
}

int using_wrong(void) {
    printf("using wrong exsemple\n");

    enumeration_text_t *enumeration_text = create();

    fill(enumeration_text);
    print(enumeration_text);

    delete(enumeration_text);

    if(enumeration_text != NULL){
        printf("!!! pointer after delete NOT NULL\n");
    } else {
        printf("^_^ pointer after delete NULL\n");
    }

    printf("\n");

    return 0;
}
