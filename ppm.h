#pragma once

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

typedef struct Pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
}Pixel;

typedef struct PPMimage {
    char M, N;
    int width;
    int height;
    int max;
    struct Pixel** pixels;
}PPMimage;

class Process {
protected:
    PPMimage img_new;
    PPMimage img;
    PPMimage waterMark;
    PPMimage prev;
    bool isPPM;
    void createNewImg(int width = NULL, int height = NULL);
public:
    Process(char* FileName = NULL);
    ~Process();
    void SavePPM(char* filename = NULL);
    int GetWidth();
    int GetHeight();
    void updown_reverse();
    void leftright_reverse();
    void negative();
    bool isPPMf();
    void halfton();
    void extract_R(); //R_color

    void extract_G(); //G_color

    void extract_B(); //B_color

    void brightnessControl(int brightness); //brightness_submit

    double rotateDegree(double degree); // for uclid

    void ResetBackground(); //for rotate

    void RGBcorrectN(int &R, int &G, int &B);

    void UclidRotate(double seta); //for rotateControl

    void rotateControl(double degree);  //degree_submit

    void cropControl(int y1, int y2,int x1, int x2);

    void contrast(int cont);

    void waterMarkInput(int x, int y, int opacity);
    void waterMarkload(char* fileName);

    void RandomDithering();
    void grayscale();
    void SavePrevPPM(char* filename);
    bool getProcessIsPPM();
    void clear();
};
