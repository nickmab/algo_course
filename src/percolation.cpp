#include <sstream>

#include "algo_lib/percolation.h"

namespace mabz { namespace percolation {

void Percolation::CreateNewConnections(int row, int col)
{
	// we assume here that the cell at this row,col address is open.
	const int idx = mN * (row-1) + (col-1);

	// cell immediately to the left...
	if (col > 1) 
	{
		if (mGrid[idx-1])
		{
			mConnections.Union(idx-1, idx);
		}
	}
	// cell immediately to the right...
	if (col < mN) 
	{
		if (mGrid[idx+1])
		{
			mConnections.Union(idx+1, idx);
		}
	}
	// cell immediately above...
	if (row > 1) 
	{
		if (mGrid[idx-mN])
		{
			mConnections.Union(idx-mN, idx);
		}
	}
	// cell immediately below...
	if (row < mN) 
	{
		if (mGrid[idx+mN])
		{
			mConnections.Union(idx+mN, idx);
		}
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
	, mGrid(n*n, false)
{
	if (n <= 0)
	{
		std::stringstream err;
		err << "Must construct percolation class with n > 0. Instead got " << n;
		throw IllegalArgumentException(err.str());
	}

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
	const int idx = mN * (row-1) + (col-1);
	if (!mGrid[idx])
	{
		mGrid[idx] = true;
		CreateNewConnections(row, col);
	}
}

bool Percolation::IsOpen(int row, int col) const 
{ 
	CheckRowColBounds(row, col);
	return mGrid[mN * (row-1) + (col-1)]; 
}

bool Percolation::IsFull(int row, int col) const 
{ 
	CheckRowColBounds(row, col);
	const int idx = mN * (row-1) + (col-1);
	
	if (!mGrid[idx]) return false;

	for (int i = 0; i < mN; i++)
	{
		if (mGrid[i] && mConnections.Connected(i, idx)) return true;
	}

	return false;
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
	const int idxStartOfBottomRow = mN * (mN-1);
	const int idxEndOfBottomRow = mN * mN - 1;
	for (int i = 0; i < mN; i++)
	{
		for (int j = idxStartOfBottomRow; j <= idxEndOfBottomRow; j++)
		{
			if (mConnections.Connected(i, j)) return true;	
		}
	}
	return false;
}

} /* namespace percolation */
} /* namespace mabz */
