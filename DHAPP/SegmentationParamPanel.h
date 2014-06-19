#ifndef __ES_DHAPP_SEGMENTPARAMPANEL_H
#define __ES_DHAPP_SEGMENTPARAMPANEL_H

#include <QWidget>
#include "ui_SegmentationParamPanel.h"



class SegmentationParamPanel : 
	public QWidget
{
	Q_OBJECT


public:
	SegmentationParamPanel(QWidget *parent = 0);
	~SegmentationParamPanel(void);

public:
	Ui::SegmentationParamPanel ui;
};

#endif 
