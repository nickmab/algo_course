#include <chrono>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include <algo_lib/percolation.h>

void usage(const char* argv0, const std::string& error)
{
	if (!error.empty())
	{
		std::cerr << error << std::endl;
	}

	std::cerr << "Usage: " << std::endl;
	std::cerr << argv0 << " <n:int> <T:int>" << std::endl;
	std::cerr << "...where n is the side length of the square grid" << std::endl;
	std::cerr << "...and T is the number of random trials to run." << std::endl;
}

bool parseArgs(int argc, char* argv[], int& outN, int& outT)
{
	if (argc != 3)
	{
		usage(argv[0], std::string("Incorrect number of arguments provided."));
		return false;
	}

	try
	{
		outN = std::stoi(std::string(argv[1]));
	}
	catch (const std::exception& ex)
	{
		std::stringstream err;
		err << "Could not parse argument \"n\" as int: " << argv[1] << std::endl;
		err << "Reason: " << ex.what() << std::endl;
		usage(argv[0], err.str());
		return false;
	}

	try
	{
		outT = std::stoi(std::string(argv[2]));
	}
	catch (const std::exception& ex)
	{
		std::stringstream err;
		err << "Could not parse argument \"T\" as int: " << argv[2] << std::endl;
		err << "Reason: " << ex.what() << std::endl;
		usage(argv[0], err.str());
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	int n;
	int T;
	
	if (!parseArgs(argc, argv, n, T))
	{
		return 1;
	}

	std::cerr << "Writing to err" << std::endl;

	std::cout << "Successful parse." << std::endl;
	std::cout << "Running " << T << " random trials with grid side length " << n << std::endl;
	
	try
	{
		auto beginTime = std::chrono::steady_clock::now();
		mabz::percolation::PercolationStats pstats(n, T);
		auto endTime = std::chrono::steady_clock::now();

		std::cout << "Time taken: " 
				  << std::chrono::duration_cast<std::chrono::seconds>(endTime - beginTime).count() 
				  << " seconds" << std::endl;
		
		std::cout << "Mean: " << pstats.Mean() << std::endl;
		std::cout << "Stdev: " << pstats.Stdev() << std::endl;
		std::cout << "ConfidenceLow: " << pstats.ConfidenceLow() << std::endl;
		std::cout << "ConfidenceHigh: " << pstats.ConfidenceHigh() << std::endl;

		return 0;
	}
	catch (const mabz::percolation::IllegalArgumentException& ex)
	{
		std::cerr << "Attempted to run percolation stats with illegal arguments: " << std::endl;
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Unanticipated exception: " << std::endl;
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}