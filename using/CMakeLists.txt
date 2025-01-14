#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2024.10
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.htmlater.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2024-2025 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(memory_using)

set(CMAKE_C_STANDARD 11)

file(GLOB FILES *.c *.h)

add_executable(${PROJECT_NAME} ${FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC yaya_memory)
