#include "MainWindow.h"
#include "PanelManager.h"
#include "SegmentationParamPanel.h"
#include "Segmentation.h"
//#include "imagematch.h"
#include "ImageListProjectFileHandle.h"
//#include "Inpainting/InpaintingParamPanel.h"
//#include "Inpainting/InpaintingWinEvent.h"
//#include "Inpainting/InpaintingManager.h"
//#include "Inpainting/InpaintingProcessor.h"
//#include "addelement.h"
//#include "ReadingBigImage/readingbigimage.h"

#include <QMessageBox>

MainWindow* MainWindow::pMainWin = NULL;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	pMainWin = this;

	ui.setupUi(this);

	panorama_progress_bar = NULL;
/*
	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()),
			this, SLOT(OpenRecentFile()));
	}

	separator_recentFile = ui.menu_recentFiles->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
		ui.menu_recentFiles->addAction(recentFileActs[i]);*/


	_panelmanager = new PanelManager(this, ui.windowMenu);
	_optionPagemanager = new OptionPageManager();
	_projectfilemanager = new ProjectFileManager();
	_undoRedoManager = new UndoRedoManager();
	_operationmodemanager = new OperationModeManager();

	toolbar_init();
	recent_init();
	panel_init();	
	self_init();
	module_init();
}

MainWindow::~MainWindow()
{
	if (_panelmanager)
	{
		delete _panelmanager;
		_panelmanager = NULL;
	}
	if (_optionPagemanager)
	{
		delete _optionPagemanager;
		_optionPagemanager = NULL;
	}
	if (_projectfilemanager)
	{
		delete _projectfilemanager;
		_projectfilemanager = NULL;
	}
	if (_undoRedoManager)
	{
		delete _undoRedoManager;
		_undoRedoManager = NULL;
	}
	if (_operationmodemanager)
	{
		delete _operationmodemanager;
		_operationmodemanager = NULL;
	}

	//if (g_matlabInitialized)
	//{
	//	MCRTerminate();
	//}	

	if(panorama_progress_bar) delete panorama_progress_bar;

	pMainWin = NULL;
}

void MainWindow::recent_init()
{
	for (int i=0;i<MaxRecentFiles;i++)
	{	
		recentFileActs[i] = new QAction(this);
		ui.menu_recentFiles->addAction(recentFileActs[i]);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()),
			this, SLOT(OpenRecentFile()));
	}

	updateRecentFileActions();
}
void MainWindow::toolbar_init()
{
	//TODO:工具条的图标未设定
	QToolBar *toolbar = ui.toolBar;
	toolbar->addAction(ui.createAct);
	toolbar->addAction(ui.openAct);
	toolbar->addAction(ui.actionClose);
	toolbar->addSeparator();
	ui.createAct->setIcon(QIcon(":/toolbar/icon/toolbar_add.png"));
	ui.openAct->setIcon(QIcon(":/toolbar/icon/toolbar_open.bmp"));
	ui.actionClose->setIcon(QIcon(":/toolbar/icon/toolbar_close.png"));

	toolbar->addAction(ui.zoom_in);
	toolbar->addAction(ui.zoom_out);
	toolbar->addAction(ui.viewInActualPixAct);
	toolbar->addAction(ui.viewInScreenAct);
	toolbar->addSeparator();
	ui.zoom_in->setIcon(QIcon(":/toolbar/icon/toolbar_up.png"));
	ui.zoom_out->setIcon(QIcon(":/toolbar/icon/toolbar_down.png"));
	ui.viewInActualPixAct->setIcon(QIcon(":/toolbar/icon/toolbar_InActualPixAct.png"));
	ui.viewInScreenAct->setIcon(QIcon(":/toolbar/icon/toolbar_InScreenAct.ico"));

	//toolbar->addAction(ui.actionPanorama);
	//toolbar->addAction(ui.actionElementQuery);
	//toolbar->addAction(ui.actionAddElement);
	toolbar->addSeparator();
	//ui.actionPanorama->setIcon(QIcon(":/toolbar/icon/toolbar_stitching.png"));
	//ui.actionElementQuery->setIcon(QIcon(":/toolbar/icon/toolbar_search.png"));
	////ui.actionAddElement->setIcon(QIcon(":/toolbar/icon/toolbar_database.ico"));

	toolbar->addAction(ui.actionSaveInpainting);
	ui.actionSaveInpainting->setIcon(QIcon(":/toolbar/icon/toolbar_database.ico"));
}

void MainWindow::module_init()
{
	_modulepick = new ElementPick;

	QScrollArea * segmentScroll = static_cast<QScrollArea*>(_panelmanager->GetPanel(tr("分割")));
	_modulesegment = new Segmentation(static_cast<SegmentationParamPanel*>(segmentScroll->widget()), this);
	//QObject::connect(ui.actionLazyBrush,SIGNAL(triggered()),_modulesegment,SLOT(execute()));
	//_modulequery = NULL;
	//_modulepan = NULL;
	//QObject::connect(ui.actionElementQuery,SIGNAL(triggered()),this,SLOT(OpenElementQuery()));
	//_addelement = NULL;
	//QObject::connect(ui.actionAddElement,SIGNAL(triggered()),this,SLOT(OpenAddElement()));
	//QObject::connect(ui.actionAddElementToPrj, SIGNAL(triggered()), this, SLOT(AddElementToPrj()));
	//QObject::connect(ui.actionPanorama,SIGNAL(triggered()),this,SLOT(OpenPanorama()));
	//QObject::connect(ui.actionOpenBigImage, SIGNAL(triggered()), this, SLOT(OpenBigImage()));
}

void MainWindow::panel_init()
{
	_resourceFileHandle = new ImageListProjectFileHandle(tr("资源"));
	_elementFileHandle = new ImageListProjectFileHandle(tr("元素"));
	_projectfilemanager->addHandle(_resourceFileHandle);
	_projectfilemanager->addHandle(_elementFileHandle);
	
	ImageList* resourceimagelist = new ImageList(_resourceFileHandle);
	ImageList* elementimagelist = new ImageList(_elementFileHandle);
	QObject::connect(resourceimagelist, SIGNAL(imageDoubleClicked(QString)), this, SLOT(OpenImage(QString)));//打开资源列表中的图片
	QObject::connect(elementimagelist, SIGNAL(imageDoubleClicked(QString)), this, SLOT(OpenImage(QString)));//打开资源列表中的图片
	_panelmanager->RegisterPanel(tr("浏览"), tr("资源"), resourceimagelist, Qt::RightDockWidgetArea);
	_panelmanager->RegisterPanel(tr("浏览"), tr("元素"), elementimagelist, Qt::RightDockWidgetArea);
	_panelmanager->ActivePanel(tr("资源"));

	SegmentationParamPanel* segmentpanel = new SegmentationParamPanel;
	QScrollArea * segmentscroll = new QScrollArea();
	segmentscroll->setWidget(segmentpanel);//by eye
	
	//InpaintingParamPanel &inpainting_panel = InpaintingParamPanel::getInstance();
	//QScrollArea * inpainting_scroll = new QScrollArea();
	//inpainting_scroll->setWidget(&inpainting_panel);
	//_panelmanager->RegisterPanel(tr("参数"), tr("修补"), inpainting_scroll, Qt::RightDockWidgetArea);

	_panelmanager->RegisterPanel(tr("参数"), tr("分割"), segmentscroll, Qt::RightDockWidgetArea);
	_optionPagemanager->RegisterPage(tr("分割算法参数"),new SegmentationOptionPage(_optionPagemanager));
}


void MainWindow::self_init()
{
	QObject::connect(ui.zoom_out,SIGNAL(triggered()),this,SLOT(ZoomOut()));//窗口内部图片缩小
	QObject::connect(ui.zoom_in,SIGNAL(triggered()),this,SLOT(ZoomIn()));//窗口内部图片缩小
	QObject::connect(ui.viewInActualPixAct, SIGNAL(triggered()), this, SLOT(onViewInActualPix()));
	QObject::connect(ui.viewInScreenAct, SIGNAL(triggered()), this, SLOT(onViewInScreen()));

	QObject::connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(onActionClose()));
	QObject::connect(ui.actionCloseAll, SIGNAL(triggered()), this, SLOT(onActionCloseAll()));
	QObject::connect(ui.actionTile, SIGNAL(triggered()), this, SLOT(onActionTile()));
	QObject::connect(ui.actionCascade, SIGNAL(triggered()), this, SLOT(onActionCascade()));
	QObject::connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onActionNext()));
	QObject::connect(ui.actionPrevious, SIGNAL(triggered()), this, SLOT(onActionPrevious()));
	QObject::connect(ui.actionSaveInpainting, SIGNAL(triggered()), this, SLOT(onSaveInpainting()));
	QObject::connect(ui.createAct, SIGNAL(triggered()), this, SLOT(NewPrj()));//新建工程
	QObject::connect(ui.saveAct, SIGNAL(triggered()), this, SLOT(SavePrj()));//保存工程
	QObject::connect(ui.saveAsAct, SIGNAL(triggered()), this, SLOT(SaveAsPrj()));//另存工程
	QObject::connect(ui.openAct, SIGNAL(triggered()), this, SLOT(OpenPrj()));//打开工程
	QObject::connect(ui.closeAct, SIGNAL(triggered()), this, SLOT(closePrj()));
	QObject::connect(ui.action_mix, SIGNAL(triggered()), this, SLOT(mixbmp()));
	QObject::connect(ui.image_import, SIGNAL(triggered()), this, SLOT(ImportImage()));//导入图片
// 	QObject::connect(resource_list, SIGNAL(imageDoubleClicked(QString)), this, SLOT(OpenImage(QString)));//打开资源列表中的图片
// 	QObject::connect(ui.optionAct,SIGNAL(triggered()),this,SLOT(ShowOptionPanel()));//打开选项卡
	QObject::connect(ui.optionAct,SIGNAL(triggered()),this,SLOT(ShowOptionPanel()));//打开选项面板

	QObject::connect(ui.undoAct, SIGNAL(triggered()), this, SLOT(UndoAct()));//撤销
	QObject::connect(ui.redoAct, SIGNAL(triggered()), this, SLOT(RedoAct()));//恢复

	QObject::connect(ui.editMenu, SIGNAL(aboutToShow()), this,SLOT(JudgeEditState()));

	connect(_projectfilemanager, SIGNAL(modified(bool)), this, SLOT(UpdateProjectFileStatus(bool)));

//操作模式注册
	QAction* arrowact = new QAction(QIcon(":/mainwin/icon/cursor_arrow.png"), QObject::tr("默认显示"), this);
	//QAction* selectact = new QAction(QIcon(":/mainwin/icon/cursor_selection.png"), QObject::tr("矩形选框"), this);
	//QAction* inpainting = new QAction(QIcon(":/inpainting/icon/cursor_inpainting.png"), QObject::tr("图像修补"), this);
	QAction* segmentact = new QAction(QIcon(":/segmentation/icon/cursor_brush.png"),QObject::tr("元素分割"), this);
	//QAction* layeract = new QAction(QIcon(":/segmentation/icon/cursor_layer.png"),QObject::tr("元素选择"), this);

	_operationmodemanager->addOperationMode(ARROWMODE, arrowact);
	//_operationmodemanager->addOperationMode(SELECTMODE, selectact);
	//_operationmodemanager->addOperationMode(INPAINTINGMODE, inpainting);
	_operationmodemanager->addOperationMode(SEGMENTATIONMODE, segmentact);
	//_operationmodemanager->addOperationMode(ELEMENTPICKMODE, layeract);

	_operationmodemanager->setMode(ARROWMODE);

	QObject::connect(ui.toolMenu, SIGNAL(aboutToShow()), this, SLOT(onToolMenuAboutToShow()));
}

void MainWindow::onOperationActionToggled(bool checked)
{
	ImageWin* activeWin = GetActiveSubWindow();
	if (activeWin)
	{
		activeWin->update();
	}

	activeRelevantPanel();
}

void MainWindow::activeRelevantPanel()
{
	int mode = _operationmodemanager->mode();
	if (mode == INPAINTINGMODE)
	{
		_panelmanager->ActivePanel(tr("修补"));
	}
	else if (mode == SEGMENTATIONMODE)
	{
		_panelmanager->ActivePanel(tr("分割"));
	}
}
void MainWindow::mixbmp(){
	QString maskfilename = QFileDialog::getOpenFileName( 
		this, 
		tr("打开Mask图像文件"), 
		QDir::currentPath(), 
		tr("BMP(*.bmp)") );
	QString zdfilename = QFileDialog::getOpenFileName( 
		this, 
		tr("打开遮挡图像文件"), 
		QDir::currentPath(), 
		tr("BMP(*.bmp)") );
	if (!maskfilename.isEmpty() && !zdfilename.isEmpty())
	{
		QImage mask = QImage(maskfilename,"BMP");
		QImage zd = QImage(zdfilename,"BMP");
		for(int i=0;i<mask.width();i++){
			for(int j=0;j<mask.height();j++){
				QColor tmp = QColor::fromRgba(zd.pixel(i,j));
				if(tmp.red()==128){
					mask.setPixel(i,j,128);
				}
			}
		}
		QString filepath = QFileDialog::getSaveFileName( 
		this, 
		tr("保存合并结果图像"), 
		QDir::currentPath(), 
		tr("BMP Image(*.bmp)") );
		if(!filepath.isEmpty()){
			if(mask.save(filepath,"BMP")){
				::QMessageBox::information(this,tr("通知"),tr("保存成功"),QMessageBox::Ok,QMessageBox::Ok);
			}
		}
	}
}
void MainWindow::onSaveInpainting()
{
	//if (CURRENT_MODE != INPAINTINGMODE) return;
	//ImageWin *cur_imagewin = GetActiveSubWindow();
	//if (!cur_imagewin) return;

	//InpaintingProcessor *processor = InpaintingManager::getInstance().get_processor(cur_imagewin);
	//if (!processor) return;

	//QImage outimg = processor->getResult();
	//QString path = GetProjectFileManager()->path();

	//QDateTime dt;   
	//dt.setTime(QTime::currentTime());   
	//dt.setDate(QDate::currentDate());   
	//QString currentDate = dt.toString("yyyyMMddhhmmss");

	//path.append(tr("/资源/"));
	//QDir dir;
	//if (!dir.exists(path))
	//{
	//	dir.mkpath(path);
	//}
	//path.append(currentDate);

	//QString imgpath = path + ".jpg";
	//outimg.save(imgpath);

	//GetResourceImageFileHandle()->addImage(tr("工程资源"), imgpath);
	//GetPanelManager()->ActivePanel(tr("资源"));
}

void MainWindow::onActionClose()
{
	ui.mdiArea->closeActiveSubWindow();
}

void MainWindow::onActionCloseAll()
{
	ui.mdiArea->closeAllSubWindows();
}

void MainWindow::onActionTile()
{
	ui.mdiArea->tileSubWindows();
}

void MainWindow::onActionCascade()
{
	ui.mdiArea->cascadeSubWindows();
}

void MainWindow::onActionNext()
{
	ui.mdiArea->activateNextSubWindow();
}

void MainWindow::onActionPrevious()
{
	ui.mdiArea->activatePreviousSubWindow();
}

//void MainWindow::OpenElementQuery()
//{
//	if (_modulequery==NULL)
//	{
//		_modulequery=new ImageMatch(this);
//	}	
//	_modulequery->show();
//}

void MainWindow::destroyAllSubWindow()
{
	onActionCloseAll();
	QList<QMdiSubWindow *> sub_window_list = ui.mdiArea->subWindowList();
	QList<QMdiSubWindow *>::iterator it = sub_window_list.begin();
	while(it != sub_window_list.end())
	{
		(*it)->hide();
		delete *it;
		++it;
	}
	img_subwindow_map.clear();
}

//void MainWindow::OpenAddElement()
//{
//	QImage outimg, maskimg;
//	if (!GetElementPickModule()->getSelectedElement(outimg, maskimg))
//	{
//		QMessageBox nothingMsgBox;
//		nothingMsgBox.setText(tr("当前没有选中任何元素！"));
//		return;
//	}
//
//	QMessageBox msgBox;
//	msgBox.setInformativeText(tr("是否将当前元素添加到数据库？"));
//	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//	msgBox.setDefaultButton(QMessageBox::Yes);
//	int ret = msgBox.exec();
//	switch (ret) {
//	   case QMessageBox::Yes:
//		   if (_addelement==NULL)
//		   {
//			   _addelement=new AddElement(this);			  
//		   }
//			_addelement->addNewElement(outimg, maskimg);
//	    	_addelement->show();
//		   break;
//	   case QMessageBox::No:
//		   return;
//	   default:
////		   // should never be reached
////		   break;
////	}	
////}
//
//void MainWindow::AddElementToPrj()
//{
//	QImage outimg, maskimg;
//	if (!GetElementPickModule()->getSelectedElement(outimg, maskimg))
//	{
//		QMessageBox nothingMsgBox;
//		nothingMsgBox.setText(tr("当前没有选中任何元素！"));
//		return;
//	}
//
//	QString path = GetProjectFileManager()->path();
//
//	QDateTime dt;   
//	QTime time;  
//	QDate date;   
//	dt.setTime(time.currentTime());   
//	dt.setDate(date.currentDate());   
//	QString currentDate = dt.toString("yyyyMMddhhmmss");
//
//	path.append(tr("/元素/"));
//	QDir dir;
//	if (!dir.exists(path))
//	{
//		dir.mkpath(path);
//	}
//	path.append(currentDate);
//	
//	QString imgpath = path + ".jpg";
//	QString maskpath = path + "_mask.png";
//
//	outimg.save(imgpath);
//	QBitmap bitmap = QBitmap::fromImage(maskimg, Qt::MonoOnly);
//	bitmap.save(maskpath);
//
//	GetElementImageFileHandle()->addImage(tr("工程元素"), imgpath);
//	GetPanelManager()->ActivePanel(tr("元素"));
//}

//void MainWindow::OpenPanorama()
//{
//	//if (_modulepan==NULL)
//	//{
//	//	//_modulepan=new PanoramaOptionParam(this);
//	//}	
//	//_modulepan->show();
//}

void MainWindow::ImportImage()
{
	QStringList filenames = QFileDialog::getOpenFileNames( 
		this, 
		QObject::tr("导入图像文件"), 
		QDir::currentPath(), 
		QObject::tr("图像文件 (*.png *.jpg *.bmp)"));

	GetResourceImageFileHandle()->addImage(tr("原始图片"), filenames);

	GetPanelManager()->ActivePanel(tr("资源"));
}

void MainWindow::ZoomIn()
{
	ImageWin * current_widget = GetActiveSubWindow();
	if (current_widget) current_widget->zoomIn();
}

void MainWindow::ZoomOut()
{
	ImageWin * current_widget = GetActiveSubWindow();
	if (current_widget) current_widget->zoomOut();
}

void MainWindow::onViewInActualPix()
{
	ImageWin * current_widget = GetActiveSubWindow();
	if (current_widget) current_widget->viewInActualPix();
}

void MainWindow::onViewInScreen()
{
	ImageWin * current_widget = GetActiveSubWindow();
	if (current_widget)
	{
		QSize window_size = ui.mdiArea->activeSubWindow()->size();
		QSize image_size = current_widget->getImage().size();

		double scale_ratio = min(window_size.width()/(double)image_size.width(), window_size.height()/(double)image_size.height());
		current_widget->scaleImage(scale_ratio/current_widget->getscaleFactor());
	}
}

void MainWindow::UpdateProjectFileStatus(bool bModified)
{
	QString titlename = GetProjectFileManager()->name() + tr(" - 华为天线提取软件");
	setWindowTitle(titlename + (bModified ? "*" : ""));
}

void MainWindow::OpenImage(QString img_filePath)
{
	QImage image(img_filePath);
	if (image.isNull())
	{
		QMessageBox::information(this, tr("无法打开文件"), tr("选择合法图像")); 
		return;
	}

 	if (img_subwindow_map.find(img_filePath) != img_subwindow_map.end())
 	{
 		img_subwindow_map[img_filePath]->show();
		img_subwindow_map[img_filePath]->setFocus();
		QWidget *child = img_subwindow_map[img_filePath]->widget();
		if (child)
		{
			child->show();
		}
 		return;
 	}

	/* 暂时将缩放去掉了
	double _scale_ratio = 1280.0/image.width() < 800.0/image.height() ? 1280.0/image.width() : 800.0/image.height();
	if (_scale_ratio < 1.0)	image = image.scaled(image.size() * _scale_ratio);
	*/

	//新窗口
	QMdiSubWindow *sub_window = CreateSubWindow(image, img_filePath);
	img_subwindow_map[img_filePath] = sub_window;
}

QMdiSubWindow* MainWindow::CreateSubWindow(QImage& m_image,const QString& m_title)
{
	//scrollArea初始化
	QScrollArea * scrollArea = new QScrollArea(this);
	
	//新的子窗口
	ImageWin *img_widget = new ImageWin(scrollArea);
	img_widget->SetImage(&m_image);
	img_widget->setPath(m_title);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(img_widget);

	//窗口的名称只显示图像的名称
	QString filename=QFileInfo(m_title).fileName();
	filename.truncate(filename.lastIndexOf("."));

	QMdiSubWindow * subWindow = ui.mdiArea->addSubWindow(scrollArea);//滚动区域
	scrollArea->setAlignment(Qt::AlignCenter);//图片显示在中心
	subWindow->setWindowTitle(filename);
	subWindow->show();
	subWindow->setAttribute(Qt::WA_DeleteOnClose, false);

	return subWindow;
}

ImageWin* MainWindow::GetActiveSubWindow()
{
	QMdiSubWindow* subwin = ui.mdiArea->activeSubWindow();
	if (subwin)
	{
		QScrollArea* current_scrollArea = (QScrollArea*)subwin->widget();
		if (current_scrollArea)
		{
			return (ImageWin*)current_scrollArea->widget();//得到滚动区域的组件
		}
	}
	
	return NULL;
}

int MainWindow::popupSaveMessageBox()
{
	QMessageBox msgBox;
	msgBox.setText(tr("工程已被修改"));
	msgBox.setInformativeText(tr("是否保存?"));
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();

	return ret;
}

void MainWindow::closeEvent( QCloseEvent * event )
{
	if (preCloseProject())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void MainWindow::NewPrj()
{
	if (!preCloseProject())
	{
		return;
	}

	//得到工程路径
	QString filepath = QFileDialog::getSaveFileName( 
		this, 
		tr("新建"), 
		QDir::currentPath(), 
		tr("Project") );

	if (!filepath.isEmpty())
	{
		qDebug()<<filepath;
		ui.mdiArea->closeAllSubWindows();

		QDir dir;
		dir.mkpath(filepath);

		QString filename = QFileInfo(filepath).fileName();
		filepath += QDir::separator() + filename + ".xml";
		GetProjectFileManager()->newfile(filepath);

		setRecentCurrentFile(filepath);
	}
}

void MainWindow::OpenPrj()
{
	if (!preCloseProject())
	{
		return;
	}

	QString filename = QFileDialog::getOpenFileName( 
		this, 
		tr("打开"), 
		QDir::currentPath(), 
		tr("工程文件(*.xml)") );

	if (!filename.isEmpty())
	{
		qDebug()<<filename;
		ui.mdiArea->closeAllSubWindows();

		GetProjectFileManager()->load(filename);

		setRecentCurrentFile(filename);
	}
}

void MainWindow::SavePrj()
{
	GetProjectFileManager()->save();
}

void MainWindow::SaveAsPrj()
{
	//得到工程路径
	QString filepath = QFileDialog::getSaveFileName( 
		this, 
		tr("Save As"), 
		QDir::currentPath(), 
		tr("Project") );

	if (!filepath.isEmpty())
	{
		qDebug()<<filepath;
		QDir dir;
		dir.mkpath(filepath);

		QString filename = QFileInfo(filepath).fileName();
		filepath += QDir::separator() + filename + ".xml";
		GetProjectFileManager()->saveas(filepath);

		setRecentCurrentFile(filepath);
	}
}

void MainWindow::closePrj()
{
	if (preCloseProject())
	{
		GetProjectFileManager()->reset();

		destroyAllSubWindow();
	}
}

bool MainWindow::preCloseProject()
{
	//TODO:
	if (GetProjectFileManager()->needSave())
	{
		int ret = popupSaveMessageBox();

		switch(ret)
		{
		case QMessageBox::Save:
			GetProjectFileManager()->save();
			break;
		case QMessageBox::Discard:
			break;
		case QMessageBox::Cancel:
			return false;
		}
	}
	return true;
}

void MainWindow::ShowOptionPanel()
{
	this->_optionPagemanager->show();
}
void MainWindow::UndoAct()
{
	this->_undoRedoManager->Undo();
}
void MainWindow::RedoAct()
{
	this->_undoRedoManager->Redo();
}
void MainWindow::JudgeEditState()
{
	ui.redoAct->setEnabled(this->_undoRedoManager->CanRedo());
	ui.undoAct->setEnabled(this->_undoRedoManager->CanUndo());
}

//void MainWindow::OpenBigImage()
//{
//	//ImageWin *image = GetActiveSubWindow();
//	//if(image == NULL)	return;
//
//	//QString image_path = image->getPath();
//	//if(image_path.isEmpty()) return;
//	//image_path.truncate(image_path.lastIndexOf("."));
//	//image_path.append(".bigimage");
//
//	///* the widget has set Qt::WA_DeleteOnClose attribute */
//	//ReadingBigImage *reader = new ReadingBigImage(this);
//
//	//if(!reader->open_file_name(image_path)) return;
//	//reader->show();
//}

void MainWindow::onToolMenuAboutToShow()
{
	ui.actionOpenBigImage->setEnabled(false);

	ImageWin *image = GetActiveSubWindow();
	if(image == NULL)	return;

	QString image_path = image->getPath();
	if(image_path.isEmpty()) return;

	image_path.truncate(image_path.lastIndexOf("."));
	image_path.append(".bigimage");

	// 打开bigimage浏览器的菜单项只在当前文件为bigimage的时候才开启
	ui.actionOpenBigImage->setEnabled(QFileInfo(image_path).exists());
}

void MainWindow::OpenRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		if (!preCloseProject())
		{
			return;
		}

		ui.mdiArea->closeAllSubWindows();

		GetProjectFileManager()->load(action->data().toString());

		setRecentCurrentFile(action->data().toString());
	}
}

void MainWindow::setRecentCurrentFile(const QString &fileName)
{
	QSettings settings(":/recent/recentFile.ini");
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	updateRecentFileActions();

}

void MainWindow::updateRecentFileActions()
{
	QSettings settings(":/recent/recentFile.ini");
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}

	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

}

QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

QProgressBar* MainWindow::create_panorama_progress_bar()
{
	if(panorama_progress_bar)	delete_panorama_progress_bar();
	panorama_progress_bar = new QProgressBar(this);

	if(statusBar() == NULL)	setStatusBar(new QStatusBar(this));
	statusBar()->addPermanentWidget(panorama_progress_bar);
	return panorama_progress_bar;
}

void MainWindow::delete_panorama_progress_bar()
{
	if(panorama_progress_bar != NULL) {
		statusBar()->removeWidget(panorama_progress_bar);
		delete panorama_progress_bar;
		panorama_progress_bar = NULL;
	}
}
