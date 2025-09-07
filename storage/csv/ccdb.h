#ifndef CCDB_INCLUDED
#define CCDB_INCLUDED

#include <zookeeper/zookeeper.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

// prototypes of locking functions
// ===============================
void watcher(zhandle_t *zzh, int type, int state, const char *path,
             void *watcherCtx); // not really watching anything
char* ccdb_mutex_read(std::string uri);
char* ccdb_mutex_check(std::string uri);
std::string ccdb_mutex_lock(std::string uri, std::string node, std::string user);
char* ccdb_mutex_release(std::string uri);
char* ccdb_publish_status(std::string uri);
char* ccdb_show_status(std::string uri);
char* ccdb_mutex_createlocknode(std::string uri);

// helper function prototypes
std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace);

/*
static member functions:

ccdb_locking.cc 
    std::vector<std::string> convert_vector(const struct String_vector* strings);

*/

// class definitions

extern int children_count;
class CCDB_Global_Status
{
public: 
    int childrenCount;
    std::vector<std::string> stVector;
    int showStatus(std::string uri);
    int showSinglePublishStatus(std::string uri);
    struct ccdbState{
        std::string uri;
	std::string published;
	std::string node; 
	std::string user;
    };
    std::vector<ccdbState> ccdbMutexes;


private: 
    static zhandle_t *zh;;
    std::string path;
    int rc;
    int id;
};




#endif
