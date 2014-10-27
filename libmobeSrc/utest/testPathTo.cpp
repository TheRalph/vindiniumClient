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
#include <sstream>
#include <tuple>
#include <json/json.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CGame.h"

#define DISPLAY_DETAILS 0

/// int, int, std::string =>  expected result, heroId, jsonMap
enum E_TEST_DATA_VALUES
{
    E_DATA_SET_NAME = 0,
    E_EXPECTED_VALUE,
    E_MY_HERO_ID,
    E_GAME_JSON_STR,
    NB_TEST_DATA_VALUES
}; // enum E_TEST_DATA_VALUES
typedef std::tuple<std::string, int, int, std::string> testData_t;

const testData_t G_JSON_DATA_01=std::make_tuple("dataSet01",  8, 2, "{\"board\":{\"size\":16,\"tiles\":\"##########  ##    ##  ##################                ##################$1        $1####################  []    []@4####################            ##################    $1    $1    ####################        ########################        ########################        ########################        ####################    $2    $1    ##################  @2@1      ####################  []    []@3####################$2        $1##################                ##################  \
##    ##  ##########\"},\"finished\":false,\"heroes\":[{\"crashed\":false,\"elo\":1748,\"gold\":915,\"id\":1,\"lastDir\":\"West\",\"life\":62,\"mineCount\":6,\"name\":\"loupiot\",\"pos\":{\"x\":11,\"y\":7},\"spawnPos\":{\"x\":3,\"y\":5},\"userId\":\"yud7ft13\"},{\"crashed\":false,\"elo\":1608,\"gold\":563,\"id\":2,\"lastDir\":\"Stay\",\"life\":37,\"mineCount\":2,\"name\":\"TheRalph\",\"pos\":{\"x\":11,\"y\":6},\"spawnPos\":{\"x\":12,\"y\":5},\"userId\":\"q08t7vxp\"},{\"crashed\":true,\"elo\":1608,\"gold\":43,\"id\":3,\"lastDir\":\"Stay\",\"life\":1,\"mineCount\":0,\"name\":\"TheRalph\",\
\"pos\":{\"x\":12,\"y\":10},\"spawnPos\":{\"x\":12,\"y\":10},\"userId\":\"q08t7vxp\"},{\"crashed\":true,\"elo\":1608,\"gold\":19,\"id\":4,\"lastDir\":\"Stay\",\"life\":1,\"mineCount\":0,\"name\":\"TheRalph\",\"pos\":{\"x\":3,\"y\":10},\"spawnPos\":{\"x\":3,\"y\":10},\"userId\":\"q08t7vxp\"}],\"id\":\"e66a0yrv\",\"maxTurns\":1200,\"turn\":801}");

const testData_t G_JSON_DATA_02=std::make_tuple("dataSet02", 86, 2, "{\"board\":{\"size\":16,\"tiles\":\"##########  ##    ##  ##################                ##################$1        $1####################  []    []@4####################            ##################    $1    $1    ####################        ########################        ########################        ########################        ####################    $2    $1    ##################  @2@1      ####################  []    []@3####################$2        $1##################                ##################  ##    ##  ##########\"},\"finished\":false,\"heroes\":[{\"crashed\":false,\"elo\":1748,\"gold\":915,\"id\":1,\"lastDir\":\"West\",\"life\":62,\"mineCount\":6,\
\"name\":\"loupiot\",\"pos\":{\"x\":11,\"y\":7},\"spawnPos\":{\"x\":3,\"y\":5},\"userId\":\"yud7ft13\"},{\"crashed\":false,\"elo\":1608,\"gold\":563,\"id\":2,\"lastDir\":\"Stay\",\"life\":37,\"mineCount\":2,\"name\":\"TheRalph\",\"pos\":{\"x\":1,\"y\":4},\"spawnPos\":{\"x\":12,\"y\":5},\"userId\":\"q08t7vxp\"},{\"crashed\":true,\"elo\":1608,\"gold\":43,\"id\":3,\
\"lastDir\":\"Stay\",\"life\":1,\"mineCount\":0,\"name\":\"TheRalph\",\"pos\":{\"x\":12,\"y\":10},\"spawnPos\":{\"x\":12,\"y\":10},\"userId\":\"q08t7vxp\"},{\"crashed\":true,\"elo\":1608,\"gold\":19,\"id\":4,\"lastDir\":\"Stay\",\"life\":1,\"mineCount\":0,\"name\":\"TheRalph\",\"pos\":{\"x\":3,\"y\":10},\"spawnPos\":{\"x\":3,\"y\":10},\"userId\":\"q08t7vxp\"}],\"id\":\"e66a0yrv\",\"maxTurns\":1200,\"turn\":801}");

const testData_t G_JSON_DATA_03=std::make_tuple("dataSet03", 11, 4, "{\"board\":{\"size\":12,\"tiles\":\"####[]$1########$2[]######$1      ####  @4  $4####$1          @1    $2######  ##  ####  ##  ########  ##        ##  ############        ################        ############@2##        ##  ########  ##  ####  ##  ######$2                $3####$2      ####  @3  \
$3######[]$2########$3[]####\"},\"finished\":false,\"heroes\":[{\"crashed\":false,\"elo\":2025,\"gold\":281,\"id\":1,\"lastDir\":\"East\",\"life\":29,\"mineCount\":3,\"name\":\"alpha\",\"pos\":{\"x\":2,\"y\":7},\
\"spawnPos\":{\"x\":1,\"y\":2},\"userId\":\"x2sx6nrt\"},{\"crashed\":false,\"elo\":1962,\"gold\":281,\"id\":2,\"lastDir\":\"West\",\"life\":89,\"mineCount\":5,\"name\":\"mega1337\",\"pos\":{\"x\":7,\"y\":2},\"spawnPos\":{\"x\":10,\"y\":2},\"userId\":\"iyo6u22k\"},{\"crashed\":false,\"elo\":2072,\"gold\":241,\"id\":3,\"lastDir\":\"South\",\"life\":90,\
\"mineCount\":3,\"name\":\"super1337\",\"pos\":{\"x\":10,\"y\":8},\"spawnPos\":{\"x\":10,\"y\":9},\"userId\":\"6k95c86b\"},{\"crashed\":false,\"elo\":1200,\"gold\":159,\"id\":4,\"lastDir\":\"West\",\
\"life\":52,\"mineCount\":1,\"name\":\"SSITeam\",\"pos\":{\"x\":1,\"y\":8},\"spawnPos\":{\"x\":1,\"y\":9},\"userId\":\"bujv0zwt\"}],\"id\":\"zc1h7zgs\",\"maxTurns\":1200,\"turn\":343}");

const testData_t G_JSON_DATA_04=std::make_tuple("dataSet04", 282, 2, "{\"board\":{\"size\":28,\"tiles\":\"####      ##########  ####    ####  ##########      ######    ##      ######               \
 ######      ##    ####$1          ########  ##    ##  ########          $4####    ####    ##$-      ##    ##      $-##    ####    ####[]  ######$1####  ######    ######@4####$-######  []####              ##    ##$-    $-##    ##              ########  $1@1        ######    ######          $-  ##################      ######        ######      ############################  ##                ##  ##################################      ##    ##      ######################################    ##    ##    ########################################                ##########################################    ####    ########################################$-    ########    $-####################################$-    ########    $-########################################\
    ####    ##########################################                ########################################    ##    ##    ######################################      ##    ##      ##################################  ##                ##  ############################      ######        ######      ##################  $2          ######    ######          $-  ########              ##    ##$-    $-##    ##              ####[]  ######$2####  ######    ######  ####$-######  []####    ####    ##$2    @2##    ##      $-##    ####  @3####$2          ########  ##    ##  ########          $3####    ##      ######                ######      ##    ######      ##########  ####    ####  ##########      ####\"},\"finished\":false,\"heroes\":[{\"crashed\":false,\"elo\":2021,\"gold\":54,\"id\":1,\"lastDir\":\"South\",\
\"life\":17,\"mineCount\":3,\"name\":\"mega1337\",\"pos\":{\"x\":6,\"y\":5},\"spawnPos\":{\"x\":3,\"y\":1},\"userId\":\"iyo6u22k\"},{\"crashed\":false,\"elo\":1936,\"gold\":51,\"id\":2,\"lastDir\":\"East\",\"life\":20,\"mineCount\":4,\"name\":\"kormander\",\"pos\":{\"x\":24,\"y\":11},\"spawnPos\":{\"x\":24,\"y\":1},\"userId\":\"i4v2093x\"},{\"crashed\":false,\"elo\":1200,\"gold\":22,\"id\":3,\"lastDir\":\"Stay\",\"life\":58,\"mineCount\":1,\"name\":\"SSITeam\",\"pos\":{\"x\":24,\"y\":26},\"spawnPos\":{\"x\":24,\"y\":26},\"userId\":\"bujv0zwt\"},{\"crashed\":false,\"elo\":1515,\"gold\":22,\"id\":4,\"lastDir\":\"North\",\"life\":58,\"mineCount\":1,\"name\":\"Terminator\",\"pos\":{\"x\":4,\"y\":18},\"spawnPos\":{\"x\":3,\"y\":26},\"userId\":\"jjqna34p\"}],\"id\":\"ad5sfqmp\",\"maxTurns\":1200,\"turn\":90}");

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool checkMap(const testData_t& inTestData)
{
    bool retVal = false;
    std::cout<<"** "<<std::get<E_DATA_SET_NAME>(inTestData)<<std::endl;
    Json::Value jsonValues;
    Json::Reader jsonReader;

    if (!jsonReader.parse(std::get<E_GAME_JSON_STR>(inTestData), jsonValues))
    {
        std::cerr<<"Can not parse json data"<<std::endl;
    }
    else
    {
        MOBE::CGame myGame(jsonValues, std::get<E_MY_HERO_ID>(inTestData));

    #if (DISPLAY_DETAILS == 1)
        myGame.printBoard();
    #endif

        const unsigned int nbCellsInBoard = myGame.getBoardEdgeSize() * myGame.getBoardEdgeSize();
        const std::vector<MOBE::E_BOARD_OBJECTS>& staticBoard = myGame.getStaticBoard();

        retVal = (staticBoard.size() == nbCellsInBoard);
        if (retVal)
        {
            std::cout<<"Static board size OK"<<std::endl;
        }
        else
        {
            std::cerr<<"Static board size NOT OK"<<std::endl;
        } // else

        int nbTheoricalAvailablePath = 0;
        int nbAvailablePath = 0;
        for (unsigned int i = 0; i < nbCellsInBoard; i++)
        {
            if (staticBoard.at(i) != MOBE::E_IMPASSABLE_WOOD)
            {
                nbTheoricalAvailablePath++;
                MOBE::path_t thePath;
                if (myGame.getPathTo(myGame.getMyHeroId(), i, false, thePath))
                {
                    nbAvailablePath++;
                    const int distToCell = myGame.getDistanceTo(myGame.getMyHeroId(), i, false);
                    int nbCellsInPath = 0;
                    for (int cellId : thePath)
                    {
                        if ( cellId >= 0 && cellId < (int)nbCellsInBoard )
                        {
                            nbCellsInPath++;
                        } else {}
                    } // for
                    MOBE::CPosition cellPos = myGame.get2DCoordOnBoard(i);
                    std::stringstream sstr;
                    std::vector<MOBE::CPosition> myHeroNeighbors = myGame.getMyHero().getPosition().get4Connected();
                    MOBE::CPosition firstPos = myGame.get2DCoordOnBoard(thePath.front());
                    MOBE::CPosition lastPos  = myGame.get2DCoordOnBoard(thePath.back());
                    sstr<<"Path to ("<<cellPos.getX()<<","<<cellPos.getY()<<") ";
                    if ( nbCellsInPath  != distToCell ||
                         lastPos != cellPos           ||
                         (
                            firstPos != myHeroNeighbors.at(0) &&
                            firstPos != myHeroNeighbors.at(1) &&
                            firstPos != myHeroNeighbors.at(2) &&
                            firstPos != myHeroNeighbors.at(3)
                         )
                       )
                    {
                        std::cerr<<"Nb cells in path="<<nbCellsInPath<<" but "<<distToCell<<" was espected."<<std::endl;
                        sstr<<"is NOT OK";
                        retVal = false;
                    }
                    else
                    {
#if (DISPLAY_DETAILS == 1)
                        sstr<<"is OK";
#endif
                    }
#if (DISPLAY_DETAILS == 1)
                    myGame.printPath(thePath, sstr.str());
#endif
                } else {}
            } else {}
        } // for

        if (std::get<E_EXPECTED_VALUE>(inTestData) != nbAvailablePath)
        {
            std::cerr<<"Nb available paths detected="<<nbAvailablePath<<" but "<<nbTheoricalAvailablePath<<" was espected."<<std::endl;
            retVal = false;
        } else {}
    } // else
    return retVal;
} // checkMap

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main( int inArgC, char **inpArgV)
{
    bool retVal = checkMap(G_JSON_DATA_01);
    retVal = checkMap(G_JSON_DATA_02) && retVal;
    retVal = checkMap(G_JSON_DATA_03) && retVal;
    retVal = checkMap(G_JSON_DATA_04) && retVal;

    if (retVal)
    {
        std::cout<<"Test PathTo OK"<<std::endl;
    }
    else
    {
        std::cerr<<"Test PathTo NOT OK"<<std::endl;
    } // else
    return (retVal? 0:1);
} // main
