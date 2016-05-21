//MULTILEVEL FEEDBACK QUEUE

#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

struct process
{
	int processid;
	int arrival_time;
	int burst_time;
	int priority;
	int time_left;  
	int starting_time;
	int end_time;
};

//returns true if process2 is priority is greater than process1 priority
//returns true if process2 is earlier than process1
class compare_arrivaltime
{
	public:
	bool operator()(const process& process1, const process& process2) const
	{	
		
		if ( process1.priority >process2.priority)
			return true;
		if(process1.priority==process2.priority && process1.arrival_time>process2.arrival_time)
			return true;
		if(process1.priority==process2.priority && process1.arrival_time==process2.arrival_time && process1.processid>process2.processid)
			return true;
		return false;
	}
}; 

int i,j,no_of_process,minimum,completion_time=0;
int processid,arrival_time,burst_time,priority;
int temp_processid,temp_arrivaltime,temp_bursttime,temp_priority,temp_remainingtime,temp_startingtime,temp_endtime;
int gannt_chart[1000];
int time=0,turn=0;
int priority_quantum=4,roundrobin_quantum=4;
int status=0;
void Initialize_gannt_chart(int t);
void print_gannt_chart();
void sort_according_to_arrivaltime(struct process *p);
void sort_according_to_processid(struct process *p);
void print_result(struct process p[],int store_pids[]);
void implement_queue2_IS_NOT_EMPTY();
void implement_queue1_IS_NOT_EMPTY();
priority_queue<process, vector<process>, compare_arrivaltime> queue1;
queue<process> queue2;
int main()
{	
	scanf("%d",&no_of_process);	
	int store_pids[no_of_process];
	struct process *p=(struct process*)malloc((no_of_process+1)*sizeof(struct process));
	for( i=0;i<no_of_process;i++)
	{	
		scanf("%d %d %d %d",&processid,&arrival_time,&burst_time,&priority);
		p[i].processid = processid;
		store_pids[i]=processid;
		p[i].arrival_time=arrival_time;
		p[i].burst_time=burst_time;
		p[i].priority=priority;
		p[i].time_left=burst_time;
		completion_time = completion_time + burst_time;
	}

	Initialize_gannt_chart(completion_time);
	
	//Sorting of processes according to arrival time 
	sort_according_to_arrivaltime(p);
		
	while((!queue1.empty())||(!queue2.empty())||(status==0)||(turn<no_of_process))
	{
		while((turn<no_of_process) && (p[turn].arrival_time==time))
		{
			if(priority_quantum!=4 && (p[turn].priority<queue1.top().priority))
			{
				queue2.push(queue1.top());
				queue1.pop();
				priority_quantum=4;
			}
			status=1;
			queue1.push(p[turn]);
			turn = turn +1;
		}


		if(!queue1.empty())
		{
				
			implement_queue1_IS_NOT_EMPTY();
		}
		else if(!queue2.empty())
		{	
			implement_queue2_IS_NOT_EMPTY();
		}		
	
		time= time+1;
		
	}

	//print_gannt_chart();

	for(i=0;i<no_of_process;i++)
	{
		int flag=0;
		int current_processid = p[i].processid;
		for(j=0;j<time;j++)
		{
			if(gannt_chart[j]==current_processid)
			{
				if(flag==0)
				{
					p[i].starting_time=j;
					flag=1;
				}
				else
					p[i].end_time=j+1;
			}
		}
	}
	// sorting of processes according to processid-lower to higher
	sort_according_to_processid(p);

	//print the result
	print_result(p,store_pids);
	return 0;
}

void print_gannt_chart()
{
	printf("\nGannt Chart of the processes is : ");
		for(i=0;i<time;i++)
			printf("%d",gannt_chart[i]);
		printf("\n");
}

/*void print_result(struct process p[])
{	
	int i;
	printf("\n");
	int new_processid,response_time,waiting_time,finish_time;
	for(i=0;i<no_of_process;i++)
	{		
		new_processid = p[i].processid;
		response_time = p[i].starting_time - p[i].arrival_time;
		waiting_time = p[i].end_time - p[i].arrival_time - p[i].burst_time;
		finish_time=p[i].end_time;
		printf("%d %d %d %d\n",new_processid,response_time,finish_time,waiting_time);
	}
}*/

void  print_result(struct process p[],int store_pids[])
{
	int i,j;
	printf("\n");
	int new_processid,response_time,waiting_time,finish_time;
	for(i=0;i<no_of_process;i++)
	{
		for(j=0;j<no_of_process;j++)
		{
			if(store_pids[i]==p[j].processid)
			{
				new_processid = p[j].processid;
				response_time = p[j].starting_time - p[j].arrival_time;
				waiting_time = p[j].end_time - p[j].arrival_time - p[j].burst_time;
				finish_time=p[j].end_time;
				printf("%d %d %d %d\n",new_processid,response_time,finish_time,waiting_time);
			}
		}
	}
}


void Initialize_gannt_chart(int t)
{
	for(i=0;i<=t;i++)
		gannt_chart[i]=0;
}

void implement_queue1_IS_NOT_EMPTY()
{
	if((roundrobin_quantum!=4)&&(!queue2.empty()))
			{
				queue2.push(queue2.front());
				queue2.pop();
				roundrobin_quantum=4;
			}
			process cur = queue1.top();
			gannt_chart[time]=cur.processid;
			cur.time_left = cur.time_left-1;
			queue1.push(cur);
			queue1.pop();
			priority_quantum=priority_quantum-1;
			if(queue1.top().time_left==0)
			{
				queue1.pop();
				priority_quantum=4;
			}
			else if(priority_quantum==0)
			{
				
				if(queue1.top().time_left>0)
					queue2.push(queue1.top());
				queue1.pop();
				priority_quantum=4;
			}
}

void implement_queue2_IS_NOT_EMPTY()
{
	queue2.front().time_left = queue2.front().time_left-1;
	gannt_chart[time]=queue2.front().processid;
	roundrobin_quantum--;
	if(queue2.front().time_left==0)
	{
		queue2.pop();
		roundrobin_quantum=4;
	}
			
	else if(roundrobin_quantum==0)
	{
		queue2.push(queue2.front());
		queue2.pop();
		roundrobin_quantum=4;
	}
}


void sort_according_to_arrivaltime(struct process p[])
{
	for(i=0;i<no_of_process-1;i++)
	{	
		minimum=i;
		for(j=i+1;j<no_of_process;j++)
		{
			if(p[j].arrival_time<p[minimum].arrival_time)
			{
				minimum=j;
			}
		}
		if(i!=minimum)
		{
			temp_processid=p[minimum].processid;
			temp_arrivaltime=p[minimum].arrival_time;
			temp_bursttime=p[minimum].burst_time;
			temp_priority=p[minimum].priority;
			temp_remainingtime=p[minimum].time_left;

			p[minimum].processid=p[i].processid;
			p[minimum].arrival_time=p[i].arrival_time;
			p[minimum].burst_time=p[i].burst_time;
			p[minimum].priority=p[i].priority;
			p[minimum].time_left=p[i].time_left;

			p[i].processid=temp_processid;
			p[i].arrival_time=temp_arrivaltime;
			p[i].burst_time=temp_bursttime;
			p[i].priority=temp_priority;
			p[i].time_left=temp_remainingtime;			
		}
	}
	
}

void sort_according_to_processid(struct process p[])
{
	for(i=0;i<no_of_process-1;i++)
	{	
		minimum=i;
		for(j=i+1;j<no_of_process;j++)
		{
			if(p[j].processid<p[minimum].processid)
			{
				minimum=j;
			}
		}
		if(i!=minimum)
		{
			temp_processid=p[minimum].processid;
			temp_arrivaltime=p[minimum].arrival_time;
			temp_bursttime=p[minimum].burst_time;
			temp_priority=p[minimum].priority;
			temp_remainingtime=p[minimum].time_left;
			temp_startingtime=p[minimum].starting_time;
			temp_endtime=p[minimum].end_time;

			p[minimum].processid=p[i].processid;
			p[minimum].arrival_time=p[i].arrival_time;
			p[minimum].burst_time=p[i].burst_time;
			p[minimum].priority=p[i].priority;
			p[minimum].time_left=p[i].time_left;
			p[minimum].starting_time=p[i].starting_time;
			p[minimum].end_time=p[i].end_time;
			
			p[i].processid=temp_processid;
			p[i].arrival_time=temp_arrivaltime;
			p[i].burst_time=temp_bursttime;
			p[i].priority=temp_priority;
			p[i].time_left=temp_remainingtime;
			p[i].starting_time=temp_startingtime;
			p[i].end_time=temp_endtime;			
		}
	}
	
}
