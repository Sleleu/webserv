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

For a server initialized on port 8080, for example, by going to `http://localhost:8080`, a presentation page will be available :

![webserv_site](https://user-images.githubusercontent.com/93100775/227726291-a19eada1-6b75-4b5e-b633-2e9778210b2a.gif)

The presentation page contains routes to tests of our CGIs such as :
- The use of a calculator
- The execution of php and python scripts

as well as for the use of text file upload and file deletion in order to test more easily the implementation of the **POST** and **DELETE** methods.
The page finally provides an access to the bible.txt in order to test the display of large pages

To test the server further upstream, a simple client has been created and is accessible from the path `./src/client`. Here is an example of its use after installation, by launching an invalid HTTP GET request "/hellooo" :

![client](https://user-images.githubusercontent.com/93100775/227727700-3e849668-479c-4ce6-972a-648ce835e482.png)


## Installation

### Clone the repository and make

```shell
git clone git@github.com:Sleleu/webserv.git
cd webserv
make
```

### If you want to install client

From ./webserv :
```shell
cd src/client
make
```


### Usage

Here is a standard use of the program :

```shell
./webserv serv.conf
```
You can modify the serv.conf file to add other parameters or servers.
Eventually, see with the option `--verbose` for more parameters about parsing of the request/response
