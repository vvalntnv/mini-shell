#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>

Job *job_map = NULL;
unsigned int jobs_count = 0;
unsigned int map_size = 0;

const char *JobTypeStrings[] = {"RUNNING", "STOPPED"};

const int JOB_STATUS_COUNT = 2;

enum {
  JOB_OK = 0,
  JOB_FULL = 1,
  JOB_OOM = 2,
  JOB_IDINV = 3,
  JOB_INVSTATUS = 4
};

void list_jobs() {
  printf("-------JOBS---------\n");
  // foreach over all jobs and then display them and their status

  if (job_map == NULL || jobs_count == 0) {
    goto end_statement;
  }

  for (unsigned int i = 0; i < jobs_count; i++) {
    const char *current_status_verbose;
    Job *current_job = &job_map[i];
    current_status_verbose = JobTypeStrings[current_job->status];

    printf("[%s] %u - %s\n", current_status_verbose, current_job->job_id,
           current_job->command);
  }

end_statement:
  printf("---------------------\n");
}

/**
 * Registers a job into the job table
 * It checks if there are available spaces in the job tables and adds one
 *
 * If no space is available, the function returns 1, if everything is ok, the
 * func returns 0, and if an error occured, it returns -1
 */
int register_job(const Job *job) {
  if (jobs_count >= MAX_JOBS_COUNT) {
    return JOB_FULL;
  }

  job_id_t job_id = jobs_count;

  if (jobs_count == map_size) {
    unsigned int new_cap = map_size ? map_size * 2 : INITIAL_SIZE;
    if (new_cap > MAX_JOBS_COUNT) {
      new_cap = MAX_JOBS_COUNT;
    }

    Job *temp_pointer = realloc(job_map, sizeof(Job) * new_cap);

    if (temp_pointer == NULL) {
      perror("Failed to reallocate");
      return JOB_OOM;
    }

    map_size = new_cap;
    job_map = temp_pointer;
  }
  job_map[job_id] = *job;
  job_map[job_id].job_id = job_id;
  jobs_count++;

  return JOB_OK;
}

int change_job_status(job_id_t job_id, JobStatus status) {
  if (!job_map || job_id >= jobs_count) {
    return JOB_IDINV;
  }

  if (status < 0) {
    return JOB_INVSTATUS;
  }

  job_map[job_id].status = status;

  return JOB_OK;
}
