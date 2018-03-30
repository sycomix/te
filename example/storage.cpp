//
// Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>

#include "te.hpp"

struct Drawable {
  void draw(std::ostream &out) const {
    te::call([](auto const &self, auto &out) { self.draw(out); }, *this, out);
  }
};

struct Square {
  void draw(std::ostream &out) const { out << "Square"; }
};

struct Circle {
  void draw(std::ostream &out) const { out << "Circle"; }
};

int main() {
  {
    te::poly<Drawable>{Square{}}.draw(std::cout);
    te::poly<Drawable>{Circle{}}.draw(std::cout);
  }

  {
    using drawable_t =
        te::poly<Drawable, te::local_storage<16>, te::static_vtable<Drawable>>;
    drawable_t{Circle{}}.draw(std::cout);  // prints Circle
    drawable_t{Square{}}.draw(std::cout);  // prints Square
  }
}
