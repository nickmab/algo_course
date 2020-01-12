#include <chrono>
#include <cmath>
#include <cstdlib> // rand
#include <iostream>
#include <sstream>
#include <vector>

#include "algo_lib/percolation.h"

namespace mabz { namespace percolation {

void Percolation::CreateNewConnections(int row, int col)
{
	// we assume here that the cell at this row,col address is freshly opened.
	const int idx = 1 + mN * (row-1) + (col-1);

	// important that when we call Union, we make the freshly opened one the 
	// first argument, as it will currently not be the root of anything.
	// (otherwise the Union function would go and needlessly change lots of roots under the hood).

	// cell immediately to the left...
	if (col > 1) 
	{
		if (mGrid[idx-1])
		{
			if (IsFull(row, col-1))
			{
				mConnections.Union(idx, 0);
				return;
			}
			else
			{
				mConnections.Union(idx, idx-1);
			}
		}
	}
	// cell immediately to the right...
	if (col < mN) 
	{
		if (mGrid[idx+1])
		{
			if (IsFull(row, col+1))
			{
				mConnections.Union(idx, 0);
				return;
			}
			else
			{
				mConnections.Union(idx, idx+1);
			}
		}
	}
	// cell immediately above...
	if (row > 1) 
	{
		if (mGrid[idx-mN])
		{
			if (IsFull(row-1, col))
			{
				mConnections.Union(idx, 0);
				return;
			}
			else
			{
				mConnections.Union(idx, idx-mN);
			}
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
			if (IsFull(row+1, col))
			{
				mConnections.Union(idx, 0);
				return;
			}
			else
			{
				mConnections.Union(idx, idx+mN);
			}
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
		throw IllegalArgumentException(err.str());
	}
	if (col < 1 || col > mN)
	{
		std::stringstream err;
		err << "Col index must be between 1 and " << mN << " inclusive. Got " << col;
		throw IllegalArgumentException(err.str());
	}
}

Percolation::Percolation(int n) 
	: mN(n)
	, mGrid(n*n + 2, false)
{
	if (n <= 0)
	{
		std::stringstream err;
		err << "Must construct Percolation class with n > 0. Instead got " << n;
		throw IllegalArgumentException(err.str());
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
		mConnections.Union(i, i);
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
		throw IllegalArgumentException(err.str());
	}

	std::cout << "Running percolation stats with n: " << n << " and trials: " << trials << std::endl;

	const double nSquared = n*n;
	std::vector<double> percolationThresholds(trials);
	double percolationThresholdCumSum{0};
	
	auto millisInOpen{0};
	auto millisInDoesPercolate{0};

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

		for (int iterCount = 1; ; iterCount++)
		{
			// randomly select a cell to open from the ones that are still closed...
			int idxToPop = rand() % remainingClosedRowColPairs.size();
			std::pair<int,int> rowColToOpen = remainingClosedRowColPairs[idxToPop];
			remainingClosedRowColPairs.erase(remainingClosedRowColPairs.begin() + idxToPop);

			// open that cell, see whether we've got a percolating grid or not...
			auto beginTime = std::chrono::steady_clock::now();
			percolation.Open(rowColToOpen.first, rowColToOpen.second);
			auto endTime = std::chrono::steady_clock::now();
			auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
			millisInOpen += millis;
			
			beginTime = std::chrono::steady_clock::now();
			if (percolation.DoesPercolate())
			{
				const double breakthroughRatio = static_cast<double>(iterCount) / nSquared;
				percolationThresholds[i] = breakthroughRatio;
				percolationThresholdCumSum += breakthroughRatio;
				endTime = std::chrono::steady_clock::now();
				millis = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
				millisInDoesPercolate += millis;
				break;
			}
			else
			{
				endTime = std::chrono::steady_clock::now();
				millis = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
				millisInDoesPercolate += millis;
			}
		}
	}

	std::cout << "Millis in Open: " << millisInOpen << std::endl;
	std::cout << "Millis in DoesPercolate: " << millisInDoesPercolate << std::endl;

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
