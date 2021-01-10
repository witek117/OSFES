#include "Render.h"
#include "CImg.h"
#include <iostream>

double Render::RenderFace::kernel(int x, int filtersize) {
    if (x == 0) {
        return 1;
    } else {
        double xp = M_PI * x;
        return filtersize * sin(xp) * sin(xp / filtersize) / (xp * xp);
    }
}

int Render::RenderFace::clamp(int x, int min, int max) {
    return std::min(max, std::max(x, min));
}

double Render::RenderFace::Mod(double x, double n) {
    return fmod( (fmod(x, n) + n), n);
}

cimg_library::CImg<unsigned char> Render::RenderFace::renderFace(double rotation) {
    int sourceWidth = sourceImage.width();
    int sourceHeight = sourceImage.height();

    int faceWidth = sourceWidth / 8;

    cimg_library::CImg<unsigned char> writeData(faceWidth, faceWidth, 1, 3, 0);

    double xKernel[twoFilterSize] = {0.0f};
    double yKernel[twoFilterSize] = {0.0f};

    auto start = std::chrono::system_clock::now();
    for (int x = 0; x < faceWidth; x++) {
        for (int y = 0; y < faceWidth; y++) {
            std::array<double, 3> cube = orientations((2.0f * ((float)x + 0.5f) / (float)faceWidth - 1.0f), (2.0f * ((float)y + 0.5f) / (float)faceWidth - 1), face);
            double r = sqrt(cube[0] * cube[0] + cube[1] * cube[1] + cube[2] * cube[2]);
            double lon = Mod(atan2(cube[1], cube[0]) + rotation, 2.0f * M_PI);
            double lat = acos(cube[2] / r);
            onePixelSample( ((double)sourceWidth * lon / M_PI / 2.0f) - 0.5f,
                            ((double)sourceHeight * lat / M_PI) - 0.5f,
                            {x, y}, writeData, xKernel, yKernel);
        }
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << elapsed_seconds.count() << "s\n";

    return writeData;
}

void Render::RenderFace::onePixelSample(double xFrom, double yFrom, std::array<int, 2> to, IMAGE& destinationImage, double xKernel[], double yKernel[]) {
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
    destinationImage.draw_point(to[0],to[1], data);
}

void Render::RenderFace::body() {
    doneImage = renderFace(0);
}

cimg_library::CImg<unsigned char>& Render::RenderFace::getImage() {
    return doneImage;
}



std::array<double, 3> Render::orientations(double x, double y, Face face) {
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

bool Render::waitForAll() {
    for (auto & face : faces) {
        if(!face.join(10000)) {
            return false;
        }
    }

    return true;
}

void Render::go() {
    for (auto & face : faces) {
        face.run();
    }

//    if(waitForAll()) {
//        std::cout << "ok" << std::endl;
//
//    }
//    std::cout << "end" << std::endl;
}

Render::Render(IMAGE& sourceImage) {
    faces[static_cast<uint8_t>(Face::PZ)] = {sourceImage, Face::PZ, 10000};
    faces[static_cast<uint8_t>(Face::NZ)] = {sourceImage, Face::NZ, 10000};
    faces[static_cast<uint8_t>(Face::PX)] = {sourceImage, Face::PX, 10000};
    faces[static_cast<uint8_t>(Face::NX)] = {sourceImage, Face::NX, 10000};
    faces[static_cast<uint8_t>(Face::PY)] = {sourceImage, Face::PY, 10000};
    faces[static_cast<uint8_t>(Face::NY)] = {sourceImage, Face::NY, 10000};
}
