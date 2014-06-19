#ifndef __ES_DHAPP_PANELMANAGER_H__
#define __ES_DHAPP_PANELMANAGER_H__

#include <QAction>
#include <QString>
#include <QMap>

class QWidget;
class QDockWidget;
class QMainWindow;
class QMenu;
class PanelManager
{
public:
	PanelManager(QMainWindow* win, QMenu* toggleMenu = NULL);
	~PanelManager(void);

	void RegisterPanel(const QString& classname, const QString& name, QWidget* panel, Qt::DockWidgetArea area);
	QWidget* GetPanel(const QString& name) const;

	void ActivePanel(const QString& name) const;
	QAction* toggleViewAction(const QString& name) const;

private:
	QMainWindow* _window;
	QMenu* _menu;
	QAction* _menuInsertPos;

	QMap<QString, QDockWidget*> _dockMap;
	QMap<QString, QDockWidget*> _classMap;
};

#endif