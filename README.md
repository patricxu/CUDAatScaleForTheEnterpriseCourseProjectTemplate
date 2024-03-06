# CUDAatScaleForTheEnterpriseCourseProjectTemplate
This is a template for the course project for the CUDA at Scale for the Enterprise

## Project Description

The NPP Image Resizer is a high-performance image processing application developed to showcase the capabilities of NVIDIA's Performance Primitives Library (NPP) for efficient image resizing operations. This project leverages the power of CUDA-enabled GPUs to achieve fast and high-quality image resizing, supporting various image formats. The purpose of this project is to provide developers and researchers with a tool that can be easily integrated into their workflows for image manipulation tasks, such as scaling images up or down with minimal loss of quality. By utilizing the GPU for computation, the NPP Image Resizer significantly reduces processing time compared to CPU-based image resizing methods, making it ideal for applications that require real-time image processing or handling large datasets.

## Code Organization

```bin/```
This folder should hold all binary/executable code that is built automatically or manually. Executable code should have use the .exe extension or programming language-specific extension.

```data/```
The folder contains the input data.

```Common/```
The hpp headers are in this folder

```src/```
The source code folder

```Makefile```
Clean: Removes all compiled objects and executables to ensure a fresh start.
Command: make clean
Build: Compiles the source code into executable form.
Command: make build
Run: Executes the compiled application.
Command: make run
All: Performs cleaning, building, and running in sequence.
Command: make all

```run.sh```
This run the application with three parameters
bin/nppresize.exe --input path/to/your/input/image --output path/to/your/output/file --scale 2.0

