#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "pcounter.hpp"


PartialCounter::PartialCounter(int prfd, int pwfd)
    : partail_count(0), prfd(prfd), pwfd(pwfd) {
    
        fifo_fd=open(fifo_name.c_str(),
                     O_WRONLY | O_NONBLOCK | O_APPEND);
}

int PartialCounter::count()
{
    int ans=0;
    std::size_t found = file_part.find(key);
    while (found!=std::string::npos) {
        ++ans;
        found=file_part.find(key,found+1);
    }
    return ans;
}

void PartialCounter::get_msg()
{
    std::stringstream tmp;
    char buffer[MAX_BUF];
    std::fill(buffer, buffer+MAX_BUF, '\0');
    
    while ( read(this->prfd, buffer, sizeof(buffer)) > 0 ) {
        for (int i=0; i<MAX_BUF; ++i)
            tmp << buffer[i];
        
        std::fill(buffer, buffer+MAX_BUF, '\0');
    }
    
    tmp >> key;
    
    std::string pdata;
    while(tmp>>pdata)
        file_part+=pdata;
    
    close(this->prfd);
}

void PartialCounter::send_result(std::string res)
{
    if (write(fifo_fd, res.c_str(), res.size()) < 0) {
        std::cout<<"faild to write on fifo!"<<std::endl;
    }
    std::cout << "[log] sending: pcounter res : " << res << std::endl;
    close(fifo_fd);
}

void PartialCounter::run()
{
    get_msg(); // get msg from pipe and set file_part and key

    int res=count();
    
    std::stringstream tmp;
    tmp << res;
    std::string sres;
    tmp >> sres;
    sres=sres+" ";
    
    send_result(sres);
    close(this->prfd);
    _exit(EXIT_SUCCESS);
}

pid_t PartialCounter::get_rfd() 
{
    return prfd;
}

pid_t PartialCounter::get_wfd()
{
    return pwfd;
}
