#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2022.12
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(memory_test)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST
    main.c
    test_memory.c
    test_memory_stats_global.c
    test_memory_stats_local.c
)

set(INC_LIST
)

add_executable(
    ${PROJECT_NAME}
    ${SRC_LIST}
    ${INC_LIST}
)

target_link_libraries(${PROJECT_NAME} PUBLIC yaya_memory)
