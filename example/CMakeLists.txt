#
# Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
function(example example)
  string(REPLACE "/" "_" tmp ${example})
  add_executable(example.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${example}.cpp)
  add_test(example.${tmp} example.${tmp})
endfunction()

example(container)
example(drawable)
example(function)
example(overload)
example(override)
example(storage)
