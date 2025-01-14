// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2025 Seityagiya Terlekchi. All rights reserved.

#include "stdio.h"

extern void test_param_stats_global(void);
extern void test_param_stats_local(void);
extern void test_param(void);
extern void test_dump(void);
extern void test_look(void);
extern void test_swap(void);
extern void test_shuf(void);
extern void test_sort(void);
extern void test_search(void);

int main(void) {
    printf("%s\n", "Beg Testing...");

    test_param();
    test_param_stats_local();
    test_param_stats_global();

    test_dump();
    test_look();
    test_swap();
    test_shuf();
    test_sort();
    test_search();

    printf("%s\n", "End Testing...");
    return 0;
}
