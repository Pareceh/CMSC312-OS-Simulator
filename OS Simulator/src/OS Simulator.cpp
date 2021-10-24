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
#include "Scheduler.h"
#include "PCB.h"
using namespace std;


int main() {
	PCB process[10];
	int input,i;
	int input2 = 1;
	ifstream in;
	string stringIn;
	string   line;
	static int count;
	int checker = 1;


	cout << "Welcome to OS Simulator\n_______________________\n\n Press [1] to continue. Press [0] to exit.\n\n";
	cin >> input;
	while(input == 1){
		cout << "_______________________\n\n Press [2] to load a job \n Press [3] to display statistics \n "
				"Press [4] for help \n Press [0] to exit.\n\n ";
		cin >> input;

		//input == 2, so we would like to load a job/process
		if(input ==2){
			cout << "Create a process [1] or load a process [2]?\n\n";
			cin >> input;

			if(input ==1){
				while(input2 == 1){
					cout << "Enter 1 for CALCULATE, enter 2 for I/O or enter 3 for FORK\n";
					cin >> input;
					if(input == 1)
						process[count].type = "CALCULATE";
					else if(input == 2)
						process[count].type = "I/O";
					else
						process[count].type = "FORK";
					cout << "Enter min cycles:\n";
					cin >> input;
					process[count].minCycle = input;
					cout << "\nEnter max cycles:\n";
					cin >> input;
					process[count].maxCycle = input;
					process[count].pid = count +1;
					count++;
					cout << "Create another process? [1] Yes ~~ [2] No\n";
					cin >> input2;
				}
			}
			else if(input == 2){
				in.open("src/template1.txt");
				if (!in) {
				    cerr << "Unable to open file template1.txt";
				    exit(1);   // call system to stop
				}
				while(!in.eof()){

					//getline(in, line,',');
					in >> line;
					cout << line << '\n';
					if(checker == 1){
						process[count].type = line;
						checker=2;
					}
					else if(checker == 2){
						process[count].minCycle = stoi(line);
						checker=3;
					}
					else{
						process[count].maxCycle = stoi(line);
						checker = 1;
					}

					process[count].pid = count + 1;
					if(checker == 1)
						count++;

				}
				cout << "COUNT IS:" << count << "\n";
				in.close();

			}

		}

		//input == 3, so we would like to know the current statistics of running programs
		if(input == 3){
			cout << "Process ID ___ Operation ___ Min Cycles ____  Max Cycles____  Actual Cycles\n";
			for(i=0; i < count; i ++){
						cout << process[i].getID() << " ___________ "
						<< process[i].getType() << " ___________ "
						<< process[i].getMinCycle() << " ___________ "
						<< process[i].getMaxCycle() << " ___________ "
						<< process[i].getActualCycle() << "\n";
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
	}
	return 0;
}


