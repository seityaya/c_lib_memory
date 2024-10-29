// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2022.12
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

void test_param_stats_global();
void test_param_stats_local();
void test_param();
void test_dump();
void test_look();
void test_swap();
void test_shuf();
void test_sort();
void test_search();

int main(int argc, char* argv[]) {
    test_param();
    test_param_stats_local();
    test_param_stats_global();

    test_dump();
    test_look();
    test_swap();
    test_shuf();
    test_sort();
    test_search();

    return 0;
}
