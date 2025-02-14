/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2012, David Sansome <me@davidsansome.com>
 * Copyright 2012, 2014, John Maguire <john.maguire@gmail.com>
 * Copyright 2018-2021, Jonas Kvinge <jonas@jkvinge.net>
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

#ifndef APPLICATION_H
#define APPLICATION_H

#include "config.h"

#include <QObject>
#include <QList>
#include <QString>

#include "scoped_ptr.h"
#include "shared_ptr.h"

#include "settings/settingsdialog.h"

class QThread;

class TaskManager;
class ApplicationImpl;
class TagReaderClient;
class Database;
class DeviceFinders;
class Player;
class NetworkAccessManager;
class SCollection;
class CollectionBackend;
class CollectionModel;
class PlaylistBackend;
class PlaylistManager;
#ifndef Q_OS_WIN
class DeviceManager;
#endif
class CoverProviders;
class AlbumCoverLoader;
class CurrentAlbumCoverLoader;
class CoverProviders;
class LyricsProviders;
class AudioScrobbler;
class LastFMImport;
class InternetServices;
class RadioServices;
#ifdef HAVE_MOODBAR
class MoodbarController;
class MoodbarLoader;
#endif

class NetworkRemote;

class Application : public QObject {
  Q_OBJECT

 public:
  explicit Application(QObject *parent = nullptr);
  ~Application() override;

  SharedPtr<TagReaderClient> tag_reader_client() const;
  SharedPtr<Database> database() const;
  SharedPtr<TaskManager> task_manager() const;
  SharedPtr<Player> player() const;
  SharedPtr<NetworkAccessManager> network() const;
  SharedPtr<DeviceFinders> device_finders() const;
#ifndef Q_OS_WIN
  SharedPtr<DeviceManager> device_manager() const;
#endif

  SharedPtr<SCollection> collection() const;
  SharedPtr<CollectionBackend> collection_backend() const;
  CollectionModel *collection_model() const;

  SharedPtr<PlaylistBackend> playlist_backend() const;
  SharedPtr<PlaylistManager> playlist_manager() const;

  SharedPtr<CoverProviders> cover_providers() const;
  SharedPtr<AlbumCoverLoader> album_cover_loader() const;
  SharedPtr<CurrentAlbumCoverLoader> current_albumcover_loader() const;

  SharedPtr<LyricsProviders> lyrics_providers() const;

  SharedPtr<AudioScrobbler> scrobbler() const;

  SharedPtr<InternetServices> internet_services() const;
  SharedPtr<RadioServices> radio_services() const;

#ifdef HAVE_MOODBAR
  SharedPtr<MoodbarController> moodbar_controller() const;
  SharedPtr<MoodbarLoader> moodbar_loader() const;
#endif

  SharedPtr<LastFMImport> lastfm_import() const;

  SharedPtr<NetworkRemote> network_remote() const;

  void Exit();

  QThread *MoveToNewThread(QObject *object);
  static void MoveToThread(QObject *object, QThread *thread);

 private slots:
  void ExitReceived();

 public slots:
  void AddError(const QString &message);
  void ReloadSettings();
  void OpenSettingsDialogAtPage(SettingsDialog::Page page);

 signals:
  void ErrorAdded(const QString &message);
  void SettingsChanged();
  void SettingsDialogRequested(const SettingsDialog::Page page);
  void ExitFinished();
  void ClearPixmapDiskCache();

 private:
  ScopedPtr<ApplicationImpl> p_;
  QList<QThread*> threads_;
  QList<QObject*> wait_for_exit_;

};

#endif  // APPLICATION_H
