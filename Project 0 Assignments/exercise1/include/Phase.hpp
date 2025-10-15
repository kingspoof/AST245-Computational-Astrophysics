#ifndef PHASE_HPP
#define PHASE_HPP

#include "Vector2D.hpp"

struct Phase
{
    Vector2D position;
    Vector2D velocity;

    Phase() {}
    Phase(Vector2D position, Vector2D velocity)
    {
        this->position = position;
        this->velocity = velocity;
    }

    Phase operator+(const Phase &other) const
    {
        return Phase(this->position + other.position, this->velocity + other.velocity);
    }
    Phase operator-(const Phase &other) const
    {
        return Phase(this->position - other.position, this->velocity - other.velocity);
    }
    Phase operator*(double scalar) const
    {
        return Phase(this->position * scalar, this->velocity * scalar);
    }
    Phase operator/(double scalar) const
    {
        return Phase(this->position / scalar, this->velocity / scalar);
    }

    Phase &operator+=(const Phase &other)
    {
        this->position += other.position;
        this->velocity += other.velocity;
        return *this;
    }
    Phase &operator-=(const Phase &other)
    {
        this->position -= other.position;
        this->velocity -= other.velocity;
        return *this;
    }
    Phase &operator*=(double scalar)
    {
        this->position *= scalar;
        this->velocity *= scalar;
        return *this;
    }
    Phase &operator/=(double scalar)
    {
        this->position /= scalar;
        this->velocity /= scalar;
        return *this;
    }
};

#endif