/**
 * Scheduler Lab
 * CS 241 - Spring 2016
 */
/** @file libscheduler.c
 */
#include "libscheduler.h"

priqueue_t pqueue;
core_t *cores;
scheme_t scheme;
static int num_core;
static int cores_used;

static int job_finished;
static float total_wait_time;
static float total_turnaround_time;
static float total_response_time;

//no need to change
int comparer_rr(const void *a, const void *b) { return 1; }

//need to change
int comparer_fcfs(const void *a, const void *b) { 
	//done by arrival time
	job_t* job_a = (job_t*) a;
	job_t* job_b = (job_t*) b;
	if(job_a->arrival_time > job_b->arrival_time){
		return 1;
	}else if(job_a->arrival_time == job_b->arrival_time){
		return 0;
	}

	return -1; 
}
int comparer_pri(const void *a, const void *b) { 
	//done by priority
	job_t* job_a = (job_t*) a;
	job_t* job_b = (job_t*) b;
	if(job_a->priority > job_b->priority){
		return 1;
	}else if(job_a->priority == job_b->priority){
		return comparer_fcfs(a, b);
	}
	
	return -1; 
}
int comparer_psjf(const void *a, const void *b) { 
	//done by remaining time
	job_t* job_a = (job_t*) a;
	job_t* job_b = (job_t*) b;
	if(job_a->remain_time > job_b->remain_time){
		return 1;
	}else if(job_a->remain_time == job_b->remain_time){
		return comparer_fcfs(a, b);
	}
	return -1; 
}

/**
  Initalizes the scheduler.

  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param numcores the number of cores that is available by the scheduler. These
  cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param s  the scheduling scheme that should be used. This value will be one of
  the six enum values of scheme_t
*/
void scheduler_start_up(int numcores, scheme_t s) {
  if (s == FCFS){
    priqueue_init(&pqueue, comparer_fcfs);
  }
  else if (s == PRI){
    priqueue_init(&pqueue, comparer_pri);
  }
  else if (s == PSJF){
    priqueue_init(&pqueue, comparer_psjf);
  }
  else if (s == RR){
    priqueue_init(&pqueue, comparer_rr);
  }
  else
    exit(1);
  scheme = s;
  num_core = numcores;
	job_finished = 0;
  cores = malloc(sizeof(core_t) * numcores);
  for(int i = 0; i < numcores; i++){
    cores[i].free = 1;
    cores[i].job = NULL;
  }
}

/**
  Called when a new job arrives.

  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it
  will be finished.
  @param priority the priority of the job. (The lower the value, the higher the
  priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made.

 */
int scheduler_new_job(int job_number, int time, int running_time,
                      int priority) {
  job_t* new_job = malloc(sizeof(job_t));
  new_job->id = job_number;
  new_job->priority = priority;
	new_job->arrival_time = time;
	new_job->start_time = -1;
	new_job->update_time = time;
	new_job->remain_time = running_time;
	new_job->running_time = running_time;

	if(num_core > cores_used){
		for(int i = 0; i < num_core; i ++){
			if(cores[i].free){
				cores[i].job = new_job;
				cores_used++;
				cores[i].free = 0;
				new_job->start_time = time;
				return i;
			}
		}		
	}else{	
  	switch(scheme){

  	  case(FCFS):{
				priqueue_offer(&pqueue, new_job);
				return -1;
  	  }break;
  	  case(PRI):{
				priqueue_offer(&pqueue, new_job);
				return -1;
  	  }break;
  	  case(PSJF):{
				int worst_time_index = 0;
				int worst_time = 0;
				for(int i = 0; i < num_core; i++){
					cores[i].job->remain_time -= time - cores[i].job->update_time;
					cores[i].job->update_time = time;
					if(worst_time < cores[i].job->remain_time){
						worst_time = cores[i].job->remain_time;
						worst_time_index = i;
					}
					if(cores[worst_time_index].job->remain_time > new_job->remain_time){
						priqueue_offer(&pqueue, cores[worst_time_index].job);
						cores[worst_time_index].job = new_job;
						new_job->start_time = time;
						return worst_time_index;	
					}else{
						priqueue_offer(&pqueue, new_job);
					}
				}
  	  }break;
  	  case(RR):{
				priqueue_offer(&pqueue, new_job);
				return -1;
  	  }break;
  	  default:{
  	    fprintf(stderr, "scheme is not correct\n");
  	    exit(1);
  	  }break;
  	}
 	} 
  return -1;
}

/**
  Called when a job has completed execution.

  The core_id, job_number and time parameters are provided for convenience. You
  may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.

  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time) {
	
	job_t* finished_job = cores[core_id].job;
	cores[core_id].job = priqueue_poll(&pqueue);
	job_finished++;
	total_wait_time += time - finished_job->arrival_time - finished_job->running_time;
 	total_response_time += finished_job->start_time - finished_job->arrival_time;
	total_turnaround_time += time - finished_job->arrival_time;	
	
	if(cores[core_id].job != NULL){
		if(cores[core_id].job->start_time == -1)
			cores[core_id].job->start_time = time;
		cores[core_id].job->update_time = time;
		cores[core_id].free = 0;
		return cores[core_id].job->id;
	}else{
		cores_used--;
		cores[core_id].free = 1;
	}

	return -1;
}

/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.

  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time) {
	priqueue_offer(&pqueue, cores[core_id].job);
	job_t* new_job = priqueue_poll(&pqueue);
	cores[core_id].job = new_job;
	if(new_job != NULL){
		if(new_job->start_time == -1){
			new_job->start_time = time;
		}
		return new_job->id;
	}
	cores[core_id].free = 1;
	return -1;
}

/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all
  jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time() {
	return total_wait_time/job_finished;
}

/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all
  jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time() {
	return total_turnaround_time/job_finished;
}

/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all
  jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time() {
	return total_response_time/job_finished;
}

/**
  Free any memory associated with your scheduler.

  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up() {
  /* Do more cleaning up here */
  priqueue_destroy(&pqueue);
  free(cores);
}

/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in
  the order they are to be scheduled. Furthermore, we have also listed the
  current state of the job (either running on a given core or idle). For
  example, if we have a non-preemptive algorithm and job(id=4) has began
  running, job(id=2) arrives with a higher priority, and job(id=1) arrives with
  a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)

  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue() {
  // This function is left entirely to you! Totally optional.
}
