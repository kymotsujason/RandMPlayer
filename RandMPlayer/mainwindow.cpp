#include "stdafx.h"
#include "mainwindow.h"
#include "about.h"
#include "resource.h"
#include <QtMultimedia>

bool loopToggle = true;
bool autoPlayToggle = true;

mainwindow::mainwindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	mPlayer = new QMediaPlayer(this);	
	pause = QIcon("Resources/Pause.png");
	play = QIcon("Resources/Play.png");
	volumeBar = new QWidget(this);
	slider = new QSlider(Qt::Horizontal, volumeBar);
	slider->setRange(0, 100);
	slider->setValue(mPlayer->volume());
	volumeBar->setWindowFlags(Qt::FramelessWindowHint);
	volumeBar->hide();
	//ui.controlDock->hide();
	currentSong = 0;
	list.clear();
	connect(mPlayer, &QMediaPlayer::positionChanged, this, &mainwindow::updatePosition);
	connect(mPlayer, &QMediaPlayer::durationChanged, this, &mainwindow::updateDuration);
	connect(slider, &QSlider::valueChanged, this, &mainwindow::volumeChanged);
	scanFolder();
}

mainwindow::~mainwindow()
{
}

QStringList mainwindow::supportedMimeTypes()
{
	QStringList result = QMediaPlayer::supportedMimeTypes();
	if (result.isEmpty())
	{
		result.append(QStringLiteral("audio/mpeg"));
	}
	return result;
}

void mainwindow::volumeChanged()
{
	mPlayer->setVolume(slider->value());
}

void mainwindow::on_actionExit_triggered()
{
	this->close();
}

void mainwindow::on_actionAbout_triggered()
{
	about mAbout;
	mAbout.setModal(true);
	mAbout.exec();
}

void mainwindow::on_actionLoop_toggled(bool toggle)
{
	if (toggle)
	{
		loopToggle = true;
	}
	else 
	{
		loopToggle = false;
	}
}

void mainwindow::on_actionAutoPlay_toggled(bool toggle)
{
	if (toggle)
	{
		autoPlayToggle = true;
	}
	else
	{
		autoPlayToggle = false;
	}
}

bool mainwindow::canHandleDrop(const QDropEvent *event)
{
	const QList<QUrl> urls = event->mimeData()->urls();
	if (urls.size() != 1)
	{
		return false;
	}
	QMimeDatabase mimeDatabase;
	return mainwindow::supportedMimeTypes().contains(mimeDatabase.mimeTypeForUrl(urls.constFirst()).name());
}

void mainwindow::dragEnterEvent(QDragEnterEvent *event)
{
	event->setAccepted(canHandleDrop(event));
}

void mainwindow::dropEvent(QDropEvent *event)
{
	mainwindow::on_stopButton_clicked();
	event->accept();
	//playUrl(event->mimeData()->urls().constFirst());
	QUrl url = event->mimeData()->urls().constFirst();
	setWindowFilePath(url.toLocalFile());
	ui.playingTitle->setText(QDir::toNativeSeparators(url.toLocalFile()));
	mPlayer->setMedia(url);
	if (autoPlayToggle)
	{
		mPlayer->play();
		ui.playButton->setText("Pause");
		ui.playButton->setIcon(pause);
	}
}

void mainwindow::on_playButton_clicked()
{
	if (mPlayer->mediaStatus() != QMediaPlayer::NoMedia)
	{
		if (ui.playButton->text() == "Play")
		{
			mPlayer->play();
			ui.playButton->setText(QString("Pause"));
			ui.playButton->setIcon(pause);
		}
		else
		{
			mPlayer->pause();
			ui.playButton->setText(QString("Play"));
			ui.playButton->setIcon(play);
		}
	}
}

void mainwindow::on_stopButton_clicked()
{
	mPlayer->stop();
	ui.playButton->setText(QString("Play"));
	ui.playButton->setIcon(play);
}

void mainwindow::on_volumeButton_clicked()
{
	if (volumeBar->isVisible())
	{
		volumeBar->hide();
	}
	else
	{
		volumeBar->move(QPoint(ui.volumeButton->pos().x(), ui.volumeButton->pos().y() + 35));
		volumeBar->show();
	}
}

void mainwindow::on_musicList_itemClicked(QListWidgetItem *item)
{
	mPlayer->stop();
	ui.playButton->setText("Play");
	ui.playButton->setIcon(play);
	ui.playingTitle->setText(item->text());
	mPlayer->setMedia(QUrl(QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" + item->text()));
	if (autoPlayToggle)
	{
		mPlayer->play(); 
		ui.playButton->setText("Pause");
		ui.playButton->setIcon(pause);
	}
}

void mainwindow::on_musicPos_sliderPressed()
{
	mPlayer->pause();
	ui.playButton->setText(QString("Skipping..."));
}

void mainwindow::on_musicPos_sliderMoved(int i)
{
	mPlayer->setPosition(i);
	ui.songTime->setText(formatTime(i) + " / " + formatTime(mPlayer->duration()));
}

void mainwindow::on_musicPos_sliderReleased()
{
	if (mPlayer->position() == mPlayer->duration())
	{
		updatePosition();
	}
	else
	{
		mPlayer->play();
		ui.playButton->setText(QString("Pause"));
		ui.playButton->setIcon(pause);
	}
}

void mainwindow::scanFolder() 
{
	QStringList filters;
	filters << "*.3gp" << "*.aa" << "*.aac" << "*.aax" << "*.act" << "*.aiff" << "*.amr" << "*.ape" << "*.au" << "*.awb" <<
		"*.dct" << "*.dss" << "*.dvf" << "*.flac" << "*.gsm" << "*.iklax" << "*.ivs" << "*.m4a" << "*.m4b" << "*.m4p" <<
		"*.mmf" << "*.mp3" << "*.mpc" << "*.msv" << "*.nsf" << "*.ogg" << "*.oga" << "*.mogg" << "*.opus" << "*.ra" <<
		"*.rm" << "*.raw" << "*.sln" << "*.tta" << "*.vox" << "*.wav" << "*.wma" << "*.wv" << "*.webm" << "*.8svx";
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
	dir.setSorting(QDir::Name);
	dir.setNameFilters(filters);
	list = dir.entryList(QDir::Files);
	ui.musicList->addItems(list);
}

QString mainwindow::formatTime(qint64 timeMilliSeconds)
{
	qint64 seconds = timeMilliSeconds / 1000;
	const qint64 minutes = seconds / 60;
	seconds -= minutes * 60;
	return QStringLiteral("%1:%2")
		.arg(minutes, 2, 10, QLatin1Char('0'))
		.arg(seconds, 2, 10, QLatin1Char('0'));
}

void mainwindow::updatePosition()
{
	ui.musicPos->setValue(mPlayer->position());
	ui.songTime->setText(formatTime(mPlayer->position()) + " / " + formatTime(mPlayer->duration()));
	if (mPlayer->position() == mPlayer->duration() && mPlayer->mediaStatus() != QMediaPlayer::NoMedia && mPlayer->duration() != 0)
	{
		mPlayer->stop();
		ui.playButton->setText(QString("Play"));
		ui.playButton->setIcon(play);
		if (loopToggle)
		{
			auto it = std::find(std::begin(list), std::end(list), ui.playingTitle->text());
			if (it != std::end(list))
			{
				if ((std::distance(std::begin(list), it) + 1) > (list.size() - 1))
				{
					ui.playingTitle->setText(list.at(0));
					ui.musicList->item(0)->setSelected(true);
					mPlayer->setMedia(QUrl(QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" + list.at(0)));
					if (autoPlayToggle)
					{
						mPlayer->play();
					}
				}
				else
				{
					ui.playingTitle->setText(list.at(std::distance(std::begin(list), it) + 1));
					ui.musicList->item(std::distance(std::begin(list), it) + 1)->setSelected(true);
					mPlayer->setMedia(QUrl(QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" + list.at(std::distance(std::begin(list), it) + 1)));
					if (autoPlayToggle)
					{
						mPlayer->play();
					}
				}
				ui.playButton->setText(QString("Pause"));
				ui.playButton->setIcon(pause);
			}
		}
	}
}

void mainwindow::updateDuration()
{
	ui.musicPos->setRange(0, mPlayer->duration());
}

//void mainwindow::enterEvent(QEvent * event)
//{
//	ui.controlDock->show();
//}
//
//void mainwindow::leaveEvent(QEvent * event)
//{
//	ui.controlDock->hide();
//}