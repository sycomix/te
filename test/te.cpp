//
// Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <sstream>
#include <type_traits>
#include <vector>

#include "common/test.hpp"
#include "te.hpp"

test should_set_get_mappings = [] {
  struct B {};
  te::detail::mappings<class A, 0>::set<B>();
  static_assert(
      std::is_same<B, decltype(get(te::detail::mappings<class A, 0>{}))>{});

  struct C {};
  te::detail::mappings<class A, 1>::set<C>();
  static_assert(
      std::is_same<C, decltype(get(te::detail::mappings<class A, 1>{}))>{});
};

test should_return_mappings_size = [] {
  static_assert(
      0 ==
      te::detail::mappings_size<class Size, std::integral_constant<int, 0>>());
  te::detail::mappings<class Size, 1>::set<std::integral_constant<int, 1>>();
  te::detail::mappings<class Size, 2>::set<std::integral_constant<int, 2>>();
  te::detail::mappings<class Size, 3>::set<std::integral_constant<int, 3>>();
  static_assert(
      3 ==
      te::detail::mappings_size<class Size, std::integral_constant<int, 1>>());
};

struct Empty {};

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

struct Triangle {
  void draw(std::ostream &out) const { out << "Triangle"; }
};

test should_erase_the_call = [] {
  te::poly<Drawable> drawable{Square{}};

  {
    std::stringstream str{};
    drawable.draw(str);
    expect("Square" == str.str());
  }

  {
    std::stringstream str{};
    drawable = Circle{};
    drawable.draw(str);
    expect("Circle" == str.str());
  }
};

test should_reassign = [] {
  te::poly<Drawable> drawable{Circle{}};
  drawable = Square{};

  {
    std::stringstream str{};
    drawable.draw(str);
    expect("Square" == str.str());
  }
};

test should_support_containers = [] {
  std::vector<te::poly<Drawable>> drawables{};

  drawables.push_back(Square{});
  drawables.push_back(Circle{});
  drawables.push_back(Triangle{});

  std::stringstream str{};
  for (const auto &drawable : drawables) {
    drawable.draw(str);
  }
  expect("SquareCircleTriangle" == str.str());
};

struct Addable {
  auto add(int i) { return te::call<int>(add_impl, *this, i); }
  auto add(int a, int b) { return te::call<int>(add_impl, *this, a, b); }

 private:
  static constexpr auto add_impl = [](auto &self, auto... args) {
    return self.add(args...);
  };
};

class Calc {
 public:
  constexpr auto add(int i) { return i; }
  constexpr auto add(int a, int b) { return a + b; }
};

test should_support_overloads = [] {
  te::poly<Addable> addable{Calc{}};
  expect(3 == addable.add(3));
  expect(3 == addable.add(1, 2));
};

namespace v1 {
struct Drawable {
  void draw(std::ostream &out) const {
    te::call([](auto const &self, auto &&... args) { self.draw(args...); },
             *this, out, "v1");
  }
};
}  // namespace v1

namespace v2 {
struct Drawable : v1::Drawable {
  Drawable() { te::extends<v1::Drawable>(*this); }
};
}  // namespace v2

namespace v3 {
struct Drawable : v2::Drawable {
  Drawable() { te::extends<v2::Drawable>(*this); }

  void draw(std::ostream &out) const {
    te::call([](auto const &self, auto &&... args) { self.draw(args...); },
             *this, out, "v3");
  }
};
}  // namespace v3

test should_support_overrides = [] {
  struct Square {
    void draw(std::ostream &out, const std::string &v) const {
      out << v << "::Square ";
    }
  };

  struct Circle {
    void draw(std::ostream &out, const std::string &v) const {
      out << v << "::Circle ";
    }
  };

  const auto draw = [](auto const &drawable, auto &str) { drawable.draw(str); };

  {
    std::stringstream str{};
    draw(te::poly<v1::Drawable>{Circle{}}, str);
    draw(te::poly<v1::Drawable>{Square{}}, str);
    expect("v1::Circle v1::Square " == str.str());
  }

  {
    std::stringstream str{};
    draw(te::poly<v2::Drawable>{Circle{}}, str);
    draw(te::poly<v2::Drawable>{Square{}}, str);
    expect("v1::Circle v1::Square " == str.str());
  }

  {
    std::stringstream str{};
    draw(te::poly<v3::Drawable>{Circle{}}, str);
    draw(te::poly<v3::Drawable>{Square{}}, str);
    expect("v3::Circle v3::Square " == str.str());
  }
};

test should_support_custom_storage = [] {
  te::poly<Addable> addable_def{Calc{}};
  expect(42 == addable_def.add(40, 2));

  te::poly<Addable, te::local_storage<16>> addable_local{Calc{}};
  expect(42 == addable_local.add(40, 2));

  te::poly<Addable, te::local_storage<16>, te::static_vtable<Addable>>
      addable_local_static{Calc{}};
  expect(42 == addable_local_static.add(40, 2));
};
