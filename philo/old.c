
// /** Function ran by the watcher thread. */
// void	*watcher_function(void *arg)
// {
// 	t_overview		*overview;
// 	t_philosopher	*philos;

// 	// overview = (t_overview *)arg;
// 	philos = (t_philosopher *)arg;
// 	overview = (philos[0]).overview;
// 	// int	i;
// 	while (1)
// 	{
// 		// i = 0;
// 		// while (i <= overview->philo_count)
// 		// {
// 		// 	if (philo_death_check(&philos[i]) == true)
// 		// 		philo_die(overview, &philos[i]);
// 		// 	i++;
// 		// }

// 		pthread_mutex_lock(&overview->done_mutex);
// 		if (overview->done == true)
// 		{
// 			// pthread_mutex_lock(&overview->write_mutex);
// 			// printf("%d DEATH FOUND BY WATCHER!\n", get_time_msec() - overview->start_time_msec);
// 			// pthread_mutex_unlock(&overview->write_mutex);
// 			pthread_mutex_unlock(&overview->done_mutex);
// 			return (NULL) ;
// 		}
// 		pthread_mutex_unlock(&overview->done_mutex);
// 		// if (overview->done == true)
// 		// 	break;
// 		usleep(1000);
// 	}
// 	return (NULL);
// }

// /** Function ran by the watcher thread. */
// void	*watcher_function(void *arg)
// {
// 	t_overview		*overview;
// 	t_philosopher	*philos;
// 	// int				philo_count;

// 	philos = (t_philosopher *)arg;
// 	usleep(1000);
// 	overview = (philos[0]).overview;
// 	// philo_count = philos->overview->philo_count;
// 	int	i;
// 	while (1)
// 	{
// 		i = 0;
// 		while (i < overview->philo_count)
// 		{
// 			if (!(philos[i]).hungry)
// 			{
// 				print_with_lock(overview, &(philos[i]), "FOUND ATE ENOUGH IN WATCHER\n");
// 				pthread_mutex_lock(&(overview->done_mutex));
// 				overview->done = true;
// 				pthread_mutex_unlock(&(overview->done_mutex));
// 				return (NULL);
// 			}
// 			if (philo_death_check(&philos[i]) == true)
// 			{
// 				print_with_lock(overview, &(philos[i]), "FOUND DEAD IN WATCHER\n");
// 				return (philo_die(overview, &philos[i]));
// 			}
// 			i++;
// 		}

// 		// printf("watcher check: %i\n", philo_count);

// 		// pthread_mutex_lock(&overview->done_mutex);
// 		// if (overview->done == true)
// 		// {
// 		// 	// pthread_mutex_lock(&overview->write_mutex);
// 		// 	// printf("%d DEATH FOUND BY WATCHER!\n", get_time_msec() - overview->start_time_msec);
// 		// 	// pthread_mutex_unlock(&overview->write_mutex);
// 		// 	pthread_mutex_unlock(&overview->done_mutex);
// 		// 	return (NULL) ;
// 		// }
// 		// pthread_mutex_unlock(&overview->done_mutex);
// 		// if (overview->done == true)
// 		// 	break;
// 	}
// 	return (NULL);
// }


// grab fork function with left or right diff
bool philo_grab(t_overview *overview, t_philosopher *philo)
{
	if (philo->philo_nbr % 2 == 0)
	{
		if (!check_keep_going(overview))
			return (false);
		pthread_mutex_lock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// if (!check_keep_going(overview))
		// 	return (false);
		print_with_lock(overview, philo, MSG_FORK);
		// if (philo_death_check(philo))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		if (!check_keep_going(overview))
		{
			pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
			return (false);
		}
		pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		// if (philo_death_check(philo))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		// if (!check_keep_going(overview))
		// {
		// 	pthread_mutex_unlock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		// 	return (false);
		// }
		print_with_lock(overview, philo, MSG_FORK);
	}
	else
	{
		if (!check_keep_going(overview))
			return (false);
		// if (philo_death_check(philo))
		// 	return (false);
		pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		print_with_lock(overview, philo, MSG_FORK);
		// if (philo_death_check(philo))
		// {
		// 	pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
		// 	return (false);
		// }
		if (!check_keep_going(overview))
		{
			pthread_mutex_lock(&overview->forks[philo->philo_nbr - 1]);
			return (false);
		}
		pthread_mutex_lock(&overview->forks[philo->philo_nbr % overview->philo_count]);
		print_with_lock(overview, philo, MSG_FORK);
	}
	return (true);
}


// ITOA CHECK
#include <stdlib.h> // for malloc()

static int	ft_nbr_size(long n)
{
	int	digit_count;

	digit_count = 1;
	if (n < 0)
	{
		n = n * -1;
		digit_count++;
	}
	while (n >= 10)
	{
		n = n / 10;
		digit_count++;
	}
	return (digit_count);
}

static int	ft_power(int base, int power)
{
	int	result;

	result = 1;
	while (power > 0)
	{
		result = result * base;
		power--;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	char	*nbr_str;
	int		i;
	int		digit_count;
	int		digit;
	long	n_long;

	n_long = (long)n;
	digit_count = ft_nbr_size(n_long);
	nbr_str = malloc((digit_count + 1) * sizeof(char));
	if (nbr_str == NULL)
		return (NULL);
	i = 0;
	if (n_long < 0)
	{
		n_long = n_long * -1;
		nbr_str[i++] = '-';
	}
	while (i < digit_count)
	{
		digit = n_long / ft_power(10, (digit_count - 1 - i));
		n_long = n_long - digit * ft_power(10, (digit_count - 1 - i));
		nbr_str[i++] = digit + 48;
	}
	nbr_str[i] = '\0';
	return (nbr_str);
}