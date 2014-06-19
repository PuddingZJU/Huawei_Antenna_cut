#ifndef _LOG_H
#define _LOG_H

#include <iostream>
#include <fstream>

#define LOG_FILE_NAME "mat_log.txt"

class MatLog
{
public:
	static MatLog& get_instance()
	{
		static MatLog s_log;
		return s_log;
	}
	
	void log(cv::Mat &mat)
	{
		ofs<<mat<<std::endl;
	}

private:
	MatLog()
	{
		ofs.open(LOG_FILE_NAME);
	}
	std::ofstream ofs;
};

#ifdef _DEBUG
#define LOG_MAT(mat) MatLog::get_instance().log(mat);
#else
#define LOG_MAT(mat)
#endif

#endif