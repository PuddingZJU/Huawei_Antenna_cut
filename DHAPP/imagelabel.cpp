#include"imagelabel.h"
#include <QGridLayout>
ImageLabel::ImageLabel()
{
    nameLabel=new QLabel;
    imageCheckBox=new QCheckBox;

    QGridLayout *layout=new QGridLayout;

    layout->addWidget(nameLabel,0,0);
    layout->addWidget(imageCheckBox,1,0);


    setLayout(layout);
}

void ImageLabel::setFileNameText(const QString &filename)
{
    imageCheckBox->setText(filename);
}

void ImageLabel::setIcon(QIcon &icon)
{
    imageCheckBox->setIcon(icon);
}

void ImageLabel::setIconSize(QSize &size)
{
    imageCheckBox->setIconSize(size);
}
