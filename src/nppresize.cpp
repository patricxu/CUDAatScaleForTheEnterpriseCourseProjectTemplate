#include <Exceptions.h>
#include <ImageIO.h>
#include <ImagesCPU.h>
#include <ImagesNPP.h>

#include <string>
#include <fstream>
#include <iostream>

#include <cuda_runtime.h>
#include <npp.h>

#include <helper_cuda.h>
#include <helper_string.h>

bool printfNPPinfo(int argc, char *argv[])
{
    const NppLibraryVersion *libVer = nppGetLibVersion();

    printf("NPP Library Version %d.%d.%d\n", libVer->major, libVer->minor,
           libVer->build);

    int driverVersion, runtimeVersion;
    cudaDriverGetVersion(&driverVersion);
    cudaRuntimeGetVersion(&runtimeVersion);

    printf("  CUDA Driver  Version: %d.%d\n", driverVersion / 1000,
           (driverVersion % 100) / 10);
    printf("  CUDA Runtime Version: %d.%d\n", runtimeVersion / 1000,
           (runtimeVersion % 100) / 10);

    // Min spec is SM 1.0 devices
    bool bVal = checkCudaCapabilities(1, 0);
    return bVal;
}

int main(int argc, char *argv[])
{
    printf("%s Starting...\n\n", argv[0]);

    try
    {
        std::string inputFilePath, outputFilePath;
        float scale = 1.0f; // Default scale

        findCudaDevice(argc, (const char **)argv);

        if (printfNPPinfo(argc, argv) == false)
        {
            exit(EXIT_SUCCESS);
        }

        if (checkCmdLineFlag(argc, (const char **)argv, "input"))
        {
            char *inputPath;
            getCmdLineArgumentString(argc, (const char **)argv, "input", &inputPath);
            inputFilePath = std::string(inputPath);
        }
        else
        {
            printf("Input file path must be specified with the --input flag.\n");
            exit(EXIT_FAILURE);
        }

        if (checkCmdLineFlag(argc, (const char **)argv, "output"))
        {
            char *outputPath;
            getCmdLineArgumentString(argc, (const char **)argv, "output", &outputPath);
            outputFilePath = std::string(outputPath);
        }
        else
        {
            printf("Output file path must be specified with the --output flag.\n");
            exit(EXIT_FAILURE);
        }

        if (checkCmdLineFlag(argc, (const char **)argv, "scale"))
        {
            char *cScale;
            getCmdLineArgumentString(argc, (const char **)argv, "scale", &cScale);
            scale = atof(cScale);
            if (scale <= 0)
            {
                printf("Scale must be a positive number.\n");
                exit(EXIT_FAILURE);
            }
        }

        // Load the input image
        npp::ImageCPU_8u_C1 oHostSrc;
        npp::loadImage(inputFilePath, oHostSrc);
        npp::ImageNPP_8u_C1 oDeviceSrc(oHostSrc);

        // Calculate new size based on scale
        NppiSize srcSize = {static_cast<int>(oDeviceSrc.width()), static_cast<int>(oDeviceSrc.height())};
        NppiSize dstSize = {static_cast<int>(srcSize.width * scale), static_cast<int>(srcSize.height * scale)};

        // Allocate memory for the resized image
        npp::ImageNPP_8u_C1 oDeviceDst(dstSize.width, dstSize.height);

        // Perform resizing
        const int nInterpolation = NPPI_INTER_LINEAR;
        const double xScale = static_cast<double>(scale);
        const double yScale = static_cast<double>(scale);

        NPP_CHECK_NPP(nppiResizeSqrPixel_8u_C1R(
            oDeviceSrc.data(), srcSize, oDeviceSrc.pitch(), {0, 0, srcSize.width, srcSize.height},
            oDeviceDst.data(), oDeviceDst.pitch(), {0, 0, dstSize.width, dstSize.height},
            xScale, yScale, 0, 0, nInterpolation));

        // Save the resized image
        npp::ImageCPU_8u_C1 oHostDst(oDeviceDst.size());
        oDeviceDst.copyTo(oHostDst.data(), oHostDst.pitch());
        saveImage(outputFilePath, oHostDst);
        std::cout << "Saved resized image to: " << outputFilePath << std::endl;

        // Free NPP resources
        nppiFree(oDeviceSrc.data());
        nppiFree(oDeviceDst.data());

        exit(EXIT_SUCCESS);
    }
    catch (npp::Exception &rException)
    {
        std::cerr << "Program error! The following exception occurred: \n"
                  << rException << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (...)
    {
        std::cerr << "Program error! An unknown type of exception occurred. \n"
                  << "Aborting." << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
