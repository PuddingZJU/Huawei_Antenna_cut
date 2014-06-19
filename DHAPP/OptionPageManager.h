#ifndef OPTIONPageMANAGER_H
#define OPTIONPageMANAGER_H

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include <QString>
#include <QMap>
#include <QDebug>
#include "ui_optionpagemanager.h"

class OptionPageManager : public QWidget
{
	Q_OBJECT

public:
	OptionPageManager(QWidget *parent = 0);
	~OptionPageManager();

	void RegisterPage(const QString& name, QWidget* Page);//name存在treeWidget中，Page存在StackedWidget
	QWidget* GetPage(const QString& name) const;

private:
	Ui::OptionPageManager ui;
	QTreeWidget * _OptionPageTree;
	QStackedWidget * _stackedOptionPage;
	QMap<QString, int> _PageMap;
private:
	void init_component();
	void init_connection();
signals:
	void CurrentIndex(int index);
	void OK();
	void Cancel();
private slots:
	void CurrentIndexChange(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void CancelAndClose();
	void OKAndClose();
};

#endif // OPTIONPageMANAGER_H
