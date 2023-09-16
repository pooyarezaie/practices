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

#include "tcounter.hpp"
#include "pcounter.hpp"

TotalCounter::TotalCounter(std::string key,
        std::string file_name) : file_name(file_name), key(key)
{
    create_fifo();
    
    if (fifo_fd < 0) {
        perror("open fifo");
        exit(1);
    }
}
TotalCounter::~TotalCounter()
{
    for (int i=0; i<pcounters.size(); ++i)
        delete pcounters[i];
    
    close(fifo_fd);
}
void TotalCounter::create_fifo()
{
	mkfifo(fifo_name.c_str(), 0666);
    fifo_fd = open(fifo_name.c_str(), O_RDONLY | O_NONBLOCK);
    
    //log if open was success
	perror("open");
}
void TotalCounter::create_pcounters(int num_of_pcounters)
{
    for (int i=0; i <num_of_pcounters; ++i) {

        int pending[2]; // save ending of pipe

        /* Create the pipe. */
        if (pipe (pending))
        {
            std::cerr<<"Pipe failed"<<std::endl;
            exit(1);
        }

        PartialCounter *newpcnt= new PartialCounter(pending[0], pending[1]);
        pcounters.push_back(newpcnt);
    }
}
void TotalCounter::clac_intervals(int &start, int &end,
        int index, int num_of_pcounters)
{
    FILE* file=fopen(this->file_name.c_str(), "rb");
    if (file==NULL) {
        std::cerr<<"can not open the file "<<file_name<<std::endl;
        exit(1);
    }

    fseek(file, 0, SEEK_END);
	int file_size = ftell(file);
    --file_size;
	fclose(file);

    int part_size = file_size / num_of_pcounters;
    if (index==num_of_pcounters-1) {
        start = (num_of_pcounters-1)*part_size;
        end = file_size-1;
    }
    else {
        start=index*part_size;
        end = (index+1)*part_size + this->key.size()-2;
        if (end >= file_size)
            end = file_size-1;
    }
}
std::string TotalCounter::read_file_part(int index,
        int num_of_pcounters)
{
    int start, end;
    clac_intervals(start, end, index, num_of_pcounters);

    std::ifstream fin;
    std::string res;

    fin.open(this->file_name.c_str());
    fin.seekg(start);

    res.resize(end-start+1);
    char tmp[res.size()+1];
    fin.read(tmp, (int)(end-start+1));
    tmp[res.size()]='\0';
    fin.close();
    res=tmp;

    return res;
}
void TotalCounter::wait_for_all_pcounters()
{    
    while (true) {
        int status;
        pid_t done = wait(&status);

        if (done==-1) {
            if (errno==ECHILD)
                break; // no more child processes
        }
        else if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cerr<<"pid "<<done<<" failed"<<std::endl;
            exit(1);
        }
    }
}
int TotalCounter::count(int num_of_pcounters)
{
    create_pcounters(num_of_pcounters);

    pid_t pid; 
    for (int i=0; i<num_of_pcounters; ++i) {

        /* Create the child process. */
        pid = fork ();

        if (pid == (pid_t)(0))
        {
            /* This is the child process.
               Close other end first. */
            close (pcounters[i]->get_wfd());
            pcounters[i]->run();
        }
        else if (pid < (pid_t) 0)
        {
            /* The fork failed. */
            std::cerr<<"Fork failed"<<std::endl;
            return -1;
        }
        else {
            
            std::cout<<"[log] pcounter forked! (pid="<<
            pid<<")"<<std::endl;
            /* This is the parent process.
               Close other end first. */
            close (pcounters[i]->get_rfd());

            std::string file_part, msg;
            file_part = read_file_part(i, num_of_pcounters);
            
            std::stringstream msgstream;
            msgstream << this->key << " " << file_part;
            
            msg = msgstream.str();
            
            std::cout<<"[log] sending key and file part to pcounter"<<std::endl;
            if (write(pcounters[i]->get_wfd(), msg.c_str(),
                        msg.size()) < 0) {
                perror("write to pipe");
            }
            
            close(pcounters[i]->get_wfd());
        }
    }

    wait_for_all_pcounters();
    
    std::cout << "[log] all pcounters are finished!"<<std::endl;
    
    char buffer[MAX_BUF];
    std::fill(buffer, buffer+MAX_BUF, '\0');
    int ans=0;
    if (read(this->fifo_fd, buffer, MAX_BUF)) {
        
        std::stringstream tmp;

        for (int i=0; i <MAX_BUF; ++i)
            tmp << buffer[i];
        
        int pres;
        for (int i=0; i < num_of_pcounters; ++i) {
            tmp >> pres;
            ans+=pres;
        }
    }
    else {
        std::cout<<"can not read form fifo!"<<std::endl;
        exit(1);
    }
    return ans;
}
