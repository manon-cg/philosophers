/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 10:39:49 by mgroos            #+#    #+#             */
/*   Updated: 2025/11/07 10:53:43 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> // for printf()
#include <sys/time.h> // for gettimeofday
#include <pthread.h>

// void	*ft_calloc(size_t nmemb, size_t size)
// {
// 	char	*array;

// 	if (nmemb > SIZE_MAX / size)
// 		return (NULL);
// 	array = malloc(size * nmemb);
// 	if (array == NULL)
// 		return (NULL);
// 	ft_bzero_gnl(array, nmemb * size);
// 	return (array);
// }

/* returns length of string s */
size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/* Less lenient version of atoi: negative numbers are not allowed. Invalid
string input returns -1. */
int	philo_atoi(const char *nptr)
{
	int			i;
	long long	number;

	i = 0;
	number = 0;
	if (nptr[i] == '-')
		return (-1);
	while (nptr[i] == '0')
		i++;
	if (ft_strlen(&(nptr[i])) > 10)
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		number = number * 10 + (nptr[i] - 48);
		i++;
	}
	if (nptr[i] || number > 2147483647 || number < -2147483648)
		return (-1);
	return ((int)number);
}

unsigned int	get_time_msec(void)
{
	struct timeval	time_struct;

	if (gettimeofday(&time_struct, NULL) != -1)
		return (time_struct.tv_usec / 1000 + time_struct.tv_sec * 1000);
	else
		return (printf("get time of day fail\n"), 0); // 0 is not safe ? hm.
	
}

void	print_with_lock(t_overview *overview, char *msg)
{
	pthread_mutex_lock(overview->write_mutex);
	// decide if im getting time here or doing that & storing it somewhere earlier
	// so i just pass it to the func as an extra var
	printf("[time] [philo] %s\n", msg);
	pthread_mutex_unlock(overview->write_mutex);
}

// /* Less lenient version of atoi; a single - at the start is allowed, 
// 	every other char needs to be a number. changes the 'valid' pointer 
// 	to -1 upon invalid input */
// int	philo_atoi(const char *nptr, int *valid)
// {
// 	int			i;
// 	long long	number;

// 	i = 0;
// 	number = 0;
// 	if (nptr[i] == '-')
// 	{
// 		*valid = -1;
// 		return (0);
// 	}
// 	while (nptr[i] == '0')
// 		i++;
// 	if (ft_strlen(&(nptr[i])) > 10)
// 		*valid = -1;
// 	while (nptr[i] >= '0' && nptr[i] <= '9')
// 	{
// 		number = number * 10 + (nptr[i] - 48);
// 		i++;
// 	}
// 	if (nptr[i] || number > 2147483647 || number < -2147483648)
// 		*valid = -1;
// 	return ((int)number);
// }
