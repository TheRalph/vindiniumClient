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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CHero.h"

namespace MOBE
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
    m_position.setX(inJsonValues["pos"]["y"].asInt()); // x/y are inverted
    m_position.setY(inJsonValues["pos"]["x"].asInt()); // x/y are inverted
    m_life = inJsonValues["life"].asInt();
    m_gold = inJsonValues["gold"].asInt();
    m_mineCount = inJsonValues["mineCount"].asInt();
    m_spawnPosition.setX(inJsonValues["spawnPos"]["y"].asInt()); // x/y are inverted
    m_spawnPosition.setY(inJsonValues["spawnPos"]["x"].asInt()); // x/y are inverted
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
    m_position.setX(inJsonValues["pos"]["y"].asInt()); // x/y are inverted
    m_position.setY(inJsonValues["pos"]["x"].asInt()); // x/y are inverted
    m_life = inJsonValues["life"].asInt();
    m_gold = inJsonValues["gold"].asInt();
    m_mineCount = inJsonValues["mineCount"].asInt();
    m_isCrashed = inJsonValues["crashed"].asInt();
    m_lastAction = getLastActionId(inJsonValues["lastDir"].asString());
//std::cout<<"["<<m_id<<"] "<<m_position.getX()<<" , "<<m_position.getY()<<std::endl;
} // update

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CHero::getLastActionId(const std::string &inActionStr) const
{
    E_BEHAVIOR_ACTIONS retVal = E_ACTION_STAY;
    bool actionFound = false;
    for (int i = 0; !actionFound && i < NB_BEHAVIOR_ACTIONS; i++)
    {
        actionFound = (G_BEHAVIOR_ACTIONS_DICTIONARY[i] == inActionStr);
        if (actionFound)
        {
            retVal = (E_BEHAVIOR_ACTIONS)i;
        } else {}
    } // for
    return retVal;
} // getLastActionId

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CHero::print() const
{
    std::cout<<"-----"<<std::endl;
    std::cout<<"Hero id="<<m_id<<std::endl;
    std::cout<<"Name="<<m_name<<std::endl;
    std::cout<<"UserId="<<m_userId<<std::endl;
    std::cout<<"Elo="<<m_elo<<std::endl;
    std::cout<<"Position="<<m_position.getX()<<" , "<<m_position.getY()<<std::endl;
    std::cout<<"LastAction"<<G_BEHAVIOR_ACTIONS_DICTIONARY[m_lastAction]<<std::endl;
    std::cout<<"Life="<<m_life<<std::endl;
    std::cout<<"Gold="<<m_gold<<std::endl;
    std::cout<<"MineCount="<<m_mineCount<<std::endl;
    std::cout<<"SpawnPosition="<<m_spawnPosition.getX()<<" , "<<m_spawnPosition.getY()<<std::endl;
    std::cout<<"IsCrashed"<<((m_isCrashed)? "true":"false")<<std::endl;
} // print

} // namespace MOBE