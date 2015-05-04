#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

struct ImageData {
    ImageData(QString _path);
    ImageData(const ImageData& other);
    ImageData() : origFileSize(0) {}
    QString path;
    size_t origFileSize;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* Inst() {return inst;}
    void Shrink(ImageData& id);
    ~MainWindow();

private slots:
    void on_selectFolderBtn_clicked();
    void on_startStopBtn_clicked();
    void on_overwriteCheckBox_clicked();
    void on_qualitySlider_valueChanged(int value);
    void processFinished();
    void populatingFilesFinished();
    void filesAddedSlot(int howMuch);

signals:
    void filesAdded(int howMuch);

private:
    Ui::MainWindow *ui;
    QList<ImageData> files;

    void populateFilesInvoke();
    void populateFilesAsync(QString rootDir);
    void addFiles(const QList<ImageData>& newFiles);
    void updateProgressBarRange();

    static MainWindow* inst;
};

#endif // MAINWINDOW_H
