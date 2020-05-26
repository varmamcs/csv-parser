#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<chrono> 
#include<queue>
#include<list>
#include<memory>
#include<thread>

#include "Record.hpp"
#include "RecordContainer.hpp"

using namespace std;
using namespace std::chrono; 

/*
 * @ToolCsv2JsonConverter
 * Read, Parse the Csv file and build the JSON output
 * The tool reads the Csv file in the main thread line by line and then creates
 * a separate thread to parse and build the RecordContainer each time after we
 * read MAX_RECORDS_PER_CONTAINER number of lines.
 *
 * Once the entire file is loaded, we iterate over RecordContainer list with two
 * records at a time and merge the containers, push them back to queue.
 * Repeat it until we get to one item in the queue
 */
class ToolCsv2JsonConverter
{
public:
	void processCsv(auto&& csvFile)
	{
		ifstream csv;
		csv.open(csvFile);

		vector<string> inputLines(MAX_RECORDS_PER_CONTAINER);
		auto idx = 0;
		while (getline(csv, inputLines[idx++]))
		{
			if (idx >= MAX_RECORDS_PER_CONTAINER)
			{
				// we dont clear the vector here as in the next iteration we
				// simply overwrite the elements anyway and save time on resize
				// the vector and reserve
				idx = 0;
				auto container = make_shared<RecordContainer>(inputLines);
				processQueue.push(container);
				threads.push_back(
					thread([container]{
						container->buildHeap();
					})
				);
			}
		}
		// some unprocessed lines still exist - last chunk of records
		if (idx != 0)
		{
			// skipping the last line in the input file as it was an empty line
			// otherwise we will need to add empty line validation in the
			// Record class
			inputLines.resize(idx - 1);
			auto container = make_shared<RecordContainer>(inputLines);
			processQueue.push(container);
			threads.push_back(
				thread([container]{
					container->buildHeap();
				})
			);
		}
		csv.close();

		// Reading input file is complete, now we have to start merging the containers in queue
		while(processQueue.size() > 1)
		{
			auto container = processQueue.front();
			processQueue.pop();
			if (container->isProcessing())
			{
				processQueue.push(container);
				continue;
			}

			while (processQueue.front()->isProcessing())
			{
				auto container2 = processQueue.front();
				processQueue.pop();
				processQueue.push(container2);
			}

			// at this stage we are sure we have two containers which are not in action
			auto container2 = processQueue.front();
			processQueue.pop();

			// Merge the container2 in to container1 in a thread
			container->startProcessing();
			container->extendReserve(*container2);
			threads.push_back(
				thread([container, container2]{
					container->mergeContainersAndHeapify(*container2);
				})
			);
			processQueue.push(container);
		}

		// wait until the last job finishes
		for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); } );

		auto container = processQueue.front();
		container->sortContainer();
	}

	void logJson()
	{
		if (processQueue.size() == 1)
		{
			auto container = processQueue.front();
			container->printContainerInJSON();
		}
		cout << "JSON is not generated yet!" << endl;
	}

private:
	queue<shared_ptr<RecordContainer>> processQueue;
	list<thread> threads;
};

int main()
{
	ToolCsv2JsonConverter tool;

	auto start = high_resolution_clock::now(); 
	tool.processCsv("data3.txt");
	auto stop = high_resolution_clock::now(); 

	// Json generated, now start logging the Json.
	tool.logJson();
	auto loggingstop = high_resolution_clock::now();

	auto duration = duration_cast<seconds>(stop - start);
	auto logJsonduration = duration_cast<seconds>(loggingstop - stop);

	cout << "Execution time : " << duration.count() << endl;
	cout << "Logging Json output time : " << logJsonduration.count() << endl;

	return 0;
}
