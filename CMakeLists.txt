#
# Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)
project(te CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -fno-exceptions -pedantic -pedantic-errors -Wall -Wextra -Werror")
include_directories(include)

enable_testing()
add_subdirectory(example)
add_subdirectory(test)
