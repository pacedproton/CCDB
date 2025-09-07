//* CCDB locking and helper functions */

#include "ccdb.h"

using namespace std;

static zhandle_t *zh = NULL;
std::vector<std::string> ret;

// initilizations

static const char* zookeeper_hostnameport = "127.0.0.1:2181";
static const char* zookeeper_mutex_node = "/ccdb/mutexes";
static const char* fs = "---";



std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
	subject.replace(pos, search.length(), replace);
	pos += replace.length();
    }
    return subject;
}


void watcher(zhandle_t *zzh, int type, int state, const char *path,
             void *watcherCtx){} // not really watching anything                                     


char* ccdb_mutex_read(string uri){

    string path;

/* Reformat CCNx URI as Zookeper node name*/

    ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node) + "/" + uri;

/* Retrieve status */
    char buffer[1024];

//      zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);                                                    
    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
    if (!zh) {

	return strerror(errno);
    }

    int buflen = sizeof(buffer);
    struct Stat stat;
    int rc = zoo_get(zh, path.c_str(), 0, buffer, &buflen, &stat);
    if (rc != ZOK) {
	cout << '\n'<< "Mutex read failed with error code: " << rc << std::endl;

	return strerror(rc);

    } else {

	cout << '\n'<< "Mutex read: " << buffer << std::endl;

    }

    return buffer;;
}


char* ccdb_mutex_check(string uri){
    string path;


    ofstream myfile;
    myfile.open ("/tmp/example.txt");
    myfile << "passed mutex check.\n";
    myfile.close();


/* Reformat CCNx URI as Zookeper node name*/

    uri = ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node) + "/" + uri;

/* DBUG block */
  std::cout << "zookeeper path at ccdb_locking::ccdb_mutex_check " << path << std::endl;
/* end DBUG bock */


/* Retrieve status */

//      zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);                                                    
    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
    if (!zh) {

	return strerror(errno);
    }

    struct Stat stat;
    int rc = zoo_exists(zh, path.c_str(), 0, &stat);
    if (rc != ZOK) {
	cout << '\n'<< "No URI lock for: " << uri << std::endl;
	cout << '\n'<< "RC: " << rc << '\n' << std::endl;
	return strerror(rc);

    } else {
	cout << '\n'<< "URI locked, " << uri << std::endl;
	cout << '\n'<< "RC: " << rc << '\n' << std::endl;
    }
    return strerror(rc);

}


std::string ccdb_mutex_lock(string uri, string node, string user){

    string path;
/* Reformat CCNx URI as Zookeper node name*/

    ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node) + "/" + uri;


/* Construct send buffer */
    string state = "1";
    string published = "0";
    string sendstring = fs + state + fs + published + fs + node + fs + user;
    string buffer = sendstring;

    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
    if (!zh) {

	return strerror(errno);
    }

    int buflen = buffer.size();

    cout << '\n' << "DDDEBUG path: " << path << '\n' << std::endl;
    cout << '\n' << "DDDEBUG buffer size: " << buflen;
    cout << '\n' << "DDDEBUG sendstring: " << sendstring << std::endl;

    struct Stat stat;
    int rc = zoo_set(zh, path.c_str(), buffer.c_str(), buflen, -1);
    if (rc != ZOK) {

	cout << '\n'<< "Always check if the node exists. Error code: " << rc << std::endl;


	return strerror(rc);
    } else {

	cout << '\n'<< sendstring << '\n' << std::endl;
    }

    return std::string(sendstring);
}



char* ccdb_mutex_release(string uri){

    string path;

/* Reformat CCNx URI as Zookeper node name*/

    ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node) + "/" + uri;

//      zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);   
            
zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
if (!zh) {

    return strerror(errno);
}

cout << "debug path" << path << std::endl;

struct Stat stat;
int rc = zoo_delete(zh, path.c_str(), -1);
if (rc != ZOK) {
    fprintf(stderr, "Error %s\n", rc);
    return strerror(rc);
} else {

    cout << '\n'<< "node deleted: " << path << std::endl;
    cout << '\n'<< "return code: " << strerror(rc) << std::endl;
}

return strerror(rc);

}

char* ccdb_mutex_createlocknode(std::string uri){
    string path;

/* Reformat CCNx URI as Zookeper node name*/

    ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node) + "/" + uri;

    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
    if (!zh) {

	return strerror(errno);
    }

    cout << '\n' << "DDDEBUG path: " << path << std::endl;

    struct Stat stat;
    int rc = zoo_create(zh, path.c_str(), "", 0, &ZOO_OPEN_ACL_UNSAFE, 0, 0, 0);
    if (rc != ZOK) {

	cout << '\n'<< "Always check if the node exists. Error code: " << rc << std::endl;

	return strerror(rc);
    } else {

        cout << '\n' << "node created: " << uri << '\n' << std::endl;
    }

    return strerror(rc);

}


char* ccdb_publish_status(string uri){}



char* ccdb_show_status(string uri){

    string path;
    String_vector node_children;

/* Reformat CCNx URI as Zookeper node name*/

    ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node);

/* Retrieve status */
    char buffer[1024];

//      zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);                                                    
    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
    if (!zh) {

	return strerror(errno);
    }

    struct String_vector stvector;
    struct String_vector *vector = &stvector;

    int rc = zoo_get_children(zh, path.c_str(), 0, vector);

    if (rc != ZOK) {
	cout << '\n'<< "Node children read failed, error code: " << rc << std::endl;

	return strerror(rc);

    } else {


//	ret = convert_vector(vector);
	int children_count = stvector.count;

	cout << '\n'<< "Number of node children " << children_count<< std::endl;

    }

    return strerror(rc);

}


int CCDB_Global_Status::showStatus(std::string uri)
{
    cout << '\n'<< "test from CCDB_Global_Status::showStatus " << uri << std::endl;


    ccdbMutexes.push_back(ccdbState());
    ccdbMutexes[0].uri = "this is a test uri into vector struct";

    cout << "vector struct element: " << ccdbMutexes[0].uri << std::endl;


    static zhandle_t *zh = NULL; 
    string path;
    string datapath;
    String_vector node_children;

/* Reformat CCNx URI as Zookeper node name*/

    ReplaceString(uri, "/", "---");

/* Add mutex node path */
    path = string(zookeeper_mutex_node);

//      zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);                                                    
    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);
    if (!zh) {

    }

    struct String_vector stvector;
    struct String_vector *vector = &stvector;

    int rc = zoo_get_children(zh, path.c_str(), 0, vector);

    if (rc != ZOK) {
	cout << '\n'<< "Node children read failed, error code: " << rc << std::endl;

    } else {

// build vector structs with zookeeper nodes


//	ret = convert_vector(stvector.data);
	int children_count = stvector.count;

	for (id = 0; id < children_count; ++id) {      //DDD
	    rc = zoo_get_children(zh, path.c_str(), 0, vector);
	    if (rc != ZOK) {
		cout << '\n'<< "error in vector read loop with RC: " << rc << std::endl;
	    }
	    ccdbMutexes.push_back(ccdbState());
	    ccdbMutexes[id].uri = stvector.data[id]; 
       	    cout << '\n'<< "node persisted into vector: " << ccdbMutexes[id].uri << std::endl;

            // begin read node data

	    datapath = string(zookeeper_mutex_node) + "/" + stvector.data[id];
	    char buffer[1024];
	    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);

	    if (!zh) {
		return errno;
	    }

//	    buffer= "\0"; // DDD fix for user issue
	    strcpy(buffer, "");

	    int buflen = sizeof(buffer);
	    struct Stat stat;
	    rc = zoo_get(zh, datapath.c_str(), 0, buffer, &buflen, &stat);
	    if (rc != ZOK) {
		cout << '\n'<< "Mutex read failed with error code: " << rc << std::endl;
		return rc;
	    } else {
	    cout << '\n'<< "Mutex read: " << buffer << std::endl;
	    
	    }
	    
	    istringstream ss(buffer);

	    getline(ss, ccdbMutexes[id].published, '$');
	    getline(ss, ccdbMutexes[id].node, '$');
	    getline(ss, ccdbMutexes[id].user, '$');

	    cout << "and the data reads: published: " << ccdbMutexes[id].published 
		 << "node: " << ccdbMutexes[id].node
		 << "user: " << ccdbMutexes[id].user << std::endl;

        
// end read node data 

	}


// subject.replace(pos, search.length(), replace);


//	cout << '\n'<< "Number of node children std::vector " << ret.size()<<std::endl;
	cout << '\n'<< "Number of node children " << children_count<< std::endl;
	cout << '\n'<< "first element " << stvector.data[1] << std::endl;
	cout << '\n'<< "0 element " << stvector.data[0] << std::endl;

// and now as a member variable

	childrenCount=children_count;
    }


}


int CCDB_Global_Status::showSinglePublishStatus(std::string uri)
{
    cout << '\n'<< "DEBUG4 test from CCDB_Global_Status::showSingleStatus " << uri << std::endl;


    static zhandle_t *zh = NULL; 
    string path;
    string datapath;

/* Reformat CCNx URI as Zookeper node name*/

    uri = ReplaceString(uri, "_", "---");
    uri = ReplaceString(uri, "$", ":");

    cout << "DEBUG" << uri << std::endl;



/* Add mutex node path */
    path = string(zookeeper_mutex_node);


	    datapath = string(zookeeper_mutex_node) + "/" + uri;
	    char buffer[1024];
	    memset(buffer,0,strlen(buffer));

	    zh = zookeeper_init(zookeeper_hostnameport, watcher, 10000, 0, 0, 0);

	    if (!zh) {
		return errno;
	    }

	    int buflen = sizeof(buffer);
	    struct Stat stat;
	    rc = zoo_get(zh, datapath.c_str(), 0, buffer, &buflen, &stat);
	    if (rc != ZOK) {
		cout << '\n'<< "Mutex read failed with error code: " << rc << std::endl;
		cout << "dataphath: " << datapath.c_str() << std::endl;
		return rc;
	    } else {
	    cout << '\n'<< "Mutex read: " << buffer << std::endl;
	    
	    }
	    

	    ccdbMutexes.push_back(ccdbState());
	    ccdbMutexes[0].uri = uri;

//	    ccdbMutexes[0].uri = stvector.data[id]; 
       	    cout << '\n'<< "node persisted into vector: " << ccdbMutexes[0].uri << std::endl;


	    istringstream ssSingle (buffer);

	    getline(ssSingle, ccdbMutexes[0].published, '$');
	    getline(ssSingle, ccdbMutexes[0].node, '$');
	    getline(ssSingle, ccdbMutexes[0].user, '$');

	    cout << "and the data reads: published: " << ccdbMutexes[0].published 
	    	 << "node: " << ccdbMutexes[0].node
	    	 << "user: " << ccdbMutexes[0].user << std::endl;

        
// end read node data 

}



