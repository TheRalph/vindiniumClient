#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations

namespace VDC
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Definition of a 2d position
 */
class CPosition
{
    public:
        /**
        * @brief Constructor of a 2d position
        * 
        * Default values are 0,0
        */
        CPosition() : m_x(0), m_y(0) {}

        /**
        * @brief Constructor of a 2d position
        * @param inX the x position
        * @param inY the y position
        */
        CPosition(const int inX, const int inY) : m_x(inX), m_y(inY) {}

        /**
        * @brief Return the X position
        * @return the X position
        */
        inline int getX() const { return m_x; }

        /**
        * @brief Return the X position
        * @return the X position
        */
        inline int getY() const { return m_y; }

        /**
        * @brief Set the X position
        * @param inX the X position to use
        */
        inline void setX(const int inX) { m_x = inX; }

        /**
        * @brief Set the Y position
        * @param inY the Y position to use
        */
        inline void setY(const int inY) { m_y = inY; }

        /**
        * @brief Set the X and Y positions
        * @param inX the X position to use
        * @param inY the Y position to use
        */
        inline void setXY(const int inX, const int inY) { m_x = inX; m_y = inY; }

    private:
        int m_x; ///< the x position
        int m_y; ///< the y position
}; // class CPosition

} // namespace VDC