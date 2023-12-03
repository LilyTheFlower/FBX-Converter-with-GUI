#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool notifyUser();

private slots:
    void on_sourceFolderPushButton_clicked();

    void on_logFolderPushButton_clicked();

    void on_destinationFolderPushButton_clicked();

    void on_enableLoggingCheckbox_stateChanged(int arg1);

    void closeEvent (QCloseEvent *event);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
