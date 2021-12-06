/*
 * Scheduler2.h
 *
 *  Created on: Dec 5, 2021
 *      Author: Hana Parece
 *      This is the new scheduler for the program, NOT a second scheduler.
 *      THE ORIGINAL SCHEDULER WITHOUT MULTITHREADING IS scheduler.h
 *
 *      Scheduler2 was created to implement multithreading.
 *
 *      I spent more time trying to get the mulithreading libraries to work with Eclipse IDE
 *      than I did actually trying to implement multithreading. In fact, I never could get it to work
 *      despite my best efforts and trying basically every fix on the internet.
 *      In theory, I believe this method should work, but I was unable to test and run on my machine.
 */

#ifndef SCHEDULER2_H_
#define SCHEDULER2_H_

#include <pthread.h>

#include <iostream>
#include <list>
#include <string>
#include <time.h>
#include<algorithm>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <unistd.h>

using namespace std;

#include "Process.h"

void CPU2(vector<Process> *job);
void dispatcher2(vector<PCB> **pcb);
void readyQueue2(vector<PCB> *pcb, int ***memoryInUse);



bool comparator(const PCB& lhs, const PCB& rhs) {
	return lhs.getPriority() < rhs.getPriority();
}

bool sorter(const PCB& lhs, const PCB& rhs) {
	return lhs.getStatus() < rhs.getStatus();
}

vector<PCB> scheduler2(vector<PCB> pcb, int **memoryInUse, clock_t **time){
	clock_t timer;
	static int count = 0;
	static vector<int> memoried;
	static vector<int>::iterator it;
	static vector<int> IDs;
	vector<vector<Process>> val = pcb[0].getTest();
	vector<Process> abc = val[0];
	vector<Process> Fork;
	vector<Process> temp1;
	static vector<PCB> waiting;
	Process hold;
	int key = pcb[pcb.size() -1].getPid();
	int newForkID = 0;

	/*count memory for the processes, we only want to add the memory that is being used once
	//if the memory of the process we just received is greater than the memory we have available
	//then the process needs to stay in the "New" or "Waiting" queue
	//if the processID is in it, then it will be sent to the "Waiting" Queue
	otherwise the process stays in the "New" state
	 */

	if(pcb[pcb.size() - 1].getMemoryUse() > (1024 - **memoryInUse)){
		// The memory required for the process is greater than the memory we have...

		it = find(memoried.begin(), memoried.end(), pcb[pcb.size()-1].getPid());

		if(it != memoried.end()){
			//the process has already been run once, so we want it to be "Waiting"

			sort(pcb.begin(), pcb.end() - 1, &comparator);
			//if the type equals FORK, we want to create a child process
			if(abc[0].getType() == "FORK" && abc[0].getActualCycle() == abc[0].getCurrentCycle()){

				for(unsigned int l = 1; l < abc.size(); l++){
					//create the child process
					Process process(abc[l].getType(), abc[l].getMinCycle(), abc[l].getMaxCycle(), abc[l].isIsCritical());
					process.setMemoryNeeded(abc[l].getMemoryNeeded());
					process.setActualCycle(abc[l].getActualCycle());
					process.setCurrentCycle(abc[l].getCurrentCycle());
					Fork.push_back(process);
				}
				//newForkID holds all IDS of the processes that have been forked, the most recent ID will be the maximum
				//of which we will use to create the child's ID and keep track of the child's parent
				newForkID= *max_element(IDs.begin(), IDs.end()) + 1;
				timer = clock () - **time;
				PCB holder(Fork,newForkID,(float)timer/CLOCKS_PER_SEC);
				holder.setparentID(pcb[0].getPid());
				pcb.push_back(holder);
			}

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
				readyQueue2(&pcb, &memoryInUse);
			}

			//aging so that the older processes may have an opportunity to run
			if(pcb.size() > 2){
				pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
			}

			return pcb;
		}
		else{
			//the process has not been run, the status will stay as new and it will not enter the scheduler
			pcb[pcb.size()-1].setStatus("New");
			sort(pcb.begin(), pcb.end() - 1, &comparator);

			//if the type equals fork, we want to create a child process
			if(abc[0].getType() == "FORK" && abc[0].getActualCycle() == abc[0].getCurrentCycle()){
				for(unsigned int l = 1; l < abc.size(); l++){
					//create the child process
					Process process(abc[l].getType(), abc[l].getMinCycle(), abc[l].getMaxCycle(), abc[l].isIsCritical());
					process.setMemoryNeeded(abc[l].getMemoryNeeded());
					process.setActualCycle(abc[l].getActualCycle());
					process.setCurrentCycle(abc[l].getCurrentCycle());
					Fork.push_back(process);
				}
				//newForkID holds all IDS of the processes that have been forked, the most recent ID will be the maximum
				//of which we will use to create the child's ID and keep track of the child's parent
				newForkID= *max_element(IDs.begin(), IDs.end()) + 1;
				timer = clock () - **time;
				PCB holder(Fork,newForkID,(float)timer/CLOCKS_PER_SEC);
				holder.setparentID(pcb[0].getPid());
				pcb.push_back(holder);
			}

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
				readyQueue2(&pcb, &memoryInUse);
			}

			//aging so that the older processes may have an opportunity to run
			if(pcb.size() > 2){
				pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
			}

			return pcb;
		}
	}

	//there are no memory issues, we continue as normal
	else{
		IDs.push_back(pcb[pcb.size() -1].getPid());
		it = find(memoried.begin(), memoried.end(), key);
		if(it != memoried.end()){

		}
		else{
			**memoryInUse += pcb[pcb.size() -1].getMemoryUse();
			memoried.push_back(pcb[pcb.size() -1].getPid());

		}

		sort(pcb.begin(), pcb.end(), &comparator);

		//if the type equals fork, we want to create a child process
		if(abc[0].getType() == "FORK" && abc[0].getActualCycle() == abc[0].getCurrentCycle()){
			for(unsigned int l = 1; l < abc.size(); l++){
				//create the child process
				Process process(abc[l].getType(), abc[l].getMinCycle(), abc[l].getMaxCycle(), abc[l].isIsCritical());
				process.setMemoryNeeded(abc[l].getMemoryNeeded());
				process.setActualCycle(abc[l].getActualCycle());
				process.setCurrentCycle(abc[l].getCurrentCycle());
				Fork.push_back(process);
			}
			//newForkID holds all IDS of the processes that have been forked, the most recent ID will be the maximum
			//of which we will use to create the child's ID and keep track of the child's parent
			newForkID= *max_element(IDs.begin(), IDs.end()) + 1;
			timer = clock () - **time;
			PCB holder(Fork,newForkID,(float)timer/CLOCKS_PER_SEC);
			holder.setparentID(pcb[0].getPid());
			pcb.push_back(holder);
		}


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
			readyQueue2(&pcb, &memoryInUse);
		}

		//aging so that the older processes may have an opportunity to run
		if(pcb.size() > 2){
			pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
		}

		return pcb;
	}
}


void  readyQueue2(vector<PCB> *pcb, int ***memoryInUse){
	vector<vector<Process>> level3 = pcb->at(0).getTest();
	vector<Process> abc = level3[0];

	//send from readyQueue to dispatcher
	for(unsigned int i = 0; i < pcb->size(); i++){
		if(pcb->at(i).getMemoryUse() <= (1024 - ***memoryInUse))
			pcb->at(i).setStatus("Ready");
	}
	dispatcher2(&pcb);
}


//dispatcher

void dispatcher2(vector<PCB> *pcb){

	for(unsigned int j = 0; j < 4 && j < pcb->size(); j++){
		int priority = pcb->at(j).getPriority();
		vector<vector<Process>> level3 = pcb->at(j).getTest();
		vector<Process> job = level3[0];
		static vector<PCB> store;
		priority = 0;
		for(unsigned int i = 0; i < job.size(); i++)
			priority += job[i].getActualCycle();
		pcb->at(j).setPriority(priority);
		pcb->at(j).setStatus("Running");
		priority--;
		pcb->at(j).setPriority(priority);

		//create the 4 threads to send to the CPU

		if(j == 0){
			thread t1(CPU2,&job);
			level3[0] = job;
			pcb->at(j).setTest(level3);
		}
		else if(j == 1){
			thread t1(CPU2,&job);
			level3[0] = job;
			pcb->at(j).setTest(level3);
		}
		else if(j == 2){
			thread t1(CPU2,&job);
			level3[0] = job;
			pcb->at(j).setTest(level3);
		}
		else if(j == 3){
			thread t1(CPU2,&job);
			level3[0] = job;
			pcb->at(j).setTest(level3);
		}

		t1.join();
		t2.join();
		t3.join();
		t4.join();
	}
}

/* hold the currently running process
 * we want to save it when we try to add a new process
 * CPU should always have a process
 */


void CPU2(vector<Process> *job){
	static vector<Process> runningCPU; //holds a single, running process

	vector<Process> temp; //temporary vector
	Process run = job->at(0); //the process that is to be run

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

	job->at(0) = runningCPU[0];
}





#endif /* SCHEDULER2_H_ */
