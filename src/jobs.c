#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>

void list_jobs(Job *job_map, int job_count) {
  if (job_map == NULL) {
    return;
  }

  printf("---------JOBS---------\n");
  for (int i = 0; i < job_count; i++) {
    Job *current_job = &job_map[i];

    const char *job_status_verbose;
    if (current_job->status >= 0 && current_job->status < JOB_STATUS_COUNT) {
      job_status_verbose = JobTypeStrings[current_job->status];
    } else {
      job_status_verbose = "UNKNOWN";
    }

    printf("[%d] %s - %s\n", current_job->job_id, job_status_verbose,
           current_job->command);
  }

  printf("---------END-----------\n");
}

Job *register_job(Job job, Job *current_job_map, int *job_count) {
  if (job_count == NULL) {
    fprintf(stderr, "The jobs count cannot be NULL\n");
    return NULL;
  }

  if (current_job_map == NULL && *job_count != 0) {
    fprintf(stderr, "current_job_map is NULL, but size is more than 0!\n");
    return NULL;
  }

  int new_count = (*job_count) + 1;
  Job *new_map = (Job *)realloc(current_job_map, (sizeof(Job) * new_count));

  if (new_map == NULL) {
    perror("register_job: FAILED ALLOCATION\n");
    return NULL;
  }

  new_map[new_count - 1] = job;
  *job_count = new_count;
  return new_map;
}

void free_map(Job *jobs_map, int jobs_count) {
  if (jobs_map == NULL) {
    return;
  }

  for (int i = 0; i < jobs_count; i++) {
    free(jobs_map[i].command);
  }

  free(jobs_map);
}
