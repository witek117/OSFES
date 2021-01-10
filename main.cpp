#include <iostream>
#include <cstring>
#include <cxxopts.hpp>

#define cimg_use_jpeg
#include "CImg.h"

#include "Render/Render.h"

std::string current_working_directory() {
    char* cwd = _getcwd( 0, 0 ) ; // **** microsoft specific ****
    std::string working_directory(cwd) ;
    std::free(cwd) ;
    return working_directory ;
}

std::string getDirectoryFromPath(std::string filePath) {
    int s1 = filePath.find_last_of("\\");
    int s2 = filePath.find_last_of("/");
    int index = std::max(s1, s2);
    return filePath.substr(0, index);
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("OSFES", "Spherical photo to cube map");

    std::string sourceImagePath = getDirectoryFromPath(__FILE__) + std::string("\\img\\original.jpg");

    options.add_options()
            ("s,source", "Param source image", cxxopts::value<std::string>()->default_value(sourceImagePath))
            ("h,help", "Print usage")
            ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("source")) {
        sourceImagePath = result["source"].as<std::string>();
    }

    std::cout << "Rendering image from: " <<  sourceImagePath << std::endl;

    cimg_library::CImg<unsigned char> sourceImage(sourceImagePath.c_str());

    Render render={sourceImage};
    render.go();

    return 0;
}
