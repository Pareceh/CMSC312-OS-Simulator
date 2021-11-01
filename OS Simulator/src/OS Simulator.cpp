//============================================================================
// Name        : OS.cpp
// Author      : Hana Parece
// Version     :
// Copyright   : Your copyright notice
// Description : OS simulator for CMSC 312
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Process.h"
#include "Scheduler.h"
using namespace std;

vector<PCB> cycle(vector<PCB> pcb);

int main() {
	vector<Process> jobQueue, temp;
	vector<PCB> control;
	int input, saveMin, saveMax;
	unsigned int i;
	int savedCount = 0;
	int checker = 1;
	static int count, tester;
	string fileName = "template1"; //default to template1 just in case
	string stringIn, saveType, line;
	ifstream in;

	cout << "Welcome to OS Simulator\n_______________________\n\nPress [1] to continue. Press [0] to exit." << endl;
	cin >> input;
	while(input == 1){
		cout << "_______________________\n\nPress [2] to load a job\nPress [3] to display statistics\n"
				<< "Press [4] for help \nPress [0] to exit." << endl << endl;
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
				jobQueue[rand() % jobQueue.size()].isCritical = true;
				tester++;
				control.push_back(PCB(jobQueue, tester));
				control = cycle(control);
				savedCount = count;
			}


		//input == 3, so we would like to know the current statistics of running programs
		else if(input == 3){
			print(jobQueue);
			control = cycle(control);
		}

		//input == 4, so we would like to display the help menu
		else if(input == 4){
			//control = scheduler(control);
			if(control.size() == 0){
				cerr << "No processes running.";
			}
			for(i=0; i < control.size(); i++){
				cout  << "JOB NUMBER: " << control[i].pid << endl;
				cout << "PRIORITY:" << control[i].priority << endl;
				cout << "STATUS:" << control[i].status << endl;
				print(control[i].test[0]);
			}

			control = cycle(control);

		}

		//input == 0, so we would like to exit the program
		else if(input == 0)
			break;
		else
				cerr <<"Invalid input\n";

		cout << "Press [1] to continue. Press[0] to exit.\n\n";
		cin >> input;

	}
	return 0;
}


vector<PCB> cycle(vector<PCB> pcb){
		vector<vector<Process>> level1 = pcb[0].test;
		vector<Process> level2 = level1[0];
		Process level3 =level2[0];
		pcb = scheduler(pcb);

		level3 = dispatcher(pcb, level2);
		if(level3.currentCycle == 0){
			level2.erase(level2.begin());
			cout << level2.size() << " " << level1.size() << " " << pcb.size() << " " << endl;
			if(level2.size() == 0){
				pcb.erase(pcb.begin());
				level1 = pcb[0].test;
				level2 = level1[0];
				level3 = level2[0];
				if(pcb.size() == 0){
					cerr << "No Processes Running";
				}
			}
			level1[0] = level2;
			pcb[0].test = level1;
			dispatcher(pcb,level2);
			}



		return pcb;
}



