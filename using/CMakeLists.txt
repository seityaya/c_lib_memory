#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2022.12
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2022-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(memory_using)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST
    main.c

    using_base.c
    using_correct.c
    using_wrong.c
)

set(INC_LIST
)

add_executable(
    ${PROJECT_NAME}
    ${SRC_LIST}
    ${INC_LIST}
)

target_link_libraries(${PROJECT_NAME} PUBLIC yaya_memory)
