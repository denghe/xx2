#pragma once
#include "xx_includes.h"

namespace xx {

    /************************************************************************************/
    // time_point <--> .net DateTime.Now.ToUniversalTime().Ticks converts

    typedef std::chrono::duration<long long, std::ratio<1LL, 10000000LL>> duration_10m;

    int64_t TimePointToEpoch10m(std::chrono::system_clock::time_point const& val);

    int64_t TimePointToEpoch10m(std::chrono::steady_clock::time_point const& val);

    std::chrono::system_clock::time_point Epoch10mToTimePoint(int64_t const& val);

    std::chrono::system_clock::time_point Now();

    std::chrono::system_clock::time_point NowTimePoint();

    std::chrono::steady_clock::time_point NowSteadyTimePoint();

    int64_t NowEpoch10m();

    int64_t NowEpochMicroseconds();

    int64_t NowEpochMilliseconds();

    double NowEpochSeconds();

    double NowEpochSeconds(double& last);

    int64_t Epoch10mToUtcDateTimeTicks(int64_t const& val);

    int64_t UtcDateTimeTicksToEpoch10m(int64_t const& val);

    int32_t TimePointToEpoch(std::chrono::system_clock::time_point const& val);

    std::chrono::system_clock::time_point EpochToTimePoint(int32_t const& val);

    int64_t NowSteadyEpoch10m();

    int64_t NowSteadyEpochMicroseconds();

    int64_t NowSteadyEpochMilliseconds();

    double NowSteadyEpochSeconds();

    double NowSteadyEpochSeconds(double& last);

    template<typename T>
    inline void AppendTimePoint_Local(std::string& s, T const& tp) {
        auto&& t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        auto bak = s.size();
        s.resize(s.size() + 30);
        auto len = std::strftime(&s[bak], 30, "%Y-%m-%d %H:%M:%S", &tm);
        s.resize(bak + len);
    }

    template<typename T>
    inline std::string TimePointToString_Local(T const& tp) {
        std::string s;
        AppendTimePoint_Local(s, tp);
        return s;
    }

}
