#include<bits/stdc++.h>
#define SIZE 100
using namespace std;

int sysResourceA;
int sysResourceB;
int sysResourceC;
int sysMemorySize;
int sysMemoryFrameSize;
int frameNum;
void showDeviceQueue();
void bankers_algorithm();
class Process
{
private:
    int processID;
    string processName;
    string processStatus;
    int processSize;
    int hasIOoperation; // 1 means yes, 0 means no
    int burst;
    int arrivalTime;
    int priority;
    static int processCounter;
public:
    Process() {}
    /*
    Process(string processName, int burst, int arrivalTime, int priority){
        this->processID = ++(this->processCounter);
        this->processName = processName;
        this->processStatus = "new";
        this->burst = burst;
        this->arrivalTime = arrivalTime;
        this->priority = priority;
    }
    */
    void setProcess(string processName, int processSize, int hasIOoperation, int burst, int arrivalTime, int priority)
    {
        this->processID = ++(this->processCounter);
        this->processName = processName;
        this->processStatus = "new";
        this->processSize = processSize;
        this->hasIOoperation = hasIOoperation;
        this->burst = burst;
        this->arrivalTime = arrivalTime;
        this->priority = priority;
    }
    void setProcessID(int processID)
    {
        this->processID = processID;
    }
    void setProcessName(string processName)
    {
        this->processName = processName;
    }
    void setProcessStatus(string processStatus)
    {
        this->processStatus = processStatus;
    }
    void setProcessSize(int processSize)
    {
        this->processSize = processSize;
    }
    void setHasIOoperation(int hasIOoperation)
    {
        this->hasIOoperation = hasIOoperation;
    }
    void setBurst(int burst)
    {
        this->burst = burst;
    }
    void setArrivalTime(int arrivalTime)
    {
        this->arrivalTime = arrivalTime;
    }
    void setPriority(int priority)
    {
        this->priority = priority;
    }
    int getProcessID()
    {
        return this->processID;
    }
    string getProcessName()
    {
        return this->processName;
    }
    string getProcessStatus()
    {
        return this->processStatus;
    }
    int getProcessSize()
    {
        return this->processSize;
    }
    int getHasIOoperation()
    {
        return this->hasIOoperation;
    }
    int getBurst()
    {
        return this->burst;
    }
    int getArrivalTime()
    {
        return this->arrivalTime;
    }
    int getPriority()
    {
        return this->priority;
    }
};

int Process::processCounter = 0;
Process JOB_QUEUE[SIZE];
Process READY_QUEUE[SIZE];
Process DEVICE_QUEUE[SIZE];

int GANTT_CHART[1000];

int job_queue_front, job_queue_rear = 0;
int ready_queue_front, ready_queue_rear = 0;
int device_queue_front, device_queue_rear = 0;

void initGanttChart()
{
    for(int i=0; i<1000; i++)
        GANTT_CHART[i] = -1;
}

void createProcess()
{

    int pSize, pOp, pBurst, pArrivalTime, pPriority;
    string pName;
    cout << "Enter Process Name: ";
    cin >> pName;
    cout << "Enter Process Size: ";
    cin >> pSize;
    cout << "Has IO operation? (1 for yes, 0 for no): ";
    cin >> pOp;
    cout << "Enter Burst: ";
    cin >> pBurst;
    cout << "Enter Arrival Time: ";
    cin >> pArrivalTime;
    cout << "Enter Priority: ";
    cin >> pPriority;
    Process P;

    JOB_QUEUE[job_queue_rear++].setProcess(pName, pSize, pOp, pBurst, pArrivalTime, pPriority);
}
void showJobQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Job Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival"
         << setw(10) << "Priority" << setw(10) << "Size" << setw(10) << "hasIO?" <<endl;
    for(int i = 0; i < job_queue_rear; i++)
    {
        cout << setw(10) << JOB_QUEUE[i].getProcessID() << setw(10) << JOB_QUEUE[i].getProcessName()
             << setw(10) << JOB_QUEUE[i].getBurst() << setw(10) << JOB_QUEUE[i].getArrivalTime()
             << setw(10) << JOB_QUEUE[i].getPriority()
             << setw(10) << JOB_QUEUE[i].getProcessSize()
             << setw(10) << JOB_QUEUE[i].getHasIOoperation() << endl;
    }

}

void showReadyQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Ready Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << endl;
    for(int i = 0; i < ready_queue_rear; i++)
    {
        cout << setw(10) << READY_QUEUE[i].getProcessID() << setw(10) << READY_QUEUE[i].getProcessName()
             << setw(10) << READY_QUEUE[i].getBurst() << setw(10) << READY_QUEUE[i].getArrivalTime()
             << setw(10) << READY_QUEUE[i].getPriority()
             << setw(10) << READY_QUEUE[i].getProcessSize()
             << setw(10) << READY_QUEUE[i].getHasIOoperation() << endl;
    }

}

void readyProcess()
{
    showDeviceQueue();
    cout << "Transferring Processes from Job Queue to Ready Queue." << endl;
    int j = 0,k=0;
    for(int i = 0; i<job_queue_rear; i++)
    {
        if(JOB_QUEUE[i].getHasIOoperation()==1)
        {
            DEVICE_QUEUE[j].setProcessID(JOB_QUEUE[i].getProcessID());
            DEVICE_QUEUE[j].setProcessName(JOB_QUEUE[i].getProcessName());
            DEVICE_QUEUE[j].setProcessSize(JOB_QUEUE[i].getProcessSize());
            DEVICE_QUEUE[j].setHasIOoperation(JOB_QUEUE[i].getHasIOoperation());
            DEVICE_QUEUE[j].setProcessStatus("waiting");
            DEVICE_QUEUE[j].setBurst(JOB_QUEUE[i].getBurst());
            DEVICE_QUEUE[j].setArrivalTime(JOB_QUEUE[i].getArrivalTime());
            DEVICE_QUEUE[j].setPriority(JOB_QUEUE[i].getPriority());
            j++;
        }
        else
        {
            READY_QUEUE[k].setProcessID(JOB_QUEUE[i].getProcessID());
            READY_QUEUE[k].setProcessName(JOB_QUEUE[i].getProcessName());
            READY_QUEUE[k].setProcessStatus("ready");
            READY_QUEUE[k].setBurst(JOB_QUEUE[i].getBurst());
            READY_QUEUE[k].setArrivalTime(JOB_QUEUE[i].getArrivalTime());
            READY_QUEUE[k].setPriority(JOB_QUEUE[i].getPriority());
            READY_QUEUE[k].setProcessSize(JOB_QUEUE[i].getProcessSize());
            READY_QUEUE[k].setHasIOoperation(JOB_QUEUE[i].getHasIOoperation());
            k++;
        }
    }
    ready_queue_rear = k;
    device_queue_rear = j;
    job_queue_rear = 0;

    cout << "Transfer Completed." << endl;
    if(device_queue_rear > 0)
       bankers_algorithm();
}
void showDeviceQueue(){
    cout.setf(ios::left, ios::adjustfield);
    cout << "Device Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << endl;
    for(int i = 0; i < device_queue_rear; i++){
        cout << setw(10) << DEVICE_QUEUE[i].getProcessID() << setw(10) << DEVICE_QUEUE[i].getProcessName()
        << setw(10) << DEVICE_QUEUE[i].getBurst() << setw(10) << DEVICE_QUEUE[i].getArrivalTime()
        << setw(10) << DEVICE_QUEUE[i].getPriority()
        << setw(10) << DEVICE_QUEUE[i].getProcessSize()
        << setw(10) << DEVICE_QUEUE[i].getHasIOoperation() << endl;    }

}
int compare(const void *a, const void *b)
{
    if (((Process *)a)->getArrivalTime() == ((Process *)b)->getArrivalTime())
    {
        return ((Process *)a)->getProcessID() - ((Process *)b)->getProcessID();
    }
    return ((Process *)a)->getArrivalTime() - ((Process *)b)->getArrivalTime();
}

void displayGanttChart(int clock)
{
    cout.setf(ios::left, ios::adjustfield);
    for(int i=0; i<clock; i++)
    {
        if(i%10==0 && i/10>=1)
            cout << endl;
        cout << setw(3) << GANTT_CHART[i];
    }
    cout << endl;
}

double average(int *list)
{
    int sum = 0;
    for(int i = 0; i<ready_queue_rear; i++)
    {
        sum += list[i];
    }
    return sum*1.0/ready_queue_rear;
}

void fcfs()
{
    qsort(READY_QUEUE, ready_queue_rear, sizeof(READY_QUEUE[0]), compare);
    showReadyQueue();
    initGanttChart();
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        if(READY_QUEUE[i].getArrivalTime()<=clock && READY_QUEUE[i].getProcessStatus()!="Terminated")
        {
            for(int j = clock; j < clock + remaining_time[i]; j++)
            {
                GANTT_CHART[j] = READY_QUEUE[i].getProcessID();
            }

            if(seen[i]==false)
            {
                response_time[i] = clock - READY_QUEUE[i].getArrivalTime();
                seen[i]=true;
            }
            clock = clock + READY_QUEUE[i].getBurst();
            READY_QUEUE[i].setProcessStatus("Terminated");
            waiting_time[i] = clock - READY_QUEUE[i].getArrivalTime() - READY_QUEUE[i].getBurst();
            turnaround_time[i] = clock - READY_QUEUE[i].getArrivalTime();
            i++;
            complete++;
        }
        else
        {
            clock++;
        }
    }
    cout << "FCFS Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
}

void sjf()
{
    //initGanttChart();
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        // find next process to execute
        int minm = INT_MAX;
        int shortest = -1;
        //bool found = false;
        for(int i=0; i<ready_queue_rear; i++)
        {
            if(READY_QUEUE[i].getBurst() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getBurst();
                shortest = i;
            }
        }
        // no suitable process found, incrementing clock time
        if(shortest==-1)
        {
            clock++;
            continue;
        }
        /*
        for(int j = clock; j < clock + remaining_time[shortest]; j++){
            GANTT_CHART[j] = READY_QUEUE[shortest].getProcessID();
        }*/
        if(seen[shortest]==false)
        {
            response_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime();
            seen[shortest]=true;
        }
        clock = clock + READY_QUEUE[shortest].getBurst();
        READY_QUEUE[shortest].setProcessStatus("Terminated");
        waiting_time[shortest] = clock -
                                 READY_QUEUE[shortest].getArrivalTime() -
                                 READY_QUEUE[shortest].getBurst();
        turnaround_time[shortest] = clock -
                                    READY_QUEUE[shortest].getArrivalTime();
        complete++;
    }
    cout << "SJF Scheduling" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
}

void srtf()
{
    int clock = 0,complete = 0;
    bool seen[ready_queue_rear];
    int remain_time[ready_queue_rear],waiting_time[ready_queue_rear],response_time[ready_queue_rear],turn_time[ready_queue_rear];
    int complete_time[ready_queue_rear];
    for(int i = 0;i<ready_queue_rear;i++){
        seen[i] = false;
        remain_time[i] = READY_QUEUE[i].getBurst();
        waiting_time[i] = 0;

    }
    int shrtst,check;
    while(complete<ready_queue_rear){
        int mini = INT_MAX;
        shrtst  = -1;
        bool flg = false;
        int wait_time  = 0;
        for(int i = 0;i<ready_queue_rear;i++){
            if(remain_time[i] < mini && READY_QUEUE[i].getArrivalTime() <= clock
               && READY_QUEUE[i].getProcessStatus()!="Terminated"){
                mini = remain_time[i];
                shrtst = i;
            }
        }
        if(shrtst == -1){
            clock++;
            continue;
        }
          if(seen[shrtst]==false)
        {
            wait_time = clock - READY_QUEUE[shrtst].getArrivalTime();
            check = shrtst;
        }
        else
        {
            if(shrtst!=check)
            {
                wait_time = clock - complete_time[shrtst];
                check = shrtst;
            }
        }
        if(seen[shrtst]==false){
            response_time[shrtst] = clock - READY_QUEUE[shrtst].getArrivalTime();
            seen[shrtst]=true;
        }
        clock++;
        remain_time[shrtst]--;
        if(remain_time[shrtst]==0)
        {
            READY_QUEUE[shrtst].setProcessStatus("Terminated");
            turn_time[shrtst] = clock - READY_QUEUE[shrtst].getArrivalTime();
            complete++;
        }
        waiting_time[shrtst] += wait_time;
        complete_time[shrtst] = clock;
    }
    for(int i=0;i<ready_queue_rear;i++)
    {
        cout<<i<<" = "<<waiting_time[i]<<"  "<<response_time[i]<<"  "<<turn_time[i]<<endl;
    }
    cout << "Preemptive SJF Scheduling (SRTF):" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turn_time) << endl;
}


void priorityScheduling()
{
    int clock = 0;
    int i = 0;
    bool seen[ready_queue_rear];
    int remaining_time[ready_queue_rear];
    for(int i=0; i<ready_queue_rear; i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [ready_queue_rear], response_time[ready_queue_rear], turnaround_time [ready_queue_rear];

    while(complete<ready_queue_rear)
    {
        // find next process to execute
        int minm = INT_MAX;
        int highest = -1;
        //bool found = false;
        for(int i=0; i<ready_queue_rear; i++)
        {
            if(READY_QUEUE[i].getPriority() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getBurst();
                highest = i;
            }
        }
        // no suitable process found, incrementing clock time
        if(highest==-1)
        {
            clock++;
            continue;
        }
        /*
        for(int j = clock; j < clock + remaining_time[shortest]; j++){
            GANTT_CHART[j] = READY_QUEUE[shortest].getProcessID();
        }*/
        if(seen[highest]==false)
        {
            response_time[highest] = clock - READY_QUEUE[highest].getArrivalTime();
            seen[highest]=true;
        }
        clock = clock + READY_QUEUE[highest].getBurst();
        READY_QUEUE[highest].setProcessStatus("Terminated");
        waiting_time[highest] = clock -
                                READY_QUEUE[highest].getArrivalTime() -
                                READY_QUEUE[highest].getBurst();
        turnaround_time[highest] = clock -
                                   READY_QUEUE[highest].getArrivalTime();
        complete++;
    }
    cout << "SJF Scheduling" << endl;
    //displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;

}

void preemptivePriorityScheduling()
{

}
void roundRobin()
{

}

void bankers_algorithm(){
    cout << "Enter into bankers algorithm" << endl;
    int need[device_queue_rear][3],maxi[device_queue_rear][3],alloc[device_queue_rear][3],safetyseq[device_queue_rear],finish[device_queue_rear];
    int work[3] = {sysResourceA,sysResourceB,sysResourceC};
    showDeviceQueue();
    cout << "Taking Input of Max" << endl;
    for(int i = 0;i<device_queue_rear;i++){
        for(int j = 0;j<3;j++){
            cin >> maxi[i][j] ;
        }
    }
    cout << "Taking Input of Allocation" << endl;
    for(int i = 0;i<device_queue_rear;i++){
        for(int j = 0;j<3;j++){
            cin >> alloc[i][j];
        }
    }
    ///need finding
    for(int i=0;i<device_queue_rear;i++){
        for(int j = 0;j<3;j++){
             need[i][j] = maxi[i][j] - alloc[i][j];
        }
    }
     for(int i=0;i<device_queue_rear;i++){
        for(int j = 0;j<3;j++){
             cout << need[i][j] << endl;
        }
    }
    memset(finish,0,sizeof(finish));
    int p = 0,fq;
    while(p<device_queue_rear){
        bool safe = false;
        for(int i = 0;i<device_queue_rear;i++){
            if(finish[i] == 0){
                for(int j = 0;j<3;j++){
                    if(need[i][j]<work[j]){
                         work[j]+=alloc[i][j];
                         finish[i] = 1;
                         safetyseq[p++] = i;
                         //flag = true;
                    }
                    else{
                        break;
                    }
                }
            }
        }
        int incomp= 0;
        for(int k = 0;k<device_queue_rear;k++){
        if(finish[k] == 0){
            incomp++;
        }
        if(incomp == device_queue_rear){
            cout << "System is in safe state" << endl;
            cout << "Safety Sequence" << endl;
            for(int i = 0;i<device_queue_rear;i++){
                cout << DEVICE_QUEUE[safetyseq[i]].getProcessName() << " " ;
            }
            cout << endl;
            safe = true;
            break;
        }
        else if(incomp > fq)
            fq = incomp;
        else if(incomp == fq){
            cout << "system is in unsafe state" << endl;
            safe = false;
            break;
        }
      }

    }



   // cout << "Read Allocation and Max matrix for each process in device queue" << endl;
    // cout << "then determine system state" << endl;
}
void executeProcess()
{
    int choice;
    cout << "CPU Scheduling Algorithm Selection" << endl;
    cout << "-------------------" << endl;
    cout << "(1) First-Come-First-Served (FCFS)" << endl;
    cout << "(2) Shortest-Job-First (SJF)" << endl;
    cout << "(3) Preemptive SJF (shortest-remaining-time-first" << endl;
    cout << "(4) Priority-based Scheduling" << endl;
    cout << "(5) Preemptive Priority-based Scheduling" << endl;
    cout << "(6) Round-Robin Scheduling" << endl;
    cout << "Enter Choice: ";
    cin >> choice;
    switch(choice)
    {
    case 1:
        fcfs();
        break;
    case 2:
        sjf();
        break;
    case 3:
        srtf();
        break;
    case 4:
        priorityScheduling();
        break;
    case 5:
        preemptivePriorityScheduling();
        break;
    case 6:
        roundRobin();
        break;
    default:
        cout << "Unrecognized Option. " << endl;
    }
}

void reexecuteProcess()
{
    for(int i=0; i<ready_queue_rear; i++)
    {
        READY_QUEUE[i].setProcessStatus("ready");
    }
    executeProcess();
}

void sysConfig()
{
    cout << "Enter number of instances of A: ";
    cin >> sysResourceA;
    cout << "Enter number of instances of B: ";
    cin >> sysResourceB;
    cout << "Enter number of instances of C: ";
    cin >> sysResourceC;
    cout << "Enter memory size: ";
    cin >> sysMemorySize;
    cout << "Enter Frame size: ";
    cin >> sysMemoryFrameSize;
    frameNum  = sysMemorySize/sysMemoryFrameSize;
}

int main()
{
    sysConfig();
    int count = 0;
    string command;
    do
    {
        cout << "> ";
        cin >> command;
        if(command=="create-process")
        {
            count++;
            createProcess();
            cout << count << endl;
        }
        else if(command=="ready-process")
        {
            readyProcess();
        }
        else if(command=="show-job-queue")
        {
            showJobQueue();
        }
        else if(command=="show-ready-queue")
        {
            showReadyQueue();
        }
        else if(command=="execute-process")
        {
            executeProcess();
        }
        else if(command=="reexecute-process")
        {
            reexecuteProcess();
        }
        else if(command=="exit")
        {
            cout << "Thank You." << endl;
            exit(0);
        }
        else
        {
            cout << "Unrecognized Command. Try again." << endl;
        }
    }
    while(true);
}
