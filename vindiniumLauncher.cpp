////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CClient.h"
#include "IBehaviorModule.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void displayHelp(const std::string& inExeName)
{
    std::string exeName(inExeName);
    std::string::size_type idOfPoint = inExeName.find_last_of('/');
    if (idOfPoint != std::string::npos)
    {
        exeName = inExeName.substr(idOfPoint+1);
    } else {}
//TODO    std::cout<<"Syntax: "<<exeName<<" vindinium_key behavior_name [mode] [nb_turns] [map] [navigator]"<<std::endl;
    std::cout<<"Syntax: "<<exeName<<" vindinium_key behavior_name [navigator]"<<std::endl;
    std::cout<<"        "<<exeName<<" --help"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"'vindinium_key' is your secret key given on the web site vindinium.org."<<std::endl;
    std::cout<<"'behavior_name' is the name of the behavior module you want to activate for this game."<<std::endl;
//TODO    std::cout<<"'mode'          is the game mode to start: ARENA or TRAINING."<<std::endl;
//TODO    std::cout<<"'nb_turns        is the number of turn to play. Only available for TRAINING mode."<<std::endl;
//TODO    std::cout<<"'map            is the map to play with. 6 maps are available: m1 m2 m3 m4 m5 m6. Only available for TRAINING mode."<<std::endl;
    std::cout<<"'navigator'     is the name of the navigator your want to open to didplay the game. No navigator by default."<<std::endl;
    std::cout<<std::endl;
    MOBE::CBehaviorMgr behaviorMrg;
    behaviorMrg.loadBehaviorModules();
    const MOBE::behaviorsMap_t &behaviors = behaviorMrg.getBehaviors();
    std::cout<<"Nb available behaviors: "<<behaviors.size()<<std::endl;
    std::cout<<std::setw(20)<<std::left<<"behavior_name"<<"comment"<<std::endl;
    std::cout<<std::setw(20)<<std::left<<"-------------"<<"-------"<<std::endl;
    for (auto &behaviorIt : behaviors)
    {
        MOBE::IBehaviorModule *pBehavior = behaviorIt.second;
        std::cout<<std::setw(20)<<std::left<<pBehavior->getName()<<pBehavior->getComment()<<std::endl;
    } // for
    std::cout<<std::setw(20)<<std::left<<"-------------"<<"-------"<<std::endl;

//TODO use --vindinium_key, --behavior_name, --mode, --nb_turn, --map, --navigator
} // displayHelp

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int inArgC, char** inpArgV)
{
    std::string exeName(inpArgV[0]);
    if (inArgC < 2 || inArgC > 4)
    {
        displayHelp(exeName);
    }
    else
    {
        int nbTurns = -1;
        std::string vindiniumKey(inpArgV[1]),
                    vindiniumURL("vindinium.org"),
                    activeBehavior(""),
                    navigator(""),
                    gameMap("m1");

        MOBE::E_VINDINIUM_MODE gameMode = MOBE::E_VINDINIUM_TRAINING_MODE;
//        MOBE::E_VINDINIUM_MODE gameMode = MOBE::E_VINDINIUM_ARENA_MODE;

        if (vindiniumKey == "--help")
        {
            displayHelp(exeName);
        }
        else
        {
            if (inArgC >= 3)
            {
                activeBehavior=inpArgV[2];
            } else {}

            if (inArgC >= 4)
            {
                navigator=inpArgV[3];
            } else {}

            MOBE::CClient vdcClient(vindiniumKey, vindiniumURL, activeBehavior, navigator);
            if (!vdcClient.startGame(gameMode, nbTurns, gameMap))
            {
                std::cerr<<"Can not start game in Trainig mode with key '"<<vdcClient.getKey()<<"' on server '"<<vdcClient.getServerHostName()<<"'"<<std::endl;
            }
            else
            {
                // End of the game
            } // else
        } // else
    } // else

    std::cout<<"Bye bye!!!"<<std::endl;
    return 0;
} // main