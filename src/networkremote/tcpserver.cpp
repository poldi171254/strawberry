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

#include "tcpserver.h"
#include "core/logging.h"
#include "networkremote/clientmanager.h"
#include <QNetworkProxy>


NetworkRemoteTcpServer::NetworkRemoteTcpServer(Application* app, QObject *parent)
  : QObject(parent),
    app_(app),
    server_(new QTcpServer(this)),
    clientMgr_(new NetworkRemoteClientManager(app_,this))
{
  connect(server_,&QTcpServer::newConnection, this, &NetworkRemoteTcpServer::NewTcpConnection);
}

void NetworkRemoteTcpServer::StartServer(QHostAddress ipAddr, int port)
{
  server_->setProxy(QNetworkProxy::NoProxy);
  if (server_->listen(ipAddr, port)){
    qLog(Debug) << "TCP Server Started on --- " << ipAddr.toString() << " and port -- " << port;
  }
}

void NetworkRemoteTcpServer::NewTcpConnection()
{
  socket_ = server_->nextPendingConnection();
  clientMgr_->AddClient(socket_);
  qLog(Debug) << "New Socket -------------------";
}

void NetworkRemoteTcpServer::StopServer()
{
  server_->close();
  qLog(Debug) << "TCP Server Stopped ----------------------";
}

bool NetworkRemoteTcpServer::ServerUp()
{
  return server_->isListening();
}

