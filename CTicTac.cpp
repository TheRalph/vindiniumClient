///////////////////////////////////////////////////////
//                                                   //
//  Copyright © 2005-2008 by SuperSonic Imagine, SA  //
//         Confidential - All Right Reserved         //
//                                                   //
///////////////////////////////////////////////////////
//
// common // CTicTac.cpp
//
#include "CTicTac.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace common;

const std::string COL_BEGIN = "\33[40;";
const std::string COL_END   = "\33[m";

const std::string RED       = "0;31m";
const std::string BOLD_RED  = "1;31m";
const std::string ORANGE    = "0;32m";
const std::string GREEN     = "1;32m";
const std::string BROWN     = "0;33m";
const std::string YELLOW    = "1;33m";
const std::string BLUE      = "0;34m";
const std::string BOLD_BLUE = "1;34m";
const std::string PURPLE    = "0;35m";
const std::string PINK      = "1;35m";
const std::string CYAN      = "0;36m";
const std::string BOLD_CYAN = "1;36m";
const std::string GREY      = "0;37m";
const std::string WHITE     = "1;37m";

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
double CTicTac::diff_timeval_seconds (const timeval &inFirst, const timeval &inSecond)
{
    double first = timevalToSeconds(inFirst),
           second = timevalToSeconds(inSecond);

    return (double) std::max(first, second) - std::min(first,second);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
double CTicTac::timevalToSeconds(const timeval &inTimeval)
{
    double ret = (double)inTimeval.tv_sec + ((double)inTimeval.tv_usec)/1000000.;
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CTicTac::CTicTac(const std::string &inComment, float* pOutTime) :
    m_comment(inComment),
    m_pOutTime(pOutTime)
{
    gettimeofday(&m_start,0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CTicTac::~CTicTac()
{
    timeval time_now;

    gettimeofday(&time_now,0);

    const float dif_ms=diff_timeval_seconds(m_start,time_now)*1000.;
    if (m_pOutTime)
    {
        *m_pOutTime = dif_ms;
    }
    else
    {
        std::cout<<COL_BEGIN<<get_timerelated_color(dif_ms)<<m_comment<<" "<<dif_ms<<" ms"<<COL_END<<std::endl;
    }

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
float CTicTac::give_me_time(bool inTrace, int inLineNb)
{
    timeval time_now;

    gettimeofday(&time_now,0);
    const float dif_ms=diff_timeval_seconds(m_start,time_now)*1000.;
    if (true == inTrace)
    {
        std::stringstream stream;
        stream << get_timerelated_color(dif_ms) << inLineNb << " GMT_" << m_comment << " " << dif_ms << " ms";
        std::cout<<COL_BEGIN<<stream.str()<<COL_END<<std::endl;
    }
    return dif_ms;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CTicTac::reset_time()
{
    gettimeofday(&m_start,0);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string CTicTac::get_timerelated_color(float inDif_ms)
{
    std::string outColor;
    if(inDif_ms< 1)
    {
        outColor = GREY;
    }
    else if(inDif_ms < 10)
    {
        outColor = YELLOW;
    }
    else
    {
        outColor = BOLD_RED;
    }
    return outColor;
}
