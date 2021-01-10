#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <array>
#include <chrono>
#include "CImg.h"

class Render {
    using IMAGE = cimg_library::CImg<unsigned char>;
    static const int filterSize = 5;
    static const int twoFilterSize = 2 * filterSize;

    IMAGE& sourceImage;

public:
    enum class Face {
        PZ = 0, NZ, PX, NX, PY, NY
    };
private:
    static std::array<double, 3> orientations(double x, double y, Face face) {
        switch (face) {
            case Face::PZ: return {-1,-x,-y};
            case Face::NZ: return {x,-1,-y};
            case Face::PX: return {x,-1,-y};
            case Face::NX: return {-x,1.0f,-y};
            case Face::PY: return {-y,-x, 1.0f};
            case Face::NY: return {y,-x,-1.0f};
        }

        return {1,1,1};
    }

    static double kernel(int x, int filtersize) {
        if (x == 0) {
            return 1;
        } else {
            double xp = M_PI * x;
            return filtersize * sin(xp) * sin(xp / filtersize) / (xp * xp);
        }
    }

    static int clamp(int x, int min, int max) {
        return std::min(max, std::max(x, min));
    }

    static double Mod(double x, double n) {
        return fmod( (fmod(x, n) + n), n);
    }

    static IMAGE renderFace(IMAGE& sourceImage, Face face, double rotation) {
        int sourceWidth = sourceImage.width();
        int sourceHeight = sourceImage.height();

        int faceWidth = sourceWidth / 8;

        IMAGE writeData(faceWidth, faceWidth, 1, 3, 0);

        double xKernel[twoFilterSize] = {0.0f};
        double yKernel[twoFilterSize] = {0.0f};

        auto start = std::chrono::system_clock::now();
        for (int x = 0; x < faceWidth; x++) {
            for (int y = 0; y < faceWidth; y++) {
                std::array<double, 3> cube = orientations((2.0f * ((float)x + 0.5f) / (float)faceWidth - 1.0f), (2.0f * ((float)y + 0.5f) / (float)faceWidth - 1), face);
                double r = sqrt(cube[0] * cube[0] + cube[1] * cube[1] + cube[2] * cube[2]);
                double lon = Mod(atan2(cube[1], cube[0]) + rotation, 2.0f * M_PI);
                double lat = acos(cube[2] / r);
                kernelResample( ((double)sourceWidth * lon / M_PI / 2.0f) - 0.5f,
                                ((double)sourceHeight * lat / M_PI) - 0.5f,
                                   {x, y}, sourceImage, writeData, xKernel, yKernel);
            }
        }

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << elapsed_seconds.count() << "s\n";

        return writeData;
    }

    static void kernelResample(double xFrom, double yFrom, std::array<int, 2> to, IMAGE& sourceImage, IMAGE& write, double xKernel[], double yKernel[]) {
        int k1 = static_cast<int>(floor(xFrom)) - filterSize + 1; // xStart = zmienne[1]
        int k2 = static_cast<int>(floor(yFrom)) - filterSize + 1; // yStart = zmienne[2]

        for (int i = 0; i < twoFilterSize; i++) {
            xKernel[i] = kernel((int)xFrom - (k1 + i), filterSize);
            yKernel[i] = kernel((int)yFrom - (k2 + i), filterSize);
        }

        double Q[3] = {0};

        for (int i = 0; i < twoFilterSize; i++) {
            double P[3] = {0};

            int yClamped = clamp(k2 + i, 0, sourceImage.height() - 1);

            for (int j = 0; j < twoFilterSize; j++) {
                int xClamped = clamp(k1 + j, 0, sourceImage.width() - 1);

                P[0] += sourceImage(xClamped, yClamped,0) * xKernel[j];
                P[1] += sourceImage(xClamped, yClamped,1) * xKernel[j];
                P[2] += sourceImage(xClamped, yClamped,2) * xKernel[j];
            }
            Q[0] += P[0] * yKernel[i];
            Q[1] += P[1] * yKernel[i];
            Q[2] += P[2] * yKernel[i];
        }
        char data[] = {(char)Q[0], (char)Q[1], (char)Q[2]};
        write.draw_point(to[0],to[1], data);
    }

public:
    Render(IMAGE& sourceImage) : sourceImage(sourceImage) { }

    void go(Face face) {
        renderFace(sourceImage, face, 0);
    }
};
