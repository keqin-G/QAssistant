#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QTreeWidget>
#include "treewidgetitem.h"
#include <QDir>

class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    TreeWidget(QWidget *parent = nullptr);
    void addDirPath(const QString& path);
    void DFS(QDir *path, TreeWidgetItem *parent);
    TreeWidgetItem* currentItem() const { return _currentItem; };

private:
    QString dirPath;
    QList<TreeWidgetItem *> rootList;
    TreeWidgetItem* _currentItem;
    int currentColumn;

};

#endif // LISTWIDGET_H
