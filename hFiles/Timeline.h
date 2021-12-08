#pragma once
#include <vector>

using namespace std;

struct Timeline
{
    /* data */
    bool friend operator < (Timeline a, Timeline b)
	{
		if(a.time==b.time)
			return a.coreNum > b.coreNum;
		else
			return a.time > b.time;
        // return a.time > b.time;
	}

public:
    double time;//time of the timeline platform
    vector<int> cores;//timeline contain cores
    int coreNum;//cores.size()

    Timeline(double t, vector<int> c){
        time = t;
        cores = c;
        coreNum = c.size();
    }
    Timeline(){
        time = 0.0;
        coreNum = 0;
    }
};