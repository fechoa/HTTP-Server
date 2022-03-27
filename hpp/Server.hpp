/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 13:37:25 by zefir             #+#    #+#             */
/*   Updated: 2022/03/19 21:08:06 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iomanip>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <string>
# include <iostream>
# include <cwchar>
# include <cstring>
# include <netinet/in.h>
# include <sys/select.h>
# include "Req.hpp"
# include "Res.hpp"
# include "Config.hpp"
# include "Loc.hpp"
# include "Client.hpp"

class Server {
private:

	int					fd;
	struct sockaddr_in	addr;
	std::string			listenStr;
	std::vector<Loc>	locationList;
	std::vector<Loc>	locationListExt;
	long				maxFd;
	std::string			port;
	std::string			ip;
	int					numberServer;

public:

	Server();
	Server(const std::string& listen, const std::vector<Loc>& locationVector, const std::vector<Loc>& locationExtVector);
	Server(const Server& cp);
	~Server();																								
	Server& operator=(const Server& cp);																	


	int						create(size_t num);
	void					initParams(size_t num);
	int 					initConnect();
	std::string				getResponse(int sock, const Client& client);
	int						acceptConnect();


	int						getFd() const;
	int						getMaxFd() const;
	struct sockaddr_in		&getAddr();
	std::string				getPort() const;
	std::string				getListen() const;
};

#endif
