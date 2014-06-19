#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include <QWidget>

#include <QCheckBox>
#include <QLabel>

class ImageLabel : public QWidget
{
public:
    ImageLabel();
    void setFileNameText(const QString &filename);
    void setIcon(QIcon &icon);
    void setIconSize(QSize & size);
private:
    QCheckBox *imageCheckBox;
    QLabel *nameLabel;

};
#endif // IMAGELABEL_H
