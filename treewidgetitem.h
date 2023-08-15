#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QTreeWidgetItem>

class TreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    TreeWidgetItem(QTreeWidget *parent);
    TreeWidgetItem(QTreeWidgetItem *parent);

    const QString& filePath() const { return path; }
    void setFilePath(const QString& path) { this->path = path;}
private:
    QString path;
};

#endif // TREEWIDGETITEM_H
