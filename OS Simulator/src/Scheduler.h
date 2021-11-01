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

Process dispatcher(vector<PCB> pcb, vector<Process> job);

bool comparator(const PCB& lhs, const PCB& rhs) {
   return lhs.priority < rhs.priority;
}

/*if a process is currently running in it's critical section, then we must send the process to the waiting queue
if there is no process in it's critical section, then we send the process to the ready queue
we will send 1 process from the top of the ready queue to the dispatcher at time
the dispatcher can only hold 1 process at a time
we are implementing a shortest time remaining first queue
*/


//the scheduler sorts by least time remaining first
vector<PCB> scheduler(vector<PCB> pcb){
	sort(pcb.begin(), pcb.end(), &comparator);

	//send the job with the lowest priority value to the dispatcher
	pcb[0].status = "Running";
	for(unsigned int i = 1; i < pcb.size(); i++)
		pcb[i].status = "Ready";

	vector<Process> abc = pcb[0].test[0];
	abc[0] = dispatcher(pcb, pcb[0].test[0]);
	pcb[0].test[0] = abc;

	return pcb;
}



vector<Process> readyQueue(Process process){
	static vector<Process>readyQueue;
	readyQueue.push_back(process);
	return readyQueue;
	/*queue of processes ready to run
	 * when a process is no longer in the dispatcher we send the top of the readyQueue to the dispatcher
	 */

}

Process dispatcher(vector<PCB> pcb, vector<Process> job){
	static vector<Process> dispatcher;
	vector<Process> test;
	vector<Process> temp;
	Process zz;
	zz = job[0];

	if(dispatcher.size() == 0){ //the dispatcher is empty, add the process
		dispatcher.push_back(zz);
		pcb[0].status = "Running";

	}

	else{//the dispatcher is full, we need to save the current process, send it back to the ready queue and add the new process
		temp.push_back(dispatcher[0]);
		dispatcher[0] = zz;
		readyQueue(temp[0]);
	}

	dispatcher[0].currentCycle--;
	if(dispatcher[0].currentCycle <= 0){
			dispatcher.clear();
		}


	return dispatcher[0];

	/* hold the currently running process
	 * we want to save it when we try to add a new process
	 * dispatcher should always have a process
	 */
}



#endif /* SCHEDULER_H_ */
