#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <algo_lib/union_find.h>

namespace mabz { namespace percolation {

class IllegalArgumentException : public std::runtime_error 
{
public:
    IllegalArgumentException(const std::string& msg) : std::runtime_error(msg) {}
};

class Percolation
{
private:
	mabz::UnionFind mConnections;

	// n x n grid, where position (row, col) is stored at
	// vector position (n * (row-1) + (col-1)); row/col indexing starts
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

} /* namespace percolation */
} /* namespace mabz */