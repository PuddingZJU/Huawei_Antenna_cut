/********************************************************************************
** Form generated from reading UI file 'SegmentationParamPanel.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTATIONPARAMPANEL_H
#define UI_SEGMENTATIONPARAMPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SegmentationParamPanel
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QSlider *dot_width_slider;
    QLabel *dot_label_num;
    QLabel *dot_label;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QComboBox *comboBox_obj;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QComboBox *comboBox_bkg;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QComboBox *big_image_seg;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *excute_bt;
    QPushButton *reset_bt;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_getmask;
    QPushButton *pushButton_getfront;

    void setupUi(QWidget *SegmentationParamPanel)
    {
        if (SegmentationParamPanel->objectName().isEmpty())
            SegmentationParamPanel->setObjectName(QString::fromUtf8("SegmentationParamPanel"));
        SegmentationParamPanel->resize(176, 328);
        verticalLayout = new QVBoxLayout(SegmentationParamPanel);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_5 = new QGroupBox(SegmentationParamPanel);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        dot_width_slider = new QSlider(groupBox_5);
        dot_width_slider->setObjectName(QString::fromUtf8("dot_width_slider"));
        dot_width_slider->setMinimum(1);
        dot_width_slider->setMaximum(25);
        dot_width_slider->setPageStep(5);
        dot_width_slider->setValue(10);
        dot_width_slider->setOrientation(Qt::Horizontal);
        dot_width_slider->setTickPosition(QSlider::TicksBelow);
        dot_width_slider->setTickInterval(4);

        gridLayout_5->addWidget(dot_width_slider, 0, 0, 1, 2);

        dot_label_num = new QLabel(groupBox_5);
        dot_label_num->setObjectName(QString::fromUtf8("dot_label_num"));
        dot_label_num->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dot_label_num->setFrameShape(QFrame::NoFrame);
        dot_label_num->setFrameShadow(QFrame::Plain);
        dot_label_num->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(dot_label_num, 1, 0, 1, 1);

        dot_label = new QLabel(groupBox_5);
        dot_label->setObjectName(QString::fromUtf8("dot_label"));
        dot_label->setMinimumSize(QSize(0, 25));
        dot_label->setStyleSheet(QString::fromUtf8(""));
        dot_label->setFrameShape(QFrame::NoFrame);
        dot_label->setFrameShadow(QFrame::Plain);
        dot_label->setMidLineWidth(1);
        dot_label->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(dot_label, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox_5);

        groupBox = new QGroupBox(SegmentationParamPanel);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, -1, -1);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        comboBox_obj = new QComboBox(groupBox);
        comboBox_obj->setObjectName(QString::fromUtf8("comboBox_obj"));

        horizontalLayout->addWidget(comboBox_obj);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        comboBox_bkg = new QComboBox(groupBox);
        comboBox_bkg->setObjectName(QString::fromUtf8("comboBox_bkg"));

        horizontalLayout_2->addWidget(comboBox_bkg);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, -1, -1, -1);
        label = new QLabel(SegmentationParamPanel);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        big_image_seg = new QComboBox(SegmentationParamPanel);
        big_image_seg->setObjectName(QString::fromUtf8("big_image_seg"));

        horizontalLayout_3->addWidget(big_image_seg);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        excute_bt = new QPushButton(SegmentationParamPanel);
        excute_bt->setObjectName(QString::fromUtf8("excute_bt"));
        excute_bt->setCheckable(false);
        excute_bt->setChecked(false);
        excute_bt->setAutoRepeat(true);

        horizontalLayout_7->addWidget(excute_bt);

        reset_bt = new QPushButton(SegmentationParamPanel);
        reset_bt->setObjectName(QString::fromUtf8("reset_bt"));

        horizontalLayout_7->addWidget(reset_bt);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton_getmask = new QPushButton(SegmentationParamPanel);
        pushButton_getmask->setObjectName(QString::fromUtf8("pushButton_getmask"));

        horizontalLayout_4->addWidget(pushButton_getmask);

        pushButton_getfront = new QPushButton(SegmentationParamPanel);
        pushButton_getfront->setObjectName(QString::fromUtf8("pushButton_getfront"));

        horizontalLayout_4->addWidget(pushButton_getfront);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(SegmentationParamPanel);

        QMetaObject::connectSlotsByName(SegmentationParamPanel);
    } // setupUi

    void retranslateUi(QWidget *SegmentationParamPanel)
    {
        SegmentationParamPanel->setWindowTitle(QApplication::translate("SegmentationParamPanel", "SegmentationParamPanel", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("SegmentationParamPanel", "\347\254\224\345\210\267\345\244\247\345\260\217", 0, QApplication::UnicodeUTF8));
        dot_label_num->setText(QString());
        dot_label->setText(QString());
        groupBox->setTitle(QApplication::translate("SegmentationParamPanel", "\347\254\224\345\210\267\345\275\242\347\212\266", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SegmentationParamPanel", "\345\211\215\346\231\257", 0, QApplication::UnicodeUTF8));
        comboBox_obj->clear();
        comboBox_obj->insertItems(0, QStringList()
         << QApplication::translate("SegmentationParamPanel", "\347\202\271", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationParamPanel", "\347\237\251\345\275\242", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("SegmentationParamPanel", "\350\203\214\346\231\257", 0, QApplication::UnicodeUTF8));
        comboBox_bkg->clear();
        comboBox_bkg->insertItems(0, QStringList()
         << QApplication::translate("SegmentationParamPanel", "\347\202\271", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationParamPanel", "\347\237\251\345\275\242", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("SegmentationParamPanel", "\345\244\247\345\233\276\345\203\217\345\210\206\345\211\262", 0, QApplication::UnicodeUTF8));
        big_image_seg->clear();
        big_image_seg->insertItems(0, QStringList()
         << QApplication::translate("SegmentationParamPanel", "\347\274\251\346\224\276\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationParamPanel", "\346\214\207\345\256\232\345\214\272\345\237\237", 0, QApplication::UnicodeUTF8)
        );
        excute_bt->setText(QApplication::translate("SegmentationParamPanel", "\346\211\247\350\241\214", 0, QApplication::UnicodeUTF8));
        excute_bt->setShortcut(QApplication::translate("SegmentationParamPanel", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        reset_bt->setText(QApplication::translate("SegmentationParamPanel", "\351\207\215\347\275\256", 0, QApplication::UnicodeUTF8));
        pushButton_getmask->setText(QApplication::translate("SegmentationParamPanel", "\345\257\274\345\207\272mask", 0, QApplication::UnicodeUTF8));
        pushButton_getfront->setText(QApplication::translate("SegmentationParamPanel", "\345\257\274\345\207\272\351\201\256\346\214\241", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SegmentationParamPanel: public Ui_SegmentationParamPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTATIONPARAMPANEL_H
