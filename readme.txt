**********************************************************************
 *  readme.txt template                                                   
**********************************************************************

Peter Nguyen 
Developed in Ubuntu using the G++ GNU compiler.
Dependencies: SFML --to install: sudo apt-get install libsfml-dev
              Boost --to install: sudo apt-get install libboost-all-dev

**********************************************************************

This program uses regex to parse a Kronos log to detect boot starts, completion status, and boot time.

e = regex("\\(log\\.c\\.166\\) server started");
This regex is used to capture the server start message to indicate when a device has booted up. It captures "(log.c.166) server started" specifically.

d = regex("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
This regex is used the capture the date and time on the line. It captures 4 digits, a dash, 2 digits, dash, 2 digits, space, 2 digits, semicolon, 2 digits, semicolon, 2 digits. This follows the format of the date and time that used on the provided logs. It uses this regex and returns the match to create the ptime object used to calculate that elapsed time from start to end of a boot.

c = regex("\\w{4}.(AbstractConnector):(Started) (SelectChannelConnector)@0.0.0.0:9080");
This regex is used to capture the message "oejs.AbstractConnector:Started SelectChannelConnector@0.0.0.0:9080". This is used to look for an indication that a boot completed.


**********************************************************************

to run:
./parse (log file)
example:
./parse device1_intouch.log

output file: device1_intouch.log.rpt




