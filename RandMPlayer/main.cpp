#include "stdafx.h"
#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainwindow w;
	w.setAttribute(Qt::WA_TranslucentBackground);
	w.show();
	return a.exec();
}
