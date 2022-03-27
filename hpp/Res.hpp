/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:04:12 by zefir             #+#    #+#             */
/*   Updated: 2022/03/21 10:37:17 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RES_HPP
# define RES_HPP

# include <string>
# include <cstdio>
# include <iomanip>
# include <iostream>
# include <sstream>
# include <fstream>
# include <algorithm>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
# include "Loc.hpp"
# include "Req.hpp"
# include "Server.hpp"
# include "Helper.hpp"
# include "Cgi.hpp"

class Res {
private:
	Req*								req;
	int									sock;
	Loc 								loc;
	std::string 						versionHTTP;
	int									resCode;
	std::string							contentType;
	std::map<std::string, std::string>	headers;
	std::map<std::string, std::string>	headersCgi;
	std::string							header;
	std::string							body;
	std::string							res;
	std::string							authorization;
	bool								isAuth;
	bool								isCGIres;
	std::map<int, std::string>			resMsgs;
	bool								setFail;
	struct stat							pointStat;

public:
	Res(Req* requ, int socket);
	Res(const Res& cp);
	~Res();

	
	void			createResponse();
	void			createHeader();

	void			processGet();
	void			processPost();
	void			processDelete();
	bool			isHaveIndexPage(const std::string& urlPathAbsolute);
	std::string		getIndexPage();
	bool			fillAutoIndex();
	void			setError(int num);
	std::string		addDefError(const std::string& code, const std::string& msg);
	void			langCheck();
	void			setResCode(int code);
	void			setContentType(const std::string& type);
	void			setBody(const std::string& bd);
	std::string		getContentType(const std::string& point);
	void			getLastChange();
	bool			isAuthValid();
	bool			isValidMethod();
	bool			isValidHttpMethod();

	
	std::string								getRes() const;
	std::string								getBody() const;
	std::string								getHeader() const;
	std::string								getResCodeStr() const;
	std::string								getAuthorization() const;
	std::string								getResCodeMessage();
	int										getResCode() const;
	int										getHeaderLength() const;
	int										getBodyLength() const;
	int										getResLength() const;
	bool									getIsAuth() const;
};

#endif
