#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <cmath>

template<class T>
class Point2D
{
public:
    explicit Point2D(const T &x = static_cast<T>(0), const T &y = static_cast<T>(0)):
        x_{x}, y_{y}
    {}
    inline void print() const noexcept
    {
        printf("(%15.8f, %15.8f)\n", x_,  y_);
    }
    inline void setX(const T &x) { x_ = x; }
    inline void setY(const T &y) { y_ = y; }
    inline auto x() const noexcept { return x_; }
    inline auto y() const noexcept { return y_; }
private:
    float x_, y_;
};

template<class T>
class Line
{
public:
    explicit Line(const T &x1 = static_cast<T>(0), const T &y1 = static_cast<T>(0),
                  const T &x2 = static_cast<T>(0), const T &y2 = static_cast<T>(0));
    inline void print() const noexcept
    {
        printf("\nLine:\n");
        printf("1:");
        p1_.print();
        printf("2:");
        p2_.print();
        printf("Angle: %15.8f Length: %15.8f\n", this->angle(), this->lenth());
    }
    inline auto angle() const noexcept { return 180 / M_PI * atan2(p2_.y() - p1_.y(),
                                                        p2_.x() - p1_.x()); }
    inline auto lenth() const noexcept { return sqrt(pow(p1_.x() - p2_.x(), 2) +
                                                       pow(p1_.y() - p2_.y(), 2)); }
private:
    Point2D<T> p1_, p2_;
};

template<class T>
Line<T>::Line(const T &x1, const T &y1, const T &x2, const T &y2):
        p1_(x1,y1), p2_(x2, y2)
    {}

#endif // TEST_HPP
