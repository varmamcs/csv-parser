# csv-parser
Generates a large CSV file. Read the CSV file and generate JSON output


It has been compiled and tested on an Ubuntu.

1. Build InputFileGenerator.cpp first using the command
       '''c++ InputFileGenerator.cpp '''
       
2. Run the executable to capture the input csv file.

3. Build ToolCsv2JsonConverter.cpp using command
       '''c++ -pthread ToolCsv2JsonConverter.cpp '''

4. Run the executable. The JSON should be printed to std out with execution time at the end.

