#include <bits/stdc++.h>

using namespace std;



string operation;
int lastInstant, noOfProcess;
vector<pair<char, int>> algorithms;
vector<int>arrivalTime;
vector<int>burstTime;
vector<tuple<string,int,int>> processes;
vector<vector<char>>timeline;
unordered_map<string,int>processToIndex;


//Results

vector<int>completionTime;
vector<int>turnAroundTime;
vector<int>waitingTime;






void parse_algorithms(string algorithm_chunk){

    stringstream stream(algorithm_chunk);
    while (stream.good())
    {
        string temp_str;
        getline(stream, temp_str, ',');
        stringstream ss(temp_str);
        getline(ss, temp_str, '-');
        char algorithm_id = temp_str[0];
        getline(ss, temp_str, '-');
        int quantum = temp_str.size() >= 1 ? stoi(temp_str) : -1;
        algorithms.push_back( make_pair(algorithm_id, quantum) );
    }


}

void parse_processes(){

    string process_chunk, process_name;
    int process_arrival_time, process_service_time;
    for(int i=0; i<noOfProcess; i++)
    {
        cin >> process_chunk;

        stringstream stream(process_chunk);
        string temp_str;
        getline(stream, temp_str, ',');
        process_name = temp_str;
        getline(stream, temp_str, ',');
        process_arrival_time = stoi(temp_str);
        getline(stream, temp_str, ',');
        process_service_time = stoi(temp_str);
        processes.push_back( make_tuple(process_name, process_arrival_time, process_service_time) );
        processToIndex[process_name] = i;
    }

    arrivalTime.resize(noOfProcess);
    burstTime.resize(noOfProcess);
    for(int i=0;i<processes.size();i++){
        arrivalTime[i] = get<1>(processes[i]);
        burstTime[i] = get<2>(processes[i]);
    }

}

void parse(){

    string algorithm_chunk;
    cin >> operation >> algorithm_chunk >> lastInstant >> noOfProcess;
    parse_algorithms(algorithm_chunk);
    parse_processes();
    completionTime.resize(noOfProcess);
    turnAroundTime.resize(noOfProcess);
    waitingTime.resize(noOfProcess);
    timeline.resize(lastInstant);
    for(int i=0; i<lastInstant; i++){
        for(int j=0; j<noOfProcess; j++){
            timeline[i].push_back(' ');
        }
    }
        
}