/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 13:37:14 by zefir             #+#    #+#             */
/*   Updated: 2022/03/19 21:12:24 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Server.hpp"

Server::Server() {}
Server::~Server() {}

Server::Server(const Server& cp) {
	*this = cp;
}

Server& Server::operator=(const Server& cp) {
	if (this != &cp) {
		fd = cp.fd;
		listenStr = cp.listenStr;
		locationList = cp.locationList;
		locationListExt = cp.locationListExt;
		addr = cp.addr;
		ip = cp.ip;
		port = cp.port;
		numberServer = cp.numberServer;
	}
	return (*this);
}

Server::Server(const std::string& listen,
			const std::vector<Loc>& locationList,
			const std::vector<Loc>& locationListExt)
: listenStr(listen), locationList(locationList), locationListExt(locationListExt) {}

int	Server::create(size_t num) {
	initParams(num);
	return (initConnect());
}

void	Server::initParams(size_t num) {
	numberServer = num;
	ip = listenStr.substr(0, listenStr.find(":"));
	port = listenStr.substr(listenStr.find(":") + 1, listenStr.size());
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	int check = inet_aton(ip.c_str(), &addr.sin_addr);
	if (check == 0) exit(EXIT_FAILURE); // Error convertation address
	addr.sin_port = htons(atoi(port.c_str()));
}

int	Server::initConnect() {
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) return (1); // Error open socket
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int)) < 0) return(1); // Error set option
	if ((bind(fd, (const struct sockaddr *)&addr, sizeof(addr))) < 0) {
		if (close(fd) < 0) exit(EXIT_FAILURE); // Error close socket
		return (1); // Error bind address for socket
	}
	if ((listen(fd, 10000)) < 0) return (1); // Error listen for socket
	return (0);
}

int	Server::acceptConnect() {
	struct sockaddr_in tmpAddr;
	unsigned int addrLen = sizeof(tmpAddr);
	memset(&tmpAddr, 0, sizeof(addr));
	int sock = accept(fd, (struct sockaddr *)&tmpAddr, (socklen_t *)&addrLen);
	if (sock < 0) return (-1); // Error failure accept connect
	fcntl(sock, F_SETFL, O_NONBLOCK);
	return (sock);
}

std::string	Server::getResponse(int sock, const Client& client) {
	char	ip[INET_ADDRSTRLEN];
	struct sockaddr_in	addrResp;
	memset(&addrResp, 0, sizeof(addrResp));
	socklen_t addrLen = sizeof(addrResp);
	getsockname(sock, (struct sockaddr *) &addrResp, &addrLen);
	inet_ntop(AF_INET, &addrResp.sin_addr, ip, (socklen_t)sizeof(ip));
	Req	req(locationList, locationListExt, sock, client.getBuff(), ip);
	Res	res(&req, sock);
	return (res.getRes());
}

int	Server::getFd() const {
	return (fd);
}

int	Server::getMaxFd() const {
	return (maxFd);
}

struct sockaddr_in&	Server::getAddr() {
	return (addr);
}

std::string	Server::getPort() const {
	return (port);
}
