# CMSC312-OS-Simulator
CMSC 312 VCU OS Simulator Project for Fall 2021

The OS Simulator is largely user driven. Cycles are completed after a menu is displayed, or many cycles can be simulated at once.

To Run: I’ll be honest, I’m new to C++. I’ve been doing the project in the Eclipse C++ IDE which lets me run straight from there. I believe there is an .exe in the debug folder, but I cannot get it to run on my own machine outside of Eclipse. 

Upon loading file:
[1] continues the program [0] exits. No cycle is run here because the simulator is just loaded.

[2] loads a job from 1 of 5 templates: template1, template2, template3, template4, and template5. The simulator is designed so additional templates can be created and saved in the templates folder. The naming scheme does not need to be consistent, so long as they are .txt. A cycle is run.

[3] displays all currently non-terminated processes in the simulator. A cycle is run.

[4] displays a help menu. A cycle is run.

[5] simulates as many cycles as the user wishes without interruption. 

[0] exits the simulator.






Known limitations:
> Program crashes if it tries to run a cycle, but there are no user loaded processes.
> 

Goals for next Phase: 
> Have process information print out to a txt file on regular intervals. 
> Have cycles run at regular intervals instead of only after user input. 
> Critical section resolution is done through waiting, and not increasing priority of the critical process
>> I/O waiting queue and I/O wait
