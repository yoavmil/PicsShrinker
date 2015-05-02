#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDirIterator>
#include <QImageReader>
#include <QDebug>
#include <QFutureWatcher>
#include <qtconcurrentmap.h>
#include <QImageWriter>

MainWindow* MainWindow::inst;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    inst = this;
    ui->setupUi(this);
    on_resolutionSlider_sliderMoved(ui->resolutionSlider->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFolderBtn_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "Select Root Folder");
    if (dirName.isEmpty())
        return;
    files.clear();
    populateFilesAsync(dirName);

    long totalSize = 0;
    for (int i = 0; i < files.size(); i++) {
        totalSize += files[i].origFileSize;
    }
    qDebug() << "found" << files.size() << "files with total size" << totalSize;
    updateProgressBarRange();
    ui->rootDirLabel->setText(dirName);
    on_overwriteCheckBox_clicked();
    if (ui->outputDirLineEdit->text().isEmpty()) {
        ui->outputDirLineEdit->setFocus();
    }
    ui->startStopBtn->setEnabled(files.size() > 0);
    ui->progressBar->setEnabled(files.size() > 0);
    if (files.size() == 0) {
        this->statusBar()->setToolTip("input directory has no image files");
    }
}
void shrink(ImageData& id);
void MainWindow::on_startStopBtn_clicked()
{
    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), this, SLOT(processFinished()));
    //QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), ui->progressBar, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), ui->progressBar, SLOT(setValue(int)));

    // Start the computation.
    //futureWatcher.setFuture(QtConcurrent::map(files, Shrink));
    for (int i = 0; i < files.size(); i++) {
        Shrink(files[i]);
    }

    //futureWatcher.waitForFinished();
}

void MainWindow::on_overwriteCheckBox_clicked()
{
    ui->outputDirLineEdit->setEnabled(ui->overwriteCheckBox->isChecked());
    if (ui->overwriteCheckBox->isChecked()) {
        ui->outputDirLineEdit->setText(ui->rootDirLabel->text());
    }
}

void MainWindow::populateFilesAsync(QString rootDir)
{
    statusBar()->setToolTip(QString("searching at %1").arg(rootDir));
    QDir dir(rootDir);
    QFileInfoList filesIL = dir.entryInfoList(QDir::Files);
    QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < filesIL.size(); i++) {
        QImageReader ir(filesIL[i].absoluteFilePath());
        if (!ir.canRead())
            continue;
        files.append(ImageData(filesIL[i].absoluteFilePath()));
    }
    for (int i = 0; i < subdirs.size(); i++) {
        populateFilesAsync(subdirs[i].absoluteFilePath());
    }
}

void MainWindow::updateProgressBarRange()
{
    ui->progressBar->setMaximum(files.size());
}

void MainWindow::Shrink(ImageData &id)
{
    qDebug() << id.path;
    QImage img(id.path);
    QImageWriter iw;
    iw.setQuality(ui->resolutionSlider->value());
    QString newName = id.path;
    newName.replace(ui->rootDirLabel->text(), ui->outputDirLineEdit->text());
    iw.setFileName(newName);
    iw.write(img);
}

void shrink(ImageData &id)
{
    MainWindow::Inst()->Shrink(id);
}


ImageData::ImageData(QString _path):
    path(_path)
{
    QFileInfo fi(path);
    origFileSize = fi.size();
    qDebug() << "found" << path << "sz" << origFileSize;
}

ImageData::ImageData(const ImageData &other)
{
    path = other.path;
    origFileSize = other.origFileSize;
}

void MainWindow::on_resolutionSlider_sliderMoved(int position)
{
    QString str = QString("%1%").arg(position);
    ui->resolutionLabel->setText(str);
}

void MainWindow::processFinished()
{
    qDebug()  << __FUNCTION__;

}
