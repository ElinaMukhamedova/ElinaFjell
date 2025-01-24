import myTrace

################################################################################
########################### Class for Peak Problems ############################
################################################################################

class PeakProblem(object):
    """
    A class representing an instance of a peak-finding problem.
    """

    def __init__(self, array, bounds):
        """
        A method for initialising an instance of the PeakFinding class.
        Takes an array and an argument indicating which rows to include.

        RUNTIME: O(1)
        """

        (startRow, startCol, numRows, numCols) = bounds

        self.array = array
        self.bounds = bounds
        self.startRow = startRow
        self.startCol = startCol
        self.numRows = numRows
        self.numCols = numCols

    def get(self, location):
        """
        Returns the value of the array at the given location, offset by
        the coordinates (startRow, startCol).

        RUNTIME: O(1)
        """

        (r, c) = location
        if not (0 <= r and r < self.numRows):
            return 0
        if not (0 <= c and c < self.numCols):
            return 0
        return self.array[self.startRow + r][self.startCol + c]
    
    def getBetterNeighbour(self, location, trace = None):
        """
        If (r, c) has better neighbour, return the neighbour. Otherwise,
        return the location (r, c).

        RUNTIME: O(1)
        """

        (r, c) = location
        best = location

        if r - 1 >= 0 and self.get((r - 1, c)) > self.get(best):
            best = (r - 1, c)
        if c - 1 >= 0 and self.get((r, c - 1)) > self.get(best):
            best = (r, c - 1)
        if r + 1 < self.numRows and self.get((r + 1, c)) > self.get(best):
            best = (r + 1, c)
        if c + 1 < self.numCols and self.get((r, c + 1)) > self.get(best):
            best = (r, c + 1)

        if not trace is None: trace.getBetterNeighbour(location, best)

        return best
    
    def getMaximum(self, locations, trace = None):
        """
        Finds the location in the current problem with the greatest value.

        RUNTIME: O(len(locations))
        """

        (bestLoc, bestVal) = (None, 0)

        for loc in locations:
            if bestLoc is None or self.get(loc) > bestVal:
                (bestLoc, bestVal) = (loc, self.get(loc))
        
        if not trace is None: trace.getMaximum(locations, bestLoc)

        return bestLoc
    
    def isPeak(self, location):
        """
        Returns true if the given location is a peak in the current subproblem.

        RUNTIME: O(1)
        """

        return (self.getBetterNeighbour(location) == location)
    
    def getSubproblem(self, bounds):
        """
        Returns a subproblem with the given bounds. The bounds is a quadruple
        of numbers: (starting row, starting column, # of rows, # of columns).

        RUNTIME: O(1)
        """

        (sRow, sCol, nRows, nCols) = bounds
        newBounds = (self.startRow + sRow, self.startCol + sCol, nRows, nCols)
        return PeakProblem(self.array, newBounds)
    
    def getSubproblemContaining(self, boundsList, location):
        """
        Returns the subproblem containing the given location. Picks the first
        of the subproblems in the list which satisfies that constraint, and
        then constructs the subproblem using getSubproblem().

        RUNTIME: O(len(boundsList))
        """

        (row, col) = location
        for (sRow, sCol, nRows, nCols) in boundsList:
            if sRow <= row and row < sRow + nRows:
                if sCol <= col and col < sCol + nCols:
                    return self.getSubproblem((sRow, sCol, nRows, nCols))
                
        # shouldn't reach here
        return self
    
    def getLocationInSelf(self, problem, location):
        """
        Remaps the location in the given problem to the same location in
        the problem that this function is being called from.

        RUNTIME: O(1)
        """

        (row, col) = location
        newRow = row + problem.startRow - self.startRow
        newCol = col + problem.startCol - self.startCol
        return (newRow, newCol)

################################################################################
################################ Helper Methods ################################
################################################################################

def getDimensions(array):
    """
    Gets the dimensions for a two-dimensional array. The first dimension
    is simply the number of items in the list; the second dimension is the
    length of the longest row.

    RUNTIME: O(len(array))
    """

    rows = len(array)
    cols = 0

    for row in array:
        if len(row) > cols:
            cols = len(row)
    
    return (rows, cols)

def createProblem(array):
    """
    Constructs an intance of the PeakProblem object for the given array,
    using bounds derived from the array using the getDimensions function.

    RUNTIME: O(len(array))
    """

    (rows, cols) = getDimensions(array)
    return PeakProblem(array, (0, 0, rows, cols))