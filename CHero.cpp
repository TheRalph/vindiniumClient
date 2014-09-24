////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CHero.h"

namespace VDC
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CHero::CHero(const Json::Value& inJsonValues)
: m_id(-1),
  m_elo(-1),
  m_life(-1),
  m_gold(-1),
  m_mineCount(-1),
  m_isCrashed(-1)
{
    m_id = inJsonValues["id"].asInt();
    m_name = inJsonValues["name"].asString();
    m_userId = inJsonValues["userId"].asString();
    m_elo = inJsonValues["elo"].asInt();
    m_position.setX(inJsonValues["pos"]["x"].asInt());
    m_position.setY(inJsonValues["pos"]["y"].asInt());
    m_life = inJsonValues["life"].asInt();
    m_gold = inJsonValues["gold"].asInt();
    m_mineCount = inJsonValues["mineCount"].asInt();
    m_spawnPosition.setX(inJsonValues["spawnPos"]["x"].asInt());
    m_spawnPosition.setY(inJsonValues["spawnPos"]["y"].asInt());
    m_isCrashed = inJsonValues["crashed"].asInt();
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CHero::~CHero()
{
    // Nothing to do
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CHero::update(const Json::Value& inJsonValues)
{
//    m_elo = inJsonValues["elo"].asInt(); ??
    m_position.setX(inJsonValues["pos"]["x"].asInt());
    m_position.setY(inJsonValues["pos"]["y"].asInt());
    m_life = inJsonValues["life"].asInt();
    m_gold = inJsonValues["gold"].asInt();
    m_mineCount = inJsonValues["mineCount"].asInt();
    m_isCrashed = inJsonValues["crashed"].asInt();

//std::cout<<"["<<m_id<<"] "<<m_position.getX()<<" , "<<m_position.getY()<<std::endl;
} // update

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CHero::print()
{
    std::cout<<"-----"<<std::endl;
    std::cout<<"Hero id="<<m_id<<std::endl;
    std::cout<<"Name="<<m_name<<std::endl;
    std::cout<<"UserId="<<m_userId<<std::endl;
    std::cout<<"Elo="<<m_elo<<std::endl;
    std::cout<<"Position="<<m_position.getX()<<" , "<<m_position.getY()<<std::endl;
    std::cout<<"Life="<<m_life<<std::endl;
    std::cout<<"Gold="<<m_gold<<std::endl;
    std::cout<<"MineCount="<<m_mineCount<<std::endl;
    std::cout<<"SpawnPosition="<<m_spawnPosition.getX()<<" , "<<m_spawnPosition.getY()<<std::endl;
    std::cout<<"IsCrashed"<<((m_isCrashed)? "true":"false")<<std::endl;
} // print

} // namespace VDC