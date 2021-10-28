/*
 * PCB.h
 *
 *  Created on: Oct 19, 2021
 *      Author: Hana Parece
 */

#ifndef PCB_H_
#define PCB_H_

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

class PCB{
public:
	int pid,minCycle, maxCycle, actualCycle, criticalSection, currentCycle;
	string type;

	PCB(){
		type = "CALCULATE";
		pid = 0;
		minCycle = 5;
		maxCycle = 30;
		actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
		criticalSection = rand()% actualCycle + 1;
		currentCycle = actualCycle;
	}

	PCB(string function,int id,int min, int max){
		type = function;
	     pid = id;
	     minCycle = min;
	     maxCycle = max;
	     actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
	     criticalSection = rand()% actualCycle + 1;
	     currentCycle = actualCycle;
	   }

	string getType(){
		return type;
	}

	int getID(){
		return pid;
	}

	int getMinCycle(){
		return minCycle;
	}

	int getMaxCycle(){
		return maxCycle;
	}

	int getActualCycle(){
		return actualCycle;
	}
};

bool inCritical(PCB process){ //returns true if the process is in it's critical section
	return (process.currentCycle == process.criticalSection);
}


#endif /* PCB_H_ */
