#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define _CRT_SECURE_NO_WARNINGS
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QMainWindow>
#include "ppm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void load_image_clicked();
    void save_image_clicked();
    void updown_reverse_image_clicked();
    void leftright_reverse_image_clicked();
    void negative_image_clicked();

    void chennel_submit_clicked();
    void brightness_image_clicked();
    void rotate_image_clicked();

    void crop_image_clicked();
    void contrast_clicked();
    void waterMark_clicked();
    void waterMark_loaded();
    void halfton_clicked();
    void dithering_clicked();
    void grayscale_clicked();

    void prev_image_load();
    void init_ui();

    void clear_clicked();

private:
    Ui::MainWindow *ui;

protected:
    Process *ppm;
};


#endif // MAINWINDOW_H
