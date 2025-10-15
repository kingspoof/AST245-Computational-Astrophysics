#include "Vector2D.hpp"
#include <cmath>

// this is a Vector2D* which is a pointer 
// *this is a Vector2D& which is a reference

Vector2D Vector2D::operator+(const Vector2D& other) const
{
    return Vector2D(this->x + other.x, this->y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const 
{
    return Vector2D(this->x - other.x, this->y - other.y);
}
Vector2D Vector2D::operator*(double scalar) const 
{
    return Vector2D(this->x * scalar, this->y * scalar);
}
Vector2D Vector2D::operator/(double scalar) const {
    return Vector2D(this->x / scalar, this->y / scalar);
}

// Mutating operators: modify this object and return reference for chaining
Vector2D& Vector2D::operator+=(const Vector2D& other) 
{
    this->x += other.x;
    this->y += other.y;
    return *this; //&this because we only want to pass back the reference to this ????
}
Vector2D& Vector2D::operator-=(const Vector2D& other) 
{
    this->x -= other.x;
    this->y -= other.y;
    return *this; //&this because we only want to pass back the reference to this ????
}
Vector2D& Vector2D::operator*=(double scalar) 
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}
Vector2D& Vector2D::operator/=(double scalar) 
{
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}


double Vector2D::magnitude() const
{
    return sqrt(this->x*this->x + this->y*this->y);
}

double Vector2D::distance_to(const Vector2D& other) const
{
    double x_distance = this->x - other.x;
    double y_distance = this->y - other.y;
    return sqrt(x_distance*x_distance + y_distance*y_distance);
}

Vector2D Vector2D::normalized() const
{
    double mag = this->magnitude();
    if (mag == 0.0) return Vector2D(0, 0);
    return *this / mag;
}