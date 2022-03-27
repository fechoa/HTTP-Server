/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:19:07 by zefir             #+#    #+#             */
/*   Updated: 2022/03/21 13:43:15 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Program.hpp"

Program::Program() : maxFd(0) {}

Program::~Program() {}

Program::Program(const Program& cp) {
	*this = cp;
}


Program& Program::operator=(const Program& cp) {
	if (this != &cp) {
		servers = cp.servers;
		config = cp.config;
		clientList = cp.clientList;
		clientListWait = cp.clientListWait;
		sFd = cp.sFd;
		maxFd = cp.maxFd;
		countServers = cp.countServers;
	}
	return (*this);
}

int	Program::initServers() {
	FD_ZERO(&sFd);
	for (size_t i = 0; i < countServers; i++) {
		int check = servers[i].create(i);
		if (check > 0)
			return (1); // Error init server (need finish all launch services)
		maxFd = servers[i].getFd() > maxFd ? servers[i].getFd() : maxFd;
		FD_SET(servers[i].getFd(), &sFd);
	}
	return (0);
}

int	Program::initProgram(const std::string& configFile) {
	config.parsing(configFile);
	servers = config.getServers();
	countServers = servers.size();
	return (initServers());
}

void	Program::preparationSet() {
	readfds = sFd;
	FD_ZERO(&writefds);
	std::vector<Client>::iterator it = clientListWait.begin();
	while (it != clientListWait.end())
	{
		FD_SET(it->getSock(), &writefds);
		it++;
	}
}

int	Program::start() {
	while(true)
	{
		preparationSet();

		int res = select(maxFd + 1, &readfds, &writefds, 0, 0);
		if (res < 0) exit(EXIT_FAILURE); // Error with set Sets
		
		std::vector<Client>::iterator it = clientListWait.begin();
		for(; it != clientListWait.end(); ++it) {
			if (FD_ISSET(it->getSock(), &writefds)) { // Check inside socket to writeSET
				if (it->sendUp() == false) // Check error Send message
					deleteWaitingFd(it, true); // Operations delete socket and its the pieces
				else if (it->getAllWrote() == true && deleteWaitingFd(it, false) == 1) // Check end writing ( Delete and set finish read)
					return (1);
			}
			else // Lost socket in writing Set (because delete it)
				if (deleteWaitingFd(it, true) == 1) return (1);
		}

		for (int i = 0; i < countServers; i++) {
			if (FD_ISSET(servers[i].getFd(), &readfds)) {
				int sock = servers[i].acceptConnect();
				if (sock < 0) return (1); // Error accept connect
				FD_SET(sock, &sFd);
				maxFd = maxFd > sock ? maxFd : sock; // Update maxfd
				clientList.push_back(Client(sock, i)); // Add new fd to set fds
			}
		}

		it = clientList.begin();
		while (it != clientList.end()) {
			if (FD_ISSET(it->getSock(), &readfds)) { // Check inside socket to readSET (copy mainset)
				if (it->recvUp() == false) {// Check error Read message
					if (close(it->getSock()) < 0) return (1); // Error close
					FD_CLR(it->getSock(), &sFd);
					it = clientList.erase(it);
					it--;
				}
				else if (it->getAllRead() == true) {
					clientWorking(*it);
					it->clearBuffer();
				}
			}
			it++;
		}
	}

	return (0);
}

void	Program::clientWorking(const Client& client) {
	std::string res = servers[client.getServerNum()].getResponse(client.getSock(), client);

	Client clientWait = client;
	clientWait.setDeliverBytes(res.size());
	clientWait.setAnswer(res);
	clientListWait.push_back(clientWait); // Add list waiting client
}

int	Program::deleteWaitingFd(std::vector<Client>::iterator& rmClient, bool flag) {
	if (flag)
	{
		if (close(rmClient->getSock()) < 0)
			return (1); // Error close
		FD_CLR(rmClient->getSock(), &sFd);
	}
	std::vector<Client>::iterator	it = clientList.begin();
	while (it != clientList.end() && it->getSock() != rmClient->getSock()) it++;
		if (it != clientList.end()) it->setAllRead(false); // Set finish read connection (for delete client)
	rmClient = clientListWait.erase(rmClient);
	--rmClient;
	return (0);
}

void	Program::turnOff() {
	for(size_t i = 0; i < servers.size(); i++)
		close(servers[i].getFd());
	std::vector<Client>::iterator it = clientListWait.begin();	
	while (it != clientListWait.end()) {
		close(it->getSock());
		it++;	
	}
	it = clientList.begin();
	while (it != clientListWait.end()) {
		close(it->getSock());
		it++;	
	}
}
