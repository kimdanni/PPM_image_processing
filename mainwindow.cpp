#include "ppm.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->loadImage, SIGNAL(clicked()), this, SLOT(load_image_clicked()));
    connect(ui->save_file, SIGNAL(clicked()), this, SLOT(save_image_clicked()));
    connect(ui->reverse_ud, SIGNAL(clicked()), this, SLOT(updown_reverse_image_clicked()));
    connect(ui->reverse_rl, SIGNAL(clicked()), this, SLOT(leftright_reverse_image_clicked()));
    connect(ui->negative, SIGNAL(clicked()), this, SLOT(negative_image_clicked()));

    connect(ui->chennel_submit, SIGNAL(clicked()), this, SLOT(chennel_submit_clicked()));

    connect(ui->brightness_submit, SIGNAL(clicked()), this, SLOT(brightness_image_clicked()));
    connect(ui->degree_submit, SIGNAL(clicked()), this, SLOT(rotate_image_clicked()));

    connect(ui->pos_submit, SIGNAL(clicked()), this, SLOT(crop_image_clicked()));
    connect(ui->contrast_submit, SIGNAL(clicked()), this, SLOT(contrast_clicked()));

    connect(ui->loadImage_watermark, SIGNAL(clicked()), this, SLOT(waterMark_loaded()));
    connect(ui->load_imageS, SIGNAL(clicked()), this, SLOT(waterMark_clicked()));

    connect(ui->Dithering, SIGNAL(clicked()), this, SLOT(dithering_clicked()));
    connect(ui->halftone, SIGNAL(clicked()), this, SLOT(halfton_clicked()));

    connect(ui->grayscale, SIGNAL(clicked()), this, SLOT(grayscale_clicked()));

    connect(ui->clear_btn, SIGNAL(clicked()), this, SLOT(clear_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_image_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this,"Open File", "C:\\", "Files (*.ppm)");
    if(fileName.length() == 0){
        return;
    }
    char str[255] = {0,};
    qsnprintf((char*)str, sizeof(str), "%s", fileName.toUtf8().constData());
    if(fileName.length() == 0){
        fileName = "C:\\Users\\kuromi\\Documents\\build-untitled3-Desktop_Qt_5_12_2_MSVC2017_32bit-Debug\\RURU.ppm";
    }
    ppm = new Process(str);
    if(ppm->isPPMf()){
        ui->image->setPixmap(QPixmap(fileName).scaledToWidth(441).scaledToHeight(491));
        ui->width_val->setText(QString::number(ppm->GetWidth()));
        ui->height_val->setText(QString::number(ppm->GetHeight()));
        ui->FileName_val->setText(str);
    }
    else{
        QMessageBox msg;
        fileName = "";
        msg.setText("IS NOT P6!!");
        msg.exec();
    }
}

void MainWindow::save_image_clicked(){
    char str[255] = {0,};
    qsnprintf((char*)str, sizeof(str), "%s", ui->save_file_directory->toPlainText().toUtf8().constData());
    // 저장하기
    ppm->SavePPM(str);
    ui->image->setPixmap(QPixmap(str).scaledToWidth(441).scaledToHeight(491));
    init_ui();
}

void MainWindow::init_ui()
{
    ui->contrast_val->setText("0 ~ 255");
    ui->brightness_val->setText("0 ~ 255");
    ui->save_file_directory->setText("");
    ui->degree_val->setText("Submit degree.");
    ui->crop_pos->setText("y1 y2 x1 x2");
    ui->add_watermark->setText("x y opacity");
    ui->height_val->setText("-");
    ui->width_val->setText("-");
    ui->FileName_val->setText("-");
    ui->R_color->setAutoExclusive(false);
    ui->R_color->setChecked(false);
    ui->R_color->setAutoExclusive(true);
    ui->G_color->setAutoExclusive(false);
    ui->G_color->setChecked(false);
    ui->G_color->setAutoExclusive(true);
    ui->B_color->setAutoExclusive(false);
    ui->B_color->setChecked(false);
    ui->B_color->setAutoExclusive(true);
    ui->save_file->setText("");
}


void MainWindow::prev_image_load(){
    char str[255] = "C:\\Users\\kuromi\\Documents\\build-untitled3-Desktop_Qt_5_12_2_MSVC2017_32bit-Debug\\prev.ppm";
    ppm->SavePrevPPM(str);
    ui->image->setPixmap(QPixmap(str).scaledToWidth(441).scaledToHeight(491));
}

void MainWindow::leftright_reverse_image_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->leftright_reverse();
        prev_image_load();
    }
}

void MainWindow::updown_reverse_image_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->updown_reverse();
        prev_image_load();
    }
}

void MainWindow::negative_image_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->negative();
        prev_image_load();
    }
}

void MainWindow::chennel_submit_clicked(){
    if(ppm->getProcessIsPPM() == true){
        if(ui->R_color->isChecked()){
            ppm->extract_R();
        }else if(ui->G_color->isChecked()){
            ppm->extract_G();
        }else{
            ppm->extract_B();
        }
        prev_image_load();
    }
}

void MainWindow::brightness_image_clicked(){
    if(ppm->getProcessIsPPM() == true){
        char brightness[255] = {0,};
        qsnprintf((char *)brightness, sizeof(brightness), "%s", ui->brightness_val->toPlainText().toUtf8().constData());
        double d=atoi(brightness);
        ppm->brightnessControl(d);
        prev_image_load();
    }
}

void MainWindow::rotate_image_clicked(){
    if(ppm->getProcessIsPPM() == true){
        char degree[255] = {0,};
        qsnprintf((char *)degree, sizeof(degree), "%s", ui->degree_val->toPlainText().toUtf8().constData());
        double d=atoi(degree);
        ppm->rotateControl(d);
        prev_image_load();
    }
}

void MainWindow::halfton_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->halfton();
        prev_image_load();
    }
}


void MainWindow::dithering_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->RandomDithering();
        prev_image_load();
    }
}

void MainWindow::crop_image_clicked(){
    if(ppm->getProcessIsPPM() == true){
        char crop[255] = {0,};
        int pose[4] = {0, 100, 0, 400};
        qsnprintf((char *)crop, sizeof(crop), "%s", ui->crop_pos->toPlainText().toUtf8().constData());
        char * ptr = strtok(crop, " ");
        for(int i = 0; i < sizeof(ptr)/4; i++){
            int d = atoi(ptr);
            pose[i] = d;
            ptr = strtok(NULL, " ");
        }
        ppm->cropControl(pose[0], pose[1],pose[2], pose[3]);
        prev_image_load();}
}

void MainWindow::contrast_clicked(){
    if(ppm->getProcessIsPPM() == true){
        char contrast[255] = {0,};
        qsnprintf((char *)contrast, sizeof(contrast), "%s", ui->contrast_val->toPlainText().toUtf8().constData());
        if(atoi(contrast) == 0){
            return;
        }
        int d=atoi(contrast);
        ppm->contrast(d);
        prev_image_load();}
}

void MainWindow::waterMark_clicked(){
    if(ppm->getProcessIsPPM() == true){
        char water[255] = {0,};
        int pose[3] = {0, 0, 10};
        qsnprintf((char *)water, sizeof(water), "%s", ui->add_watermark->toPlainText().toUtf8().constData());
        char * ptr = strtok(water, " ");
        for(int i = 0; i < 3; i++){
            int d = atoi(ptr);
            pose[i] = d;
            ptr = strtok(NULL, " ");
        }
        ppm->waterMarkInput(pose[0], pose[1], pose[2]);
        prev_image_load();}
}

void MainWindow::waterMark_loaded(){
    if(ppm->getProcessIsPPM() == true){
        QString fileName = QFileDialog::getOpenFileName(this,"Open File", "C:\\", "Files (*.ppm)");
        char str[500] = {0,};
        if(fileName.length() == 0){
            fileName = "C:\\Users\\kuromi\\Documents\\build-untitled3-Desktop_Qt_5_12_2_MSVC2017_32bit-Debug\\ruru_waterMark.ppm";
        }
        qsnprintf((char*)str, sizeof(str), "%s", fileName.toUtf8().constData());
        ppm->waterMarkload(str);
    }
}

void MainWindow::grayscale_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->grayscale();
        prev_image_load();}
}

void MainWindow::clear_clicked(){
    if(ppm->getProcessIsPPM() == true){
        ppm->clear();
        init_ui();
        prev_image_load();}
}



//pattern dithering


