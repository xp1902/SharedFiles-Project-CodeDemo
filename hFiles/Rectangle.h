#pragma once
#include<vector>
// #include "ResourceScheduler.h"
#include<queue>
#include<algorithm>
#include<numeric>

using namespace std;

//任务矩形中对应分配的一个core的block集合
struct Pair
{
	int time;
    vector<int> blocks;
	bool friend operator < (Pair a,Pair b)
	{
		// if(a.time==b.time)
		// 	return a.idx>b.idx;
		// else
		// 	return a.meter > b.meter;
        return a.time > b.time;
	}
	Pair(int t, vector<int> bs){
		time = t;
        blocks = bs;
	}
    Pair(){
        time= 0.0;
    }
};

//任务矩形
class Rectangle{

public:
    Rectangle(int, vector<int> jobBlock, vector<vector<double>> dataSize, vector<double> Sc);
    Rectangle(){
        idleTime = 0.0;
        duration = 0.0;
        heigth = 0;
        width = 0;
        blockNum = 0;
        id = -1;
    }
    //vector<vector<int>> allocatedCores;//allocated cores for this job, memory the core location
    double idleTime;
    double duration;
    int heigth;
    int blockNum;
    int width;
    int id;//rectangle 对应的job ID
    //vector<vector<int>> allocation;//core number-job blocks in this core

    vector<double> blockSize;//size of blocks of job
    vector<double> executeTime;//execute time of each blocks
    //vector<int> blockLocation;//block locations

    vector<Pair> segments;

    //void sortBlocks();
    void dynamicSchedule(int);//
    
    vector<vector<double>> compute_block_time(vector<vector<double>>, vector<double>);
    //double g(int);
};