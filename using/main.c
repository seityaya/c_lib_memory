// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

#include "yaya_memory_wrapper.h"

#define ENUMERATION_TEXT_COUNT 5

typedef struct {
    int index;
    void *pointer;
} enumeration_text_t;

int main(void) {
    enumeration_text_t *enumeration_text = {0};

    if(!memory_req(&enumeration_text, ENUMERATION_TEXT_COUNT, sizeof(enumeration_text_t))){
        return -1;
    }

    for(int i = 0; i < ENUMERATION_TEXT_COUNT; i++){
        enumeration_text[i].index = i;
        enumeration_text[i].pointer  = &enumeration_text[i];
    }

    for(int i = 0; i < ENUMERATION_TEXT_COUNT; i++){
        printf("%d = %p\n", enumeration_text[i].index, enumeration_text[i].pointer);
    }

    if(!memory_ret(&enumeration_text)){
        return -1;
    }

    return 0;
}
