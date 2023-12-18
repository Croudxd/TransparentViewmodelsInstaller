#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <direct.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <QDir>
#include <QString>
#include <ostream>
#include <string.h>
#include <filesystem>
#include <QMessageBox>

namespace fs = std::filesystem;



QString path;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this),
    this->setFixedSize(800,500);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_textChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Add

void MainWindow::on_pushButton_clicked()
{
    // Check if the path is not empty before changing the directory
    if (!path.isEmpty())
    {
        // Convert QString to std::string and then to C-style string
        std::string sPath = path.toStdString();
        const char* cPath = sPath.c_str();

        // Change the current working directory
        if (_chdir(cPath) == 0)
        {
            //In tf2 folder
            _chdir("tf");
            //in tf folder
            if(_chdir("cfg") == 0)
            {
                //in cfg folder
                changeAutoExec();
                if(_chdir("..")==0)
                {

                }
                //back to tf
            } else
            {
                std::cerr << "Error changing directory: " << strerror(errno) << std::endl;
            }
            if(_chdir("custom") == 0)
                //change into custom folder.
            {
                char* buffer;

                // Get the current working directory:
                if ( (buffer = _getcwd( NULL, 0 )) == NULL )
                    perror( "_getcwd error" );
                else
                {
                    QString currentDirectory = QString::fromStdString(buffer);
                    changeHudLayoutRes(currentDirectory);
                    free(buffer);
                }
                newDirectories();
                //creates new directories, cd into all of them leaving working directory in thumbnails.
                copyFiles();
                //copysFiles into this folder.
            } else
            {
                _mkdir("custom");
                //if cant open custom create custom folder.
                if(chdir("custom") == 0)
                {
                    newDirectories();
                    copyFiles();
                }
            }
        } else
        {
            std::cerr << "Error changing directory: " << strerror(errno) << std::endl;
        }
    }
    else
    {
        std::cerr << "Path is empty." << std::endl;
    }
}


//create new directory. cd into all of them leaving cwd in thumbnails.
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


//This adds to path entered by user, will then add folders it needs to copy files into. returning path.
QString MainWindow::getNewDirectory(){
    QStringList directoriesToAdd;
    directoriesToAdd << "tf" << "custom" << "transparent" << "materials" << "VGUI" << "replay" << "thumbnails" << "REFRACTnormal_transparent.vmt";

    QString qPath = path;

    // Use QDir to handle directory separators
    QDir dir(qPath);
    for (const QString& directory : directoriesToAdd)
    {
        qPath = dir.filePath(directory);
        dir.cd(directory);
    }

    std::wcout << qPath.toStdWString();
    return qPath;
}

QString MainWindow::getNewDirectoryTwo(){
    QStringList directoriesToAdd;
    directoriesToAdd << "tf" << "custom" << "transparent" << "materials" << "VGUI" << "replay" << "thumbnails" <<  "REFRACTnormal_transparent.vtf";

    QString qPath = path;

    // Use QDir to handle directory separators
    QDir dir(qPath);
    for (const QString& directory : directoriesToAdd)
    {
        qPath = dir.filePath(directory);
        dir.cd(directory);
    }
    return qPath;
}

//copies files from current source path to new destination source path (thumbnails folder).
void MainWindow::copyFiles()
{
    wchar_t sourceFilePath[MAX_PATH];
    wchar_t destinationFilePathOne[MAX_PATH];
    wchar_t destinationFilePathTwo[MAX_PATH];

    wchar_t fileOnePath[MAX_PATH];
    wchar_t fileTwoPath[MAX_PATH];

    QString sourcePath = getSourcePath();
    QString filePathOne = QDir::cleanPath(sourcePath + "/REFRACTnormal_transparent.vmt");
    QString filePathTwo = QDir::cleanPath(sourcePath + "/REFRACTnormal_transparent.vtf");


    std::wstring fileOne = filePathOne.toStdWString();
    if (wcsncpy(fileOnePath, fileOne.c_str(), MAX_PATH) == nullptr)
    {
        return;
    }


    std::wstring fileTwo = filePathTwo.toStdWString();
    if (wcsncpy(fileTwoPath, fileTwo.c_str(), MAX_PATH) == nullptr)
    {
        return;
    }

    std::cout << filePathOne.toStdString() << std::endl;
    std::cout << filePathTwo.toStdString() << std::endl;

    QString qDestinationOne = getNewDirectory();
    QString qDestinationTwo = getNewDirectoryTwo();
    qDestinationOne.toWCharArray(destinationFilePathOne);
    qDestinationTwo.toWCharArray(destinationFilePathTwo);


    if (GetModuleFileName(nullptr, sourceFilePath, MAX_PATH) == 0)
    {
        std::cerr << "Error getting the path of the executable." << std::endl;
        return;
    }

    // Copy fileOnePath
    std::ifstream sourceFileOne(fileOnePath, std::ios::binary);
    std::ofstream destinationFileOne(destinationFilePathOne, std::ios::binary);

    if (sourceFileOne && destinationFileOne)
    {
        destinationFileOne << sourceFileOne.rdbuf();
    } else {
        std::cerr << "Failed to copy File One" << std::endl;
    }

    // Copy fileTwoPath
    std::ifstream sourceFileTwo(fileTwoPath, std::ios::binary);
    std::wofstream destinationFileTwo(destinationFilePathTwo, std::ios::binary | std::ios::app); // Use append mode for the second file

    if (sourceFileTwo && destinationFileTwo)
    {
        destinationFileTwo << sourceFileTwo.rdbuf();
    } else
    {
        std::wcerr << "Failed to copy File Two" << std::endl;
    }

    if (sourceFileOne.fail() || destinationFileOne.fail())
    {
        std::wcerr << "Error opening source file One for reading." << std::endl;
        return;
    }

    if (sourceFileTwo.fail() || destinationFileTwo.fail())
    {
        std::wcerr << "Error opening destination file One for writing." << std::endl;

        return;
    }

    sourceFileOne.close();
    sourceFileTwo.close();
    sourceFileTwo.close();
    destinationFileOne.close();
    destinationFileTwo.close();
   }

std::vector<std::string> MainWindow::listAllFiles(QString path)
{
    std::vector<std::string> names;
    std::string sPath = path.toStdString();

    for (const auto& entry : fs::recursive_directory_iterator(sPath))
    {
        if (entry.is_regular_file())
        {
            names.push_back(entry.path().string());
        }
    }
    return names;
}

void MainWindow::deleteLastLine(const std::string& filePath)
{
    std::ifstream inputFile(filePath);
    std::vector<std::string> lines;

    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            lines.push_back(line);
        }
        inputFile.close();

        if (!lines.empty())
        {
            lines.pop_back();  // Remove the last line
            std::ofstream outputFile(filePath, std::ios::trunc);

            for (const std::string& newLine : lines)
            {
                outputFile << newLine << '\n';
            }

            outputFile.close();
        }
        else
        {
            std::cerr << "File is empty: " << filePath << std::endl;
        }
    }
    else
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
}



   void MainWindow::changeHudLayoutRes(QString path)
{
    std::vector<std::string> filePaths = listAllFiles(path);

    for (const std::string& filePath : filePaths)
    {
        if (fs::path(filePath).filename() == "HudLayout.res")
        {
            // Open and modify the hudlayout.res file as needed
            std::ofstream file(filePath, std::ios::app);
            if (file.is_open())
            {
                deleteLastLine(filePath);
                // Your modification code here
                const char *commands = R""""(
    "TransparentViewmodelMask"
    {
        //alpha doesn't work for this, you need to change the texture's alpha
        "ControlName"   "ImagePanel"
        "fieldName"        "TransparentViewmodelMask"
        "xpos"          "0"
        "ypos"          "0"
        "zpos"          "-100"
        "wide"          "f0"
        "tall"          "480"
        "visible"       "1"
        "enabled"       "1"
        "image"            "replay/thumbnails/REFRACTnormal_transparent"
        "scaleImage"    "1"
    }
}
                )"""";
                file << commands;

                file.close();
            }
            else
            {
                std::cerr << "Error opening file: " << filePath << std::endl;
            }

            // If you found and modified the file, you might want to break out of the loop here.
            // If you want to process all instances, remove the break statement.
            break;
        }
    }
}
//finds file called autoexec, adds commands to the file. If not one there will create one.
void MainWindow::changeAutoExec()
{
    const char* fileName = "autoexec.cfg";
    //find autoexec in custom

        std::ofstream outFile(fileName);
        if(outFile.is_open())
        {
            //change autoexec/add commands.
        }
        std::ifstream inFile(fileName);
        if(inFile.is_open())
        {
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
            if (outFile.is_open())
            {
                outFile << content;
            } else
            {
                std::cerr << "Error opening autoexec.cfg for writing." << std::endl;
            }
        } else
        {
            std::cerr << "Error opening autoexec.cfg for reading." << std::endl;
        }
    }




void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    //gets path from user.
    path = arg1;

}

//C:\Program Files (x86)\Steam\steamapps\common\Team Fortress 2


//Remove


void MainWindow::on_pushButton_2_clicked()
{
    removeTransparentFiles();
}




void MainWindow::removeTransparentFiles(){
    //use all functions below to complete remove transparent viewmodels.
    removeAutoExecCommands();
    removeHudLayoutCommands();
    removeTransparentDirectory();

}

void MainWindow::removeAutoExecCommands(){
    if (!path.isEmpty())
    {
        // Convert QString to std::string and then to C-style string
        std::string sPath = path.toStdString();
        const char* cPath = sPath.c_str();

        // Change the current working directory
        if (_chdir(cPath) == 0)
        {
            //In tf2 folder
            _chdir("tf");
            //in tf folder
            if(_chdir("cfg") == 0)
            {
                const char* fileName = "autoexec.cfg";
                std::ofstream outFile(fileName);
                if(outFile.is_open())
                {
                    //change autoexec/add commands.
                }
                std::ifstream inFile(fileName);
                if(inFile.is_open())
                {
                    deleteLastLine(fileName);
                }
                } else
                    {
                        std::cerr << "Error opening autoexec.cfg for writing." << std::endl;
                    }

                if(_chdir("..")==0)
                {

                }
                //back to tf
            } else
            {
                std::cerr << "Error changing directory: " << strerror(errno) << std::endl;
            }
    }
}



void MainWindow::removeHudLayoutCommands(){
    std::vector<std::string> filePaths = listAllFiles(path);
    if(_chdir("custom") == 0){

        for (const std::string& filePath : filePaths)
        {
            if (fs::path(filePath).filename() == "HudLayout.res")
            {
                // Open and modify the hudlayout.res file as needed
                std::ofstream file(filePath, std::ios::app);
                if (file.is_open())
                {
                    for(int i = 0; i < 17; i++){
                    deleteLastLine(filePath);
                    }
                    std::string deleteCommand = "}";
                    file << deleteCommand;
                    file.close();
                    if(_chdir("..") ==0){

                    } else {
                        std::cerr << "Error changing back a directory" << std::endl;
                    }

                }
                else
                {
                    std::cerr << "Error opening file: " << filePath << std::endl;
                }

                // If you found and modified the file, you might want to break out of the loop here.
                // If you want to process all instances, remove the break statement.
                break;
            }
        }
    } else {
        std::cerr << "Error finding custom folder: " << std::endl;
    }
}

void MainWindow::removeTransparentDirectory(){
if(_chdir("custom") == 0)
    {
        if(fs::remove_all("transparent") == true){
            //delete transparent folders, good to go.
        } else {
            std::cerr << "Error transparent folders" << std::endl;
        }
    }
}

