#include <QThread>

#include "networkremote/networkremote.h"
#include "core/application.h"
#include "core/logging.h"
#include "core/player.h"


NetworkRemote* NetworkRemote::sInstance = nullptr;
const char *NetworkRemote::kSettingsGroup = "Remote";

NetworkRemote::NetworkRemote(Application* app, QObject *parent)
    : QObject(parent),
      app_(app),
      original_thread_(nullptr)
{
  setObjectName("Strawberry Remote");
  original_thread_ = thread();
  sInstance = this;
  server_ = new TcpServer(app_);
}

NetworkRemote::~NetworkRemote()
{
  stopTcpServer();
}

void NetworkRemote::Init()
{
  LoadSettings();
  if (use_remote_){
    startTcpServer();
  }
  else {
    stopTcpServer();
  }
  qLog(Debug) << "NetworkRemote Init() ";
}

void NetworkRemote::Update()
{
  LoadSettings();
  if (use_remote_){
    stopTcpServer();
    startTcpServer();
  }
  else {
    stopTcpServer();
  }
  qLog(Debug) << "NetworkRemote Updated ==== ";
}

void NetworkRemote::LoadSettings()
{
  s_->Load();
  use_remote_ = s_->UserRemote();
  local_only_ = s_->LocalOnly();
  remote_port_ = s_->GetPort();
  ipAddr_.setAddress(s_->GetIpAddress());
}

void NetworkRemote::startTcpServer()
{
  server_->StartServer(ipAddr_,remote_port_);
}

void NetworkRemote::stopTcpServer()
{
  if (server_->ServerUp()){
    qLog(Debug) << "TcpServer stopped ";
    server_->StopServer();
  }
}

NetworkRemote* NetworkRemote::Instance() {
  if (!sInstance) {
    // Error
    return nullptr;
  }

  qLog(Debug) << "NetworkRemote instance is up ";
  return sInstance;
}

