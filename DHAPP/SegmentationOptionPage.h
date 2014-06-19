#ifndef SEGMENTATIONOPTIONPANEL_H
#define SEGMENTATIONOPTIONPANEL_H

#include <QWidget>
#include "ui_SegmentationOptionPage.h"
#include "Segmentation\SegmentInterface.h"
#include "OptionPageManager.h"
using namespace NMH;

class SegmentationOptionPage : public QWidget
{
	Q_OBJECT

public:
	SegmentationOptionPage(QWidget *parent = 0);
	~SegmentationOptionPage();
	Ui::SegmentationOptionPage ui;
	const GrabCutParam * GetGrabCutParam() const;
	const MixtureParam * GetMixtureParam() const;
	const GraphCutParam * GetGraphCutParam() const;
	const LazyBrushParam * GetLazyBrushParam() const;
	const QColor GetObjcolor() const{return obj_color;};
	const QColor GetBkgcolor() const{return bkg_color;};

private:
	QColor obj_color;
	QColor bkg_color;
	QPushButton *chose_obj_color_button;
	QPushButton *chose_bkg_color_button;
	QLineEdit *lamda_edit;
	QLineEdit *kappa_edit;
	NMH::GrabCutParam _GrabCutParam;
	NMH::MixtureParam _MixtureParam;
	NMH::GraphCutParam _GraphCutParam;
	NMH::LazyBrushParam _LazyBrushParam;
	OptionPageManager * _optionManager;

	void init_component();
	void init_connect();

private slots:
	void segment_method_changed(const QString & method);
	void canny_chk_changed(int state);
	void use_canny_or_log();
	void GetSettings();
	void SetSettings();
	void chose_object_color();
	void chose_bkg_color();
};

#endif // SEGMENTATIONOPTIONPANEL_H
