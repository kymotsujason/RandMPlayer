#pragma once

#include <QtWidgets/QMainWindow>
#include <qmediaplayer.h>
#include "ui_mainwindow.h"

class mainwindow : public QMainWindow
{
	Q_OBJECT

public:
	mainwindow(QWidget *parent = Q_NULLPTR);
	~mainwindow();

protected:
	/*void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);*/

private slots:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void on_actionExit_triggered();
	void on_actionAbout_triggered();
	void on_actionLoop_toggled(bool toggle);
	void on_actionAutoPlay_toggled(bool toggle);
	void on_playButton_clicked();
	void on_stopButton_clicked();
	void on_volumeButton_clicked();
	void on_musicList_itemClicked(QListWidgetItem* item);
	void on_musicPos_sliderPressed();
	void on_musicPos_sliderMoved(int i);
	void on_musicPos_sliderReleased();

private:
	QWidget *volumeBar;
	QSlider *slider = nullptr;
	Ui::mainwindowClass ui;
	bool canHandleDrop(const QDropEvent *event);
	QStringList supportedMimeTypes();
	void scanFolder();
	QMediaPlayer *mPlayer = nullptr;
	void volumeChanged();
	void updatePosition();
	void updateDuration();
	int currentSong;
	QStringList list;
	QString formatTime(qint64 timeMilliSeconds);
};
