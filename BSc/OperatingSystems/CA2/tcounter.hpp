#ifndef _TCOUNTER_H
#define _TCOUNTER_H

#include <vector>

#define MAX_BUF 1024

const std::string fifo_name="/tmp/pipe/myfifo";

class PartialCounter;

class TotalCounter {
private:
    std::vector<PartialCounter* > pcounters;
  
    int total_count, fifo_fd;
    std::string file_name, key;

    void create_fifo();
    void create_pcounters(int num_of_pcounters);
    void clac_intervals(int &start, int &end, int index,
            int num_of_pcounters);
    std::string read_file_part(int index, int num_of_pcounters);
    void wait_for_all_pcounters();
public:
    TotalCounter(std::string key, std::string file_name);
    ~TotalCounter();
    int count(int num_of_pcounters);
};

#endif
