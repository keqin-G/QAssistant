#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>
#include <QTextBrowser>
#include <QFile>
#include <QShortcut>
#include <QListWidget>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TreeWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QDockWidget *textEditDockWidget;
    QDockWidget *textBrowserDockWidget;
    QDockWidget *treeViewDockWidget;
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    TreeWidget *treeWidget;
    QFileSystemModel *fileSystem;
    QFileInfo currentFile;



private slots:
    void showFile(const QString& filePath);
    void updateText();

    void on_actionSave_triggered();

    void on_actionOpenDir_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
