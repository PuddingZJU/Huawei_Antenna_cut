#ifndef __ES_DHAPP_MAINWINDOW_H__
#define __ES_DHAPP_MAINWINDOW_H__

#include <QtGui/QMainWindow>
#include <QMdiArea>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>
#include <QScrollBar>
#include <QScrollArea>
#include <QString>
#include <QMdiSubWindow>
#include <QDebug>
#include <QtXml>
#include <QCloseEvent>
#include <QProgressBar>
#include <iostream>
#include "ui_MainWindow.h"
#include "ImageWin.h"
#include "imagelist.h"
#include "PanelManager.h"
#include "OptionPageManager.h"
#include "OperationModeManager.h"
#include "ProjectFileManager.h"
#include "SegmentationOptionPage.h"
#include "UndoRedoManager.h"
//#include "PanoramaOptionParam.h"
#include "ElementPick.h"
using namespace std;


class ImageMatch;//ElementQuery
class AddElement;//add element to db
class Segmentation;
class PanoramaOptionParam;
class MainWindow : public QMainWindow
{
	Q_OBJECT

	friend class ImageWin;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

	static MainWindow& Instance(){return *pMainWin;}

	QMdiSubWindow* CreateSubWindow(QImage& m_image,const QString& m_title);//新建子窗口
	ImageWin* GetActiveSubWindow();

	const PanelManager* GetPanelManager(){return _panelmanager;}
	OperationModeManager* GetOperationModeManager(){return _operationmodemanager;}
	ProjectFileManager* GetProjectFileManager(){return _projectfilemanager;}
	const OptionPageManager* GetOptionManager(){return _optionPagemanager;}
	UndoRedoManager* GetUndoRedoManager(){return _undoRedoManager;}
	
	ElementPick*  GetElementPickModule(){return _modulepick;}
	Segmentation* GetSegmentationModule(){return _modulesegment;}

	ImageListProjectFileHandle* GetResourceImageFileHandle(){return _resourceFileHandle;}
	ImageListProjectFileHandle* GetElementImageFileHandle(){return _elementFileHandle;}

	/* create the progress bar reside in main window status bar, and return the progress bar instance */
	QProgressBar* create_panorama_progress_bar();
	void delete_panorama_progress_bar();

	Ui::MainWindow ui;//eye

public slots:
	void NewPrj();
	void SavePrj();//保存工程
	void SaveAsPrj();//另存工程
	void OpenPrj();//打开工程
	void closePrj();//关闭工程
	void OpenRecentFile();

	void OpenImage(QString img_filePath);
	void ZoomOut();
	void ZoomIn();
	void ImportImage();
	void onViewInActualPix();
	void onViewInScreen();

	void onActionClose();
	void onActionCloseAll();
	void onActionTile();
	void onActionCascade();
	void onActionNext();
	void onActionPrevious();
	void onSaveInpainting();

	void ShowOptionPanel();//显示选项卡面板
	//void OpenElementQuery();//打开elementquery面板
	//void OpenAddElement();//打开添加到数据库面板
	//void AddElementToPrj();//添加到工程
	//void OpenPanorama();//打开全景图面板
	//void OpenBigImage();//打开大图像浏览器
	void mixbmp();
	void UndoAct();
	void RedoAct();

	void JudgeEditState();
	void UpdateProjectFileStatus(bool bModified);

protected:
	void closeEvent ( QCloseEvent * event );

private slots:
	
	void onOperationActionToggled(bool checked);
	void onToolMenuAboutToShow();	//在工具菜单打开的时候调用

private:
	void self_init();
	void panel_init();
	void module_init();
	
	void toolbar_init();
	void recent_init();
	void activeRelevantPanel();
	int  popupSaveMessageBox();
	bool preCloseProject();//关闭工程 false 表示 cancel
	void destroyAllSubWindow();

private:
	PanelManager* _panelmanager;	//面板管理器
	OptionPageManager* _optionPagemanager;//选项管理器
	ProjectFileManager* _projectfilemanager;//工程文件管理器
	UndoRedoManager * _undoRedoManager;//恢复、撤销管理器
	OperationModeManager* _operationmodemanager;//操作模式管理器

	ImageListProjectFileHandle* _resourceFileHandle;
	ImageListProjectFileHandle* _elementFileHandle;

//Module
	Segmentation* _modulesegment;
	ImageMatch* _modulequery;
	AddElement* _addelement;
	PanoramaOptionParam* _modulepan;
	ElementPick* _modulepick;

	std::map<QString, QMdiSubWindow*> img_subwindow_map;

	static MainWindow* pMainWin;

	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	void setRecentCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);

	/* progress bar add in status bar for panorama */
	QProgressBar* panorama_progress_bar;
};

#define CURRENT_MODE (MainWindow::Instance().GetOperationModeManager()->mode())

#endif // __ES_DHAPP_MAINWINDOW_H__
