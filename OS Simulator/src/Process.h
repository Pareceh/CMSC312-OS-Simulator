/*
 * PCB.h
 *
 *  Created on: Oct 19, 2021
 *  Description: Basically everything that has to do with managing and creating processes goes in here. The PCB handles the manipulation of the processes.
 *      Author: Hana Parece
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;



class Process{
public:
	int minCycle, maxCycle, actualCycle, currentCycle;
	bool isCritical;
	string type;

	Process(){
		type = "CALCULATE";
		minCycle = 5;
		maxCycle = 30;
		actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
		currentCycle = actualCycle;
		isCritical = false;
	}

	Process(string function,int min, int max, bool crit){
		type = function;
	     minCycle = min;
	     maxCycle = max;
	     actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
	     currentCycle = actualCycle;
	     isCritical = crit;
	   }

};



//the PCB needs to access the Processes instead of us accessing the processes directly
class PCB: public Process{
public:
	int pid;
	int priority = 0;
	unsigned int i;
	string status;
	vector<vector<Process>> test;

	PCB(vector<Process> processes, int id){
		pid = id++;
		test.push_back(processes);
		status = "New";

		//in this case, the higher the priority the LATER it will be executed
		for(i = 0; i < processes.size(); i++)
			priority += processes[i].actualCycle;
	}

};


vector<vector<Process>> createQueue(vector<PCB> pcb){
	vector<vector<Process>> result;
	vector<Process> smaller;
	unsigned int i;
	for(i = 0; i < pcb.size(); i ++)
		result = pcb[i].test;
	for(i=0; i < result.size(); i ++)
		smaller = result[i];
	if(smaller[0].type == "I/O")
		pcb[0].status = "Waiting";
	else
		pcb[0].status = "Running";
	for(i = 0;pcb.size(); i ++)
		pcb[i].status = "Ready";

	return result;
}


//function to print all the processes of a program
void print(vector<Process> jobQueue){
	unsigned int i;
	cout
						<< left
						<< setw(12)
						<< "Operation"
						<< left
						<< setw(12)
						<< "Min Cycles"
						<< left
						<< setw(12)
						<< "Max Cycles"
						<< left
						<< setw(12)
						<< "Actual Cycles"
						<< left
						<< setw(12)
						<< "Current Cycles"
						<< left
						<< setw(12)
						<< "IsCritical?"
						<< endl;
				for(i=0; i < jobQueue.size(); i++){
							cout

									<< left
									<< setw(12)
									<< jobQueue[i].type
									<< left
									<< setw(12)
									<< jobQueue[i].minCycle
									<< left
									<< setw(12)
									<< jobQueue[i].maxCycle
									<< left
									<< setw(12)
									<< jobQueue[i].actualCycle
									<< left
									<< setw(12)
									<< jobQueue[i].currentCycle
									<< left
									<< setw(12)
									<< jobQueue[i].isCritical
									<< endl;
				}
				cout << endl;
}


#endif /* PROCESS_H_ */
