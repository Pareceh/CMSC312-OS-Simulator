//============================================================================
// Name        : OS.cpp
// Author      : Hana Parece
// Created On  : Oct 15 2021
// Description : OS simulator for CMSC 312
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <iomanip>
#include "Process.h"
#include "Scheduler.h"
using namespace std;


vector<PCB> cycle(vector<PCB> pcb, int *memoryInUse); //to simulate 1 cycle
int randomIO(); //to create a random I/O Event (1/100 chance per cycle)


/***************************/

//main method
int main() {
	clock_t time = clock();
	vector<Process> jobQueue, temp;
	vector<vector<Process>> getTest;
	vector<PCB> control;
	int input, saveMin, saveMax;
	unsigned int i;
	int savedCount = 0;
	int j;
	int checker = 1;
	static int count, counter2;
	string fileName = "template1"; //default to template1 just in case
	string stringIn, saveType, line;
	ifstream in;
	const unsigned int TOTAL_MEMORY = 1024;
	int memoryInUse = 0;

	cout << "Welcome to OS Simulator\n_______________________\n\nPress [1] to continue. Press [0] to exit." << endl;
	cin >> input;
	while(input == 1){
		cout << "Memory in Use: " << memoryInUse << endl;
		cout << "Memory Available: " << TOTAL_MEMORY - memoryInUse << endl;
		cout << "_______________________\n\nPress [2] to load a job\nPress [3] to display statistics\n"
				<< "Press [4] for help\nPress [5] to simulate a cycles and display no output\nPress [0] to exit." << endl << endl;

		cin >> input;

		//input == 2, so we would like to load a job/process
		if(input == 2){
			cout << "Enter template name:" << endl;
			cin >> fileName;
			fileName = "templates/" + fileName + ".txt";
			in.open(fileName);
			if (!in) {
				while(!in){
					cerr << "Unable to open file. Please check spelling." << endl;
					cout << "Enter template name:" << endl;
					cin >> fileName;
					fileName = "templates/" + fileName + ".txt";
					in.open(fileName);
				}
			}
			in.close();

			cout << "How many processes do you wish to make?" << endl;
			cin >> input;
			jobQueue.clear();

			//read the input of the file
			while(count < (input + savedCount)){
				in.open(fileName);
				while(!in.eof() && count < (input + savedCount)){
					in >> line;
					if(checker == 1){
						saveType = line;
						checker=2;
					}
					else if(checker == 2){
						saveMin = stoi(line);
						checker=3;
					}
					else{
						saveMax = stoi(line);
						checker = 1;
						Process process(saveType, saveMin, saveMax, false);
						jobQueue.push_back(process);
					}
					if(checker == 1){
						count++;
					}

				}
				in.close();
			}
			//randomly assign one of the processes to be critical, every program will have 1 critical process
			jobQueue[rand() % jobQueue.size()].setIsCritical(true);
			counter2++;
			time = clock () - time;
			control.push_back(PCB(jobQueue, counter2, (float)time/CLOCKS_PER_SEC));
			savedCount = count;
			control = cycle(control, &memoryInUse);
		}


		//input == 3, so we would like to know the current statistics of running programs
		else if(input == 3){
			if(control.size() == 0){
				cerr << "No processes running.";
			}
			for(i=0; i < control.size(); i++){
				cout  << "JOB NUMBER: " << control[i].getPid() << endl;
				cout << "PRIORITY: " << control[i].getPriority() << endl;
				cout << "STATUS: " << control[i].getStatus() << endl;
				cout << "ARRIVAL TIME: " << control[i].getArrivalTime() << endl;
				cout << "MEMORY NEEDED: " << control[i].getMemoryUse() << endl;
				if(control[i].getparentID() != 0){
					cout << "THIS CHILD'S PARENT'S ID: " << control[i].getparentID() << endl;
				}
				getTest = control[i].getTest();
				print(getTest[0]);
			}
			control = cycle(control, &memoryInUse);
		}

		//input == 4, so we would like to display the help menu
		else if(input == 4){
			cout << "Help Menu\n\nOS Simulator designed by Hana Parece for CMSC 312: VCU Fall 2021.\nThe OS Simulator is designed"
					"to simulate the creation and running of programs and processes through loading processes from 5 program templates.\n"
					"After the OS Simulator finishes 1 'action' lie loading processes or displaying an output, 1 CPU cycle is completed.\n"
					"To load processes, select [2] from the menu. The valid template names are: template1, template2, template3, template4, and template5.\n"
					"More .txt file templates can be added to the 'templates' folder to allow for additional templates. No template naming scheme is required other"
					"than the file being .txt\n"
					"To display statistics onto the console, select [3] from the menu. These statistics only show the processes currently in CPU memory.\n"
					"To display the help menu, select [4] from the menu.\n"
					"The menu selection [5] will simulate 1 cycle without displaying an input.\n"
					"To continue the program after any menu selection, the user will be prompted to select [1]. To exit the program after any menu selection,"
					"the user will be prompted to select [0].\n\n";
		}

		//input == 5, so we would like to simulate a cycle without displaying an output
		else if(input ==5){
			cout << "Simulate how many cycles?\n";
			cin >> input;
			for(j = 0; j < input; j++)
				control = cycle(control, &memoryInUse);
		}


		//input == 0, so we would like to exit the program
		// in theory we would want to free up all memory when we exit the program
		else if(input == 0){
			cout << "Closing processes..." << endl;
			control.erase(control.begin(), control.end()); //shut down all processes
			cout << "Freeing memory..." << endl;
			memoryInUse = 0; // free up memory
			break;
		}

		else{
			cerr <<"Invalid input\n";
		}


		cout << "Press [1] to continue. Press[0] to exit." << endl << endl;
		cin >> input;
		if(input > 1){
			cout << "Invalid input, closing program..." << endl;;
			cout << "Closing processes..." << endl;
			control.erase(control.begin(), control.end()); //shut down all processes
			cout << "Freeing memory..." << endl;
			memoryInUse = 0; // free up memory
		}


	}
	return 0;
}


//function to perform one cycle
// during each cycle we want there to be a random chance of an I/O event happening.

vector<PCB> cycle(vector<PCB> pcb, int *memoryInUse){
	vector<vector<Process>> level1 = pcb[0].getTest();
	vector<Process> level2 = level1[0];
	Process level3 =level2[0];
	if(randomIO() > 3){
		pcb = scheduler(pcb, &memoryInUse);
		if(level3.currentCycle == 0){
			*memoryInUse = *memoryInUse - level2[0].getMemoryNeeded();
			pcb[0].setMemoryNeeded(pcb[0].getMemoryNeeded() - level2[0].getMemoryNeeded());
			level2.erase(level2.begin());

			if(level2.size() == 0){
				pcb.erase(pcb.begin());
				level1 = pcb[0].getTest();
				level2 = level1[0];
				level3 = level2[0];
				if(pcb.size() == 0){
					cerr << "No Processes Running";
				}
			}

			level1[0] = level2;
			pcb[0].setTest(level1);
			CPU(level2);
		}
	}
	else{
		for(unsigned int i = 0; i < pcb.size(); i++){
			pcb[i].setStatus("Waiting");
		}
	}


	return pcb;
}

int randomIO(){
	static int count;
	int num;
	num = rand() % 100 + 1;
	if(num == 50){
		cout << "Random I/O Event Triggered, Waiting " << 3 - count <<  " Cycles" << endl;
		if(count == 3){
			count = 0;
			return count;
		}
		else
			count++;
		return count;
	}
	count = 50;
	return count;

}
