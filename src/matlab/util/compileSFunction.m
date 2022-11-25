
%% Build the S-Function for the PSS/E Comm block
p = simulinkproject;

cd(fullfile(p.RootFolder,'sfun'));

mex('sfun_pssecomm.cpp','pipeclient.cpp');

cd(p.RootFolder);