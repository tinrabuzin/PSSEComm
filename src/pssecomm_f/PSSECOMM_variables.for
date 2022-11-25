      module PSSECOMM_variables
      use iso_c_binding
      include 'COMON4.INS'
      implicit none      
      
      integer L
      integer M
      integer ierr
      integer iTransferType
      integer iTxVARIdx
      integer iTxVARNum
      integer iRxVARIdx
      integer iRxVARNum
         
      real(c_double),  target :: fInputArray(50)
      real(c_double),  target :: fOutputArray(50)
      type(c_ptr) :: cptrInput
      type(c_ptr) :: cptrOutput
      
      endmodule PSSECOMM_variables