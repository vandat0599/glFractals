#pragma once

#include <sstream>
#include <string>

#define stringify1(s) stringify0(s)
#define stringify0(s) #s

const auto ROOT_PATH_STR = std::string(stringify1(ROOT_PATH));

template <typename T>
struct Point2D {
    T x, y;

    Point2D() : x(0), y(0) {}
    Point2D(T aX, T aY) : x(aX), y(aY) {}
};

template <typename T>
Point2D<T> operator+(const Point2D<T>& l, const Point2D<T>& r)
{
    return {l.x + r.x, l.y + r.y};
}

template <typename T>
Point2D<T> operator-(const Point2D<T>& l, const Point2D<T>& r)
{
    return {l.x - r.x, l.y - r.y};
}

template <typename T>
Point2D<T> operator*(T f, const Point2D<T>& p)
{
    return {f * p.x, f * p.y};
}

template <typename T>
Point2D<T> operator*(const Point2D<T>& p, T f)
{
    return {f * p.x, f * p.y};
}

template <typename T>
Point2D<T> operator/(T f, const Point2D<T>& p)
{
    return {f / p.x, f / p.y};
}

template <typename T>
Point2D<T> operator/(const Point2D<T>& p, T f)
{
    return {f / p.x, f / p.y};
}

template <typename T>
std::string str(const Point2D<T> p)
{
    std::stringstream ss;
    ss << std::scientific;
    ss << p.x << ", " << p.y;
    return ss.str();
};