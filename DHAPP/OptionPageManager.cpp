#include "optionpagemanager.h"

OptionPageManager::OptionPageManager(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init_component();
	init_connection();
}

OptionPageManager::~OptionPageManager()
{

}
void OptionPageManager::init_component()
{
	this->_OptionPageTree = ui.OptionPageTree;
	this->_OptionPageTree->setHeaderHidden(true);//隐藏标头
	this->_OptionPageTree->setColumnCount(1);
	this->_stackedOptionPage = ui.stackedOptionPage;
}
void OptionPageManager::init_connection()
{
	QObject::connect(this->_OptionPageTree,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this,SLOT(CurrentIndexChange(QTreeWidgetItem *, QTreeWidgetItem *)));
	QObject::connect(this,SIGNAL(CurrentIndex(int)),this->_stackedOptionPage,SLOT(setCurrentIndex(int)));

	QObject::connect(ui.OK,SIGNAL(clicked()),this,SLOT(OKAndClose()));
	QObject::connect(ui.Cancel,SIGNAL(clicked()),this,SLOT(CancelAndClose()));
}
void OptionPageManager::CurrentIndexChange(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	QString name = current->text(0);
	emit CurrentIndex(_PageMap[name]);

}
void OptionPageManager::RegisterPage(const QString &name, QWidget *Page)
{

	int index = this->_stackedOptionPage->addWidget(Page);

	QTreeWidgetItem * item = new QTreeWidgetItem(QStringList(name));

	this->_OptionPageTree->addTopLevelItem(item);

	this->_PageMap[name] = index;


}
QWidget* OptionPageManager::GetPage(const QString &name) const
{
	int index = _PageMap[name];
	qDebug()<<name<<index;
	return this->_stackedOptionPage->widget(index);
	
}
void OptionPageManager::OKAndClose()
{
	emit OK();
	this->hide();
}
void OptionPageManager::CancelAndClose()
{
	emit Cancel();
	this->hide();
}