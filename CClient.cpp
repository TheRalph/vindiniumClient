////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
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

namespace VDC
{

static const std::string G_SUBURL_PER_MODE[NB_VINDINIUM_MODE]={
    "api/training",
    "api/arena"
}; ///< Dictionary of Vindinium modes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CClient::CClient(const std::string& inVindiniumKey, const std::string& inVindiniumUrl)
: m_key(inVindiniumKey),
  m_svrHostName(inVindiniumUrl)
{
    m_svrIp = m_httpTools.getIpFromHostName(inVindiniumUrl);
    srand(time(NULL)); // init random
    std::cout<<"Vindinium client with key: '"<<inVindiniumKey<<"' has been created at '"<<m_svrHostName<<"' ("<<m_svrIp<<")."<<std::endl;
} // Constructor

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CClient::~CClient()
{
    std::cout<<"Vindinium client with key: '"<<m_key<<"' has been destroyed."<<std::endl;
} // Destructor

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
E_VINDINIUM_ACTIONS CClient::playAI(const CGame& /*inGame*/)
{
    return (E_VINDINIUM_ACTIONS)(rand()%NB_VINDINIUM_ACTIONS);
} // playAI

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool CClient::startGame(const E_VINDINIUM_MODE inMode)
{
    bool retVal = false;

    std::string fullUrl(m_svrHostName+"/"+G_SUBURL_PER_MODE[inMode]);

//std::cout<<"fullURL="<<fullUrl<<std::endl;

    std::string jsonResult;
    if (!m_httpTools.getDataFile( fullUrl,     // the url to use
                                  jsonResult,  // the result
                                  false,       // no http header
                                  "application/x-www-form-urlencoded", // the mime format
                                  "",          // no cookie
                                  "POST",      // the request method
//                                  "key="+m_key // the vindinum key
                                  "key="+m_key+"&turns=50&map=m1" // the vindinum key and training options
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

currentGame.print();

            while (!currentGame.isFinished())
            {
                E_VINDINIUM_ACTIONS newDirection = playAI(currentGame);

                m_httpTools.getDataFile( playUrlForRequest, // the url to use
                                         jsonResult,  // the result
                                         false,       // no http header
                                         "application/x-www-form-urlencoded", // the mime format
                                         "",          // no cookie
                                         "POST",      // the request method
                                         "key="+m_key+"&dir="+G_VINDINIUM_ACTIONS_DICTIONARY[newDirection] // the vindinum key and direction
                                       );
                jsonReader.parse(jsonResult, jsonValues);
                currentGame.update(jsonValues["game"]); // hero update is done in game

                std::cout<<"---> Turn="<<currentGame.getTrueTurn()<<"/"<<currentGame.getTrueMaxTurn()<<" ( "<<(int)(100.0*(float)currentGame.getTrueTurn()/(float)currentGame.getTrueMaxTurn())<<" % ) # Finished: "<<((currentGame.isFinished())? "true":"false")<<'\r';
                std::cout.flush();

//                usleep(10); /// do not get to much cpu ...
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

    return retVal;
} // startGame

} // namespace VDC