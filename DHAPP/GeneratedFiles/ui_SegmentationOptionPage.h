/********************************************************************************
** Form generated from reading UI file 'SegmentationOptionPage.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTATIONOPTIONPAGE_H
#define UI_SEGMENTATIONOPTIONPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SegmentationOptionPage
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QPushButton *ObjColorBt;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QPushButton *BkgColorBt;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *MethodComboBox;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QLineEdit *lamda_lineEdit;
    QLabel *label_5;
    QLineEdit *kappa_lineEdit;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_13;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *use_log_input;
    QRadioButton *use_canny_input;
    QGroupBox *groupBox_10;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *canny_checkbox;
    QCheckBox *canny_erode_checkbox;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_5;
    QRadioButton *linear_radioButton;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *nolinear_radioButton;
    QComboBox *comboBox;
    QGroupBox *groupBox_8;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *edge_weight_lineEdit;
    QGroupBox *CannyGroupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_7;
    QLineEdit *canny_min_edit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QLineEdit *canny_max_edit;

    void setupUi(QWidget *SegmentationOptionPage)
    {
        if (SegmentationOptionPage->objectName().isEmpty())
            SegmentationOptionPage->setObjectName(QString::fromUtf8("SegmentationOptionPage"));
        SegmentationOptionPage->resize(440, 649);
        verticalLayout = new QVBoxLayout(SegmentationOptionPage);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(SegmentationOptionPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        ObjColorBt = new QPushButton(groupBox);
        ObjColorBt->setObjectName(QString::fromUtf8("ObjColorBt"));
        ObjColorBt->setAutoFillBackground(false);
        ObjColorBt->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0);"));
        ObjColorBt->setCheckable(false);
        ObjColorBt->setChecked(false);
        ObjColorBt->setAutoDefault(false);
        ObjColorBt->setFlat(false);

        horizontalLayout_6->addWidget(ObjColorBt);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_7->addWidget(label_4);

        BkgColorBt = new QPushButton(groupBox);
        BkgColorBt->setObjectName(QString::fromUtf8("BkgColorBt"));
        BkgColorBt->setAutoFillBackground(false);
        BkgColorBt->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        BkgColorBt->setCheckable(false);
        BkgColorBt->setChecked(false);
        BkgColorBt->setAutoDefault(false);
        BkgColorBt->setDefault(false);
        BkgColorBt->setFlat(false);

        horizontalLayout_7->addWidget(BkgColorBt);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(SegmentationOptionPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 32));
        QFont font;
        font.setPointSize(9);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        MethodComboBox = new QComboBox(SegmentationOptionPage);
        MethodComboBox->setObjectName(QString::fromUtf8("MethodComboBox"));
        MethodComboBox->setMinimumSize(QSize(0, 32));
        MethodComboBox->setFont(font);

        horizontalLayout->addWidget(MethodComboBox);


        verticalLayout->addLayout(horizontalLayout);

        groupBox_3 = new QGroupBox(SegmentationOptionPage);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 0, 1, 1);

        lamda_lineEdit = new QLineEdit(groupBox_3);
        lamda_lineEdit->setObjectName(QString::fromUtf8("lamda_lineEdit"));

        gridLayout_2->addWidget(lamda_lineEdit, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        kappa_lineEdit = new QLineEdit(groupBox_3);
        kappa_lineEdit->setObjectName(QString::fromUtf8("kappa_lineEdit"));

        gridLayout_2->addWidget(kappa_lineEdit, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        stackedWidget = new QStackedWidget(SegmentationOptionPage);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_2->setContentsMargins(0, -1, 0, -1);
        groupBox_13 = new QGroupBox(page);
        groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
        groupBox_13->setMinimumSize(QSize(0, 80));
        verticalLayout_6 = new QVBoxLayout(groupBox_13);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        use_log_input = new QRadioButton(groupBox_13);
        use_log_input->setObjectName(QString::fromUtf8("use_log_input"));
        use_log_input->setChecked(true);

        verticalLayout_6->addWidget(use_log_input);

        use_canny_input = new QRadioButton(groupBox_13);
        use_canny_input->setObjectName(QString::fromUtf8("use_canny_input"));

        verticalLayout_6->addWidget(use_canny_input);


        verticalLayout_2->addWidget(groupBox_13);

        groupBox_10 = new QGroupBox(page);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        groupBox_10->setMinimumSize(QSize(0, 80));
        verticalLayout_7 = new QVBoxLayout(groupBox_10);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        canny_checkbox = new QCheckBox(groupBox_10);
        canny_checkbox->setObjectName(QString::fromUtf8("canny_checkbox"));

        verticalLayout_7->addWidget(canny_checkbox);

        canny_erode_checkbox = new QCheckBox(groupBox_10);
        canny_erode_checkbox->setObjectName(QString::fromUtf8("canny_erode_checkbox"));
        canny_erode_checkbox->setEnabled(false);
        canny_erode_checkbox->setCheckable(true);

        verticalLayout_7->addWidget(canny_erode_checkbox);


        verticalLayout_2->addWidget(groupBox_10);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_8 = new QVBoxLayout(page_2);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, -1, 0, -1);
        groupBox_7 = new QGroupBox(page_2);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setMinimumSize(QSize(0, 80));
        verticalLayout_5 = new QVBoxLayout(groupBox_7);
        verticalLayout_5->setSpacing(9);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(9, 9, 9, -1);
        linear_radioButton = new QRadioButton(groupBox_7);
        linear_radioButton->setObjectName(QString::fromUtf8("linear_radioButton"));
        linear_radioButton->setChecked(true);

        verticalLayout_5->addWidget(linear_radioButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        nolinear_radioButton = new QRadioButton(groupBox_7);
        nolinear_radioButton->setObjectName(QString::fromUtf8("nolinear_radioButton"));

        horizontalLayout_2->addWidget(nolinear_radioButton);

        comboBox = new QComboBox(groupBox_7);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_2->addWidget(comboBox);


        verticalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_8->addWidget(groupBox_7);

        groupBox_8 = new QGroupBox(page_2);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_8);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupBox_8);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        edge_weight_lineEdit = new QLineEdit(groupBox_8);
        edge_weight_lineEdit->setObjectName(QString::fromUtf8("edge_weight_lineEdit"));

        horizontalLayout_3->addWidget(edge_weight_lineEdit);


        verticalLayout_8->addWidget(groupBox_8);

        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);

        CannyGroupBox = new QGroupBox(SegmentationOptionPage);
        CannyGroupBox->setObjectName(QString::fromUtf8("CannyGroupBox"));
        CannyGroupBox->setMinimumSize(QSize(0, 80));
        verticalLayout_4 = new QVBoxLayout(CannyGroupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(12);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_7 = new QLabel(CannyGroupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_5->addWidget(label_7);

        canny_min_edit = new QLineEdit(CannyGroupBox);
        canny_min_edit->setObjectName(QString::fromUtf8("canny_min_edit"));

        horizontalLayout_5->addWidget(canny_min_edit);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(12);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_8 = new QLabel(CannyGroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_4->addWidget(label_8);

        canny_max_edit = new QLineEdit(CannyGroupBox);
        canny_max_edit->setObjectName(QString::fromUtf8("canny_max_edit"));

        horizontalLayout_4->addWidget(canny_max_edit);


        verticalLayout_4->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(CannyGroupBox);

        stackedWidget->raise();
        CannyGroupBox->raise();
        groupBox_3->raise();
        groupBox->raise();

        retranslateUi(SegmentationOptionPage);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SegmentationOptionPage);
    } // setupUi

    void retranslateUi(QWidget *SegmentationOptionPage)
    {
        SegmentationOptionPage->setWindowTitle(QApplication::translate("SegmentationOptionPage", "SegmentationOptionPage", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SegmentationOptionPage", "\347\254\224\345\210\267\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SegmentationOptionPage", "\345\211\215\346\231\257", 0, QApplication::UnicodeUTF8));
        ObjColorBt->setText(QString());
        ObjColorBt->setShortcut(QApplication::translate("SegmentationOptionPage", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SegmentationOptionPage", "\350\203\214\346\231\257", 0, QApplication::UnicodeUTF8));
        BkgColorBt->setText(QString());
        BkgColorBt->setShortcut(QApplication::translate("SegmentationOptionPage", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SegmentationOptionPage", "\345\210\206\345\211\262\347\256\227\346\263\225", 0, QApplication::UnicodeUTF8));
        MethodComboBox->clear();
        MethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("SegmentationOptionPage", "GraphCut", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationOptionPage", "GrabCut", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationOptionPage", "LazyBrush", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationOptionPage", "Mixture", 0, QApplication::UnicodeUTF8)
        );
        groupBox_3->setTitle(QApplication::translate("SegmentationOptionPage", "\345\271\263\346\273\221\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("SegmentationOptionPage", "lamda", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        lamda_lineEdit->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        lamda_lineEdit->setInputMask(QString());
        lamda_lineEdit->setText(QApplication::translate("SegmentationOptionPage", "1.5", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("SegmentationOptionPage", "kappa", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        kappa_lineEdit->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        kappa_lineEdit->setInputMask(QString());
        kappa_lineEdit->setText(QApplication::translate("SegmentationOptionPage", "4.0", 0, QApplication::UnicodeUTF8));
        groupBox_13->setTitle(QApplication::translate("SegmentationOptionPage", "Edge\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        use_log_input->setText(QApplication::translate("SegmentationOptionPage", "\344\275\277\347\224\250LoG\344\275\234\344\270\272\350\276\223\345\205\245", 0, QApplication::UnicodeUTF8));
        use_canny_input->setText(QApplication::translate("SegmentationOptionPage", "\344\275\277\347\224\250Canny\344\275\234\344\270\272\350\276\223\345\205\245", 0, QApplication::UnicodeUTF8));
        groupBox_10->setTitle(QApplication::translate("SegmentationOptionPage", "Canny\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        canny_checkbox->setText(QApplication::translate("SegmentationOptionPage", "\344\275\277\347\224\250Canny\345\242\236\345\274\272\350\276\271\347\274\230", 0, QApplication::UnicodeUTF8));
        canny_erode_checkbox->setText(QApplication::translate("SegmentationOptionPage", "\345\257\271Canny\347\273\223\346\236\234Erode", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("SegmentationOptionPage", "\347\273\204\345\220\210\346\226\271\345\274\217", 0, QApplication::UnicodeUTF8));
        linear_radioButton->setText(QApplication::translate("SegmentationOptionPage", "\347\272\277\346\200\247", 0, QApplication::UnicodeUTF8));
        nolinear_radioButton->setText(QApplication::translate("SegmentationOptionPage", "\351\235\236\347\272\277\346\200\247", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("SegmentationOptionPage", "\345\271\263\346\226\271", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SegmentationOptionPage", "\347\253\213\346\226\271", 0, QApplication::UnicodeUTF8)
        );
        groupBox_8->setTitle(QApplication::translate("SegmentationOptionPage", "\346\267\267\345\220\210\346\257\224\351\207\215", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SegmentationOptionPage", "LazyBrush\347\256\227\346\263\225\346\257\224\351\207\215", 0, QApplication::UnicodeUTF8));
        CannyGroupBox->setTitle(QApplication::translate("SegmentationOptionPage", "Canny\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("SegmentationOptionPage", "canny\346\234\200\345\260\217\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        canny_min_edit->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        canny_min_edit->setInputMask(QString());
        canny_min_edit->setText(QApplication::translate("SegmentationOptionPage", "60", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("SegmentationOptionPage", "canny\346\234\200\345\244\247\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        canny_max_edit->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        canny_max_edit->setInputMask(QString());
        canny_max_edit->setText(QApplication::translate("SegmentationOptionPage", "180", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SegmentationOptionPage: public Ui_SegmentationOptionPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTATIONOPTIONPAGE_H
