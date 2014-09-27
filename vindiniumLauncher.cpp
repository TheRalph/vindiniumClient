////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CClient.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void displaySyntax(const std::string& inExeName)
{
    std::string exeName(inExeName);
    std::string::size_type idOfPoint = inExeName.find_last_of('/');
    if (idOfPoint != std::string::npos)
    {
        exeName = inExeName.substr(idOfPoint+1);
    } else {}
    std::cout<<"Syntax: "<<exeName<<" vindinium_key behavior_name [navigator]"<<std::endl;
} // displaySyntax

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int inArgC, char** inpArgV)
{
    if (inArgC < 2 && inArgC > 4)
    {
        displaySyntax(inpArgV[0]);
    }
    else
    {
        std::string vindiniumKey(inpArgV[1]),
                    vindiniumURL("vindinium.org"),
                    activeBehavior(""),
                    navigator("");

        if (inArgC >= 3)
        {
            activeBehavior=inpArgV[2];
        } else {}

        if (inArgC >= 4)
        {
            navigator=inpArgV[3];
        } else {}

        BEEN::CClient vdcClient(vindiniumKey, vindiniumURL, activeBehavior, navigator);
        if (!vdcClient.startGame(BEEN::E_VINDINIUM_TRAINING_MODE))
        {
            std::cerr<<"Can not start game in Trainig mode with key '"<<vdcClient.getKey()<<"' on server '"<<vdcClient.getServerHostName()<<"'"<<std::endl;
        }
        else
        {
            // End of the game
        } // else
    } // else

    std::cout<<"Bye bye!!!"<<std::endl;
    return 0;
} // main

