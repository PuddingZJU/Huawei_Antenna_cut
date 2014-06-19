#include "SegmentationOptionPage.h"

SegmentationOptionPage::SegmentationOptionPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_optionManager = dynamic_cast<OptionPageManager *>(this->parentWidget());
	init_component();
	init_connect();
	GetSettings();
}
void SegmentationOptionPage::init_component()
{

	chose_obj_color_button = ui.ObjColorBt;
	chose_bkg_color_button = ui.BkgColorBt;
	chose_obj_color_button->setAutoFillBackground(true);
	chose_bkg_color_button->setAutoFillBackground(true);

	obj_color = Qt::yellow;
	bkg_color = Qt::red;

	lamda_edit = ui.lamda_lineEdit;
	kappa_edit = ui.kappa_lineEdit;

	QValidator *validator = new QDoubleValidator(0.0, 10.0, 10, this);
	lamda_edit->setValidator( validator );
	lamda_edit->setToolTip(QString::fromLocal8Bit("Data Size"));
	kappa_edit->setValidator( validator );
	kappa_edit->setToolTip(QString::fromLocal8Bit("Smooth Size"));
	QValidator *validator2 = new QIntValidator(20, 200, this);
     	ui.canny_min_edit->setValidator(validator2);
		ui.canny_max_edit->setValidator(validator2);
	// segment method
	ui.stackedWidget->hide();
	ui.CannyGroupBox->hide();

}
void SegmentationOptionPage::init_connect()
{
	QObject:: connect(chose_obj_color_button, SIGNAL(clicked()), this, SLOT(chose_object_color()));
	QObject:: connect(chose_bkg_color_button, SIGNAL(clicked()), this, SLOT(chose_bkg_color()));

	QObject::connect(ui.MethodComboBox, SIGNAL(currentIndexChanged (const QString & )), this, SLOT(segment_method_changed(const QString &)));
	QObject:: connect(ui.canny_checkbox, SIGNAL(stateChanged(int )), this, SLOT(canny_chk_changed(int)));
	QObject:: connect(ui.use_canny_input, SIGNAL(toggled(bool)), this, SLOT(use_canny_or_log()));
	QObject:: connect(ui.use_log_input, SIGNAL(toggled(bool)), this, SLOT(use_canny_or_log()));
	QObject::connect(_optionManager,SIGNAL(OK()),this,SLOT(GetSettings()));
	QObject::connect(_optionManager,SIGNAL(Cancel()),this,SLOT(SetSettings()));
}
void SegmentationOptionPage::GetSettings()
{

	_LazyBrushParam.sd_param.lamda = ui.lamda_lineEdit->text().toDouble();
	_LazyBrushParam.sd_param.kappa = ui.kappa_lineEdit->text().toDouble();

	_LazyBrushParam.canny_param.canny_min = ui.canny_min_edit->text().toInt();
	_LazyBrushParam.canny_param.canny_max = ui.canny_max_edit->text().toInt();

	_LazyBrushParam.use_canny_input = ui.use_canny_input->isChecked();
	_LazyBrushParam.use_canny_enhance = ui.canny_checkbox->isChecked();
	_LazyBrushParam.canny_erode = ui.canny_erode_checkbox->isChecked();

	
	//eye
	_GrabCutParam.sd_param.lamda = ui.lamda_lineEdit->text().toDouble();
	_GrabCutParam.sd_param.kappa = ui.kappa_lineEdit->text().toDouble();

	_GrabCutParam.canny_param.canny_min = ui.canny_min_edit->text().toInt();
	_GrabCutParam.canny_param.canny_max = ui.canny_max_edit->text().toInt();

	
	_GraphCutParam.sd_param.lamda = ui.lamda_lineEdit->text().toDouble();
	_GraphCutParam.sd_param.kappa = ui.kappa_lineEdit->text().toDouble();

	
	_MixtureParam.sd_param.lamda = ui.lamda_lineEdit->text().toDouble();
	_MixtureParam.sd_param.kappa = ui.kappa_lineEdit->text().toDouble();

	_MixtureParam.canny_param.canny_min = ui.canny_min_edit->text().toInt();
	_MixtureParam.canny_param.canny_max = ui.canny_max_edit->text().toInt();

	_MixtureParam.edge_weight = ui.edge_weight_lineEdit->text().toDouble();
	if (ui.linear_radioButton->isChecked())
	{
		_MixtureParam.mixture_type = MT_Linear;
	}
	else if (ui.comboBox->currentIndex() == 0)
	{
		_MixtureParam.mixture_type = MT_Squared;
	}
	else if (ui.comboBox->currentIndex() == 1)
	{
		_MixtureParam.mixture_type = MT_Cubical;
	}
}
void SegmentationOptionPage::SetSettings()
{
	ui.lamda_lineEdit->setText(QString::number(_LazyBrushParam.sd_param.lamda));
	ui.kappa_lineEdit->setText(QString::number(_LazyBrushParam.sd_param.kappa));
	ui.canny_min_edit->setText(QString::number(_LazyBrushParam.canny_param.canny_min));
	ui.canny_max_edit->setText(QString::number(_LazyBrushParam.canny_param.canny_max));
	bool use_canny = _LazyBrushParam.use_canny_input;
	ui.use_log_input->setChecked(!use_canny);
	ui.use_canny_input->setChecked(use_canny);
	ui.canny_checkbox->setChecked(_LazyBrushParam.use_canny_enhance);
	ui.canny_erode_checkbox->setChecked(_LazyBrushParam.canny_erode);
	ui.edge_weight_lineEdit->setText(QString::number(_MixtureParam.edge_weight));
	switch (_MixtureParam.mixture_type)
	{
	case MT_Linear:
		ui.linear_radioButton->setChecked(true);
		break;
	case MT_Squared:
		ui.comboBox->setCurrentIndex(0);
		break;
	case MT_Cubical:
		ui.comboBox->setCurrentIndex(1);
		break;
	default:
		break;
	}
}
SegmentationOptionPage::~SegmentationOptionPage()
{

}
void SegmentationOptionPage::segment_method_changed(const QString & method)
{
	if (method == "GraphCut")
	{
		ui.stackedWidget->hide();
		ui.CannyGroupBox->hide();
	}
	else if (method == "GrabCut")
	{
		ui.stackedWidget->hide();
		ui.CannyGroupBox->show();
	}
	else if (method == "LazyBrush")
	{
		ui.stackedWidget->setCurrentIndex(0);
		ui.stackedWidget->show();
		ui.CannyGroupBox->show();
	}
	else if (method == "Mixture")
	{
		ui.stackedWidget->setCurrentIndex(1);
		ui.stackedWidget->show();
		ui.CannyGroupBox->show();
	}
}
void SegmentationOptionPage::canny_chk_changed(int state)
{
	if (state == Qt::Checked) ui.canny_erode_checkbox->setEnabled(true);
	else ui.canny_erode_checkbox->setEnabled(false);
}

void SegmentationOptionPage::use_canny_or_log()
{
	if (ui.use_canny_input->isChecked())
	{
		ui.canny_checkbox->setChecked(true);
	}
	else
	{
		ui.canny_checkbox->setChecked(false);
	}
}
const GrabCutParam * SegmentationOptionPage:: GetGrabCutParam() const
{
	return &_GrabCutParam;
}
const MixtureParam * SegmentationOptionPage:: GetMixtureParam() const
{
	return &_MixtureParam;
}
const GraphCutParam * SegmentationOptionPage:: GetGraphCutParam() const
{
	return &_GraphCutParam;
}
const LazyBrushParam * SegmentationOptionPage:: GetLazyBrushParam() const
{
	return &_LazyBrushParam;
}
void SegmentationOptionPage::chose_object_color()
{
	obj_color = QColorDialog::getColor(obj_color);
	int red = obj_color.red();
	int green = obj_color.green();
	int blue = obj_color.blue();
	QString string = QString("color:red;background-color:rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
	chose_obj_color_button->setStyleSheet(string);
}

void SegmentationOptionPage::chose_bkg_color()
{
	bkg_color = QColorDialog::getColor(bkg_color);
	int red = bkg_color.red();
	int green = bkg_color.green();
	int blue = bkg_color.blue();
	QString string = QString("color:red;background-color:rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
	chose_bkg_color_button->setStyleSheet(string);
}