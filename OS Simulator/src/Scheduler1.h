/*
 * Scheduler2.h
 *
 *  Created on: Dec 5, 2021
 *      Author: Hana Parece
 *    This is a secondary scheduler for the program.
 *      This scheduler is a priority scheduler, where the priorites are randomly defined as opposed
 *      to the other scheduler, which is a shortest job first scheduler.
 */

#ifndef SCHEDULER1_H_
#define SCHEDULER1_H_

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

void CPU1(vector<Process> *job);
vector<PCB> dispatcher1(vector<PCB> **pcb);
vector<PCB> readyQueue1(vector<PCB> *pcb, int ***memoryInUse);



bool comparator1(const PCB& lhs, const PCB& rhs) {
	return lhs.getPriority() < rhs.getPriority();
}

bool sorter1(const PCB& lhs, const PCB& rhs) {
	return lhs.getStatus() < rhs.getStatus();
}

vector<PCB> scheduler1(vector<PCB> pcb, int **memoryInUse, clock_t **time){
	clock_t timer;
	static int count = 0;
    int calculateCount = 0;
    static int arbiCount = 0;
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

    //here we assign priorities to all the processes, priority is defined by the number of calculate functions + a counter
    for(unsigned int m =0; m < pcb.size(); m++){
        if(pcb[m].getPriority() == 0){
        for(unsigned int n = 0; n < abc.size(); n++){
            if(abc[m].getType() == "CALCULATE"){
                calculateCount++;
            }
        }
        arbiCount++;
        pcb[m].setPriority(calculateCount + arbiCount);
        calculateCount = 0;
        }
    }

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

			sort(pcb.begin(), pcb.end() - 1, &comparator1);
			//if the type equals FORK, we want to create a child process
			if(abc[0].getType() == "FORK" && abc[0].getActualCycle() == abc[0].getCurrentCycle()){

				if(abc.size() > 1){ //if there is no other process after the fork, we don't have anything TO fork.
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
				PCB holder(Fork,newForkID,(float)timer/CLOCKS_PER_SEC,1);
				holder.setparentID(pcb[0].getPid());
				pcb.push_back(holder);
			}
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
				pcb = readyQueue1(&pcb, &memoryInUse);
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
			sort(pcb.begin(), pcb.end() - 1, &comparator1);

			if(abc.size() > 1){ //if there is no other process after the fork, we don't have anything TO fork.
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
				PCB holder(Fork,newForkID,(float)timer/CLOCKS_PER_SEC,1);
				holder.setparentID(pcb[0].getPid());
				pcb.push_back(holder);
			}
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
				pcb = readyQueue1(&pcb, &memoryInUse);
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

		sort(pcb.begin(), pcb.end(), &comparator1);

	if(abc.size() > 1){ //if there is no other process after the fork, we don't have anything TO fork.
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
			PCB holder(Fork,newForkID,(float)timer/CLOCKS_PER_SEC,1);
			holder.setparentID(pcb[0].getPid());
			pcb.push_back(holder);
		}
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
			pcb = readyQueue1(&pcb, &memoryInUse);
		}

		//aging so that the older processes may have an opportunity to run
		if(pcb.size() > 2){
			pcb[pcb.size()- 1].setPriority(pcb[pcb.size()- 1].getPriority() - 1);
		}


		return pcb;
	}
}

//ready Queue
vector<PCB> readyQueue1(vector<PCB> *pcb, int ***memoryInUse ){
	vector<vector<Process>> level3 = pcb->at(0).getTest();
	vector<Process> abc = level3[0];
	vector<PCB> test;

	//send from readyQueue to dispatcher
	for(unsigned int i = 0; i < pcb->size(); i++){
		if(pcb->at(i).getMemoryUse() <= (1024 - ***memoryInUse))
			pcb->at(i).setStatus("Ready");
	}
	test = dispatcher1(&pcb);
	level3 = test[0].getTest();
	abc = level3[0];

	return test;
}

//dispatcher

vector<PCB> dispatcher1(vector<PCB> **pcb){
    vector<PCB> newPCB = **pcb;
    thread th1,th2,th3,th4;
	vector<Process> jobT1, jobT2, jobT3, jobT4;
	vector<vector<Process>> level3;
	vector<Process> job;
	int priority;
	int jsave;

	for(unsigned int j = 0; j < 4 && j < newPCB.size(); j++){
		priority = newPCB[j].getPriority();
		level3 = newPCB[j].getTest();
		job = level3[0];

		//we will use these to help keep track of the threads
		if(j == 0)
		jobT1 = job;
		else if(j == 1)
		jobT2 = job;
		else if(j ==2)
		jobT3 = job;
		else if(j ==3)
		jobT4 = job;

		newPCB[j].setStatus("Running");
		jsave = j;
	}

	//create the 4 threads to send to the CPU
				if(newPCB.size() > 0){
			if(jsave == 0){
				th1 = thread (CPU1,&jobT1);
			}
			else if(jsave == 1){
				th1 = thread (CPU1,&jobT1);
				th2 = thread (CPU1,&jobT2);
			}
			else if(jsave == 2){
				th1 = thread (CPU1,&jobT1);
				th2 = thread (CPU1,&jobT2);
				th3 = thread (CPU1,&jobT3);
			}
			else if(jsave > 3){
				th1 = thread (CPU1,&jobT1);
				th2 = thread (CPU1,&jobT2);
				th3 = thread (CPU1,&jobT3);
				th4 = thread (CPU1,&jobT4);
			}
	}

			th1.join();
			level3[0] = jobT1;
			newPCB[0].setTest(level3);

		if(newPCB.size() > 1){
			th2.join();
			level3[0] = jobT2;
			newPCB[1].setTest(level3);

			if(newPCB.size() > 2){
				th3.join();
				level3[0] = jobT3;
				newPCB[2].setTest(level3);
			}
				if(newPCB.size() > 3){
					th4.join();
					level3[0] = jobT4;
					newPCB[3].setTest(level3);
				}
		}
        return newPCB;
}

/* hold the currently running process
 * we want to save it when we try to add a new process
 * CPU should always have a process
 */


void CPU1(vector<Process> *job){
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
	job->at(0).setCurrentCycle(job->at(0).getCurrentCycle() -1);
	if(runningCPU[0].getCurrentCycle() <= 0){
		runningCPU.clear();
	}

	//job->at(0) = runningCPU[0];
}





#endif /* SCHEDULER2_H_ */
