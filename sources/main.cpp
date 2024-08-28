/* ************************************************************************** */
/*                                                        :::      ::::::::   */
/*   Webserv                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arincon and ddania-c                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04                               #+#    #+#             */
/*   Finished: 2024/06/??                             ###   ########.fr       */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Server.hpp"

int main(int argc, char **argv, char **envp)
{
	if (argc != 2)
	{
		std::cerr << RED << "this program take 1 argument" << RES << std::endl;
		return 1;
	}

	Server *server = new Server(argv[1], envp);
	delete server;

	return 0;
}

/* TODO

• The first server for a host:port will be the default for this host:port (that means
it will answer to all the requests that don’t belong to an other server).

+Site 2

• Manage all the errors.
*/
