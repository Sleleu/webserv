# webserv

## Project

This project is about writing an HTTP server in c++98.

The project was carried out in 3 main areas:
- Parsing a configuration file allowing the instantiation of multiple servers
- Creation of a server receiving HTTP requests from clients, and returning the appropriate response
- Parsing each request and building the response and return code

The instantiation of the handler and its servers is done in the following way :

![Server_process](https://user-images.githubusercontent.com/93100775/227724907-d7efc293-9ccf-4f88-9090-6aa802b6433f.png)

## Final render

Once the program is launched, the terminal displays some information such as the initialization of the servers with their names, ports,
as well as the incoming requests and responses sent on each socket :

![render_shell](https://user-images.githubusercontent.com/93100775/227725307-0cab5a30-8cb9-41ba-b65b-4a9123b7b940.png)

## Installation

### Clone the repository and make

```shell
git clone git@github.com:Sleleu/webserv.git
cd webserv
make
```

### Usage

Here is a standard use of the program :

```shell
./webserv serv.conf
```
You can modify the serv.conf file to add other parameters or servers.
Eventually, see with the option `--verbose` for more parameters about parsing of the request/response
