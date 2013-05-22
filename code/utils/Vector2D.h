/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

#pragma once
#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

/**
 * Handy Vector2D object
 */


// Includes
#include <vector>


/**
 * The Vector2D class allows you to create vectors for use in 2D mathematics.
 * It has the option of being constructed as a direction vector.
 * The following operators are overloaded and can be used with other Vector2D objects:
 * + += - -= * *= / /= == != [] <<
 */
class Vector2D
{

public:

    Vector2D();
    Vector2D(float x_, float y_, bool directional = false);
    float Get_Mag();
    std::vector<float> To_STD_Vector();
    float Dot(Vector2D& other);

    /**
     * 1st component of the vector.
     */
    float xCom;

    /**
     * 2nd component of the vector.
     */
    float yCom;

    // operator overloads
    Vector2D& operator+=(Vector2D const& right);
    Vector2D& operator-=(Vector2D const& right);
    Vector2D& operator*=(Vector2D const& right);
    Vector2D& operator*=(float const& right);
    Vector2D& operator/=(Vector2D const& right);
    Vector2D& operator/=(float const& right);
    float& operator[](int index);

};

Vector2D operator+(Vector2D left, Vector2D const& right);
Vector2D operator-(Vector2D left, Vector2D const& right);
Vector2D operator*(Vector2D left, Vector2D const& right);
Vector2D operator*(Vector2D left, float const& right);
Vector2D operator/(Vector2D left, Vector2D const& right);
Vector2D operator/(Vector2D left, float const& right);
bool operator==(const Vector2D& left, const Vector2D& right);
bool operator!=(const Vector2D& left, const Vector2D& right);
std::ostream& operator<<(std::ostream& out, const Vector2D& vec);


#endif
