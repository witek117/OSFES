#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <array>
#include <chrono>
#include "CImg.h"

class Render {
    using IMAGE = cimg_library::CImg<unsigned char>;

    static std::array<double, 3> orientations(double x, double y, std::string face)
    {
        if (face == "pz") {
            return {-1.0f, -x, -y};
        } else if (face == "nz") {
            return { 1.0f, x, -y };
        } else if (face == "px") {
            return { x, -1.0f, -y };
        } else if (face == "nx") {
            return { -x, 1.0f, -y };
        } else if (face == "py") {
            return { -y, -x, 1.0f };
        } else if (face == "ny") {
            return {y, -x, -1.0f };
        } else {
            return { 1.0f, 1.0, 1.0f };
        }
    }

    static double fast_sin(double x) {
        x /= 2 * M_PI;
        x -= (int) x;

        if (x <= 0.5) {
            double t = 2 * x * (2 * x - 1);
            return (M_PI * t) / ((M_PI - 4) * t - 1);
        }
        else {
            double t = 2 * (1 - x) * (1 - 2 * x);
            return -(M_PI * t) / ((M_PI - 4) * t - 1);
        }
    }

    static double kernel(int x, int filtersize) {
        if (x == 0) {
            return 1;
        } else {
            double xp = M_PI * x;
            return filtersize * fast_sin(xp) * fast_sin(xp / filtersize) / (xp * xp);
        }
    }

    static int clamp(int x, int min, int max) {
        return std::min(max, std::max(x, min));
    }

    static double Mod(double x, double n) {
        return fmod( (fmod(x, n) + n), n);
    }

public:
    static IMAGE renderFace(int btmwid, int btmhei, IMAGE& newBitmap, const std::string& face, double rotation)
    {
        //int faceWidth = (int)Math.Min(maxWidth, readData.Width / 4);
        const int filterSize = 5;
        const int twoFilterSize = 2 * filterSize;

        int faceWidth = btmwid / 8;
        int faceHeight = faceWidth;

        IMAGE writeData(faceWidth,faceHeight,1,3,0);

        double xKernel[twoFilterSize] = {0.0f};
        double yKernel[twoFilterSize] = {0.0f};
        int zmienne[6] = {0};

        double Q[3] = {0.0f};
        double P[3] = {0.0f};

        auto start = std::chrono::system_clock::now();
        for (int x = 0; x < faceWidth; x++)
        {
            for (int y = 0; y < faceHeight; y++)
            {
                std::array<double, 3> cube = orientations((2.0f * ((float)x + 0.5f) / (float)faceWidth - 1.0f), (2.0f * ((float)y + 0.5f) / (float)faceHeight - 1), face);
                double r = sqrt(cube[0] * cube[0] + cube[1] * cube[1] + cube[2] * cube[2]);
                double lon = Mod(atan2(cube[1], cube[0]) + rotation, 2.0f * M_PI);
                double lat = acos(cube[2] / r);
                kernelResample(((double)btmwid * lon / M_PI / 2.0f) - 0.5f, ((double)btmhei * lat / M_PI) - 0.5f, {x, y}, filterSize, newBitmap, btmwid, btmhei, writeData, xKernel, yKernel, zmienne, P, Q);
            }
        }

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << elapsed_seconds.count() << "s\n";

        return writeData;
    }

    static void kernelResample(double xFrom, double yFrom, std::array<int, 2> to, int filterSize, IMAGE& newBitmap, int btmwid, int btmhei, IMAGE& write, double xKernel[], double yKernel[], int zmienne[], double P[], double Q[])
    {
        int twoFilterSize = 2 * filterSize; //twoilterSize = dane[0]
        double x1 = floor(xFrom);
        double y1 = floor(yFrom);

        zmienne[1] = (int)x1 - filterSize + 1; // xStart = zmienne[1]
        zmienne[2] = (int)y1 - filterSize + 1; // yStart = zmienne[2]

        for (int i = 0; i < twoFilterSize; i++)
        {
            xKernel[i] = kernel((int)xFrom - (zmienne[1] + i), filterSize);
            yKernel[i] = kernel((int)yFrom - (zmienne[2] + i), filterSize);
        }
        Q[0] = 0;
        Q[1] = 0;
        Q[2] = 0;

        zmienne[3] = 0; // zmienne[3] = y
        zmienne[4] = 0; // zmienne[4] = x
        zmienne[5] = 0; // zmienne[5] = rr
        int yClamped = 0;
        int xClaped = 0;
        for (int i = 0; i < twoFilterSize; i++)
        {
            zmienne[3] = zmienne[2] + i;
            P[0] = 0;
            P[1] = 0;
            P[2] = 0;
            yClamped = clamp(zmienne[3], 0., btmhei - 1);

            for (int j = 0; j < twoFilterSize; j++)
            {
                xClaped = clamp(zmienne[4], 0, btmwid - 1);
                zmienne[4] = zmienne[1] + j;
                //newBitmap.pixel_type();
                P[0] += newBitmap(xClaped, yClamped,0) * xKernel[j];
                P[1] += newBitmap(xClaped, yClamped,1) * xKernel[j];
                P[2] += newBitmap(xClaped, yClamped,2) * xKernel[j];
            }
            Q[0] += P[0] * yKernel[i];
            Q[1] += P[1] * yKernel[i];
            Q[2] += P[2] * yKernel[i];
        }
        int data[] = {(int)Q[0], (int)Q[1], (int)Q[2]};
        write.draw_point(to[0],to[1], data);
    }
};
