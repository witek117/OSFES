#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <array>
#include <chrono>
#include "CImg.h"
#include "osapi.h"
#include <iostream>

class Render {
public:
    enum class Face : uint8_t {
        PZ = 0, NZ, PX, NX, PY, NY
    };
private:

    using IMAGE = cimg_library::CImg<unsigned char>;
    static const int filterSize = 5;
    static const int twoFilterSize = 2 * filterSize;

    class RenderFace : public osapi::Thread{
        IMAGE sourceImage;
        Face face;
    public:
        IMAGE doneImage;

        static double kernel(int x, int filtersize);

        static int clamp(int x, int min, int max);

        static double Mod(double x, double n);

        IMAGE renderFace(double rotation);

        void onePixelSample(double xFrom, double yFrom, std::array<int, 2> to, IMAGE& destinationImage, double xKernel[], double yKernel[]);

        void body() override;

        RenderFace(IMAGE sourceImage, Face face, unsigned int stackSize)
            : Thread(0, stackSize, osapi::Joinable::JOINABLE, "Face"), sourceImage(sourceImage), face(face) {
        }

        RenderFace() : Thread(0, 0, osapi::Joinable::NOT_JOINABLE, "Face"), face(Face::NX) { }

        IMAGE& getImage();
    };

    static std::array<double, 3> orientations(double x, double y, Face face);

public:
    RenderFace faces[6];

    Render(IMAGE& sourceImage);

    bool waitForAll() ;

    void go();

    void saveFiles(std::string& directoryPath) {
        std::string path = directoryPath + "\\PZ.jpg";
        faces[static_cast<uint8_t>(Face::PZ)].getImage().save(path.c_str());

        path = directoryPath + "\\NZ.jpg";
        faces[static_cast<uint8_t>(Face::NZ)].getImage().save(path.c_str());

        path = directoryPath + "\\PX.jpg";
        faces[static_cast<uint8_t>(Face::PX)].getImage().save(path.c_str());

        path = directoryPath + "\\NX.jpg";
        faces[static_cast<uint8_t>(Face::NX)].getImage().save(path.c_str());

        path = directoryPath + "\\PY.jpg";
        faces[static_cast<uint8_t>(Face::PY)].getImage().save(path.c_str());

        path = directoryPath + "\\NY.jpg";
        faces[static_cast<uint8_t>(Face::NY)].getImage().save(path.c_str());
    }
};
