#include <iostream>

#define cimg_use_jpeg
#include "CImg.h"

#include "Render/Render.h"
int main(int argc,char **argv) {
    cimg_library::CImg<unsigned char> sourceImage(R"(D:\Git\OSFES\img\original.jpg)");
    int sourceWidth = sourceImage.width();
    int sourceHeight = sourceImage.height();

    auto nnn = Render::renderFace(sourceImage, Render::Face::NX, 0);
    nnn.save_jpeg(R"(D:\Git\OSFES\img\pz.jpg)");

    return 0;
}
