#pragma once

#include <functional>

namespace mabz { namespace sorts {

enum RWB { RED, WHITE, BLUE };

// Given an stl-like container with only Red, White and Blue balls in it
// (containing only RWB enum elements - see above).
// (total of n balls in any random order), sort the container such that 
// they're ordered Red, White Blue, with at most "n" swaps of ball position,
// at most "n" calls to check the colour of a ball and constant extra memory.
// Modifies the given container inplace.
// Returns the number of loops taken.
template <typename Container>
int DutchFlagSort(Container& c)
{
	std::function<void(int,int)> swap = [&] (int i, int j) ->void {
		const RWB swapVal = c[i];
		c[i] = c[j];
		c[j] = swapVal;
	};

	const int size = c.size();
	int leftRedPosition = -1; // all balls at this index and left of it are confirmed red.
	int rightBluePosition = size; // all balls at this index and right of it are confirmed blue.
	
	int inspectPosition = 0;
	int loopCount = 0;
	while (inspectPosition < rightBluePosition)
	{
		++loopCount;
		const RWB colour = c[inspectPosition];
		
		if (colour == RWB::RED)
		{
			// if the following is false, then this cell is already in the correct sort order.
			if (inspectPosition > (leftRedPosition + 1))
			{
				// The colour of the incoming swapped cell must be white.
				// If it were Blue, we'd have sent it forward. If it were red, we wouldn't 
				// be looking at it in this swap check - we don't re-check confirmed reds.
				// It must be white. As such, we can safely increment the inspection 
				// counter forward without even bothering to check it!
				swap(inspectPosition, leftRedPosition + 1);
			}
			inspectPosition++;
			leftRedPosition++;
		}
		else if (colour == RWB::BLUE)
		{
			// if the following is false, then this cell is already in the correct sort order...
			// ...and we are finished.
			if (inspectPosition < (rightBluePosition - 1))
			{
				
				swap(inspectPosition, rightBluePosition - 1);
				rightBluePosition--;
			}
			else
			{
				break;
			}
		}
		else /* it's white */
		{
			// if it's white, we don't know what to do with it, so no swap is
			// necessary at this time.
			inspectPosition++;
		}
	}

	return loopCount;
}

} /* namespace sorts */
} /* namespace mabz */