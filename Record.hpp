#ifndef RECORD_HPP
#define RECORD_HPP

#include<iostream>
#include<string>

using namespace std;

/*
 * @Record
 * Record represents an input line from the CSV file
 * in a format which is ideal for converting it in to the JSON format
 * we read the CSV line and parse the necessary property values
 */
class Record
{
public:
	Record(const string& s)
	{
		auto start = 0;
		auto token = 0;
		while (start != string::npos)
		{
			++token;
			auto end = s.find(',', start + 1);

			switch (token)
			{
			case 1:
				property3 = s.substr(start, end);
				property1 = stoi(property3.substr(property3.size() - 5));
				break;

			case 2:
				dColumnB = stod(s.substr(start + 1, end - start - 1));
				break;

			case 3:
				dColumnC = stod(s.substr(start + 1, end - start - 1));
				break;

			case 7:
				property4 = s.substr(start + 1, end - start - 1);
				break;

			case 8:
				property5 = s.substr(start + 1, end - start - 1);
				break;

			case 10:
				property3 += s.substr(start+1);
				break;

			default:
				break;
			};

			start = end;
		}
	}

	bool operator<(const Record& r)
	{
		if (property1 < r.property1)
		{
			return true;
		}
		if (property1 == r.property1)
		{
			if (property4 < r.property4)
			{
				return true;
			}
			if (property4 == r.property4)
			{
				return (property5 < r.property5);
			}
		}
		return false;
	}

	void printRecordInJson(double columnBCMaxAvg)
	{
		property2 = columnBCMaxAvg;
		cout << "{" << endl
			<< "\"property1\":\"" << property1 << "\"," << endl
			<< "\"property2\":\"" << property2 << "\"," << endl
			<< "\"property3\":\"" << property3 << "\"," << endl
			<< "\"property4\":\"" << property4 << "\"," << endl
			<< "\"property5\":\"" << property5 << "\"" << endl
			<< "}" << endl;
	}

	double columnB() { return dColumnB; }
	double columnC() { return dColumnC; }

private:
	int property1;
	double dColumnB;
	double dColumnC;
	double property2;
	string property3;
	string property4;
	string property5;
};

#endif // RECORD_HPP
