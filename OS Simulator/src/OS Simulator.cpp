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
	int j;
	int checker = 1;
	static int count, tester;
	string fileName = "template1"; //default to template1 just in case
	string stringIn, saveType, line;
	ifstream in;

	cout << "Welcome to OS Simulator\n_______________________\n\nPress [1] to continue. Press [0] to exit." << endl;
	cin >> input;
	while(input == 1){
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
				//randomly assign one of the processes to be critical
				jobQueue[rand() % jobQueue.size()].isCritical = true;
				tester++;
				control.push_back(PCB(jobQueue, tester));
				control = cycle(control); //do one cycle
				savedCount = count;
			}


		//input == 3, so we would like to know the current statistics of running programs
		else if(input == 3){
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

			readyQueue(control);

		}

		//input == 5, so we would like to simulate a cycle without displaying an output
		else if(input ==5){
			cout << "Simulate how many cycles?\n";
			cin >> input;
			for(j = 0; j < input; j++)
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


//function to perform one
vector<PCB> cycle(vector<PCB> pcb){
		vector<vector<Process>> level1 = pcb[0].test;
		vector<Process> level2 = level1[0];
		Process level3 =level2[0];
		pcb = scheduler(pcb);

		level3 = dispatcher(pcb, level2);
		if(level3.currentCycle == 0){
			level2.erase(level2.begin());
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



