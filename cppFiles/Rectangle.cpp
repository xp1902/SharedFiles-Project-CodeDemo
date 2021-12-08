#include "../hFiles/Rectangle.h"

Rectangle::Rectangle(int idx, vector<int> jobBlock, vector<vector<double>> dataSize, vector<double> Sc){
    id = idx;
    blockSize.resize(jobBlock[idx]);
    for(int i = 0; i <= blockSize.size() - 1; i ++){
        blockSize[i] = dataSize[idx][i];
    }
    blockNum = jobBlock[idx];
    executeTime.resize(blockNum);
    executeTime = compute_block_time(dataSize, Sc)[idx];
    //blockLocation.resize(blockNum);
    width = 0;
    heigth = blockNum;
    idleTime = 0.0;

    duration = accumulate(executeTime.begin(), executeTime.end(), 0.0);
    segments.resize(blockNum);
    //前面把每个block当做一个segment放到segments中
}

//根据给定的coreNum来计算此情况下的时间最小Rectangles排列方法
void Rectangle::dynamicSchedule(int coreNum){
    
}

//compute the computing time of each blocks in each job, dataSize->blockTime
//single host version
vector<vector<double> > Rectangle::compute_block_time(vector<vector<double>> dataSize, vector<double> Sc){
	vector<vector<double>> timeEachBlock;
	for(int i = 0; i <= dataSize.size() - 1; i ++){
		timeEachBlock[i].resize(dataSize[i].size(), 0);
		for(int j = 0; j <= dataSize[i].size() - 1; j++){
			timeEachBlock[i][j] = dataSize[i][j] / Sc[i];
		}
	}
	return timeEachBlock;
}

//computing time per job
vector<double> compute_jobTime(vector<vector<double>> timeEachBlocks){
	vector<double> timeEachJob;
	timeEachJob.resize(timeEachBlocks.size(), 0.0);
	for(int i = 0; i <= timeEachBlocks.size() - 1; i ++){
		timeEachJob[i] = accumulate(timeEachBlocks[i].begin(), timeEachBlocks[i].end(), 0.0);

	}
	return timeEachJob;
}