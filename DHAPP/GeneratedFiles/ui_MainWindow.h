/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *createAct;
    QAction *openAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *optionAct;
    QAction *actionClose;
    QAction *actionCloseAll;
    QAction *actionTile;
    QAction *actionCascade;
    QAction *actionNext;
    QAction *actionPrevious;
    QAction *saveAsAct;
    QAction *image_import;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pastAct;
    QAction *zoom_in;
    QAction *zoom_out;
    QAction *viewInScreenAct;
    QAction *viewInActualPixAct;
    QAction *actionPanorama;
    QAction *actionElementQuery;
    QAction *actionLazyBrush;
    QAction *actionGrabCut;
    QAction *actionGraphCut;
    QAction *actionMixture;
    QAction *actionAddElement;
    QAction *actionOpenBigImage;
    QAction *actionAddElementToPrj;
    QAction *actionSaveInpainting;
    QAction *action_getmask;
    QAction *action_getFront;
    QAction *action_mix;
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QMdiArea *mdiArea;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *menu_import;
    QMenu *menu_recentFiles;
    QMenu *toolMenu;
    QMenu *segmentMenu;
    QMenu *helpMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1189, 883);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1024, 768));
        createAct = new QAction(MainWindow);
        createAct->setObjectName(QString::fromUtf8("createAct"));
        createAct->setMenuRole(QAction::TextHeuristicRole);
        openAct = new QAction(MainWindow);
        openAct->setObjectName(QString::fromUtf8("openAct"));
        closeAct = new QAction(MainWindow);
        closeAct->setObjectName(QString::fromUtf8("closeAct"));
        closeAllAct = new QAction(MainWindow);
        closeAllAct->setObjectName(QString::fromUtf8("closeAllAct"));
        saveAct = new QAction(MainWindow);
        saveAct->setObjectName(QString::fromUtf8("saveAct"));
        exitAct = new QAction(MainWindow);
        exitAct->setObjectName(QString::fromUtf8("exitAct"));
        helpAct = new QAction(MainWindow);
        helpAct->setObjectName(QString::fromUtf8("helpAct"));
        aboutAct = new QAction(MainWindow);
        aboutAct->setObjectName(QString::fromUtf8("aboutAct"));
        undoAct = new QAction(MainWindow);
        undoAct->setObjectName(QString::fromUtf8("undoAct"));
        redoAct = new QAction(MainWindow);
        redoAct->setObjectName(QString::fromUtf8("redoAct"));
        optionAct = new QAction(MainWindow);
        optionAct->setObjectName(QString::fromUtf8("optionAct"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionCloseAll = new QAction(MainWindow);
        actionCloseAll->setObjectName(QString::fromUtf8("actionCloseAll"));
        actionTile = new QAction(MainWindow);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        actionCascade = new QAction(MainWindow);
        actionCascade->setObjectName(QString::fromUtf8("actionCascade"));
        actionNext = new QAction(MainWindow);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        actionPrevious = new QAction(MainWindow);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        saveAsAct = new QAction(MainWindow);
        saveAsAct->setObjectName(QString::fromUtf8("saveAsAct"));
        image_import = new QAction(MainWindow);
        image_import->setObjectName(QString::fromUtf8("image_import"));
        cutAct = new QAction(MainWindow);
        cutAct->setObjectName(QString::fromUtf8("cutAct"));
        copyAct = new QAction(MainWindow);
        copyAct->setObjectName(QString::fromUtf8("copyAct"));
        pastAct = new QAction(MainWindow);
        pastAct->setObjectName(QString::fromUtf8("pastAct"));
        zoom_in = new QAction(MainWindow);
        zoom_in->setObjectName(QString::fromUtf8("zoom_in"));
        zoom_out = new QAction(MainWindow);
        zoom_out->setObjectName(QString::fromUtf8("zoom_out"));
        viewInScreenAct = new QAction(MainWindow);
        viewInScreenAct->setObjectName(QString::fromUtf8("viewInScreenAct"));
        viewInActualPixAct = new QAction(MainWindow);
        viewInActualPixAct->setObjectName(QString::fromUtf8("viewInActualPixAct"));
        actionPanorama = new QAction(MainWindow);
        actionPanorama->setObjectName(QString::fromUtf8("actionPanorama"));
        actionElementQuery = new QAction(MainWindow);
        actionElementQuery->setObjectName(QString::fromUtf8("actionElementQuery"));
        actionLazyBrush = new QAction(MainWindow);
        actionLazyBrush->setObjectName(QString::fromUtf8("actionLazyBrush"));
        actionLazyBrush->setCheckable(true);
        actionLazyBrush->setChecked(false);
        actionGrabCut = new QAction(MainWindow);
        actionGrabCut->setObjectName(QString::fromUtf8("actionGrabCut"));
        actionGrabCut->setCheckable(true);
        actionGrabCut->setChecked(true);
        actionGraphCut = new QAction(MainWindow);
        actionGraphCut->setObjectName(QString::fromUtf8("actionGraphCut"));
        actionGraphCut->setCheckable(true);
        actionGraphCut->setChecked(false);
        actionMixture = new QAction(MainWindow);
        actionMixture->setObjectName(QString::fromUtf8("actionMixture"));
        actionMixture->setCheckable(true);
        actionAddElement = new QAction(MainWindow);
        actionAddElement->setObjectName(QString::fromUtf8("actionAddElement"));
        actionOpenBigImage = new QAction(MainWindow);
        actionOpenBigImage->setObjectName(QString::fromUtf8("actionOpenBigImage"));
        actionAddElementToPrj = new QAction(MainWindow);
        actionAddElementToPrj->setObjectName(QString::fromUtf8("actionAddElementToPrj"));
        actionSaveInpainting = new QAction(MainWindow);
        actionSaveInpainting->setObjectName(QString::fromUtf8("actionSaveInpainting"));
        action_getmask = new QAction(MainWindow);
        action_getmask->setObjectName(QString::fromUtf8("action_getmask"));
        action_getFront = new QAction(MainWindow);
        action_getFront->setObjectName(QString::fromUtf8("action_getFront"));
        action_mix = new QAction(MainWindow);
        action_mix->setObjectName(QString::fromUtf8("action_mix"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        mdiArea = new QMdiArea(centralwidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mdiArea->sizePolicy().hasHeightForWidth());
        mdiArea->setSizePolicy(sizePolicy1);
        mdiArea->setDocumentMode(false);

        gridLayout_3->addWidget(mdiArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1189, 21));
        menubar->setDefaultUp(false);
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        menu_import = new QMenu(fileMenu);
        menu_import->setObjectName(QString::fromUtf8("menu_import"));
        menu_recentFiles = new QMenu(fileMenu);
        menu_recentFiles->setObjectName(QString::fromUtf8("menu_recentFiles"));
        toolMenu = new QMenu(menubar);
        toolMenu->setObjectName(QString::fromUtf8("toolMenu"));
        segmentMenu = new QMenu(toolMenu);
        segmentMenu->setObjectName(QString::fromUtf8("segmentMenu"));
        helpMenu = new QMenu(menubar);
        helpMenu->setObjectName(QString::fromUtf8("helpMenu"));
        editMenu = new QMenu(menubar);
        editMenu->setObjectName(QString::fromUtf8("editMenu"));
        viewMenu = new QMenu(menubar);
        viewMenu->setObjectName(QString::fromUtf8("viewMenu"));
        windowMenu = new QMenu(menubar);
        windowMenu->setObjectName(QString::fromUtf8("windowMenu"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(editMenu->menuAction());
        menubar->addAction(toolMenu->menuAction());
        menubar->addAction(viewMenu->menuAction());
        menubar->addAction(windowMenu->menuAction());
        menubar->addAction(helpMenu->menuAction());
        fileMenu->addAction(createAct);
        fileMenu->addAction(openAct);
        fileMenu->addAction(menu_recentFiles->menuAction());
        fileMenu->addSeparator();
        fileMenu->addAction(closeAct);
        fileMenu->addAction(saveAct);
        fileMenu->addAction(saveAsAct);
        fileMenu->addSeparator();
        fileMenu->addAction(menu_import->menuAction());
        fileMenu->addSeparator();
        fileMenu->addAction(exitAct);
        menu_import->addAction(image_import);
        toolMenu->addAction(segmentMenu->menuAction());
        toolMenu->addAction(action_mix);
        segmentMenu->addAction(actionLazyBrush);
        segmentMenu->addAction(actionGrabCut);
        segmentMenu->addAction(actionGraphCut);
        segmentMenu->addAction(actionMixture);
        helpMenu->addAction(helpAct);
        helpMenu->addAction(aboutAct);
        editMenu->addAction(undoAct);
        editMenu->addAction(redoAct);
        editMenu->addSeparator();
        editMenu->addAction(cutAct);
        editMenu->addAction(copyAct);
        editMenu->addAction(pastAct);
        editMenu->addSeparator();
        editMenu->addAction(optionAct);
        viewMenu->addAction(zoom_in);
        viewMenu->addAction(zoom_out);
        viewMenu->addAction(viewInScreenAct);
        viewMenu->addAction(viewInActualPixAct);
        windowMenu->addSeparator();
        windowMenu->addAction(actionClose);
        windowMenu->addAction(actionCloseAll);
        windowMenu->addSeparator();
        windowMenu->addAction(actionTile);
        windowMenu->addAction(actionCascade);
        windowMenu->addSeparator();
        windowMenu->addAction(actionNext);
        windowMenu->addAction(actionPrevious);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\215\216\344\270\272\345\244\251\347\272\277\346\217\220\345\217\226\350\275\257\344\273\266", 0, QApplication::UnicodeUTF8));
        createAct->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272", 0, QApplication::UnicodeUTF8));
        createAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        openAct->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        openAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        closeAct->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
        closeAllAct->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        saveAct->setText(QApplication::translate("MainWindow", "\345\255\230\345\202\250", 0, QApplication::UnicodeUTF8));
        saveAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        exitAct->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        exitAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        helpAct->setText(QApplication::translate("MainWindow", "\345\270\256\345\212\251\344\270\273\351\242\230", 0, QApplication::UnicodeUTF8));
        helpAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        aboutAct->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216", 0, QApplication::UnicodeUTF8));
        undoAct->setText(QApplication::translate("MainWindow", "\346\222\244\351\224\200", 0, QApplication::UnicodeUTF8));
        undoAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        redoAct->setText(QApplication::translate("MainWindow", "\346\201\242\345\244\215", 0, QApplication::UnicodeUTF8));
        redoAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        optionAct->setText(QApplication::translate("MainWindow", "\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
        actionCloseAll->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\346\211\200\346\234\211", 0, QApplication::UnicodeUTF8));
        actionTile->setText(QApplication::translate("MainWindow", "\345\210\222\345\210\206\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        actionCascade->setText(QApplication::translate("MainWindow", "\351\207\215\345\217\240\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("MainWindow", "\344\270\213\344\270\200\344\270\252\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        actionPrevious->setText(QApplication::translate("MainWindow", "\344\270\212\344\270\200\344\270\252\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        saveAsAct->setText(QApplication::translate("MainWindow", "\345\255\230\345\202\250\344\270\272", 0, QApplication::UnicodeUTF8));
        image_import->setText(QApplication::translate("MainWindow", "\345\216\237\345\247\213\346\213\215\346\221\204\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8));
        cutAct->setText(QApplication::translate("MainWindow", "\345\211\252\345\210\207", 0, QApplication::UnicodeUTF8));
        cutAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        copyAct->setText(QApplication::translate("MainWindow", "\346\213\267\350\264\235", 0, QApplication::UnicodeUTF8));
        copyAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        pastAct->setText(QApplication::translate("MainWindow", "\347\262\230\350\264\264", 0, QApplication::UnicodeUTF8));
        pastAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        zoom_in->setText(QApplication::translate("MainWindow", "\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
        zoom_in->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0, QApplication::UnicodeUTF8));
        zoom_out->setText(QApplication::translate("MainWindow", "\347\274\251\345\260\217", 0, QApplication::UnicodeUTF8));
        zoom_out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        viewInScreenAct->setText(QApplication::translate("MainWindow", "\351\200\202\345\220\210\345\261\217\345\271\225", 0, QApplication::UnicodeUTF8));
        viewInActualPixAct->setText(QApplication::translate("MainWindow", "\345\256\236\351\231\205\345\203\217\347\264\240", 0, QApplication::UnicodeUTF8));
        viewInActualPixAct->setShortcut(QApplication::translate("MainWindow", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        actionPanorama->setText(QApplication::translate("MainWindow", "\346\213\274\346\216\245", 0, QApplication::UnicodeUTF8));
        actionElementQuery->setText(QApplication::translate("MainWindow", "\345\205\203\347\264\240\346\243\200\347\264\242", 0, QApplication::UnicodeUTF8));
        actionLazyBrush->setText(QApplication::translate("MainWindow", "Lazybrush", 0, QApplication::UnicodeUTF8));
        actionGrabCut->setText(QApplication::translate("MainWindow", "Grabcut", 0, QApplication::UnicodeUTF8));
        actionGraphCut->setText(QApplication::translate("MainWindow", "Graphcut", 0, QApplication::UnicodeUTF8));
        actionMixture->setText(QApplication::translate("MainWindow", "Mixture", 0, QApplication::UnicodeUTF8));
        actionAddElement->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\345\210\260\346\225\260\346\215\256\345\272\223", 0, QApplication::UnicodeUTF8));
        actionOpenBigImage->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200bigimage\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        actionAddElementToPrj->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\345\210\260\345\267\245\347\250\213", 0, QApplication::UnicodeUTF8));
        actionSaveInpainting->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\344\277\256\345\244\215\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8));
        action_getmask->setText(QApplication::translate("MainWindow", "\345\257\274\345\207\272mask", 0, QApplication::UnicodeUTF8));
        action_getFront->setText(QApplication::translate("MainWindow", "\345\257\274\345\207\272\345\211\215\346\231\257", 0, QApplication::UnicodeUTF8));
        action_mix->setText(QApplication::translate("MainWindow", "\345\220\210\345\271\266\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", 0, QApplication::UnicodeUTF8));
        menu_import->setTitle(QApplication::translate("MainWindow", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
        menu_recentFiles->setTitle(QApplication::translate("MainWindow", "\346\234\200\350\277\221\346\211\223\345\274\200\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        toolMenu->setTitle(QApplication::translate("MainWindow", "\345\267\245\345\205\267(&T)", 0, QApplication::UnicodeUTF8));
        segmentMenu->setTitle(QApplication::translate("MainWindow", "\345\205\203\347\264\240\345\210\206\345\211\262", 0, QApplication::UnicodeUTF8));
        helpMenu->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", 0, QApplication::UnicodeUTF8));
        editMenu->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", 0, QApplication::UnicodeUTF8));
        viewMenu->setTitle(QApplication::translate("MainWindow", "\350\247\206\345\233\276(&V)", 0, QApplication::UnicodeUTF8));
        windowMenu->setTitle(QApplication::translate("MainWindow", "\347\252\227\345\217\243(&W)", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "\345\267\245\345\205\267\346\240\217", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
