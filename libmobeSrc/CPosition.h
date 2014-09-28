#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations

namespace MOBE
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

        /**
        * @brief Return the 4 connected positions of the current one
        * @return the 4 connected positions of the current one
        */
        inline std::vector<CPosition> get4Connected() const
        {
            std::vector<CPosition> vector4Connected;
            vector4Connected.reserve(4);
            vector4Connected.emplace_back(m_x+1, m_y);
            vector4Connected.emplace_back(m_x-1, m_y);
            vector4Connected.emplace_back(m_x  , m_y+1);
            vector4Connected.emplace_back(m_x  , m_y-1);
            return vector4Connected;
        } // get4Connected

    private:
        int m_x; ///< the x position
        int m_y; ///< the y position
}; // class CPosition

} // namespace MOBE