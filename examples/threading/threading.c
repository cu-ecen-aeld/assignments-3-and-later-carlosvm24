#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
	if (thread_param != NULL)
	{
		struct thread_data *thread_func_args = (struct thread_data *)thread_param;
		
		DEBUG_LOG("Entering thread, wait for %d ms\n", thread_func_args->thread_timer_ms);
		usleep(thread_func_args->thread_timer_ms);
		
		int rc = pthread_mutex_lock(thread_func_args->mutex);
		
		if (rc != 0)
		{
			ERROR_LOG("pthread_mutex_lock failed with %d\n", rc);
			thread_func_args->thread_complete_success = false;
		}
		else
		{
			DEBUG_LOG("Entering mutex, wait for %d ms\n", thread_func_args->mutex_timer_ms);
			usleep(thread_func_args->mutex_timer_ms);
			
			rc = pthread_mutex_unlock(thread_func_args->mutex);
			DEBUG_LOG("Leaving mutex, return with %d\n", rc);
			
			if (rc != 0)
			{
				ERROR_LOG("pthread_mutex_unlock failed with %d\n", rc);
				thread_func_args->thread_complete_success = false;
			}
		}
		
		if (rc == 0)
		{
			thread_func_args->thread_complete_success = true;
		}
	}
	
	return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
	bool ret = true;
	int rc = 0;
	struct thread_data *thread_param = (struct thread_data *)malloc(sizeof(struct thread_data));
	
	if (thread_param == NULL)
	{
		ERROR_LOG("Attempt to allocate thread structure failed\n");
		ret = false;
	}
	else
	{
		thread_param->thread_timer_ms = wait_to_obtain_ms;
		thread_param->mutex_timer_ms = wait_to_release_ms;
		thread_param->mutex = mutex;
		thread_param->thread_complete_success = false;
	}
	
	if (ret)
	{
		DEBUG_LOG("Creating thread, thread_id %ld\n", *thread);
		rc = pthread_create(thread, NULL, threadfunc, thread_param);
	
		if (rc != 0)
		{
			ERROR_LOG("Attempt to pthread_create thread %ld failed with %d\n", *thread, rc);
			ret = false;
		}
	}

	return ret;
}

