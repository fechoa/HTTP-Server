/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zefir <zefir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:19:21 by zefir             #+#    #+#             */
/*   Updated: 2022/03/17 22:11:36 by zefir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include "Helper.hpp"

class Client {
private:
	int				sockfd;
	int				numberServer;
	std::string		buffer;
	std::string		answer;
	bool			isAllRead;
	bool			isAllWrote;
	int				deliveredBytes;
	int				stillDeliverBytes;
	char			chunk[__BUFFER__];

public:
	Client();									
	Client(int sockfd, int nserver);				
	Client(const Client& cp);  					
	~Client();								
	Client& operator=(const Client& cp); 



	bool			recvUp();
	bool			sendUp();
	bool			endOfRead();
	int				getSock() const;
	bool			getAllRead() const;
	bool			getAllWrote() const;
	int				getServerNum() const;
	std::string		getBuff() const;
	void			setAllRead(bool isFinish);
	void			setAllWrote(bool isFinish);
	void			setDeliverBytes(int val);
	void			setAnswer(const std::string& ans);
	void			clearBuffer();
};

#endif
