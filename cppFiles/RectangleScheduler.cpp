#include "../hFiles/RectangleScheduler.h"


	/*
	*	vector<double> jobFinishTime
	*	vector<vector<tuple<int, int, int>>> runLoc jobID->blockID->(hostID, coreID, rank)
	*	vector<vector<vector<tuple<int, int, double, double>>>> hostCoreTask host->core->task-> <job,block,startTime,endTime>
	*	vector<vector<double> > hostCoreFinishTime; // host->core->finishTime
	*/
void RectangleScheduler::schedule(int id, vector<vector<vector<tuple<int, int, double, double>>>> hostCoreTask, vector<vector<tuple<int, int, int>>> runLoc, vector<double> jobFinishTime, vector<int> jobCores){
    //遍历确定矩形的运行时间（长度）最短的id
    double time;
    int coreNum;
    for(int i = 1; i <= 10; i ++){
        if(i == 1) time = rects[1][id].width;
        else{
            if(rects[i][id].width < time){
                time = rects[i][id].width;
                coreNum = i;
            }
        }
    }
    Timeline line = lines.back();
    while(true){
        int cores = 0;
        for(int i = lines.size() - 1; i >= 0; i --){
            cores += lines[i].coreNum;
            if(cores >= coreNum){
                //给任务分配好核，然后分割形成新的timeline
                lineSplit(rects[coreNum][id], i, cores - coreNum);

                //set the variables jobFinishTime
                jobFinishTime[rects[coreNum][id].id] = lines.back().time;
                //set runLoc
                //the rank is set to all 1...
                for(int i = 0; i <= coreNum - 1; i ++){
                    for(int j = 0; j <= rects[coreNum][id].segments[i].blocks.size() - 1; j++){
                        tuple<int, int, int> tp(lines.back().cores[i] / 100, lines.back().cores[i] % 100, 1);//还没想好rank怎么填进去...
                        runLoc[id][rects[coreNum][id].segments[i].blocks[j]] = tp;
                    }
                }
                //set host core finishTime
                //write here

                //set hostCoreTask
                //write here

                //set jobCores
                jobCores[id] = coreNum;
                break;
            } 
        }
    }
    return;
}

//把rectangle放到timeline的合适位置，并切分好timeline，最好把形成的新的timeline放到timelines的最后，并且timeline中的第一个核分配给了Rectangle中第一个segment
//以此类推
void RectangleScheduler::lineSplit(Rectangle rect, int idx, int remainCores){
    // double time = 0.0;
    // vector<int> cores;
    // cores.resize(rect.heigth);
    // Timeline newLine(time, cores);

    // for(int i = lines.size() - 1; i >= idx; i --){
    //     Timeline line = lines.back();
    //     time = max(time, line.time);

    //     if(i == idx){
    //         for(int j = 1; j <= line.coreNum - remainCores; i --){
    //             newLine.cores.push_back(line.cores.back());
    //             line.cores.pop_back();
    //         }
    //     }
    //     else{
    //         lines.pop_back();
    //         newLine.cores.insert(newLine.cores.begin(), line.cores.begin(), line.cores.end());
    //     }
    // }
    // newLine.time = time;
    // lines.push_back(newLine);
}