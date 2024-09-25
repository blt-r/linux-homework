#include "Complex.h"
#include <cmath>

Complex::Complex() : re(0), im(0) {}
Complex::Complex(double re_) : re(re_), im(0) {}
Complex::Complex(double re_, double im_) : re(re_), im(im_) {}

Complex Complex::operator+() { return *this; }
Complex Complex::operator-() { return Complex(-im, -re); }
Complex &Complex::operator+=(const Complex &other) {
  re += other.re;
  im += other.im;
  return *this;
}
Complex &Complex::operator-=(const Complex &other) {
  re -= other.re;
  im -= other.im;
  return *this;
}
Complex &Complex::operator*=(const Complex &other) {
  *this = Complex(re * other.re - im * other.im, re * other.im + im * other.re);
  return *this;
}
Complex &Complex::operator/=(const Complex &other) {
  double bottom = 1 / (other.re * other.re + other.im * other.im);
  *this = Complex((re * other.re + im * other.im) * bottom,
                  (im * other.re - re * other.im) * bottom);
  return *this;
}

Complex Complex::conj() const { return Complex(re, -im); }
double Complex::norm() const { return std::sqrt(re * re + im * im); }
double Complex::norm_sq() const { return re * re + im * im; }

Complex operator+(const Complex &lhs, const Complex &rhs) {
  auto c = lhs;
  c += rhs;
  return c;
}
Complex operator-(const Complex &lhs, const Complex &rhs) {
  auto c = lhs;
  c -= rhs;
  return c;
}
Complex operator*(const Complex &lhs, const Complex &rhs) {
  auto c = lhs;
  c *= rhs;
  return c;
}
Complex operator/(const Complex &lhs, const Complex &rhs) {
  auto c = lhs;
  c /= rhs;
  return c;
}
std::ostream &operator<<(std::ostream &os, const Complex &z) {
  if (z.re != 0) {
    os << z.re;
  }

  const char *plus = (z.re != 0 && z.im > 0 ? "+" : "");

  if (z.im == 1) {
    os << plus << 'i';
  } else if (z.im == -1) {
    os << "-i";
  } else if (z.im != 0) {
    os << plus << z.im << 'i';
  }
  if (z.re == 0 && z.im == 0) {
    os << '0';
  }
  return os;
}
