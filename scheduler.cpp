/* Zachary Leggett
 * Version 1.0 9/24/2020
 *
 * This program simulates a CPU scheduler, with 3 scheduler-types.
 *
 * FCFS: First Come First Serve, tasks are processed as they arrive
 *
 * SRTF (Non-Preemptive): Shortest Remaining Time First, tasks with the least remaining time
 * are processed first, with preemption
 *
 * RR (Non-Preemptive): Round Robin, tasks are processed for the time specified by the time
 * quantum before moving on to the next task and eventually wrapping back around
 * to the first task, with preemption
 *
 * Task File Format:
 * pid arrival_time burst_time
 *
 * Compiled using Makefile or
 * g++ scheduler.cpp -o scheduler
 *
 * Run using:
 * ./scheduler task_list_file [FCFS|RR|SRTF] [time_quantum]
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
using namespace std;

/*
 * Structure for tasks/processes
*/
struct Process {
    int pid; //process id
    int bt; //burst time
    int rt; //remaining time
    int st; //start time
    int art; //arrival time
    int ct; //completion time
    int wat; //waiting time
    int ta; //turnaround time
};

/*
 * Comparator for Process Structure
 * Used to sort a vector of Processes
 * in ascending order of remaining time
*/
bool srtfComp (struct Process a, struct Process b);

int main(int argc, char **argv)
{
    /*
     * The code block below parses the
     * command line arguments.
    */
    //-----------------------------------------------------------------------------------------
    if (argc < 3) {
        cout << "Usage: scheduler task_list_file [FCFS|RR|SRTF] [time_quantum]\n";
        exit(1);
    }

    string type = argv[2]; //scheduler-policy
    int typenum; //number assigned based on policy used in switch statement
    int quantum; //time quantum specified by user
    if (type == "FCFS") {
        typenum = 1;
    }
    else if (type == "RR") {
        typenum = 2;
        if (argc < 4) {
            cout << "You must specify a time quantum for RR\n";
            exit(1);
        }
        quantum = atoi(argv[3]); //convert inputted time quantum to integer
    }
    else if (type == "SRTF") {
        typenum = 3;
    }
    else {
        cout << "Invalid scheduler-policy\n";
        exit(1);
    }

    ifstream file1 (argv[1]); //input file
    if ( !file1.is_open() ) { //attempt to open input file
        cout << "Could not open specified file\n";
        exit(1);
    }
    //-----------------------------------------------------------------------------------------

    /*
     * The code block below parses the input file for integers and populates
     * the initial tasks list
    */
    //-----------------------------------------------------------------------------------------
    int nums = 0; //number of ints in file
    int count = 0; //number of processes
    vector<int> values; //vector to contain all integers parsed from input file
    int temp; //temp integer to hold parsed integers from file
    while (file1 >> temp) {
        values.push_back(temp); //load the values vector with all integers in the input file
        nums++;
    }
    count = nums/3; //number of processes
    struct Process proc[count]; //list of all processes parsed from file
    int int_cnt = 0; //int in vector
    int proc_cnt = 0; //process being populated
    while (int_cnt < nums) {
        proc[proc_cnt].pid = values[int_cnt]; //initialize pid value
        proc[proc_cnt].art = values[int_cnt + 1]; //initialize arrival time value
        proc[proc_cnt].bt = values[int_cnt + 2]; //initialize burst time value
        int_cnt = int_cnt + 3; //there are 3 integers per row, so adding 3 moves to next row of the file
        proc_cnt++; //increment to next process to populate
    }
    //-----------------------------------------------------------------------------------------

    cout << "Scheduling Policy: " << type << "\n";
    cout << "There are " << count << " tasks loaded from " << argv[1] << ". Press any key to continue ...";
    getchar();
    cout << "==================================================================\n";

    vector<struct Process> tasks; //vector of future tasks
    vector<struct Process> ready; //vector of tasks ready to be processed
    vector<struct Process> finished; //vector of finished tasks
    int time = 0; //clock counter
    int index = 0; //used by RR to keep track of which task in ready vector to be processed
    int q = quantum; //keeps track of place in time quantum for RR
    for (int w = 0; w < count; w++) {
            tasks.push_back(proc[w]); //place all tasks from intial list into future tasks vector
    }

    /*
     * The while loop below powers the time driven simulation.
    */
    //-----------------------------------------------------------------------------------------
    while (!tasks.empty() || !ready.empty()) { //loop will continue until both the future tasks and ready tasks vectors are empty
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].art <= time) { //adds tasks from future tasks to ready vector if they have arrived
                tasks[i].rt = tasks[i].bt; //set the reamining time to equal the burst time
                ready.push_back(tasks[i]); //add task to ready vector
                tasks.erase(tasks.begin() + i); //erase task loaded into the ready vector
                i--; //decrement loop counter since one element of the vector was erased
            }
            else {
                break; //no other tasks have arrived
            }
        }

        /*
         * This switch statement processes tasks in the ready vector
         * based on the sceduler policy.
        */
        //-----------------------------------------------------------------------------------------
        switch(typenum) { //process tasks in ready vector based on scheduler-policy
            case 1: //FCFS: First Come First Serve
                if (ready.empty()) {
                    cout << "<time " << time << "> idle\n"; //ready vector is empty
                }
                else {
                    if (ready[0].rt == 0) { //current task is finished
                        ready[0].ct = time; //mark completion time
                        cout << "<time " << time << "> process " << ready[0].pid << " is finished...\n";
                        finished.push_back(ready[0]); //add to finished tasks
                        ready.erase(ready.begin()); //remove from ready vector
                    }

                    if (ready[0].rt == ready[0].bt) {
                        ready[0].st = time; //mark start time
                    }

                    if (ready[0].rt > 0) {
                        ready[0].rt--; //decrement remaining time
                        cout << "<time " << time << "> process " << ready[0].pid << " is running\n";
                    }
                }
                break;
            
            case 2: //RR: Round Robin, accepts both zero or non-zero arrival time
                if (ready.empty()) {
                    cout << "<time " << time << "> idle\n"; //ready vector is empty
                }
                else {
                    if (index > (ready.size() - 1)) { //loop back to 0 if at end of ready vector
                        index = 0;
                    }

                    if (ready[index].rt == 0) { //current task is finished
                        ready[index].ct = time; //mark completion time
                        cout << "<time " << time << "> process " << ready[index].pid << " is finished...\n";
                        finished.push_back(ready[index]); //add to finished tasks
                        ready.erase(ready.begin() + index); //remove from ready vector
                        q = quantum; //reset q to inital quantum value

                        if (index > ready.size() - 1) { //loop back to 0 if at end of ready vector
                            index = 0;
                        }
                    }

                    if (ready[index].rt == ready[index].bt) {
                        ready[index].st = time; //mark start time
                    }

                    if (ready[index].rt > 0) {
                        ready[index].rt--; //decrement remaining time
                        q--; //decrement quantum time remaining
                        cout << "<time " << time << "> process " << ready[index].pid << " is running\n";
                        if (q == 0) {
                            q = quantum; //reset quantum time remaining
                            if (ready[index].rt != 0) {
                                index++; //increment index of ready vector only if the current task has time remaining
                            }
                        }
                    }
                }
                break;

            case 3: //SRTF: Shortest Remaining Time First, accepts both zero or non-zero arrival time
                if (ready.empty()) {
                    cout << "<time " << time << "> idle\n"; //ready vector is empty
                }
                else {
                    sort(ready.begin(), ready.end(), srtfComp); //sort ready vector in ascending order of time remaining
                    if (ready[0].rt == 0) { //current task is finished
                        ready[0].ct = time; //mark completion time
                        cout << "<time " << time << "> process " << ready[0].pid << " is finished...\n";
                        finished.push_back(ready[0]); //add to finished tasks
                        ready.erase(ready.begin()); //remove from ready vector
                    }
                    if (ready[0].rt == ready[0].bt) {
                        ready[0].st = time; //mark start time
                    }
                    if (ready[0].rt > 0) {
                        ready[0].rt--; //decrement remaining time
                        cout << "<time " << time << "> process " << ready[0].pid << " is running\n";
                    }
                }
                break;
        }
        //-----------------------------------------------------------------------------------------

        time++; //increment clock
    }
    //-----------------------------------------------------------------------------------------

    cout << "<time " << (time - 1) << "> All processes finish ......\n";
    
    float total_tat = 0; //total waiting time
    float total_wt = 0; //total turnaround time

    for (int i = 0; i < finished.size(); i++) {
        finished[i].ta = finished[i].ct - finished[i].art; //calculate turnaround time of task
        total_tat += finished[i].ta;
    }
    for (int i = 0; i < finished.size(); i++) {
        finished[i].wat = finished[i].ta - finished[i].bt; //calculate waiting time of task
        total_wt += finished[i].wat;
    }

    cout << "==================================================================\n";
    cout << std::fixed << std::setprecision(2);
    cout << "Average Waiting Time: " << (total_wt/count) << "\n";
    cout << "Average Turnaround Time: " << (total_tat/count) << "\n";
    cout << "==================================================================\n";

}

/*
 * Comparator for Process Structure
 * Used to sort a vector of Processes
 * in ascending order of remaining time
*/
bool srtfComp (struct Process a, struct Process b){
    return (a.rt < b.rt);
}