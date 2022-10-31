#include <iostream>
#include<bits/stdc++.h>
using namespace std;

//struct work as comparator in priority queue to arrange first
//by arrival time if the two arrival times are equal,then compare
//by process ID
struct Compare
{
    bool operator()(vector<int> const & a, vector<int> const & b)
    {
        if(a[3]==b[3])
            return a[0] > b[0];
        return a[3] > b[3];
    }
};

//Function to read the file and store the data in a priority queue
void readmatrix(priority_queue< vector<int>, vector< vector<int> >, Compare > &matrix)
{
    char filename[15];
    cin >> filename;
    freopen(filename,"rt",stdin);
    for(int i=0 ;;i++)
    {
        vector<int> ab(4);
        cin >> ab[0]>>ab[1]>>ab[2]>>ab[3];
        ab.push_back(ab[1]) ;
        if(ab[1]==0)
        {
            ab[1]=-4;
            //ab[2]++;
        }
        matrix.push(ab);

        if(feof(stdin))
            break;
    }
}
//Function to loop on any priority queue and print it by passing the priority queue and its size
void printmatrix(int x, int y, priority_queue< vector<int>, vector< vector<int> >, Compare > ab)
{
    for(int i=0; i<x; i++)
    {
        for(int j=0; j<y; j++)
        {
            cout << ab.top()[j]<<' ';
        }
        ab.pop();
        cout << '\n';
    }
}
//to calculate turnaround time we need to know the time the process enter the system and the time it exit
//so we used arrival array of pairs to store that information
//get arrival time function save the process id and the time entering the system
//time the process exit is saved at run time
void get_arrival_time(priority_queue< vector<int>, vector< vector<int> >, Compare > matrix,pair<int,pair<int,int> >arrival[])
{
    int n = matrix.size();
    for(int i=0; !matrix.empty(); i++)
    {
        arrival[i] = make_pair(matrix.top()[0], make_pair(matrix.top()[3], -1));
        matrix.pop();
    }
    for (int i=0; i<n-1; i++)
        for (int j=0; j<n-i-1; j++)
            if (arrival[j].first > arrival[j+1].first)
                swap(arrival[j], arrival[j+1]);
}
//function used to enter the process the system by matching it arrival time and system time
//passing it matrix and time of system and ready queue . its job to move processes to ready queue
void to_Ready (int time, priority_queue< vector<int>, vector< vector<int> >, Compare > &matrix,
                     priority_queue< vector<int>, vector< vector<int> >, Compare > &ready,
                     vector<vector<int> > &block, pair<int,pair<int,int> > arrival[])
{
    while(time==matrix.top()[3] && !matrix.empty())
    {
        if(matrix.top()[1]==-4)
        {
            vector <int> saeed;
            saeed = matrix.top();
            block.push_back(saeed);
            saeed[3]+= saeed[2];
            saeed[1] = 0;
            arrival[saeed[0]].second.second = time + saeed[2];
            //matrix.push(saeed);
            matrix.pop();
        }
        else
        {
            ready.push(matrix.top());
            matrix.pop();
        }
    }
}
//function work as follow
//.......1) if no process are running on the system just check if ready queue has process
//..........in it if it has , then pick and and run it
//.......2) if theres a process runs , decreases its CPU time by one and then check if the
//..........CPU time is zero then its will be blocked for I/O if -1 then its time to leave
//..........the system then save its leaving time in arrival array of pairs and delete it
//..........and search for ready process in ready queue if there is run it
void to_Running_FCFS (int time, priority_queue< vector<int>, vector< vector<int> >, Compare > &matrix,
                priority_queue< vector<int>, vector< vector<int> >, Compare > &ready,
                vector<vector<int> > &block,
                vector<int>&running, pair<int,pair<int,int> > arrival[])
{
    vector<int>notrun(4,-1);
    if(running[1]==-1)
    {
        if(!ready.empty())
        {
            running = ready.top();
            ready.pop();
        }
    }
    else
    {   //CPU = 0
        running[1]--;  //CPU -1 original_CPU = 5

        if(running[1]<1)
        {
            if(running[1]==0 && running[4]!=-1)
            {
                running[3] = time + running[2];
                running[1] = running[4];
                running[4] = -1;

                if(running[2]!=0)
                {
                    block.push_back(running);
                }
                matrix.push(running);
            }
            else
            {
                arrival[running[0]].second.second = time;
            }
            if(!ready.empty())
            {
                running= ready.top();
                ready.pop();
            }
            else
            {
                running = notrun;
            }
        }
    }
}
//function work as follow
//.......1) if no process are running on the system just check if ready queue has process
//..........in it if it has , then pick and and run it
//.......2) if theres a process runs , decreases its CPU time by one and decrease it time slice by one
//..........and then check if the CPU time is zero then its will be blocked for I/O if -1 then its time to leave
//..........the system then save its leaving time in arrival array of pairs and delete it and check for a ready
//..........process in ready queue if there is run it and give it a new time slice
void to_Running_RR (int time, int slice, int &runtime_slice,priority_queue< vector<int>, vector< vector<int> >, Compare > &matrix,
                priority_queue< vector<int>, vector< vector<int> >, Compare > &ready,
                vector<vector<int> > &block,
                vector<int>&running, pair<int,pair<int,int> > arrival[] )
{
    vector<int>notrun(4,-1);
    int here ;
    here = 0 ;
    if(runtime_slice!=0)
    {
        if(running[1]==-1)
        {
            if(!ready.empty())
            {
                runtime_slice = slice;
                running = ready.top();
                ready.pop();
            }
        }
        else
        {
            runtime_slice--;
            running[1]--;
            if(running[1]<1)
            {
                if(running[1]==0 && running[4]!=-1)
                {
                    running[3] = time + running[2];
                    running[1] = running[4];
                    running[4] = -1 ;

                    if(running[2]!=0)
                        block.push_back(running);

                    matrix.push(running);
                }
                else
                    arrival[running[0]].second.second = time;

                if(!ready.empty())
                {
                    here = 1 ;
                    runtime_slice = slice;
                    running = ready.top();
                    ready.pop();
                }
                else
                {
                    here = 1 ;
                    runtime_slice = slice;
                    running = notrun;
                }
            }
            if(runtime_slice==0 && here==0)
            {
                running[3] = time;
                ready.push(running);
                runtime_slice = slice;
                running = ready.top();
                ready.pop();
            }
      }
    }
}
//iterate for the blocked process for input output and decrease the
//I/O time by one till it down to zero then delete it from the vector
void adjust_Block (vector<vector<int> > &block )
{
    for(int i=block.size()-1; i>=0; i--)
    {
        if(block[i][2]==1)
            block.erase(block.begin()+i);
        else
            block[i][2]--;
    }
}
//Every quantum time we call save state function to save the processes state in file
//check if there is a running process then print it and also check for ready and blocked processes
void save_state(int time, priority_queue< vector<int>, vector< vector<int> >, Compare > ready,
                vector<vector<int> > block,
                vector<int>running )
{
    cout << "At "<<time<<"==>";

    if(running[3]!=-1)
        cout<<running[0]<<" running  ";

    while(!ready.empty())
    {
        cout<<ready.top()[0]<<" ready  ";
        ready.pop();
    }

    for(int i=0; i<block.size(); i++)
        cout<<block[i][0]<<" block  ";

    cout<<'\n';
}
//Final
//1) finishing time relative to the final time after finishing last process
//2) CPU utilization by dividing the number of quanta the CPU were work by
//...the All quanta the system take
//3) Turnaround time using arrival array of pairs which store every process ID and
//...the time enter the system and the time left the system
void final_calcaulations (int all, int run, pair<int,pair<int,int> > arrival[], int n)
{
    cout <<'\n';
    cout << "Finishing time : "<<all-1<<'\n';

    cout << "CPU Utilization : "<< setprecision(4)<< (float)run/(all)<<'\n';

    for(int i=0; i<n ;i++)
        cout <<"Turnaround time of Process "<<arrival[i].first<<" : "<< arrival[i].second.second-arrival[i].second.first<<'\n';
}
//FCFS algorithm is simple is just the process start to run on CPU , wait it until it finish or Blocked
//for I/O purpose ,then run another if there is a process in a ready queue
void FCFS(priority_queue< vector<int>, vector< vector<int> >, Compare > &matrix, pair<int,pair<int,int> > arrival[])
{
    int run = 0;
    int number_of_processess = matrix.size();
    char filename[15] = "FCFS.txt";
    freopen(filename,"wt",stdout);

    vector<int>Running (4,-1);
    vector<vector<int> > Block;
    priority_queue< vector<int>, vector< vector<int> >, Compare > Ready;

    int time = 0;
    for(;;time++)
    {
        adjust_Block(Block);
        //if(running[1]==-4)
        to_Ready(time, matrix, Ready, Block, arrival);

        to_Running_FCFS(time, matrix, Ready, Block, Running, arrival);

        if(Running[0]!=-1)
            run++;
        if(matrix.empty() && Ready.empty() && Running[0]==-1 && Block.empty())
            break;

        save_state(time, Ready, Block, Running);
    }
    final_calcaulations(time, run, arrival, number_of_processess);

    return;
}
//RR Algorithm is give every process a slice of time not until it finish the time slice is
//user selected if it is
void RR(priority_queue< vector<int>, vector< vector<int> >, Compare > &matrix, int slice, pair<int,pair<int,int> > arrival[])
{
    int run = 0;
    int number_of_processess = matrix.size();
    char filename[15] = "RR.txt";
    freopen(filename,"wt",stdout);

    vector<int>Running (4,-1);
    vector<vector<int> > Block;
    priority_queue< vector<int>, vector< vector<int> >, Compare > Ready;

    int runtime_slice = slice;
    int time = 0;
    for(;;time++)
    {
        adjust_Block(Block);
        to_Ready(time, matrix, Ready, Block, arrival);
        to_Running_RR(time, slice, runtime_slice, matrix, Ready,Block, Running, arrival);

        if(Running[0]!=-1)
            run++;
        if(matrix.empty() && Ready.empty() && Running[0]==-1 && Block.empty())
            break;

        save_state(time, Ready, Block, Running);
    }
    final_calcaulations(time, run, arrival, number_of_processess);

    return;
}

int main(void)
{
    int mode, slice;
    cin >> mode;
    if(mode==1)
        cin >> slice;

    priority_queue< vector<int>, vector< vector<int> >, Compare > matrix;
    readmatrix(matrix);
    pair<int, pair<int,int> > arrival[matrix.size()];

    get_arrival_time(matrix, arrival);

    if(mode==0)
        FCFS(matrix, arrival);
    else
        RR(matrix, slice, arrival);

    return 0;
}
