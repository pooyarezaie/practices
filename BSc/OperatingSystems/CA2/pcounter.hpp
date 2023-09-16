#ifndef _PCOUNTER_HPP
#define _PCOUNTER_HPP

#include <string>
#include <sys/wait.h>

#define MAX_BUF 1024

extern const std::string fifo_name;

class PartialCounter {
private:
    int partail_count;
    std::string key, file_part;
    int fifo_fd; // for named pipe
    int prfd; // reading end of pipe
    int pwfd; // writing end of pipe  

    int count();
    void get_msg();
    void send_result(std::string res);
public: 
    PartialCounter(int prfd, int pwfd);    
    void run();
    pid_t get_rfd();
    pid_t get_wfd();
};

#endif
