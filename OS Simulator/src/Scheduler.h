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

Process CPU(vector<Process> job);
Process dispatcher(vector<PCB> * pcb);
vector<Process> readyQueue(vector<PCB> *pcb, int ***memory);


bool comparator(const PCB& lhs, const PCB& rhs) {
	return lhs.getPriority() < rhs.getPriority();
}

bool sorter(const PCB& lhs, const PCB& rhs) {
	return lhs.getStatus() < rhs.getStatus();
}


/*if a process is currently running in it's critical section, then we must send the process to the waiting queue
if there is no process in it's critical section, then we send the process to the ready queue
we will send 1 process from the top of the ready queue to the dispatcher at time
the dispatcher can only hold 1 process at a time
we are implementing a shortest time remaining first queue

scheduler -> queue -> dispatcher -> cpu
 */


//the scheduler sorts by least time remaining first
vector<PCB> scheduler(vector<PCB> pcb, int **memoryInUse){
	static int count = 0;
	static vector<int> memoried;
	static vector<int>::iterator it;
	vector<vector<Process>> val = pcb[0].getTest();
	vector<Process> abc = val[0];
	static vector<PCB> waiting;
	vector<Process> temp1;
	Process hold;
	int key = pcb[pcb.size() -1].getPid();

	//count memory for the processes, we only want to add the memory that is being used once
	//if the memory of the process we just received is greater than the memory we have available
	//then the process needs to stay in the "New" or "Waiting" queue
	//if the processID is in it, then it will be sent to the "Waiting" Queue
	//otherwise the process stays in the "New" state

	if(pcb[pcb.size() - 1].getMemoryUse() > (1024 - **memoryInUse)){

		it = find(memoried.begin(), memoried.end(), pcb[pcb.size()-1].getPid());

		if(it != memoried.end()){
			//send to waiting
			if(abc[0].getType() == "I/O" && (abc[0].getActualCycle() == abc[0].getCurrentCycle())){
				pcb[0].setStatus("Waiting");
				pcb[pcb.size()-1].setStatus("Waiting");
				count++;
				if(pcb.size() >1 ){
					auto mi = pcb.begin() + pcb.size() -1;
					rotate(mi, mi + 1, pcb.end());
				}
				if(count == 3){
					pcb[0].setStatus("Ready");
					count = 0;
				}

			}

			//send to ready queue
			if(pcb[0].getStatus()!= "Waiting"){
				pcb[0].setStatus("Ready");
				abc = readyQueue(&pcb, &memoryInUse);
			}

			//aging so that the older processes may have an opportunity to run
			if(pcb.size() > 2){
				pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
			}

			val[0] = abc;
			pcb[0].setTest(val);

			return pcb;
		}
		else{
			//status stays as new
			pcb[pcb.size()-1].setStatus("New");
			sort(pcb.begin(), pcb.end() - 1, &comparator);

			//here is the waiting queue
			if(abc[0].getType() == "I/O" && (abc[0].getActualCycle() == abc[0].getCurrentCycle())){
				pcb[0].setStatus("Waiting");
				count++;
				if(pcb.size() >1 ){
					auto mi = pcb.begin() + pcb.size() -1;
					rotate(mi, mi + 1, pcb.end());
				}
				if(count == 3){
					pcb[0].setStatus("Ready");
					count = 0;
				}

			}

			//send to ready queue
			if(pcb[0].getStatus()!= "Waiting"){
				pcb[0].setStatus("Ready");
				abc = readyQueue(&pcb, &memoryInUse);
			}

			//aging so that the older processes may have an opportunity to run
			if(pcb.size() > 2){
				pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
			}

			val[0] = abc;
			pcb[0].setTest(val);

			return pcb;
		}
	}

	//there are no memory issues, continue
	else{
		it = find(memoried.begin(), memoried.end(), key);
		if(it != memoried.end()){

		}
		else{
			**memoryInUse += pcb[pcb.size() -1].getMemoryUse();
			memoried.push_back(pcb[pcb.size() -1].getPid());

		}

		sort(pcb.begin(), pcb.end(), &comparator);


		//here is the waiting queue
		if(abc[0].getType() == "I/O" && (abc[0].getActualCycle() == abc[0].getCurrentCycle())){
			pcb[0].setStatus("Waiting");
			count++;
			if(pcb.size() >1 ){
				auto mi = pcb.begin() + pcb.size();
				rotate(mi, mi + 1, pcb.end());
			}
			if(count == 3){
				pcb[0].setStatus("Ready");
				count = 0;
			}

		}

		//send to ready queue
		if(pcb[0].getStatus()!= "Waiting"){
			pcb[0].setStatus("Ready");
			abc = readyQueue(&pcb, &memoryInUse);
		}

		//aging so that the older processes may have an opportunity to run
		if(pcb.size() > 2){
			pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
		}

		val[0] = abc;
		pcb[0].setTest(val);

		return pcb;
	}
}

//ready Queue
vector<Process> readyQueue(vector<PCB> *pcb, int ***memoryInUse ){
	vector<vector<Process>> level3 = pcb->at(0).getTest();
	vector<Process> abc = level3[0];

	//send from readyQueue to dispatcher
	for(unsigned int i = 0; i < pcb->size(); i++){
		if(pcb->at(i).getMemoryUse() <= (1024 - ***memoryInUse))
			pcb->at(i).setStatus("Ready");
	}
	abc[0] = dispatcher(pcb);

	return abc;
}


//dispatcher
Process dispatcher(vector<PCB> *pcb){
	int priority = pcb->at(0).getPriority();
	vector<vector<Process>> level3 = pcb->at(0).getTest();
	vector<Process> job = level3[0];
	static vector<PCB> store;

	//critical section resolution, not perfect and would like to edit for the next phase
	// resolves critical section by if the process that is about to run is in it's critical section
	// it has the highest priority so will not be interrupted
	/*if(job[0].isIsCritical()){
		pcb->at(0).setStatus("Running");
		pcb->at(0).setPriority(-1000);
	}
	else{*/
	//priority = 0;
	for(unsigned int i = 0; i < job.size(); i++)
		priority += job[i].getActualCycle();
	pcb->at(0).setPriority(priority);
	pcb->at(0).setStatus("Running");
	priority--;
	pcb->at(0).setPriority(priority);
	//}
	return CPU(job);
}

/* hold the currently running process
 * we want to save it when we try to add a new process
 * CPU should always have a process
 */
Process CPU(vector<Process> job){
	static vector<Process> runningCPU; //holds a single, running process

	vector<Process> temp; //temporary vector
	Process run = job[0]; //the process that is to be run

	if(runningCPU.size() == 0){ //the CPU is empty, add the process
		runningCPU.push_back(run);
	}

	else{//the CPU is running, we need to save the current process, send it back to the ready queue and add the new process
		temp.push_back(runningCPU[0]);
		runningCPU[0] = run;
	}

	runningCPU[0].setCurrentCycle(runningCPU[0].getCurrentCycle() -1);
	if(runningCPU[0].getCurrentCycle() <= 0){
		runningCPU.clear();
	}

	return runningCPU[0];


}



#endif /* SCHEDULER_H_ */
