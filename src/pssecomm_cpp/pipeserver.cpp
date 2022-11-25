#define NOMINMAX

#include <windows.h>
#include <string>
#include <iostream>
#include <algorithm>

HANDLE hPipe;

extern "C"
{
    int read_outputs_from_pipe(double* outputArray, int num_outputs)
    {
        DWORD dwRead;
        ReadFile(hPipe, outputArray, num_outputs*16 - 1, &dwRead, NULL);
        return 0;
    }

    int write_inputs_to_pipe(double* inputArray, int num_inputs)
    {
        DWORD dwRead;
        WriteFile(hPipe, inputArray, num_inputs * 16 - 1, &dwRead, NULL);

        return 0;
    }

    int start_server(int num_inputs, int num_outputs)
    {
        int max_size = std::max(num_inputs, num_outputs);
        hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\PSSEComm"),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            max_size * 16,
            max_size * 16,
            NMPWAIT_USE_DEFAULT_WAIT,
            NULL);
        ConnectNamedPipe(hPipe, NULL);
        return 0;
    }
}