      module PSSECOMM_functions
      contains
      
      subroutine PSSECOMM_ReadWrite()
          use PSSECOMM_variables
          use PSSECOMM_wrapper
          implicit none
          
          ierr = write_inputs_to_pipe(cptrInput, iTxVARNum)
          ierr = read_outputs_from_pipe(cptrOutput, iRxVARNum)

      end subroutine PSSECOMM_ReadWrite

      subroutine PSSECOMM_StartServer()
          use PSSECOMM_variables
          use PSSECOMM_wrapper
          implicit none    

          ierr = start_server(iRxVARNum, iTxVARNum)
      end subroutine PSSECOMM_StartServer
      
      
      subroutine PSSECOMM_Global2Local(iSLOT, iMODE)
          include 'COMON4.INS'
          use PSSECOMM_variables
          implicit none

          integer iSLOT
          integer iMODE

          ! Model indices
          M = STRTCCT(4,iSLOT)
      
          ! Transfering CONs/ICONs to local variables
          iTransferType                                    = ICON(M)    ! Transfer type (STATEs or VARs)
          iTxVARIdx                                        = ICON(M+1)  ! Starting index of VAR to be transfered
          iTxVARNum                                        = ICON(M+2)  ! Number of VARs to be transfered from starting index
          iRxVARIdx                                        = ICON(M+3)  ! Starting index of VAR where the received vars will be written 
          iRxVARNum                                        = ICON(M+4)  ! Number of VARs to be received from starting index
          
          ! Define the pointers
          cptrInput      = c_loc(fInputArray(1))
          cptrOutput     = c_loc(fOutputArray(1))
          
          ! Transfer the VARs to the input array
          if (iTransferType.eq.1) then
              fInputArray(1:iTxVARNum-1) = STATE(iTxVARIdx:(iTxVARIdx+iTxVARNum-1))
          else
              fInputArray(1:iTxVARNum-1) = VAR(iTxVARIdx:(iTxVARIdx+iTxVARNum-1))
          end if
          

      end subroutine PSSECOMM_Global2Local

      subroutine PSSECOMM_Local2Global(iSLOT,iMODE)
          use PSSECOMM_variables
          implicit none
          integer iSLOT
          integer iMODE
          
          VAR(iRxVARIdx:(iRxVARIdx+iRxVARNum-1)) = fOutputArray(1:iRxVARNum-1)
          
      end subroutine PSSECOMM_Local2Global
      
      endmodule PSSECOMM_functions