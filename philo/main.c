/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/04 14:57:39 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <sys/time.h> // for gettimeofday
#include <stddef.h> // for size_t
#include <pthread.h>  // for threading

void	*philo_function(void *arg)
{
	(void)arg;
	while (1)
	{
		printf("Running...\n");
		usleep(1000);
	}
	return (NULL);
}

void	arg_error(void)
{
	// use fdputstr to put this on stderr instead?

	printf("Make sure to structure your input as follows:\n");
	printf("./philo [number_of_philosophers] [time_to_die] [time_to_eat] \
[time_to_sleep] [OPTIONAL: number_of_times_each_philosopher_must_eat]\n");
}

/** Takes the arguments provided by the user and stores them in the overview
 * struct.
 */
bool	handle_args(int argc, char *argv[], t_overview *overview)
{
	int			valid;

	if (argc < 5 || argc > 6)
		return (arg_error(), false);
	valid = 1;
	overview->philo_count = philo_atoi(argv[1], &valid);
	overview->time_to_die = philo_atoi(argv[2], &valid);
	overview->time_to_eat = philo_atoi(argv[3], &valid);
	overview->time_to_sleep = philo_atoi(argv[4], &valid);
	overview->amount_to_eat = 0; // mayb not necessary if im not looking at it
	overview->end_point_given = false;	
	if (argc > 5)
	{
		overview->amount_to_eat = philo_atoi(argv[5], &valid);
		overview->end_point_given = true;	
	}	
	if ((valid) == -1)
	{
		free(overview);
		return (printf("Make sure all input is valid ints\n"), false);
	}
	return (true);
}

/** For testing only: prints philo struct. */
void print_overview(t_overview *overview)
{
	printf("philocount: %i\ntime_to_die: %i\ntime_to_eat: %i\n", 
		overview->philo_count, overview->time_to_die, overview->time_to_eat);
	printf("time_to_sleep: %i\namount_to_eat: %i\n", overview->time_to_sleep,
		overview->amount_to_eat);
}

void	create_philos(t_overview *overview)
{
	int	i;
	pthread_t thread;

	i = 0;
	// while (i < overview->philo_count)
	// {
	// 	pthread_create(&thread, NULL, philo_function, NULL);
	// 	i++;
	// }
	(void)overview;
	pthread_create(&thread, NULL, philo_function, NULL);
}

int	main(int argc, char *argv[])
{
	t_overview		overview;
	struct timeval	time_struct;

	printf("going to handle args:\n");
	if (handle_args(argc, argv, &overview))
	{
		print_overview(&overview);
	}
	if (gettimeofday(&time_struct, NULL) != -1)
		overview.start_time = time_struct.tv_usec;
	else
		return(printf("get time of day fail\n"), EXIT_FAILURE);
	create_philos(&overview);

	printf("TIME in usec?? since when???: %ld\n", time_struct.tv_usec);
	
}
