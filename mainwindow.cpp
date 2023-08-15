#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listwidget.h"
#include "treewidgetitem.h"

#include <QLabel>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    treeViewDockWidget = new QDockWidget("Files", this);
    textBrowserDockWidget = new QDockWidget("preview", this);
    textEditDockWidget = new QDockWidget("edit", this);
    textEdit = new QTextEdit();
    textBrowser = new QTextBrowser();
    treeWidget = new TreeWidget();

    treeViewDockWidget->setWidget(treeWidget);
    textBrowserDockWidget->setWidget(textBrowser);
    textEditDockWidget->setWidget(textEdit);

    addDockWidget(Qt::LeftDockWidgetArea, treeViewDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, textBrowserDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, textEditDockWidget);
    this->setCentralWidget(nullptr);

    QMenu *dirMenu = new QMenu();
    dirMenu->addAction("NewFile");
    dirMenu->addAction("NewDir");
    dirMenu->addAction("Rename")->setData(1);

    QMenu *fileMenu = new QMenu();
    fileMenu->addAction("Rename")->setData(2);

    auto func = [this](QAction* action){
        TreeWidgetItem *curr = this->treeWidget->currentItem();
        if (action->text() == "NewFile")
        {
            QString name = QInputDialog::getText(this, "NewFile", "FileName");
            if (name.isEmpty()) return;
            QFile file(name); file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.close();

            TreeWidgetItem *item = new TreeWidgetItem(curr);
            item->setFilePath(curr->filePath() + "/" + name);
            item->setText(0, name);
        }
        if (action->text() == "NewDir")
        {
            QString name = QInputDialog::getText(this, "NewDir", "DirName");
            if (name.isEmpty()) return;
            QDir dir(curr->filePath() + "/" + name);
            dir.mkpath(dir.path());

            TreeWidgetItem *item = new TreeWidgetItem(curr);
            item->setFilePath(dir.path());
            item->setText(0, name);
        }
        if (action->text() == "Rename")
        {
            if (action->data() == 1)
            {
                QString name =QInputDialog::getText(this, "Rename", "NewName");
                QString filePath = curr->filePath();
                QDir oldDir(filePath);
                QString newPath = filePath + "/../" + name;
                QDir newDir(newPath);
                if (oldDir.rename(filePath, newPath))
                {
                    curr->setFilePath(newDir.path());
                    curr->setText(0, name);
                }
            }
            else if (action->data() == 2)
            {
                QString name =QInputDialog::getText(this, "Rename", "NewName");
                QString filePath = curr->filePath();
                QFileInfo info(filePath);
                if (QFile(filePath).rename(info.path() + "/" + name))
                {
                    curr->setFilePath(info.path() + "/" + name);
                    curr->setText(0, name);
                }
            }
        }

    };
    connect(dirMenu, &QMenu::triggered, func);
    connect(fileMenu, &QMenu::triggered, func);
    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::updateText);
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int column){
        const QString& fileName = dynamic_cast<TreeWidgetItem*>(item)->filePath();
        this->showFile(fileName);
    });
    connect(treeWidget, &QTreeWidget::itemPressed, [dirMenu, fileMenu](QTreeWidgetItem *item, int column){
        if (qApp->mouseButtons() == Qt::RightButton)
        {
            QFileInfo info(dynamic_cast<TreeWidgetItem*>(item)->filePath());
            if (info.isDir())
            {
                dirMenu->exec(QCursor::pos());
            }
            else if (info.isFile())
            {
                fileMenu->exec(QCursor::pos());
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFile(const QString& filePath)
{
    QFileInfo fileinfo(filePath);
    currentFile = fileinfo;
    if (fileinfo.isDir())
    {
        return;
    }
    QFile file(fileinfo.filePath());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString text = in.readAll();
    this->textEdit->setText(text);

    file.close();
}

void MainWindow::updateText()
{
    textBrowser->setMarkdown(textEdit->document()->toMarkdown());
}


void MainWindow::on_actionSave_triggered()
{
    QFile file(currentFile.filePath());
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << textEdit->document()->toPlainText();
}


void MainWindow::on_actionOpenDir_triggered()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "Open");
    qDebug()<<dirPath;
    this->treeWidget->addDirPath(dirPath);
}

