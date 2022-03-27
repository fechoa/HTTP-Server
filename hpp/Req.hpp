/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zefir <zefir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:14:45 by zefir             #+#    #+#             */
/*   Updated: 2022/03/18 10:06:34 by zefir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQ_HPP
# define REQ_HPP

# include <unistd.h>
# include <stddef.h>
# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <iomanip>
# include "Loc.hpp"

class Req {
private:
	Loc									pickLocation;
	Loc									pickLocationExt;
	std::vector<Loc>					locationList;
	std::vector<Loc>					locationListExt;
	int									sockn;
	std::string							buffer;
	std::string							ip;
	int									contentLength;
	std::string							method;
	std::string							point;
	std::string							versionHTTP;
	std::string							body;
	std::string							pointUrl;
	std::string							pointUrlAbsolute;
	std::map<std::string, std::string>	headers;
	std::string							query;
	bool								badReq;


public:
	Req();
	Req(std::vector<Loc>& locationList, std::vector<Loc>& locationListExt,
			int sockn, const std::string& buffer, const std::string& ip);
	Req(Req const& src);
	~Req();
	Req & operator=(Req const& rhs);



	void			parse(const std::string& bufferReq);
	void			chooseUrl();
	void			cutHeadBody(const std::string& bufferReq, std::string& tmpHeader, std::string& tmpBody);
	void			parseHeader(std::string& hd, std::string& bd);
	std::string		bodyChunk();
	void			chooseLoc();
	bool			chooseLocExt();
	void			unionLoc();
	void			cutUrl();
	void			pointUp(const std::string& pnt);
	void			pathUp();
	

	
	Loc									getPickLoc() const;
	std::string							getUrlAbsolute() const;
	std::string							getMethod() const;
	int									getSockn() const;
	std::string							getBody() const;
	std::string							getPoint() const;
	std::string							getUrl() const;
	std::string							getQuery() const;
	std::string							getAuth() const;
	std::map<std::string, std::string>	getHeaders() const;
	size_t								getContentLength() const;
	bool								getIsBadReq() const;
	std::string							getVersHttp() const;
	std::string							getIp() const;
};

#endif
