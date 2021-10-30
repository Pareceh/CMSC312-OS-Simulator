/*
 * PCB.h
 *
 *  Created on: Oct 19, 2021
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
	int pid,minCycle, maxCycle, actualCycle, currentCycle;
	string type;

	Process(){
		type = "CALCULATE";
		pid = 0;
		minCycle = 5;
		maxCycle = 30;
		actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
		currentCycle = actualCycle;
	}

	Process(string function,int id,int min, int max){
		type = function;
	     pid = id;
	     minCycle = min;
	     maxCycle = max;
	     actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
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


class PCB: public Process{

};


#endif /* PROCESS_H_ */