// File			::	test.cpp
// Description 	::	test file to verify the implementation 
//					of open cl
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <chrono>
#include <ctime>

#include <CL/cl.hpp>
namespace betacore{
	void print(const int n, int * A){
		std::cout<<"{";
		for(int i=0; i < n; i++){
			if(i)
			{
				std::cout<<",";
			}
			std::cout<<	A[i];
		}
		std::cout<<"}"<<std::endl;
	}

	//Load kernel file line by line. 
	// file name  abs path to file + kernel name 
	// data : string that will contain the kernel file information
	int load_file(char* file_name, std::string &data)
	{
		std::string line;
		std::ifstream my_file (file_name);
		if (my_file.is_open())
		{
			while ( getline (my_file,line) )
			{
				data+= line +"\n";
			}
			my_file.close();
			return 0;
		}
		else {
				return 1;
			}
	}
}//namespace
//Entry point
int main(int argc, char* argv[]){
	if(argc<2){
		std::cout<< "Usage: Please provide a kernel_locatoin" <<std::endl;
		return -1;
	}
	
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);

	if (all_platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }


	cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    // get default device (CPUs, GPU's) of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
	
    // use device[0] because that's a GPU; device[1] is the CPU
    cl::Device default_device=all_devices[0];

    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

	// a context is like a "runtime link" to the device and platform;
    // i.e. communication is possible
    cl::Context context({default_device});
    // create the program that we want to execute on the device
    cl::Program::Sources sources;

	std::string kernel_code="";
	if(betacore::load_file(argv[1],kernel_code)){
		std::cout<<"Problem Loading kernel file"<<std::endl;
		exit(1);
	}
	//Copy the kernel_code into sources object 
	sources.push_back({kernel_code.c_str(), kernel_code.length()});


	// Create a cl program that we will build.
    cl::Program program(context, sources);

    if (program.build({default_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }

	// Allocate memory need for the GPU
	// This is done with cl::Buffer
	int n = 10;
	int * source = new int[n];
	int * result = new int[n];
	int buffer_size = sizeof(int) * n;
	for(int i=0; i< n; i++){
		source[i]=i;
		result[i]=0;
	}
	betacore::print(n, source);
	cl::Buffer buffer_source(context, CL_MEM_READ_ONLY ,buffer_size);//__constant int * source,
	cl::Buffer buffer_result(context, CL_MEM_READ_WRITE , buffer_size ); //__global int * result,



	// Create A Command queue
	cl::CommandQueue queue(context, default_device);

    // push read-write commands to queue with respect to their buffer
    queue.enqueueWriteBuffer(buffer_source , CL_TRUE, 0, buffer_size, source);
	queue.enqueueWriteBuffer(buffer_result, CL_TRUE, 0, buffer_size, result);

	//GSD -Create kernel the Load data to the kernel
	cl::Kernel test(program, "test");
	test.setArg(0,n);
	test.setArg(1,buffer_source);
	test.setArg(2,buffer_result);

	//Run the kernel with some timing metrics 
	auto start_time = std::chrono::system_clock::now();
	auto start_cpu = std::clock();
	// Run n work groups with 1 work item 
	queue.enqueueNDRangeKernel(test, cl::NullRange ,cl::NDRange( n ),cl::NDRange(1));
	
	// System & cpu time information
	auto end = std::chrono::system_clock::now();
	auto end_cpu = std::clock();
	
	// Copy the result vector(s) back from the gpu 
	queue.enqueueReadBuffer(buffer_result, CL_TRUE, 0, buffer_size, result); 

	//Print out values
	betacore::print(n, result);
	
	//Get the diffrance in time and print out time information
	std::chrono::duration<double> diff = end-start_time;
	std::cout<< "Running time:\t" << diff.count() <<"s" << std::endl;			
	std::cout<< "cpu start: "<< start_cpu << "\t" <<"cpu end:"<<end_cpu<<"\tCLOCKS_PER_SEC:"<<CLOCKS_PER_SEC  <<std::endl;		
	double cpu = (end_cpu - start_cpu) / (double)CLOCKS_PER_SEC ;			
	std::cout<< "cpu time:\t" <<cpu<<"s" << std::endl;

	//Deallocation  memory (if needed)
	delete result;
	result = nullptr;
	delete source;
	source=nullptr;
	return 0;
}