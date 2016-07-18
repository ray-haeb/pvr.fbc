
#include "PVRFbcData.h"
#include "client.h"

#include <regex>
#include <iterator>

PVRFbcData::PVRFbcData( std::string const &fbcHostName ):fbcHostName( fbcHostName )
{
    LoadM3uData();
}

std::string PVRFbcData::GetFileContents(std::string const &url)
{
  std::string strContent;
  void* fileHandle = XBMC->OpenFile(url.c_str(), 0);
  if (fileHandle)
  {
    char buffer[1024];
    while (int bytesRead = XBMC->ReadFile(fileHandle, buffer, 1024))
      strContent.append(buffer, bytesRead);
    XBMC->CloseFile(fileHandle);
  }
  return strContent;
}


bool PVRFbcData::LoadM3uData()
{
    m_channels.clear();
    m_groups.clear();
    std::string const urlPrefix = "http://" + fbcHostName + "/dvb/m3u/";
    std::vector<PVRFbcChannel> hd = ParseM3u( GetFileContents( urlPrefix + "hdtv.m3u" ) );
    std::vector<PVRFbcChannel> sd = ParseM3u( GetFileContents( urlPrefix + "sdtv.m3u" ) );
    std::vector<PVRFbcChannel> radio = ParseM3u( GetFileContents( urlPrefix + "radio.m3u" ) );
    std::uint32_t id = 0;
    for( auto &i : hd ) { i.type = PVRFbcChannelType::hd; i.id = id++; }
    for( auto &i : sd ) { i.type = PVRFbcChannelType::sd; i.id = id++; }
    for( auto &i : radio ) { i.type = PVRFbcChannelType::radio; i.id = id++; }
    m_channels.reserve( hd.size() + sd.size() + radio.size() );
    std::move( hd.begin(), hd.end(), std::back_inserter( m_channels ) );
    std::move( sd.begin(), sd.end(), std::back_inserter( m_channels ) );
    std::move( radio.begin(), radio.end(), std::back_inserter( m_channels ) );
    m_groups.emplace_back( id++, "HDTV" );
    m_groups.emplace_back( id++, "SDTV" );
    m_groups.emplace_back( id++, "Radio" );
    for( auto const &i : m_channels )
    {
        switch( i.type )
        {
        case PVRFbcChannelType::hd:
            m_groups[0].members.push_back( i.id );
            break;
        case PVRFbcChannelType::sd:
            m_groups[1].members.push_back( i.id );
            break;
        case PVRFbcChannelType::radio:
            m_groups[2].members.push_back( i.id );
            break;
        }
    }
    return true;
}

std::vector<PVRFbcChannel> PVRFbcData::ParseM3u(std::string const &input)
{
    std::vector<PVRFbcChannel> ret;
    std::regex rx( "#EXTINF:-1,(.*)\\n.*\\n(rtsp://.*)" );
    std::sregex_iterator i( input.begin(), input.end(), rx );
    std::sregex_iterator e;
    while( i != e )
    {
        if( i->size() == 3 )
        {
            ret.emplace_back( (*i)[2].str(), (*i)[1].str() );
        }
        ++i;
    }
}


int PVRFbcData::GetChannelsAmount(void)
{
    return m_channels.size();
}

PVR_ERROR PVRFbcData::GetChannels(ADDON_HANDLE handle, bool bRadio)
{
    for( auto const &channel : m_channels )
    {
        if( bRadio )
        {
            if( channel.type == PVRFbcChannelType::radio )
            {
                PVR_CHANNEL xbmcChannel;
                memset(&xbmcChannel, 0, sizeof(PVR_CHANNEL));
                xbmcChannel.iUniqueId = channel.id;
                xbmcChannel.bIsRadio = true;
                strncpy(xbmcChannel.strChannelName, channel.name.c_str(), sizeof(xbmcChannel.strChannelName) - 1);
                strncpy(xbmcChannel.strStreamURL, channel.url.c_str(), sizeof(xbmcChannel.strStreamURL) - 1);
                xbmcChannel.bIsHidden         = false;
                PVR->TransferChannelEntry(handle, &xbmcChannel);
            }
        }else
        {
            if( channel.type != PVRFbcChannelType::radio )
            {
                PVR_CHANNEL xbmcChannel;
                memset(&xbmcChannel, 0, sizeof(PVR_CHANNEL));
                xbmcChannel.iUniqueId = channel.id;
                xbmcChannel.bIsRadio = false;
                strncpy(xbmcChannel.strChannelName, channel.name.c_str(), sizeof(xbmcChannel.strChannelName) - 1);
                strncpy(xbmcChannel.strStreamURL, channel.url.c_str(), sizeof(xbmcChannel.strStreamURL) - 1);
                xbmcChannel.bIsHidden         = false;
                PVR->TransferChannelEntry(handle, &xbmcChannel);

            }
        }
    }
    return PVR_ERROR_NO_ERROR;
}

int PVRFbcData::GetChannelGroupsAmount(void)
{
    return m_groups.size();
}

PVR_ERROR PVRFbcData::GetChannelGroups(ADDON_HANDLE handle, bool bRadio)
{
    PVR_CHANNEL_GROUP xbmcGroup;
    memset(&xbmcGroup, 0, sizeof(PVR_CHANNEL_GROUP));
    if( bRadio )
    {
        strncpy(xbmcGroup.strGroupName, m_groups[2].name.c_str(), sizeof(xbmcGroup.strGroupName) - 1);
        PVR->TransferChannelGroup(handle, &xbmcGroup);
    }else
    {
        strncpy(xbmcGroup.strGroupName, m_groups[0].name.c_str(), sizeof(xbmcGroup.strGroupName) - 1);
        PVR->TransferChannelGroup(handle, &xbmcGroup);
        memset(&xbmcGroup, 0, sizeof(PVR_CHANNEL_GROUP));
        strncpy(xbmcGroup.strGroupName, m_groups[1].name.c_str(), sizeof(xbmcGroup.strGroupName) - 1);
        PVR->TransferChannelGroup(handle, &xbmcGroup);
    }
    return PVR_ERROR_NO_ERROR;
}

PVR_ERROR PVRFbcData::GetChannelGroupMembers(ADDON_HANDLE handle, const PVR_CHANNEL_GROUP &group)
{
    for( auto const &g : m_groups )
    {
        if( g.name == group.strGroupName )
        {
            for( auto const &id : g.members )
            {
                PVR_CHANNEL_GROUP_MEMBER xbmcGroupMember;
                memset(&xbmcGroupMember, 0, sizeof(PVR_CHANNEL_GROUP_MEMBER));

                strncpy(xbmcGroupMember.strGroupName, group.strGroupName, sizeof(xbmcGroupMember.strGroupName) - 1);
                xbmcGroupMember.iChannelUniqueId = id;

                PVR->TransferChannelGroupMember(handle, &xbmcGroupMember);
            }
        }
    }
}


