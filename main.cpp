#include <iostream>

#define cimg_use_jpeg
#include "CImg.h"

const unsigned char red[]   = { 255,0,0 };

int main(int argc,char **argv) {
    cimg_library::CImg<unsigned char> image(R"(D:\Git\OSFES\img\parrot.jpg)");
    int width = image.width();
    int height = image.height();

    cimg_library::CImg<unsigned char> visu(image.width(),image.height(),1,3,0);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++) {
            int data[] = {image(x,y,0), image(x,y,1), image(x,y,2)};
             visu.draw_point(x,y, data);
        }
    }

    visu.save_jpeg(R"(D:\Git\OSFES\img\out.jpg)");

    return 0;
}
