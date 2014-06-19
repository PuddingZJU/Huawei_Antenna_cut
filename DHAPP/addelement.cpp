#include "addelement.h"
#include "ui_AddElement.h"
#include "elementquery.h"
#include "imagematch.h"
#include "MainWindow.h"
#include <time.h>
#include <QVector>

AddElement::AddElement(QWidget *parent) :
QDialog(parent),ui(new Ui::AddElementClass)
{
	ui->setupUi(this);
	connect(ui->pushButton_reIndexing,SIGNAL(clicked()),this,SLOT(doReIndexing()));
	connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));
	maxLevel=1;
	levelElementMaxNum=10;
}


AddElement::~AddElement()
{

}


void AddElement::doReIndexing()
{
	if (!g_matlabInitialized)
	{
		MCRInitialize();
		g_matlabInitialized=true;
	}
	QVector<FeatureType> featureTypes;
	if (ui->checkBox_colorMoment->isChecked())
	{
		featureTypes.push_back(FEATURE_TYPE_COLOR_MOMENT);
	}
	if (ui->checkBox_colorHist->isChecked())
	{
		featureTypes.push_back(FEATURE_TYPE_COLOR_HIST);
	}
	if (ui->checkBox_GLCM->isChecked())
	{
		featureTypes.push_back(FEATURE_TYPE_TEXTURE_GLCM);
	}
	if (ui->checkBox_shapeContext->isChecked())
	{
		featureTypes.push_back(FEATURE_TYPE_SHAPE_CONTEXT);
	}
	for (int i=0;i<featureTypes.size();i++)
	{
		reIndexing(g_datasetName.toLatin1().data(),featureTypes[i],maxLevel,levelElementMaxNum);
	}
	updateUnIndexedNumber();
}




void AddElement::updateUnIndexedNumber()
{	

	ui->label_colorMoment->setText(QString::number(getUnindexedNumber(g_datasetName.toLatin1().data(),FEATURE_TYPE_COLOR_MOMENT))
									+tr("元素未参与索引"));
	ui->label_colorHist->setText(QString::number(getUnindexedNumber(g_datasetName.toLatin1().data(),FEATURE_TYPE_COLOR_HIST))
									+tr("元素未参与索引"));
	ui->label_GLCM->setText(QString::number(getUnindexedNumber(g_datasetName.toLatin1().data(),FEATURE_TYPE_TEXTURE_GLCM))
									+tr("元素未参与索引"));
	ui->label_shapeContext->setText(QString::number(getUnindexedNumber(g_datasetName.toLatin1().data(),FEATURE_TYPE_SHAPE_CONTEXT))
									+tr("元素未参与索引"));
}


void AddElement::addNewElement(QImage element,QImage elementMask)
{
	if (!g_matlabInitialized)
	{
		MCRInitialize();
		g_matlabInitialized=true;
	}

	//注意添加'/'或'\\'或使用绝对路径  
	//根据时间确定文件名，避免数据库文件名重复带来的问题
	time_t rawtime; 
	struct tm * timeinfo; 
	time ( &rawtime ); 
	timeinfo = localtime ( &rawtime );
	QString timestr;
	timestr=QString::number(timeinfo->tm_year)+
		QString::number(timeinfo->tm_yday)+
		QString::number(timeinfo->tm_hour)+
		QString::number(timeinfo->tm_min)+
		QString::number(timeinfo->tm_sec);

	QString elementPath="./new_"+timestr+".jpg";	
	QString elementMaskPath=elementPath+".png";

	element.save(elementPath,"jpg");
	elementMask.save(elementMaskPath,"png");
	addElementToDB(g_datasetName.toLatin1().data(),elementPath.toLatin1().data());
	addElementMaskToDB(g_datasetName.toLatin1().data(),elementMaskPath.toLatin1().data());
	updateUnIndexedNumber();

	QMessageBox addDoneMsgBox;
	addDoneMsgBox.setText(tr("添加完毕"));
	return;
	
}