#pragma once

/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <utility>
#include <vector>

#include "DVDClock.h"

#define VideoPlayer_AUDIO    1
#define VideoPlayer_VIDEO    2
#define VideoPlayer_SUBTITLE 3
#define VideoPlayer_TELETEXT 4
#define VideoPlayer_RDS      5

class CDVDMsg;
class CDVDStreamInfo;
class CProcessInfo;

class IVideoPlayer
{
public:
  virtual int OnDiscNavResult(void* pData, int iMessage) = 0;
  virtual void GetVideoResolution(unsigned int &width, unsigned int &height) = 0;
  virtual ~IVideoPlayer() = default;
};

class IDVDStreamPlayer
{
public:
  explicit IDVDStreamPlayer(CProcessInfo &processInfo) : m_processInfo(processInfo) {};
  virtual ~IDVDStreamPlayer() = default;
  virtual bool OpenStream(CDVDStreamInfo hint) = 0;
  virtual void CloseStream(bool bWaitForBuffers) = 0;
  virtual void SendMessage(CDVDMsg* pMsg, int priority = 0) = 0;
  virtual void FlushMessages() = 0;
  virtual bool IsInited() const = 0;
  virtual bool AcceptsData() const = 0;
  virtual bool IsStalled() const = 0;

  enum ESyncState
  {
    SYNC_STARTING,
    SYNC_WAITSYNC,
    SYNC_INSYNC
  };
protected:
  CProcessInfo &m_processInfo;
};

struct SStartMsg
{
  double timestamp;
  int player;
  double cachetime;
  double cachetotal;
};

struct SStateMsg
{
  IDVDStreamPlayer::ESyncState syncState;
  int player;
};

class CDVDVideoCodec;

class IDVDStreamPlayerVideo : public IDVDStreamPlayer
{
public:
  explicit IDVDStreamPlayerVideo(CProcessInfo &processInfo) : IDVDStreamPlayer(processInfo) {};
  ~IDVDStreamPlayerVideo() override = default;
  bool OpenStream(CDVDStreamInfo hint) override = 0;
  void CloseStream(bool bWaitForBuffers) override = 0;
  virtual void Flush(bool sync) = 0;
  bool AcceptsData() const override = 0;
  virtual bool HasData() const = 0;
  bool IsInited() const override = 0;
  void SendMessage(CDVDMsg* pMsg, int priority = 0) override = 0;
  virtual void EnableSubtitle(bool bEnable) = 0;
  virtual bool IsSubtitleEnabled() = 0;
  virtual void EnableFullscreen(bool bEnable) = 0;
  virtual double GetSubtitleDelay() = 0;
  virtual void SetSubtitleDelay(double delay) = 0;
  bool IsStalled() const override = 0;
  virtual bool IsRewindStalled() const { return false; }
  virtual double GetCurrentPts() = 0;
  virtual double GetOutputDelay() = 0;
  virtual std::string GetPlayerInfo() = 0;
  virtual int GetVideoBitrate() = 0;
  virtual void SetSpeed(int iSpeed) = 0;
  virtual bool IsEOS() { return false; };
};

class CDVDAudioCodec;
class IDVDStreamPlayerAudio : public IDVDStreamPlayer
{
public:
  explicit IDVDStreamPlayerAudio(CProcessInfo &processInfo) : IDVDStreamPlayer(processInfo) {};
  ~IDVDStreamPlayerAudio() override = default;
  bool OpenStream(CDVDStreamInfo hints) override = 0;
  void CloseStream(bool bWaitForBuffers) override = 0;
  virtual void SetSpeed(int speed) = 0;
  virtual void Flush(bool sync) = 0;
  bool AcceptsData() const override = 0;
  virtual bool HasData() const = 0;
  virtual int  GetLevel() const = 0;
  bool IsInited() const override = 0;
  void SendMessage(CDVDMsg* pMsg, int priority = 0) override = 0;
  virtual void SetVolume(float fVolume) {};
  virtual void SetMute(bool bOnOff) {};
  virtual void SetDynamicRangeCompression(long drc) = 0;
  virtual std::string GetPlayerInfo() = 0;
  virtual int GetAudioChannels() = 0;
  virtual double GetCurrentPts() = 0;
  bool IsStalled() const override = 0;
  virtual bool IsPassthrough() const = 0;
  virtual float GetDynamicRangeAmplification() const = 0;
  virtual bool IsEOS() { return false; };
};
