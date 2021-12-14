/*
 * Process.h
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
	int minCycle, maxCycle, actualCycle, currentCycle, memoryNeeded, resourceNeed;
	bool isCritical;
	string type;


	Process(){
		type = "CALCULATE";
		minCycle = 5;
		maxCycle = 30;
		actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
		currentCycle = actualCycle;
		isCritical = false;
		memoryNeeded = 5;
		resourceNeed = 0;

	}

	Process(string function,int minimum, int max, bool crit){
		type = function;
		minCycle = minimum;
		maxCycle = max;
		actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
		currentCycle = actualCycle;
		isCritical = crit;
		memoryNeeded = rand()% 20 + 1;
		resourceNeed = rand()% 5;


	}

	int getResource() const {
		return resourceNeed;
	}

	void setResource(int resourceNeed) {
		this->resourceNeed = resourceNeed;
	}

	int getActualCycle() const {
		return actualCycle;
	}

	void setActualCycle(int actualCycle) {
		this->actualCycle = actualCycle;
	}

	int getCurrentCycle() const {
		return currentCycle;
	}

	void setCurrentCycle(int currentCycle) {
		this->currentCycle = currentCycle;
	}

	bool isIsCritical() const {
		return isCritical;
	}

	void setIsCritical(bool isCritical) {
		this->isCritical = isCritical;
	}


	int getMaxCycle() const {
		return maxCycle;
	}

	void setMaxCycle(int maxCycle) {
		this->maxCycle = maxCycle;
	}

	int getMinCycle() const {
		return minCycle;
	}

	void setMinCycle(int minCycle) {
		this->minCycle = minCycle;
	}

	const string& getType() const {
		return type;
	}

	void setType(const string &type) {
		this->type = type;
	}

	int getMemoryNeeded() const {
		return memoryNeeded;
	}

	void setMemoryNeeded(int memoryNeeded) {
		this->memoryNeeded = memoryNeeded;
	}
};



//the PCB needs to access the Processes instead of us accessing the processes directly
class PCB: public Process{
public:
	int pid;
	int parentID;
	int priority = 0;
	unsigned int i;
	string status;
	vector<vector<Process>> test;
	float arrivalTime;
	int memoryUse = 0;
	string message;

	PCB(vector<Process> processes, int id, float time, int scheduler){
		pid = id++;
		parentID = 0; //parentID is 0 if the process is NOT a child
		test.push_back(processes);
		status = "New";
		arrivalTime = time;
		message = "This is a message! :D";

		//in this case, the higher the priority the LATER it will be executed
		if(scheduler == 2){
			for(i = 0; i < processes.size(); i++){
				priority += processes[i].getActualCycle();
				memoryUse += processes[i].getMemoryNeeded();
			}
		}
		else if(scheduler == 1){
			for(i = 0; i < processes.size(); i++){
				memoryUse += processes[i].getMemoryNeeded();
			}
			priority = 0;
		}
	}

	//getters and setters for PCB class

	string getMessage() const{
		return message;
	}

	void setMessage(string message){
		this->message = message;
	}

	int getparentID() const{
		return parentID;
	}

	void setparentID(int parentID){
		this->parentID = parentID;
	}

	int getMemoryUse() const {
		return memoryUse;
	}

	void setMemoryUse(unsigned int memoryUse) {
		this->memoryUse = memoryUse;
	}

	int getPid() const {
		return pid;
	}

	void setPid(int pid) {
		this->pid = pid;
	}

	int getPriority() const {
		return priority;
	}

	void setPriority(int priority = 0) {
		this->priority = priority;
	}

	const string& getStatus() const {
		return status;
	}

	void setStatus(const string &status) {
		this->status = status;
	}

	const vector<vector<Process> >& getTest() const {
		return test;
	}

	void setTest(const vector<vector<Process> > &test) {
		this->test = test;
	}

	float getArrivalTime() const {
		return arrivalTime;
	}

	void setArrivalTime(float arrivalTime) {
		this->arrivalTime = arrivalTime;
	}
};


//function to print all the processes of a program
void print(vector<Process> jobQueue){
	unsigned int i;
	cout
	<< left
	<< setw(14)
	<< "Operation"
	<< left
	<< setw(14)
	<< "Min Cycles"
	<< left
	<< setw(14)
	<< "Max Cycles"
	<< left
	<< setw(14)
	<< "Actual"
	<< left
	<< setw(14)
	<< "Current"
	<< left
	<< setw(14)
	<< "Is Critical?"
	<< left
	<< setw(14)
	<< "Memory Need"
	<< endl;
	for(i= 0; i < jobQueue.size(); i++){
		cout
		<< left
		<< setw(14)
		<< jobQueue[i].getType()
		<< left
		<< setw(14)
		<< jobQueue[i].getMinCycle()
		<< left
		<< setw(14)
		<< jobQueue[i].getMaxCycle()
		<< left
		<< setw(14)
		<< jobQueue[i].getActualCycle()
		<< left
		<< setw(14)
		<< jobQueue[i].getCurrentCycle()
		<< left
		<< setw(14)
		<< jobQueue[i].isIsCritical()
		<< left
		<< setw(14)
		<< jobQueue[i].getMemoryNeeded()
		<< endl;
	}
	cout << endl;
}

#endif /* PROCESS_H_ */
