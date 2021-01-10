#include "cxxopts.hpp"
#include "Render/Render.h"
#define cimg_use_jpeg
#include "CImg.h"

#include <cstdlib>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

std::string current_working_directory() {
    char* cwd = _getcwd( 0, 0 ) ; // **** microsoft specific ****
    std::string working_directory(cwd) ;
    std::free(cwd) ;
    return working_directory ;
}

std::string getDirectoryFromPath(std::string filePath) {
    int s1 = filePath.find_last_of('\\');
    int s2 = filePath.find_last_of('/');
    int index = std::max(s1, s2);
    return filePath.substr(0, index);
}

std::string getFileName(std::string filePath) {
    int s1 = filePath.find_last_of('\\');
    int s2 = filePath.find_last_of('/');
    int index = std::max(s1, s2);
    std::string fileName = filePath.substr(index);
    return fileName.substr(0, fileName.find('.'));
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("OSFES", "Spherical photo to cube map");

    std::string sourceImagePath = getDirectoryFromPath(__FILE__) + std::string("\\img\\original.jpg");
    std::string outputDirectoryPath = getDirectoryFromPath(__FILE__) + "\\img\\original";

    options.add_options()
            ("s,source", "Param source image", cxxopts::value<std::string>()->default_value(sourceImagePath))
            ("o,output", "Param output directory path", cxxopts::value<std::string>()->default_value(outputDirectoryPath))
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

//    if (result.count("output")) {
//        outputDirectoryPath = result["output"].as<std::string>() + getFileName(sourceImagePath);
//    } else {
//        outputDirectoryPath = getDirectoryFromPath(sourceImagePath) + getFileName(sourceImagePath);
//    }
//
//    struct stat st = {0};
//    stat(outputDirectoryPath.c_str(), &st);
//
//    if(!S_ISDIR(st.st_mode)) {
//        mkdir(outputDirectoryPath.c_str());
//    }

    std::cout << "Rendering image from: " <<  sourceImagePath << std::endl;

//    cimg_library::CImg<unsigned char> sourceImage(sourceImagePath.c_str());
    cimg_library::CImg<unsigned char> sourceImage("A:/original.jpg");

//    Render render={sourceImage};
//    render.go();
//
//    if(render.waitForAll()) {
//        render.saveFiles(outputDirectoryPath);
//    }

    return 0;
}
