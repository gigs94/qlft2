Compression Programming Project

Objective

Write a program that compresses a data file by taking advantage of patterns in the data. You should implement code that compresses the file with no data loss, and also decompresses the compressed output back to original form. Do not use any compression libraries or programs when implementing your solution. 

The file is in bid/ask/trade (BAT) format (attached file). BAT format is a CSV linefeed row-delimited text file where each row in the file comprises a tick. These files are typically several tens of gigabytes in size, so while the attached file is small, this fact should be taken into account when determining a good compression algorithm. The columns in the BAT format are (in order):

ticker	-variable-length string
exchange	-a single character indicating the host posting the event
side	-a single character indicating type of event
condition	-a single character indicating special condition on event
time	-time at which event occurred, in microseconds-after-midnight
reptime	-time at which the event was reported, in microseconds-after-midnight
price	-price on the event, in US dollars
size	-number of shares on the event

Example
CCC,i,B,0,42180525828,42180526502,13.06,730
KMPR,i,B,0,42180527447,42180528122,31.49,200
KMPR,i,T,0,42180533404,42180534083,31.55,100
KMPR,i,A,S,42180533404,42180534083,31.55,400
STIBX,i,A,0,42180550149,42180550822,27.04,500
CNX,i,B,0,42180555115,42180555793,35.87,436
FFG,i,B,0,42180562216,42180562892,8.85,4218
TES,i,B,0,42180564054,42180564732,10.25,7100

Deliverables
•	Source Code
•	Project for building program (if applicable)
•	Metrics demonstrating the compression results of the program.
•	Documentation and discussion of design, algorithm, assumptions and implementation

What we are looking for
•	Well-designed and implemented code
•	Well-tested code
•	Correct code
•	Good analysis and documentation where necessary
•	Compression ratio
•	Performance

We will test your code with gigabyte-sized files so please make sure your algorithm will run on files of this size in a reasonable amount of time and on a reasonably powered machine.

