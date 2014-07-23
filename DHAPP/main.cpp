#include "MainWindow.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	QApplication::setLibraryPaths(QStringList(QCoreApplication::applicationDirPath()+"/plugins")); 
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	MainWindow w;
	w.show();
	qDebug()<<QImageReader::supportedImageFormats();
	return a.exec();
}
