#ifndef __ES_DHAPP_PROJECTFILEINTERFACE_H
#define __ES_DHAPP_PROJECTFILEINTERFACE_H

#include <QObject>

class QDomDocument;
class ProjectFileInterface : public QObject
{
	Q_OBJECT
public:
	/** Load the project file.
	*/
	virtual void loadProjectFile(const QDomDocument*) = 0;

	/** Save the project file.
	*/
	virtual void saveProjectFile(QDomDocument*) = 0;

signals:
	/** When you have loaded the project, then emit the signal.
	* Usually used to refresh related panel.
	*/
	void loaded();

	/** When you have modified the project, then emit the signal.
	* Usually used to give user a tip to remember to save the project.
	*/
	void modified();	
};

#endif