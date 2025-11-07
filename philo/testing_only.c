#include "philo.h"
#include <stdio.h>

/** For testing only: prints philo struct. */
void print_overview(t_overview *overview)
{
	printf("philocount: %i\ntime_to_die: %i\ntime_to_eat: %i\n", 
		overview->philo_count, overview->time_to_die, overview->time_to_eat);
	printf("time_to_sleep: %i\namount_to_eat: %i\n", overview->time_to_sleep,
		overview->amount_to_eat);
	printf("start time in msec: %d\n", overview->start_time_msec);
}
