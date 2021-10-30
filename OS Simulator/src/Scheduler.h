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


bool comparator(const Process& lhs, const Process& rhs) {
   return lhs.actualCycle < rhs.actualCycle;
}

/*if a process is currently running in it's critical section, then we must send the process to the waiting queue
if there is no process in it's critical section, then we send the process to the ready queue
we will send 1 process from the top of the ready queue to the dispatcher at time
the dispatcher can only hold 1 process at a time
we are implementing a shortest time remaining first queue
*/


vector<Process> scheduler(vector<Process> processes){
	//the scheduler sorts by least time remaining first
	sort(processes.begin(), processes.end(), &comparator);
	return processes;
}



vector<Process> readyQueue(Process process){
	static vector<Process>readyQueue;
	readyQueue.push_back(process);
	return readyQueue;
	/*queue of processes ready to run
	 * when a process is no longer in the dispatcher we send the top of the readyQueue to the dispatcher
	 */

}

vector<Process> dispatcher(Process process){
	static vector<Process> dispatcher;
	vector<Process> test;
	vector<Process> temp;
	if(dispatcher.size() == 0){ //the dispatcher is empty, add the process
		dispatcher.push_back(process);

	}

	else{//the dispatcher is full, we need to save the current process, send it back to the ready queue and add the new process
		temp.push_back(dispatcher[0]);
		dispatcher[0] = process;
		readyQueue(temp[0]);
	}

	dispatcher[0].currentCycle--;
	if(dispatcher[0].currentCycle <= 0){
			dispatcher.clear();
		}


	return dispatcher;

	/* hold the currently running process
	 * we want to save it when we try to add a new process
	 * dispatcher should always have a process
	 */
}



#endif /* SCHEDULER_H_ */
