      module PSSECOMM_wrapper
      interface       
        function start_server(iRxVARNum, iTxVARNum) bind (c, name='start_server')
          use iso_c_binding
          integer(c_int), value :: iRxVARNum
          integer(c_int), value :: iTxVARNum
          integer(c_int) :: start_server
        end function start_server

        function read_outputs_from_pipe(outputArray, iRxVARNum) bind (c, name='read_outputs_from_pipe')
              use iso_c_binding
              type(c_ptr),    value :: outputArray
              integer(c_int), value :: iRxVARNum
              integer(c_int) :: read_outputs_from_pipe        
        end function read_outputs_from_pipe
        
        function write_inputs_to_pipe(inputArray, iTxVARNum) bind (c, name='write_inputs_to_pipe')
              use iso_c_binding
              type(c_ptr),    value :: inputArray
              integer(c_int), value :: iTxVARNum
              integer(c_int) :: write_inputs_to_pipe        
        end function write_inputs_to_pipe
      end interface
      endmodule PSSECOMM_wrapper