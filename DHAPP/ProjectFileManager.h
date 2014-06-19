#ifndef __ES_DHAPP_PROJECTFILEMANAGER_H__
#define __ES_DHAPP_PROJECTFILEMANAGER_H__

#include <set>
#include <vector>
#include <QString>
#include <QObject>

class QDomDocument;
class ProjectFileInterface;
class ProjectFileManager
	: public QObject
{
	Q_OBJECT
public:
	ProjectFileManager(void);
	~ProjectFileManager(void);

	QString name();
	QString path();
	QString getFilePath();
	bool needSave(){return !setToSaveHandle.empty();}

	void newfile(const QString& path);
	void load(const QString& path);
	void save();
	void saveas(const QString& path);

	void addHandle(ProjectFileInterface* handle);
	void reset();

protected:
	void initDomDocument();

signals:
	void modified(bool);

protected slots:
	void onModified();

private:
	QString filepath;

	QDomDocument* document;
	std::vector<ProjectFileInterface*> vecHandle;
	std::set<ProjectFileInterface*> setToSaveHandle;
};

#endif