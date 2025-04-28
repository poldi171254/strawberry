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

#include "outgoingmsg.h"
#include "core/application.h"
#include "core/logging.h"
#include <QTcpSocket>
#include "core/player.h"
#include "playlist/playlistmanager.h"

using namespace Qt::Literals::StringLiterals;

NetworkRemoteOutgoingMsg::NetworkRemoteOutgoingMsg(Application *app, QObject *parent)
  : QObject(parent),
    app_(app),
    msg_(new nw::remote::Message),
    responeSong_(new nw::remote::ResponseSongMetadata)
{
}

void NetworkRemoteOutgoingMsg::Init(QTcpSocket *socket, SharedPtr<Player> player)
{
  socket_ = socket;
  player_ = player;
}

void NetworkRemoteOutgoingMsg::SendCurrentTrackInfo()
{
  msg_->Clear();
  song_ = new nw::remote::SongMetadata;
  responeSong_->Clear();
  currentItem_ = app_->playlist_manager()->active()->current_item();

  if (currentItem_ != nullptr){
    song_->mutable_title()->assign(currentItem_->Metadata().PrettyTitle().toStdString());
    song_->mutable_album()->assign(currentItem_->Metadata().album().toStdString());
    song_->mutable_artist()->assign(currentItem_->Metadata().artist().toStdString());
    song_->mutable_albumartist()->assign(currentItem_->Metadata().albumartist().toStdString());
    song_->set_track(currentItem_->Metadata().track());
    song_->mutable_stryear()->assign(currentItem_->Metadata().PrettyYear().toStdString());
    song_->mutable_genre()->assign(currentItem_->Metadata().genre().toStdString());
    song_->set_playcount(currentItem_->Metadata().playcount());
    song_->mutable_songlength()->assign(currentItem_->Metadata().PrettyLength().toStdString());
    msg_->set_type(nw::remote::MSG_TYPE_REPLY_SONG_INFO);
    msg_->mutable_response_song_metadata()->set_player_state(nw::remote::PLAYER_STATUS_PLAYING);
    msg_->mutable_response_song_metadata()->set_allocated_song_metadata(song_);
  }
  else {
    std::cout << "I cannnot figure out how to get the song data if the song isn't playing";
    /* NOTE:  TODO
     * I couldn't figure out how to get the song data if the song wasn't playing
     *
     * */
    msg_->set_type(nw::remote::MSG_TYPE_UNSPECIFIED);
    msg_->mutable_response_song_metadata()->set_player_state(nw::remote::PLAYER_STATUS_UNSPECIFIED);
  }
  SendMsg();
}

void NetworkRemoteOutgoingMsg::SendMsg()
{
  std::string  msgOut;

  msg_->SerializeToString(&msgOut);


  bytesOut_ = msg_->ByteSizeLong();

  if(socket_->isWritable())
  {
    socket_->write(QByteArray::fromStdString(msgOut));
    qLog(Debug) << socket_->bytesToWrite() << " bytes written to socket " << socket_->socketDescriptor();
    msg_->Clear();
  }
}

