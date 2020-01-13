#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "algo_lib/exceptions.h"
#include "algo_lib/percolation.h"

namespace mabz { namespace percolation {

void Percolation::CreateNewConnections(int row, int col)
{
	// we assume here that the cell at this row,col address is freshly opened.
	const int idx = 1 + mN * (row-1) + (col-1);

	// cell immediately to the left...
	if (col > 1) 
	{
		if (mGrid[idx-1])
		{
			mConnections.Union(idx, idx-1);
		}
	}
	// cell immediately to the right...
	if (col < mN) 
	{
		if (mGrid[idx+1])
		{
			mConnections.Union(idx, idx+1);
		}
	}
	// cell immediately above...
	if (row > 1) 
	{
		if (mGrid[idx-mN])
		{
			mConnections.Union(idx, idx-mN);
		}
	}
	else
	{
		// cell in the top row; connect to the entry node!
		mConnections.Union(idx, 0);
	}
	// cell immediately below...
	if (row < mN) 
	{
		if (mGrid[idx+mN])
		{
			mConnections.Union(idx, idx+mN);
		}
	}
	else
	{
		// cell in the bottom row; connect to the exit node!
		mConnections.Union(idx, mN*mN + 1);
	}
}

void Percolation::CheckRowColBounds(int row, int col) const
{
	if (row < 1 || row > mN)
	{
		std::stringstream err;
		err << "Row index must be between 1 and " << mN << " inclusive. Got " << row;
		throw mabz::IllegalArgumentException(err.str());
	}
	if (col < 1 || col > mN)
	{
		std::stringstream err;
		err << "Col index must be between 1 and " << mN << " inclusive. Got " << col;
		throw mabz::IllegalArgumentException(err.str());
	}
}

Percolation::Percolation(int n) 
	: mConnections(n*n + 2)
	, mN(n)
	, mGrid(n*n + 2, false)
{
	if (n <= 0)
	{
		std::stringstream err;
		err << "Must construct Percolation class with n > 0. Instead got " << n;
		throw mabz::IllegalArgumentException(err.str());
	}

	ResetGrid();
}

void Percolation::ResetGrid()
{
	mConnections.Reset();

	// add the value/index number of every cell in the grid to our union,
	// with nothing connected to anything else.
	const auto gridSize = mGrid.size();
	for (int i = 0; i < gridSize; ++i)
	{
		mGrid[i] = false;
	}
}

void Percolation::Open(int row, int col)
{
	CheckRowColBounds(row, col);
	const int idx = 1 + mN * (row-1) + (col-1);
	if (!mGrid[idx])
	{
		mGrid[idx] = true;
		CreateNewConnections(row, col);
	}
}

bool Percolation::IsOpen(int row, int col) const 
{ 
	CheckRowColBounds(row, col);
	return mGrid[1 + mN * (row-1) + (col-1)]; 
}

bool Percolation::IsFull(int row, int col) const
{ 
	CheckRowColBounds(row, col);
	return mConnections.Connected(0, 1 + mN * (row-1) + (col-1));
}

int Percolation::GetNumberOfOpenSites() const
{
	int count{0};
	for (const auto& b : mGrid)
	{
		count += b;
	}
	return count;
}

bool Percolation::DoesPercolate() const
{
	return mConnections.Connected(0, mN*mN + 1);
}

PercolationStats::PercolationStats(int n, int trials)
{
	if (n <= 0 || trials <= 0)
	{
		std::stringstream err;
		err << "Must construct PercolationStats with positive n and trials. "
		    << "Instead, got n: " << n << " and trials: " << trials; 
		throw mabz::IllegalArgumentException(err.str());
	}

	std::cout << "Running percolation stats with n: " << n << " and trials: " << trials << std::endl;

	const double nSquared = n*n;
	std::vector<double> percolationThresholds(trials);
	double percolationThresholdCumSum{0};
	
	auto randEng = std::default_random_engine{};

	// First we generate all the random trials...
	for (int i = 0; i < trials; i++)
	{
		std::cout << "Trial number: " << i << std::endl;

		Percolation percolation(n);

		std::vector<std::pair<int,int> > remainingClosedRowColPairs;
		for (int r = 1; r <= n; r++)
		{
			for (int c = 1; c <= n; c++)
			{
				remainingClosedRowColPairs.emplace_back(r, c);
			}
		}

		// shuffle randomly
		std::shuffle(std::begin(remainingClosedRowColPairs), std::end(remainingClosedRowColPairs), randEng);

		int iterCount = 1;
		for (auto& rowColToOpen : remainingClosedRowColPairs)
		{
			// open that cell, see whether we've got a percolating grid or not...
			percolation.Open(rowColToOpen.first, rowColToOpen.second);
			if (percolation.DoesPercolate())
			{
				const double breakthroughRatio = static_cast<double>(iterCount) / nSquared;
				percolationThresholds[i] = breakthroughRatio;
				percolationThresholdCumSum += breakthroughRatio;
				break;
			}

			iterCount++;
		}
	}

	// Now we calculate the statistics
	mMean = percolationThresholdCumSum / trials;

	double sumSqMeanDeviations{0};
	for (const auto& t : percolationThresholds)
	{
		const double diff = t - mMean;
		sumSqMeanDeviations += diff*diff;
	}
	mStdev = std::sqrt(sumSqMeanDeviations / (trials-1));

	const double confidenceHalfWidth = 1.96 * mStdev / std::sqrt(trials);
	mConfidenceLow = mMean - confidenceHalfWidth;
	mConfidenceHigh = mMean + confidenceHalfWidth;
}

} /* namespace percolation */
} /* namespace mabz */
