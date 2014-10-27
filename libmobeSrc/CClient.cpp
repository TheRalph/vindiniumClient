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
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CClient.h"
#include "CGame.h"
#include "CHero.h"
#include "IBehaviorModule.h"
#include "CDuration.h"

namespace MOBE
{

static const std::string G_SUBURL_PER_MODE[NB_VINDINIUM_MODE]={
    "api/training",
    "api/arena"
}; ///< Dictionary of Vindinium modes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CClient::CClient(const std::string& inVindiniumKey, const std::string& inVindiniumUrl, const std::string& inBehaviorName, const std::string& inNavigator)
: m_key(inVindiniumKey),
  m_svrHostName(inVindiniumUrl),
  m_navigator(inNavigator),
  m_pActiveBehavior(NULL)

{
    m_svrIp = m_httpTools.getIpFromHostName(inVindiniumUrl);
    srand(time(NULL)); // init random

    /// load behaviors
    int nbLoadedModules = m_behaviorMgr.loadBehaviorModules();
    std::cout<<"Nb available behavior: "<<nbLoadedModules<<std::endl;

    if (!inBehaviorName.empty())
    {
        setActiveBehavior(inBehaviorName);
    } else {}

    std::cout<<"Vindinium client with key: '"<<inVindiniumKey<<"' has been created. Given server: '"<<m_svrHostName<<"' ("<<m_svrIp<<")."<<std::endl;
    std::cout<<"Waiting server for a new game ..."<<std::endl;
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CClient::~CClient()
{
    std::cout<<"Vindinium client with key: '"<<m_key<<"' has been destroyed."<<std::endl;
} // Destructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CClient::setActiveBehavior(const std::string& inBehaviorName)
{
    IBehaviorModule *pBehavior = m_behaviorMgr.getBehavior(inBehaviorName);

    if (pBehavior)
    {
        m_pActiveBehavior = pBehavior;
        std::cout<<"Active behavior: "<<m_pActiveBehavior->getName()<<std::endl;
    }
    else 
    {
        std::cerr<<"ERROR: can not find behavior '"<<inBehaviorName<<"'"<<std::endl;
    }
} // setActiveBehavior

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CClient::setNavigator(const std::string& inNavigator)
{
    m_navigator = inNavigator;
} // setNavigator

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string CClient::getServerIp() const
{
    return m_svrIp;
} // getServerIp

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string CClient::getServerHostName() const
{
    return m_svrHostName;
} // getServerHostName

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string CClient::getKey() const
{
    return m_key;
} // getKey

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
E_BEHAVIOR_ACTIONS CClient::playAI(const CGame& /*inGame*/)
{
    return (E_BEHAVIOR_ACTIONS)(rand()%NB_BEHAVIOR_ACTIONS);
} // playAI

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CClient::startGame(const E_VINDINIUM_MODE inMode, const int inNbTurns, const std::string& inMap)
{
    bool retVal = false;

    if (!m_pActiveBehavior)
    {
        std::cerr<<"ERROR: No active behavior"<<std::endl;
    }
    else
    {
        std::string fullUrl(m_svrHostName+"/"+G_SUBURL_PER_MODE[inMode]);

//std::cout<<"fullURL="<<fullUrl<<std::endl;
        std::stringstream sstr;
        sstr<<"key="<<m_key;
        if (inNbTurns > 0)
        {
            sstr<<"&turns="<<inNbTurns;
        } else {}
        if (!inMap.empty())
        {
            sstr<<"&map="<<inMap;
        } else {}

        std::string jsonResult;
        if (!m_httpTools.getDataFile( fullUrl,     // the url to use
                                      jsonResult,  // the result
                                      false,       // no http header
                                      "application/x-www-form-urlencoded", // the mime format
                                      "",          // no cookie
                                      "POST",      // the request method
                                      sstr.str()   // the vindinum play configuration
                                     ))
        {
            std::cerr<<"Can not connect to '"<<fullUrl<<"'"<<std::endl;
        }
        else
        {
//std::cout<<result<<std::endl;

            Json::Value jsonValues;
            Json::Reader jsonReader;

            if (!jsonReader.parse(jsonResult, jsonValues))
            {
                std::cerr<<"Can not parse json data from '"<<fullUrl<<"'"<<std::endl;
            }
            else
            {
                std::cout<<"Starting game..."<<std::endl;

                CGame currentGame(jsonValues["game"], jsonValues["hero"]["id"].asInt());

                std::string token   = jsonValues["token"].asString();
                std::string viewUrl = jsonValues["viewUrl"].asString();
                std::string playUrl = jsonValues["playUrl"].asString();
                std::string playUrlForRequest(playUrl);

                /// suppress http://
                if (playUrlForRequest.size() > 7 && playUrlForRequest.compare(0,7,"http://")==0 )
                {
                    playUrlForRequest = playUrlForRequest.substr(7);
                } else {}
                std::cout<<std::endl<<"viewURL = '"<<viewUrl<<"'"<<std::endl;
                std::cout<<std::endl;

                if (!m_navigator.empty())
                {
                    ::system(std::string(m_navigator+" "+viewUrl+"&").c_str());
                } else {}
//currentGame.print();

                while (!currentGame.isFinished())
                {
                    E_BEHAVIOR_ACTIONS newDirection = E_ACTION_STAY;
                    float behaviorTimeMs = 0;
                    {
                        CDuration duration("Behavior", behaviorTimeMs);

                        newDirection = m_pActiveBehavior->playBehavior(currentGame);
                    } // end behavior computation

                    float waitingForServerInMs = 0;
                    {
                        CDuration duration("waitingForServer", waitingForServerInMs);

                        m_httpTools.getDataFile( playUrlForRequest, // the url to use
                                                 jsonResult,  // the result
                                                 false,       // no http header
                                                 "application/x-www-form-urlencoded", // the mime format
                                                 "",          // no cookie
                                                 "POST",      // the request method
                                                 "key="+m_key+"&dir="+G_BEHAVIOR_ACTIONS_DICTIONARY[newDirection] // the vindinum key and direction
                                               );
                    } // end waiting for request

                    float processDataInMs = 0;
                    {
                        CDuration duration("processData", processDataInMs);

                        jsonReader.parse(jsonResult, jsonValues);
                        currentGame.update(jsonValues["game"]); // hero update is done in game
                    } // end data processing

                    std::cout<<"---> Turn="<<std::setw(3)<<currentGame.getTrueTurn()<<"/"<<currentGame.getTrueMaxTurn()<<" ( "<<std::setw(3)<<(int)(100.0*(float)currentGame.getTrueTurn()/(float)currentGame.getTrueMaxTurn())<<" % ) ";
//                    std::cout<<m_pActiveBehavior->getName()<<" ";
                    std::cout<<std::setw(6)<<G_BEHAVIOR_ACTIONS_DICTIONARY[newDirection]<<" ";
                    std::cout.setf( std::ios::fixed, std::ios::floatfield );
//                    std::cout<<std::setprecision(3)<<"Process data="<<std::setw(6)<<processDataInMs<<" ms, Behavior="<<std::setw(6)<<behaviorTimeMs<<" ms, ";
                    std::cout<<std::setprecision(3)<<"Client="<<std::setw(6)<<processDataInMs+behaviorTimeMs<<" ms, ";
                    std::cout<<std::setprecision(3)<<"Server="<<std::setw(8)<<waitingForServerInMs<<" ms, ";
                    for (int heroId = 1; heroId <= currentGame.getNbPlayers(); heroId++)
                    {
                        const CHero& hero = currentGame.getHero(heroId);
                        std::cout<<hero.getName()<<" ("<<std::setw(4)<<hero.getGold()<<") ";
                    } // for
                    std::cout<<std::endl;
//                    std::cout<<"     \r"<<std::endl;
//                    std::cout.flush();
                } // while
                retVal = currentGame.isFinished();
                std::cout<<std::endl;
                int idMax = 0;
                for (int i = 0; i < currentGame.getNbPlayers(); i++)
                {
                    const CHero& hero = currentGame.getHero(i+1);
                    std::cout<<hero.getName()<<": "<<hero.getGold()<<std::endl;
                    if (hero.getGold() > currentGame.getHero(idMax+1).getGold())
                    {
                        idMax = i;
                    } else {}
                } // for
                std::cout<<"===> Winner is "<<currentGame.getHero(idMax+1).getName()<<std::endl;
            } // else
        } // else
    } // else

    return retVal;
} // startGame

} // namespace MOBE
