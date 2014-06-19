#include "MainWindow.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
#ifndef _DEBUG 
	QApplication::setLibraryPaths(QStringList(QCoreApplication::applicationDirPath()+"/plugins"));
#endif 
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	MainWindow w;
	w.show();
	qDebug()<<QImageReader::supportedImageFormats();
	return a.exec();
}
