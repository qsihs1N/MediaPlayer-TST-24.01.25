#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<qmediaplaylist.h>
#include<QDebug>
#include<QFileDialog>
#include<QStringList>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QVideoWidget>
#include <QMainWindow>
#include <QUrl>
#include <QMediaContent>
#include<QPaintEvent>
#include<QListWidgetItem>
#include<QTimer>
#include<QMouseEvent>
#include<QSettings>
#include<QLabel>
#include<QTime>
#include <QPixmap>
#include <QtGlobal>
#include<QComboBox>
#include<QMessageBox>
#include<QThread>
#include <QVideoProbe>
#include<QImage>
#include<QPainter>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonDocument>
#include <QMimeData>
#include <QScreen>
#include <QGuiApplication>
#include<QMediaRecorder>
#include <QMediaObject>
#include<QVariantMap>
#include <QMediaMetaData>
#include <QProcess>
#include<QWidget>
#include<QApplication>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_open_clicked();

    void on_play_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_list_itemDoubleClicked(QListWidgetItem *item);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_grab_clicked();

    void on_save_clicked();

    void on_load_clicked();

    void on_list_customContextMenuRequested(const QPoint &pos);

    void hideUI();

    void showUI();

    void updateUIVisibility();

    void on_comboBox_3_currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event) override;


private:
    Ui::MainWindow *ui;
    QMediaPlayer *pPlayer;
    QMediaPlaylist *pPlaylist;
    QVideoWidget *pVideowidget;
    int currentPlayingIndex;
    bool isPlay;
    bool isOpen;
    void updateSlider();
    void on_sliderMoved(int position);
    void on_durationChanged(qint64 duration);
    void onSliderPressed();
    void on_volumeSlider_valueChanged(int value);
    QLabel *label_all;
    QLabel *label_now;
    qreal playbackSpeed;
    void removeSelectedItems();
    bool isMediaFileNameSelected(const QString &mediaFileName);
    void onNextVideo();
    void on_mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void setupSliderStylesheet();
    void setupVolumeSliderStylesheet();
    QList<int> playedIndexes;
    void updateCurrentFileName();
    void updatePlaylistStyle();
    void moveItemDown(QListWidgetItem *item);
    void moveItemUp(QListWidgetItem *item);
    void setupPushbuttonStylesheet1();
    void setupComboBoxStylesheet1();
    void setMainWinStylesheet1();
    void setupPushbuttonStylesheet2();
    void setupComboBoxStylesheet2();
    void setMainWinStylesheet2();
    void setupListStylesheet1();
    void setupListStylesheet2();
    void addMediaToList(const QString &fileName);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    bool isValidMediaFile(const QString &fileExtension);
    void showInvalidMediaFileError(const QString &fileName);
    bool uiVisible;
    void hideUIcounter();
    bool uicondition;
    void resizeEvent(QResizeEvent *event);
    void showTipDialog();
    bool isMuted;
    void toggleMute();
    void savePlaylistToFile(const QString &fileName);
    QJsonArray createMediaArrayPlaylist();
    void loadMediaUrlsFromJsonArray(const QJsonArray &mediaArray, QMediaPlaylist *pPlaylist, QListWidget *listWidget);
    bool hasValidPlaylist(const QJsonObject &playlistObject);
    QJsonObject getJsonObject(const QJsonDocument &jsonDoc);
    bool isJsonObject(const QJsonDocument &jsonDoc);
    QJsonDocument parseJsonData(const QByteArray &data, QString &errorMessage);
    QByteArray readFileContents(const QString &fileName);
    bool isFileExtensionValid(const QString &fileName);
    bool doesFileExist(const QString &fileName);
    QString getPlaylistFileName(QWidget *parent);
    void stopCurrentVideo();
    void handleRandomPlayback();
    void handleSequentialPlayback();
    void playNewVideo();
    void on_label_doubleClicked();

};

#endif // MAINWINDOW_H
