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
       The csv file is loaded line by line in the main thread, as soon as it reaches 0.5 million lines, we pass the vector of the lines to RecordContainer. The RecordContainer builds 0.5 m Record objects from those lines and creates a heap of the objects.

