/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Math util source file
 ****************************/

#include "main.h"


/**
 * Constructor
 */
Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}


/**
 * Constructor
 */
Vector2D::Vector2D(float x_, float y_, bool directional)
{
    if(directional)
    {
        x = cos(x_) * y_;
        y = sin(x_) * y_;
    }
    else
    {
        x = x_;
        y = y_;
    }
}


/**
 * Return mag
 */
float Vector2D::get_mag()
{
    return sqrt(pow(x, 2) + pow(y, 2));
}


/**
 * Get vector back as a boost::tuple
 */
boost::tuple<float, float> Vector2D::to_tuple()
{
    return boost::tuple<float, float>(x, y);
}


/**
 * Calculate dot product
 * @param other The Vector2D to calculate the dot product from
 */
float Vector2D::dot(Vector2D& other)
{
    return x*other.x + y*other.y;
}


/**
 * += Operator
 */
Vector2D& Vector2D::operator+=(Vector2D const& right)
{
    x += right.x;
    y += right.y;
    return *this;
}

/**
 * + Operator
 */
Vector2D operator+(Vector2D left, Vector2D const& right)
{
  left += right;
  return left;
}


/**
 * -= Operator
 */
Vector2D& Vector2D::operator-=(Vector2D const& right)
{
    x -= right.x;
    y -= right.y;
    return *this;
}

/**
 * - Operator
 */
Vector2D operator-(Vector2D left, Vector2D const& right)
{
  left -= right;
  return left;
}


/**
 * *= Operator
 */
Vector2D& Vector2D::operator*=(Vector2D const& right)
{
    x *= right.x;
    y *= right.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float const& right)
{
    x *= right;
    y *= right;
    return *this;
}

/**
 * * Operator
 */
Vector2D operator*(Vector2D left, Vector2D const& right)
{
  left *= right;
  return left;
}

Vector2D operator*(Vector2D left, float const& right)
{
  left *= right;
  return left;
}


/**
 * /= Operator
 */
Vector2D& Vector2D::operator/=(Vector2D const& right)
{
    x /= right.x;
    y /= right.y;
    return *this;
}

Vector2D& Vector2D::operator/=(float const& right)
{
    x /= right;
    y /= right;
    return *this;
}

/**
 * / Operator
 */
Vector2D operator/(Vector2D left, Vector2D const& right)
{
  left /= right;
  return left;
}
Vector2D operator/(Vector2D left, float const& right)
{
  left /= right;
  return left;
}


/**
 * == operator
 */
bool operator==(const Vector2D& left, const Vector2D& right)
{
    return ((left.x == right.x) && (left.y == right.y));
}

/**
 * != operator
 */
bool operator!=(const Vector2D& left, const Vector2D& right)
{
    return !operator==(left, right);
}


/**
 * [] operator
 */
float& Vector2D::operator[](int index)
{
    if(index == 0)
        return x;
    return y;
}


/**
 * << operator
 */
std::ostream& operator<<(std::ostream& out, const Vector2D& vec)
{
    out << "(" << vec.x << ", " << vec.y << ")";
    return out;
}
