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
#include <cstdlib>
#include <unistd.h>
using namespace std;

#include "Process.h"

Process CPU(vector<PCB> pcb, vector<Process> job);
Process dispatcher(vector<PCB> * pcb);
vector<Process> readyQueue(vector<PCB> *pcb);
void waitingQueue(vector<PCB> *pcb);
void assignStatus(vector<PCB> *pcb);


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

	vector<vector<Process>> val = pcb[0].getTest();
	vector<Process> abc = val[0];
	assignStatus(&pcb);

	if(pcb[0].getStatus() == "Waiting"){
		waitingQueue(&pcb);

	}
	//else if(pcb[0].getStatus() == "Ready")
		abc = readyQueue(&pcb);

	//aging so that the older processes may have an opportunity to run
	if(pcb.size() > 1){
		pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
	}

	val[0] = abc;
	pcb[0].setTest(val);


	return pcb;
}


void assignStatus(vector<PCB> *pcb){
	vector<vector<Process>> holder = pcb->at(0).getTest();
	vector<Process> storage = holder[0];
	unsigned int j;
	for(j = 0; j < pcb->size(); j++){
		if(storage[0].type == "I/O" && (pcb->at(j).getStatus() != "Running")){
			pcb->at(j).setStatus("Waiting");
		}
		else
			pcb->at(j).setStatus("Ready");
	}
}



// wait in the waiting queue, don't send to CPU
void waitingQueue(vector<PCB> *pcb){
	cout << "Process in PCB, Waiting for I/O" << endl;
	sleep(2);
	pcb->at(0).status = "Ready";
}


vector<Process> readyQueue(vector<PCB> *pcb){
	pcb->at(0).setStatus("Running");
	vector<vector<Process>> level3 = pcb->at(0).getTest();
	vector<Process> abc = level3[0];

	//send from readyQueue to dispatcher
	abc[0] = dispatcher(pcb);
	return abc;
}

Process dispatcher(vector<PCB> *pcb){
	vector<vector<Process>> level3 = pcb->at(0).getTest();
	vector<Process> job = level3[0];
	pcb->at(0).setStatus("Running");
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

	if(runningCPU.size() == 0){ //the CPU is empty, add the process
		runningCPU.push_back(run);
		pcb[0].setStatus("Running");

	}

	else{//the dispatcher is full, we need to save the current process, send it back to the ready queue and add the new process
		temp.push_back(runningCPU[0]);
		runningCPU[0] = run;

	}

	runningCPU[0].currentCycle--;
	pcb[0].setPriority(pcb[0].getPriority() - 1);
	if(runningCPU[0].currentCycle <= 0){
		runningCPU.clear();
	}

	return runningCPU[0];


}



#endif /* SCHEDULER_H_ */
