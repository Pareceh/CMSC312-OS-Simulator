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

vector<Process> cycle(vector<Process> jobQueue);


int main() {
	vector<Process> jobQueue, temp;
	int input,i, saveMin, saveMax;
	int savedCount = 0;
	int checker = 1;
	static int count;
	string fileName = "template1";
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
								Process process(saveType,(count +1), saveMin, saveMax);
								jobQueue.push_back(process);
							}
							if(checker == 1){
								count++;
								jobQueue = cycle(jobQueue);
							}

					}
					in.close();
				}
				savedCount = count;
			}


		//input == 3, so we would like to know the current statistics of running programs
		else if(input == 3){
			cout
					<< left
					<<setw(12)
					<<"Process ID"
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
					<< endl;
			for(i=0; i < jobQueue.size(); i++){
						cout
								<< left
								<<setw(12)
								<< jobQueue[i].getID()
								<< left
								<< setw(12)
								<< jobQueue[i].getType()
								<< left
								<< setw(12)
								<< jobQueue[i].getMinCycle()
								<< left
								<< setw(12)
								<< jobQueue[i].getMaxCycle()
								<< left
								<< setw(12)
								<< jobQueue[i].getActualCycle()
								<< left
								<< setw(12)
								<< jobQueue[i].currentCycle
								<< endl;
			}


			jobQueue = cycle(jobQueue);
		}

		//input == 4, so we would like to display the help menu
		else if(input == 4){
			jobQueue = cycle(jobQueue);

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


vector<Process> cycle(vector<Process> jobQueue){ //a cycle is completed whenever the CPU finishes loading a process OR a menu is displayed
	vector<Process> temp;
	jobQueue = scheduler(jobQueue);
								temp = dispatcher(jobQueue[0]);
									if(temp.size() ==0){
										jobQueue.erase(jobQueue.begin());
										dispatcher(jobQueue[0]);
									}
									else{
										jobQueue[0] = temp[0];
									}

								jobQueue = scheduler(jobQueue);
								return jobQueue;
}
