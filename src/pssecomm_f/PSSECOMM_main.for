      subroutine PSSECOMM(MINS,SLOT)
          use PSSECOMM_variables
          use PSSECOMM_functions
          
          include 'COMON4.INS'
          implicit none
      
          integer SLOT
          integer MINS

          call PSSECOMM_Global2Local(SLOT, MODE)
      
          select case (MODE)
              ! MODE 1 - Initialization
              case(1)
                  call PSSECOMM_StartServer()
              ! MODE 2 - Calculate derivatives
              case(2)
                  ! Not implemented
                  return
              ! MODE 3 - Calculating model outputs
              case(3)
                  
                 call PSSECOMM_ReadWrite()
              ! MODE 4 - Set number of states
              case(4)
                  if (MIDTRM) then
                      call NOTMID
                  else
                      NINTEG=MAX(NINTEG,0)
                  end if
              ! MODE 5 - Model report
              case(5)
                  call docuheading
              ! MODE 6 - Activity DYDA
              case(6)
                  ! Not implemented
                  return
              ! MODE 7 - Model check
              case(7)
                  ! Not implemented
                  return
              ! MODE 8 - Data descriptions
              case(8)
                  ! Not implemented    
                  return
          end select
          
          ! Transfer from local working space to global arrays (for simulation mode calls)
          if ( (MODE.le.4)) then
              call PSSECOMM_Local2Global(SLOT,MODE)
          end if

      end subroutine PSSECOMM



