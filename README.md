# csv-parser
Generates a large CSV file. Read the CSV file and generate JSON output


It has been compiled and tested on an Ubuntu PC.Here are the steps to run this tool.

1. Build InputFileGenerator.cpp first using the command.

       c++ InputFileGenerator.cpp
       
2. Run the executable to capture the input csv file. This step would create a data file (data.txt) of size ~10GB.

3. Build ToolCsv2JsonConverter.cpp using command

       c++ -pthread ToolCsv2JsonConverter.cpp

4. Run the executable. The JSON should be printed to standard output. We log the execution time for processing the CSV and logging the JSON as well.





Summary of implementation

       The csv file is loaded line by line in the main thread, as soon as it reaches 0.5 million lines, we pass the vector of the lines to RecordContainer. We create a thread and invoke a method of RecordContainer to build 0.5 m Record objects from those lines and creates a heap of the objects. We maintain an atomic bool in the RecordContainer to indicate whether creation of heap is complete or in progress.
       
       In the main thread we continue to read the lines and create threads as and when we reach multiples of 0.5 m lines.
       
       The main thread also pushes all these RecordContainer objects (smart pointers) in to a queue (first in first out). 
       
       After finish reading all the lines from the file, we close the file handle and start working on the queue.
       
       we iterate over the queue again and again with the following steps until the queue size gets to one.
       
       In each iteration we fetch two objects from queue (which are marked as not in processing mode) and merge them in a new thread and push it back to queue.
       
       When the queue size is one, it indicates that we merged all the RecordContainer Objects in to one Object wherein the Records are maintained in a Heap. We then sort all those objects from the heap and log to standard output.
