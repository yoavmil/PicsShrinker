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
    void on_resolutionSlider_sliderMoved(int position);
    void processFinished();

private:
    Ui::MainWindow *ui;
    QVector<ImageData> files;

    void populateFilesAsync(QString rootDir);
    void updateProgressBarRange();

    static MainWindow* inst;
};

#endif // MAINWINDOW_H
