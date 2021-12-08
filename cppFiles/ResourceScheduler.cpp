#include "../hFiles/ResourceScheduler.h"
// #include "../hFiles/Rectangle.h"
// #include "../hFiles/Timeline.h"
#include "../hFiles/RectangleScheduler.h"

ResourceScheduler::ResourceScheduler(int tasktype,int caseID) {
	taskType = tasktype;
	string filePath = "../input/task" + to_string(taskType) + "_case"+to_string(caseID)+".txt";
	freopen(filePath.c_str(), "r", stdin);
	cin >> numJob >> numHost >> alpha;
	if (taskType == 2)
		cin >> St;
	hostCore.resize(numHost);
	for (int i = 0; i < numHost; i++)
		cin >> hostCore[i];

	jobBlock.resize(numJob);
	for (int i = 0; i < numJob; i++)
		cin >> jobBlock[i];

	Sc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		cin >> Sc[i];

	dataSize.resize(numJob);
	for (int i = 0; i < numJob; i++) {
		dataSize[i].resize(jobBlock[i]);
		for (int j = 0; j < jobBlock[i]; j++)
			cin >> dataSize[i][j];
	}

	location.resize(numJob);
	for (int i = 0; i < numJob; i++) {
		location[i].resize(jobBlock[i]);
		for (int j = 0; j < jobBlock[i]; j++)
			cin >> location[i][j];
	}

	jobFinishTime.resize(numJob, 0);
	jobCore.resize(numJob);

	runLoc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		runLoc[i].resize(jobBlock[i]);

	hostCoreTask.resize(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreTask[i].resize(hostCore[i]);

	hostCoreFinishTime.resize(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreFinishTime[i].resize(hostCore[i], 0);
}

void ResourceScheduler::schedule() {

	vector<vector<int>> hostCoreBlock(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreBlock[i].resize(hostCore[i], 0);

	for (int i = 0; i < numJob; i++) {
		set<pair<int, int>> allocatedJobCore;
		for (int j = 0; j < jobBlock[i]; j++) {
			int hid = rand() % numHost;
			int cid = rand() % hostCore[hid];
			allocatedJobCore.insert({ hid,cid });
			runLoc[i][j] = make_tuple(hid, cid, hostCoreBlock[hid][cid]++);
		}
		/*for (int j = 0; j < jobBlock[i]; j++)
			finishTime[i]+=*/
		jobFinishTime[i] = rand() % 200;
		jobCore[i] = allocatedJobCore.size();
	}


	for (int i = 0; i < numHost; i++) {
		for (int j = 0; j < hostCore[i]; j++) {
			int numTask = rand() % 10 + 1;
			hostCoreTask[i][j].resize(numTask);
			for (int k = 0; k < numTask; k++) {
				int jid = rand() % numJob;
				int bid = rand() % jobBlock[jid];
				int endTime = hostCoreFinishTime[i][j] + rand() % 100 + 1;
				hostCoreTask[i][j][k] = make_tuple(jid, bid, hostCoreFinishTime[i][j], endTime);
				hostCoreFinishTime[i][j] = endTime;
			}
		}
	}
}

/*
*Scheduling algorithm:
*1.at first we create a rectangle which minimize the inside idle time
*2.then we can schedule those rectangles to hosts' cores, using job scheduling algorithm
*3.core, how to choose cores to compute each blocks?
*	3.1.every job could be a sumation of many partition according to hosts
*	3.2.when we want to allocate one job's using cores, we want to minimize the transfer time
*	3.3.at every turn we fill all the hosts' cores, we need to minimize it
*	3.4.how about to we create a class of Rectangle(containing cores host perpective, idle time, )
*/
void ResourceScheduler::mySchedule(){
	vector<vector<Rectangle>> rects;
	//根据job来建立rectangles 比如分配核为2时的最佳Rectangle
	rects.resize(11);
	for(int i = 1; i <= 10; i ++)
		rects[i].resize(numJob);
	for(int i = 0; i <= rects.size() - 1; i ++){
		for(int j = 1; j <= 10; j ++){
			Rectangle rect = Rectangle(i, jobBlock, dataSize, Sc);
			rects[j].push_back(rect);
		}
	}
	int coreSum = accumulate(hostCore.begin(), hostCore.end(), 0);

	//create a series of Timelines initially only one with time=0
	vector<Timeline> timelines;
	timelines.resize(1);
	vector<int> cores;
	cores.resize(accumulate(hostCore.begin(), hostCore.end(), 0));
	//corenumber computing method: hostID * 100 + coreID
	//corenumber 2 hostID: corenumber / 100
	//corenumber 2 corenumber: corenumber % 100
	for(int i = 0; i <= hostCore.size() - 1; i ++){
		for(int j = 0; j <= hostCore[i] - 1; j++){
			cores.push_back(i * 100 + j);
		}	
	}
	timelines.push_back(Timeline(0.0, cores));


	RectangleScheduler RS(timelines, rects);

	/*
	*	vector<double> jobFinishTime
	*	vector<int> jobCore
	*	vector<vector<tuple<int, int, int>>> runLoc 				jobID->blockID->(hostID, coreID, rank)
	*	vector<vector<vector<tuple<int, int, double, double>>>> 	hostCoreTask host->core->task-> <job,block,startTime,endTime>
	*	vector<vector<double> > hostCoreFinishTime					host->core->finishTime
	*/
	//main loop
	for(int i = 0; i <= numJob - 1; i ++){
		//从rects中选出一个任务，可以按总时间，矩形面积，核的数量等等
		int id = nextRect(RS.rects);
		RS.schedule(id, hostCoreTask, runLoc, jobFinishTime, jobCore);
		RS.reSort();
	}

	//set hostCoreFinishTime
	for(int i = 0; i <= RS.lines.size() - 1; i ++){
		for(int j = 0; j <= RS.lines[i].cores.size() - 1; j++)
		{
			int core = RS.lines[i].cores[j];
			hostCoreFinishTime[core / 100][core % 100] = RS.lines[i].time;
		}
	}
}

//怎么从rect中选出一个任务
int nextRect(vector<vector<Rectangle>> rects){
	//write here


	return 0;
}

//number of total cores
int num_cores(vector<int> hostCores){
	return accumulate(hostCores.begin(), hostCores.end(), 0);
}

//block perspective output
void ResourceScheduler::outputSolutionFromBlock() {
	cout << "\nTask2 Solution (Block Perspective) of Teaching Assistant:\n\n";
	for (int i = 0; i < numJob; i++) {
		double speed = g(jobCore[i]);
		cout << "Job" << i << " obtains " << jobCore[i] << " cores (speed=" << speed << ") and finishes at time " << jobFinishTime[i] << ": \n";
		for (int j = 0; j < jobBlock[i]; j++) {
			cout << "\tBlock" << j << ": H" << get<0>(runLoc[i][j]) << ", C" << get<1>(runLoc[i][j]) << ", R" << get<2>(runLoc[i][j]) << " (time=" << fixed << setprecision(2) << dataSize[i][j] / speed << ")" << " \n";
		}
		cout << "\n";
	}
	cout << "The maximum finish time: " << *max_element(jobFinishTime.begin(), jobFinishTime.end()) << "\n";
	cout << "The total response time: " << accumulate(jobFinishTime.begin(), jobFinishTime.end(), 0.0) << "\n\n";
}

//core perspective output
void ResourceScheduler::outputSolutionFromCore() {
	cout << "\nTask2 Solution (Core Perspective) of Teaching Assistant:\n\n";
	double maxHostTime = 0, totalRunningTime = 0.0;
	for (int i = 0; i < numHost; i++) {
		double hostTime = *max_element(hostCoreFinishTime[i].begin(), hostCoreFinishTime[i].end());
		maxHostTime = max(hostTime, maxHostTime);
		totalRunningTime += accumulate(hostCoreFinishTime[i].begin(), hostCoreFinishTime[i].end(), 0.0);
		cout << "Host" << i << " finishes at time " << hostTime << ":\n\n";
		for (int j = 0; j < hostCore[i]; j++) {
			cout << "\tCore" << j << " has " << hostCoreTask[i][j].size() << " tasks and finishes at time " << hostCoreFinishTime[i][j] << ":\n";
			for (int k = 0; k < hostCoreTask[i][j].size(); k++) {
				cout << "\t\tJ" << setw(2) << setfill('0') << get<0>(hostCoreTask[i][j][k]) << ", B" << setw(2) << setfill('0') << get<1>(hostCoreTask[i][j][k]) << ", runTime " << fixed << setprecision(1) << setw(5) << setfill('0') << get<2>(hostCoreTask[i][j][k]) << " to " << fixed << setprecision(1) << setw(5) << setfill('0') << get<3>(hostCoreTask[i][j][k]) << "\n";
			}
			cout << "\n";
		}
		cout << "\n\n";
	}
	cout << "The maximum finish time of hosts: " << maxHostTime << "\n";
	cout << "The total efficacious running time: " << totalRunningTime << "\n";
	cout << "Utilization rate: " << totalRunningTime / accumulate(hostCore.begin(), hostCore.end(), 0.0) / maxHostTime << "\n\n";
}

void ResourceScheduler::visualization() {
	return;
}

double ResourceScheduler::g(int e) {
	return 1 - alpha * (1 - e);
}