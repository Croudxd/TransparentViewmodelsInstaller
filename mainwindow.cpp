#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <direct.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <Qdir>
#include <QString>
#include <ostream>


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
            //In tf2 folder
            _chdir("tf");
            //in tf folder
            if(_chdir("cfg") == 0){
                //in cfg folder
                changeAutoExec();
                if(_chdir("..")==0){

                } else {
                    std::cout << "Not going back a directory" << std::endl;
                }
                //back to tf
            } else {
                std::cerr << "Error changing directory: " << strerror(errno) << std::endl;
            }

            if(_chdir("custom") == 0){
                newDirectories();
                copyFiles();
                //this isnt getting called but should be called when we get out of cfg. So creating custom folder with new directories.
            } else {
                _mkdir("custom");
                if(chdir("custom") == 0){
                    newDirectories();
                    copyFiles();
                } else {
                    std::cout << "Something is fucked" << std::endl;
                }
            }
        } else {
            std::cerr << "Error changing directory: " << strerror(errno) << std::endl;
        }
    } else {
        std::cerr << "Path is empty." << std::endl;
    }
}


//create new directory.
void MainWindow::newDirectories(){
    _mkdir("transparent");
    _chdir("transparent");
    _mkdir("materials");
    _chdir("materials");
    _mkdir("VGUI");
    _chdir("VGUI");
    _mkdir("replay");
    _chdir("replay");
    _mkdir("thumbnails");
    _chdir("thumbnails");
}
// Download files to directory
QString MainWindow::getNewDirectory(){
    QStringList directoriesToAdd;
    directoriesToAdd << "tf" << "custom" << "transparent" << "materials" << "VGUI" << "replay" << "thumbnails";

    QString qPath = path;

    // Use QDir to handle directory separators
    QDir dir(qPath);
    for (const QString& directory : directoriesToAdd) {
        qPath = dir.filePath(directory);
        dir.cd(directory);
    }

    std::wcout << qPath.toStdWString();
    return qPath;
}


void MainWindow::copyFiles(){
    wchar_t sourceFilePath[MAX_PATH];
    wchar_t destinationFilePath[MAX_PATH];
    wchar_t fileOnePath[MAX_PATH];
    wchar_t fileTwoPath[MAX_PATH];

    QString sourcePath = getSourcePath();
    QString filePathOne = QDir::cleanPath(sourcePath + "/REFRACTnormal_transparent.vmt");
    QString filePathTwo = QDir::cleanPath(sourcePath + "/REFRACTnormal_transparent.vtf");


    std::wstring fileOne = filePathOne.toStdWString();
    if (wcsncpy(fileOnePath, fileOne.c_str(), MAX_PATH) == nullptr) {
        return;
    }


    std::wstring fileTwo = filePathTwo.toStdWString();
    if (wcsncpy(fileTwoPath, fileTwo.c_str(), MAX_PATH) == nullptr) {
        return;
    }

    std::cout << filePathOne.toStdString() << std::endl;
    std::cout << filePathTwo.toStdString() << std::endl;

    QString qDestination = getNewDirectory();
    qDestination.toWCharArray(destinationFilePath);

    if (GetModuleFileName(nullptr, sourceFilePath, MAX_PATH) == 0) {
        std::cerr << "Error getting the path of the executable." << std::endl;
        return;
    }

    // Copy fileOnePath
    std::ifstream sourceFileOne(fileOnePath, std::ios::binary);
    std::wofstream destinationFileOne(destinationFilePath, std::ios::binary);
    std::wcout << "Destination Directory: " << qDestination.toStdWString() << std::endl;

    if (sourceFileOne && destinationFileOne) {
        destinationFileOne << sourceFileOne.rdbuf();
        std::wcout << "File One copied successfully" << std::endl;
    } else {
        std::wcout << "Failed to copy File One" << std::endl;
    }

    // Copy fileTwoPath
    std::ifstream sourceFileTwo(fileTwoPath, std::ios::binary);
    std::wofstream destinationFileTwo(destinationFilePath, std::ios::binary | std::ios::app); // Use append mode for the second file

    if (sourceFileTwo && destinationFileTwo) {
        destinationFileTwo << sourceFileTwo.rdbuf();
        std::wcout << "File Two copied successfully" << std::endl;
    } else {
        std::wcout << "Failed to copy File Two" << std::endl;
    }

    if (!sourceFileOne.is_open()) {
        std::wcerr << "Error opening source file One for reading." << std::endl;
        return;
    }

    if (!destinationFileOne.is_open()) {
        std::wcerr << "Error opening destination file One for writing." << std::endl;
        sourceFileOne.close();
        return;
    }
   }


//

void changeHudLayoutRes(){
    //go through all custom folders and find hudlayout.res
    //change file to include commands.

}

void MainWindow::changeAutoExec(){
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
