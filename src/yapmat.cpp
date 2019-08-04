#include <cstddef>
#include <unistd.h>

#include <bits/stdc++.h>

using namespace std;

string YDIR = "/tmp/yapmat/";

static void yapmat_manage(string name) {

    if (name == "") {
        // name = get_random_name();
    }

    // Create directory $YDIR/$name

    // fork() & exec() the wrapper command
    // passing all the right parameters to it

}

static void yapmat_list() {

    // List all directories under YDIR

    // Perhaps even print the contents of $YDIR/$proc/cmd

}

static void yapmat_logs(string id) {

    if (id == "all") {

        // Iterate, read, and print $YDIR/$proc/stdout (err?)

    } else {

        // Read & print $YDIR/<id>/stdout (err?)

    }

}

static void yapmat_status(string id) {

    if (id == "all") {

        // Iterate over $YDIR/$proc
        // and print the status - running, stopped

    } else {

        // Just print status of $YDIR/<id>

    }

}

static void yapmat_start(string id) {

    // How does manage differ from start?

    // echo "running" > $YDIR/$id/status

}


static void yapmat_stop(string id) {

    // Do not delete the directory of the process

    // Send SIGKILL to $YDIR/$id/wpid

    // echo "stopped" > $YDIR/$id/status
}


static void yapmat_restart(string id) {

    yapmat_stop(id);

    // Ensure that process isn't running anymore?

    yapmat_start(id);

}

static void yapmat_unmanage(string id) {

    // Stop the running process
    yapmat_stop(id);

    // Delete the directory $YDIR/$id

}

static void yapmat_kill(string id, int signum) {

    // Create a signal handler in the wrapper program
    // that forwards all signals received to the child program

    // Read process id from $YDIR/$id/wpid

    // Send signal signum to

}

int main(int argc, char* argv[])
{

    if (argc < 2) {
        cerr << "No extra arguments passed" << endl;
        exit(1);
    }

    // if/else chain - call right function based on the
    // arguments that are passed

}
