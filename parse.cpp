// Copyright 2021 Peter Nguyen
/*
Last Updated: 12/3/21
parse.cpp: This program is to parse logs from the Kronos device.
           It utilizes regex and reads in file indicated by a command
           line argument. It looks for successful or failed boots and 
           outputs a files with information on the starts.
*/
#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost;
using std::cout;
using std::endl;
using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;

int main (int argc, char** argv) {
    // Input file
    std::ifstream read(argv[1]);
    // Make output file name, convert argv to string and concatenate
    std::string f = argv[1];
    f = f + ".rpt";
    std::ofstream output(f);

    std::string line;
    regex e, d, c;
    bool boot = false;

    if(read.fail()) {
        std::cout << "Cannot open file";
        return 1;
    }
    
    // Regex declarations
    try {
        e = regex("\\(log\\.c\\.166\\) server started");
        d = regex("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
        c = regex("\\w{4}.(AbstractConnector):(Started) (SelectChannelConnector)@0.0.0.0:9080");
    } catch (regex_error& exc) {
        cout << "Regex constructor failed with code " << exc.code() << endl;
        exit(1);
    }

    // Keep track of line number
    int lineNum = 1;

    // Boost variables
    boost::smatch sm;
    ptime t1, t2;
    time_duration td;

    while (getline(read, line)){
        // Incomplete boot
        if (regex_search(line, e) && (boot == true)) {
            output << "**** Incomplete Boot ****\n\n";
            boot = false;
        }
        
        // Start of boot
        if ((regex_search(line, e)) && (boot == false)) {
            output << "=== Device Boot ===\n";
            output << lineNum << "(" << argv[1] << "): ";
            // Look for regex match for date/time
            regex_search(line, sm, d);
            t1 = (boost::posix_time::time_from_string(sm[0]));
            output << to_iso_extended_string(t1) << " Boot Start\n";
            boot = true;
        }

        // Completed boot
        else if(regex_search(line, c) && (boot == true)) {
            output << lineNum << "(" << argv[1] << "): ";
            // Look for regex match for date/time
            regex_search(line, sm, d);
            t2 = (boost::posix_time::time_from_string(sm[0]));
            output << to_iso_extended_string(t2) << " Boot Completed\n";
            td = t2-t1;
            output << "\tBoot Time: " << td.total_milliseconds() << "\n\n";
            boot = false;
        }
        lineNum++;
    }
    
    // Incomplete boot due to end of file
    if (boot == true) {
        boot = false;
        output << "**** Incomplete Boot ****\n\n";
    }

    output.close();
    read.close();
}
