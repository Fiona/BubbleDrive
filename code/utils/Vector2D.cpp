/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
 * Handy Vector2D object
 */


// Includes
#include <iostream>
#include <math.h>
#include "Vector2D.h"



/**
 * Blank constructor. Creates a vector at 0.0, 0.0
 */
Vector2D::Vector2D()
{
    xCom = 0.0f;
    yCom = 0.0f;
}


/**
 * Constructor.
 * @param x The 1st component of the vector, or the direction if directional
 * @param y The 2st component of the vector, or the magnitude if directional
 * @param directonal If the components should be interpreted as a 
 *   direction/magnitude pair instead of a positional vector.
 */
Vector2D::Vector2D(float x_, float y_, bool directional)
{
	
    if(directional)
    {
        xCom = std::cos(x_) * y_;
        yCom = std::sin(x_) * y_;
    }
    else
    {
        xCom = x_;
        yCom = y_;
    }

}


/**
 * Return mag
 */
float Vector2D::Get_Mag()
{

    return std::sqrt(std::pow(xCom, 2) + pow(yCom, 2));

}



/**
 * Get vector back as an std::vector
 */
std::vector<float> Vector2D::To_STD_Vector()
{

	std::vector<float> ret;
	ret.push_back(xCom);
	ret.push_back(yCom);
    return ret;

}


/**
 * Calculate dot product
 * @param other The Vector2D to calculate the dot product from
 */
float Vector2D::Dot(Vector2D& other)
{

    return xCom*other.xCom + yCom*other.yCom;

}


/**
 * += Operator
 */
Vector2D& Vector2D::operator+=(Vector2D const& right)
{

    xCom += right.xCom;
    yCom += right.yCom;
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

    xCom -= right.xCom;
    yCom -= right.yCom;
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

    xCom *= right.xCom;
    yCom *= right.yCom;
    return *this;

}

Vector2D& Vector2D::operator*=(float const& right)
{

    xCom *= right;
    yCom *= right;
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

    xCom /= right.xCom;
    yCom /= right.yCom;
    return *this;

}

Vector2D& Vector2D::operator/=(float const& right)
{

    xCom /= right;
    yCom /= right;
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

    return ((left.xCom == right.xCom) && (left.yCom == right.yCom));

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
        return xCom;
    return yCom;

}


/**
 * << operator
 */
std::ostream& operator<<(std::ostream& out, const Vector2D& vec)
{

    out << "(" << vec.xCom << ", " << vec.yCom << ")";
    return out;

}
