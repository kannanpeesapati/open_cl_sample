#include "opencv2/opencv.hpp"
#include <CL/cl.hpp>
#include <fstream>

using namespace cv;

bool GetFileData(const char* fname, std:: string& str)
{
	FILE* fp = fopen(fname, "r");
	if (fp == NULL)
	{
		printf("no found filen");
		return false;
	}

	int n = 0;
	while (feof(fp) == 0)
	{
		str += fgetc(fp);
	}

	return true;
}

cv::Mat ClProcess(cv::Mat rgb, std::string path)
{
    //cv::Mat temp(rgb.size(), rgb.type());
    std::string code_file;
    if (false == GetFileData("CLVideo.cl", code_file))
		printf("Error in reading the file");

	char* buf_code = new char[code_file.size()];
	strcpy(buf_code, code_file.c_str());
	buf_code[code_file.size() - 1] = NULL;
    //std::cout << buf_code << std::endl;

    cl_device_id device;
    cl_platform_id platform_id = NULL;
    cl_context context;
    cl_command_queue cmdQueue;
    cl_program program;
    cl_kernel kernel_selectBackground;

    cl_mem rgb_buffer;

    int size_scaled = rgb.rows * rgb.cols;
    size_t one_channel_uchar_size = sizeof(uchar) * size_scaled;
    size_t img_size[1];
    img_size[0]=size_scaled;
    
    auto err = clGetPlatformIDs(1, &platform_id, NULL);

    if (err != CL_SUCCESS)
	{
		std::cout << "clGetPlatformIDs error" << std::endl;
	}

    //choose GPU
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    //step 2:create context
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

    //step 3:create command queue
    cmdQueue = clCreateCommandQueue(context, device, 0, NULL);

    //step 4:create a data buffer
    rgb_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, one_channel_uchar_size * 3, NULL, NULL);
    
    clEnqueueWriteBuffer(cmdQueue, rgb_buffer, CL_FALSE, 0, one_channel_uchar_size * 3, rgb.data, 0, NULL, NULL);
   
    program = clCreateProgramWithSource(context, 1, (const char**)&buf_code, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    kernel_selectBackground = clCreateKernel(program, "selectBackground", NULL);
    clSetKernelArg(kernel_selectBackground, 0, sizeof(cl_mem), &rgb_buffer);

    std::cout << "Path to cl file: " << path << std::endl;

    //OpenCL kernerl execution
    clEnqueueNDRangeKernel(cmdQueue, kernel_selectBackground, 1, NULL, img_size, NULL, 0, NULL, NULL);

    //download data from OpenCL buffer
    clEnqueueReadBuffer(cmdQueue, rgb_buffer, CL_TRUE, 0, one_channel_uchar_size * 3, rgb.data, 0, NULL, NULL);

    return rgb;
}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("edgeDetection",WINDOW_NORMAL);
    namedWindow("Tinted",WINDOW_NORMAL);
    namedWindow("Orignal Image",WINDOW_NORMAL);
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);


        imshow("Orignal Image", frame);
        resizeWindow("Orignal Image", 400,400);

        imshow("edgeDetection", edges);
        resizeWindow("edgeDetection", 400,400);

        cv::Mat clImage = ClProcess(frame, "CLVideo.cl");
        imshow("Tinted", clImage);
        resizeWindow("Tinted", 400,400);
        if(waitKey(30) >= 0) break;

    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
