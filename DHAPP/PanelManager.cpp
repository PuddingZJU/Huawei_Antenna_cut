#include "PanelManager.h"

#include <QDockWidget>
#include <QMainWindow>
#include <QMenu>

PanelManager::PanelManager(QMainWindow* win, QMenu* toggleMenu)
:_window(win),
_menu(toggleMenu),
_menuInsertPos(NULL)
{
	if(_menu)
	{
		_menuInsertPos = _menu->actions().isEmpty() ? NULL : _menu->actions()[0];
	}
}

PanelManager::~PanelManager(void)
{
}

void PanelManager::RegisterPanel(const QString& classname, const QString& name, QWidget* panel, Qt::DockWidgetArea area)
{
	if (!_window)	return;

	QDockWidget* dock = new QDockWidget(name, _window);
	dock->setObjectName(name + "_dock");
	dock->setWidget(panel);

	if (_classMap.find(classname) != _classMap.end())
	{
		_window->tabifyDockWidget(_classMap.value(classname), dock);
	}
	else
	{
		_window->addDockWidget(area, dock);
	}
	_classMap[classname] = dock;
	_dockMap[name] = dock;

	if (_menu)
	{
		if (classname.isEmpty())
		{
			_menu->insertAction(_menuInsertPos, dock->toggleViewAction());
		}
		else
		{
			QList<QAction*> curactions = _menu->actions();
			QList<QAction*>::const_iterator it = curactions.begin();
			for (; it != curactions.end(); it++)
			{
				if ((*it)->text() == classname)
				{
					(*it)->menu()->addAction(dock->toggleViewAction());
					break;
				}
			}
			if (it == curactions.end())
			{
				QMenu* submenu = new QMenu(classname, _window);
				_menu->insertMenu(_menuInsertPos, submenu);
				submenu->addAction(dock->toggleViewAction());
			}
		}
	}
}

QWidget* PanelManager::GetPanel (const QString& name)const
{
	return _dockMap[name]->widget();
}

void PanelManager::ActivePanel (const QString& name)const
{
	QDockWidget* dockwidget = _dockMap[name];
	dockwidget->setVisible(true);
	dockwidget->setFocus();
	dockwidget->raise();
}

QAction* PanelManager::toggleViewAction (const QString& name)const
{
	return _dockMap[name]->toggleViewAction();
}