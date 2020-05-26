#ifndef RECORD_CONTAINER_HPP
#define RECORD_CONTAINER_HPP

#include<iostream>
#include<string>
#include<vector>
#include <algorithm>
#include <atomic>

#include "Record.hpp"

using namespace std;

#define MAX_RECORDS_PER_CONTAINER 500000

/*
 * @RecordContainer
 * This class constructs a container with Record objects from a vector of strings
 * It builds a Heap of the objects, exposes a wrapper to sort the heapified records
 * Also exposed a method to merge multiple containers.
 * 
 */
class RecordContainer
{
public:
	RecordContainer(const vector<string>& lines)
		: processing(false), maxColumnB(0.0), maxColumnC(0.0), inputLines(lines)
	{
		records.reserve(MAX_RECORDS_PER_CONTAINER);
	}

	bool isProcessing() const { return processing; }
	void startProcessing() { processing = true; }

	void buildHeap()
	{
		if (inputLines.size() > MAX_RECORDS_PER_CONTAINER)
		{
			// input lines exceeded the capacity
			// something wrong - this should never happen
			// we can throw exception here
			return;
		}

		for (unsigned int i = 0; i < inputLines.size(); ++i)
		{
			records.push_back( Record(inputLines[i]) );
			auto r = records.back();
			if (r.columnB() > maxColumnB)
			{
				maxColumnB = r.columnB();
			}
			if (r.columnC() > maxColumnC)
			{
				maxColumnC = r.columnC();
			}
			push_heap( records.begin(), records.end() );
		}
		finishedProcessing();
	}

	void mergeContainersAndHeapify(const RecordContainer& r)
	{
		auto newSize = records.size() + r.records.size();

		// extend the heap to include the new container
		records.reserve(newSize);
		records.insert(records.end(), r.records.begin(), r.records.end());
		make_heap(records.begin(), records.end());

		if (maxColumnB < r.maxColumnB)
		{
			maxColumnB = r.maxColumnB;
		}
		if (maxColumnC < r.maxColumnC)
		{
			maxColumnC = r.maxColumnC;
		}
		finishedProcessing();
	}

	// this should be called absolutely at the end
	// after all the containers merged
	void sortContainer()
	{
		sort_heap(records.begin(), records.end());
	}

	void printContainerInJSON()
	{
		double property2 = (maxColumnB + maxColumnC) / 2.0;
		cout << "{" << endl;
		cout << "\"array\": [" << endl;
		for_each(records.begin(), records.end(), [property2](auto& r) {
				r.printRecordInJson(property2);
			} );
		cout << "]" << endl;
		cout << "}" << endl;
	}
private:
	void finishedProcessing() { processing = false;}

	atomic<bool> processing;
	double maxColumnB, maxColumnC;

	const vector<string> inputLines;
	vector<Record> records;
};

#endif // RECORD_CONATINER_HPP
