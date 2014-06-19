#include "ImageListProjectFileHandle.h"

#include <QDomNodeList>
#include <QDomElement>
#include <QDomDocument>
#include <algorithm>
#include <qdebug.h>
using namespace std;

ImageListProjectFileHandle::ImageListProjectFileHandle(const QString& catalog)
:catalog_(catalog)
{
}

ImageListProjectFileHandle::~ImageListProjectFileHandle(void)
{
}

void ImageListProjectFileHandle::loadProjectFile(const QDomDocument* doc)
{
	imagefiles_.clear();

	QDomElement root = doc->documentElement();

	QDomNodeList nodelist = root.elementsByTagName(catalog_);
	if (nodelist.size())
	{
		QDomNode n = nodelist.at(0).firstChild();
		while (!n.isNull())
		{
			QDomNode img = n.firstChild();
			while(!img.isNull())
			{
				imagefiles_[n.toElement().tagName()].push_back(img.firstChild().toText().data());
				if(n.toElement().tagName() == tr("ԭʼͼƬ"))	
					image_files_checked_[n.toElement().tagName()].push_back(true);
				else
					image_files_checked_[n.toElement().tagName()].push_back(false);
				img = img.nextSibling();				
			}
			n = n.nextSibling();
		}	
	}

	emit loaded();
}

void ImageListProjectFileHandle::saveProjectFile(QDomDocument* doc)
{
	QDomElement root = doc->documentElement();

	QDomNodeList nodelist = root.elementsByTagName(catalog_);
	for (int i = 0; i < nodelist.size(); i++)
	{
		root.removeChild(nodelist.at(i));
	}
	
	QDomElement cataroot = doc->createElement(catalog_);
	root.appendChild(cataroot);

	for (ImageListFiles::iterator it = imagefiles_.begin(); it != imagefiles_.end(); it++)
	{
		QDomElement element = doc->createElement(it->first);
		cataroot.appendChild(element);
		for (std::vector<QString>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{	
			QDomText text = doc->createTextNode(*it2);
			QDomElement lielem = doc->createElement("local");
			lielem.appendChild(text);
			element.appendChild(lielem);
		}
	}	
}

void ImageListProjectFileHandle::addImage(const QString& hierarchy, const QString imagepath, bool bChecked /* = true */)
{
	/* if not exist the same image path */
	std::vector<QString>::iterator beg = imagefiles_[hierarchy].begin();
	std::vector<QString>::iterator end = imagefiles_[hierarchy].end();
	if(find(beg, end, imagepath) == end) {
		imagefiles_[hierarchy].push_back(imagepath);
		/* default is checked */
		image_files_checked_[hierarchy].push_back(bChecked);
	}

	emit modified();
}

void ImageListProjectFileHandle::addImage(const QString& hierarchy, const QStringList& image_paths, bool bChecked /*= true*/)
{
	std::vector<QString>::iterator beg = imagefiles_[hierarchy].begin();
	std::vector<QString>::iterator end = imagefiles_[hierarchy].end();
	for(size_t i = 0; i < image_paths.size(); i++)
	{
		QString imagepath = image_paths.at(i);
		qDebug()<<imagepath;
		//if(i==image_paths.size()-1)
		//{
			imagefiles_[hierarchy].push_back(imagepath);
			image_files_checked_[hierarchy].push_back(bChecked);
		//}
	}

	emit modified();
}
