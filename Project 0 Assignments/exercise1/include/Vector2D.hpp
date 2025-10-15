#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP


struct Vector2D
{
    double x, y;

    Vector2D(): x(0), y(0) {}
    Vector2D(double x_pos, double y_pos): x(x_pos), y(y_pos) {}


    // Non-mutating ones (return a new object)
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    Vector2D operator/(double scalar) const;

    // Mutating operators: modify this object and return reference for chaining
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(double scalar);
    Vector2D& operator/=(double scalar);

    //Other stuff, that might be usefull
    /**
     * Returns the magnitude (length) of the vector
     */
    double magnitude() const;

    /**
     * returns the distance between the two vectors
     * 
     * @param other other vector
     */
    double distance_to(const Vector2D& other) const;

    /**
     * returns a new vector which holds the normalized direction of this one
     * leaves this vector unchanged
     */
    Vector2D normalized() const;

};



#endif