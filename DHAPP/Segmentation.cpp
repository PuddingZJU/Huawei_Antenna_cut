#include "Segmentation.h"

#include <QtGui>
#include "MainWindow.h"
#include "SegmentationParamPanel.h"
#include "Segmentation\CommonFunc.h"
#include "Segmentation\SegmentInterface.h"
#include "Segmentation\gmm\GMM.h"
#include <nlopt.hpp>
#include <math.h>
using namespace NMH;
#include <OperationModeManager.h>
#include <fstream>
#include "myGMM.h"
#define MAX_PIXEL_NUM	(300*1024)
using cv::Mat;
QImage opt_orig_image;
QImage opt_mask_image;
QImage sobel_image;
QImage laplace_image;
std::vector<double> init(3);
cv::RotatedRect opt_objRect;
cv::RotatedRect opt_backgroundRect;
cv::Point2f opt_objRectPoints[4];
cv::Point2f opt_backgroundRectPoints[4];
cv::Point2i lt,lb,rt,rb;
int x_offset,y_offset,use_file=0,use_feature=3,use_color=0,use_enhance=0,use_gmm=0,isright=0;
void opt_data_init(QImage _orig_image,QImage _mask_image,cv::RotatedRect _objRect,cv::RotatedRect _backgroundRect);
double opt_l_high,opt_l_low,opt_x_high,opt_x_low;
void set_x_offset(int x_offset);
void set_y_offset(int y_offset);
double getGMMFeature(double angle,cv::Point2f p);
double getBlockFeature(double angle,cv::Point2f p);
double getSobelFeature(double angle,cv::Point2f p);
double getLaplaceFreture(double angle,cv::Point2f p);
myGMM *bgmm,*fgmm;
void optimization_phase1(int tag); 
double myfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);
bool no_k;
//Mat qimage2mat(const QImage& qimage) { 
//	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine()); 
//	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 ); 
//	int from_to[] = { 0,0, 1,1, 2,2 }; 
//	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 ); 
//	return mat2; 
//}; 
cv::Mat guidedFilter(cv::Mat I, cv::Mat p, int r, double eps)
{
	/*
		% GUIDEDFILTER   O(1) time implementation of guided filter.
		%
		%   - guidance image: I (should be a gray-scale/single channel image)
		%   - filtering input image: p (should be a gray-scale/single channel image)
		%   - local window radius: r
		%   - regularization parameter: eps
	*/

	cv::Mat _I;
	I.convertTo(_I, CV_64FC1);
	I = _I;

	cv::Mat _p;
	p.convertTo(_p, CV_64FC1);
	p = _p;

	//[hei, wid] = size(I);
	int hei = I.rows;
	int wid = I.cols;

	//N = boxfilter(ones(hei, wid), r); % the size of each local patch; N=(2r+1)^2 except for boundary pixels.
	cv::Mat N;
	cv::boxFilter(cv::Mat::ones(hei, wid, I.type()), N, CV_64FC1, cv::Size(r, r));

	//mean_I = boxfilter(I, r) ./ N;
	cv::Mat mean_I;
	cv::boxFilter(I, mean_I, CV_64FC1, cv::Size(r, r));
	
	//mean_p = boxfilter(p, r) ./ N;
	cv::Mat mean_p;
	cv::boxFilter(p, mean_p, CV_64FC1, cv::Size(r, r));

	//mean_Ip = boxfilter(I.*p, r) ./ N;
	cv::Mat mean_Ip;
	cv::boxFilter(I.mul(p), mean_Ip, CV_64FC1, cv::Size(r, r));

	//cov_Ip = mean_Ip - mean_I .* mean_p; % this is the covariance of (I, p) in each local patch.
	cv::Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);

	//mean_II = boxfilter(I.*I, r) ./ N;
	cv::Mat mean_II;
	cv::boxFilter(I.mul(I), mean_II, CV_64FC1, cv::Size(r, r));

	//var_I = mean_II - mean_I .* mean_I;
	cv::Mat var_I = mean_II - mean_I.mul(mean_I);

	//a = cov_Ip ./ (var_I + eps); % Eqn. (5) in the paper;	
	cv::Mat a = cov_Ip/(var_I + eps);

	//b = mean_p - a .* mean_I; % Eqn. (6) in the paper;
	cv::Mat b = mean_p - a.mul(mean_I);

	//mean_a = boxfilter(a, r) ./ N;
	cv::Mat mean_a;
	cv::boxFilter(a, mean_a, CV_64FC1, cv::Size(r, r));
	mean_a = mean_a/N;

	//mean_b = boxfilter(b, r) ./ N;
	cv::Mat mean_b;
	cv::boxFilter(b, mean_b, CV_64FC1, cv::Size(r, r));
	mean_b = mean_b/N;

	//q = mean_a .* I + mean_b; % Eqn. (8) in the paper;
	cv::Mat q = mean_a.mul(I) + mean_b;

	return q;
}

void converToType(cv::Mat& mat, int mattype)
{
	cv::Mat _mat;
	mat.convertTo(_mat, mattype);
	mat = _mat;
}

void smooth_guidedfilter(IplImage* pInputImage)
{
	//I = double(imread('.\img_smoothing\cat.bmp')) / 255;
	cv::Mat I(pInputImage, true);
	converToType(I, CV_64FC1);
	I = I/255.0;

	//p = I;
	cv::Mat p(I);

	//r = 4; % try r=2, 4, or 8
	double r = 4;

	//eps = 0.2^2; % try eps=0.1^2, 0.2^2, 0.4^2
	double eps = pow(0.2, 2);

	//q = guidedfilter(I, p, r, eps);
	cv::Mat q = guidedFilter(I, p, r, eps);

	IplImage* iplImage = new IplImage(q);

	cvConvertImage(iplImage, pInputImage);

	delete iplImage;
}

void procEachImageChannel(IplImage* pInputImage, void proc(IplImage*))
{
	if(pInputImage->nChannels == 1) {
		proc(pInputImage);
	} else if(pInputImage->nChannels == 3) {
		IplImage* lChannelImgs[3];
		for(int i = 0; i < 3; i++) {
			lChannelImgs[i] = cvCreateImage(
				cvSize(pInputImage->width, pInputImage->height),
				IPL_DEPTH_8U, 1);
		}
		cvSplit(pInputImage, lChannelImgs[0], lChannelImgs[1], lChannelImgs[2], NULL);
		for(int i = 0; i < 3; i++) {
			proc(lChannelImgs[i]);
		}
		cvMerge(lChannelImgs[0], lChannelImgs[1], lChannelImgs[2], NULL, pInputImage);

		for(int i = 0; i < 3; i++) {
			cvReleaseImage(&lChannelImgs[i]);
		}
	} else {
		printf("unsupported channel number in enhance()\n");
		assert(0);
	}
}



void enhance_guidedfilter(IplImage* pInputImage)
{
	cv::Mat I(pInputImage, true);
	converToType(I, CV_64FC1);

	I = I/255.0;

	//q = zeros(size(I));
	cv::Mat q(cv::Mat::zeros(I.rows, I.cols, CV_64FC1));

	//q(:, :, 1) = guidedfilter(I(:, :, 1), p(:, :, 1), r, eps);
	cv::Mat p = I;
	int r = 10;
	double eps = 1.0;
	q = guidedFilter(I, p, r, eps);

	//I_enhanced = (I - q) * 5 + q;
	cv::Mat I_enhanced = (I - q)*(5) + q;

	IplImage* iplImage = new IplImage(I_enhanced);

	cvConvertImage(iplImage, pInputImage);

	delete iplImage;
}

Segmentation::Segmentation(SegmentationParamPanel* panel, MainWindow* win)
	:QObject(win),
	_panel(panel),
	_window(win)
{
	obj_paint = false;
	bkg_paint = false;

	in_selecting = false;

	has_record_rect = false;

	init_components();
	init_connects();

	ImageWin::RegisterEventReceiver(this);
	_OptionPageManager = _window->GetOptionManager();
	_OptionPage = dynamic_cast<SegmentationOptionPage *>(_OptionPageManager->GetPage(tr("分割算法参数")));
}

Segmentation::~Segmentation(void)
{
}


void Segmentation::init_components()
{
	/// panel ui
	excute_button = _panel->ui.excute_bt;
	reset_button = _panel->ui.reset_bt;
	export_mask_button = _panel->ui.pushButton_getmask;
	export_front_button = _panel->ui.pushButton_getfront;
	dot_label = _panel->ui.dot_label;
	dot_label_num = _panel->ui.dot_label_num;
	dot_width_slider = _panel->ui.dot_width_slider;
	dot_width_changed(dot_width_slider->value());

	segment_action_group = new QActionGroup(this);
	segment_action_group->addAction(_window->ui.actionLazyBrush);
	segment_action_group->addAction(_window->ui.actionGrabCut);
	segment_action_group->addAction(_window->ui.actionGraphCut);
	segment_action_group->addAction(_window->ui.actionMixture);
	_window->ui.actionGrabCut->setChecked(true);
}

void Segmentation::init_connects()
{
	QObject:: connect(excute_button, SIGNAL(clicked()), this, SLOT(excuteSegmentation()));

	QObject:: connect(reset_button, SIGNAL(clicked()), this, SLOT(Reset()));
	QObject:: connect(export_mask_button, SIGNAL(clicked()), this, SLOT(ExportMaskImage()));
	QObject:: connect(export_front_button, SIGNAL(clicked()), this, SLOT(ExportFrontImage()));
	//QObject:: connect(paint_obj_bt, SIGNAL(clicked(bool )), this, SLOT(paint_object_changed()));
	//QObject:: connect(paint_bkg_bt, SIGNAL(clicked(bool )), this, SLOT(paint_background_changed()));

	QObject:: connect(dot_width_slider, SIGNAL(valueChanged(int )), this, SLOT(dot_width_changed(int)));

}
void Segmentation::ExportFrontImage(){
	QString filepath = QFileDialog::getSaveFileName( 
		_panel, 
		tr("Save As"),
		QDir::currentPath(), 
		tr("BMP Image(*.bmp)") );

	if (!filepath.isEmpty())
	{
		QImage res = mask_result.alphaChannel();
		for(int i=0;i<res.width();i++){
			for(int j=0;j<res.height();j++){
				QColor tmp = QColor::fromRgba(res.pixel(i,j));
				if(tmp.red()==0){
					//tmp.setRgb(255,255,255);
					res.setPixel(i,j,128);
				}
				else{
					res.setPixel(i,j,255);
				}
			}
		}
		res.save(filepath,"BMP");
	}
}
void Segmentation::ExportMaskImage(){
	QString filepath = QFileDialog::getSaveFileName( 
		_panel, 
		tr("Save As"),
		QDir::currentPath(), 
		tr("BMP Image(*.bmp)") );

	if (!filepath.isEmpty())
	{
		QImage res = mask_result.alphaChannel();
		for(int i=0;i<res.width();i++){
			for(int j=0;j<res.height();j++){
				QColor tmp = QColor::fromRgba(res.pixel(i,j));
				if(tmp.red()==0){
					//tmp.setRgb(255,255,255);
					res.setPixel(i,j,255);
				}
				else{
					tmp.setRgb(0,0,0);
					res.setPixel(i,j,0);
				}
			}
		}
		res.save(filepath,"BMP");
	}
}
void Segmentation::RecordPoints(ImageWin* imgwin,QPoint pos)
{
	SeedRecord seed_record;

	QComboBox *combo_box_obj = _panel->ui.comboBox_obj;
	QComboBox *combo_box_bkg = _panel->ui.comboBox_bkg;

	DotPoint &point = seed_record.point;
	point.line_width =_panel->ui.dot_label_num->text().toInt();//
	point.scale = imgwin->getscaleFactor();

	DotRect &rect = seed_record.rect;
	rect.scale = imgwin->getscaleFactor();

	if (obj_paint)
	{
		seed_record.category = SC_Obj;
		//qDebug()<<"SC_Obj";

		if (combo_box_obj->currentIndex() == 0)
		{
			seed_record.type = ST_Point;

			point.color = _OptionPage->GetObjcolor();

			point.local_pos = pos;
			seeds.push_back(seed_record);
		}
		else if (combo_box_obj->currentIndex() == 1)
		{
			if (!has_record_rect)
			{
				seed_record.type = ST_Rect;

				rect.color = _OptionPage->GetObjcolor();
				rect.local_rect.setTopLeft(press_pos_local);
				rect.local_rect.setBottomRight(pos);
				seeds.push_back(seed_record);
				has_record_rect = true;
			}
			else
			{
				SeedRecord &tmp_record = seeds[seeds.size() - 1];
				tmp_record.rect.local_rect.setBottomRight(pos);
			}
		}
	}
	/// 背景
	if(this->bkg_paint) 
	{
		seed_record.category = SC_Bkg;
		//qDebug()<<"SC_Bkg";

		if ( combo_box_bkg->currentIndex() == 0)
		{
			seed_record.type = ST_Point;

			point.color = _OptionPage->GetBkgcolor();

			point.local_pos = pos;
			seeds.push_back(seed_record);
		}
		else if (combo_box_bkg->currentIndex() == 1)
		{
			if (!has_record_rect)
			{
				seed_record.type = ST_Rect;

				rect.color = _OptionPage->GetBkgcolor();

				rect.local_rect.setTopLeft(press_pos_local);
				rect.local_rect.setBottomRight(pos);
				seeds.push_back(seed_record);
				has_record_rect = true;
			}
			else
			{
				SeedRecord &tmp_record = seeds[seeds.size() - 1];
				tmp_record.rect.local_rect.setBottomRight(pos);
			}
		}
	}
}

void Segmentation::RecordSeeds(ImageWin* imgwin,SeedRecords seeds)
{
	if (ImageWin_SeedMap.find(imgwin) != ImageWin_SeedMap.end())
	{
		seeds_pos = ImageWin_SeedItMap[imgwin];
		SeedsVector = ImageWin_SeedMap[imgwin];
		//qDebug()<<tr("第二次记录")<<imgwin->getPath();
	}
	else//不存在 
	{
		SeedsVector.clear();
		seeds_pos = 0;
		//qDebug()<<tr("第一次记录")<<imgwin->getPath();
	}
	//将seeds放入SeedRecods中 
	qDebug()<<"before"<<seeds_pos;
	std::vector<SeedRecords>::iterator it = SeedsVector.insert(SeedsVector.begin()+seeds_pos,seeds);

	SeedsVector.erase(SeedsVector.begin(),it);
	seeds_pos=0;//插入新的笔画，则恢复初始状态 

	qDebug()<<"after"<<seeds_pos;
	qDebug()<<"before"<<imgwin->getPath()<<"SeedsVector"<<SeedsVector.size()<<"ImageWin_SeedMap[imgwin]"<<ImageWin_SeedMap[imgwin].size();

	//判断ImageWin是否已存在 
	ImageWin_SeedItMap[imgwin] = seeds_pos;
	ImageWin_SeedMap[imgwin] = SeedsVector;

	qDebug()<<imgwin->getPath()<<"SeedsVector"<<SeedsVector.size()<<"ImageWin_SeedMap[imgwin]"<<ImageWin_SeedMap[imgwin].size();

	//记录操作 
	_window->GetUndoRedoManager()->AddCommand(new SegCommand(ON_Stroke,imgwin,this));
}
QImage mat2qimage(const cv::Mat& mat)
{
	/*cv::Mat rgb;
	cv::cvtColor(mat, rgb, CV_BGR2RGB);*/
	imwrite("so.jpg",mat);
	return QImage("so.jpg"); 
}
QImage Segmentation::excute(ImageWin* current_imgWin)
{
	QImage image = current_imgWin->getImage();//eye
	QImage mask_img = this->make_mask_image();//eye

	QImage segment_img;
	cv::Mat out_seg;

	int width = image.width();
	int height = image.height();

	bool big_image = image.width() * image.height() > MAX_PIXEL_NUM;
	bool use_scale = _panel->ui.big_image_seg->currentIndex() == 0;
	double scale_ratio = -1;

	QSize src_size = image.size();
	QSize dst_size;
	big_image = false;

	if (big_image)
	{
		cout<<"it is a big image\t"<<"width = "<<width<<"\theight = "<<height<<endl;
		if (use_scale)
		{
			scale_ratio = sqrt(image.width() * image.height()/(double)MAX_PIXEL_NUM);
			dst_size = image.size()/scale_ratio;
			image = image.scaled(dst_size);
			mask_img = mask_img.scaled(dst_size);
		}
		else	//使用选定区域的方式分割
		{
			QRect _trect = imagewin_selection_rect[current_imgWin];
			qDebug()<<"selection region is "<<_trect<<endl;
			image = image.copy(_trect);
			mask_img = mask_img.copy(_trect);
		}
		//image.save("C:\\Users\\leon\\Desktop\\img.bmp","BMP");
		//mask_img.save("C:\\Users\\leon\\Desktop\\mask.bmp","BMP");


		string inter_image_str("inter_image");
		string inter_mask_image_str("inter_mask_image");

		cv::Mat inter_image;
		cv::Mat inter_mask_image;
		QImage2Mat(image, inter_image);
		mask_image_to_mask_mat(mask_img, inter_mask_image);
		for (int y = 0; y < inter_mask_image.rows; y++)
		{
			for (int x = 0; x < inter_mask_image.cols; x++)
			{
				if (inter_mask_image.at<char>(y, x) == 1)
				{
					inter_mask_image.at<char>(y, x) = 0x7F;
				}
			}
		}

		//cv::namedWindow(inter_image_str);
		//cv::imshow(inter_image_str, inter_image);
		//cv::waitKey(-1);

		//cv::namedWindow(inter_mask_image_str);
		//cv::imshow(inter_mask_image_str, inter_mask_image);
		//cv::waitKey(-1);
	}

	string fs = current_imgWin->getPath().toStdString();
	string pth = "pic_";
	pth.push_back(fs[fs.size()-5]);
	pth += ".input";
	use_file=1;
	if(!use_file){
		std::ofstream out;
		out.open(pth);
		for(int i=0;i<mask_img.width();i++){
			for(int j=0;j<mask_img.height();j++){
				QColor tmp = QColor::fromRgba(mask_img.pixel(i,j));
				if(tmp.red()==255){
					out<<"o "<<i<<" "<<j<<endl;
					objectPoints.push_back(cv::Point2i(i,j));
				}
				if(tmp.red()==0){
					backgroundPoints.push_back(cv::Point2i(i,j));
					out<<"b "<<i<<" "<<j<<endl;
				}
			}
		}
	}
	
	else{
		std::ifstream in;
		in.open(pth.c_str());
		char type;
		int _i,_j;
		while (in>>type>>_i>>_j)
		{
			if (type == 'b')
			{
				backgroundPoints.push_back(cv::Point2i(_i,_j));
			}
			else{
				objectPoints.push_back(cv::Point2i(_i,_j));
			}
		}
		in.close();
	}
	cv::Mat im = cv::imread(fs.c_str(), CV_LOAD_IMAGE_COLOR); 
	if (use_gmm)
	{
		double *fdata = new double[objectPoints.size()];
		double *bdata = new double[backgroundPoints.size()];
		for (int i = 0 ; i < objectPoints.size();i++)
		{
			fdata[i]= qGray(image.pixel(objectPoints[i].x,objectPoints[i].y));
			//qDebug()<<fdata[i]<<endl;
		}
		for (int i = 0 ; i < backgroundPoints.size();i++)
		{
			bdata[i]= qGray(image.pixel(backgroundPoints[i].x,backgroundPoints[i].y));
			//qDebug()<<bdata[i]<<endl;
		}
		fgmm = new myGMM(1,1);
		fgmm->Train(fdata,objectPoints.size());
		bgmm = new myGMM(1,1);
		bgmm->Train(bdata,backgroundPoints.size());
	}
	if (im.empty()) {
		cout << "Cannot load image!" << endl;
	}
	cv::Mat sobel_mat(im.rows, im.cols, 1),laplace_mat(im.rows, im.cols, 1),sobel_mat_x(im.rows, im.cols, 1),sobel_mat_y(im.rows, im.cols, 1),abs_sobel_mat_x(im.rows, im.cols, 1),abs_sobel_mat_y(im.rows, im.cols, 1);
	Mat im_gray;
	cvtColor(im, im_gray, CV_RGB2GRAY);
	//imshow("Gray", im_gray);
	if (use_feature==1 ){
		cv::Sobel( im_gray, sobel_mat_x, im_gray.depth(), 1, 0, 3);
		cv::Sobel( im_gray, sobel_mat_y, im_gray.depth(), 0, 1, 3);
		cv::convertScaleAbs(sobel_mat_x,abs_sobel_mat_x);
		cv::convertScaleAbs(sobel_mat_y,abs_sobel_mat_y);
		cv::addWeighted(abs_sobel_mat_x,0.5,abs_sobel_mat_y,0.5,0,sobel_mat);
		if (use_enhance)
		{
			IplImage* sobel_ipl = new IplImage(sobel_mat);
			procEachImageChannel(sobel_ipl,smooth_guidedfilter);
			procEachImageChannel(sobel_ipl,enhance_guidedfilter);
			procEachImageChannel(sobel_ipl,enhance_guidedfilter);
			sobel_mat = Mat(sobel_ipl,true);
		}
		sobel_image = mat2qimage(sobel_mat);
		sobel_image.save("sobel.bmp","BMP");
	}
	if (use_feature==2){
		cv::Laplacian(im_gray,laplace_mat,im_gray.depth());
		//imshow("Gray", laplace_mat);
		if (use_enhance)
		{
			IplImage* laplace_ipl = new IplImage(laplace_mat);
			procEachImageChannel(laplace_ipl,smooth_guidedfilter);
			procEachImageChannel(laplace_ipl,enhance_guidedfilter);
			procEachImageChannel(laplace_ipl,enhance_guidedfilter);
			laplace_mat = Mat(laplace_ipl,true);
		}
		laplace_image = mat2qimage(laplace_mat);
		laplace_image.save("laplace.bmp","BMP");
	}
	
	cv::RotatedRect objRect = cv::minAreaRect(objectPoints);
	cv::RotatedRect backgroundRect = cv::minAreaRect(backgroundPoints);
	objectPoints.clear();
	backgroundPoints.clear();
	cv::Point2f objRectPoints[4];
	cv::Point2f backgroundRectPoints[4];
	objRect.points(objRectPoints);
	backgroundRect.points(backgroundRectPoints);
	QImage test_image = image.copy();
	QPainter p;
	p.begin(&test_image);
	p.setPen(Qt::SolidLine);
	p.setPen(QColor(0,255,0));
	for(int i=0;i<4;i++){
		p.drawLine((int)objRectPoints[i].x,(int)objRectPoints[i].y,(int)objRectPoints[(i+1)%4].x,(int)objRectPoints[(i+1)%4].y);	
	}
	p.setPen(QColor(0,0,255));
	for(int i=0;i<4;i++){
		p.drawLine((int)backgroundRectPoints[i].x,(int)backgroundRectPoints[i].y,(int)backgroundRectPoints[(i+1)%4].x,(int)backgroundRectPoints[(i+1)%4].y);
	}

	if (use_feature == 3)
	{
		
		lt.x = min(backgroundRectPoints[0].x,backgroundRectPoints[1].x);
		lt.y = max(objRectPoints[1].y,objRectPoints[2].y);
		lb.x = max(objRectPoints[1].x,objRectPoints[0].x);
		lb.y = min(objRectPoints[0].y,objRectPoints[3].y);
		rb.x = max(backgroundRectPoints[2].x,backgroundRectPoints[3].x);
		rt.y = lt.y;
		rt.x = min(objRectPoints[2].x,objRectPoints[3].x);
		rb.y = lb.y;
		QImage  Limage = image.copy(QRect(QPoint(lt.x,lt.y),QPoint(lb.x,lb.y)));
		QImage Rimage = image.copy(QRect(QPoint(rt.x,rt.y),QPoint(rb.x,rb.y)));
		Mat l_m,r_m,l_lab,r_lab;
		QImage2Mat(Limage,l_m);
		QImage2Mat(Rimage,r_m);
		imwrite("l.jpg",l_m);
		imwrite("r.jpg",r_m);
		cvtColor(l_m,l_lab,CV_RGB2Lab);
		cvtColor(r_m,r_lab,CV_RGB2Lab);
		imwrite("l_lab.jpg",l_lab);
		imwrite("r_lab.jpg",r_lab);

		return Limage;
	}
	else
	{
		opt_data_init(image,mask_img,objRect,backgroundRect);
		p.setPen(QColor(255,0,0));

		optimization_phase1(0);
		if(fabs(init[0]-90)<1e-4){
			p.drawLine(init[1],opt_l_high,init[1],opt_l_low);
		}
		else{
			double res_k = tan(init[0] *PI /180.00);
			double res_b = init[2]-init[1]*res_k;
			double res_x1 = (opt_l_low-res_b)/res_k;
			double res_x2 = (opt_l_high-res_b)/res_k;
			p.drawLine(res_x1,opt_l_low,res_x2,opt_l_high);
		}
		optimization_phase1(1);
		if(fabs(init[0]-90)<1e-4){
			p.drawLine(init[1],opt_l_high,init[1],opt_l_low);
		}
		else{
			double res_k = tan(init[0]*PI /180.00);
			double res_b = init[2]-init[1]*res_k;
			double res_x1 = (opt_l_low-res_b)/res_k;
			double res_x2 = (opt_l_high-res_b)/res_k;
			p.drawLine(res_x1,opt_l_low,res_x2,opt_l_high);
		}
		p.end();
		return test_image;

	}
	
	/*if (_window->ui.actionLazyBrush->isChecked())
	{
	segment_img = segment_lazybrush(image, mask_img, out_seg);
	}
	else if (_window->ui.actionGrabCut->isChecked())
	{
	segment_img = segment_grabcut(image, mask_img, out_seg);
	}
	else if (_window->ui.actionGraphCut->isChecked())
	{
	segment_img = segment_hist(image, mask_img, out_seg);
	}
	else if (_window->ui.actionMixture->isChecked())
	{
	segment_img = segment_mixture(image, mask_img, out_seg);
	}*/

	if (big_image)
	{
		cv::Mat dst_out_seg;
		if (use_scale)
		{
			cv::resize(out_seg, dst_out_seg, cv::Size(src_size.width(), src_size.height()));
		}
		else
		{
			dst_out_seg.create(src_size.height(), src_size.width(), CV_8UC1);
			dst_out_seg.setTo(cv::Scalar(SegmentationBackground));

			QRect _trect = imagewin_selection_rect[current_imgWin];
			QPoint _tpoint = _trect.topLeft();
			for (int y = 0; y < _trect.height(); y++)
			{
				for (int x = 0; x < _trect.width(); x++)
				{
					int yy = y + _tpoint.y();
					int xx = x + _tpoint.x();

					if (out_seg.at<uchar>(y, x) == SegmentationForeground)
						dst_out_seg.at<uchar>(yy, xx) = SegmentationForeground;
				}
			}
		}

		//cv::namedWindow("dst_out_seg");
		//imshow("dst_out_seg", dst_out_seg);
		//cv::waitKey(-1);
		partion_segment_to_elements(dst_out_seg);
		segment_img = get_image_from_segmat(dst_out_seg);

	}
	mask_result = segment_img;
	return segment_img;
}

QImage Segmentation::make_mask_image()
{
	ImageWin* current_imgWin = _window->GetActiveSubWindow();
	QImage image = current_imgWin->getImage();//eye

	QImage paint_image(image.size(), QImage::Format_RGB32);
	paint_image.fill(0xFF7F7F7F);

	QPainter painter(&paint_image);
	painter.setRenderHint(QPainter::Antialiasing, true);

	int m_SeedsIt = ImageWin_SeedItMap[current_imgWin];

	vector<SeedRecords> &m_SeedsVector = ImageWin_SeedMap[current_imgWin];

	std::vector<SeedRecords>::iterator it = m_SeedsVector.begin()+m_SeedsIt;

	for (int i=m_SeedsIt; i < m_SeedsVector.size(); i++,it++)//所有笔画
	{
		SeedRecords &_seeds =*it;
		for (int ii = 0; ii < _seeds.size(); ii++)//每一笔
		{
			SeedRecord &seed_record = _seeds[ii];

			if (seed_record.category == SC_Bkg)
			{
				painter.setPen(QPen(QColor(0, 0, 0), seed_record.point.line_width/seed_record.point.scale, Qt::SolidLine, Qt::RoundCap));
				painter.setBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));
			}
			else if (seed_record.category == SC_Obj)
			{
				painter.setPen(QPen(QColor(0xFF, 0xFF, 0xFF), seed_record.point.line_width/seed_record.point.scale, Qt::SolidLine, Qt::RoundCap));
				painter.setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF), Qt::SolidPattern));
			}

			if (seed_record.type == ST_Point)
			{
				painter.drawPoint(seed_record.point.local_pos/seed_record.point.scale);
			}
			else if (seed_record.type == ST_Rect)
			{
				QRect tmp_rect;
				tmp_rect.setTopLeft(seed_record.rect.local_rect.topLeft()/seed_record.rect.scale);
				tmp_rect.setBottomRight(seed_record.rect.local_rect.bottomRight()/seed_record.rect.scale);
				painter.drawRect(tmp_rect);
			}
		}
	}

	return paint_image;
}
void Segmentation::showResult(QPaintDevice * device,QImage &img_result)
{
	QPainter painter(device);
	painter.setRenderHint(QPainter::Antialiasing, true);

	//r,g,b,a
	int width = img_result.width();
	int height = img_result.height();

	ImageWin * current_win = (ImageWin *)device;

	QImage image = current_win->getImage();
	scaleFactor = current_win->getscaleFactor();

	QRect rect = QRect(QPoint(0,0), scaleFactor * image.size());
	painter.drawImage(rect, img_result);
}

QImage Segmentation::segment_lazybrush(QImage &image, QImage &mask_img, cv::Mat &out_seg)
{
	QImage result = image;

	cv::Mat img_mat;
	cv::Mat mask_mat;

	QImage2Mat(image, img_mat);
	mask_image_to_mask_mat(mask_img, mask_mat);

	const LazyBrushParam * param;

	param = _OptionPage->GetLazyBrushParam();
	/*param.sd_param.kappa = _OptionPage->ui.kappa_lineEdit->text().toDouble();

	param.canny_param.canny_min = _OptionPage->ui.canny_min_edit->text().toInt();
	param.canny_param.canny_max = _OptionPage->ui.canny_max_edit->text().toInt();

	param.use_canny_input = _OptionPage->ui.use_canny_input->isChecked();
	param.use_canny_enhance = _OptionPage->ui.canny_checkbox->isChecked();
	param.canny_erode = _OptionPage->ui.canny_erode_checkbox->isChecked();*/

	LazyBrushSegmentor lazybrusher(*param);

	lazybrusher.segment(img_mat, mask_mat, out_seg);

	partion_segment_to_elements(out_seg);

	return get_image_from_segmat(out_seg);
}

void Segmentation::mask_image_to_mask_mat(QImage &mask_img, cv::Mat &mask_mat)
{
	mask_mat.create(mask_img.height(), mask_img.width(), CV_8SC1);

	for (int y = 0; y < mask_mat.rows; y++)
	{
		for (int x = 0; x < mask_mat.cols; x++)
		{
			QRgb pix = mask_img.pixel(x, y);
			if (pix == 0xFF000000)
			{
				mask_mat.at<char>(y, x) = MASK_BACKGROUND;
			}
			else if (pix == 0xFFFFFFFF) 
			{
				mask_mat.at<char>(y, x) = MASK_FOREGROUND;
			}
			else
			{
				mask_mat.at<char>(y, x) = MASK_UNKNOW;
			}
		}
	}
}

QImage Segmentation::segment_grabcut(QImage &image, QImage &mask_img, cv::Mat &out_seg)
{
	cv::Mat img_mat;
	cv::Mat mask_mat;

	QImage2Mat(image, img_mat);
	mask_image_to_mask_mat(mask_img, mask_mat);

	// 	cv::namedWindow("source_image");
	// 	cv::namedWindow("seed_image");
	// 	cv::imshow("source_image", img_mat);
	// 	cv::imshow("seed_image", mask_mat);
	// 	cv::waitKey();

	const NMH::GrabCutParam * param;
	//eye
	param = _OptionPage->GetGrabCutParam();
	//param.sd_param.kappa = _OptionPage->ui.kappa_lineEdit->text().toDouble();

	//param.canny_param.canny_min = _OptionPage->ui.canny_min_edit->text().toInt();
	//param.canny_param.canny_max = _OptionPage->ui.canny_max_edit->text().toInt();


	NMH::GrabCutSegmentor grabcut_segmentor(*param);

	grabcut_segmentor.segment(img_mat, mask_mat, out_seg);

	partion_segment_to_elements(out_seg);

	return get_image_from_segmat(out_seg);
}

QImage Segmentation::get_image_from_segmat(cv::Mat &out_seg)
{
	QImage img(out_seg.cols, out_seg.rows, QImage::Format_ARGB32);
	img.fill(0x00000000);
	for (int y = 0; y < out_seg.rows; y++)
	{
		for (int x = 0; x < out_seg.cols; x++)
		{
			char seg_value = out_seg.at<char>(y, x);
			if (seg_value == SegmentationBackground)
			{
				img.setPixel(x, y, 0xDD000000);
			}
		}
	}
	return img;
}

void Segmentation::partion_segment_to_elements(cv::Mat &out_seg)
{
	IplImage* pMaskImg = cvCloneImage(&(IplImage)out_seg);
	cvThreshold(pMaskImg, pMaskImg, 0, 255, CV_THRESH_BINARY_INV );

	static CvMemStorage* g_storage = cvCreateMemStorage(0);
	CvSeq* contours = 0, *contour = 0;
	cvFindContours(pMaskImg, g_storage, &contours,
		sizeof(CvContour), CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0) );
	cvZero(pMaskImg);
	int i = 128;
	for (contour = contours;contour!= 0;contour= contour->h_next)
	{
		cvDrawContours(
			pMaskImg,
			contour,
			cvScalarAll(i++),
			cvScalarAll(i),
			0, CV_FILLED );
	}
	MainWindow::Instance().GetElementPickModule()->setMask(MainWindow::Instance().GetActiveSubWindow(), pMaskImg);
	MainWindow::Instance().GetElementPickModule()->setContours(MainWindow::Instance().GetActiveSubWindow(), contours);
}

QImage Segmentation::segment_hist(QImage &image, QImage &mask_img, cv::Mat &out_seg)
{
	QImage result;

	cv::Mat img_mat;
	cv::Mat mask_mat;

	QImage2Mat(image, img_mat);
	mask_image_to_mask_mat(mask_img, mask_mat);

	const NMH::GraphCutParam * param;
	param = _OptionPage->GetGraphCutParam();
	qDebug()<<param->sd_param.kappa<<"Lamda"<<param->sd_param.lamda;

	NMH::GraphCutSegmentor graphcut_segmentor(*param);

	graphcut_segmentor.segment(img_mat, mask_mat, out_seg);

	partion_segment_to_elements(out_seg);

	return get_image_from_segmat(out_seg);
}

QImage Segmentation::segment_mixture(QImage &image, QImage &mask_img, cv::Mat &out_seg)
{
	QImage result = image;

	cv::Mat img_mat;
	cv::Mat mask_mat;

	QImage2Mat(image, img_mat);
	mask_image_to_mask_mat(mask_img, mask_mat);

	const NMH::MixtureParam * param;
	param= _OptionPage->GetMixtureParam();
	/*param.sd_param.kappa = _OptionPage->ui.kappa_lineEdit->text().toDouble();

	param.canny_param.canny_min = _OptionPage->ui.canny_min_edit->text().toInt();
	param.canny_param.canny_max = _OptionPage->ui.canny_max_edit->text().toInt();

	param.edge_weight = _OptionPage->ui.edge_weight_lineEdit->text().toDouble();
	if (_OptionPage->ui.linear_radioButton->isChecked())
	{
	param.mixture_type = MT_Linear;
	}
	else if (_OptionPage->ui.comboBox->currentIndex() == 0)
	{
	param.mixture_type = MT_Squared;
	}
	else if (_OptionPage->ui.comboBox->currentIndex() == 1)
	{
	param.mixture_type = MT_Cubical;
	}*/

	NMH::MixtureSegmentor segmentor(*param);

	segmentor.segment(img_mat, mask_mat, out_seg);

	partion_segment_to_elements(out_seg);

	return get_image_from_segmat(out_seg);
}

//void Segmentation::paint_object_changed()
//{
//	if (this->obj_paint)
//		this->obj_paint = false;
//	else
//	{
//		this->obj_paint = true;
//		this->bkg_paint = false;
//		_panel->ui.paint_bkg_checkBox->setChecked(false);
//	}
//}
//void Segmentation::paint_background_changed()
//{
//	if (this->bkg_paint)
//		this->bkg_paint = false;
//	else
//	{
//		this->bkg_paint = true;
//		this->obj_paint = false;
//		_panel->ui.paint_obj_checkBox->setChecked(false);
//	}
//}

void Segmentation::dot_width_changed(int _value)
{
	line_width = _value;
	QString text = QString::number(dot_width_slider->value());
	dot_label_num->setText(text);
	draw_point_on_dotLabel(_value);
}

void Segmentation::draw_point_on_dotLabel(int _value)
{
	QImage image(dot_label->size(), QImage::Format_RGB32);
	image.fill(0x00FFFFFF);
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
	painter.setPen(QPen(Qt::black, _value, Qt::SolidLine, Qt::RoundCap));

	painter.drawPoint(dot_label->size().width()/2, dot_label->size().height()/2);
	dot_label->setPixmap(QPixmap::fromImage(image));
}

bool Segmentation::procMousePress(ImageWin* imgwin, QMouseEvent* event)
{
	if (MainWindow::Instance().GetOperationModeManager()->mode() != SEGMENTATIONMODE)
		return false;

	has_record_rect = false;
	in_selecting = false;

	//seeds.clear();
	if (QApplication::keyboardModifiers () == Qt::ControlModifier && event->button() == Qt::LeftButton)
	{
		in_selecting = true;
		obj_paint = bkg_paint = false;
		selecting_start_pos = selecting_end_pos = event->pos()/scaleFactor;
	}
	else if (event->button() == Qt::LeftButton) 
	{
		obj_paint  = true;
		bkg_paint = false;
	}
	else
	{
		bkg_paint  = true;
		obj_paint = false;
	}

	press_pos_local = event->pos();
	if (bkg_paint || obj_paint)	this->RecordPoints(imgwin,event->pos());
	imgwin->update();
	return true;
}

bool Segmentation::procMouseMove(ImageWin* imgwin, QMouseEvent* event)
{
	if (MainWindow::Instance().GetOperationModeManager()->mode() != SEGMENTATIONMODE)
		return false;
	if (in_selecting)
	{
		selecting_end_pos = event->pos()/scaleFactor;
		//qDebug()<<selecting_end_pos<<"\t";
	}
	else if(imgwin->getPressedMouseButton() == Qt::LeftButton || imgwin->getPressedMouseButton() == Qt::RightButton)
	{
		this->RecordPoints(imgwin,event->pos());
	}

	imgwin->update();
	return true;
}
bool Segmentation::procMouseRelease(ImageWin* imgwin, QMouseEvent* event)
{
	if (MainWindow::Instance().GetOperationModeManager()->mode() != SEGMENTATIONMODE)
		return false;

	if (in_selecting)
	{
		selecting_end_pos = event->pos()/scaleFactor;
		QRect selection_region(selecting_start_pos, selecting_end_pos);
		selection_region = selection_region.normalized();

		//qDebug()<<selecting_start_pos<<"\t"<<selecting_end_pos<<"width = "<<selection_region.width()<<"\n";
		imagewin_selection_rect[imgwin] = selection_region;
		in_selecting = false;
	}
	else
	{
		this->RecordPoints(imgwin, event->pos());
		this->RecordSeeds(imgwin, seeds);

		if (ImageWin_SeedMap.find(imgwin) == ImageWin_SeedMap.end())
		{
			qDebug()<<tr("没找到seedsVector")<<imgwin->getPath();
			return false;
		}

		vector<SeedRecords> &m_SeedsVector = ImageWin_SeedMap[imgwin];

		lamda = _OptionPage->ui.lamda_lineEdit->text().toDouble();
		kappa = _OptionPage->ui.kappa_lineEdit->text().toDouble();

		if(lamda == 0.0 || kappa == 0.0)
		{
			QMessageBox msgBox;
			msgBox.setText("The input value is invalid.");
			msgBox.exec();
			return true;
		}

		if (!m_SeedsVector.size())
		{
			QMessageBox msgBox;
			msgBox.setText("Please draw some object and background seeds.");
			msgBox.exec();
			return true;
		}
	}

	imgwin->update();
	seeds.clear();
	return true;
}

bool Segmentation::procPaint(ImageWin* imgwin, QPaintEvent *event)
{
	int curmode = MainWindow::Instance().GetOperationModeManager()->mode();
	if (curmode != SEGMENTATIONMODE && curmode != ELEMENTPICKMODE)
		return false;

	if (ImageWin_ResultMap.find(imgwin)!=ImageWin_ResultMap.end())//!result.isNull()&&
	{
		this->showResult(imgwin,ImageWin_ResultMap[imgwin]);
		//qDebug()<<"123"<<imgwin->getPath();
	}

	if(curmode == ELEMENTPICKMODE)
		return false;

	if (ImageWin_SeedMap.find(imgwin)!=ImageWin_SeedMap.end())
	{
		//qDebug()<<"Draw"<<imgwin->getPath();
		this->DrawPoints(imgwin);
	}

	this->DrawPoint(imgwin);
	return true;
}
void Segmentation::DrawPoint(QPaintDevice * device)
{
	QPainter painter(device);
	painter.setRenderHint(QPainter::Antialiasing, true);

	ImageWin * current_win = (ImageWin *)device;
	scaleFactor = current_win->getscaleFactor();

	for (int ii = 0; ii < seeds.size(); ii++)//每一笔
	{
		SeedRecord &seed_record = seeds[ii];
		if (seed_record.type == ST_Point)
		{
			painter.setPen(QPen(seed_record.point.color, seed_record.point.line_width * scaleFactor/seed_record.point.scale, Qt::SolidLine, Qt::RoundCap));
			painter.drawPoint(seed_record.point.local_pos * scaleFactor/seed_record.point.scale);
			//qDebug()<<"paint";
		}
		else if (seed_record.type == ST_Rect)
		{
			painter.setBrush(QBrush(seed_record.rect.color, Qt::SolidPattern));
			painter.setPen(QPen(seed_record.rect.color, 1, Qt::SolidLine, Qt::RoundCap));
			QRect tmp_rect;
			tmp_rect.setTopLeft(seed_record.rect.local_rect.topLeft() * scaleFactor/seed_record.rect.scale);
			tmp_rect.setBottomRight(seed_record.rect.local_rect.bottomRight() * scaleFactor/seed_record.rect.scale);
			painter.drawRect(tmp_rect);
		}
	}

	QRect selection_region;
	if (in_selecting)
	{
		selection_region = QRect(selecting_start_pos*scaleFactor, selecting_end_pos*scaleFactor);
	}
	else
	{
		selection_region = imagewin_selection_rect[current_win];
		if (selection_region.isValid())
		{
			selection_region = QRect(selection_region.topLeft()*scaleFactor, selection_region.bottomRight()*scaleFactor).normalized();
		}
	}

	selection_region = selection_region.normalized();
	if (selection_region.isValid() && selection_region.width() > 1 && selection_region.height() > 1)
	{
		painter.setPen(QPen(Qt::white, 2, Qt::DotLine, Qt::RoundCap));
		painter.drawRect(selection_region);
	}
}

void Segmentation::DrawPoints(QPaintDevice * device)
{
	QPainter painter(device);
	painter.setRenderHint(QPainter::Antialiasing, true);

	ImageWin * current_win = (ImageWin *)device;
	scaleFactor = current_win->getscaleFactor();

	int m_SeedsIt = ImageWin_SeedItMap[current_win];

	vector<SeedRecords> &m_SeedsVector = ImageWin_SeedMap[current_win];

	std::vector<SeedRecords>::iterator it = m_SeedsVector.begin()+m_SeedsIt;

	for ( int i=m_SeedsIt; i < m_SeedsVector.size(); i++,it++)//所有笔画
	{
		SeedRecords &_seeds=*it;
		for (int ii = 0; ii < _seeds.size(); ii++)//每一笔
		{
			SeedRecord &seed_record = _seeds[ii];
			if (seed_record.type == ST_Point)
			{
				painter.setPen(QPen(seed_record.point.color, seed_record.point.line_width * scaleFactor/seed_record.point.scale, Qt::SolidLine, Qt::RoundCap));
				painter.drawPoint(seed_record.point.local_pos * scaleFactor/seed_record.point.scale);
			}
			else if (seed_record.type == ST_Rect)
			{
				painter.setBrush(QBrush(seed_record.rect.color, Qt::SolidPattern));
				painter.setPen(QPen(seed_record.rect.color, 1, Qt::SolidLine, Qt::RoundCap));
				QRect tmp_rect;
				tmp_rect.setTopLeft(seed_record.rect.local_rect.topLeft() * scaleFactor/seed_record.rect.scale);
				tmp_rect.setBottomRight(seed_record.rect.local_rect.bottomRight() * scaleFactor/seed_record.rect.scale);
				painter.drawRect(tmp_rect);
			}
		}
	}
}

void Segmentation::excuteSegmentation()
{
	ImageWin* current_imgWin = _window->GetActiveSubWindow();
	QImage &image = current_imgWin->getImage();
	int debug_a = image.width() * image.height();  
	int debug_b = _panel->ui.big_image_seg->currentIndex();
	int debug_c = imagewin_selection_rect[current_imgWin].isValid(); 
	int debug_d = imagewin_selection_rect[current_imgWin].width();
	if ((image.width() * image.height() > MAX_PIXEL_NUM) && 
		_panel->ui.big_image_seg->currentIndex() == 1 && 
		(!imagewin_selection_rect[current_imgWin].isValid() || imagewin_selection_rect[current_imgWin].width() < 10))
	{
		QMessageBox msgBox;
		msgBox.setText(tr("未选择分割区域，请重新选择"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	result = this->excute(current_imgWin);
	ImageWin_ResultMap[current_imgWin] = result;

	current_imgWin->update();
}

void Segmentation::UndoRecordPoints(ImageWin *imgwin)
{
	int m_SeedsIt = ImageWin_SeedItMap[imgwin];

	vector<SeedRecords> &m_SeedsVector = ImageWin_SeedMap[imgwin];

	if (m_SeedsIt!=m_SeedsVector.size()&&m_SeedsVector.begin()!=m_SeedsVector.end())
		m_SeedsIt++;
	ImageWin_SeedItMap[imgwin] = m_SeedsIt;
	qDebug()<<"undo"<<ImageWin_SeedItMap[imgwin];
}
void Segmentation::Reset()
{
	ImageWin* current_imgWin = _window->GetActiveSubWindow();

	//删除点集合，运算结果
	ImageWin_ResultMap.remove(current_imgWin);
	ImageWin_SeedMap.remove(current_imgWin);
	ImageWin_SeedItMap.remove(current_imgWin);
	imagewin_selection_rect[current_imgWin] = QRect();

	current_imgWin->update();
}

void opt_data_init(QImage _orig_image,QImage _mask_image,cv::RotatedRect _objRect,cv::RotatedRect _backgroundRect)
{
	opt_orig_image = _orig_image.copy();
	opt_backgroundRect = _backgroundRect;
	opt_objRect = _objRect;
	opt_objRect.points(opt_objRectPoints);
	opt_backgroundRect.points(opt_backgroundRectPoints);
	cv::Point2f fixRect[4];
	for (int i=0;i<4;i++)
	{
		int ii = opt_objRect.center.x < opt_objRectPoints[i].x;
		int jj = opt_objRect.center.y < opt_objRectPoints[i].y;
		if (ii == 0 && jj == 0)
		{
			fixRect[1] = opt_objRectPoints[i];
		}
		if (ii == 0 && jj == 1)
		{
			fixRect[0] = opt_objRectPoints[i];
		}
		if (ii == 1 && jj == 1)
		{
			fixRect[3] = opt_objRectPoints[i];
		}
		if (ii == 1 && jj == 0)
		{
			fixRect[2] = opt_objRectPoints[i];
		}
	}
	for (int i=0;i<4;i++)
	{
		opt_objRectPoints[i] = fixRect[i];
	}

	for (int i=0;i<4;i++)
	{
		int ii = opt_backgroundRect.center.x < opt_backgroundRectPoints[i].x;
		int jj = opt_backgroundRect.center.y < opt_backgroundRectPoints[i].y;
		if (ii == 0 && jj == 0)
		{
			fixRect[1] = opt_backgroundRectPoints[i];
		}
		if (ii == 0 && jj == 1)
		{
			fixRect[0] = opt_backgroundRectPoints[i];
		}
		if (ii == 1 && jj == 1)
		{
			fixRect[3] = opt_backgroundRectPoints[i];
		}
		if (ii == 1 && jj == 0)
		{
			fixRect[2] = opt_backgroundRectPoints[i];
		}
	}
	for (int i=0;i<4;i++)
	{
		opt_backgroundRectPoints[i] = fixRect[i];
	}
	opt_l_high = std::min(opt_backgroundRectPoints[0].y,opt_backgroundRectPoints[3].y);
	opt_l_low = std::max(opt_backgroundRectPoints[1].y,opt_backgroundRectPoints[2].y);
	x_offset = 3;
	y_offset = 1;
}

void set_x_offset(int x_offset){
	if (x_offset==0)
	{
		return;
	}
	x_offset = abs(x_offset);
}
void set_y_offset(int y_offset){
	if (y_offset==0)
	{
		return;
	}
	y_offset = abs(y_offset);
}
double getGMMFeature(double angle,cv::Point2f p){
	double res = 0;
	double avg = 0;
	no_k = fabs(angle-90)<1e-4;
	double tmp1;
	double tmp2;
	double cnt_f=0;
	double cnt_b=0;
	int x;
	int cnt;
	double k2,b2,k,b;
	QImage testimg = opt_orig_image.copy();
	int gmm_x_offset=1;
	if(!no_k)
	{
		cnt=0;
		k = tan(angle *PI /180.00);
		b = p.y-k*p.x;
		for (int i = opt_x_low;i<=opt_x_high;i++)
		{	
			int y = k*(i+gmm_x_offset)+b;
			if(y > opt_l_high || y < opt_l_low)
				continue;
			double *val = new double(qGray(opt_orig_image.pixel(i+gmm_x_offset,k*(i+gmm_x_offset)+b)));
			testimg.setPixel(i+gmm_x_offset,y,255);
			tmp1 = fgmm->GetProbability(val);
			tmp2 = bgmm->GetProbability(val);
			//qDebug()<<tmp1<<" "<<tmp2;
			cnt_f += tmp1>=tmp2;
			cnt_b += tmp1<tmp2;
			cnt++;
		}
		cnt += cnt==0;
		if (isright)
		{
			res += cnt_b/cnt;
		}
		else
		{
			res += cnt_f/cnt;
		}
		cnt = 0;
		for (int i = opt_x_low;i<=opt_x_high;i++)
		{	
			
			int y = k*(i-gmm_x_offset)+b;
			if(y > opt_l_high || y < opt_l_low)
				continue;
			double *val = new double(qGray(opt_orig_image.pixel(i-gmm_x_offset,k*(i-gmm_x_offset)+b)));
			testimg.setPixel(i-gmm_x_offset,y,255);
			tmp1 = fgmm->GetProbability(val);
			tmp2 = bgmm->GetProbability(val);
			//qDebug()<<tmp1<<" "<<tmp2;
			cnt_f += tmp1>=tmp2;
			cnt_b += tmp1<tmp2;
			cnt++;
		}
		cnt += cnt==0;
		if (isright)
		{
			res += cnt_f/cnt;
		}
		else
		{
			res += cnt_b/cnt;
		}
	}
	else{
		x=p.x;
		for (int h = opt_l_low;h<=opt_l_high;h++)
		{	
			double *val = new double(qGray(opt_orig_image.pixel(p.x+gmm_x_offset,h)));
			testimg.setPixel(x,h,255);
			tmp1 = fgmm->GetProbability(val);
			tmp2 = bgmm->GetProbability(val);
			//qDebug()<<tmp1<<" "<<tmp2;
			cnt_f += tmp1>=tmp2;
			cnt_b += tmp1<tmp2;
		}
		if (isright)
		{
			res += cnt_b/(opt_l_high-opt_l_low);
		}
		else
		{
			res += cnt_f/(opt_l_high-opt_l_low);
		}
		for (int h = opt_l_low;h<=opt_l_high;h++)
		{	
			double *val = new double(qGray(opt_orig_image.pixel(p.x-gmm_x_offset,h)));
			testimg.setPixel(x,h,255);
			tmp1 = fgmm->GetProbability(val);
			tmp2 = bgmm->GetProbability(val);
			//qDebug()<<tmp1<<" "<<tmp2;
			cnt_f += tmp1>=tmp2;
			cnt_b += tmp1<tmp2;
		}
		if (isright)
		{
			res += cnt_f/(opt_l_high-opt_l_low);
		}
		else
		{
			res += cnt_b/(opt_l_high-opt_l_low);
		}
	}
	//testimg.save("tmp.bmp","BMP");
	//qDebug()<<res;
	return res;
}
double getBlockFeature(double angle,cv::Point2f p){
	double res = 0;
	double avg = 0;
	no_k = fabs(angle-90)<1e-4;
	QColor tmp1;
	QColor tmp2;
	int x;
	double k2,b2,k,b;
	if(!no_k)
	{
		k = tan(angle *PI /180.00);
		b = p.y-k*p.x;
		if (k!=0.0)
		{
			k2 = -1.0/k;
			b2 = p.y-k2*p.x;
		}
	}
	for (int h = opt_l_low;h<=opt_l_high;h++)
	{	
		if(!no_k)
		{
			x = (h-b)/k;
			if (k!=0.0)
			{
				b2 = h-k2*x;
			}
		}
		else{
			x=p.x;
		}
		for (int j=1;j<=x_offset;j++)
		{
			if (no_k)
			{
				tmp1 = QColor::fromRgb(opt_orig_image.pixel(x+j,h));
				//opt_orig_image.setPixel(x+j,h,255);
				tmp2 = QColor::fromRgb(opt_orig_image.pixel(x-j,h));
			//	opt_orig_image.setPixel(x-j,h,255);
			}
			else{
				if(x+j > opt_x_high || x+j < opt_x_low || k2*(x+j)+b2 > opt_l_high || k2*(x+j)+b2 < opt_l_low) 
					continue;
				if(x-j > opt_x_high || x-j < opt_x_low || k2*(x-j)+b2 > opt_l_high || k2*(x-j)+b2 < opt_l_low)
					continue;
				tmp1 = QColor::fromRgba(opt_orig_image.pixel(x+j,k2*(x+j)+b2));
				
				//opt_orig_image.setPixel(x+j,k2*(x+j)+b2,255);
				tmp2 = QColor::fromRgba(opt_orig_image.pixel(x-j,k2*(x-j)+b2));
				//opt_orig_image.setPixel(x-j,k2*(x-j)+b2,255);
			}
			/*	res += abs(tmp2.red() - tmp1.red());
			res += abs(tmp2.green() - tmp1.green());
			res += abs(tmp2.blue() - tmp1.blue());*/
			res += abs(qGray(tmp1.rgb())-qGray(tmp2.rgb()));
			
		}
		avg += res/(opt_l_high-opt_l_low);

	}
	//opt_orig_image.save("test.bmp","bmp");
	return avg;
}
double getSobelFeature(double angle,cv::Point2f p){
	double res = 0;
	double avg = 0;
	no_k = fabs(angle-90)<1e-4;
	QColor tmp1;
	int x;
	double k2,b2,k,b;
	if(!no_k)
	{
		k = tan(angle *PI /180.00);
		b = p.y-k*p.x;
		if (k!=0.0)
		{
			k2 = -1.0/k;
			b2 = p.y-k2*p.x;
		}
	}
	for (int h = opt_l_low;h<=opt_l_high;h++)
	{	
		if(!no_k)
		{
			x = (h-b)/k;
			if (k!=0.0)
			{
				b2 = h-k2*x;
			}
		}
		else{
			x=p.x;
		}
		if(x > opt_x_high || x < opt_x_low) 
			continue;
		tmp1 = QColor::fromRgba(sobel_image.pixel(x,h));
		avg += tmp1.red()/(opt_l_high-opt_l_low);

	}
	return avg;
}

double getLaplaceFeature(double angle,cv::Point2f p){
	double res = 0;
	double avg = 0;
	no_k = fabs(angle-90)<1e-4;
	QColor tmp1;
	int x;
	double k2,b2,k,b;
	if(!no_k)
	{
		k = tan(angle *PI /180.00);
		b = p.y-k*p.x;
		if (k!=0.0)
		{
			k2 = -1.0/k;
			b2 = p.y-k2*p.x;
		}
	}
	for (int h = opt_l_low;h<=opt_l_high;h++)
	{	
		if(!no_k)
		{
			if (k!=0.0)
			{
				x = (h-b)/k;
				b2 = h-k2*x;
			}
			else{
				continue;
			}
		}
		else{
			x=p.x;
		}
		if(x > opt_x_high || x < opt_x_low) 
			continue;
		tmp1 = QColor::fromRgba(laplace_image.pixel(x,h));
		avg += tmp1.red()/(opt_l_high-opt_l_low);

	}
	return avg;
}
double myfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
{

	double angle  = x[0];
	cv::Point2f p(x[1],x[2]);
	double res =0;
	switch (use_feature)
	{
		case 0:{res = getBlockFeature(angle,p);break;}
		case 1:{res = getSobelFeature(angle,p);break;}
		case 2:{res = getLaplaceFeature(angle,p);break;}
	}
	if (use_gmm)
	{
		res *= getGMMFeature(angle,p);
	}
	return res;
}
void optimization_phase1(int tag){
	isright = tag;
	nlopt::opt opt(nlopt::LN_COBYLA, 3);
	std::vector<double> lb(3);
	std::vector<double> ub(3);
	lb[0]=45;
	ub[0]=135;
	if(tag==0){
	ub[1]=(double)std::min(opt_objRectPoints[0].x,opt_objRectPoints[1].x);
	lb[1]=(double)std::max(opt_backgroundRectPoints[0].x,opt_backgroundRectPoints[1].x);
	}
	else if(tag == 1){
		lb[1]=(double)std::min(opt_objRectPoints[2].x,opt_objRectPoints[3].x);
		ub[1]=(double)std::max(opt_backgroundRectPoints[2].x,opt_backgroundRectPoints[3].x);
	}
	lb[2]=opt_l_low;
	ub[2]=opt_l_high;
	opt.set_lower_bounds(lb);
	opt.set_upper_bounds(ub);
	opt.set_xtol_rel(1e-4);
	opt.set_max_objective(myfunc, NULL);
	double maxf = 0;
	opt_x_high =ub[1];
	opt_x_low = lb[1];
	init[0]=45;
	init[1]=(lb[1]+ub[1])/2;
	init[2]=(lb[2]+ub[2])/2;
	qDebug()<<myfunc(init,init,NULL);
	qDebug()<<init[0]<<"\t"<<init[1]<<"\t"<<init[2]<<endl;
	nlopt::result result = opt.optimize(init,maxf);
	qDebug()<<maxf;
	qDebug()<<init[0]<<"\t"<<init[1]<<"\t"<<init[2]<<endl;
	if (use_gmm){
		double angle  = init[0];
		cv::Point2f p(init[1],init[2]);
		qDebug()<<getGMMFeature(angle,p);
	}

}

