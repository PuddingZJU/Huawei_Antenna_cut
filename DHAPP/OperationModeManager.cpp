#include "OperationModeManager.h"

#include "MainWindow.h"

#include <QIcon>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>

OperationModeManager::OperationModeManager(void)
{
	toolbar_ = new QToolBar(QObject::tr("操作模式"));
	toolbar_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	MainWindow::Instance().addToolBar(Qt::LeftToolBarArea, toolbar_);

	actgroup_ = new QActionGroup(toolbar_);

	for (int i = 0; i < MODECOUNT; i++)
	{
		actarray_[i] = NULL;
	}
}

OperationModeManager::~OperationModeManager(void)
{
}

void OperationModeManager::addOperationMode(OperationMode opermode, QAction* act)
{
	if (act == NULL)
	{
		return;
	}

	static QFont font(QObject::tr("宋体"), 6);

	act->setFont(font);
	act->setCheckable(true);
	actgroup_->addAction(act);
	toolbar_->addAction(act);

	QObject::connect(act, SIGNAL(toggled(bool)), &MainWindow::Instance(), SLOT(onOperationActionToggled(bool)));

	if (opermode < MODECOUNT && opermode >= 0)
	{
		actarray_[opermode] = act;
	}	
}

int OperationModeManager::mode() const
{
	QAction* curact = actgroup_->checkedAction();
	if (curact)
	{		
		for (int i = 0; i < MODECOUNT; i++)
		{
			if (actarray_[i] == curact)
			{
				return i;
			}
		}
	}
	return INVALIDMODE;
}

bool OperationModeManager::setMode(OperationMode opermode)
{
	if (opermode == INVALIDMODE)
	{
		QAction* curact = actgroup_->checkedAction();
		if(curact)
			curact->setChecked(false);
		return true;
	}
	else if (opermode < MODECOUNT && opermode >= 0 && actarray_[opermode])
	{
		actarray_[opermode]->setChecked(true);
		return true;
	}
	return false;
}