#ifndef _SEGMENT_FILE_HANDLE_H
#define _SEGMENT_FILE_HANDLE_H

#include "ProjectFileInterface.h"
#include <fstream>

#define SEGMENT_FILE_NAME "seg.dat"

class SegmentFileHandle: public ProjectFileInterface
{
public:
	SegmentFileHandle();
	~SegmentFileHandle(void);

	virtual void loadProjectFile(const QDomDocument*);
	virtual void saveProjectFile(QDomDocument*);

	void saveSegmentInfo(std::ofstream &ofs);
	void loadSegmentInfo(std::ifstream &ifs);

private:
	std::string segment_file_name;
};

#endif