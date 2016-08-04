
# Design #

The problem calls for creating an algorithm that will enable modification.   We are not supposed to use existing algorithms or programs.   This leads me to believe that it's the process of thinking through the problem that is at the core of this exercise.   We should look through the data and determine characteristics of the data and apply these ideas to our code and see if things work or not.   TDD could be used to drive these ideas into a better solution.  My guy reaction is to think of a modular design, analyze the data, make some inferences of from the statistical characteristics and then write some tests and code.


## Modular Design ##

Based on the feedback from assignment 1, it pays to be modular.   We can take the code that we wrote for assignment 1 to read the csv file as well as do some of the StocksProcessor and StockAverager code and modify it for this purpose.   That will save some time and we already know that it processes different fields in the file.   We should modularize this design to be able to construct the row for different types of data... maybe, we are getting a little ahead of ourselves... lets go to the next subject and interate through all the pieces before we design the entire thing before anything else...

Chose to go through the design with the though that it could be easily expanded and modified.   Worked on several different compression types and made the code relatively easy to modify to corrolate types with column data (yes, I choose to do column compression for this exercise... seemed like a good thing to do based on my exploration of the data).

## Exploration ##

Should be things out there that we can learn from.  What are the best compression 'ideas'?   Most things point to the use of stuff like Huffman so we aren't going to look at those.  But what are the things that we can learn?   Knowing what I know about compression, there are several things that we should consider.   We can 'slice' the data several ways:  row, column, block.   The assignment says it needs to run on LARGE files but it doesn't limit to loading the file into memory.  If we had a hard constraint about this method then we would put more emphasis into the block concept.   

Compression can take many forms... for example, we can compress this file and still keep it ascii-based, it wont be as compact as binary, but it will be a cleaner format that is still human readable.   We could then take that ascii and push it into a binary format that would get us some better compression stats.


### Statistics of the Data ##

| ----------- | ----------------------------- |
| Column Name | Unique values for each column | 
| ----------- | ----------------------------- |
| ticker      | 799 |
| exchange    | 12 |
| side        | 3 |
| condition   | 6 |
| time        | 213548 |
| reptime     | 259505 |
| price       | 8993 |
| size        | 1748 |


 1. There are type of data here that we can exploit.   meaning, we know the data and it's structured so we can figure out differences in each piece of data and use that to store information instead of trying to find [repetitive] patterns in the data.   For example, can we use the delta-time between the previous entry and the next entry and between the reptime?   If these times are short enough then we can make those smaller ints and use that.   Since we are not trying to uncompress a single row in the middle (and more specifically, there are no performance requirements for that use case), we don't have to worry about having to start at the beginning of the file and walk through everything to recontruct the original data.

#### Questions about the instructions ####

 1. we have to uncompress the file, but does it have to be in the same order?   Interestingly, we can probably get some better compression stats if we can sort certain things like the times.
 1. why are some of the reptimes BEFORE the time values??   Are these errors in the data or is that a characteristic of the system that we need to take into account in the design?
 1. does whitespace count?   the input file does not have a newline character on the last line... if I add one, it doesn't change the meaning of data in any shape but it isn't the same file that it originally was.   Also, what about \r\n vs \n?   does that matter for an ascii file?   maybe on some systems but most modern systems can auto determine the line endings.
 1. why is there a huge time jump in the negative direction in the middle of the file??  seems odd that it's exactly in the middle at line 200000.... maybe I should sort the data and do some of my calcs... would be a lot cleaner but then I'd have to reconstruct the position.... hmmm.


## Column ##

Knowing what I know from some specialty databases that I have used, column compression can save LOTS of space for data that has similar structure and form like this data seems to have.  I will probably start by analyzing the contents of this data and look at the statistical characteristics of the data.   Hopefully the sample of data that is presented here is a statistical representation of the real file that will be used.

This is the direction that I ultimately choose to explore.  It allowed me to develop a lot of classes and showed some of my object oriented skills... just need to get the algorithms right for each type of data so that it's optimal.



## Row ##

Row compression is probably the least interesting form to look at... unless we were just going to do a hellman algo or something similar.


## Block ##

Block is a special form, I think.   We can make the block size be the entire file... or it can be a fixed number of records.   Either way, it can be a higher level than either of the other methods and should be able to be use in conjunction with the data.




## Design Continued ##

Seems like we should go for the hybrid approach... be able to compress a column, or row, as well as to be able to label field types (like time) and be able to create offsets for that.   That should be easy to implement first.


So the design should all the definition of fields based on column types, and to be able to assign a compression type to that data.  Then we can create the header with the field types and the seed values and then populate the compressed data.
