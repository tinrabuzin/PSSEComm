import psse35
import psspy


rawpath = r'./smib.raw'
dllpath= r'./dll/PSSECOMM.dll'

dyrpath = r'./smib.dyr'
outfile = r'./smib.out'

# Load the dll and raw file
ierr = psspy.addmodellibrary(dllpath)

psspy.readrawversion(0, '35', rawpath)

# Power flow
psspy.fnsl([0,0,0,1,1,0,99,0])

# Convert generators and loads
ierr = psspy.cong(0)
ierr,rload=psspy.conl(0,1,1,[0,0], [100.0,0.0,0.0, 100.0])
ierr,rload=psspy.conl(0,1,2,[0,0], [100.0,0.0,0.0, 100.0])
ierr,rload=psspy.conl(0,1,3,[0,0], [100.0,0.0,0.0, 100.0])
# Factorization and swithcing
ierr=psspy.fact()
ierr=psspy.tysl(0)

# Preparing the dynamics
ierr=psspy.dyre_new([1,1,1,1],dyrpath)
psspy.voltage_channel([-1,-1,-1,1], 'V') # MV_pos
psspy.branch_p_and_q_channel([-1,-1,-1,1, 4], '1', ['P', 'Q'])
psspy.var_channel([-1,1],r"""SIG_FROM_SIMULINK_1""")
psspy.var_channel([-1,2],r"""SIG_FROM_SIMULINK_2""")
# Running simulation
psspy.dynamics_solution_params(realar3=0.001, outfile=outfile)
psspy.strt(0, outfile)
psspy.run(0, 1.0,100,1,4)
psspy.dist_bus_fault(1, 3, 100, [0.0001, 0.001])
psspy.run(0, 1.14,100,1,4)
psspy.dist_clear_fault(1)
psspy.run(0, 20.0,100,1,10)