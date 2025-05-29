#include <stdint.h>

#ifndef JOBS_H
#define JOBS_H
#define MAX_JOBS_COUNT 100
#define INITIAL_SIZE 10

#include <signal.h>

typedef uint8_t job_id_t;

typedef enum JobType {
  JOB_RUNNING,
  JOB_STOPPED
} JobStatus;

extern const char* JobTypeStrings[];
extern const int JOB_STATUS_COUNT; 

typedef struct Job {
  job_id_t job_id;
  pid_t pgid;
  char *command;
  JobStatus status;
} Job;

void list_jobs();
int register_job(const Job *job);
int change_job_status(job_id_t job_id, JobStatus status);

void free_map();

extern Job *job_map;
extern unsigned int jobs_count;
#endif
