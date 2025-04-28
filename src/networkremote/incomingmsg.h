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

#ifndef INCOMINGMSG_H
#define INCOMINGMSG_H

#include <QObject>
#include <QByteArray>
#include <string>

// Forward declarations
class QTcpSocket;

namespace nw { namespace remote { class Message; } }

class NetworkRemoteIncomingMsg : public QObject
{
  Q_OBJECT
public:
  explicit NetworkRemoteIncomingMsg(QObject *parent = nullptr);
  ~NetworkRemoteIncomingMsg(); 
  void Init(QTcpSocket* socket);
  void SetMsgType();
  qint32 GetMsgType();

private Q_SLOTS:
  void ReadyRead();

Q_SIGNALS:
  void InMsgParsed();

private:
  nw::remote::Message *msg_;
  QTcpSocket *socket_;
  long bytesIn_;
  QByteArray msgStream_;
  std::string msgString_;
  qint32 msgType_;
};

#endif // INCOMINGMSG_H
