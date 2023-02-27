#include "../../header/response/CgiHandler.hpp"

    CgiHandler::CgiHandler(HttpResponse & response, HttpRequest const & request) : _output("")
    {
        if (response.getCgiPath() == "" && response.getExtension() != "cgi")
            return ;
        try
        {
            setEnvMap(response, request);
            _scriptPath = (response.getCgiPath() != "") ? response.getCgiPath():response.getTargetPath();
            _filename = request.getFileName();
            convertMap();
            setEnvChar();
            executeCgi();
        }
        catch (std::exception e)
        {
            response.setError("500", "Internal Server Error");
            response.setBody(BODY_500);
        }
    }

    void    CgiHandler::setEnvMap(HttpResponse & response, HttpRequest const & request)
    {
        stringMap	argMap = request.getArgs();
        for (stringMap::const_iterator it = argMap.begin() ; it != argMap.end() ; it ++)
            _envMap[it->first] = it->second;
        _envMap["UPLOAD"] = response.getIsUpload();
        _envMap["UPLOAD_PATH"] = "./html" + response.getUploadPath();
        _envMap["CONTENT"] = request.getContent();
        _envMap["PATH_INFO"] = response.getTargetPath();
    }

    void    CgiHandler::convertMap()
    {
        for (std::map<std::string, std::string>::iterator it = _envMap.begin() ; it != _envMap.end() ; it++)
        {
            if (it->second[0] == '%')
            {
                std::string hexString = it->second.substr(1);
                unsigned int decimalValue;
                std::stringstream ss;
                ss << std::hex << hexString;
                ss >> decimalValue;
                it->second[0] = decimalValue;
                it->second = it->second.substr(0, 1);
            }
        }
    }

    void    CgiHandler::setEnvChar()
    {
        _env = new char*[_envMap.size() + 1];
        int i = 0;
        for (stringMap::const_iterator it = _envMap.begin() ; it != _envMap.end() ; it ++)
        {
            std::string line = it->first + "=" + it->second;
            _env[i] = new char[line.size() + 1];
            strcpy(_env[i], (const char*)line.c_str());
            i ++;
        }
        _env[i] = NULL;
    }

    void    CgiHandler::executeCgi()
    {
        int pipefd[2];
        pid_t pid;

        std::vector<char*> args;
        args.push_back((char*)"/bin/bash");
        args.push_back((char*)_scriptPath.c_str());
        // if (_scriptPath != _envMap["PATH_INFO"])
        //     args.push_back((char*)_envMap["PATH_INFO"].c_str());
        args.push_back((char*)_filename.c_str());
        args.push_back(NULL);

        // for(int i = 0; _env[i] ; i ++)
        // 	std::cout << _env[i] << std::endl;

        if (pipe(pipefd) == -1)
        {
            std::cerr << "Error: failed to create pipe" << std::endl;
            throw std::exception();
            return ;
        }

        pid = fork();
        if (pid == -1) {
            std::cerr << "Error: failed to fork process" << std::endl;
            throw std::exception();
            return ;
        }
        else if (pid == 0)
        {

            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);

            char** argv = &args[0];
            execve(argv[0], argv, _env);
            std::cerr << "Error: failed to execute script" << std::endl;
            exit (5);
            return ;
        }
        else
        {
            close(pipefd[1]);
            char buffer[BUFFER_SIZE];
            _output = "";
            int nbytes;
            while ((nbytes = read(pipefd[0], buffer, BUFFER_SIZE)) > 0)
                _output += std::string(buffer, nbytes);
            close(pipefd[0]);

            int status;
            waitpid(pid, &status, 0);
            // if (WIFEXITED(status))
                // std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
            for(int i = 0; _env[i] ; i ++)
                delete [] _env[i];
            delete [] _env;

        }
    }

    std::string CgiHandler::getOutputBody() const
    {
        std::string outputBody = _output.substr(_output.find("\n\n") + 1);
        return outputBody;
    }
    std::string CgiHandler::getOutputHeader() const
    {
        std::string outputHeader = _output.substr(0, _output.find("\n\n"));
        return outputHeader;
    }
    std::string CgiHandler::getOutput() const { return _output;}
