#include "listwidget.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QQueue>
#include <QApplication>
#include <QHeaderView>

TreeWidget::TreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    this->setColumnCount(1);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setAutoScroll(true);
    this->header()->setVisible(false);
    this->setFrameStyle(QFrame::Sunken);
    this->setAnimated(true);

    connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem* curr, QTreeWidgetItem* prev){
        this->_currentItem = dynamic_cast<TreeWidgetItem*>(curr);

    });
}

void TreeWidget::addDirPath(const QString &path)
{
    dirPath = path;
    QDir dir(dirPath);
    if (dir.exists() == false)
    {
        dir.mkpath(dir.path());
        return;
    }

    TreeWidgetItem *root;
    root = new TreeWidgetItem(this);
    rootList.append(root);
    QApplication::processEvents();
    this->addTopLevelItem(root);
    root->setText(0, dir.dirName());
    DFS(&dir, root);
    root->setExpanded(true);
    _currentItem = root;
    this->setCurrentItem(_currentItem);
}

void TreeWidget::DFS(QDir *path, TreeWidgetItem *parent)
{
    QApplication::processEvents();
    QStringList list = path->entryList();
    QFileInfoList infoList = path->entryInfoList();

    for (int i = 2; i < list.size(); i++)
    {
        QFileInfo f = infoList[i];
        if (f.isFile())
        {
            TreeWidgetItem *item = new TreeWidgetItem(parent);
            item->setFilePath(f.filePath());
            item->setText(0, f.fileName());
        }
        else if (f.isDir())
        {
            QDir *next = new QDir(f.filePath());
            TreeWidgetItem *item = new TreeWidgetItem(parent);
            item->setFilePath(f.filePath());
            item->setText(0, list[i]);
            DFS(next, item);
        }
    }
}
