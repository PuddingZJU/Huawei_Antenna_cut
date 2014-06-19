#include "MainWindow.h"
#include "ImageListProjectFileHandle.h"
#include "processControl.h"
#include "imagematch.h"
#include "resulttableitem.h"
#include "ui_imagematch.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QTableWidgetItem>
#include <vector>
#include <string>


QString g_datasetName="DUNGHUANG.IMAGE.61KU_TEST";
bool g_matlabInitialized=false;

WaitingDialog::WaitingDialog(QWidget *parent)  
{  
	m_ProgressBar = new QProgressBar(this);   
	m_LabelProgressInfo = new QLabel(this);

	m_CurrentValue=0;
	m_MaxValue=100;
	m_UpdateInterval=0;
	m_ProgressBar->setRange(0, m_MaxValue);  
	m_ProgressBar->setValue(0); 
	 
	m_ProgressBar->setTextVisible(false);  
	m_LabelProgressInfo->setText("");
	QVBoxLayout *layout = new QVBoxLayout;  
	layout->addWidget(m_ProgressBar); 
	layout->addWidget(m_LabelProgressInfo);

	setLayout(layout);  

	connect(&m_Timer, SIGNAL(timeout()), this, SLOT(UpdateSlot())); 
}  

WaitingDialog::~WaitingDialog()  
{  
}  

void WaitingDialog::UpdateSlot()  
{  
	m_CurrentValue++;  
	if( m_CurrentValue == m_MaxValue )  
		m_CurrentValue = 0;  
	m_ProgressBar->setValue(m_CurrentValue);  
}  
void WaitingDialog::UpdateProgressValue(double percent)  
{   
	m_ProgressBar->setValue(m_MaxValue*percent);  
}  

void WaitingDialog::UpdateProgressInfo(QString info)
{
	m_LabelProgressInfo->setText(info);
}


void WaitingDialog::Start(int interval/* =100 */, int maxValue/* =100 */)  
{  
	m_UpdateInterval = interval;  
	m_MaxValue = maxValue;  
	m_Timer.start(m_UpdateInterval);  
	m_ProgressBar->setRange(0, m_MaxValue);  
	m_ProgressBar->setValue(0);  
}  

void WaitingDialog::Stop()  
{  
	m_Timer.stop();  
}  

QueryThread::QueryThread(QObject *parent): QThread(parent) 
{
	_resultCount=0;
	_elementType=IMAGE_ELEMENT_TYPE_ALL;
}
QueryThread::~QueryThread()
{
}

void QueryThread::doQuery(QString queryImagePath,QString queryImageMaskPath,const std::vector<FeatureType>& featureTypes,ElementType elementType,int resultCount)
{ 
	_queryImagePath=queryImagePath;
	_queryImageMaskPath=queryImageMaskPath;
	_featureTypes=featureTypes;
	_elementType=elementType;
	_resultCount=resultCount;
	start(HighestPriority);   
}



void QueryThread::run()
{
	QStringList result;

	//StringArray queryResults;
	FeatureType* fTypes=new FeatureType[_featureTypes.size()];
	for (int i=0;i<_featureTypes.size();i++)
	{
		fTypes[i]=_featureTypes[i];
	}	
	char** queryResults;
	queryResults=new char*[_resultCount];
	for (int i=0;i<_resultCount;i++)
	{
		queryResults[i]=new char[1000];
	}

	//time_t start=clock();

	int n=runQuery(g_datasetName.toLatin1().data(),
		_queryImagePath.toLatin1().data(),
		fTypes,
		_featureTypes.size(),
		_elementType,
		queryResults,
		_resultCount,
		_queryImageMaskPath.toLatin1().data());

	//time_t end=clock();
	//printf("time:%d\n",end-start);

	for (int i=0;i<n;i++)
	{
		result.append(QString(queryResults[i]));
	}

	delete fTypes;

	emit notify(result);
}


ImageMatch::ImageMatch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageMatchClass)
{
    ui->setupUi(this);
    /*初始复选框都为未选中*/
    ui->checkBox_allSelected->setCheckState(Qt::Unchecked);
    ui->checkBox_allSelected->setEnabled(false);
    ui->checkBox->setCheckState(Qt::Unchecked);
    ui->checkBox_2->setCheckState(Qt::Unchecked);
    ui->checkBox_3->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);

    ui->tableWidget_result->verticalHeader()->setVisible(false);
    ui->tableWidget_result->horizontalHeader()->setVisible(false);
    ui->tableWidget_result->setShowGrid(false);

    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->comboBox_5->setEnabled(false);

    ui->pushButton_exportResult->setEnabled(false);


	connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(colorFeatureEnable()));
    connect(ui->checkBox_2,SIGNAL(clicked()),this,SLOT(textureFeatureEnable()));
    connect(ui->checkBox_3,SIGNAL(clicked()),this,SLOT(shapeFeatureEnable()));
    connect(ui->checkBox_4,SIGNAL(clicked()),this,SLOT(elementFeatureEnable()));

//  connect(ui->pushButton_loadTargetImage,SIGNAL(clicked()),this,SLOT(loadTargetImage()));
//	connect(ui->pushButton_loadTargetMask,SIGNAL(clicked()),this,SLOT(loadTargetImageMask()));
    connect(ui->pushButton_startSearch,SIGNAL(clicked()),this,SLOT(startSearchImage()));
    connect(ui->tableWidget_result,SIGNAL(cellClicked(int,int)),this,SLOT(tableCellClicked(int,int)));

    connect(ui->checkBox_allSelected,SIGNAL(clicked(bool)),this,SLOT(selectAll(bool)));
    connect(ui->pushButton_exportResult,SIGNAL(clicked()),this,SLOT(exportResult()));

    colorFeature=false;
    textureFeature=false;
    shapeFeature=false;
    elementFeature=false;

	targetImage=NULL;
	targetImageMask=NULL;
	query=NULL;

}

ImageMatch::~ImageMatch()
{
    delete ui;
}


void ImageMatch::setTargetImage(QImage tgtImg)
{
	if (targetImage)
	{
		delete targetImage;
	}
	targetImage=new QImage(tgtImg);
}

QList<QString> ImageMatch::getResultList()
{
	return resultList;
}


/* 加载目标图片 */
void ImageMatch::loadTargetImage()
{
	if (targetImage!=NULL)
	{
		delete targetImage;
	}
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",
                                                        tr("Image Files(*.png *.jpg *.jpeg *.bmp *.txt)"));
    if(fileName.isEmpty()){return;}

    targetImage = new QImage();

        if(!(targetImage->load(fileName)))
        {
            QString errorMessage = "The file: \""+ fileName + "\" is not a image file!";
            QMessageBox::information(NULL,
                                     "FileError",
                                     errorMessage
                                     );

            delete targetImage;
            return;
        }
		else
		{
			if (targetImageMask!=NULL)
			{
				delete targetImageMask;
				targetImageMask=NULL;
			}
		}

        //QImage img = targetImage->scaled(ui->label_targetImage->size(),Qt::KeepAspectRatio);
        //ui->label_targetImage->setAlignment(Qt::AlignCenter);
        //ui->label_targetImage->setPixmap(QPixmap::fromImage(img));
}

/* 加载目标图片掩码 */
void ImageMatch::loadTargetImageMask()
{
	if (targetImageMask!=NULL)
	{
		delete targetImageMask;
	}
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",
		tr("Image Files(*.png *.jpg *.jpeg *.bmp *.txt)"));
	if(fileName.isEmpty()){return;}

	targetImageMask = new QImage();

	if(!(targetImageMask->load(fileName)))
	{
		QString errorMessage = "The file: \""+ fileName + "\" is not a image file!";
		QMessageBox::information(NULL,
			"FileError",
			errorMessage
			);

		delete targetImageMask;
		return;
	}
}


void ImageMatch::OnNotify(QStringList resultList)
{
	for (int i=0;i<resultList.size();i++)
	{
		printf("%d %s\n",i,resultList[i].toLatin1().data());
	}
	queryProgressDialog->UpdateProgressValue(0.9);
	queryProgressDialog->UpdateProgressInfo(tr("获取检索结果..."));

	
	//将检索的结果显示在表单中
	list=resultList;
	if(list.size()%3==0)
		ui->tableWidget_result->setRowCount(list.size()/3);
	else
		ui->tableWidget_result->setRowCount(list.size()/3+1);
	ui->tableWidget_result->setColumnCount(3);
	if(list.size()>0)
	{
		ui->checkBox_allSelected->setEnabled(true);
		ui->checkBox_allSelected->setCheckState(Qt::Checked);
		ui->pushButton_exportResult->setEnabled(true);
	}
	int row,column=0;
	for(int i=0;i<list.size();i++)
	{

		row=i/3;

		if(column==3)
			column=0;
		ui->tableWidget_result->setRowHeight(row,150);
		ui->tableWidget_result->setColumnWidth(column,ui->tableWidget_result->size().width()/3);
		ResultTableItem *result=new ResultTableItem;
		result->setFilePath(list[i]);

		QLabel *label_image=new QLabel;
		QImage *image=new QImage;

		if(!( image->load(list[i])))
		{
			QString errorMessage = "The file: \""+ list[i]+ "\" is not a image file!";
			QMessageBox::information(NULL,
				"FileError",
				errorMessage
				);

			delete image;
			return;
		}
		QImage img=image->scaled(70,150,Qt::KeepAspectRatio);
		label_image->setPixmap(QPixmap::fromImage(img));

		ui->tableWidget_result->setItem(row,column,result);
		ui->tableWidget_result->setCellWidget(row,column,label_image);

		column++;
	}



	//	ui->pushButton_loadTargetImage->setEnabled(true);
	//	ui->pushButton_loadTargetMask->setEnabled(true);
	ui->pushButton_startSearch->setEnabled(true);

	queryProgressDialog->UpdateProgressValue(1.0);
	queryProgressDialog->UpdateProgressInfo(tr("检索完毕!"));
	queryProgressDialog->close();
	delete queryProgressDialog;
}

void ImageMatch::startSearchImage()
{ 

    //获取界面上的参数
    if(ui->checkBox->isChecked())
    {
        colorFeature=true; //是否进行颜色特征的检索
        colorItem=ui->comboBox_2->currentText(); //将选定的何种颜色特征以QString方式存储
    }
    else
    {
        colorFeature=false; //是否进行颜色特征的检索
        colorItem="";
    }

    if(ui->checkBox_2->isChecked())
    {
        textureFeature=true;
        textureItem=ui->comboBox_3->currentText();
    }
    else
    {
        textureFeature=false;
        textureItem="";
    }

    if(ui->checkBox_3->isChecked())
    {
        shapeFeature=true;
        shapeItem=ui->comboBox_4->currentText();
    }
    else
    {
        shapeFeature=false;
        shapeItem="";
    }
    if(ui->checkBox_4->isChecked())
    {
        elementFeature=true;
        elementItem=ui->comboBox_5->currentText();
    }
    else
    {
        elementFeature=false;
        elementItem="";
    }

   initTableWidget();

   QImage outImg,maskImg;
   if (MainWindow::Instance().GetElementPickModule()->getSelectedElement(outImg,maskImg))
   {
	   if (targetImage)
		   delete targetImage;
	   if (targetImageMask)
		   delete targetImageMask;
	   targetImage=new QImage(outImg);
	   targetImageMask=new QImage(maskImg);		
   }
   else if (MainWindow::Instance().GetActiveSubWindow())
   {
	   if(targetImage)
		   delete targetImage;
	   QImage winImage=MainWindow::Instance().GetActiveSubWindow()->getSelectionImage();
	   if (winImage.width()==1||winImage.height()==1)
	   {
		   targetImage=new QImage(MainWindow::Instance().GetActiveSubWindow()->getImage());
	   }
	   else
	   {
			targetImage=new QImage(winImage);
	   }	   
   }
   else
   {
	   return;
   }


    if(targetImage&&(colorFeature || textureFeature || shapeFeature||elementFeature)) //获得参数后才进行检索
    {
	    getResult();//得到颜色检索的结果
    }

}

/*单击表格中单元时进行预览*/
void ImageMatch::tableCellClicked(int row,int column)
{
    resultImage=new QImage();
    ResultTableItem *temp;
    if(temp=(ResultTableItem *)ui->tableWidget_result->currentItem())
    {
        resultImage->load(temp->getFilePath());
//        QImage img=resultImage->scaled(ui->label_resultImage->size(),Qt::KeepAspectRatio);
//		  ui->label_resultImage->setAlignment(Qt::AlignCenter);
//        ui->label_resultImage->setPixmap(QPixmap::fromImage(img));

        if(temp->checkState()==Qt::Checked)
        {
            temp->setCheckState(Qt::Unchecked);
            ui->checkBox_allSelected->setCheckState(Qt::Unchecked);
        }
        else
            temp->setCheckState(Qt::Checked);
    }

}



void ImageMatch::getResult()
{
	
	QString targetImagePath="targetImage_tmp.jpg";
	QString targetImageMaskPath;
	targetImage->save(targetImagePath,"jpg");

	std::vector<std::string> resultPaths;
	ElementType elementType=IMAGE_ELEMENT_TYPE_ALL;
	QString resultCntString=ui->lineEdit_resultCount->text();
	if (resultCntString.length()==0)
	{
		QMessageBox::information(NULL,"Error","result count not set!");
		return;
	}
	int resultCount=atoi(resultCntString.toLatin1().data());
	if (resultCount<=0||resultCount>1000)
	{
		QMessageBox::information(NULL,"Error","result count should in [1,1000]!");
		return;
	}
	std::vector<FeatureType> featureTypes;	
	if (colorFeature)
	{
		if (ui->comboBox_2->currentIndex()==0)
		{
			featureTypes.push_back(FEATURE_TYPE_COLOR_MOMENT);
		}
		else if(ui->comboBox_2->currentIndex()==1)
		{
			featureTypes.push_back(FEATURE_TYPE_COLOR_HIST);
		}
	}
	if (textureFeature)
	{
		//if(ui->comboBox_3->currentIndex()==0)
		featureTypes.push_back(FEATURE_TYPE_TEXTURE_GLCM);
	}
	if (shapeFeature)
	{
		if (targetImageMask==NULL)
		{
			QMessageBox::information(NULL,"Error","query image mask not loaded!");
			return;
		}
		targetImageMaskPath="targetImage_mask_tmp.png";
		bool flag=targetImageMask->save(targetImageMaskPath);
		//if(ui->comboBox_4->currentIndex()==0)
		featureTypes.push_back(FEATURE_TYPE_SHAPE_CONTEXT);
	}
	
	
	//ui->pushButton_loadTargetImage->setDisabled(true);
	//ui->pushButton_loadTargetMask->setDisabled(true);
	ui->pushButton_startSearch->setDisabled(true);

	if (query==NULL)
	{
		query=new QueryThread(this);
	}
	
	queryProgressDialog = new WaitingDialog(this);  
	queryProgressDialog->setWindowTitle(tr("请等待..."));
	//dialog->Start(50, 150);
	queryProgressDialog->show();
	queryProgressDialog->UpdateProgressValue(0.1);
	queryProgressDialog->UpdateProgressInfo(tr("初始化..."));
	
	if (!g_matlabInitialized)
	{
		MCRInitialize();
		g_matlabInitialized=true;
	}	
	queryProgressDialog->UpdateProgressValue(0.3);
	queryProgressDialog->UpdateProgressInfo(tr("数据库图像特征匹配..."));
	disconnect(query,SIGNAL(notify(QStringList)),this,SLOT(OnNotify(QStringList)));
	connect(query,SIGNAL(notify(QStringList)),this,SLOT(OnNotify(QStringList)));   
	query->doQuery(targetImagePath,targetImageMaskPath,featureTypes,elementType,resultCount);
}

void ImageMatch::selectAll(bool state)
{
//  if(state)
    {
        int row=0,column=0;
        for(int i=0;i<list.size();i++)
        {
            row=i/3;
            if(column==3)
                column=0;
            ResultTableItem *temp;
            if(temp=(ResultTableItem *)ui->tableWidget_result->item(row,column))
			{
				if (state)
					temp->setCheckState(Qt::Checked);
				else
					temp->setCheckState(Qt::Unchecked);
			}
            column++;
        }
    }
}

bool removeDirWithContent(const QString &dirName){
	static QVector<QString> dirNames;
	static QString funcErrMsg="删除[%1]失败.";
	static QString funcInfFndMsg="发现[%1].";
	static QString funcInfDelMsg="删除[%1]成功.";
	QDir dir;
	QFileInfoList filst;
	QFileInfoList::iterator curFi;

	dirNames.clear();
	if(dir.exists()){
		dirNames<<dirName;
	}
	else{
		return true;
	}

	for(int i=0;i<dirNames.size();++i){
		dir.setPath(dirNames[i]);
		filst=dir.entryInfoList(QDir::Dirs|QDir::Files
			|QDir::Readable|QDir::Writable
			|QDir::Hidden|QDir::NoDotAndDotDot
			,QDir::Name);
		if(filst.size()>0){
			curFi=filst.begin();
			while(curFi!=filst.end()){
				//遇到文件夹,则添加至文件夹列表dirs尾部
				if(curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if(curFi->isFile()){
					//遇到文件,则删除之
					if(!dir.remove(curFi->fileName())){
						return false;
					}
				}
				curFi++;
			}//end of while
		}
	}

	for(int i=dirNames.size()-1;i>=0;--i){
		dir.setPath(dirNames[i]);
		if(!dir.rmdir(".")){
			return false;
		}
	}
	return true;
}

void ImageMatch::exportResult()
{
    resultList.clear();
    int row=0,column=0;
    for(int i=0;i<list.size();i++)
    {
        row=i/3;        
        if(column==3)
            column=0;
        ResultTableItem *temp=(ResultTableItem *)ui->tableWidget_result->item(row,column);
        if(temp->checkState()==Qt::Checked)
            resultList.append(temp->getFilePath());
        column++;
    }

	
	//将结果导出到主界面检索结果选项卡
	for (int j=0;j<resultList.size();j++)
	{
		MainWindow::Instance().GetResourceImageFileHandle()->addImage(tr("检索结果"),resultList[j]);
	}

/*//将结果导出到文件夹
	QString dir="queryResults";
	QDir *cttemp = new QDir;
	if(cttemp->exists(dir))
		removeDirWithContent(dir);
	cttemp->mkdir(dir);	

	for (int j=0;j<resultList.size();j++)
	{
		QString oldName=resultList[j];
		QString newName=dir+"\\"+QString::number(j)+".jpg";
		QFile::copy(oldName,newName);
	}
*/
    if(resultList.size()==0)
    {
        QMessageBox::warning(this,"Expert warning","You didn`t select any picture!");
    }
	emit resultExported(resultList);
}

void ImageMatch::colorFeatureEnable()
{
    if(ui->checkBox->checkState()==Qt::Checked)
        ui->comboBox_2->setEnabled(true);
    else
        ui->comboBox_2->setEnabled(false);
}

void ImageMatch::textureFeatureEnable()
{
    if(ui->checkBox_2->checkState()==Qt::Checked)
        ui->comboBox_3->setEnabled(true);
    else
        ui->comboBox_3->setEnabled(false);
}
void ImageMatch::shapeFeatureEnable()
{
    if(ui->checkBox_3->checkState()==Qt::Checked)
        ui->comboBox_4->setEnabled(true);
    else
        ui->comboBox_4->setEnabled(false);
}
void ImageMatch::elementFeatureEnable()
{
    if(ui->checkBox_4->checkState()==Qt::Checked)
        ui->comboBox_5->setEnabled(true);
    else
        ui->comboBox_5->setEnabled(false);
}

void ImageMatch::initTableWidget()
{
    ui->tableWidget_result->clearContents();

    ui->tableWidget_result->setRowCount(0);
    ui->tableWidget_result->setColumnCount(0);
    ui->checkBox_allSelected->setCheckState(Qt::Unchecked);
    ui->checkBox_allSelected->setEnabled(false);
    ui->pushButton_exportResult->setEnabled(false);
    list.clear();
}
