#pragma once

#include "PVRFbcChannel.h"
#include "client.h"

class PVRFbcData
{
public:
  PVRFbcData( std::string const &fbcHostName );
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
