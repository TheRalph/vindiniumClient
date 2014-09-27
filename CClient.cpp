////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CClient.h"
#include "CGame.h"
#include "CHero.h"
#include "CTicTac.h"
#include "IBehaviorModule.h"

#define PROFILE_TIME 0

namespace BEEN
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
E_GAME_ACTIONS CClient::playAI(const CGame& /*inGame*/)
{
    return (E_GAME_ACTIONS)(rand()%NB_VINDINIUM_ACTIONS);
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
                    E_GAME_ACTIONS newDirection = m_pActiveBehavior->playBehavior(currentGame);
{
#if (PROFILE_TIME == 1)
common::CTicTac toc("http");
#endif
                    m_httpTools.getDataFile( playUrlForRequest, // the url to use
                                            jsonResult,  // the result
                                            false,       // no http header
                                            "application/x-www-form-urlencoded", // the mime format
                                            "",          // no cookie
                                            "POST",      // the request method
                                            "key="+m_key+"&dir="+G_VINDINIUM_ACTIONS_DICTIONARY[newDirection] // the vindinum key and direction
                                        );
}

                    jsonReader.parse(jsonResult, jsonValues);

{
#if (PROFILE_TIME == 1)
common::CTicTac toc("update");
#endif
                    currentGame.update(jsonValues["game"]); // hero update is done in game
}

                    std::cout<<"---> Turn="<<currentGame.getTrueTurn()<<"/"<<currentGame.getTrueMaxTurn()<<" ( "<<(int)(100.0*(float)currentGame.getTrueTurn()/(float)currentGame.getTrueMaxTurn())<<" % )"<<'\r';
                    std::cout.flush();
                } // while
                retVal = currentGame.isFinished();
                std::cout<<std::endl;
                int idMax = 0;
                for (int i = 0; i < currentGame.getNbPlayers(); i++)
                {
                    CHero& hero = currentGame.getHero(i);
                    std::cout<<hero.getName()<<": "<<hero.getGold()<<std::endl;
                    if (hero.getGold() > currentGame.getHero(idMax).getGold())
                    {
                        idMax = i;
                    } else {}
                } // for
                std::cout<<"===> Winner is "<<currentGame.getHero(idMax).getName()<<std::endl;
            } // else
        } // else
    } // else

    return retVal;
} // startGame

} // namespace BEEN
