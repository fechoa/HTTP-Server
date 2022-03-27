/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 22:06:10 by zefir             #+#    #+#             */
/*   Updated: 2022/03/21 11:57:43 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp/Cgi.hpp"

Cgi::Cgi(Req* req, Res* res) : req(req), res(res) {
	fillEnv();
	envs = toArr();
	fillArgs();
}

Cgi::Cgi(Cgi const &src) {
	*this = src;
}

Cgi::~Cgi() {
	freeStrs(args);
	freeStrs(envs);
}

void	Cgi::freeStrs(char** strs) {
	if (strs)
	{
		for (int i = 0; strs[i]; i++)
			delete[] (strs)[i];
		delete[] strs;
	}	
}

std::string	Cgi::transformEnv(std::string form) {
	std::transform(form.begin(), form.end(), form.begin(), ::toupper);
	std::replace(form.begin(), form.end(), '-', '_');
	return (form);
}

bool	Cgi::actions() {
	pid_t  pid;
    int    serv_to_cgi[2]; // cgi reads
    int    cgi_to_serv[2]; // serv reads
    char tmp[1024];
    getcwd(tmp, 1024);
    std::string path = std::string(tmp) + "/" + req->getPickLoc().getCgiPath();
	
    pipe(serv_to_cgi);
    pipe(cgi_to_serv);
    pid = fork();
    if (pid == 0)
    {
        dup2(serv_to_cgi[0], 0);
        dup2(cgi_to_serv[1], 1);
        close(serv_to_cgi[1]);
        close(cgi_to_serv[0]);
        if (execve(path.c_str(), args, envs) < 0)
        {
            std::cerr << "CGI doesn't work" <<
				"path: " << path
			 << std::endl;
            exit(1);
        }
    }
    else
    {
        char	buffer[1024];
        int		ret;

        write(serv_to_cgi[1], req->getBody().c_str(), req->getBody().size());
        close(cgi_to_serv[1]);
        close(serv_to_cgi[0]);
        while ((ret = read(cgi_to_serv[0], buffer, 1023)) != 0)
        {
            buffer[ret] = 0;
            result += buffer;
        }
        wait(0);
    }
    return true;
}



void	Cgi::fillEnv() {
	std::string listen = req->getPickLoc().getListen();
	std::string ip = listen.substr(0, listen.find(":"));
	std::string port = listen.substr(listen.find(":") + 1 , listen.size());
	
	std::map<std::string, std::string>	reqHeaders = req->getHeaders();
	env["AUTH_TYPE"] = reqHeaders["WWW-Authenticate"];
	env["CONTENT_LENGTH"] = reqHeaders["Content-Length"];
	env["CONTENT_TYPE"] = reqHeaders["Content-Type"];
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["PATH_INFO"] =  req->getUrl();
	env["PATH_TRANSLATED"] = req->getUrlAbsolute();
	env["QUERY_STRING"] = req->getQuery();
	env["REMOTE_ADDR"] = req->getIp();
	if (res->getIsAuth()) {
		env["REMOTE_IDENT"] = reqHeaders["Authorization"];
		env["REMOTE_USER"] = res->getAuthorization();
	}
	env["REQUEST_METHOD"] = req->getMethod();
	if (req->getQuery() != "") env["REQUEST_URI"] = req->getUrl() + "?" + req->getQuery();
	else env["REQUEST_URI"] = req->getUrl();

	env["SCRIPT_NAME"] = req->getUrl();
	env["SERVER_NAME"] = ip;
	env["SERVER_PORT"] = port;
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "newserver";
	env["REDIRECT_STATUS"] = "200";
	std::map<std::string, std::string>::iterator it = reqHeaders.begin();
	while (it != reqHeaders.end()) {
		if (it->second != "") env["HTTP_" + transformEnv(it->first)] = it->second;
		it++;
	}
}

void	Cgi::fillArgs() {
	args = new char*[3];
	args[0] = new char[req->getPickLoc().getCgiPath().size() + 1];
	args[0] = std::strcpy(args[0], req->getPickLoc().getCgiPath().c_str());
	args[1] = new char[req->getUrlAbsolute().size() + 1];
	args[1] = std::strcpy(args[1], req->getUrlAbsolute().c_str());
	args[2] = 0;
}

void	Cgi::parseResponse() {
	size_t					i;
	std::string				line;
 	std::string				key;
    std::string				value;
	std::string				header;
	std::string				body;
	std::string				res = result;

	if (res.find("\r\n\r\n") != std::string::npos) {
		header = res.substr(0, res.find("\r\n\r\n"));
		result = res.substr(res.find("\r\n\r\n") + 4, res.length());
	}
	else return ;

	while (true) {
		Helper::strcut(&line, &header, "\r\n", &i);
		if(line.find(":") == std::string::npos) continue ;	
		key = line.substr(0, line.find(":"));
		value = line.substr(line.find(":") + 2, line.size());
		headers[key] = value;
		if (i == std::string::npos) break ;	
	}
}

char	**Cgi::toArr() {
	char    **res = new char*[env.size() + 1];
	int     i = 0;
	
	std::map<std::string, std::string>::iterator it = env.begin();
	while (it != env.end()) {
		std::string	elem = it->first + "=" + it->second;
		res[i] = new char[elem.size() + 1];
		res[i] = std::strcpy(res[i], elem.c_str());
		i++;
		it++;
	}
	res[i] = NULL;

	return (res);
}

std::string	Cgi::getResult() {
	return (result);
}

std::map<std::string, std::string>	Cgi::getCgiHeaders() {
	return (headers);
}
