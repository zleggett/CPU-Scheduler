# CPU-Scheduler
A simulated CPU scheduler written in C++

## Supported Scheduler Types
**FCFS**: First Come First Serve, tasks are processed as they arrive
 
**SRTF**: Shortest Remaining Time First, tasks with the least remaining time are processed first, with preemption
 
**RR**: Round Robin, tasks are processed for the time specified by the time quantum before moving on to the next task and eventually wrapping back around to the first task, with preemption

## Task File
The scheduler accepts a task file which contains the tasks to be scheduled.

Task files should use the following format:\
pid arrival_time burst_time

An example task file is provided in the repo.

## Usage
Can be compiled using the provided make file or by running ```g++ scheduler.cpp -o scheduler```

Run using ```./scheduler task_list_file [FCFS|RR|SRTF] [time_quantum]```

## Output
The scheduler will show the running process at each time step and indicate when a process is finished. It will also calculate the average waiting and turnaround time for the given set of tasks.

An example output can be seen below.
```
Scheduling Policy: FCFS
There are 6 tasks loaded from task.list. Press any key to continue ...
==================================================================
<time 0> process 1 is running
<time 1> process 1 is running
<time 2> process 1 is running
<time 3> process 1 is running
<time 4> process 1 is running
<time 5> process 1 is running
<time 6> process 1 is running
<time 7> process 1 is running
<time 8> process 1 is running
<time 9> process 1 is running
<time 10> process 1 is finished...
<time 10> process 2 is running
<time 11> process 2 is running
<time 12> process 2 is running
<time 13> process 2 is running
<time 14> process 2 is running
<time 15> process 2 is running
<time 16> process 2 is running
<time 17> process 2 is running
<time 18> process 2 is running
<time 19> process 2 is finished...
<time 19> process 3 is running
<time 20> process 3 is running
<time 21> process 3 is running
<time 22> process 3 is running
<time 23> process 3 is running
<time 24> process 3 is finished...
<time 24> process 4 is running
<time 25> process 4 is running
<time 26> process 4 is running
<time 27> process 4 is running
<time 28> process 4 is finished...
<time 28> process 5 is running
<time 29> process 5 is running
<time 30> process 5 is running
<time 31> process 5 is running
<time 32> process 5 is running
<time 33> process 5 is running
<time 34> process 5 is finished...
<time 34> process 6 is running
<time 35> process 6 is running
<time 36> process 6 is running
<time 37> process 6 is running
<time 38> process 6 is running
<time 39> process 6 is running
<time 40> process 6 is running
<time 41> process 6 is finished...
<time 41> All processes finish ......
==================================================================
Average Waiting Time: 14.17
Average Turnaround Time: 21.00
==================================================================
```
