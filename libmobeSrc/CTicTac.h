///////////////////////////////////////////////////////
//                                                   //
//  Copyright © 2005-2008 by SuperSonic Imagine, SA  //
//         Confidential - All Right Reserved         //
//                                                   //
///////////////////////////////////////////////////////
//
// common / CTicTac.h
//
#pragma once

#include <string>
#include <sys/time.h>

namespace common
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CTicTac
{
    public:
        CTicTac(const std::string &inComment, float* pOutTime = 0);
        ~CTicTac();

        std::string get_comment() { return m_comment; }
        float give_me_time(bool inTrace=true, int inLineNb = 0);
        void  reset_time();

    private:
        double diff_timeval_seconds (const timeval &inFirst, const timeval &inSecond);
        double timevalToSeconds(const timeval &inTimeval);


        timeval     m_start;
        std::string m_comment;
        float*      m_pOutTime;

        std::string get_timerelated_color(float inDif_ms);
};

}
