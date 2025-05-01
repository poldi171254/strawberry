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

#include "incomingmsg.h"
#include <QTcpSocket>
#include "networkremote/RemoteMessages.pb.h"
#include "core/logging.h"

NetworkRemoteIncomingMsg::NetworkRemoteIncomingMsg(QObject *parent)
  : QObject(parent),
    msg_(new nw::remote::Message),
    socket_(nullptr),
    bytesIn_(0),
    msgType_(0)
{}

NetworkRemoteIncomingMsg::~NetworkRemoteIncomingMsg()
{
  delete msg_;
}

void NetworkRemoteIncomingMsg::Init(QTcpSocket *socket)
{
  socket_ = socket;
  QObject::connect(socket_, &QIODevice::readyRead, this, &NetworkRemoteIncomingMsg::ReadyRead);
}

void NetworkRemoteIncomingMsg::SetMsgType()
{
  msgString_ = msgStream_.toStdString();
  msg_->ParseFromString(msgString_);
  Q_EMIT InMsgParsed();
}

qint32 NetworkRemoteIncomingMsg::GetMsgType()
{
  return msg_->type();
}

void NetworkRemoteIncomingMsg::ReadyRead()
{
  qLog(Debug) << "Ready To Read";
  msgStream_ = socket_->readAll();
  if (msgStream_.length() > 0) SetMsgType();
}
