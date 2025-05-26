#ifndef JOBS_H
#define JOBS_H

#include <signal.h>
typedef enum JobType {
  JOB_RUNNING,
  JOB_STOPPED
} JobStatus;

const char* JobTypeStrings[] = {
  "RUNNING",
  "STOPPED"
};

const int JOB_STATUS_COUNT = 2; 

typedef struct Job {
  int job_id;
  pid_t pgid;
  char *command;
  JobStatus status;
} Job;

void list_jobs(Job *job_map, int job_count);
Job *register_job(Job job, Job *current_job_map, int *job_count);

void free_map(Job *jobs_map, int jobs_count);
#endif
