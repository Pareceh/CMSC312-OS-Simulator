/*
 * Scheduler.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Hana Parece
 *      Used to define the scheduler that schedules the processes.
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include <iostream>
#include <list>
#include <string>
#include<algorithm>
#include <stdexcept>
#include <vector>
using namespace std;

#include "Process.h"

Process CPU(vector<PCB> pcb, vector<Process> job);
Process dispatcher(vector<PCB> * pcb);

bool comparator(const PCB& lhs, const PCB& rhs) {
	return lhs.priority < rhs.priority;
}

/*if a process is currently running in it's critical section, then we must send the process to the waiting queue
if there is no process in it's critical section, then we send the process to the ready queue
we will send 1 process from the top of the ready queue to the dispatcher at time
the dispatcher can only hold 1 process at a time
we are implementing a shortest time remaining first queue

scheduler -> queue -> dispatcher -> cpu
 */


//the scheduler sorts by least time remaining first
vector<PCB> scheduler(vector<PCB> pcb){
	sort(pcb.begin(), pcb.end(), &comparator);
	vector<Process> abc = pcb[0].test[0];

	if(abc[0].type == "I/O"){
		pcb[0].status = "Waiting";
	}
	else{
	}
	for(unsigned int i = 1; i < pcb.size(); i++)
		pcb[i].status = "Ready";
	//send the job with the lowest priority value to the dispatcher

	abc[0] = dispatcher(&pcb);
	pcb[0].priority--;

	//aging so that the older processes may have an opportunity to run
	if(pcb.size() > 1){
		pcb[pcb.size()- 1].priority--;
	}

	pcb[0].test[0] = abc;

	return pcb;
}

waitingQueue(vector<PCB> pcb){

}


vector<Process> readyQueue(vector<PCB> pcb){

}

Process dispatcher(vector<PCB> * pcb){
	vector<Process> job = pcb->at(0).test[0];
	pcb->at(0).status = "Running";
	return CPU(*pcb,job);
}

/* hold the currently running process
 * we want to save it when we try to add a new process
 * CPU should always have a process
 */
Process CPU(vector<PCB> pcb, vector<Process> job){
	static vector<Process> runningCPU; //holds a single, running process
	vector<Process> temp; //temporary vector
	Process run = job[0]; //the process that is to be run
	bool inCrit = run.isCritical;


	if(runningCPU.size() == 0){ //the CPU is empty, add the process
		runningCPU.push_back(run);
		pcb[0].status = "Running";

	}

	else{//the dispatcher is full, we need to save the current process, send it back to the ready queue and add the new process
		temp.push_back(runningCPU[0]);
		runningCPU[0] = run;
		//readyQueue(temp[0]);
	}

	runningCPU[0].currentCycle--;
	pcb[0].priority--;
	if(runningCPU[0].currentCycle <= 0){
		runningCPU.clear();
	}

	return runningCPU[0];


}



#endif /* SCHEDULER_H_ */
