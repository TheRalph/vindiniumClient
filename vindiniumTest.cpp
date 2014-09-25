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
    std::cout<<"Syntax: "<<exeName<<" vindinium_key [navigator]"<<std::endl;
} // displaySyntax

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int inArgC, char** inpArgV)
{
    if (inArgC < 2 && inArgC > 3)
    {
        displaySyntax(inpArgV[0]);
    }
    else
    {
        VDC::CClient vdcClient(inpArgV[1]);
        if (inArgC==3)
        {
            vdcClient.setNavigator(inpArgV[2]);
        } else {}

        if (!vdcClient.startGame(VDC::E_VINDINIUM_TRAINING_MODE))
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

