#=============================================================================
# Copyright (c) 2014, Raphaël La Greca <raphael.la.greca+vindinium@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  * Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CPosition.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations
namespace Json
{
    class Value;
} // namespace Json

namespace MOBE
{

/**
 * @brief Vindinium managed directions
 */
enum E_BEHAVIOR_ACTIONS
{
    E_ACTION_STAY = 0,   ///< Stay here
    E_ACTION_MOVE_NORTH, ///< Move to the North
    E_ACTION_MOVE_SOUTH, ///< Move to the South
    E_ACTION_MOVE_EAST,  ///< Move to the East
    E_ACTION_MOVE_WEST,  ///< Move to the West
    NB_BEHAVIOR_ACTIONS  ///< number of managed actions
}; // enum E_BEHAVIOR_ACTIONS

static const std::string G_BEHAVIOR_ACTIONS_DICTIONARY[NB_BEHAVIOR_ACTIONS]={
    "Stay",
    "North",
    "South",
    "East",
    "West"
}; ///< Dictionary of behaviors actions

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Client for web game 'Vindinium' (http://vindinium.org/)
 */
class CHero
{
    public:
        /**
        * @brief Constructor of Vindinium game
        * @param inJsonValues the json data describing the game
        */
        CHero(const Json::Value& inJsonValues);

        /**
        * @brief Destructor of Vindinium game
        */
        virtual ~CHero();

        /**
        * @brief Update the current hero with new json data
        * @param inJsonValues the json data describing the hero
        */
        void update(const Json::Value& inJsonValues);

        /**
        * @brief Return the Id of the current Hero
        * @return the Id of the current Hero
        */
        inline int getId() const { return m_id; }

        /**
        * @brief Return the name of the current Hero
        * @return the name of the current Hero
        */
        inline std::string getName() const { return m_name; }

        /**
        * @brief Return the user id of the current Hero
        * @return the user id of the current Hero
        */
        inline std::string getUserId() const { return m_userId; }

        /**
        * @brief Return the elo of the current Hero
        * @return the elo of the current Hero
        */
        inline int getElo() const { return m_elo; }

        /**
        * @brief Return the position of the current Hero
        * @return the position of the current Hero
        */
        inline CPosition getPosition() const { return m_position; }

        /**
        * @brief Return the life value of the current Hero
        * @return the life value of the current Hero
        */
        inline int getLife() const { return m_life; }

        /**
        * @brief Return the gold value of the current Hero
        * @return the gold value of the current Hero
        */
        inline int getGold() const { return m_gold; }

        /**
        * @brief Return the mine count value of the current Hero
        * @return the mine count value of the current Hero
        */
        inline int getMineCount() const { return m_mineCount; }

        /**
        * @brief Return the spawn position of the current Hero
        * @return the spawn position of the current Hero
        */
        inline CPosition getSpawnPosition() const { return m_spawnPosition; }

        /**
        * @brief Return true if the current hero has crashed, false otherwise
        * @return true if the current hero has crashed, false otherwise
        */
        inline bool isCrashed() const { return m_isCrashed; }

        /**
        * @brief Return the vector of cellIds of gold mines owned by the current hero
        * @return the vector of cellIds of gold mines owned by the current hero
        */
        inline std::vector<int>& getOwnedGoldMineCellIds() { return m_ownedGoldMineCellIds; }

        /**
        * @brief Return the const vector of cellIds of gold mines owned by the current hero
        * @return the const vector of cellIds of gold mines owned by the current hero
        */
        inline const std::vector<int>& getOwnedGoldMineCellIds() const { return m_ownedGoldMineCellIds; }

        /**
        * @brief Return the maximum life of an hero
        */
        inline const int getMaxLife() const { return m_maxLife; }

        /**
        * @brief Set the maximum life of the hero
        * @param inMaxLife the max life of a hero
        */
        inline void setMaxLife(const int inMaxLife) { m_maxLife = inMaxLife; }

        /**
        * @brief Return the last action of the hero
        */
        inline const E_BEHAVIOR_ACTIONS getLastAction() const { return m_lastAction; }

        /**
        * @brief Print in the standard output the current hero data
        */
        void print() const;

    private:
        E_BEHAVIOR_ACTIONS getLastActionId(const std::string &inActionStr) const;

    private:
        int m_id;
        std::string m_name;
        std::string m_userId;
        int m_elo;
        CPosition m_position;
        int m_life;
        int m_gold;
        int m_mineCount;
        CPosition m_spawnPosition;
        bool m_isCrashed;
        std::vector<int> m_ownedGoldMineCellIds;
        int m_maxLife;
        E_BEHAVIOR_ACTIONS m_lastAction;
}; // class CHero

} // namespace MOBE