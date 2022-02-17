#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "ppm.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

Process::Process(char* fileName) {
    if(fileName == ""){
        isPPM = false;
    }
    FILE* fp = fopen(fileName, "rb");
    if (fp) {
        fscanf(fp, "%c%c\n",  &img.M,     &img.N);
        fscanf(fp, "%d %d\n", &img.width, &img.height);
        fscanf(fp, "%d\n",    &img.max);
        img.pixels = (Pixel**)calloc(img.height, sizeof(Pixel*));
        for (int i = 0; i < img.height; i++) {
            img.pixels[i] = (Pixel*)calloc(img.width, sizeof(Pixel));
        }
        for (int i = 0; i < img.height; i++) {
                for (int j = 0; j < img.width; j++) {
                    fread(&img.pixels[i][j], sizeof(char), 3, fp);
                }
        }
        fclose(fp);
        isPPM = true;
    }
    prev = img;
}
bool Process::getProcessIsPPM(){
    return isPPM;
}

void Process::waterMarkload(char* fileName){
    FILE* fp = fopen(fileName, "rb");
    if (fp) {
        fscanf(fp, "%c%c\n",  &waterMark.M,     &waterMark.N);
        fscanf(fp, "%d %d\n", &waterMark.width, &waterMark.height);
        fscanf(fp, "%d\n",    &waterMark.max);
        waterMark.pixels = (Pixel**)calloc(waterMark.height, sizeof(Pixel*));
        for (int i = 0; i < waterMark.height; i++) {
            waterMark.pixels[i] = (Pixel*)calloc(waterMark.width, sizeof(Pixel));
        }
        for (int i = 0; i < waterMark.height; i++) {
                for (int j = 0; j < waterMark.width; j++) {
                    fread(&waterMark.pixels[i][j], sizeof(char), 3, fp);
                }
        }
        fclose(fp);
    }
}

void Process::waterMarkInput(int x, int y, int opacity){
    int x1 = 0;
    int y1 = 0;
    double op = (double)opacity / 10;
    unsigned char r, g, b;
    createNewImg();
    for (int i = 0; i < img_new.height; i++) {
            for (int j = 0; j < img_new.width; j++) {
                img_new.pixels[i][j] = prev.pixels[i][j];
            }
        }

    for (int i = y + 1; i <= y + waterMark.height; i++) {
        x1 = 0;
        if (i > prev.height) {
            break;
        }
        for (int j = x + 1; j <= x + waterMark.width; j++) {
            if (j > prev.width) {
                break;
            }
            if (waterMark.pixels[y1][x1].R != 0){
                r = (unsigned char)((double)img_new.pixels[i-1][j-1].R*(1-op) + (double)waterMark.pixels[y1][x1].R*op);
                g = (unsigned char)((double)img_new.pixels[i-1][j-1].G*(1-op) + (double)waterMark.pixels[y1][x1].G*op);
                b = (unsigned char)((double)img_new.pixels[i-1][j-1].B*(1-op) + (double)waterMark.pixels[y1][x1].B*op);
                img_new.pixels[i-1][j-1] = {r, g, b};
            }
            x1++;
        }
        y1++;
    }
    prev = img_new;
    return;
}

Process::~Process() { delete[] prev.pixels; delete[] img_new.pixels; delete[] waterMark.pixels;}

void Process::createNewImg(int height, int width){
    int proWidth, proHeight;
    if(width == NULL){
        proWidth = prev.width;
    }else{
        proWidth = width;
    }
    if(height == NULL){
        proHeight = prev.height;
    }else{
        proHeight = height;
    }
    img_new.M = prev.M;
    img_new.N = prev.N;
    img_new.width = proWidth;
    img_new.height = proHeight;
    img_new.max = prev.max;
    img_new.pixels = (Pixel**)calloc(proHeight, sizeof(Pixel*));

    for (int i = 0; i < proHeight; i++) {
        img_new.pixels[i] = (Pixel*)calloc(proWidth, sizeof(Pixel));
    }
    return;
}

bool Process::isPPMf() {
    if (prev.M != 'P'){return false;}
    if(prev.N != '6'){return false;}
    if(prev.max != prev.max){return false;}
    return true;
}
void Process::SavePPM(char* filename) {
    FILE* fp = fopen( filename, "wb");
    if (fp) {
        fprintf(fp, "P6\n%d %d\n%d\n", prev.width, prev.height, prev.max);
        for (int i = 0; i < prev.height; i++) {
            for (int j = 0; j < prev.width; j++) {
                fprintf(fp, "%c", prev.pixels[i][j].R);
                fprintf(fp, "%c", prev.pixels[i][j].G);
                fprintf(fp, "%c", prev.pixels[i][j].B);
            }
        }
        fclose(fp);
    }
    return;
}

void Process::SavePrevPPM(char* filename) {
    FILE* fp = fopen( filename, "wb");
    if (fp) {
        fprintf(fp, "P6\n%d %d\n%d\n", prev.width, prev.height,prev.max);
        for (int i = 0; i < prev.height; i++) {
            for (int j = 0; j < prev.width; j++) {
                fprintf(fp, "%c", prev.pixels[i][j].R);
                fprintf(fp, "%c", prev.pixels[i][j].G);
                fprintf(fp, "%c", prev.pixels[i][j].B);
            }
        }
        fclose(fp);
    }
    return;
}


int Process::GetWidth(){
    return prev.width;
}
int Process::GetHeight(){
    return prev.height;
}

void Process::updown_reverse(){
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            img_new.pixels[i][j] = prev.pixels[prev.height - i-1][j];
        }
    }

    prev = img_new;
    return;
}

void Process::leftright_reverse(){
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            img_new.pixels[i][j] = prev.pixels[i][prev.width -j -1];
        }
    }

    prev = img_new;
    return;
}

void Process::negative(){
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            img_new.pixels[i][j].R = prev.max - prev.pixels[i][j].R;	// 픽셀값 색 반전
            img_new.pixels[i][j].G = prev.max - prev.pixels[i][j].G;
            img_new.pixels[i][j].B = prev.max - prev.pixels[i][j].B;
        }
    }
    prev = img_new;
    return;
}

void Process::extract_R(){
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            img_new.pixels[i][j] = {prev.pixels[i][j].R,prev.pixels[i][j].R,prev.pixels[i][j].R};   // 픽셀값 색 반전
        }
    }
    prev = img_new;
    return;
}

void Process::extract_G(){
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            img_new.pixels[i][j] = {prev.pixels[i][j].G, prev.pixels[i][j].G, prev.pixels[i][j].G};   // 픽셀값 색 반전
        }
    }

    prev = img_new;
    return;
}

void Process::extract_B(){
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            img_new.pixels[i][j] = {prev.pixels[i][j].B, prev.pixels[i][j].B, prev.pixels[i][j].B};   // 픽셀값 색 반전
        }
    }

    prev = img_new;
    return;
}

void Process::RGBcorrectN(int &R, int &G, int &B){
    if (R > prev.max) {
        R = img.max;
    }
    if (G > prev.max) {
        G = prev.max;
    }
    if (B > prev.max) {
        B = prev.max;
    }
    if (R < 0) {
        R = 0;
    }
    if (G < 0) {
        G = 0;
    }
    if (B < 0) {
        B = 0;
    }
    return;
}

void Process::brightnessControl(int brightness){
    int R, G, B;
    createNewImg();
    for (int i = 0; i < prev.height; i++) {
       for (int j = 0; j < prev.width; j++) {
           R = prev.pixels[i][j].R + brightness;   // 픽셀값 색 반전
           G = prev.pixels[i][j].G + brightness;
           B = prev.pixels[i][j].B + brightness;
           RGBcorrectN(R,G,B);
           img_new.pixels[i][j].R = R;
           img_new.pixels[i][j].G = G;
           img_new.pixels[i][j].B = B;
           }
    }

    prev = img_new;
    return;
}

double Process::rotateDegree(double degree){
    double pi = 3.141592;
    // seta값 계산
    return pi / (180.0 / degree);
}

void Process::ResetBackground(){
    for (int i = 0; i < img_new.height; i++) {
        for (int j = 0; j < img_new.width; j++) {
            img_new.pixels[i][j] = {0};
        }
    }
}

void Process::UclidRotate(double seta){
    int center_x = prev.width / 2;
    int center_y = prev.height / 2;
    int new_x;
    int new_y;
    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            new_x = round((i - center_y) * sin(seta) + (j - center_x) * cos(seta) + center_x);
            new_y = round((i - center_y) * cos(seta) - (j - center_x) * sin(seta) + center_y);

            if (new_x < 0)      continue;
            if (new_x >= prev.width)   continue;
            if (new_y < 0)      continue;
            if (new_y >= prev.height)   continue;

            img_new.pixels[i][j] = prev.pixels[new_y][new_x];
        }
    }
}
void Process::rotateControl(double degree){
    double seta;
    seta = rotateDegree(degree);
    createNewImg();
    ResetBackground();
    UclidRotate(seta);

    prev = img_new;
    return;
}

void Process::cropControl(int y1, int y2,int x1, int x2){
    if(y1 > y2){
        int tmp_y = y1;
        y1 = y2;
        y2 = tmp_y;
    }
    if(x1 > x2){
        int tmp_x = x1;
        x1 = x2;
        x2 = tmp_x;
    }
    if(y1 < 0){
        y1 = 0;
    }
    if(y2 > prev.height){
        y2 = prev.height - 1;
    }
    if(x2 > prev.width){
        x2 = prev.width - 1;
    }
    if(x1 < 0){
        x1 = 0;
    }

    int new_height = y2 - y1 + 1;
    int new_width = x2 - x1 + 1;
    int y = 0;
    int x = 0;


    createNewImg(new_height, new_width);
    for (int i = y1; i <= y2; i++) {
        x = 0;
        for (int j = x1; j <= x2; j++) {
            img_new.pixels[y][x] = prev.pixels[i][j];
            x++;
        }
        y++;
    }

    prev = img_new;
    return;
}

void Process::contrast(int cont){
    int R,G,B;
    int cont1;
    int mean_R, mean_G, mean_B = 0;

    createNewImg();

    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            mean_R = mean_R + prev.pixels[i][j].R;
            mean_G = mean_G + prev.pixels[i][j].G;
            mean_B = mean_B + prev.pixels[i][j].B;
        }
    }
    mean_R = mean_R / (prev.height * prev.width);
    mean_G = mean_G / (prev.height * prev.width);
    mean_B = mean_B / (prev.height * prev.width);

    for (int i = 0; i < prev.height; i++) {
        for (int j = 0; j < prev.width; j++) {
            if (prev.pixels[i][j].R > mean_R) {
                R = prev.pixels[i][j].R + cont;   // 픽셀값 색 반전
                RGBcorrectN(R, G, B);
            }else{
                R = prev.pixels[i][j].R - cont;
                RGBcorrectN(R, G, B);
            }
            if (prev.pixels[i][j].G > mean_G) {
                G = prev.pixels[i][j].G + cont;   // 픽셀값 색 반전
                RGBcorrectN(R, G, B);
            }else{
                G = prev.pixels[i][j].G - cont;
                RGBcorrectN(R, G, B);
            }
            if (prev.pixels[i][j].B > mean_B) {
                B = prev.pixels[i][j].B + cont;   // 픽셀값 색 반전
                RGBcorrectN(R, G, B);
            }else{
                B = prev.pixels[i][j].B - cont;
                RGBcorrectN(R, G, B);
            }
            img_new.pixels[i][j].R = R;
            img_new.pixels[i][j].G = G;
            img_new.pixels[i][j].B = B;
            }
    }

    prev = img_new;
    return;
}

void Process::RandomDithering(){
    int n = 0;
    int mean = 0;
    createNewImg();
    for(int i = 0; i < prev.height; i++){
        for(int j = 0; j < prev.width; j++){
            n = rand() % prev.max;
            mean = (prev.pixels[i][j].R + prev.pixels[i][j].G + prev.pixels[i][j].B) / 3;
            if(mean < n){
                img_new.pixels[i][j] = {0,0,0};
            }else{
                img_new.pixels[i][j] = {(unsigned char)prev.max,(unsigned char)prev.max,(unsigned char)prev.max};
            }
        }
    }

    prev = img_new;
    return;
}

void Process::clear(){
    img_new = img;
    prev = img;
}

void Process::halfton() {
    int diameter = 15;
    int radius = diameter / 2;
    int startRow = (prev.height % diameter) / 2;
    int endRow = prev.height - (prev.height % diameter) / 2;
    int startCol = (prev.width % diameter) / 2;
    int endCol = prev.width - (prev.width % diameter) / 2;
    int tradius = 0;
    int centerX, centerY;
    double length;
    createNewImg();
    for (int row=startRow; row < endRow; row+=diameter)
    {
        for(int col=startCol; col < endCol; col+=diameter)
        {
            // 평균 값의 크기
            int mean = 0;
            for(int x=col; x < col+diameter; x++)
                for(int y = row; y < row+diameter; y++)
                    mean += (prev.pixels[y][x].R + prev.pixels[y][x].G + prev.pixels[y][x].B) / 3;
            mean = mean / (diameter*diameter);

            // 평균 값의 크기를 반지름으로 치환
            tradius = mean / (prev.max / radius);

            centerX = col + radius;
            centerY = row + radius;
            for(int x=centerX-radius; x < centerX+radius; x++)
                for(int y=centerY-radius; y < centerY+radius; y++)
                {
                    length = sqrt(pow((double)(centerY - y), 2) + pow((double)(centerX - x), 2));
                    if (length > tradius) img_new.pixels[y][x] = {0, 0, 0};
                    else img_new.pixels[y][x] = {(unsigned char)prev.max,(unsigned char)prev.max,(unsigned char)prev.max};
                }
        }
    }

    prev = img_new;
    return;
}

void Process::grayscale(){
    unsigned char mean = 0;
    createNewImg();
    for(int i = 0; i <prev.height; i++){
        for(int j = 0; j < prev.width; j++){
            mean = (prev.pixels[i][j].R + prev.pixels[i][j].G + prev.pixels[i][j].B) / 3;
            img_new.pixels[i][j] = {mean, mean, mean};
        }
    }

    prev = img_new;
    return;
}
