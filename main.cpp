#include <iostream>
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "TriangleApp.h"

int main() {
    TriangleApp app;

    //try{
        app.Run();
    //}catch (const std::exception& e){
        //std::cerr << e.what() <<std::endl;
   //     return EXIT_FAILURE;
    //}
    //return EXIT_SUCCESS;
}
