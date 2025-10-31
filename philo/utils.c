/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 10:39:49 by mgroos            #+#    #+#             */
/*   Updated: 2025/10/31 10:41:39 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // for size_t


/* returns length of string s */
size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

// change this so it doesn't allow any negative numbers
/* Less lenient version of atoi; a single - at the start is allowed, 
	every other char needs to be a number. changes the 'valid' pointer 
	to -1 upon invalid input */
int	philo_atoi(const char *nptr, int *valid)
{
	// pushswap version, see if i wanna edit anyting
	int			i;
	long long	number;
	int			sign;

	i = 0;
	number = 0;
	sign = 1;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (nptr[i] == '0')
		i++;
	if (ft_strlen(&(nptr[i])) > 10)
		*valid = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		number = number * 10 + (nptr[i] - 48);
		i++;
	}
	if (nptr[i] || number > 2147483647 || number < -2147483648)
		*valid = -1;
	return ((int)number * sign);
}
