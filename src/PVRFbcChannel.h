/*
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

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
