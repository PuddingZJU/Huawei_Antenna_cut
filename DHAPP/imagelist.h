#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QTreeWidget>
#include <QFileDialog>
#include <iostream>
#include <QDebug>
using namespace std;

class ImageListProjectFileHandle;
class ImageList : public QTreeWidget
{
	Q_OBJECT

public:
	ImageList(ImageListProjectFileHandle* filehandle = 0, QWidget *parent = 0);
	~ImageList();

public:
	//void addResources(QStringList m_hierarchys);//添加多个资源
	void addResource(QString m_hierarchy,QString m_Resource);//添加单个资源,m_hierarchy以逗号分割:敦煌,全景图。m_Resource是图片的路径:D:/xxx/xxx.jpg

	/* update the image list check state before get the checked image file */
	void updateImageListCheckState();

signals:
	void imageDoubleClicked(QString);

private slots:
	void refresh();

	QSize getScaledSize(QSize img_size);

	void DisplayImage(QTreeWidgetItem * item,int column);

private:
	int icon_w;
	int icon_h;//用于图像的缩放
	ImageListProjectFileHandle* filehandle_;
};

#endif // IMAGELIST_H
