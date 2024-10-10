#pragma once
#include <ostream>

struct Complex {
  double re;
  double im;

  Complex();
  Complex(double re_);
  Complex(double re_, double im_);

  Complex operator-();
  Complex operator+();
  Complex &operator+=(const Complex &other);
  Complex &operator-=(const Complex &other);
  Complex &operator*=(const Complex &other);
  Complex &operator/=(const Complex &other);

  Complex conj() const;
  double norm() const;
  double norm_sq() const;
};

Complex operator+(const Complex &lhs, const Complex &rhs);
Complex operator-(const Complex &lhs, const Complex &rhs);
Complex operator*(const Complex &lhs, const Complex &rhs);
Complex operator/(const Complex &lhs, const Complex &rhs);

std::ostream &operator<<(std::ostream &os, const Complex &z);

static const Complex I(0, 1);
