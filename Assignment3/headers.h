struct PCB
{
		char program_name[30];
		int PID;
		char status[30];
		int priority;
		int arrival_time;
		int interruptable;
		int est_runtime;
		int est_remaining_runtime;
		int wait_time;
		int turnaround_time;
		int context_switches;
};