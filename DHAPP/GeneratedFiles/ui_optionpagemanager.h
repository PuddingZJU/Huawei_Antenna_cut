/********************************************************************************
** Form generated from reading UI file 'optionpagemanager.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONPAGEMANAGER_H
#define UI_OPTIONPAGEMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionPageManager
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *OptionPageTree;
    QSpacerItem *horizontalSpacer;
    QStackedWidget *stackedOptionPage;
    QWidget *page;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *OK;
    QPushButton *Cancel;

    void setupUi(QWidget *OptionPageManager)
    {
        if (OptionPageManager->objectName().isEmpty())
            OptionPageManager->setObjectName(QString::fromUtf8("OptionPageManager"));
        OptionPageManager->resize(652, 543);
        verticalLayout = new QVBoxLayout(OptionPageManager);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        OptionPageTree = new QTreeWidget(OptionPageManager);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        OptionPageTree->setHeaderItem(__qtreewidgetitem);
        OptionPageTree->setObjectName(QString::fromUtf8("OptionPageTree"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OptionPageTree->sizePolicy().hasHeightForWidth());
        OptionPageTree->setSizePolicy(sizePolicy);
        OptionPageTree->setMaximumSize(QSize(150, 16777215));

        horizontalLayout->addWidget(OptionPageTree);

        horizontalSpacer = new QSpacerItem(18, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        stackedOptionPage = new QStackedWidget(OptionPageManager);
        stackedOptionPage->setObjectName(QString::fromUtf8("stackedOptionPage"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedOptionPage->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedOptionPage->addWidget(page_2);

        horizontalLayout->addWidget(stackedOptionPage);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        OK = new QPushButton(OptionPageManager);
        OK->setObjectName(QString::fromUtf8("OK"));

        horizontalLayout_8->addWidget(OK);

        Cancel = new QPushButton(OptionPageManager);
        Cancel->setObjectName(QString::fromUtf8("Cancel"));

        horizontalLayout_8->addWidget(Cancel);


        verticalLayout->addLayout(horizontalLayout_8);


        retranslateUi(OptionPageManager);

        QMetaObject::connectSlotsByName(OptionPageManager);
    } // setupUi

    void retranslateUi(QWidget *OptionPageManager)
    {
        OptionPageManager->setWindowTitle(QApplication::translate("OptionPageManager", "\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        OK->setText(QApplication::translate("OptionPageManager", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Cancel->setText(QApplication::translate("OptionPageManager", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OptionPageManager: public Ui_OptionPageManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONPAGEMANAGER_H
