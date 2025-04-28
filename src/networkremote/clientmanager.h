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

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QList>

class Application;
class NetworkRemoteClient;

class NetworkRemoteClientManager : public QObject
{
  Q_OBJECT
public:
  explicit NetworkRemoteClientManager(Application *app, QObject *parent = nullptr);
  ~NetworkRemoteClientManager();
  void AddClient(QTcpSocket *socket);

private Q_SLOTS:
  void RemoveClient(NetworkRemoteClient *client);
  void Error(QAbstractSocket::SocketError socketError);
  void StateChanged();

private:
  Application *app_;
  QList<NetworkRemoteClient*> clients_;
};

#endif // CLIENTMANAGER_H
