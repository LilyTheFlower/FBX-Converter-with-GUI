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
private slots:
    void on_sourceFolderPushButton_clicked();

    void on_logFolderPushButton_clicked();

    void on_destinationFolderPushButton_clicked();

    void on_enableLoggingCheckbox_stateChanged(int arg1);

    void closeEvent (QCloseEvent *event);

    void on_replaceOriginalsCheckBox_stateChanged(int arg1);

    void saveStringtoSettings(std::string recordID, std::string data);

    std::string readStringFromSettings(std::string recordID);

    void populateTable();
    void on_selectPushButton_clicked();

    void on_fbxFileTableWidget_itemSelectionChanged();

    void on_conversionPushButton_clicked();

    static void onPrintLogFailure();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
