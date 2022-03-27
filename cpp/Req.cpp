/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crendeha <crendeha@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:13:58 by zefir             #+#    #+#             */
/*   Updated: 2022/03/20 15:45:06 by crendeha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Req.hpp"
			
Req::Req(std::vector<Loc>& locationList, std::vector<Loc>& locationListExt,
			int sockn, const std::string& buffer, const std::string& ip) 
: locationList(locationList), locationListExt(locationListExt),
			sockn(sockn), buffer(buffer), ip(ip), contentLength(0), badReq(false) {

	parse(buffer);
	chooseUrl();
	pathUp();
}

Req::Req(const Req& cp) {
	*this = cp;
}

Req::~Req() {}

Req& Req::operator=(const Req& cp) {
	if (this != &cp) {
		buffer= cp.buffer;
		sockn = cp.sockn;
	}
	return (*this);
}

void	Req::parse(const std::string& bufferReq) {
	std::string			tmpHeader;
	std::string			tmpBody;

	cutHeadBody(bufferReq, tmpHeader, tmpBody);
	parseHeader(tmpHeader, tmpBody);
	body = tmpBody;
	body = headers["Transfer-Encoding"] == "chunked" ? bodyChunk() : body;
	contentLength = body.size();
}

void	Req::cutHeadBody(const std::string& bufferReq, std::string& tmpHeader, std::string& tmpBody) {
	if (bufferReq.find("\r\n\r\n") != std::string::npos) {
		tmpHeader = bufferReq.substr(0, bufferReq.find("\r\n\r\n"));
		tmpBody = bufferReq.substr(bufferReq.find("\r\n\r\n") + 4, bufferReq.length());
	}
	else {
		tmpHeader = bufferReq;
		tmpBody = "";
	}
}

void	Req::parseHeader(std::string& hd, std::string& bd) {
    std::string					key;
    std::string					value;
	std::vector<std::string>	pieceOfHeader;
	
	std::string					line = hd.substr(0, hd.find("\r\n"));
	hd = hd.substr(hd.find("\r\n") + 2, hd.size());
	pieceOfHeader = Helper::ft_split(line, ' ');
	
	if (pieceOfHeader.size() != 3)
		badReq = true;

	method = pieceOfHeader[0];
	point = pieceOfHeader[1];
	versionHTTP = pieceOfHeader[2];

	while (true) {
		line = hd.substr(0, hd.find("\r\n"));
		hd = hd.substr(hd.find("\r\n") + 2, hd.size());

		if (line.find(":") ==  std::string::npos) {
			badReq = true;
			break ;
		}
		key = line.substr(0, line.find(":"));
		value = line.substr(line.find(":") + 2, line.size());
		if (headers.count(key)) {
			badReq = true;
			break ;
		}
		headers[key] = value;
		if (hd.find("\r\n") == std::string::npos) break ;
	}
}

std::string	Req::bodyChunk() {

	char*	tmpBody = (char *)body.c_str();
	char*	result = (char *)malloc(sizeof(char) * body.size()); // Check?
	char*	answer = result;
	char	tmpStr[100];
	char*	curPtr;
	int		lenChunk;
	size_t	hexLen;

	memset(result, 0, body.size() - 1);

	while (true) {
		curPtr = strstr(tmpBody, "\r\n");
		hexLen = curPtr - tmpBody;
		memset(tmpStr, 0, 99);
		memcpy(tmpStr, tmpBody, hexLen);
		
		std::stringstream ss;
		ss << std::hex << tmpStr;
		ss >> lenChunk;
		if (!lenChunk) break ;

		tmpBody += hexLen + 2;
		memcpy(result, tmpBody, lenChunk);

		result = result + lenChunk;
		tmpBody += lenChunk + 2;
	}

	std::string newBody = answer;
	free(answer);
	
	return (newBody);
}

void	Req::chooseUrl() {
	chooseLoc();
	if(chooseLocExt()) unionLoc();
	cutUrl();
}

void	Req::chooseLoc() {
	std::vector<Loc>::iterator it = locationList.begin();
	while (it != locationList.end()) {
		Loc loc(*it);
		if (loc.getPath() == point.substr(0, loc.getPath().size())) {
			if (loc.getPath() == "/" || point.size() == loc.getPath().size() || point[loc.getPath().size()] == '/') {
				if (pickLocation.isSet() == false) pickLocation = loc;
				else if (pickLocation.getPath().size() < loc.getPath().size()) pickLocation = loc;
			}
		}
		it++;
	}
	pointUrl = point;
	point = pickLocation.getPath() != "/" ? point.substr(pickLocation.getPath().size(), point.size()) : point;
	
	if (point[0] != '/') point.insert(0, "/");
}

bool	Req::chooseLocExt() {
	std::string pointExt = point.find(".") == std::string::npos ? "" : point.substr(point.find(".") + 1, point.size());
	std::vector<Loc>::iterator it = locationListExt.begin();
	while (it != locationListExt.end()) {
		if (it->getPath() == pointExt) {
			pickLocationExt = *it;
			return true;
		}
		it++;
	}
	return (false);
}

void	Req::unionLoc() {
	std::map<std::string, std::string>	tmpLoc = pickLocation.getLocations();
	std::map<std::string, std::string>	tmpLocExt = pickLocationExt.getLocations();
	std::map<std::string, std::string>::iterator it = tmpLocExt.begin();

	while (it != tmpLocExt.end()) {
		if (it->first != "path" && it->second != "") {
			if (it->first.compare("accepted_method") == 0)
				tmpLoc["accepted_method"] += it->second.size() > 0 ? "/" + it->second : "";
			else tmpLoc[it->first] = it->second;
		}
		it++;
	}
	pickLocation = Loc(tmpLoc);
}

void	Req::cutUrl() {
	if (point.find("?") != std::string::npos) {
		query = point.substr(point.find("?") + 1, point.size() - 2);
		point = point.substr(0, point.find("?"));
		pointUrl = pointUrl.substr(0, pointUrl.find("?"));
	}
}

void	Req::pointUp(const std::string& pnt) {
	point = pnt;
	if (pickLocation.getRoot()[0] == '/')
		pointUrlAbsolute = Helper::mergeUrls(pickLocation.getRoot(), point);
	else {
		char cwd[1000]; // Check error path
		if (getcwd(cwd, sizeof(cwd)) == NULL) std::cout << "Error get path\n";
		std::string currentdir = cwd;
		pointUrlAbsolute = Helper::mergeUrls(currentdir, pickLocation.getRoot());
		pointUrlAbsolute = Helper::mergeUrls(pointUrlAbsolute, point);
	}
}

void	Req::pathUp() {
	if (pickLocation.getRoot()[0] == '/')
		pointUrlAbsolute = Helper::mergeUrls(pickLocation.getRoot(), point);
	else {
		char cwd[1024]; // Check error path
		if (getcwd(cwd, sizeof(cwd)) == NULL) std::cout << "Error get path\n";
		std::string currentdir = cwd;
		pointUrlAbsolute = Helper::mergeUrls(currentdir, pickLocation.getRoot());
		pointUrlAbsolute = Helper::mergeUrls(pointUrlAbsolute, point);
	}
}

Loc	Req::getPickLoc() const {
	return (pickLocation);
}

std::string	Req::getUrlAbsolute() const {
	return (pointUrlAbsolute);
}

std::string	Req::getMethod() const {
	return (method);
}

int		Req::getSockn() const {
	return (sockn);
}

std::string		Req::getBody() const {
	return (body);
}

std::string		Req::getPoint() const {
	return (point);
}

std::string		Req::getUrl() const {
	return (pointUrl);
}

std::string		Req::getQuery() const {
	return (query);
}

std::map<std::string, std::string>	Req::getHeaders() const {
	return(headers);
}

size_t	Req::getContentLength() const {
	return (contentLength);
}

bool	Req::getIsBadReq() const {
	return(badReq);
}

std::string	Req::getVersHttp() const {
	return(versionHTTP);
}

std::string			Req::getIp() const {
	return (ip);
}
