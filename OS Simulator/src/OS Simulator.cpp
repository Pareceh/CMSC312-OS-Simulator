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
#include "Scheduler.h"
#include "PCB.h"
using namespace std;


int main() {
	vector<PCB> processes;
	int input,i, saveMin, saveMax;
	int input2 =1;
	int checker = 1;
	static int count, cycle;
	string fileName = "template1";
	string stringIn, saveType, line;
	ifstream in;

	cout << "Welcome to OS Simulator\n_______________________\n\n Press [1] to continue. Press [0] to exit.\n\n";
	cin >> input;
	while(input == 1){
		cout << "_______________________\n\n Press [2] to load a job \n Press [3] to display statistics \n "
				"Press [4] for help \n Press [0] to exit.\n\n ";
		cin >> input;

		//input == 2, so we would like to load a job/process
		if(input == 2){
			cout << "Create a process [1] or load a process [2]?\n";
			cin >> input;

			if(input == 1){
				while(input2 == 1){
					cout << "Enter 1 for CALCULATE, enter 2 for I/O or enter 3 for FORK\n";
					cin >> input;
					if(input == 1)
						saveType = "CALCULATE";
					else if(input == 2)
						saveType = "I/O";
					else
						saveType = "FORK";

					cout << "Enter min cycles:\n";
					cin >> input;
					saveMin = input;
					cout << "\nEnter max cycles:\n";
					cin >> input;
					saveMax = input;

					PCB process(saveType,(count +1), saveMin, saveMax);
					processes.push_back(process);

					count++;
					cout << "Create another process? [1] Yes ~~ [2] No\n";
					cin >> input2;
				}
			}
			else if(input == 2){
				cout << "Enter template name:\n";
				cin >> fileName;
				fileName = "templates/" + fileName + ".txt";
				in.open(fileName);
				if (!in) {
					while(!in){
						cerr << "Unable to open file. Please check spelling. \n";
						cout << "Enter template name:\n";
						cin >> fileName;
						fileName = "templates/" + fileName + ".txt";
						in.open(fileName);
					}
				}

				while(!in.eof()){
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
							PCB process(saveType,(count +1), saveMin, saveMax);
							processes.push_back(process);
						}
						if(checker == 1){
							count++;
							processes = scheduler(processes);
						}
				}
				cout << "COUNT IS:" << count << "\n";
				in.close();

			}

		}

		//input == 3, so we would like to know the current statistics of running programs
		if(input == 3){
			cout << "Process ID ___ Operation ___ Min Cycles ____  Max Cycles____  Actual Cycles\n";
			for(i=0; i < processes.size(); i++){
						cout << processes[i].getID() << " ___________ "
						<< processes[i].getType() << " ___________ "
						<< processes[i].getMinCycle() << " ___________ "
						<< processes[i].getMaxCycle() << " ___________ "
						<< processes[i].getActualCycle() << "\n";
			}


		}

		//input == 4, so we would like to display the help menu
		if(input == 4){
		}
		//input == 0, so we would like to exit the program
		if(input == 0)
			break;

		cout << "Press [1] to continue. Press[0] to exit.\n\n";
		cin >> input;
		cycle++;
	}
	return 0;
}
