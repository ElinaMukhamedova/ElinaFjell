################################################################################
########################### Class for Tracing Execution ########################
################################################################################

class TraceRecord(object):
    """
    A class for storing the trace of an algorithm, to be exported and displayed
    using the HTML visualiser.
    """

    def __init__(self):
        """
        Initialise the trace to empty.
        
        RUNTIME: O(1)
        """

        self.sequence = []

    def getMaximum(self, arguments, maximum):
        """
        A function for recording the fact that the getMaximum function of
        some subproblem has been called.
        
        RUNTIME: O(1)
        """

        self.sequence.append({
            "type" : "findingMaximum",
            "coords" : arguments
        })

        self.sequence.append({
            "type" : "foundMaximum",
            "coord" : maximum
        })

    def getBetterNeighbour(self, neighbour, better):
        """
        A function for recording the fact that the getBetterNeighbour function
        of some subproblem has been called.

        RUNTIME: O(1)
        """

        self.sequence.append({
            "type" : "findingNeighbour",
            "coord" : neighbour
        })

        if (neighbour != better):
            self.sequence.append({
                "type" : "foundNeighbour",
                "coord" : better
            })
    
    def setProblemDimensions(self, subproblem):
        """
        A function for recording the fact that the dimensions of the currently
        studied subproblem have changed.

        RUNTIME: O(1)
        """

        self.sequence.append({
            "type" : "subproblem",
            "startRow" : subproblem.startRow,
            "numRows" : subproblem.numRows,
            "startCol" : subproblem.startCol,
            "numCols" : subproblem.numCols
        })

    def bestSeen(self, bestSeen):
        """
        A function for recording the fact that the variable "bestSeen" has been
        updated.

        RUNTIME: O(1)
        """

        self.sequence.append({
            "type" : "bestSeen",
            "coord" : bestSeen
        })
    
    def foundPeak(self, peak):
        """
        A function for recording the fact that the peak has been found.

        RUNTIME: O(1)
        """

        self.sequence.append({
            "type" : "foundPeak",
            "coord" : peak
        })