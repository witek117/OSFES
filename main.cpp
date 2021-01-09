#include <iostream>

#define cimg_use_jpeg
#include "CImg.h"

int main(int argc,char **argv) {
//    const char* file_i = cimg_option("-i","img/parrot.jpg","Input image");
//    // const double sigma = cimg_option("-blur",1.0,"Variance of gaussian pre-blurring");
//    const cimg_library::CImg<unsigned char> image = cimg_library::CImg<>(file_i); //.normalize(0,255).blur((float)sigma).resize(-100,-100,1,3);

    cimg_library::CImg<unsigned char> image("D:\\Git\\OSFES\\img\\parrot.jpg");
    int width = image.width();
    int height = image.height();

    std::cout << width << " " << height;


    int x = 10;
    int y = 10;

    int val_red   = image(x,y,0);
    int val_green   = image(x,y,1);
    int val_blue   = image(x,y,2);

    return 0;
}
