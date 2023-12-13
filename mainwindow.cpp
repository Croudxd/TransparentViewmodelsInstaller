#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <direct.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>

QString path;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_textChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Check if the path is not empty before changing the directory
    if (!path.isEmpty()) {
        // Convert QString to std::string and then to C-style string
        std::string sPath = path.toStdString();
        const char* cPath = sPath.c_str();

        // Change the current working directory
        if (_chdir(cPath) == 0) {
            std::cout << "Changed directory to: " << cPath << std::endl;
            _chdir("tf");

            if(_chdir("custom") == 0){
                changeAutoExec();
                newDirectories();
                copyFiles();
            } else {
                _mkdir("custom");
                newDirectories();
                copyFiles();
            }
        } else {
            std::cerr << "Error changing directory: " << strerror(errno) << std::endl;
        }
    } else {
        std::cerr << "Path is empty." << std::endl;
    }
}


//create new directory.
void newDirectories(){
    _mkdir("transparent");
    _mkdir("materials");
    _mkdir("VGUI");
    _mkdir("replay");
    _mkdir("thumbnails");
}
// Download files to directory

void copyFiles(){
    const wchar_t* sourceFilePath = "Path to executable";
    const wchar_t* sourceDestinationFilePath = L"where i am in thumbnails.";

    if (copyFile(sourceFilePath, (sourceDestinationFilePath + L"file.txt").c_str(), FALSE)){
        //sucess
    } else {
        //error.
    }
}

//

void changeHudLayoutRes(){
    //go through all custom folders and find hudlayout.res
    //change file to include commands.

}

void changeAutoExec(){
    const char* fileName = "autoexec.cfg";
    //find autoexec in custom
    if(_chdir("custom") == 0){
        std::ofstream outFile(fileName);
        if(outFile.is_open()){
            //change autoexec/add commands.
        }
    } else {
        _mkdir("custom");
        std::ifstream inFile(fileName);
        if(inFile.is_open()){
            std::stringstream buffer;
            buffer << inFile.rdbuf();
            std::string content = buffer.str();
            //change autoexec/add commands.
            content += "mat_motion_blur_enabled 1";
            content += "mat_motion_blur_strength 0 ";
            content += "mat_disable_bloom 1 ";
            content += "mat_hdr_level 0";
            content += "mat_colcorrection_disableentities 1";
            content += "mat_colorcorrection 0";
            inFile.close();
            std::ofstream outFile(fileName);
            if (outFile.is_open()) {
                outFile << content;

                std::cout << "Settings added to autoexec.cfg." << std::endl;
            } else {
                std::cerr << "Error opening autoexec.cfg for writing." << std::endl;
            }
        } else {
            std::cerr << "Error opening autoexec.cfg for reading." << std::endl;
        }
    }
 }


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    path = arg1;

}

//C:\Program Files (x86)\Steam\steamapps\common\Team Fortress 2
