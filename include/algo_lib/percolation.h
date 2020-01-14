#pragma once

#include <exception>
#include <string>
#include <vector>

#include <algo_lib/union_find.h>

namespace mabz { namespace percolation {

class Percolation
{
private:
	mabz::UnionFind mConnections;

	// n x n grid, PLUS an "entry" node and an "exit" node, 
    // where the 0th position in the vector is the entry node,
    // grid position (row, col) is stored at vector position 
    // 1 + n * (row-1) + (col-1) and position n^2 + 1 is the exit node.
	// from 1 whereas obviously vector indexing starts from 0.
	// true for open, false for closed.
	int mN;
	std::vector<bool> mGrid;

	// To be called after opening a previously closed cell in the grid.
	// Expects row and col indices to already be checked/validated.
	void CreateNewConnections(int row, int col);

	// Throw IllegalArgumentException if out of bounds (1-mN inclusive).
	void CheckRowColBounds(int row, int col) const;

public:
    // creates n-by-n grid, with all sites initially blocked
    Percolation(int n);

    // Lets you clear everything and start again if you want.
    void ResetGrid();

    // opens the site (row, col) if it is not open already
    void Open(int row, int col);

    // is the site (row, col) open?
    bool IsOpen(int row, int col) const;

    // is the site (row, col) full? "Full" means it's connected
    // to an open site in the top row.
    bool IsFull(int row, int col) const;

    // returns the number of open sites
    int GetNumberOfOpenSites() const;

    // does the system percolate? 
    // true if there is a full site in the bottom row.
    bool DoesPercolate() const;
};

class PercolationStats 
{
private:
    double mMean;
    double mStdev;
    double mConfidenceLow;
    double mConfidenceHigh;

public:
    // perform independent trials on an n-by-n grid
    PercolationStats(int n, int trials);

    // sample mean of percolation threshold
    double Mean() const { return mMean; }

    // sample standard deviation of percolation threshold
    double Stdev() const { return mStdev; }

    // low endpoint of 95% confidence interval
    double ConfidenceLow() const { return mConfidenceLow; }

    // high endpoint of 95% confidence interval
    double ConfidenceHigh() const { return mConfidenceHigh; }
};

} /* namespace percolation */
} /* namespace mabz */