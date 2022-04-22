#include "stdio.h"
#include "stdlib.h"

// gcc -std=c99 -Wall -DUNIX -g -DDEBUG -DAPPLE -o platform_details main.c -framework OpenCL

#ifdef __APPLE__
    #include <OpenCL/opencl.h>
#else
    #include <CL/cl.h>
#endif

void displayPlatformInfo(cl_platform_id id, cl_platform_info param_name, const char* paramNameAsStr)
{
    cl_int error = 0;
    size_t paramSize = 0;

    error = clGetPlatformInfo(id, param_name, 0, NULL, &paramSize);
    // Get the size of the string that will be returned
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve platform info paramSize for %s! (%d, %s)\n", 
        paramNameAsStr,
        error,
        error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_PLATFORM ? "CL_INVALID_PLATFORM" : "CL_OUT_OF_RESOURCES");
        exit(1);
    }

    // Allocate the memory to store the string
    char *moreInfo = (char*) alloca(sizeof(char) * paramSize);

    // Get the string
    error = clGetPlatformInfo(id, param_name, paramSize, moreInfo, NULL);

    if (error != CL_SUCCESS)
    {
        printf("Error: Unable to find any OpenCL Platform information.\n");
        return;
    }

    // Print the string
    printf("%s: %s\n", paramNameAsStr, moreInfo);
}

int main(int argc, char** argv)
{
    cl_platform_id* platforms;

    cl_uint numPlatforms;
    cl_int error;
    cl_uint i = 0;

    // Get the number of platforms
    error = clGetPlatformIDs(0, NULL, &numPlatforms);

    if (error < 0)
    {
        printf("Unable to find any OpenCL platforms\n");
        return 1;
    }

    // Allocate enough space for each platform
    platforms = (cl_platform_id*) alloca(sizeof(cl_platform_id) * numPlatforms);
    printf("Found %d platforms\n", numPlatforms);

    // Get all the platforms
    error = clGetPlatformIDs(numPlatforms, platforms, NULL);
    if (error < 0)
    {
        printf("Unable to find any OpenCL platforms\n");
        return 1;
    }

    // Print the information for each platform
    for (i = 0; i < numPlatforms; i++)
    {
        displayPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");
        displayPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
        displayPlatformInfo(platforms[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");
        displayPlatformInfo(platforms[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
        displayPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");
    }
    return 0;
}


