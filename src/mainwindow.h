#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <vector>
#include <string>
#include <QStringList>
#include <QStringListModel>
#include <QMessageBox>
#include <QCloseEvent>
#include "filemanager/filemanager.h"
#include "core/audio.h"

namespace Audio
{
    class Workspace;
    class Track;
    class Audiofile;
    class FilePart;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void closeWindow(QCloseEvent *event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void set_playback_lcd(double value);

private slots:
    void on_loadFile_btn_clicked(); //Load file from drive

    void on_unloadFile_btn_clicked();   //Unload file from project

    void on_comboBox_currentIndexChanged(const QString &arg1);  //Reorganize combo box and info window

    void on_addTrack_btn_clicked(); //Add track

    void on_play_btn_clicked();

    void on_stop_btn_clicked();

    void on_addSound2Track_btn_clicked();   //Load file to current track

private:
    void AdjustComboBox(const QString &name);
    void AdjuctDragNDrop(bool flag); //true when filesModel, false when deviceModel;

    Ui::MainWindow *ui;
    Audio::Workspace *workspace;

    QStringListModel *filesModel;
    QStringListModel *devicesModel;
    QStringList filesList;
    QStringList devicesList;
    int currModel; //0 - files, 1 - devices
};

#endif // MAINWINDOW_H
