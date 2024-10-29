#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2022.12
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(
    yaya_memory
    VERSION      2.0.0.0
    LANGUAGES    C
    HOMEPAGE_URL "https://github.com/seityaya/c_memory"
    DESCRIPTION  "A wrapper over the standard alloc functions"
)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST
    yaya_memory.c
)

set(INC_LIST
    yaya_memory.h
    yaya_memory_wrapper.h
)

add_library(${PROJECT_NAME} ${SRC_LIST} ${INC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
