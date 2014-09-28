#pragma once

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Global includes
#include <string>
#include <chrono>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Local includes

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Forward declarations

namespace MOBE
{

static const std::string G_COL_BEGIN = "\33[40;";
static const std::string G_COL_END   = "\33[m";

static const std::string G_RED       = "0;31m";
static const std::string G_BOLD_RED  = "1;31m";
static const std::string G_ORANGE    = "0;32m";
static const std::string G_GREEN     = "1;32m";
static const std::string G_BROWN     = "0;33m";
static const std::string G_YELLOW    = "1;33m";
static const std::string G_BLUE      = "0;34m";
static const std::string G_BOLD_BLUE = "1;34m";
static const std::string G_PURPLE    = "0;35m";
static const std::string G_PINK      = "1;35m";
static const std::string G_CYAN      = "0;36m";
static const std::string G_BOLD_CYAN = "1;36m";
static const std::string G_GREY      = "0;37m";
static const std::string G_WHITE     = "1;37m";

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Class to measure the time duration in ms
 */
class CDuration
{
    public:

        /**
        * @brief Constructor
        * 
        * At the destruction of this object, the duration of the life of the current object will be displayed
        */
        CDuration(const std::string &inComment) : m_comment(inComment), m_pOutTime(nullptr)
        {
            resetTime();
        } // constructor

        /**
        * @brief Constructor with output value.
        * 
        * If this constructor is used, no display will be done at destruction
        * The duration of the life of the current object will be set to the given outTime variable
        */
        CDuration(const std::string &inComment, float &outTime) : CDuration(inComment)
        {
            m_pOutTime = &outTime;
        } // constructor

        /**
        * @brief Destructor
        * 
        * At the destruction of this object, the duration of the life of the current object will be displayed or set to a variable according to the constructor used
        */
        virtual ~CDuration()
        {
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            float durationInMs = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_start).count()/1000.0;
            if (m_pOutTime)
            {
                *m_pOutTime = durationInMs;
            }
            else
            {
                std::cout<<G_COL_BEGIN<<getTimeRelatedColor(durationInMs)<<m_comment<<" "<<durationInMs<<" ms"<<G_COL_END<<std::endl;
            }
        } // Destructor

        /**
        * @brief get the comment of the current duration
        */
        inline std::string getComment()
        {
            return m_comment;
        } // getComment

        /**
        * @brief reset the starting time of the current duration
        */
        inline void resetTime()
        {
            m_start = std::chrono::steady_clock::now();
        } // resetTime

    private:
        /**
        * @brief return the color string to use according to a given duration
        * @param inDurationMs the duration to analyse
        * @return the color string to use according to a given duration
        */
        inline std::string getTimeRelatedColor(float inDurationMs)
        {
            std::string outColor;
            if(inDurationMs < 1)
            {
                outColor = G_GREY;
            }
            else if(inDurationMs < 10)
            {
                outColor = G_YELLOW;
            }
            else
            {
                outColor = G_BOLD_RED;
            } // else
            return outColor;
        } // getTimeRelatedColor

    private:
        std::chrono::steady_clock::time_point m_start;
        std::string m_comment;
        float*      m_pOutTime;
}; // class CDuration

} // namespace MOBE
