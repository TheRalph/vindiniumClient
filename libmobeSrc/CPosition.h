//=============================================================================
// Copyright (c) 2014, Raphaël La Greca <raphael.la.greca+vindinium@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
//  * Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=============================================================================

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
        * @brief return true if the current position is equal to the given one
        * @return true if the current position is equal to the given one
        */
        inline bool operator==(const CPosition& inPosition) { return (inPosition.getX() == getX()) && (inPosition.getY() == getY()); }

        /**
        * @brief return true if the current position is different to the given one
        * @return true if the current position is different to the given one
        */
        inline bool operator!=(const CPosition& inPosition) { return !operator==(inPosition); }

        /**
        * @brief Return the 4 connected positions of the current one
        * @return the 4 connected positions of the current one
        * 
        * 4-connected cell order: 0,+1 // +1,0 // 0,-1 // -1,0
        */
        inline std::vector<CPosition> get4Connected() const
        {
            std::vector<CPosition> vector4Connected;
            vector4Connected.reserve(4);
            vector4Connected.emplace_back(m_x  , m_y+1);
            vector4Connected.emplace_back(m_x+1, m_y);
            vector4Connected.emplace_back(m_x  , m_y-1);
            vector4Connected.emplace_back(m_x-1, m_y);
            return vector4Connected;
        } // get4Connected

        /**
        * @brief Return the 8 connected positions of the current one
        * @return the 8 connected positions of the current one
        * 
        * 8-connected cell order: 0,+1 // +1,+1 // +1,0 // +1,-1 // 0,-1 // -1,-1 // -1,0 // -1,+1
        */
        inline std::vector<CPosition> get8Connected() const
        {
            std::vector<CPosition> vector8Connected;
            vector8Connected.reserve(8);
            vector8Connected.emplace_back(m_x  , m_y+1);
            vector8Connected.emplace_back(m_x+1, m_y+1);
            vector8Connected.emplace_back(m_x+1, m_y);
            vector8Connected.emplace_back(m_x+1, m_y-1);
            vector8Connected.emplace_back(m_x  , m_y-1);
            vector8Connected.emplace_back(m_x-1, m_y-1);
            vector8Connected.emplace_back(m_x-1, m_y);
            vector8Connected.emplace_back(m_x-1, m_y+1);
            return vector8Connected;
        } // get4Connected

    private:
        int m_x; ///< the x position
        int m_y; ///< the y position
}; // class CPosition

} // namespace MOBE