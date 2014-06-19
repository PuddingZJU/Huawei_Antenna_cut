#include "SegmentationParamPanel.h"

SegmentationParamPanel::SegmentationParamPanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("\
						QCheckBox#paint_obj_checkBox::indicator,#paint_bkg_checkBox::indicator{\
						image:url(:/hello4/icon/64.png);\
						width: 15px;\
						height: 15px;\
						}\
						QCheckBox#paint_obj_checkBox::indicator:checked,#paint_bkg_checkBox::indicator:checked{\
						background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
											stop: 0 rgba(0,0,0,100),   stop: 0.3 rgba(255,255,255,100),  \
											stop: 0.5 rgba(255,255,255,100),  stop: 0.7 rgba(255,255,255,100),\
											stop: 1.0 rgba(0,0,0,100));\
						border:0.5px solid gray;\
						border-radius:1px;\
						padding: 1px;\
						}\
						QCheckBox#paint_obj_checkBox::indicator:checked:hover,#paint_bkg_checkBox::indicator:checked:hover{\
						background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
											stop: 0 rgba(88,131,230,255),   stop: 0.3 rgba(255,255,255,100),  \
											stop: 0.5 rgba(255,255,255,100),  stop: 0.7 rgba(255,255,255,100),\
											stop: 1.0 rgba(88,131,230,255));\
						border:0.5px solid gray;\
						border-radius:1px;\
						padding: 1px;\
						}\
						QCheckBox#paint_obj_checkBox::indicator:hover,#paint_bkg_checkBox::indicator:hover{\
						background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
											stop: 0 rgba(88,182,230,100),  stop: 0.3 rgba(255,255,255,100),  \
											stop: 0.5 rgba(255,255,255,100), stop: 0.7 rgba(255,255,255,100),\
											stop: 1.0 rgba(88,182,230,100));\
						border:0.5px solid rgba(88,182,230,100);\
						border-radius:1px;\
						}\
						");

}

SegmentationParamPanel::~SegmentationParamPanel(void)
{

}
