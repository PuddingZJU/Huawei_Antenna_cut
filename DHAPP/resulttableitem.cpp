#include"resulttableitem.h"
ResultTableItem::ResultTableItem()
{

    setCheckState(Qt::Checked);
    filePath="";

}

void ResultTableItem::setFilePath(QString &path)
{
    filePath=path;
}

QString & ResultTableItem::getFilePath()
{
    return filePath;
}
