/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgroos <mgroos@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:00:22 by mgroos            #+#    #+#             */
/*   Updated: 2025/10/31 11:24:09 by mgroos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** Sleep for the amount of time provided by the user. */
void	philo_sleep(t_overview *overview)
{
	// mutex
	printf("[time] [philo] %s\n", MSG_SLEEP);
	usleep(overview->time_to_sleep);
}

// eat function

// grab fork function

// think function



/** Handle philosopher death by waiting for the other philosophers to 
 * stop eating/sleeping, and then printing the death message.
 */
void	philo_die(void) // pass a name variable or whatever
{
	usleep(1000);
	// lock & open mutexes ? idk
	printf("[timestamp] [philo nbr] %s\n", MSG_DIE);
}
