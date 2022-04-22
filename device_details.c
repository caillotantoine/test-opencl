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
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve platform info paramSize for %s! (%d, %s)\n", 
        paramNameAsStr,
        error,
        error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_PLATFORM ? "CL_INVALID_PLATFORM" : "CL_OUT_OF_RESOURCES");
        exit(1);
    }

    char *moreInfo = (char*) alloca(sizeof(char) * paramSize);

    error = clGetPlatformInfo(id, param_name, paramSize, moreInfo, NULL);

    if (error != CL_SUCCESS)
    {
        printf("Error: Unable to find any OpenCL Platform information.\n");
        return;
    }
    printf("%s: %s\n", paramNameAsStr, moreInfo);
}

void displayDeviceDetails(cl_device_id id, cl_device_info param_name, const char* paramNameAsStr)
{
    cl_int error = 0;
    size_t paramSize = 0;

    // Get the size of the string that will be queried
    error = clGetDeviceInfo(id, param_name, 0, NULL, &paramSize);

    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve device info paramSize for %s! (%d, %s)\n", 
        paramNameAsStr,
        error,
        error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_DEVICE ? "CL_INVALID_DEVICE" : "CL_OUT_OF_RESOURCES");
        exit(1);
    }

    // For each type of information, there is a different size of buffer to allocate
    switch (param_name)
    {
    case CL_DEVICE_TYPE: 
        {
            // Allocate enough space for the paramSize as a cl_device_type
            cl_device_type* deviceType = (cl_device_type*) alloca(sizeof(cl_device_type) * paramSize);
            // Get the actual value
            error = clGetDeviceInfo(id, param_name, paramSize, deviceType, NULL);
            if (error != CL_SUCCESS)
            {
                printf("Error: Failed to retrieve device info for %s! (%d, %s)\n", 
                paramNameAsStr,
                error,
                error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_DEVICE ? "CL_INVALID_DEVICE" : "CL_OUT_OF_RESOURCES");
                exit(1);
            }
            printf("Device type: %s\n", *deviceType == CL_DEVICE_TYPE_CPU ? "CPU" : *deviceType == CL_DEVICE_TYPE_GPU ? "GPU" : "Unknown");
        }
        break;

    case CL_DEVICE_VENDOR_ID:
        {
            // Allocate enough space for the paramSize as a cl_device_type
            cl_int* vendorId = (cl_int*) alloca(sizeof(cl_int) * paramSize);
            // Get the actual value
            error = clGetDeviceInfo(id, param_name, paramSize, vendorId, NULL);
            if (error != CL_SUCCESS)
            {
                printf("Error: Failed to retrieve device info for %s! (%d, %s)\n", 
                paramNameAsStr,
                error,
                error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_DEVICE ? "CL_INVALID_DEVICE" : "CL_OUT_OF_RESOURCES");
                exit(1);
            }
            printf("Vendor ID: %d\n", *vendorId);
        }
        break;

    
    case CL_DEVICE_MAX_COMPUTE_UNITS:
        {
            // Allocate enough space for the paramSize as a cl_device_type
            cl_uint* maxComputeUnits = (cl_uint*) alloca(sizeof(cl_uint) * paramSize);
            // Get the actual value
            error = clGetDeviceInfo(id, param_name, paramSize, maxComputeUnits, NULL);
            if (error != CL_SUCCESS)
            {
                printf("Error: Failed to retrieve device info for %s! (%d, %s)\n", 
                paramNameAsStr,
                error,
                error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_DEVICE ? "CL_INVALID_DEVICE" : "CL_OUT_OF_RESOURCES");
                exit(1);
            }
            printf("Max compute unit: %d\n", *maxComputeUnits);
        }
        break;

    case CL_DEVICE_MAX_WORK_GROUP_SIZE:
        {
            // Allocate enough space for the paramSize as a cl_device_type
            size_t* maxWorkGroupSize = (size_t*) alloca(sizeof(size_t) * paramSize);
            // Get the actual value
            error = clGetDeviceInfo(id, param_name, paramSize, maxWorkGroupSize, NULL);
            if (error != CL_SUCCESS)
            {
                printf("Error: Failed to retrieve device info for %s! (%d, %s)\n", 
                paramNameAsStr,
                error,
                error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_DEVICE ? "CL_INVALID_DEVICE" : "CL_OUT_OF_RESOURCES");
                exit(1);
            }
            printf("Max work group size: %zu\n", *maxWorkGroupSize);
        }
        break;

    case CL_DEVICE_MAX_WORK_ITEM_SIZES:
        {
            // Allocate enough space for the paramSize as a cl_device_type
            size_t* maxwis = (size_t*) alloca(sizeof(size_t) * paramSize);
            // Get the actual value
            error = clGetDeviceInfo(id, param_name, paramSize, maxwis, NULL);
            if (error != CL_SUCCESS)
            {
                printf("Error: Failed to retrieve device info for %s! (%d, %s)\n", 
                paramNameAsStr,
                error,
                error == CL_INVALID_VALUE ? "CL_INVALID_VALUE" : error == CL_INVALID_DEVICE ? "CL_INVALID_DEVICE" : "CL_OUT_OF_RESOURCES");
                exit(1);
            }
            for(cl_uint i = 0; i < paramSize; i++)
            {
                printf("Max work item size: %zu\n", maxwis[i]);
            }
        }
        break;
    
    default:
        break;

    }

}

void displayDeviceInfo(cl_platform_id id, cl_device_type dev_type)
{
    cl_int error = 0;
    cl_uint numDevices = 0;
    cl_device_id *devices = NULL;

    // Get the number of devices
    error = clGetDeviceIDs(id, dev_type, 0, NULL, &numDevices);
    switch(error)
    {
        case CL_INVALID_PLATFORM:
            printf("Error: Invalid platform!\n");
            break;
        
        case CL_INVALID_DEVICE_TYPE:
            printf("Error: Invalid device type!\n");
            break;

        case CL_DEVICE_NOT_FOUND:
            printf("Error: No devices found of type %llu!\n", dev_type);
            break;

        case CL_INVALID_VALUE:
            printf("Error: Invalid value!\n");
            break;
    }
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to find any OpenCL devices!\n");
        return;
    }

    // Allocate enough space for each device
    devices = (cl_device_id*) alloca(sizeof(cl_device_id) * numDevices);

    // Fill the devices with device IDs
    error = clGetDeviceIDs(id, dev_type, numDevices, devices, NULL);
    switch(error)
    {
        case CL_INVALID_PLATFORM:
            printf("Error: Invalid platform!\n");
            break;
        
        case CL_INVALID_DEVICE_TYPE:
            printf("Error: Invalid device type!\n");
            break;

        case CL_DEVICE_NOT_FOUND:
            printf("Error: No devices found of type %llu!\n", dev_type);
            break;

        case CL_INVALID_VALUE:
            printf("Error: Invalid value!\n");
            break;
    }
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to find any OpenCL devices!\n");
        return;
    }

    printf("Number of devices found: %d\n", numDevices);


    for (int i = 0; i < numDevices; i++)
    {
        printf("Device %d:\n", i);
        displayDeviceDetails(devices[i], CL_DEVICE_TYPE, "CL_DEVICE_TYPE");
        displayDeviceDetails(devices[i], CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID");
        displayDeviceDetails(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
        displayDeviceDetails(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE");
        displayDeviceDetails(devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, "CL_DEVICE_MAX_WORK_ITEM_SIZES");
    }
}

int main(int argc, char** argv)
{
    cl_platform_id* platforms;

    cl_uint numPlatforms;
    cl_int error;
    cl_uint i = 0;

    error = clGetPlatformIDs(0, NULL, &numPlatforms);

    if (error < 0)
    {
        printf("Unable to find any OpenCL platforms\n");
        return 1;
    }

    platforms = (cl_platform_id*) alloca(sizeof(cl_platform_id) * numPlatforms);
    printf("Found %d platforms\n", numPlatforms);

    error = clGetPlatformIDs(numPlatforms, platforms, NULL);
    if (error < 0)
    {
        printf("Unable to find any OpenCL platforms\n");
        return 1;
    }

    for (i = 0; i < numPlatforms; i++)
    {
        displayPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");
        displayPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
        displayPlatformInfo(platforms[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");
        displayPlatformInfo(platforms[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
        displayPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");
        displayDeviceInfo(platforms[i], CL_DEVICE_TYPE_ALL);
    }

    return 0;
}


