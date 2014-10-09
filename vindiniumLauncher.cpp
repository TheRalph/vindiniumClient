////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <getopt.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes
#include "CClient.h"
#include "IBehaviorModule.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct SParameters
{
    std::string            m_url;
    std::string            m_key;
    MOBE::E_VINDINIUM_MODE m_mode;
    std::string            m_map;
    int                    m_turns;
    std::string            m_behaviour;
    std::string            m_browser;
};

static char *short_options = (char *)"hu:a:m:n:b:w:";
static struct option long_options[] =
{
    {"help",      no_argument,       NULL, 'h'},
    {"url",       required_argument, NULL, 'u'},
    {"mode",      required_argument, NULL, 'a'},
    {"map",       required_argument, NULL, 'm'},
    {"turns",     required_argument, NULL, 'n'},
    {"behaviour", required_argument, NULL, 'b'},
    {"browser",   required_argument, NULL, 'w'},
    {0, 0, 0, 0}
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void displayHelp(const std::string& inExeName)
{
    std::cout << "Usage: " << inExeName << " [options] <key>"                     << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "Argument:"                                                      << std::endl;
    std::cout << "  <key>"                                                        << std::endl;
    std::cout << "      Your secret key given on the Vindinium server."           << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "Options:"                                                       << std::endl;
    std::cout << "  -h, --help"                                                   << std::endl;
    std::cout << "      Print this help message."                                 << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "  -u <url>, --url=<url>"                                        << std::endl;
    std::cout << "      The URL of the Vindinium sever."                          << std::endl;
    std::cout << "      Default: \"vindinium.org\"."                              << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "  -a <mode>, --mode=<mode>"                                     << std::endl;
    std::cout << "      The game mode (API) to start."                            << std::endl;
    std::cout << "      Available modes: training (0) and arena (1)."             << std::endl;
    std::cout << "      Default: training."                                       << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "  -m <map>, --map=<map>"                                        << std::endl;
    std::cout << "      The map to play with."                                    << std::endl;
    std::cout << "      Only available for the training mode."                    << std::endl;
    std::cout << "      Available maps : m1..m6."                                 << std::endl;
    std::cout << "      Default: m6."                                             << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "  -n <nb turns>, --turns=<nb turns>"                            << std::endl;
    std::cout << "      The number of turns to play."                             << std::endl;
    std::cout << "      Only available for the training mode."                    << std::endl;
    std::cout << "      Default: -1."                                             << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "  -b <behaviour>, --behaviour=<behaviour>"                      << std::endl;
    std::cout << "      The behavior module you want to activate for this game."  << std::endl;
    std::cout << "      Default: random"                                          << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "  -w <browser>, --browser=<browser>"                            << std::endl;
    std::cout << "      The browser your want to open to display the game."       << std::endl;
    std::cout << "      Default: none."                                           << std::endl;
    std::cout                                                                     << std::endl;
    std::cout << "Example: '"<<inExeName<<" --behaviour=random --browser=firefox --mode=arena' uviqc5an"<< std::endl;
    std::cout                                                                     << std::endl;

    // Behaviour
    MOBE::CBehaviorMgr behaviorMrg;
    behaviorMrg.loadBehaviorModules();
    const MOBE::behaviorsMap_t &behaviors = behaviorMrg.getBehaviors();
    std::cout << "Number of available behaviours: " << behaviors.size() << std::endl;
    std::cout << std::endl;
    std::cout << std::setw(20) << std::left << "behavior_name"  << "comment" << std::endl;
    std::cout << std::setw(20) << std::left << "-------------" << "-------" << std::endl;
    for (auto &behaviorIt : behaviors)
    {
        MOBE::IBehaviorModule *pBehavior = behaviorIt.second;
        std::cout << std::setw(20) << std::left
                  << pBehavior->getName() << pBehavior->getComment() << std::endl;
    } // for
    std::cout << std::setw(20) << std::left << "-------------" << "-------" << std::endl;
    std::cout << std::endl;
} // displayHelp

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool parseOptions(const int inArgc, char **inArgv, struct SParameters& outParameters)
{
    bool parseOk = true;
    std::string exeName = std::string(basename(inArgv[0]));

    if (inArgc == 1)
    {
        displayHelp(exeName);
        parseOk = false;
    }
    else
    {
        int opt = 0;
        do
        {
            opt = getopt_long(inArgc, inArgv, short_options, long_options, NULL);

            if (opt > 0)
            {
                const char optChar = (char)(opt&255); 
                switch(optChar)
                {
                    case 'h':
                        displayHelp(exeName);
                        break;

                    case 'u':
                        outParameters.m_url = std::string(optarg);
                        break;

                    case 'k':
                        outParameters.m_key = std::string(optarg);
                        break;

                    case 'b':
                        outParameters.m_behaviour = std::string(optarg);
                        break;

                    case 'a':
                        {
                            int mode = atoi(optarg);
                            if ((mode >= 0) && (mode < MOBE::NB_VINDINIUM_MODE))
                            {
                                outParameters.m_mode = (MOBE::E_VINDINIUM_MODE)mode;
                            }
                            else
                            {
                                std::cerr << "Invalid mode" << std::endl;
                                parseOk = false;
                            } // else
                        }
                        break;

                    case 'n':
                        outParameters.m_turns = atoi(optarg);
                        break;

                    case 'm':
                        outParameters.m_map = std::string(optarg);
                        break;

                    case 'w':
                        outParameters.m_browser = std::string(optarg);
                        break;

                    case '?':
                        displayHelp(exeName);
                        parseOk=false;
                        break;

                    default:
                        std::cerr<<"Unknown option '"<<optChar<<"'"<<std::endl;
                        parseOk=false;
                        break;
                } // switch
            } else {}
        } while (opt != -1 && parseOk);

        // One argument after options: the key
        if (optind != inArgc - 1)
        {
            displayHelp(exeName);
            parseOk=false;
        }
        else
        {
            outParameters.m_key = std::string(inArgv[optind]);
        }
    } // else

    return parseOk;
} // parseOptions

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int inArgC, char** inpArgV)
{
    struct SParameters parameters =
    {
        .m_url =        "vindinium.org",
        .m_key =        "",
        .m_mode =       MOBE::E_VINDINIUM_TRAINING_MODE,
        .m_map =        "m6",
        .m_turns =      -1,
        .m_behaviour =  "random",
        .m_browser =    ""
    };

    bool parseOk = parseOptions(inArgC, inpArgV, parameters);
    if ( parseOk && !parameters.m_key.empty())
    {
        MOBE::CClient vdcClient(parameters.m_key,
                                parameters.m_url,
                                parameters.m_behaviour,
                                parameters.m_browser);

        if (!vdcClient.startGame(parameters.m_mode,
                                parameters.m_turns,
                                parameters.m_map))
        {
            std::cerr << "Cannot start game in training mode with key '"
                      << vdcClient.getKey()
                      << "' on server '" << vdcClient.getServerHostName()
                      << "'" << std::endl;
        }
        else
        {
            // End of the game
        } // else
    } else {}

    std::cout << "Bye bye!!!" << std::endl;
    return (parseOk? 0:1);
} // main

