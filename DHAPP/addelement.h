#ifndef ADDELEMENT_H
#define ADDELEMENT_H

#include <QDialog>

namespace Ui {
	class AddElementClass;
}

class QImage;
class ProcessControl;

class AddElement : public QDialog
{
	Q_OBJECT

public:
	AddElement(QWidget *parent = 0);
	~AddElement();
	void addNewElement(QImage element,QImage elementMask);
signals:

private slots:
	void doReIndexing();

private:
	void updateUnIndexedNumber();

private:
	Ui::AddElementClass *ui;
	ProcessControl* pc;
	int maxLevel;
	int levelElementMaxNum;
};





#endif // ADDELEMENT_H
