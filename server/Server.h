

#ifndef SERVER_H
#define SERVER_H

#include <string>

#include "Date.h"
#include "Patient.h"
#include "WorkerThread.h"
#include "Queue.h"
#include "MasterWorker.h"

using namespace std;

class Server {
public:

    static WorkerThread * workerThreads;
    
    static MasterWorker* masterWorkers;
    
    static Queue * queue;
    
    static pthread_t * tids;

    static Patient* searchPatientRecord(string id);
    
    static int * connectToMasterWorkers();
    
    static int * connectToMasterWorkers(string country);

    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

    static void diseaseFrequency(int reply_fd, string whatDisease, Date from, Date to, string country);

    static void diseaseFrequency(int reply_fd, string whatDisease, Date from, Date to, int numWorkers);
    
    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

    static void numPatientAdmissions(int reply_fd, string whatDisease, Date from, Date to, string country);

    static void numPatientAdmissions(int reply_fd, string whatDisease, Date from, Date to, int numWorkers);

    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

    static void numPatientDischarges(int reply_fd, string whatDisease, Date from, Date to, string country);

    static void numPatientDischarges(int reply_fd, string whatDisease, Date from, Date to, int numWorkers);

    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

    static void topkAgeRanges(int reply_fd, int k, string whatDisease, Date from, Date to, string country);

    ///////////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~```

    static WorkerThread * setupCommunication(int statisticsPortNum, int queryPortNum, int numWorkers, int poolSize);

    static void shutdown(int numWorkers);
    
    static void *child();

    static void serviceMaster(WorkerThread * workers, int numWorkers, int b);
    
    static void serviceClients(WorkerThread * workers, int numWorkers, int b);
    

    static void closeCommunication(int numWorkers, WorkerThread * workers);

    static bool shutdown_flag;
    static pthread_mutex_t mvar;
};
#endif

