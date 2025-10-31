/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:39:04 by mgroos            #+#    #+#             */
/*   Updated: 2025/10/31 10:42:59 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc/free
#include <sys/time.h> // for gettimeofday
#include <stddef.h> // for size_t

void	arg_error(void)
{
	// use fdputstr to put this on stderr instead?

	printf("Make sure to structure your input as follows:\n");
	printf("./philo [number_of_philosophers] [time_to_die] [time_to_eat] \
[time_to_sleep] [OPTIONAL: number_of_times_each_philosopher_must_eat]\n");
}

bool	handle_args(int argc, char *argv[], t_overview *overview_struct)
{
	int			valid;

	if (argc < 5 || argc > 6)
		return (arg_error(), false);
	valid = 1;
	overview_struct->philo_count = philo_atoi(argv[1], &valid);
	overview_struct->time_to_die = philo_atoi(argv[2], &valid);
	overview_struct->time_to_eat = philo_atoi(argv[3], &valid);
	overview_struct->time_to_sleep = philo_atoi(argv[4], &valid);

	if (argc > 5)
	{
		overview_struct->amount_to_eat = philo_atoi(argv[5], &valid);
		overview_struct->end_point_given = true;	
	}	
	if ((valid) == -1)
	{
		free(overview_struct);
		return (printf("Make sure all input is valid ints\n"), false);
	}
	return (true);
}

// print for testing
void print_overview(t_overview *overview)
{
	printf("philocount: %i\ntime_to_die: %i\ntime_to_eat: %i\n", 
		overview->philo_count, overview->time_to_die, overview->time_to_eat);
	printf("time_to_sleep: %i\namount_to_eat: %i\n", overview->time_to_sleep,
		overview->amount_to_eat);
}	

int	main(int argc, char *argv[])
{
	t_overview	*overview_struct;
	struct timeval tv;

	overview_struct = malloc(sizeof(t_overview)) ; // calloc !
	if (overview_struct == NULL)
		return (EXIT_FAILURE); // calloc fail!

	printf("going to handle args:\n");
	if (handle_args(argc, argv, overview_struct))
	{
		print_overview(overview_struct);
		free(overview_struct);
	}

	gettimeofday(&tv, NULL);
	
	printf("TIME in usec?? since when???: %ld\n", tv.tv_usec);

		
	

	
}