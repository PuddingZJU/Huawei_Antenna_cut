#include "ProjectFileManager.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include "ProjectFileInterface.h"

ProjectFileManager::ProjectFileManager(void)
:document(NULL)
{
}

ProjectFileManager::~ProjectFileManager(void)
{
}

QString ProjectFileManager::name()
{
	return QFileInfo(filepath).baseName();
}

QString ProjectFileManager::path()
{
	return QFileInfo(filepath).absolutePath();
}

QString ProjectFileManager::getFilePath()
{
	return filepath;
}

void ProjectFileManager::newfile(const QString& path)
{
	filepath = path;
	setToSaveHandle.clear();
	initDomDocument();

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	QTextStream out(&file);
	document->save(out, 4);
	file.close();

	for (std::vector<ProjectFileInterface*>::iterator it = vecHandle.begin();
		it != vecHandle.end(); it++)
	{
		ProjectFileInterface* pfi = *it;
		pfi->loadProjectFile(document);
	}
	emit modified(false);
}

void ProjectFileManager::load(const QString& path)
{
	filepath = path;
	setToSaveHandle.clear();
	initDomDocument();

	QFile file(path);

	if (!path.isEmpty())
	{
		if(!file.open(QIODevice::ReadOnly))
		{
			QMessageBox msgbox;
			msgbox.setText(QObject::tr("文件无法打开"));
			msgbox.exec();
		}
		else
		{
			QString errormsg;
			int errorline, errorcolumn;
			if(!document->setContent(&file, &errormsg, &errorline, &errorcolumn))
			{
				QMessageBox msgbox;
				msgbox.setText(QObject::tr("文件解析出错(%1, %2)：%3").arg(errorline).arg(errorcolumn).arg(errormsg));
				msgbox.exec();

				file.close();
			}
		}
	}

	for (std::vector<ProjectFileInterface*>::iterator it = vecHandle.begin();
		it != vecHandle.end(); it++)
	{
		ProjectFileInterface* pfi = *it;
		pfi->loadProjectFile(document);
	}
	emit modified(false);
}

void ProjectFileManager::save()
{
	for (std::set<ProjectFileInterface*>::iterator it = setToSaveHandle.begin();
		it != setToSaveHandle.end(); it++)
	{
		ProjectFileInterface* pfi = *it;
		pfi->saveProjectFile(document);			
	}
	
	QFile file(filepath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QMessageBox msgbox;
		msgbox.setText(QObject::tr("文件无法保存"));
		msgbox.exec();
		return;
	}
	QTextStream out(&file);
	document->save(out, 4);
	file.close();

	setToSaveHandle.clear();
	emit modified(false);
}

void ProjectFileManager::saveas(const QString& path)
{
	filepath = path;
	save();
}

void ProjectFileManager::addHandle(ProjectFileInterface* handle)
{
	if (handle)
	{
		vecHandle.push_back(handle);
		connect(handle, SIGNAL(modified()), this, SLOT(onModified()));
	}	
}

void ProjectFileManager::initDomDocument()
{
	if (document)
	{
		delete document;
	}
	document = new QDomDocument;
	QDomProcessingInstruction instruction = document->createProcessingInstruction("xml","version=\'1.0\' encoding=\'GBK\'");
	document->appendChild(instruction);

	QDomElement root = document->createElement(QObject::tr("工程"));
	document->appendChild(root);
}

void ProjectFileManager::onModified()
{
	setToSaveHandle.insert(static_cast<ProjectFileInterface*>(QObject::sender()));
	emit modified(true);
}

void ProjectFileManager::reset()
{
	load(QString());
}