/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crendeha <crendeha@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:17:41 by zefir             #+#    #+#             */
/*   Updated: 2022/03/20 15:32:46 by crendeha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Client.hpp"

Client::Client() {}
Client::~Client() {}

Client::Client(const Client& cp) {
	*this = cp;
}

Client& Client::operator=(const Client& cp) {
	if (this != &cp) {
		sockfd = cp.sockfd;
		numberServer = cp.numberServer;
		buffer = cp.buffer;
		answer = cp.answer;	
		isAllRead = cp.isAllRead;
		isAllWrote = cp.isAllWrote;
		deliveredBytes = cp.deliveredBytes;
		stillDeliverBytes = cp.stillDeliverBytes;
	}

	return (*this);
}

Client::Client(int socket, int nserver)
: sockfd(socket), numberServer(nserver), buffer(""),isAllRead(false),
	isAllWrote(false), deliveredBytes(0), stillDeliverBytes(0) {}


bool	Client::recvUp() {
	memset(&chunk, 0, __BUFFER__);
	int	res = recv(sockfd, chunk, __BUFFER__ - 1, 0);
	if (res < 0) exit(EXIT_FAILURE); // Error reading
	else if (res == 0) return (false); // Finish reading
	else buffer += std::string(chunk); // Add str (success read)

	// DELETE
	// std::cout << chunk; // Check chunk delivery to serv
	//
	
	isAllRead = endOfRead(); // Check remainder for read
	return (true);
}

bool	Client::endOfRead() {
	if (buffer.find("\r\n\r\n") == std::string::npos) return (false); // Check end header
	if (buffer.find("Transfer-Encoding: chunked") == std::string::npos) return (true); // Check dynamic option
	if (buffer.substr(buffer.size() - 5) !=  "0\r\n\r\n") return (false); // Check end in current buffer
	return (true);
}

bool	Client::sendUp() {
	int	res = send(sockfd, answer.c_str(), answer.size(), 0);
	if (res < 0) return (false); // Error send message, close FD!!!! (false) 
	deliveredBytes += res;
	stillDeliverBytes -= res;
	answer.erase(0, res);
	isAllWrote = stillDeliverBytes > 0 ? false : true; // Check remainder for write
	return (true);
}

int	Client::getSock() const {
	return (sockfd);
}

void	Client::setAllRead(bool isFinish) {
	isAllRead = isFinish;
}

void	Client::setAllWrote(bool isFinish) {
	isAllWrote = isFinish;
}

bool	Client::getAllRead() const {
	return (isAllRead);
}
bool	Client::getAllWrote() const {
	return (isAllWrote);
}
int		Client::getServerNum() const {
	return (numberServer);
}
void	Client::setDeliverBytes(int val) {
	stillDeliverBytes = val;
}
void	Client::setAnswer(const std::string& ans) {
	answer = ans;
}

std::string Client::getBuff() const {
	return (buffer);
}

void	Client::clearBuffer() {
	buffer.clear();
}