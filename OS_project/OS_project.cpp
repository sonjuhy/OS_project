#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Process {
	int num;
	int ArriveTime;
	int BrustTime;
	int Priority;
	int remaind;
	int save_point;
	int wait;
	int TAT;//Turnaroundtime
	int stat;//0 : wait, 1 : runnung, 2 : finish, 3: stop
};
void Schedular_fun(Process **proc, int mode);
Process *Proc_refresh(Process **proc, int row);
Process *Proc_replace(Process** proc);
Process* Proc_fun(Process **proc, int row);

Process* FileIO_fun(Process **proc);

Process* Arrive_fun(Process **proc);//for FCFS
Process* Burst_fun(Process **proc);//for SJF
Process* SRTF_fun(Process **proc);//for SRTF
Process* Priority_fun(Process **process);//for Priority
Process* RoundRobin_fun(Process **proc);//for RoundRobin

int A_Block(Process **proc, int j);
int B_Block(Process **proc, int j);
int row_count = 0;

int main()
{
	struct Process *proc = 0;//process# , CPU_time, Arrival_time, Priority
	Process tmp_proc = { 0, };
	int mode = 0, key = 0, row = 0, Read_check = 0;

	proc = (Process*)malloc(sizeof(struct Process));
	proc = Proc_refresh(&proc, 0);

	printf("Main Menu\n");
	printf("---------\n");
	printf("1.  Read processes from proc.txt\n");
	printf("2.  Generate random processes\n");
	printf("3.  First come first Serve (FCFS)\n");
	printf("4.  Shortest Job First (SJF)\n");
	printf("5.  Shortest Remaining time First (SRTF)\n");
	printf("6.  Priority\n");
	printf("7.  Round Robin (RR)\n");
	printf("8.  Exit\n");

	while (1) {
		printf("Choose function : ");
		scanf("%d", &row);
		switch (row - 1) {
		case 0://Read txt
			proc = FileIO_fun(&proc);
			if (row_count == -1 || proc == 0) {
				printf("Error\n");
				return -1;
			}
			Read_check = 1;
			break;
		case 1://Generate
			break;
		case 2://FCFS
			mode = 1;
			break;
		case 3://SJF
			mode = 2;
			break;
		case 4://SRTF
			mode = 3;
			break;
		case 5://Priority
			mode = 4;
			break;
		case 6://RR
			mode = 5;
			break;
		case 7://exit
			return 0;
			break;
		default:
			printf("Wrong select\n");
			break;
		}
		if (Read_check != 1) {
			printf("Doesn't Read data from txt,Read txt frist\n");
		}
		else if(row != 1) {
			Schedular_fun(&proc, mode);
			proc = Proc_replace(&proc);
		}
	}
	
	return 0;
}
Process *Proc_replace(Process** proc) {
	Process tmp_p = { 0, };
	int i = 0, j = 0;
	for (i = 0; i < row_count; i++) {
		for (j = i; j < row_count; j++) {
			if ((*proc)[i].num > (*proc)[j].num) {
				tmp_p = (*proc)[j];
				(*proc)[j] = (*proc)[i];
				(*proc)[i] = tmp_p;
			}
		}
	}
	for (i = 0; i < row_count; i++) {
		(*proc)[i].remaind = (*proc)[i].BrustTime;
		(*proc)[i].save_point = 0;
		(*proc)[i].stat = 0;
		(*proc)[i].TAT = 0;
		(*proc)[i].wait = 0;
	}

	return *proc;
}
Process *Proc_refresh(Process **proc, int row) {
	(*proc)[row].ArriveTime = 0;
	(*proc)[row].BrustTime = 0;
	(*proc)[row].num = 0;
	(*proc)[row].Priority = 0;
	(*proc)[row].remaind = 0;
	(*proc)[row].save_point = 0;
	(*proc)[row].stat = 0;
	(*proc)[row].TAT = 0;
	(*proc)[row].wait = 0;

	return *proc;
}
Process *Proc_fun(Process **proc, int row) {
	(*proc) = (Process*)realloc(*proc, (row+1)*sizeof(Process));
	(*proc) = Proc_refresh(&(*proc), row);
	return *proc;
}
Process* FileIO_fun(Process **proc) {
	FILE *file;
	unsigned int i = 0, j = 0;
	int buf_size = 0, count = 0, place = 0;
	char *inp;
	int a = 0;
	file = fopen("C:\\Users\\SonJunHyeok\\Desktop\\Universty\\prco.txt","r");
	if (file == 0) {
		return 0;
	}
	
	fseek(file, 0, SEEK_END);
	buf_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	inp = (char*)malloc(sizeof(char)*(buf_size+1));
	printf("Process#   CPU_time   Arrival_time   Priority\n");
	printf("--------   --------   ------------   --------\n");
	while (fgets(inp, buf_size, file)) {//Read line
		for (i = 0; i < strlen(inp); i++) {
			if (inp[i] != ' ' && inp[i] != '\0' && inp[i] != '\n') {//data is exist
				switch (count) {
				case 0:
					(*proc)[place].num = atoi(&inp[i]);
					break;
				case 1:
					(*proc)[place].BrustTime = atoi(&inp[i]);
					if ((*proc)[place].BrustTime == 0) {
						printf("CPU time can't 0\n");
						return 0;
					}
					(*proc)[place].remaind = atoi(&inp[i]);
					break;
				case 2:
					(*proc)[place].ArriveTime = atoi(&inp[i]);
					break;
				case 3:
					(*proc)[place].Priority = atoi(&inp[i]);
					break;
				}
				count++;//next element
				for (i = i; i < strlen(inp); i++) {
					if (inp[i + 1] == ' ') {
						if (count == 4 && inp[i + 2] != '\0') {
							printf("proc.txt is wrong\n");
							return 0;
						}
						break;
					}
					else if (count > 1 && inp[i + 1] == '\0') {
						*proc = Proc_fun(&(*proc), place + 1);
						place++;
						break;
					}
				}
			}
		}
		count = 0;
	}
	for (i = 0; i < place; i++) {
		printf("  <%d>         <%d>         <%d>          <%d>\n", (*proc)[i].num, (*proc)[i].BrustTime, (*proc)[i].ArriveTime, (*proc)[i].Priority);
	}
	row_count = place;
	free(inp);
	fclose(file);

	return *proc;
}
Process* Arrive_fun(Process **proc) {
	int i = 0, j = 0, tmp_arr = 0;
	Process tmp_p = { 0, };
	for (j = 0; j < row_count; j++) {
		for (i = j+1; i < row_count; i++) {
			//tmp_p = &(*proc)[i];
			if ((*proc)[j].ArriveTime >= (*proc)[i].ArriveTime) {//arrive time compare
				if ((*proc)[j].ArriveTime == (*proc)[i].ArriveTime) {//arrive time same case
					if ((*proc)[j].Priority <= (*proc)[i].Priority) {//compare priority
						if ((*proc)[j].Priority == (*proc)[i].Priority) {//same priority
							if ((*proc)[j].num > (*proc)[i].num) {//compare process number
								tmp_p = (*proc)[i];
								(*proc)[i] = (*proc)[j];
								(*proc)[j] = (tmp_p);
							}
						}
					}
				}
				else {//arrive time not same
					tmp_p = (*proc)[i];
					(*proc)[i] = (*proc)[j];
					(*proc)[j] = (tmp_p);
				}
			}
		}
	}
	return *proc;
}
Process* Burst_fun(Process **proc) {
	int i = 0, j = 0, tmp_arr = 0;
	Process tmp_p = { 0, };

	for (j = 0; j < row_count - 1; j++) {
		for (i = j; i < row_count - 1; i++) {
			if ((*proc)[i].BrustTime >= (*proc)[i + 1].BrustTime) {//brust time compare
				if ((*proc)[i].BrustTime == (*proc)[i + 1].BrustTime) {//same brust time
					if ((*proc)[i].Priority <= (*proc)[i + 1].Priority) {//priority compare
						if ((*proc)[i].Priority == (*proc)[i + 1].Priority) {//same priority
							if ((*proc)[i].num > (*proc)[i].num) {//compare process number
								tmp_p = (*proc)[i];
								(*proc)[i] = (*proc)[j];
								(*proc)[j] = (tmp_p);
							}
						}
					}
				}
				else {//brust time not same
					tmp_p = (*proc)[i];
					(*proc)[i] = (*proc)[j];
					(*proc)[j] = (tmp_p);
				}
			}
		}
	}
	return *proc;
}
Process* SRTF_fun(Process **proc) {
	int i = 0, j = 0, RP = 0, time = 0, wp = 0, finish_pro = row_count, last_finish = 0;
	Process tmp_proc = { 0, };
	(*proc) = Arrive_fun(&(*proc));
	while (1) {
		if ((*proc)[RP].remaind == 0) {
			(*proc)[RP].stat = 2;//finish
			(*proc)[RP].TAT = time - (*proc)[RP].ArriveTime;
			(*proc)[RP].save_point = time;
			finish_pro--;
			last_finish = RP;
			RP = -1;
		}
		if (finish_pro == 0) {
			break;
		}
		if (RP == -1) {
			if (finish_pro == 1) {
				RP = j;
				(*proc)[RP].remaind--;
			}
			else if (finish_pro > 1) {
				j = A_Block(&(*proc), last_finish + 1);
				RP = j;
				if (RP == -1) {
					return 0;
				}
			}			
			else if((*proc)[j].ArriveTime <= time){
				(*proc)[RP].wait = time - (*proc)[RP].ArriveTime;
				(*proc)[RP].remaind--;
			}
			(*proc)[RP].stat = 1;//running
		}
		else if (!(RP == 0 && (*proc)[RP].ArriveTime > time)) {
			if (finish_pro > 1) {
				j = A_Block(&(*proc), RP + 1);
				if (j == -1) {
					return 0;
				}
			}
			if ((*proc)[RP].remaind > (*proc)[j].remaind) {
				wp++;
				(*proc)[RP].stat = 3;//stop
				(*proc)[RP].save_point = time;

				tmp_proc = (*proc)[j];
				(*proc)[j] = (*proc)[RP];
				(*proc)[RP] = tmp_proc;//stop process for changing
				if ((*proc)[RP].stat == 3) {
					wp--;
					(*proc)[RP].stat = 1;//ruunning
					(*proc)[RP].wait = time - (*proc)[RP].save_point;
				}
				else {
					(*proc)[RP].stat = 1;//running
					(*proc)[RP].wait = time - (*proc)[RP].ArriveTime;
				}
				(*proc)[RP].remaind--;
			}
			else {
				(*proc)[RP].remaind--;
			}
		}
		time++;
	}
	return *proc;
}
Process* Priority_fun(Process **proc) {
	int i = 0, j = 0, RP = 0, time = 0, wp = 0, finish_pro = row_count, last_finish = 0;
	Process tmp_proc = { 0, };

	(*proc) = Arrive_fun(&(*proc));
	while (1) {
		if ((*proc)[RP].remaind == 0) {
			(*proc)[RP].stat = 2;//finish
			(*proc)[RP].TAT = time - (*proc)[RP].ArriveTime;
			(*proc)[RP].save_point = time;
			finish_pro--;
			last_finish = RP;
			RP = -1;
		}
		if (finish_pro == 0) {
			break;
		}
		if (RP == -1) {
			if (finish_pro == 1) {
				RP = j;
				(*proc)[RP].remaind--;
			}
			else if (finish_pro > 1) {
				j = B_Block(&(*proc), last_finish + 1);
				RP = j;
				if (RP == -1) {
					return 0;
				}
			}
			else if ((*proc)[j].ArriveTime <= time) {
				(*proc)[RP].wait = time - (*proc)[RP].ArriveTime;
				(*proc)[RP].remaind--;
			}
			(*proc)[RP].stat = 1;//running
		}
		else if (!(RP == 0 && (*proc)[RP].ArriveTime > time)) {
			if (finish_pro > 1) {
				j = B_Block(&(*proc), RP + 1);
				if (j == -1) {
					return 0;
				}
			}
			if ((*proc)[RP].Priority > (*proc)[j].Priority) {
				wp++;
				(*proc)[RP].stat = 3;//stop(wait)
				(*proc)[RP].save_point = time;

				tmp_proc = (*proc)[j];
				(*proc)[j] = (*proc)[RP];
				(*proc)[RP] = tmp_proc;//stop process for changing
				if ((*proc)[RP].stat == 3) {
					wp--;
					(*proc)[RP].stat = 1;//ruunning
					(*proc)[RP].wait = time - (*proc)[RP].save_point;
				}
				else {
					(*proc)[RP].stat = 1;//running
					(*proc)[RP].wait = time - (*proc)[RP].ArriveTime;
				}
				(*proc)[RP].remaind--;
			}
			else {
				(*proc)[RP].remaind--;
			}
		}
		time++;
	}
	return *proc;
}
int A_Block(Process **proc, int next) {
	int i = 0, select = next;
	if (next >= row_count) {
		for (i = 0; i < row_count; i++) {
			if ((*proc)[i].stat == 3 || (*proc)[i].stat == 0) {
				next = i;
			}
		}
	}
	for (i = 0; i < row_count; i++) {
		if ((*proc)[i].stat != 2 && (*proc)[i].stat == 1) {
			if ((*proc)[i].remaind < (*proc)[select].BrustTime) {
				select = next;
			}
			else if((*proc)[i].remaind > (*proc)[select].BrustTime){
				continue;
			}
			else {
				if ((*proc)[i].Priority <= (*proc)[select].Priority) {
					if ((*proc)[i].Priority == (*proc)[select].Priority) {
						if ((*proc)[i].num < (*proc)[select].num) {//i select
							select = i;
						}
						else { //next select
							select = next;
						}
					}
				}
				else {
					select = i;
				}
			}
		}
	}
	return select;
}
int B_Block(Process **proc, int next) {
	int i = 0, select = next;
	if (next >= row_count) {
		for (i = 0; i < row_count; i++) {
			if ((*proc)[i].stat == 3 || (*proc)[i].stat == 0) {
				next = i;
			}
		}
	}
	select = next;
	for (i = 0; i < row_count; i++) {
		if ((*proc)[i].stat != 2 && (*proc)[i].stat != 1) {
			if ((*proc)[i].Priority <= (*proc)[select].Priority) {	
				if ((*proc)[i].Priority == (*proc)[select].Priority) {
					if ((*proc)[i].remaind < (*proc)[select].BrustTime) {
						select = next;
					}
					else if ((*proc)[i].remaind > (*proc)[select].BrustTime) {
						continue;
					}
					else if ((*proc)[i].num < (*proc)[select].num) {//i select
						select = i;
					}
				}
			}
			else {
				select = i;
			}
		}
	}
	return select;
}
Process* RoundRobin_fun(Process **proc) {
	int Q_size = 0, i =0, j=0,time = 0, finish_count = row_count;
	printf("Write Quantum size : ");
	scanf("%d", &Q_size);
	(*proc) = Arrive_fun(&(*proc));

	while (1) {
		if ((*proc)[j].ArriveTime <= time) {
			if (!((*proc)[j].remaind == (*proc)[j].BrustTime && (*proc)[j].num == 0)) {
				(*proc)[j].wait += time - (*proc)[j].save_point;
			}
			for (i = 0; i < Q_size; i++) {
				(*proc)[j].remaind--;
				time++;
				if ((*proc)[j].remaind == 0) {
					(*proc)[j].stat = 2;//finish
					(*proc)[j].TAT = time - (*proc)[j].ArriveTime;
					finish_count--;
					break;
				}
			}
			if ((*proc)[j].stat != 2) {
				(*proc)[j].save_point = time;
				(*proc)[j].stat = 3;//stop
			}
			j++;
			if (j >= row_count) {
				j = 0;
			}
			if (finish_count == 0) {
				break;
			}
		}
		time++;
	}

	return *proc;
}
void Schedular_fun(Process **proc, int mode) {
	int i = 0, max = 0, CPU_time = 0, waiting_time = 0;
	float turnaround_time = 0.0;

	switch (mode) {
	case 1://FCFS
		(*proc) = Arrive_fun(&(*proc));
		break;
	case 2://SJF
		(*proc) = Burst_fun(&(*proc));
		break;
	case 3://SRTF
		(*proc) = SRTF_fun(&(*proc));
		break;
	case 4://Priority
		(*proc) = Priority_fun(&(*proc));
		break;
	case 5://RR
		(*proc) = RoundRobin_fun(&(*proc));
		break;
	}
	printf("\n");
	if (mode == 1 || mode == 2) {
		for (i = 0; i < row_count; i++) {
			if (i != 0 && !(CPU_time < (*proc)[i].BrustTime)) {
				waiting_time += CPU_time - (*proc)[i].BrustTime;
			}
			//run
			if (CPU_time < (*proc)[i].ArriveTime) {
				CPU_time = ((*proc)[i].BrustTime + (*proc)[i].ArriveTime);
			}
			else {
				CPU_time = CPU_time + (*proc)[i].BrustTime;
			}
			turnaround_time = CPU_time - (*proc)[i].BrustTime;
		}
		printf("Waiting time : %d\n", waiting_time);
		printf("Average Turnaround time : %f\n", turnaround_time / (float)row_count);
	}
	else {
		for (i = 0; i < row_count; i++) {
			waiting_time += (*proc)[i].wait;
			turnaround_time += (*proc)[i].TAT;
		}
		printf("Waiting time : %d\n", waiting_time);
		printf("Average Turnaround time : %f\n", turnaround_time / (float)row_count);
	}
}

