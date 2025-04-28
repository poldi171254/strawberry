/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2025, Leopold List <leo@zudiewiener.com>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef OUTGOINGMSG_H
#define OUTGOINGMSG_H

#include <QObject>
#include <QByteArray>
#include "playlist/playlistitem.h"
#include "includes/shared_ptr.h"
#include "networkremote/RemoteMessages.pb.h"

class Application;
class Playlist;
class Player;
class QTcpSocket;

namespace nw {namespace remote {
  class Message;
  class RequestSongMetadata;
  class ResponseSongMetadata;  
}}


class NetworkRemoteOutgoingMsg : public QObject
{
     Q_OBJECT
public:
  explicit NetworkRemoteOutgoingMsg(Application *app, QObject *parent = nullptr);
  void Init(QTcpSocket*, SharedPtr<Player>);
  void SendCurrentTrackInfo();
  void SendMsg();

private:
  Application *app_;
  PlaylistItemPtr currentItem_;
  Playlist *playlist_;
  QTcpSocket *socket_;
  qint32 msgType_;
  QByteArray msgStream_;
  nw::remote::Message *msg_;
  long bytesOut_;
  std::string msgString_;
  nw::remote::SongMetadata *song_;
  nw::remote::ResponseSongMetadata *responeSong_;
  SharedPtr<Player> player_ ;
};

#endif // OUTGOINGMSG_H
