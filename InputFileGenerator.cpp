#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <functional>
#include <random>

using namespace std;

/*
 * @InputFileGenerator
 * InputFileGenerator generates a file with mentioned size in GB.
 * 	Each line in the file has 10 columns with the following format
 *  	Column A contains string with 5-digit number suffixed (e.g. Test12345) - rand_int used to generate the suffix
 *		Column B & C needs to be of numeric type up to 3 decimal places, rand_double used to generate these two columns
 *		Column D-J needs to be of string type with alphanumeric values allowed.
 */
class InputFileGenerator
{
public:
	InputFileGenerator() :
    	rand_double(bind(uniform_real_distribution<>(0,1000), default_random_engine())),
    	rand_int(bind(uniform_real_distribution<>(10000,99999), default_random_engine()))
	{
	}

	~InputFileGenerator()
	{
	}

	void operator()(const string& filename, const unsigned int size)
	{
		const string line = generateLine();
		/*										   <---represents 1 GB equivalent--->  */
		unsigned long long maxIterations =  size * ((1024 * 1024 * 1024) / line.size());

		cout << "Generating input file with filename[" << filename << "] and size " << size << " GB" << endl;
		ofstream file;
		file.open(filename.c_str());
		if (file.is_open())
		{
			for (unsigned long long i = 0; i < maxIterations; ++i)
			{
				file << generateLine() << endl;
			}
			file.close();
		}
	}

private:
	const string generateLine()
	{
		stringstream str;
		str << "RandIntGenratedandthenDouble" << rand_int() << ","			// Column A
			<< rand_double() << ","											// Column B
			<< rand_double() << ","											// Column C
			<< "asjdhaksjdhsakjdhsakjdhaskjdhksajdhsakjdhsakjdhsadkjsahdksajdhsakduwr73624823648gjewrgwehgjhsfgdsjhfg23432432432432,"
			<< "ajkshdksajhdksajhdkjashduweiuqyewqiueywqieuywqieuywqieuywqieuywqieuywqieuywqieuywe872364283746832746eruyewtru432432,"
			<< "wuyreiwuyrddjsfgjshdgfuewyriuewyriewuyriewuyrewiuryewiuryewiuryewiuryewiuryewiuryjdsjhfgdsjf32473624832648324687364,"
			<< "asjhfkd,"													// Column G
			<< "aksjhdaskjd,"												// Column H
			<< "jkhkjhkj67765765765765hgfhfhgfhgfhggfhgfhgfhgfhgfhgfhgfhgfhgfhgfhgfhgfhgfghf32847832648736248732648736432784638274e,"
			<< "w38724jhgsdjhf";											// Column J
		
		return str.str();
	}

    function<double()> rand_double;
    function<int()> rand_int;
};

int main()
{
	InputFileGenerator generateInputFile;
	generateInputFile("data.txt", 10);
	return 0;
}
