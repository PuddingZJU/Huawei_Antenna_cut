#ifndef _OUTOFCOREPANORAMAINTERFACE_H__
#define _OUTOFCOREPANORAMAINTERFACE_H__
#include <vector>
#include <string>
#include <QSet>
#include <QString>
#include "panoramaInterface.h"

void InitStxxlConfig(const char *stxxl_file_dir, int disk_usage);

/**
 * @param max_scaled_image_size means the maximum scaled image in M unit,
 * if the image size is bigger than this size, the image will be scaled 
 * below this maximum size.
 */
bool OutOfCoreStitchImage(const std::vector<std::string> &file_names, PanoramaParam pParam,
						  const char *file_name, int max_scaled_image_size = 128);

class ManagerStxxlFiles
{
public:
	static ManagerStxxlFiles* GetInstance();
	void RemoveStxxlFiles();
	void AddFile(const QString &file_name);

private:
	QSet<QString> outofcore_file_set;
};

#endif