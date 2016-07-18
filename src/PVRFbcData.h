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

#include "PVRFbcChannel.h"
#include "client.h"

class PVRFbcData
{
public:
  PVRFbcData( std::string const &fbcHostName = "fritz.box" );
  virtual ~PVRFbcData(void);

  virtual int GetChannelsAmount(void);
  virtual PVR_ERROR GetChannels(ADDON_HANDLE handle, bool bRadio);
  virtual bool GetChannel(const PVR_CHANNEL &channel, PVRFbcChannel &myChannel);

  virtual int GetChannelGroupsAmount(void);
  virtual PVR_ERROR GetChannelGroups(ADDON_HANDLE handle, bool bRadio);
  virtual PVR_ERROR GetChannelGroupMembers(ADDON_HANDLE handle, const PVR_CHANNEL_GROUP &group);

protected:
  virtual std::string GetFileContents(std::string const& url);
  virtual bool LoadM3uData(void);
  virtual std::vector<PVRFbcChannel> ParseM3u( std::string const &input );
private:
  std::string fbcHostName;
  std::vector<PVRFbcChannelGroup> m_groups;
  std::vector<PVRFbcChannel>      m_channels;
};
