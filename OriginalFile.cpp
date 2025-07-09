#include<bits/stdc++.h>
#include "parser.c++"
using namespace std;





const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[8] = {"", "FCFS", "RR-", "SPN", "SRT", "AGING", "FB-1", "FB-2i"};

void fillInWaitTime(){
    for (int i = 0; i < noOfProcess; i++)
    {
        int arrival = arrivalTime[i];
        for (int k = arrival; k < completionTime[i]; k++)
        {
            if (timeline[k][i] != '*')
                timeline[k][i] = '.';
        }
    }
}

void clear_timeline(){
    for(int i=0; i<lastInstant; i++)
        for(int j=0; j<noOfProcess; j++)
            timeline[i][j] = ' ';
}


 
void firstComeFirstServe(){

    int n = arrivalTime.size();
        
    vector<pair<int,int>>v(n);
    for(int i=0;i<n;i++){
        v[i] = {arrivalTime[i],i};
    }
    
    // cout<<2;
    sort(v.begin(),v.end());
    int time = 0;

    for(int i=0;i<n;i++){
        int at = v[i].first;

        if(time < at){
            time = at;
        }
        int process = v[i].second;

        completionTime[process] = time + burstTime[process];
        turnAroundTime[i] = completionTime[i] - arrivalTime[i];
        waitingTime[i] = turnAroundTime[i] - burstTime[i];

        for (int j = time; j < completionTime[process]; j++)
            timeline[j][process] = '*';
        for (int j = at; j < time; j++)
            timeline[j][process] = '.';

        time += burstTime[process];
    }

    // responseTime = waitingTime;
    

    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"ResponseTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<responseTime[i]<<" ";
    // }
    // cout<<endl;
}


void roundRobin(int timeQuantum){

    queue<pair<int,int>>q;
    int quantum = timeQuantum;
    int j = 0;
    int n = noOfProcess;

    for(int i=0;i<=lastInstant;i++){
        while(j<n && arrivalTime[j] == i){
            q.push({j,burstTime[j]});
            j++;
        }
        
        if(!q.empty()){
            int process = q.front().first;
            q.front().second = q.front().second - 1;
            int bt = q.front().second;
            

            int remain = bt;
            quantum--;
            timeline[i][process]='*';

            while(j<n && arrivalTime[j] == i+1){
                q.push({j,burstTime[j]});
                j++;
            }

            if(remain == 0 && quantum == 0){
                completionTime[process] = i+1; 
                turnAroundTime[process] = completionTime[process] - arrivalTime[process];
                waitingTime[process] = turnAroundTime[process] - burstTime[process];
                quantum = timeQuantum;
                q.pop();
            }
            else if(remain != 0 && quantum == 0){
                q.pop();
                q.push({process,remain});
                quantum = timeQuantum;
            }
            else if(remain == 0 && quantum != 0){
                completionTime[process] = i+1; 
                turnAroundTime[process] = completionTime[process] - arrivalTime[process];
                waitingTime[process] = turnAroundTime[process] - burstTime[process];
                quantum = timeQuantum;
                q.pop();
            }
        }

    }
    fillInWaitTime();
    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;
}

void shortestJobFirst(){

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    int n = arrivalTime.size();
    
    int j = 0;
    for(int i=0;i<lastInstant;i++){

        while(j<n && arrivalTime[j] <= i){
            pq.push({burstTime[j],j});
            j++;
        }

        int a = i;
        if(!pq.empty()){
            int bt = pq.top().first;
            int process = pq.top().second;
            pq.pop();

            int temp = arrivalTime[process];
            for (; temp < i; temp++)
                timeline[temp][process] = '.';

            temp = i;
            for (; temp < i + bt; temp++)
                timeline[temp][process] = '*';

            completionTime[process] = a + burstTime[process];
            turnAroundTime[process] = completionTime[process] - arrivalTime[process];
            waitingTime[process] = turnAroundTime[process] - burstTime[process];

            a += burstTime[process];
        }

        if(a != i){
            i = a-1;
        }

    }

    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"ResponseTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<responseTime[i]<<" ";
    // }
    // cout<<endl;
}



void shortestRemainingTime(){

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    int n = arrivalTime.size();
    
    int j = 0;
    for(int i=0;i<=lastInstant;i++){

        while(j<n && arrivalTime[j] <= i){
            pq.push({burstTime[j],j});
            j++;
        }

        if(!pq.empty()){
            int bt = pq.top().first;
            int process = pq.top().second;
            pq.pop();

            timeline[i][process] = '*';
            
            if(bt == 1){
                completionTime[process] = i + 1;
                turnAroundTime[process] = completionTime[process] - arrivalTime[process];
                waitingTime[process] = turnAroundTime[process] - burstTime[process];
            }
            else{
                pq.push({bt-1,process});
            }

        }

    }

    fillInWaitTime();

    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;
}


void feedbackQ1() {

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    int n = arrivalTime.size();
    unordered_map<int,int>remainingServiceTime;

    int j = 0;
    if(arrivalTime[0] == 0){
        pq.push({0,j});
        remainingServiceTime[j] = burstTime[j];
        j++;
    }

    for(int i=0;i<=lastInstant;i++){
        if(!pq.empty()){
            int process = pq.top().second;
            int priority_level = pq.top().first;
            pq.pop();

            while(j<noOfProcess && arrivalTime[j] <= i+1){
                pq.push({0,j});
                remainingServiceTime[j] = burstTime[j];
                j++;
            }

            remainingServiceTime[process]--;
            timeline[i][process]='*';

            if(remainingServiceTime[process] == 0){
                completionTime[process] = i+1;
                turnAroundTime[process] = completionTime[process] - arrivalTime[process];
                waitingTime[process] = turnAroundTime[process] - burstTime[process];
            }
            else{
                if(pq.size() >= 1)
                pq.push({priority_level+1,process});
                else pq.push({priority_level,process});
            }
        }
        while(j<noOfProcess && arrivalTime[j] <= i+1){
            pq.push({0,j});
            remainingServiceTime[j] = burstTime[j];
            j++;
        }
    }

    fillInWaitTime();
    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;
}





void feedbackQ2() {

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
    int n = arrivalTime.size();
    unordered_map<int,int>remainingServiceTime;

    int j = 0;
    if(arrivalTime[0] == 0){
        pq.push({0,j});
        remainingServiceTime[j] = burstTime[j];
        j++;
    }

    for(int i=0;i<=lastInstant;i++){
        if(!pq.empty()){
            int process = pq.top().second;
            int priority_level = pq.top().first;
            pq.pop();

            while(j<noOfProcess && arrivalTime[j] <= i+1){
                pq.push({0,j});
                remainingServiceTime[j] = burstTime[j];
                j++;
            }

            int currentQuantum = pow(2,priority_level);
            int temp = i;
            while(currentQuantum && remainingServiceTime[process]){
                currentQuantum--;
                remainingServiceTime[process]--;
                timeline[temp++][process]='*';
            }

            if(remainingServiceTime[process] == 0){
                completionTime[process] = temp;
                turnAroundTime[process] = completionTime[process] - arrivalTime[process];
                waitingTime[process] = turnAroundTime[process] - burstTime[process];
            }
            else{
                if(pq.size() >= 1)
                pq.push({priority_level+1,process});
                else pq.push({priority_level,process});
            }
            i = temp - 1;
        }
        while(j<noOfProcess && arrivalTime[j] <= i+1){
            pq.push({0,j});
            remainingServiceTime[j] = burstTime[j];
            j++;
        }
    }

    fillInWaitTime();

    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;
}



void aging(int quantum) {

    
    vector<tuple<int,int,int>>v; //tuple of priority level, process index and total waiting time
    int j=0,currentProcess=-1;
    int n = noOfProcess;
    for(int time =0;time<lastInstant;time++){
        while(j<noOfProcess && arrivalTime[j]<=time){
            v.push_back(make_tuple(burstTime[j],j,0));
            j++;
        }

        for(int i=0;i<v.size();i++){
            if(get<1>(v[i])==currentProcess){
                get<2>(v[i])=0;
                get<0>(v[i])=burstTime[currentProcess];
            }
            else{
                get<0>(v[i])++;
                get<2>(v[i])++;
            }
        }
        sort(v.begin(),v.end(),[&](const tuple<int,int,int>&a,const tuple<int,int,int>&b){
            if(get<0>(a)==get<0>(b))
                return get<2>(a)> get<2>(b);
            return get<0>(a) > get<0>(b);
        });
        currentProcess=get<1>(v[0]);
        int currentQuantum = quantum;
        while(currentQuantum-- && time<lastInstant){
            timeline[time][currentProcess]='*';
            completionTime[currentProcess] = time;
            turnAroundTime[currentProcess] = completionTime[currentProcess] - arrivalTime[currentProcess];
            waitingTime[currentProcess] = turnAroundTime[currentProcess] - burstTime[currentProcess];
            time++;
        }
        time--;
    }
    // cout<<'"rjaohfiau"';

    fillInWaitTime();

    // cout<<"CompletionTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<completionTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"TurnaroundTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<turnAroundTime[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"WaitingTime :-> ";
    // for(int i=0;i<n;i++){
    //     cout<<waitingTime[i]<<" ";
    // }
    // cout<<endl;
}






void printAlgorithm(int algorithm_index){

    int algorithm_id = algorithms[algorithm_index].first - '0';
    if(algorithm_id==2){
        cout<<ALGORITHMS[algorithm_id]<<algorithms[algorithm_index].second<<endl;
    }
    else{
        cout<<ALGORITHMS[algorithm_id]<<endl;
    }
}

void printProcesses(){

    cout<<"Process    ";
    for (int i=0;i<noOfProcess;i++)
        cout<<"|  "<<get<0>(processes[i])<<"  ";
    cout<<"|\n";
}
void printArrivalTime(){

    cout<<"Arrival    ";
    for (int i=0;i<noOfProcess;i++)
        printf("|%3d  ",get<1>(processes[i]));
    cout<<"|\n";
}
void printServiceTime(){

    cout<<"Service    |";
    for (int i=0;i<noOfProcess;i++)
        printf("%3d  |",get<2>(processes[i]));
    cout<<" Mean|\n";

}
void printFinishTime(){

    cout<<"Finish     ";
    for (int i=0;i<noOfProcess;i++)
        printf("|%3d  ",completionTime[i]);
    cout<<"|-----|\n";

}
void printTurnAroundTime(){

    cout<<"Turnaround |";
    int sum = 0;
    for (int i=0;i<noOfProcess;i++)
    {
        printf("%3d  |",turnAroundTime[i]);
        sum += turnAroundTime[i];
    }
    if((1.0 * sum / turnAroundTime.size())>=10){
		printf("%2.2f|\n",(1.0 * sum / turnAroundTime.size()));
    }
    else{
        printf(" %2.2f|\n",(1.0 * sum / turnAroundTime.size()));
    }

}


void printWaitingTime(){

    cout<<"Waiting    |";
    int sum = 0;
    for (int i=0;i<noOfProcess;i++){
        printf("%3d  |",waitingTime[i]);
        sum += waitingTime[i];
    }

    if((1.0 * sum / waitingTime.size())>=10){
		printf("%2.2f|\n",(1.0 * sum / waitingTime.size()));
    }
    else{
        printf(" %2.2f|\n",(1.0 * sum / waitingTime.size()));
    }

}



void printStats(int algorithm_index){

    printAlgorithm(algorithm_index);
    printProcesses();
    printArrivalTime();
    printServiceTime();
    printFinishTime();
    printTurnAroundTime();
    printWaitingTime();

}

void printTimeline(int algorithm_index){

    for (int i=0;i<=lastInstant;i++)
        cout<< i % 10<<" ";
    cout<<"\n";
    cout<< "------------------------------------------------\n";
    for (int i = 0; i < noOfProcess; i++){
        cout<<get<0>(processes[i])<<"     |";
        for (int j=0;j<lastInstant;j++){
            cout<<timeline[j][i]<<"|";
        }
        cout << " \n";
    }
    cout << "------------------------------------------------\n";
}




void executeAlgorithm(char algorithm_id, int quantum,string operation){

    switch (algorithm_id)
    {
    case '1':
        if(operation==TRACE)cout<<"FCFS  ";
        firstComeFirstServe();
        break;
    case '2':
        if(operation==TRACE)cout<<"RR-"<<quantum<<"  ";
        roundRobin(quantum);
        break;
    case '3':
        if(operation==TRACE)cout<<"SPN   ";
        shortestJobFirst();
        break;
    case '4':
        if(operation==TRACE)cout<<"SRT   ";
        shortestRemainingTime();
        break;
    case '5':
        if(operation==TRACE)cout<<"Aging ";
        aging(quantum);
        break;
    case '6':
        if(operation==TRACE)cout<<"FB-1  ";
        feedbackQ1();
        break;
    case '7':
        if(operation==TRACE)cout<<"FB-2i ";
        feedbackQ2();
        break;
    default:
        break;
    }
}


int main() {

    // arrivalTime = {0,2,4,6,8};
    // burstTime = {3,6,4,5,2};
    // lastInstant = 20;
    // int n = arrivalTime.size();
    // noOfProcess = n;
    // completionTime.resize(n);
    // waitingTime.resize(n);
    // turnAroundTime.resize(n);
    // completionTime.resize(n);


    parse();
    for (int idx = 0; idx < algorithms.size(); idx++)
    {
        clear_timeline();
        executeAlgorithm(algorithms[idx].first, algorithms[idx].second,operation);
        if (operation == TRACE)
            printTimeline(idx);
        else if (operation == SHOW_STATISTICS)
            printStats(idx);
        cout << "\n";
    }
    return 0;



    // parse();
    // for (int i=0;i<algorithms.size();i++){

    //     clear_timeline();
    //     executeAlgorithm(algorithms[i].first, algorithms[i].second,operation);
    //     if (operation == TRACE)
    //         printTimeline(i);
    //     else if (operation == SHOW_STATISTICS)
    //         printStats(i);
    //     cout << "\n";
    // }











    // firstComeFirstServe();

    // shortestJobFirst();

    // shortestRemainingTime();

    // roundRobin(1);

    // feedbackQ1();

    // feedbackQ2();

    // aging(1);

}