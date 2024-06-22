# CPU Scheduling Simulation

## Introduction

A CPU scheduler is a software that schedules processes for execution with the following objectives:
- Maximize CPU utilization
- Maximize throughput
- Minimize waiting time
- Minimize turnaround time
- Minimize response time
- Ensure fairness
- Balance priorities

This project implements four standard CPU scheduling algorithms and one innovative algorithm.

## Algorithms

### 1. First Come First Serve (FCFS)

#### Definition
FCFS (First-Come, First-Served) is the simplest CPU scheduling algorithm where the process that arrives first is executed first.

#### Characteristics
- **Non-preemptive**: Processes run to completion without interruption.
- **Queue Structure**: Processes are managed in a FIFO (First-In, First-Out) queue.
- **Performance**: Can lead to high average waiting times due to the "convoy effect."
- **Context Switches**: Minimal, as each process runs to completion.
- **Use Case**: Suitable for batch systems where execution times are predictable.

#### Example
Consider three processes with burst times:
- P1: 10 ms
- P2: 5 ms
- P3: 8 ms

If processes arrive in order P1, P2, P3, the Gantt chart will be:
Average waiting time = (0 + 10 + 15) / 3 = 8.33 ms

### 2. Shortest Job First (SJF)

#### Definition
SJF (Shortest Job First) selects the process with the shortest execution time to execute next.

#### Characteristics
- **Types**:
  - **Non-preemptive**: Runs to completion without interruption.
  - **Preemptive (Shortest Remaining Time First, SRTF)**: A new process with a shorter burst time can preempt the current process.
- **Optimality**: Minimizes average waiting time.
- **Queue Structure**: Managed based on burst times.
- **Performance**: Lower average waiting time but can cause "starvation" for longer processes.
- **Context Switches**: More frequent in preemptive SJF.
- **Use Case**: Suitable where burst times can be estimated accurately.

#### Example
Consider four processes with burst times:
- P1: 4 ms
- P2: 9 ms
- P3: 5 ms
- P4: 2 ms

If processes arrive in order P1, P2, P3, P4, the Gantt chart for non-preemptive SJF will be:
Average waiting time = (0 + 2 + 6 + 11) / 4 = 4.75 ms

### 3. Round Robin (RR)

#### Definition
Round Robin (RR) is a preemptive algorithm that assigns a fixed time quantum to each process in a cyclic manner.

#### Characteristics
- **Preemptive**: Each process is interrupted when its time slice expires.
- **Queue Structure**: Managed in a circular queue.
- **Performance**: Good response time but higher average waiting time.
- **Context Switches**: Frequent if the time quantum is too short.
- **Use Case**: Suitable for time-sharing systems where responsiveness is critical.

#### Example
Consider three processes with burst times and a time quantum of 4 ms:
- P1: 12 ms
- P2: 7 ms
- P3: 10 ms
- P4: 5 ms

Average waiting time = (16 + 7 + 15 + 13) / 4 = 12.75 ms

### 4. Shortest Remaining Time First (SRTF)

#### Definition
SRTF (Shortest Remaining Time First) is a preemptive version of SJF where the process with the smallest remaining burst time is selected.

#### Characteristics
- **Preemptive**: A new process with a shorter remaining time can preempt the current process.
- **Queue Structure**: Managed based on remaining burst times.
- **Performance**: Minimizes average waiting time but can cause "starvation."
- **Context Switches**: Frequent due to preemption.
- **Use Case**: Suitable where burst times can be accurately estimated.

#### Example
Consider three processes with burst times and a time quantum of 4 ms:
- P1: 3 ms
- P2: 6 ms
- P3: 4 ms
- P4: 2 ms

Average waiting time = (0 + 2 + 5 + 7) / 4 = 3.5 ms

### 5. Innovative Algorithm

#### Definition
This innovative algorithm aims to reduce waiting and turnaround times significantly compared to other scheduling algorithms, including simple Round Robin.

#### Approach
- Executes the shortest job first.
- Uses a dynamic "Smart time quantum" based on the median burst time of all processes.

#### Stages
1. **Arrange**: Processes in increasing order of burst time.
2. **Calculate**: Median burst time; set as the Smart time quantum.
3. **Execute**: Allocate CPU based on the Smart time quantum until completion.

#### Advantages
- **Reduced Waiting Time**: Prioritizes shorter jobs and dynamically adjusts the time quantum.
- **Improved Turnaround Time**: Efficient handling of processes.
- **Eliminates Discrepancies**: Dynamic time quantum addresses issues with static time quanta.

#### Example
Consider four processes with burst times:
- P1: 6 ms
- P2: 5 ms
- P3: 8 ms
- P4: 3 ms

**Stage 1**: Arrange in order: P4, P1, P3, P2  
**Stage 2**: Calculate median = (5 + 6) / 2 = 5.5 (rounded to 6 ms)  
**Stage 3**: Allocate CPU as per the algorithm:

This innovative CPU scheduling algorithm ensures efficient process management, resulting in better overall performance metrics.

## Running the Scheduler

The code is in C++ and can be run through the terminal.
  To compile and run the code, use the following commands:
  g++ -o scheduler scheduler.cpp
  ./scheduler

## Resources

- [YouTube Playlist on CPU Scheduling]( https://www.youtube.com/watch?v=EWkQl0n0w5M&list=PLBlnK6fEyqRitWSE_AyyySWfhRgyA-rHk )
- [Documentation](https://www.kernel.org/doc/html/v4.16/process/howto.html)
- [Research on Improved Round Robin Scheduling Algorithm](https://www.researchgate.net/publication/49619229_An_Improved_Round_Robin_Schedduling_Algorithm_for_CPU_Scheduling)

