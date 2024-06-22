#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// Arrays to store waiting time, burst time, arrival time, turn around time, and process IDs
int wt[100], bt[100], at[100], tat[100], p[100];
// Number of processes
int n;
// Arrays to store average waiting time, average turn around time, and CPU idle times for different algorithms
float awt[5], atat[5], idle[5];

// Function to take input for number of processes and their burst and arrival times
void inputProcesses() {
    cout << "Enter Number of processes: ";
    cin >> n;

    vector<pair<pair<int, int>, int>> processes(n); // Vector to store (arrival time, burst time, process ID)

    for (int i = 0; i < n; i++) {
        p[i] = i + 1;
        cout << "Enter Burst Time of process " << p[i] << ": ";
        cin >> bt[i];
        cout << "Enter Arrival Time of process " << p[i] << ": ";
        cin >> at[i];

        processes[i] = {{at[i], bt[i]}, p[i]};
    }

    sort(processes.begin(), processes.end()); // Sort processes by arrival time, then by burst time

    for (int i = 0; i < n; i++) {
        p[i] = processes[i].second;
        at[i] = processes[i].first.first;
        bt[i] = processes[i].first.second;
    }

    fill(awt, awt + 5, 0.0);
    fill(atat, atat + 5, 0.0);
    fill(idle, idle + 5, 0.0);
}

// First-Come, First-Served (FCFS) scheduling algorithm
void fcfs() {
    int currentTime = at[0] + bt[0];
    idle[0] = at[0];
    wt[0] = 0;
    tat[0] = bt[0];

    for (int i = 1; i < n; i++) {
        if (at[i] <= currentTime) {
            wt[i] = currentTime - at[i];
            currentTime += bt[i];
        } else {
            idle[0] += at[i] - currentTime;
            currentTime = at[i] + bt[i];
            wt[i] = 0;
        }
        tat[i] = wt[i] + bt[i];
        awt[0] += wt[i];
        atat[0] += tat[i];
    }

    awt[0] /= n;
    atat[0] /= n;

    cout << "SR.\tA.T.\tB.T.\tW.T.\tT.A.T.\n";
    for (int i = 0; i < n; i++) {
        cout << p[i] << '\t' << at[i] << '\t' << bt[i] << '\t' << wt[i] << '\t' << tat[i] << '\n';
    }
}

// Function to calculate the median of burst times of remaining processes
int medianBurstTime(const vector<int>& remainingBt) {
    vector<int> validBt;
    for (int bt : remainingBt) {
        if (bt > 0) validBt.push_back(bt);
    }
    sort(validBt.begin(), validBt.end());
    int size = validBt.size();
    if (size % 2 == 0) {
        return (validBt[size / 2 - 1] + validBt[size / 2]) / 2;
    }
    return validBt[size / 2];
}

// Custom innovative scheduling algorithm
void innovativeScheduling() {
    int count = 0, currentTime = 0;
    int waitingTime = 0, turnaroundTime = 0;
    vector<int> remainingBt(n);

    for (int i = 0; i < n; i++) {
        remainingBt[i] = bt[i];
    }

    cout << "\nProcess ID\tArrival Time\tBurst Time\tTurnaround Time\tWaiting Time\n";

    while (count != n) {
        int minBt = INT_MAX;
        bool isIdle = true;
        int nextProcess = -1;

        for (int i = 0; i < n; i++) {
            if (at[i] <= currentTime && remainingBt[i] > 0) {
                isIdle = false;
                if (remainingBt[i] < minBt) {
                    minBt = remainingBt[i];
                    nextProcess = i;
                }
            }
        }

        if (isIdle) {
            int nextArrival = *min_element(at, at + n);
            idle[4] += nextArrival - currentTime;
            currentTime = nextArrival;
            continue;
        }

        int timeQuantum = medianBurstTime(remainingBt);
        if (remainingBt[nextProcess] <= timeQuantum) {
            currentTime += remainingBt[nextProcess];
            remainingBt[nextProcess] = 0;
            count++;
            waitingTime += currentTime - at[nextProcess] - bt[nextProcess];
            turnaroundTime += currentTime - at[nextProcess];
            cout << "Process[" << p[nextProcess] << "]\t" << at[nextProcess] << '\t' << bt[nextProcess] << '\t'
                 << currentTime - at[nextProcess] << '\t' << currentTime - at[nextProcess] - bt[nextProcess] << '\n';
        } else {
            remainingBt[nextProcess] -= timeQuantum;
            currentTime += timeQuantum;
        }
    }

    awt[4] = (float)waitingTime / n;
    atat[4] = (float)turnaroundTime / n;

    for (int i = 0; i < n; i++) {
        bt[i] = remainingBt[i];
    }
}

// Round Robin (RR) scheduling algorithm
void roundRobin() {
    int total = 0, remainingProcesses = n, timeQuantum;
    int waitingTime = 0, turnaroundTime = 0;
    vector<int> remainingBt(bt, bt + n);

    cout << "\nEnter Time Quantum: ";
    cin >> timeQuantum;

    cout << "\nProcess ID\tArrival Time\tBurst Time\tTurnaround Time\tWaiting Time\n";

    for (int i = 0; remainingProcesses != 0;) {
        if (at[i] <= total) {
            if (remainingBt[i] <= timeQuantum && remainingBt[i] > 0) {
                total += remainingBt[i];
                remainingBt[i] = 0;
                remainingProcesses--;
                int waitTime = total - at[i] - bt[i];
                int tatTime = total - at[i];
                cout << "Process[" << p[i] << "]\t" << at[i] << '\t' << bt[i] << '\t' << tatTime << '\t' << waitTime << '\n';
                waitingTime += waitTime;
                turnaroundTime += tatTime;
            } else if (remainingBt[i] > 0) {
                remainingBt[i] -= timeQuantum;
                total += timeQuantum;
            }
        } else {
            idle[2] += at[i] - total;
            total = at[i];
        }

        i = (i + 1) % n;
    }

    awt[2] = (float)waitingTime / n;
    atat[2] = (float)turnaroundTime / n;
}

// Shortest Remaining Time First (SRTF) scheduling algorithm
void srtf() {
    int count = 0, currentTime = 0;
    int smallest = -1;
    double avgWait = 0, avgTat = 0;
    vector<int> remainingBt(bt, bt + n);

    cout << "\nProcess ID\tArrival Time\tBurst Time\tTurnaround Time\tWaiting Time\n";

    while (count != n) {
        int minBt = INT_MAX;
        bool isIdle = true;

        for (int i = 0; i < n; i++) {
            if (at[i] <= currentTime && remainingBt[i] > 0) {
                isIdle = false;
                if (remainingBt[i] < minBt) {
                    minBt = remainingBt[i];
                    smallest = i;
                }
            }
        }

        if (isIdle) {
            int nextArrival = *min_element(at, at + n);
            idle[3] += nextArrival - currentTime;
            currentTime = nextArrival;
            continue;
        }

        remainingBt[smallest]--;
        if (remainingBt[smallest] == 0) {
            count++;
            double endTime = currentTime + 1;
            avgWait += endTime - at[smallest] - bt[smallest];
            avgTat += endTime - at[smallest];
            cout << "Process[" << p[smallest] << "]\t" << at[smallest] << '\t' << bt[smallest] << '\t'
                 << endTime - at[smallest] << '\t' << endTime - at[smallest] - bt[smallest] << '\n';
        }
        currentTime++;
    }

    awt[3] = avgWait / n;
    atat[3] = avgTat / n;

    for (int i = 0; i < n; i++) {
        bt[i] = remainingBt[i];
    }
}

// Function to display the results of the selected scheduling algorithm
void displayResults(int choice) {
    cout << "Average Waiting Time: " << awt[choice - 1] << "\n";
    cout << "Average Turn Around Time: " << atat[choice - 1] << "\n";
    cout << "CPU Idleness Time: " << idle[choice - 1] << "\n";
}

int main() {
    cout << "Welcome to CPU Scheduling Simulation:\n\n";
    inputProcesses();
    int choice, continueChoice;

    cout << "Choice\tAlgorithm\n1\tFCFS\n2\tSJF\n3\tRound Robin\n4\tSRTF\n5\tInnovative\n";
    do {
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                fcfs();
                break;
            case 2:
                // Shortest Job First (SJF) is not implemented separately as it is covered by SRTF
                cout << "SJF algorithm is not implemented separately in this program.\n";
                break;
            case 3:
                roundRobin();
                break;
            case 4:
                srtf();
                break;
            case 5:
                innovativeScheduling();
                break;
            default:
                cout << "Invalid choice. Please select an option between 1 and 5.\n";
                break;
        }
        displayResults(choice);

        cout << "\nEnter 1 to continue, 0 to stop: ";
        cin >> continueChoice;
    } while (continueChoice == 1);

    return 0;
}
