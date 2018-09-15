#pragma once

#include <QDialog>
#include "ui_about.h"

class about : public QDialog
{
	Q_OBJECT

public:
	about(QWidget *parent = Q_NULLPTR);
	~about();

private:
	Ui::about ui;
	about *mAbout;
};
