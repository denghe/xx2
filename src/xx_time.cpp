#pragma once
#include "xx_time.h"

namespace xx {

    int64_t TimePointToEpoch10m(std::chrono::system_clock::time_point const& val) {
        return std::chrono::duration_cast<duration_10m>(val.time_since_epoch()).count();
    }

    int64_t TimePointToEpoch10m(std::chrono::steady_clock::time_point const& val) {
        return std::chrono::duration_cast<duration_10m>(val.time_since_epoch()).count();
    }

    std::chrono::system_clock::time_point Epoch10mToTimePoint(int64_t const& val) {
        return std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(duration_10m(val)));
    }

    std::chrono::system_clock::time_point Now() {
        return std::chrono::system_clock::now();
    }

    std::chrono::system_clock::time_point NowTimePoint() {
        return std::chrono::system_clock::now();
    }

    std::chrono::steady_clock::time_point NowSteadyTimePoint() {
        return std::chrono::steady_clock::now();
    }

    int64_t NowEpoch10m() {
        return TimePointToEpoch10m(NowTimePoint());
    }

    int64_t NowEpochMicroseconds() {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    int64_t NowEpochMilliseconds() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    double NowEpochSeconds() {
        return (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000000.0;
    }

    double NowEpochSeconds(double& last) {
        auto now = NowEpochSeconds();
        auto rtv = now - last;
        last = now;
        return rtv;
    }

    int64_t Epoch10mToUtcDateTimeTicks(int64_t const& val) {
        return val + 621355968000000000LL;
    }

    int64_t UtcDateTimeTicksToEpoch10m(int64_t const& val) {
        return val - 621355968000000000LL;
    }

    int32_t TimePointToEpoch(std::chrono::system_clock::time_point const& val) {
        return (int32_t)(val.time_since_epoch().count() / 10000000);
    }

    std::chrono::system_clock::time_point EpochToTimePoint(int32_t const& val) {
        return std::chrono::system_clock::time_point(std::chrono::system_clock::time_point::duration((int64_t)val * 10000000));
    }

    int64_t NowSteadyEpoch10m() {
        return TimePointToEpoch10m(NowSteadyTimePoint());
    }

    int64_t NowSteadyEpochMicroseconds() {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    int64_t NowSteadyEpochMilliseconds() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    double NowSteadyEpochSeconds() {
        return (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000.0;
    }

    double NowSteadyEpochSeconds(double& last) {
        auto now = NowSteadyEpochSeconds();
        auto rtv = now - last;
        last = now;
        return rtv;
    }

}
