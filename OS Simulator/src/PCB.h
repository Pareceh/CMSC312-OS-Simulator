/*
 * PCB.h
 *
 *  Created on: Oct 19, 2021
 *      Author: mpare
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
	int pid,minCycle, maxCycle, actualCycle;
	string type;

	PCB(){
		type = "CALCULATE";
		pid = 0;
		minCycle = 5;
		maxCycle = 30;
		actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
	}

	PCB(string function,int id,int min, int max){
		type = function;
	     pid = id;
	     minCycle = min;
	     maxCycle = max;
	     actualCycle = rand()%(maxCycle-minCycle +1) + minCycle;
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

#endif /* PCB_H_ */
