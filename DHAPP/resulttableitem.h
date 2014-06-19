#ifndef RESULTTABLEITEM_H
#define RESULTTABLEITEM_H
#include <QTableWidgetItem>
/*为表格中每个单元格增加一个图片路径数据*/
class ResultTableItem: public QTableWidgetItem
{
public:
    ResultTableItem();
    void setFilePath(QString &path);
    QString& getFilePath();
private:
    QString filePath;
};
#endif // RESULTTABLEITEM_H
