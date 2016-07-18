#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum PVRFbcChannelType { sd, hd, radio };

struct PVRFbcChannel
{
    PVRFbcChannel( std::string url = "", std::string name = "" ): id(0), url( url ), name( name ), type( sd ) {}
    std::uint32_t id;
    std::string url;
    std::string name;
    PVRFbcChannelType type;
};


struct PVRFbcChannelGroup
{
    PVRFbcChannelGroup( std::uint32_t id = 0, std::string name = "" ): id( id ), name( name ) {}
    std::uint32_t id;
    std::string name;
    std::vector<std::uint32_t> members;
};
