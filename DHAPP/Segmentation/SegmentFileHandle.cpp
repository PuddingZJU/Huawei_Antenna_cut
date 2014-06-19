#include "SegmentFileHandle.h"
#include "Segmentation.h"
#include "MainWindow.h"

using namespace std;

SegmentFileHandle::SegmentFileHandle(): segment_file_name(SEGMENT_FILE_NAME)
{
}

SegmentFileHandle::~SegmentFileHandle()
{
}

void SegmentFileHandle::loadProjectFile(const QDomDocument*)
{
	//TODO:
}

void SegmentFileHandle::saveProjectFile(QDomDocument*)
{
	//TODO:
}

void SegmentFileHandle::saveSegmentInfo(std::ofstream& ofs)
{
	//TODO:
	Segmentation *seg_module = MainWindow::Instance().GetSegmentationModule();
	if (!seg_module) return;

	QMap<ImageWin *, int> &ImageWin_Seed_idx_Map = seg_module->ImageWin_SeedItMap;
	QMap<ImageWin *, std::vector<SeedRecords> > &ImageWin_SeedMap = seg_module->ImageWin_SeedMap;
	QMap<ImageWin *, QImage> &ImageWin_ResultMap = seg_module->ImageWin_ResultMap;

	QMap<ImageWin *, std::vector<SeedRecords> >::iterator it = ImageWin_SeedMap.begin();
	for ( ; it != ImageWin_SeedMap.end(); ++it)
	{
		QString file_path = it.key()->getPath();
		if (ImageWin_Seed_idx_Map.find(it.key()) == ImageWin_Seed_idx_Map.end()) continue;

		int operation_idx = ImageWin_Seed_idx_Map[it.key()];
		std::vector<SeedRecords> &seed_records = it.value();
		
	}
}

void SegmentFileHandle::loadSegmentInfo(std::ifstream &ifs)
{
	//TODO:
}