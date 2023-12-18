#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void newDirectories();

    void copyFiles();

    void changeAutoExec();

    QString getNewDirectory();

    QString getSourcePath();

    QString getNewDirectoryTwo();

    void changeHudLayoutRes(QString path);

    std::vector<std::string> listAllFiles(QString path);

    void deleteLastLine(const std::string& filePath);

    //REMOVING TRANSPARENT FUNCTIONS.
    void removeTransparentFiles();

    void removeAutoExecCommands();

    void removeHudLayoutCommands();

    void removeTransparentDirectory();



private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
