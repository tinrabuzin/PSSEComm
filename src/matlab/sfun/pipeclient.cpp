#include <vector>
#include <utility>
#include <iostream>
#include <memory>
#include <windows.h>

#include "pipeclient.hpp"

namespace {

#define NUM_INPUTS 20
#define NUM_OUTPUTS 2

// class ZmqMgr for managing socket connection with the server
class Plumber {
  public:
    Plumber()
    {
        hPipe = CreateFile(TEXT("\\\\.\\pipe\\PSSEComm"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    }

    ~Plumber() {
        CloseHandle(hPipe);
    }

    void write_outputs(const double *outputArray);

    void read_inputs(double *inputArray);
    
  private:
    HANDLE hPipe;
    //std::string pipe_name;
};


void Plumber::write_outputs(const double *outputArray){
    DWORD dwWritten;
    WriteFile(hPipe, outputArray, NUM_OUTPUTS * 16 - 1, &dwWritten, NULL);
}

void Plumber::read_inputs(double *inputArray){
    DWORD dwRead;
    ReadFile(hPipe, inputArray, NUM_INPUTS * 16 - 1, &dwRead, NULL);

}

}

// Wrapper functions
void *setupruntimeresources_wrapper()
{
    return reinterpret_cast<void *>(new Plumber());
}

void cleanupruntimeresouces_wrapper(void *pl)
{
    delete reinterpret_cast<Plumber *>(pl);
}

// Helper function to send a request with input arguments to the server
void read_inputs_helper(void *pl, double *inputArray)
{
    reinterpret_cast<Plumber *>(pl)->read_inputs(inputArray);
}

void write_outputs_helper(void *pl, const double *outputArray)
{
    reinterpret_cast<Plumber *>(pl)->write_outputs(outputArray);
}