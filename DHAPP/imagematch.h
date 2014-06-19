#ifndef IMAGEMATCH_H
#define IMAGEMATCH_H


//#include "elementquery.h"
#include <QMainWindow>
#include <QFileInfo>
#include<QTableWidgetItem>
#include <QThread>   

#include <QProgressBar>   
//#include <QPushButton>  

#include <QtCore>  
#include <QtGui>
//#include <iostream>
//using namespace std;

namespace Ui {
class ImageMatchClass;
}

extern QString g_datasetName;
extern bool g_matlabInitialized;

class WaitingDialog : public QDialog  
{  
	Q_OBJECT  
private:  
	int m_CurrentValue;           
	int m_UpdateInterval;        
	int m_MaxValue;               
	QTimer m_Timer;  
	QProgressBar *m_ProgressBar; 
	QLabel *m_LabelProgressInfo;
public:  
	WaitingDialog(QWidget *parent = 0);  
	~WaitingDialog();  
	void UpdateProgressValue(double percent);
	void UpdateProgressInfo(QString info);
	void Start(int interval=100, int maxValue=100);  
	void Stop();  
private slots:  
	void UpdateSlot();  	

};  



class QueryThread : public QThread   
{   
	Q_OBJECT   
signals:   
	void notify(QStringList resultList);
  
public:   
	QueryThread(QObject *parent = 0);  
	~QueryThread();
	//C:\SDK\opencv\build\x86\vc10\binvoid doQuery(QString queryImagePath,QString queryImageMaskPath,const std::vector<FeatureType>& featureTypes,ElementType elementType,int resultCount);   
protected:   
	void run();
private:
	QString _queryImagePath;
	QString _queryImageMaskPath;
//	std::vector<FeatureType> _featureTypes;
//	ElementType _elementType;
	int _resultCount;
};   

class ImageMatch : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ImageMatch(QWidget *parent = 0);
    ~ImageMatch();
	QList<QString> getResultList();
	void setTargetImage(QImage tgtImg);

signals:
	void resultExported(QList<QString> imagePaths);

private slots:
    void loadTargetImage();
	void loadTargetImageMask();
    void startSearchImage();
    void tableCellClicked(int row, int column );
    void selectAll(bool);
    void exportResult();

    void colorFeatureEnable();
    void textureFeatureEnable();
    void shapeFeatureEnable();
    void elementFeatureEnable();

	void OnNotify(QStringList);
	

private:
    Ui::ImageMatchClass *ui;

    QImage *targetImage;
	QImage *targetImageMask;
    QImage *resultImage;

    /*存储面板上的参数*/
    bool  colorFeature;
    bool  textureFeature;
    bool  shapeFeature;
    bool  elementFeature;

    QString colorItem;
    QString textureItem;
    QString shapeItem;
    QString elementItem;
    /*******************/

    //QList<QFileInfo>  listInfo; 

    QList<QString> list;  //存储检索结果的图片路径

    QList<QString> resultList; //选中的图片路径信息
    void getResult();
    void initTableWidget();

private:
	WaitingDialog *queryProgressDialog;   
	//QueryThread* query;
};





#endif // IMAGEMATCH_H
