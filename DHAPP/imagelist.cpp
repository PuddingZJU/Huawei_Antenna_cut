#include "imagelist.h"

#include "ImageListProjectFileHandle.h"
#include <QBitmap>
#include <QImageReader>

ImageList::ImageList(ImageListProjectFileHandle* filehandle, QWidget *parent)
: filehandle_(filehandle),
  QTreeWidget(parent)
{
	//init();
	icon_w = icon_h = 100;
	this->setIconSize(QSize(icon_w,icon_h));
	this->setHeaderHidden(true);
	connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int )),this,SLOT(DisplayImage(QTreeWidgetItem * ,int )));//双击打开图片

	if (filehandle_)
	{
		connect(filehandle_, SIGNAL(loaded()), this, SLOT(refresh()));
		connect(filehandle_, SIGNAL(modified()), this, SLOT(refresh()));
	}
}

ImageList::~ImageList()
{

}

void ImageList::refresh()
{
	if (filehandle_)
	{
		updateImageListCheckState();
		this->clear();

		const ImageListFiles& imagefiles = filehandle_->getImages();
		for (ImageListFiles::const_iterator it = imagefiles.begin(); it != imagefiles.end(); it++)
		{
			QTreeWidgetItem* diritem = new QTreeWidgetItem(QStringList(it->first));
			addTopLevelItem(diritem);
			QString hierarchi_name = it->first;

			for(int index = 0; index < it->second.size(); ++index)
			{
				QString imgpath = it->second[index];
				QFileInfo imgfile = QFileInfo(imgpath);
				QString maskpath = imgfile.path() + QDir::separator() + imgfile.completeBaseName() + "_mask.png";
				QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(imgfile.completeBaseName()));

				/* set the total image path in UserRole, used in panorama image stitich */
				item->setData(0, Qt::UserRole, imgpath);

				/* set the check state according to previous check state */
				if(filehandle_->get_image_checked(hierarchi_name, index))
					item->setCheckState(0, Qt::Checked);
				else
					item->setCheckState(0, Qt::Unchecked);

				QIcon scaled_icon;
				if (QFileInfo(maskpath).exists())
				{
					QPixmap thumbnail(imgpath);
					QImage maskimg(maskpath);
					maskimg.invertPixels();
					thumbnail.setMask(QBitmap::fromImage(maskimg));
					scaled_icon = thumbnail.scaled(getScaledSize(thumbnail.size()), Qt::KeepAspectRatio);
				} 
				else 
				{
					QImageReader reader(imgpath);
					if(reader.supportsOption(QImageIOHandler::Size))
					{
						reader.setScaledSize(getScaledSize(reader.size()));
						scaled_icon = QPixmap::fromImageReader(&reader);
					}
					else
					{
						QPixmap img = QPixmap::fromImageReader(&reader);
						scaled_icon = img.scaled(getScaledSize(img.size()), Qt::KeepAspectRatio);
					}
				}

				/* set the scaled icon */
				item->setIcon(0, scaled_icon);
				item->setToolTip(0, imgpath);
				diritem->addChild(item);
			}
		}
		expandAll();
	}
}

void ImageList::DisplayImage(QTreeWidgetItem * item,int column)
{
	if(item->toolTip(column)!=NULL)
		emit imageDoubleClicked(item->toolTip(column));
}

void ImageList::addResource(QString m_hierarchy,QString m_Resource)
{
	if (filehandle_)
	{
		filehandle_->addImage(m_hierarchy, m_Resource);
		return;
	}

	QIcon thumbnail;
	QString filename;
	int index;
	QTreeWidgetItem * item;
	QList<QTreeWidgetItem *> f_items;//匹配item
	double scaleFactor;
	QString toolTip = m_Resource;
	filename = QFileInfo(m_Resource).fileName();
	
	if (m_hierarchy.isEmpty())//表示只有图片一层
	{

		f_items = this->findItems(filename,Qt::MatchFixedString);

		if (f_items.size()<=0)//未在树中出现
		{

			item = new QTreeWidgetItem(QStringList(filename));

			QPixmap Pixmap(m_Resource);
			//scaleFactor = (icon_w/(double)Pixmap.width()) < (icon_h/double(Pixmap.height()))? (icon_w/double(Pixmap.width())) : (icon_h/double(Pixmap.height()));
			thumbnail = QIcon(Pixmap);//.scaled(Pixmap.width()*scaleFactor,Pixmap.height()*scaleFactor)
			item->setIcon(0,thumbnail);
			item->setToolTip(0,toolTip);
			this->insertTopLevelItem(0,item);
		}
		return;
	}

	QStringList hierarchy;
	

	hierarchy = m_hierarchy.split(".");
	f_items = this->findItems(hierarchy.at(0),Qt::MatchFixedString);
	if (f_items.size()<=0)
	{
		item = new QTreeWidgetItem(QStringList(hierarchy.at(0)));
		this->insertTopLevelItem(0,item);
	}
	else
		item = f_items.at(0);
	
	QTreeWidgetItem * item_child;
	int j;
	for (int i=1;i<hierarchy.size();i++)
	{
		
		for(j=0;j<item->childCount();j++)
		{

			if (item->child(j)->text(0)==hierarchy.at(i))//匹配字符串
			{
				item_child = item->child(j);
				break;
			}
			
		}
		if(j>=item->childCount())//第一次在树中出现
		{
			item_child = new QTreeWidgetItem(QStringList(hierarchy.at(i)));

			item->addChild(item_child);
			//展开列表
			this->expandItem(item);
		}

		item = item_child;
	}


	for(j=0;j<item->childCount();j++)
	{

		if (item->child(j)->text(0)==filename)//匹配字符串
		{
			break;
		}

	}
	if(j>=item->childCount())//第一次在树中出现
	{
		item_child = new QTreeWidgetItem(QStringList(filename));
		QPixmap Pixmap(m_Resource);
		//scaleFactor = (icon_w/(double)Pixmap.width()) < (icon_h/double(Pixmap.height()))? (icon_w/double(Pixmap.width())) : (icon_h/double(Pixmap.height()));
		thumbnail = QIcon(Pixmap);//.scaled(Pixmap.width()*scaleFactor,Pixmap.height()*scaleFactor)
		item_child->setIcon(0,thumbnail);
		item_child->setToolTip(0,toolTip);
		item->addChild(item_child);
		//展开列表
		this->expandItem(item);
	}
	this->scrollToItem(item_child);

}

void ImageList::updateImageListCheckState()
{
	const ImageListFiles& imagefiles = filehandle_->getImages();

	for(int row = 0; row < this->topLevelItemCount(); ++row)
	{
		QTreeWidgetItem* row_item = this->topLevelItem(row);
		QString hierarcai_name = row_item->data(0, Qt::DisplayRole).toString();

		for(int col = 0; col < row_item->childCount(); ++col)
		{
			filehandle_->set_image_checked(hierarcai_name, col, row_item->child(col)->checkState(0) == Qt::Checked);
		} // Loop for each childItem
	} // Loop for topLevelItem
}

QSize ImageList::getScaledSize(QSize img_size)
{
	/* ensure the maximum length of the image is less than the max length of item icon */
	int max_length = qMax(icon_h, icon_w);
	int h = img_size.height();
	int w = img_size.width();
	double ratio = (double)h/w;
	if(h > w) 
	{
		h = max_length;
		w = h/ratio;
	}
	else
	{
		w = max_length;
		h = w*ratio;
	}
	return QSize(w,h);
}
