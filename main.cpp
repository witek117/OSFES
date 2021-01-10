#include <iostream>

#define cimg_use_jpeg
#include "CImg.h"

#include "Render/Render.h"
int main() {
    cimg_library::CImg<unsigned char> sourceImage(R"(D:\Git\OSFES\img\original.jpg)");

    Render render(sourceImage);
    render.go(Render::Face::NX);



    return 0;
}
