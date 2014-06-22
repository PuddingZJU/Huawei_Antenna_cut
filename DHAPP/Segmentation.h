#pragma once
#include <QObject>
#include <QImage>
#include <QtGui>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QDebug>
#include <QMap>

#include "cv.h"
#include "ImageWinEventInterface.h"
#include "OptionPageManager.h"
#include "SegmentationOptionPage.h"
#include <vector>
#include "SegCommand.h"
#define PI 3.14159265
/////eye
struct DotPoint 
{
	double scale;
	QPoint local_pos;
	int line_width;
	QColor color;
};

struct DotRect
{
	double scale;
	QRect local_rect;
	QColor color;
};

enum SeedType
{
	ST_Point,
	ST_Rect,

	ST_Count
};

enum SeedCategory
{
	SC_None,
	SC_Obj,
	SC_Bkg,

	SC_Count
};
struct SeedRecord
{
	SeedType type;
	SeedCategory category;

	DotPoint point;
	DotRect rect;
};

typedef std::vector<SeedRecord> SeedRecords;

enum SegmentMethod
{
	SM_Lazy,
	SM_Grab,
	SM_Graph,
	SM_Mixture,
	SM_Count
};

//////////////////////////////////////////////////////////////////////////eye
class SegmentationParamPanel;
class MainWindow;
class SegmentFileHandle;

class Segmentation : public QObject,public ImageWinEventInterface
{
	Q_OBJECT
		friend class ImageWin;
		friend class SegmentFileHandle;

public:
	Segmentation(SegmentationParamPanel* panel, MainWindow* win);
	~Segmentation(void);

	bool procPaint(ImageWin* imgwin, QPaintEvent *event);

	bool procMouseMove(ImageWin* imgwin, QMouseEvent* event);
	bool procMousePress(ImageWin* imgwin, QMouseEvent* event);
	bool procMouseRelease(ImageWin* imgwin, QMouseEvent* event);

	void ImageWin_SeedItMapPlusOne(ImageWin* imgwin){ImageWin_SeedItMap[imgwin]++;qDebug()<<"undo"<<ImageWin_SeedItMap[imgwin];}
	void ImageWin_SeedItMapMinusOne(ImageWin* imgwin){ImageWin_SeedItMap[imgwin]--;qDebug()<<"redo"<<ImageWin_SeedItMap[imgwin];}


private:

	

private slots:

		QImage excute(ImageWin* current_imgWin);
		void excuteSegmentation();
		void Reset();
		void ExportMaskImage();
		void ExportFrontImage();
		//void paint_object_changed();
		//void paint_background_changed();
		void dot_width_changed(int _value);

private:
	
	bool obj_paint;
	bool bkg_paint;
	bool in_selecting;
	int line_width;

	double lamda;
	double kappa;

	QString file_name;
	QImage image;
	QImage result;
	QImage mask_result;
	bool has_record_rect;
	SeedRecords seeds;
	std::vector<SeedRecords> SeedsVector;
	int seeds_pos;
	std::vector<cv::Point2i> objectPoints;
	std::vector<cv::Point2i> backgroundPoints;
	QPoint press_pos_local;	//鼠标按下时的位置
	QPoint selecting_start_pos;	//
	QPoint selecting_end_pos;	//当前鼠标的位置

	double scaleFactor;
	QMap<ImageWin*, int> ImageWin_SeedItMap;
	QMap<ImageWin*, std::vector<SeedRecords>> ImageWin_SeedMap;
	QMap<ImageWin*, QImage> ImageWin_ResultMap;
	
	std::map<ImageWin*, QRect> imagewin_selection_rect;

private:

	QPushButton *excute_button;
	QPushButton *reset_button;
	QPushButton *export_mask_button;
	QPushButton *export_front_button;

	QCheckBox *paint_obj_bt;
	QCheckBox *paint_bkg_bt;

	QToolButton *zoom_in;
	QToolButton *zoom_out;
	QSlider *image_slider;

	QLabel *dot_label;
	QLabel *dot_label_num;
	QSlider *dot_width_slider;

	QActionGroup * segment_action_group;

	SegmentationParamPanel* _panel;
	MainWindow* _window;
	const OptionPageManager* _OptionPageManager;
	SegmentationOptionPage * _OptionPage;
private:
	void init_components();
	void init_connects();

	void partion_segment_to_elements(cv::Mat &out_seg);

	void draw_point_on_dotLabel(int _value);

	QImage segment_lazybrush(QImage &image, QImage &mask_img, cv::Mat &out_seg);
	QImage segment_grabcut(QImage &image, QImage &mask_img, cv::Mat &out_seg);
	QImage segment_hist(QImage &image, QImage &mask_img, cv::Mat &out_seg);
	QImage segment_mixture(QImage &image, QImage &mask_img, cv::Mat &out_seg);
	QImage get_image_from_segmat(cv::Mat &out_seg);
	void mask_image_to_mask_mat(QImage &mask_img, cv::Mat &mask_mat);
	void showResult(QPaintDevice * device,QImage &img_result);
	QImage make_mask_image();
	void RecordPoints(ImageWin* imgwin,QPoint pos);
	void RecordSeeds(ImageWin* imgwin,SeedRecords seeds);
	void UndoRecordPoints(ImageWin* imgwin);
	void DrawPoints(QPaintDevice * device);
	void DrawPoint(QPaintDevice * device);
};
