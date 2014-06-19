#pragma once

#include <map>
#include <vector>
#include <QString>
#include <QStringList>

#include "ProjectFileInterface.h"
#include <cassert>

typedef std::map<QString, std::vector<QString> > ImageListFiles;
typedef std::map<QString, std::vector<bool> > ImageListChecked;
typedef std::map<QString, std::vector<bool> >::iterator CheckedIte;

class ImageListProjectFileHandle
	: public ProjectFileInterface
{
public:
	ImageListProjectFileHandle(const QString& catalog);
	~ImageListProjectFileHandle(void);

	virtual void loadProjectFile(const QDomDocument*);
	virtual void saveProjectFile(QDomDocument*);

	const ImageListFiles& getImages() const{return imagefiles_;}
	void addImage(const QString& hierarchy, const QString imagepath, bool bChecked = true); 
	void addImage(const QString& hierarchy, const QStringList& image_paths, bool bChecked = true);

	void set_image_checked(QString hierarchy, int index, bool bChecked) {
		assert(index < image_files_checked_[hierarchy].size());
		image_files_checked_[hierarchy][index] = bChecked;
	}

	bool get_image_checked(QString hierarchy, int index) {
		assert(index < image_files_checked_[hierarchy].size());
		return image_files_checked_[hierarchy][index];
	}

private:
	QString catalog_;
	ImageListFiles imagefiles_;
	ImageListChecked image_files_checked_;
};
