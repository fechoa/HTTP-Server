/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zefir <zefir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:19:00 by zefir             #+#    #+#             */
/*   Updated: 2022/03/17 22:11:27 by zefir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include <signal.h>
# include <map>
# include <sys/select.h>
# include "Server.hpp"
# include "Client.hpp"
# include "Config.hpp"

class Program {
private:
	Config					config;
	std::vector<Server>		servers;
	size_t					countServers;
	int						maxFd;
	fd_set					sFd;
	fd_set					writefds;
	fd_set					readfds;
	std::vector<Client>		clientList;
	std::vector<Client>		clientListWait;	


public:
	Program();
	Program(const Program& cp);
	~Program();
	Program& operator=(const Program& cp);

	int		initProgram(const std::string& configFile);
	int		initServers();
	void	preparationSet();

	int		start();
	int		deleteWaitingFd(std::vector<Client>::iterator& rmClient, bool flag);
	void	clientWorking(const Client& client);	
	void	turnOff();


};

#endif
