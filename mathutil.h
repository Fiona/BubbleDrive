/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Math util header
 Has vector2d stuff.
 ****************************/

#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_ 


/**
 * The Vector2D class allows you to create vectors for use in 2D mathematics.
 * It has the option of being constructed as a direction vector.
 * The following operators are overloaded and can be used with other Vector2D objects:
 * + += - -= * *= / /= == != [] <<
 */
class Vector2D
{

public:

    /**
     * Blank constructor. Creates a vector at 0.0, 0.0
     */
    Vector2D();

    /**
     * Constructor.
     * @param x The 1st component of the vector, or the direction if directional
     * @param y The 2st component of the vector, or the magnitude if directional
     * @param directonal If the components should be interpreted as a 
     *   direction/magnitude pair instead of a positional vector.
     */
    Vector2D(float x_, float y_, bool directional = false);

    /**
     * 1st component of the vector.
     */
    float x;

    /**
     * 2nd component of the vector.
     */
    float y;

    /**
     * Return mag
     */
    float get_mag();

    /**
     * Get vector back as a boost::tuple
     */
    boost::tuple<float, float> to_tuple();

    /**
     * Calculate dot product
     * @param other The Vector2D to calculate the dot product from
     */
    float dot(Vector2D& other);

    /**
     * += operator
     */
    Vector2D& operator+=(Vector2D const& right);

    /**
     * -= operator
     */
    Vector2D& operator-=(Vector2D const& right);

    /**
     * *= operator
     */
    Vector2D& operator*=(Vector2D const& right);
    Vector2D& operator*=(float const& right);

    /**
     * /= operator
     */
    Vector2D& operator/=(Vector2D const& right);
    Vector2D& operator/=(float const& right);

    /**
     * [] operator
     */
    float& operator[](int index);

};


/**
 * + operator
 */
Vector2D operator+(Vector2D left, Vector2D const& right);

/**
 * - operator
 */
Vector2D operator-(Vector2D left, Vector2D const& right);

/**
 * * operator
 */
Vector2D operator*(Vector2D left, Vector2D const& right);
Vector2D operator*(Vector2D left, float const& right);

/**
 * / operator
 */
Vector2D operator/(Vector2D left, Vector2D const& right);
Vector2D operator/(Vector2D left, float const& right);

/**
 * == operator
 */
bool operator==(const Vector2D& left, const Vector2D& right);

/**
 * != operator
 */
bool operator!=(const Vector2D& left, const Vector2D& right);

/**
 * << operator
 */
std::ostream& operator<<(std::ostream& out, const Vector2D& vec);


#endif
