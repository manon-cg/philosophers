/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 10:39:49 by mgroos            #+#    #+#             */
/*   Updated: 2025/10/31 11:36:40 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // for printf()

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

/* Less lenient version of atoi; a single - at the start is allowed, 
	every other char needs to be a number. changes the 'valid' pointer 
	to -1 upon invalid input */
int	philo_atoi(const char *nptr, int *valid)
{
	int			i;
	long long	number;

	i = 0;
	number = 0;
	if (nptr[i] == '-')
	{
		printf("Only use positive numbers!\n");
		*valid = -1;
		return (0);
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
	return ((int)number);
}
