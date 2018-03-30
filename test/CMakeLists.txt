#
# Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
include_directories(${CMAKE_CURRENT_LIST_DIR})

function(test test)
  string(REPLACE "/" "_" tmp ${test})
  add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)
  add_test(test.${tmp} test.${tmp})
endfunction()

test(te)
