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
        * @brief Print in the standard output the current hero data
        */
        void print();

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
}; // class CHero

} // namespace MOBE