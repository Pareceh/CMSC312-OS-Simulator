The OS Simulator is largely user driven. Cycles are completed after a menu is displayed, or many cycles can be simulated at once.

To Run: I switched over to VSCode using MinGW-x64, as it was the only way I could get the threads to compile and run correctly. Still seems to compile in Eclipse though with MinGW-x64.

Important Note: In this program, the lower the priority the better. Lower priority is run first.

Upon loading file:

[1] continues the program [0] exits. No cycle is run here because the simulator is just loaded.
[1] Uses Priority Scheduler [2] uses Shortest Time Remaining Scheduler

[2] loads a job from 1 of 5 templates: template1, template2, template3, template4, and template5. The simulator is designed so additional templates can be created and saved in the templates folder. The naming scheme does not need to be consistent, so long as they are .txt. A cycle is run.

[3] displays all currently non-terminated processes in the simulator. A cycle is run.

[4] displays a help menu. A cycle is run.

[5] simulates as many cycles as the user wishes without interruption. 

[0] exits the simulator.


Example after loading from template1

The program is split into 4 files, Process.h, Scheduler.h, Scheduler2.h  and OS Simulator.cpp. 
	Process.h contains most of the information relating to the PCB and Process objects.
	Scheduler1.h contains the information for scheduling and running processes on the CPU using a simple Priority scheduler.
	Scheduler2.h contains the information for scheduling and running processes on the CPU using a Shortest Time Remaining scheduler. 
	OS Simulator.cpp is where the main method is, as well as the method for running 1 cycle.

Phase III Features:
Interprocess Communication Method “Inbox”
	> Process stores a message into an inbox, and then later the first process in the PCB picks up the message. Until FORK calls are started, there would typically only be one message in the inbox at a time. 

Two Schedulers and Comparison
	> There are two schedulers implemented, a Shortest Time Remaining scheduler and a simple Priority scheduler. User is asked when the program is first loaded which scheduler they would like to primarily use. Both schedulers are run and compared, but only the scheduler that was requested actually saves any information.Time for each is displayed to the console. 

Process Priorities
	> Processes are assigned priorities depending on which scheduler is chosen. Priority = calculate processes +  a base value. Shortest Time Remaining = number of cycles left. 

Process Resources
	> Processes request a random value of resources upon creation. There is a random chance for all resources to be released when a cycle is finished, and all resources are released when the process finishes.

Multi-level parent-child relationship
	> Everytime a FORK is called, a child process is made. Children can also make children processes if they call a FORK. Cascading termination is implemented to remove all children if their parent is missing. Works for grandchildren too. 

Deadlock avoidance algorithm
	> Banker’s algorithm is implemented, stops process if there are not enough resources. Deadlock can also be prevented by having all the threads join() and wait for each other. 

Known limitations:
> Program crashes if it tries to run a cycle, but there are no user loaded processes.
> There is currently no critical section resolution.
> A cycle is only done when a user interacts with the UI. This is rather clunky to say the least.
