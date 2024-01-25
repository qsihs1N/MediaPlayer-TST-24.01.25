#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("我的播放器");
    setWindowIcon(QIcon(":/new/prefix1/a"));
    //初始说明是否展示的选择记录保存在配置文件中
    QSettings settings1("MyMediaPlayerSettingschoice", "MediaPlayer");
    if (settings1.value("showTipDialog", true).toBool())
    {
        showTipDialog();
    }

    // 从配置文件中读取上次保存的音量设置，如果没有保存过，则使用默认值
    QSettings settings("MyMediaPlayerSettings", QSettings::IniFormat);
    int savedVolume = settings.value("volume", 50).toInt();

    //播放器对象 播放队列对象 显示视频对象
    pPlayer = new QMediaPlayer;
    pPlaylist = new QMediaPlaylist;
    pVideowidget = new QVideoWidget(ui->label);
   //设置播放队列
    pPlayer->setPlaylist(pPlaylist);
    //设置播放显示窗口
    pPlayer->setVideoOutput(pVideowidget);
    //设置配置文件中的音量
    pPlayer->setVolume(savedVolume);
    //使音量滑块保存为先前的状态
    ui->horizontalSlider_2->setValue(savedVolume);

    //记录一些状态
    isOpen=false;
    isPlay=false;

    //使进度条速度与视频播放速度等比
    connect(pPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    QTimer *timer = new QTimer(this);

    //进度条的位置更新速度与timer连接
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSlider);
    timer->start(25);

    //关联进度条与视频
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::on_sliderMoved);
    connect(ui->horizontalSlider, &QSlider::sliderPressed, this, &MainWindow::onSliderPressed);

    // 连接音量调节滑块的 valueChanged 信号与槽函数
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &MainWindow::on_volumeSlider_valueChanged);

    //连接del按钮与删除函数
    connect(ui->del, &QPushButton::clicked, this, &MainWindow::removeSelectedItems);

    //获取指针
    label_all=ui->label_all;
    label_now=ui->label_now;
    playbackSpeed=1.0;
    ui->centralwidget->installEventFilter(this);

    //添加事件过滤器
    ui->comboBox->setFocusPolicy(Qt::NoFocus);
    ui->list->setFocusPolicy(Qt::NoFocus);
    ui->comboBox_2->setFocusPolicy(Qt::NoFocus);
    ui->comboBox_3->setFocusPolicy(Qt::NoFocus);

    connect(pPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::on_mediaStatusChanged);
    pPlaylist->clear();

    //初始化样式
    setupSliderStylesheet();
    setupVolumeSliderStylesheet();
    setupListStylesheet1();
    setupPushbuttonStylesheet1();
    setupComboBoxStylesheet1();
    setMainWinStylesheet1();

    ui->stop->setText("停止");
    ui->pause->setText("暂停");
    ui->play->setText("播放");
    ui->open->setText("打开");
    ui->grab->setText("截图");
    ui->save->setText("保存");
    ui->del->setText("删除");
    ui->load->setText("加载");
    ui->comboBox->addItem("倍速");
    ui->comboBox->addItem("0.5x");
    ui->comboBox->addItem("0.75x");
    ui->comboBox->addItem("1x");
    ui->comboBox->addItem("1.5x");
    ui->comboBox->addItem("2x");
    ui->comboBox->addItem("3x");
    ui->comboBox->addItem("4x");
    ui->comboBox_2->addItem("顺序", QVariant(QMediaPlaylist::Sequential));
    ui->comboBox_2->addItem("随机", QVariant(QMediaPlaylist::Random));
    ui->comboBox_2->addItem("循环", QVariant(QMediaPlaylist::Loop));
    ui->play->setToolTip("播放 (S)");
    ui->pause->setToolTip("暂停 (S)");
    ui->horizontalSlider->setToolTip("回退/快进 (A/D)");
    ui->horizontalSlider_2->setToolTip("增大/减小/静音 (Q/E/X)");
    ui->del->setToolTip("删除 (DEL)");
    ui->comboBox_3->addItem("反显");
    ui->comboBox_3->addItem("正显");

    // 连接ComboBox的currentIndexChanged信号到槽函数
    connect(ui->comboBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBox_2_currentIndexChanged);
    // 正在播放的索引初始化为无效值
    currentPlayingIndex = -1;
    //启用右键菜单信号
    ui->list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->list, &QListWidget::customContextMenuRequested, this, &MainWindow::on_list_customContextMenuRequested);
    // 启用拖放操作
    setAcceptDrops(true);
    //全局事件过滤器，捕获键盘事件
    qApp->installEventFilter(this);
    //label事件过滤器，捕获鼠标事件
    ui->label->installEventFilter(this);
    ui->label->setMouseTracking(true);

    // 设置ui初始状态
    uiVisible = true;
    uicondition=true;
    updateUIVisibility();
    //label大小策略全为expanding
    ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(ui->comboBox_3, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_3_currentIndexChanged);
    //静音初始化
    isMuted=false;
}

MainWindow::~MainWindow()
{   delete pPlayer;
    delete pPlaylist;
    delete pVideowidget;
    delete ui;

}

void MainWindow::showTipDialog()
{
    // 创建提示框
    QMessageBox tipDialog;
    tipDialog.setWindowTitle("欢迎使用播放器");
    tipDialog.setText("这是一个简单的播放器，您可以在这里播放您的音乐和视频。\n\n"
                      "使用方法：\n"
                      "- 打开文件：点击“打开”按钮或者使用菜单中的“文件”->“打开”选项。\n"
                      "- 播放/暂停：点击视频窗口下方的播放/暂停按钮或者使用S或鼠标双击进行播放/暂停。\n"
                      "- 快进/回退：拖拽进度条或点击进度条的上下方或使用A/D进行快进/回退。\n"
                      "- 音量调整：拖拽音量条或使用Q/E进行音量调整或使用X键进行静音控制。\n"
                      "- 倍速选择：点击倍速可以选择各种倍速。\n"
                      "- 保存列表：当播放列表存在项目可以点击保存按钮进行列表保存。\n"
                      "- 加载列表：点击加载可以加载已保存的播放列表。\n"
                      "- 整理列表：可以通过鼠标右键以及删除按钮来整理列表。\n"
                      "- 播放模式：顺序播放、随机播放和循环播放三种播放模式。\n"
                      "- ui界面：有反显和正显两种显示模式。\n"
                      "- 截图功能：点击播放列表下方的截图可以保存当前的视频帧。\n"
                      "- 全屏切换：使用F键及Z键进行全屏切换。\n\n"
                      "希望您有一个愉快的体验！");

    // 添加按钮：关闭和永不提示
    tipDialog.addButton("永不提示", QMessageBox::AcceptRole);
    tipDialog.addButton("关闭", QMessageBox::RejectRole);

    // 显示提示框并等待用户选择
    int result = tipDialog.exec();

    // 处理用户选择
    if (result == QMessageBox::AcceptRole)
    {
        // 用户选择“永不提示”，保存设置
        QSettings settings1("MyMediaPlayerSettingschoice", "MediaPlayer");
        settings1.setValue("showTipDialog", false);
        QString configFile = settings1.fileName();
        qDebug() << "Config File Path: " << configFile;
    }
}

//删除功能的实现
void MainWindow::removeSelectedItems()
{
    QList<QListWidgetItem *> selectedItems = ui->list->selectedItems();

    // 检查是否有选中的项目
    if (selectedItems.isEmpty())
    {
        QMessageBox::information(this, "移除选中项", "没有选中的项！");
        return;
    }

    QMediaContent mediaContent = pPlayer->media();

    // 获取正在播放的媒体的 URL
    QUrl mediaUrl = mediaContent.canonicalUrl();

    // 输出媒体的文件名
    QString mediaFileName = QFileInfo(mediaUrl.path()).fileName();

    // 检查是否有正在播放的项目
    bool isPlayingMediaSelected = isMediaFileNameSelected(mediaFileName) && pPlayer->state() == QMediaPlayer::PlayingState;

    // 遍历选中的项并移除
    for (QListWidgetItem *item : selectedItems)
    {
        int row = ui->list->row(item);

        // 如果删除的项目正在播放，停止播放
        if (isPlayingMediaSelected)
        {
            pPlayer->stop();
            isPlay = false;
        }

        // 移除索引
        if (playedIndexes.contains(row))
        {
            playedIndexes.removeOne(row);
        }

        // 从播放列表和列表视图中移除项
        ui->list->takeItem(row);
        pPlaylist->removeMedia(row);
        delete item;
    }

    // 更新播放列表样式
    updatePlaylistStyle();

    // 显示提示信息
    QMessageBox::information(this, "移除选中项", "选中项移除成功！");
}

//判断选中文件是否在list中
bool MainWindow::isMediaFileNameSelected(const QString &mediaFileName)
{
    // 获取当前选中的所有项
    QList<QListWidgetItem *> selectedItems = ui->list->selectedItems();

    // 遍历选中的项并比较文件名
    for (QListWidgetItem *item : selectedItems)
    {
        // 获取列表项中显示的文件名
        QString fileName = QFileInfo(item->text()).fileName();

        // 检查是否与传入的媒体文件名匹配
        if (fileName == mediaFileName)
        {
            return true;
        }
    }

    // 如果没有匹配项，返回 false
    return false;
}

//重绘窗口保证大小相符
void MainWindow:: paintEvent(QPaintEvent *e)
{
    if(isOpen)
    {
        pVideowidget->resize(ui->label->size());
    }
}

void MainWindow::on_open_clicked()
{
    // 停止当前播放
    pPlayer->stop();

    // 启用水平滑块
    ui->horizontalSlider->setEnabled(true);

    // 弹出文件打开对话框，允许选择多个文件
    QStringList filenames = QFileDialog::getOpenFileNames(this, "选择文件", "C:/Users",
                                                          "所有文件(*.*);;"
                                                          "MP3文件(*.mp3);;"
                                                          "MP4文件(*.mp4);;"
                                                          "AVI文件(*.avi);;"
                                                          "MPV文件(*.mpv)");

    // 遍历选择的文件列表
    foreach (QString const &filename, filenames)
    {
        // 将文件名转换为URL
        QUrl url(filename);

        // 获取文件扩展名
        QString fileExtension = QFileInfo(filename).suffix().toLower();

        // 检查文件类型
        if (fileExtension == "mp3" || fileExtension == "mp4" || fileExtension == "avi")
        {
            // 将文件添加到播放列表
            pPlaylist->addMedia(url);

            // 在UI列表中添加文件名
            ui->list->addItem(filename);
        }
        else
        {
            // 如果不是支持的影音格式，显示错误消息
            QMessageBox::warning(this, "错误", "不是影音格式，无法读取：" + filename);
        }
    }

    // 已经打开文件
    isOpen = true;

    // 设置播放速度
    pPlayer->setPlaybackRate(playbackSpeed);

    // 启用删除按钮
    ui->del->setEnabled(true);
}

// 播放按钮点击事件
void MainWindow::on_play_clicked()
{
    // 检查播放列表是否为空
    if (pPlaylist->isEmpty())
    {
        QMessageBox::warning(this, "警告", "播放列表为空，无法播放/暂停。");
        return;
    }

    pPlayer->play();
    isPlay = true;
    pVideowidget->resize(ui->label->size());
    currentPlayingIndex = pPlaylist->currentIndex();

    // 更新播放列表
    updatePlaylistStyle();
}

// 暂停按钮点击事件
void MainWindow::on_pause_clicked()
{
    // 检查播放列表是否为空
    if (pPlaylist->isEmpty())
    {
        QMessageBox::warning(this, "警告", "播放列表为空，无法播放/暂停。");
        return;
    }

    pPlayer->pause();
    isPlay = false;
}

// 停止按钮点击事件
void MainWindow::on_stop_clicked()
{
    // 检查播放列表是否为空
    if (pPlaylist->isEmpty())
    {
        QMessageBox::warning(this, "警告", "播放列表为空，无法停止。");
        return;
    }

    pPlayer->stop();
    isPlay = false;
}

//双击播放列表项目播放
void MainWindow::on_list_itemDoubleClicked(QListWidgetItem *item)
{
    pPlayer->stop();
    int row = ui->list->row(item);
    pPlaylist->setCurrentIndex(row);
    pPlayer->play();
    isPlay = true;
    // 更新当前正在播放的索引
    currentPlayingIndex = row;

    // 更新播放列表的样式
    updatePlaylistStyle();
}

// 停止当前视频的播放
void MainWindow::stopCurrentVideo()
{
    pPlayer->stop();
    isPlay = false;
}

// 处理随机播放模式
void MainWindow::handleRandomPlayback()
{
    // 如果所有视频都已播放过，则清空记录的已播放索引，停止播放并返回
    if (playedIndexes.size() == pPlaylist->mediaCount())
    {
        playedIndexes.clear();
        pPlayer->stop();
        return;
    }

    // 随机选择一个未播放过的视频进行播放
    while (true)
    {
        int randomIndex = qrand() % pPlaylist->mediaCount();
        if (!playedIndexes.contains(randomIndex))
        {
            playedIndexes.append(randomIndex);
            pPlaylist->setCurrentIndex(randomIndex);
            playNewVideo();
            break;
        }
    }
}

// 处理顺序播放模式
void MainWindow::handleSequentialPlayback()
{
    // 如果当前视频不是最后一个，则播放下一个视频
    if (currentPlayingIndex < pPlaylist->mediaCount() - 1)
    {
        pPlaylist->next();
        playNewVideo();
    } else
    {
        // 如果是最后一个视频，则停止播放
        pPlayer->stop();
        isPlay = false;
        return;
    }
}

// 播放新视频
void MainWindow::playNewVideo()
{
    // 开始播放，并更新播放状态和当前播放索引
    pPlayer->play();
    isPlay = true;
    currentPlayingIndex = pPlaylist->currentIndex();
    updatePlaylistStyle();
}

// 下一个视频的槽函数
void MainWindow::onNextVideo()
{
    // 停止当前视频的播放
    stopCurrentVideo();

    // 获取当前播放模式和当前播放索引
    int currentIndex = pPlaylist->currentIndex();
    QMediaPlaylist::PlaybackMode playbackMode =static_cast<QMediaPlaylist::PlaybackMode>(ui->comboBox_2->currentData().toInt());

    // 根据播放模式执行相应的操作
    if (playbackMode == QMediaPlaylist::Random)
    {
        handleRandomPlayback();
    } else if (playbackMode == QMediaPlaylist::Loop)
    {
        // 在循环模式下，重置当前视频的播放位置并播放
        pPlayer->setPosition(0);
        playNewVideo();
        return;
    } else
    {
        handleSequentialPlayback();
    }
}

void MainWindow::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        // 当前媒体播放结束，切换到下一个视频
        onNextVideo();

    }
}

void MainWindow::on_durationChanged(qint64 duration)
{
    ui->horizontalSlider->setRange(0, duration);
    // 如果当前位置已经到达了视频的总时长，切换到下一个视频

}

void MainWindow::updateSlider()
{
    // 获取当前播放位置
    qint64 currentPosition = pPlayer->position();

    // 确保当前播放位置在有效范围内
    if (currentPosition >= 0 && currentPosition <= ui->horizontalSlider->maximum())
    {
        // 设置滑块位置
        ui->horizontalSlider->setValue(currentPosition);

        // 更新总时长标签
        qint64 totalDuration = pPlayer->duration();
        QString totalDurationStr = QTime(0, 0).addMSecs(totalDuration).toString("mm:ss");
        label_all->setText(totalDurationStr);

        // 更新当前播放时长标签
        qint64 currentTime = currentPosition;

        // 将毫秒转化为分钟
        QString currentTimeStr = QTime(0, 0).addMSecs(currentTime).toString("mm:ss");
        label_now->setText(currentTimeStr);
    }
}

void MainWindow::on_sliderMoved(int position)
{
    pPlayer->setPosition(position);
}

//点击滑块的事件
void MainWindow::onSliderPressed()
{
    // 获取鼠标相对于滑块的位置
    QPoint globalMousePos = QCursor::pos();
    QPoint sliderTopLeft = ui->horizontalSlider->mapToGlobal(ui->horizontalSlider->geometry().topLeft());
    int mouseX = globalMousePos.x() - sliderTopLeft.x();

    // 计算滑块的值并设置滑块位置
    int sliderValue = ui->horizontalSlider->minimum() +
                      mouseX * (ui->horizontalSlider->maximum() - ui->horizontalSlider->minimum()) /
                          ui->horizontalSlider->width();
    ui->horizontalSlider->setValue(sliderValue);

    // 暂停播放
    pPlayer->pause();
    isPlay = false;
}

//点击进度条的事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int mouseY = qAbs(event->pos().y() - ui->horizontalSlider->geometry().topRight().y());

        if (mouseY <= 30)
        {

            // 获取鼠标点击的位置（相对于进度条）
            int mouseX = event->pos().x() - ui->horizontalSlider->geometry().left();

            // 将鼠标点击位置转换为进度条的值
            int sliderValue = ui->horizontalSlider->minimum() +
                              mouseX * (ui->horizontalSlider->maximum() - ui->horizontalSlider->minimum()) /
                                  ui->horizontalSlider->width();

            // 设置播放器位置并暂停
            pPlayer->setPosition(sliderValue);
            pPlayer->pause();
            isPlay=false;
        }
    }
}

//音量的实现
void MainWindow::on_volumeSlider_valueChanged(int value)
{
    // 将滑块的值设置为音量
    pPlayer->setVolume(value);
    // 保存音量设置到配置文件
    QSettings settings("MyMediaPlayerSettings", QSettings::IniFormat);
    settings.setValue("volume", value);
}

//一些快捷键adsqe
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->centralwidget)
    {
        // 处理键盘事件
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            // 处理左右方向键事件
            if (keyEvent->key() == Qt::Key_A)
            {
                if (pPlaylist->isEmpty()) {
                    QMessageBox::warning(this, "警告", "播放列表为空，无法快进/回退。");
                }
                // 往后退 0.5 秒
                qint64 newPosition = pPlayer->position() - 500;
                if (newPosition < 0)
                {
                    newPosition = 0;
                }
                pPlayer->setPosition(newPosition);
                return true;
            }
            if (keyEvent->key() == Qt::Key_D)
            {
                if (pPlaylist->isEmpty())
                {
                    QMessageBox::warning(this, "警告", "播放列表为空，无法快进/回退。");
                }
                // 往前进 0.5 秒
                qint64 newPosition = pPlayer->position() + 500;
                qint64 duration = pPlayer->duration();
                if (newPosition > duration)
                {
                    newPosition = duration;
                }
                pPlayer->setPosition(newPosition);
                return true;
            }
            if (keyEvent->key() == Qt::Key_S)
            {
                if (pPlaylist->isEmpty())
                {
                    QMessageBox::warning(this, "警告", "播放列表为空，无法播放/暂停。");
                }
                // 执行播放/暂停操作
                else if (pPlayer->state() == QMediaPlayer::PlayingState)
                {
                    pPlayer->pause();
                } else
                {
                    pPlayer->play();
                    currentPlayingIndex = pPlaylist->currentIndex();

                    // 更新播放列表的样式
                    updatePlaylistStyle();
                }
                return true;
            }
            if(keyEvent->key() == Qt::Key_X)
            {
                toggleMute();
                return true;
            }
            if(keyEvent->key() == Qt::Key_E)
            {
                ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value() + 10);
                return true;
            }
            if(keyEvent->key() == Qt::Key_Q)
            {
                ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value() - 10);
                return true;
            }
            if(keyEvent->key() == Qt::Key_F)
            {

                hideUIcounter();
                uicondition=false;
                return true;

                }
            if(keyEvent->key() == Qt::Key_Z)
            {

                showUI();
                uicondition=true;
                return true;

            }
            if(keyEvent->key() == Qt::Key_Delete)
            {

                removeSelectedItems();
                return true;

            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

//倍速的实现
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qreal speeds[] = {1.0,0.5,0.75,1.0, 1.5, 2.0, 3.0, 4.0};
    playbackSpeed=speeds[index];
    pPlayer->setPlaybackRate(playbackSpeed);

}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    // 获取用户选择的播放模式
    QMediaPlaylist::PlaybackMode selectedMode = static_cast<QMediaPlaylist::PlaybackMode>(
        ui->comboBox_2->currentData().toInt());

    // 将播放模式应用到播放列表
    pPlaylist->setPlaybackMode(selectedMode);
}

//突出列表中正在播放的视频名称
void MainWindow::updatePlaylistStyle()
{
    // 清除所有项的样式
    for (int i = 0; i < ui->list->count(); ++i)
    {
        QListWidgetItem *item = ui->list->item(i);
        if (item)
        {
            QFont font = item->font();
            font.setBold(false);  // 恢复正常字体
            item->setFont(font);
            item->setBackground(QBrush(Qt::white));  // 恢复正常背景颜色
        }
    }

    // 获取当前正在播放的索引
    int currentPlayingIndex = pPlaylist->currentIndex();

    // 如果有正在播放的索引，突出显示它
    if (currentPlayingIndex != -1)
    {
        QListWidgetItem *playingItem = ui->list->item(currentPlayingIndex);
        if (playingItem)
        {
            QFont font = playingItem->font();
            font.setBold(true);  // 设置为粗体字体
            playingItem->setFont(font);
            playingItem->setBackground(QBrush(Qt::white));  //设置背景颜色

            // 更新文件名显示
            QString fileName = pPlaylist->media(currentPlayingIndex).canonicalUrl().fileName();
            playingItem->setText(fileName);
        }
    }
}

void MainWindow::setupVolumeSliderStylesheet()
{
    QString styleSheet = "\
        QSlider::groove:horizontal {\
            height: 5px; \
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #BEBEBE, stop:1 #BEBEBE);\
            border: 1px solid #4A708B; \
            border-radius: 2px; \
    } \
        QSlider::handle:horizontal {\
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #45ADED, stop:1 #04558A); \
            border: 1px solid #4A708B; \
            width: 10px; \
            margin: -2px 0; \
            border-radius: 5px; \
    } \
        QSlider::add-page:horizontal {\
            background: #BEBEBE; \
            height: 5px; \
            border-radius: 2px; \
    }";

        ui->horizontalSlider_2->setStyleSheet(styleSheet);
}

void MainWindow::on_grab_clicked()
{
    // 检查播放器状态
    if (pPlayer->state() == QMediaPlayer::StoppedState)
    {
        QMessageBox::information(this, "截图", "请在播放视频时截图！");
        return;
    }

    // 如果当前是播放状态，先暂停视频
    pPlayer->pause();

    // 获取当前主窗口的屏幕
    QScreen *screen = QGuiApplication::primaryScreen();

    // 检查屏幕截图是否成功
    if (!screen)
    {
        QMessageBox::warning(this, "截图", "无法获取屏幕信息！");
        return;
    }

    // 截取主窗口的内容
    QPixmap snapshot = screen->grabWindow(this->winId());

    // 检查截图是否为空
    if (snapshot.isNull())
    {
        QMessageBox::warning(this, "截图", "截图为空！");
        return;
    }

    // 生成保存文件路径
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("Images (*.png)");
    fileDialog.setDefaultSuffix("png");

    if (fileDialog.exec())
    {
        QStringList fileNames = fileDialog.selectedFiles();
        QString fileName = fileNames.first();

        if (!fileName.isEmpty())
        {
            // 保存截图为图像文件
            if (snapshot.save(fileName, "PNG"))
            {
                QMessageBox::information(this, "截图", "截图保存成功！");
            } else
            {
                QMessageBox::warning(this, "截图", "保存截图失败！");
            }
        }
    }

    // 最后继续播放视频
    pPlayer->play();
}

void MainWindow::savePlaylistToFile(const QString &fileName)
{
    QJsonObject playlistObject;

    // 将每个媒体文件的 URL 存储到 JSON 数组中
    QJsonArray mediaArray = createMediaArrayPlaylist();
    playlistObject["playlist"] = mediaArray;

    // 将 JSON 对象保存到文件中
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument jsonDoc(playlistObject);
        file.write(jsonDoc.toJson());
        file.close();
        QMessageBox::information(this, "保存播放列表", "播放列表保存成功！");
    } else
    {
        QMessageBox::warning(this, "保存播放列表", "无法保存播放列表！");
    }
}

// 创建包含播放列表媒体文件的 JSON 数组
QJsonArray MainWindow::createMediaArrayPlaylist()
{
    QJsonArray mediaArray;
    for (int i = 0; i < pPlaylist->mediaCount(); ++i)
    {
        QMediaContent mediaContent = pPlaylist->media(i);
        QUrl mediaUrl = mediaContent.canonicalUrl();
        mediaArray.append(mediaUrl.toString());
    }
    return mediaArray;
}

// 响应“保存”按钮的点击事件
void MainWindow::on_save_clicked()
{
    // 检查播放列表是否为空
    if (pPlaylist->isEmpty())
    {
        QMessageBox::information(this, "保存播放列表", "播放列表为空，无法保存！");
        return;
    }

    // 弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, "保存播放列表", QDir::homePath(), "Playlist (*.json)");

    if (!fileName.isEmpty())
    {
        savePlaylistToFile(fileName);
    }
}

//用于打开文件对话框和获取文件名
QString MainWindow::getPlaylistFileName(QWidget *parent)
{
    QFileDialog fileDialog(parent);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Playlist (*.json)");
    fileDialog.setDefaultSuffix("json");

    if (fileDialog.exec())
    {
        QStringList fileNames = fileDialog.selectedFiles();
        return fileNames.first();
    }

    return QString(); // 返回空字符串表示未选择文件
}

// 用于检查文件是否存在
bool MainWindow::doesFileExist(const QString &fileName)
{
    return QFile::exists(fileName);
}

// 用于检查文件扩展名是否为 .json
bool MainWindow::isFileExtensionValid(const QString &fileName)
{
    return QFileInfo(fileName).suffix().toLower() == "json";
}

// 用于读取文件内容
QByteArray MainWindow::readFileContents(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = file.readAll();
        file.close();
        return data;
    }
    return QByteArray(); // 返回空 QByteArray 表示读取失败
}

// 用于解析 JSON 数据
QJsonDocument MainWindow::parseJsonData(const QByteArray &data, QString &errorMessage)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonError.error != QJsonParseError::NoError)
    {
        errorMessage = jsonError.errorString();
    }

    return jsonDoc;
}

// 用于检查 JSON 文档是否为对象
bool MainWindow::isJsonObject(const QJsonDocument &jsonDoc)
{
    return jsonDoc.isObject();
}

// 用于获取 JSON 对象
QJsonObject MainWindow::getJsonObject(const QJsonDocument &jsonDoc)
{
    return jsonDoc.object();
}

// 用于检查 JSON 对象是否包含有效的播放列表信息
bool MainWindow::hasValidPlaylist(const QJsonObject &playlistObject)
{
    return playlistObject.contains("playlist") && playlistObject["playlist"].isArray();
}

// 用于从 JSON 数组加载媒体文件的 URL 到播放列表和列表视图
void MainWindow::loadMediaUrlsFromJsonArray(const QJsonArray &mediaArray, QMediaPlaylist *pPlaylist, QListWidget *listWidget)
{
    pPlaylist->clear();
    listWidget->clear();

    for (const QJsonValue &value : mediaArray)
    {
        if (value.isString())
        {
            QUrl mediaUrl(value.toString());
            pPlaylist->addMedia(mediaUrl);
            listWidget->addItem(mediaUrl.fileName());
        }
    }
}

//负责整个加载播放列表的逻辑
void MainWindow::on_load_clicked()
{
    // 获取文件名
    QString fileName = getPlaylistFileName(this);

    // 检查是否选择了文件
    if (fileName.isEmpty())
    {
        return;
    }

    // 检查文件是否存在
    if (!doesFileExist(fileName))
    {
        QMessageBox::warning(this, "加载播放列表", "文件不存在：" + fileName);
        return;
    }

    // 检查文件扩展名是否为 .json
    if (!isFileExtensionValid(fileName))
    {
        QMessageBox::warning(this, "加载播放列表", "文件格式错误！请选择有效的 JSON 文件。");
        return;
    }

    // 读取文件内容
    QByteArray data = readFileContents(fileName);

    // 检查是否成功读取文件
    if (data.isEmpty())
    {
        QMessageBox::warning(this, "加载播放列表", "无法加载播放列表！");
        return;
    }

    // 解析 JSON 数据
    QString jsonError;
    QJsonDocument jsonDoc = parseJsonData(data, jsonError);

    // 检查是否成功解析 JSON 数据
    if (!jsonError.isEmpty())
    {
        QMessageBox::warning(this, "加载播放列表", "JSON 解析错误：" + jsonError);
        return;
    }

    // 检查 JSON 文档是否为对象
    if (!isJsonObject(jsonDoc))
    {
        QMessageBox::warning(this, "加载播放列表", "JSON 格式错误：根节点不是对象。");
        return;
    }

    // 获取 JSON 对象
    QJsonObject playlistObject = getJsonObject(jsonDoc);

    // 检查 JSON 对象是否包含有效的播放列表信息
    if (!hasValidPlaylist(playlistObject))
    {
        QMessageBox::warning(this, "加载播放列表", "JSON 格式错误：缺少有效的播放列表信息。");
        return;
    }

    // 从 JSON 数组加载媒体文件的 URL 到播放列表和列表视图
    QJsonArray mediaArray = playlistObject["playlist"].toArray();
    loadMediaUrlsFromJsonArray(mediaArray, pPlaylist, ui->list);

    QMessageBox::information(this, "加载播放列表", "播放列表加载成功！");
    updatePlaylistStyle();
}

//鼠标右键选择移动项目
void MainWindow::on_list_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->list->itemAt(pos);

    if (item)
    {
        QMenu contextMenu(this);

        QAction *moveUpAction = contextMenu.addAction("向上移动");
        QAction *moveDownAction = contextMenu.addAction("向下移动");

        connect(moveUpAction, &QAction::triggered, [this, item]()
        {
            moveItemUp(item);
        });

        connect(moveDownAction, &QAction::triggered, [this, item]()
        {
            moveItemDown(item);
        });

        contextMenu.exec(ui->list->mapToGlobal(pos));
        updatePlaylistStyle();
    }
}

// 向上移动列表项的槽函数
void MainWindow::moveItemUp(QListWidgetItem *item)
{
    // 获取当前项在列表中的行号
    int row = ui->list->row(item);

    // 如果不是第一项，则进行上移操作
    if (row > 0)
    {
        int currentIndex = pPlaylist->currentIndex();
        QMediaContent currentMedia = pPlaylist->currentMedia();

        // 取出上一项，并在当前位置插入
        QListWidgetItem *prevItem = ui->list->takeItem(row - 1);
        ui->list->insertItem(row, prevItem);
        ui->list->insertItem(row - 1, item);
        ui->list->setCurrentItem(item);

        // 在播放列表中移动媒体
        pPlaylist->moveMedia(row, row - 1);

        // 更新当前播放索引
        if (currentIndex == row)
        {
            pPlaylist->setCurrentIndex(row - 1);
            currentPlayingIndex = row - 1;
        } else if (currentIndex == row - 1)
        {
            pPlaylist->setCurrentIndex(row);
            currentPlayingIndex = row;
        } else
        {
            pPlaylist->setCurrentIndex(currentIndex);
        }
    }
}

// 向下移动列表项的槽函数
void MainWindow::moveItemDown(QListWidgetItem *item)
{
    // 获取当前项在列表中的行号
    int row = ui->list->row(item);

    // 如果不是最后一项，则进行下移操作
    if (row < ui->list->count() - 1)
    {
        int currentIndex = pPlaylist->currentIndex();
        QMediaContent currentMedia = pPlaylist->currentMedia();

        // 取出下一项，并在当前位置插入
        QListWidgetItem *nextItem = ui->list->takeItem(row + 1);
        ui->list->insertItem(row, nextItem);
        ui->list->insertItem(row + 1, item);
        ui->list->setCurrentItem(item);

        // 在播放列表中移动媒体
        pPlaylist->moveMedia(row, row + 1);

        // 更新当前播放索引
        if (currentIndex == row)
        {
            pPlaylist->setCurrentIndex(row + 1);
            currentPlayingIndex = row + 1;
        } else if (currentIndex == row + 1)
        {
            pPlaylist->setCurrentIndex(row);
            currentPlayingIndex = row;
        } else
        {
            pPlaylist->setCurrentIndex(currentIndex);
        }
    }
}

void MainWindow::setupPushbuttonStylesheet1()
{
    QString buttonStyle = "QPushButton {"
                          "    background-color: #666666;"
                          "    color: white;"
                          "}";


    // 应用样式表到按钮
    ui->stop->setStyleSheet(buttonStyle);
    ui->open->setStyleSheet(buttonStyle);
    ui->pause->setStyleSheet(buttonStyle);
    ui->play->setStyleSheet(buttonStyle);
    ui->del->setStyleSheet(buttonStyle);
    ui->save->setStyleSheet(buttonStyle);
    ui->load->setStyleSheet(buttonStyle);
    ui->grab->setStyleSheet(buttonStyle);
}

void MainWindow::setupPushbuttonStylesheet2()
{
    QString buttonStyle = "QPushButton {"
                          "    background-color: #FFFFFF;"
                          "    color: black;"

                          "}";


    // 应用样式表到按钮
    ui->stop->setStyleSheet(buttonStyle);
    ui->open->setStyleSheet(buttonStyle);
    ui->pause->setStyleSheet(buttonStyle);
    ui->play->setStyleSheet(buttonStyle);
    ui->del->setStyleSheet(buttonStyle);
    ui->save->setStyleSheet(buttonStyle);
    ui->load->setStyleSheet(buttonStyle);
    ui->grab->setStyleSheet(buttonStyle);
}

void MainWindow::setupComboBoxStylesheet1()
{
    // 设置 QComboBox 样式表
    QString comboBoxStyle = "QComboBox {"
                            "    background-color: #666666;"   // 背景颜色
                            "    color: white;"                // 字体颜色
                            "}";


    ui->comboBox->setStyleSheet(comboBoxStyle);
    ui->comboBox_2->setStyleSheet(comboBoxStyle);
    ui->comboBox_3->setStyleSheet(comboBoxStyle);
}

void MainWindow::setupComboBoxStylesheet2()
{
    // 设置 QComboBox 样式表
    QString comboBoxStyle = "QComboBox {"
                            "    background-color: #FFFFFF;"   // 背景颜色
                            "    color: black;"                // 字体颜色
                            "}";


    ui->comboBox->setStyleSheet(comboBoxStyle);
    ui->comboBox_2->setStyleSheet(comboBoxStyle);
    ui->comboBox_3->setStyleSheet(comboBoxStyle);
}

//设置滑块样式
void MainWindow::setupSliderStylesheet()
{
    QString styleSheet = "\
        QSlider::groove:horizontal {\
            height: 5px; \
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #BEBEBE, stop:1 #BEBEBE);\
            border: 1px solid #4A708B; \
            border-radius: 2px; \
    } \
        QSlider::handle:horizontal {\
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #45ADED, stop:1 #04558A); \
            border: 1px solid #4A708B; \
            width: 10px; \
            margin: -2px 0; \
            border-radius: 5px; \
    } \
        QSlider::add-page:horizontal {\
            background: #BEBEBE; \
            height: 5px; \
            border-radius: 2px; \
    }";

        ui->horizontalSlider->setStyleSheet(styleSheet);
}

void MainWindow::setMainWinStylesheet1()
{
    QString mainWinStyle = "MainWindow {"
                           "    background-color: #999999;"
                           "}";

    // 应用样式表到主窗口
    this->setStyleSheet(mainWinStyle);
}

void MainWindow::setMainWinStylesheet2()
{
    QString mainWinStyle = "MainWindow {"
                           "    background-color: #cccccc;"
                           "}";

    // 应用样式表到主窗口
    this->setStyleSheet(mainWinStyle);
}

void MainWindow::setupListStylesheet1()
{
    // 设置 QListWidget 样式表
    QString listStyle = "QListWidget {"
                        "    background-color: #666666;"  // 设置背景颜色
                        "}";

    // 设置 QListWidget 中项的样式表
    QString itemStyle = "QListWidget::item {"
                        "    background: transparent;"  // 设置项的背景为透明
                        "    color: #FFFFFF;"  // 设置项的字体颜色
                        "}";

    // 合并样式表
    QString combinedStyle = listStyle + itemStyle;

    // 应用样式表
    ui->list->setStyleSheet(combinedStyle);
}

void MainWindow::setupListStylesheet2()
{
    // 设置 QListWidget 样式表
    QString listStyle = "QListWidget {"
                        "    background-color: #FFFFFF;"  // 设置背景颜色
                        "}";

    // 设置 QListWidget 中项的样式表
    QString itemStyle = "QListWidget::item {"
                        "    background: transparent;"  // 设置项的背景为透明
                        "    color: #000000;"  // 设置项的字体颜色
                        "}";

    // 合并样式表
    QString combinedStyle = listStyle + itemStyle;

    // 应用样式表
    ui->list->setStyleSheet(combinedStyle);
}

// 处理拖放进入事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

// 处理拖放释放事件
void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        foreach (QUrl url, urlList)
        {
            QString fileName = url.toLocalFile();

            // 处理拖放的文件，将文件添加到播放列表
            addMediaToList(fileName);
        }
    }
}

// 添加媒体到播放列表
void MainWindow::addMediaToList(const QString &fileName)
{
    QUrl url(fileName);

    // 获取文件扩展名
    QString fileExtension = QFileInfo(fileName).suffix().toLower();

    // 检查文件类型
    if (isValidMediaFile(fileExtension))
    {
        // 添加媒体到播放列表
        pPlaylist->addMedia(url);

        // 将文件名添加到列表
        ui->list->addItem(fileName);

        // 调整视频小部件的大小
        pVideowidget->resize(ui->label->size());
    } else
    {
        // 显示错误消息
        showInvalidMediaFileError(fileName);
    }
}

bool MainWindow::isValidMediaFile(const QString &fileExtension)
{
    // 支持的媒体文件扩展名列表
    QStringList supportedExtensions = {"mp3", "mp4", "avi"};

    // 检查文件扩展名是否在支持的列表中
    return supportedExtensions.contains(fileExtension);
}

void MainWindow::showInvalidMediaFileError(const QString &fileName)
{
    // 显示错误对话框
    QMessageBox::warning(this, "错误", "不是支持的影音格式，无法读取：" + fileName);
}

void MainWindow::hideUI()
{
    uiVisible = false;
    updateUIVisibility();

    ui->label->setGeometry(ui->centralwidget->geometry());
    repaint();
}

void MainWindow::hideUIcounter()
{
    hideUI();
    QTimer::singleShot(0, this, &MainWindow::hideUI);
}

void MainWindow::showUI()
{
    uiVisible = true;
    updateUIVisibility();
}

void MainWindow::updateUIVisibility()
{
    ui->comboBox->setVisible(uiVisible);
    ui->horizontalSlider->setVisible(uiVisible);
    ui->label_list->setVisible(uiVisible);
    ui->horizontalSlider_2->setVisible(uiVisible);
    ui->widget->setVisible(uiVisible);
    ui->widget_2->setVisible(uiVisible);
    ui->widget_3->setVisible(uiVisible);
    ui->label_3->setVisible(uiVisible);
    ui->open->setVisible(uiVisible);
    ui->pause->setVisible(uiVisible);
    ui->play->setVisible(uiVisible);
    ui->stop->setVisible(uiVisible);
    ui->label_2->setVisible(uiVisible);
    ui->label_now->setVisible(uiVisible);
    ui->comboBox_3->setVisible(uiVisible);
    ui->comboBox_2->setVisible(uiVisible);
    ui->del->setVisible(uiVisible);
    ui->grab->setVisible(uiVisible);
    ui->load->setVisible(uiVisible);
    ui->save->setVisible(uiVisible);
    ui->menubar->setVisible(uiVisible);
    ui->statusbar->setVisible(uiVisible);
    ui->list->setVisible(uiVisible);
    repaint();
}

// 窗口大小变化事件处理函数
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // 如果 UI 条件为假，隐藏 UI
    if (uicondition == false)
    {
        hideUIcounter();
    }
}

// 设置显示模式的槽函数
void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    // 获取下拉框当前选择的文本
    QString mode = ui->comboBox_3->itemText(index);

    // 根据选择的模式设置不同的样式
    if (mode == "反显")
    {
        setupPushbuttonStylesheet1();
        setupComboBoxStylesheet1();
        setMainWinStylesheet1();
        setupListStylesheet1();
    } else if (mode == "正显")
    {
        setupPushbuttonStylesheet2();
        setupComboBoxStylesheet2();
        setMainWinStylesheet2();
        setupListStylesheet2();
    }
}

// 切换静音状态的槽函数
void MainWindow::toggleMute()
{
    // 切换静音状态
    isMuted = !isMuted;
    pPlayer->setMuted(isMuted);

    // 根据静音状态设置标签文本
    if (isMuted == true)
    {
        ui->label_3->setText("静音");
    } else
    {
        ui->label_3->setText("音量");
    }
}

//鼠标双击更改视频播放状态
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 检查是否双击了窗口
    if (event->button() == Qt::LeftButton && ui->label->geometry().contains(event->pos()))
    {
        // 执行双击窗口操作
        on_label_doubleClicked();
    }
    QMainWindow::mouseDoubleClickEvent(event);
}

void MainWindow::on_label_doubleClicked()
{
    // 检查播放列表是否为空
    if (pPlaylist->isEmpty())
    {
        QMessageBox::warning(this, "警告", "播放列表为空，无法播放/暂停。");
        return;
    }

    // 根据当前状态切换播放/暂停
    if (pPlayer->state() == QMediaPlayer::PlayingState)
    {
        pPlayer->pause();
        isPlay = false;
    } else
    {
        pPlayer->play();
        isPlay = true;
        currentPlayingIndex = pPlaylist->currentIndex();
        updatePlaylistStyle();
    }
}
