import java.lang.IndexOutOfBoundsException;
import java.lang.IllegalArgumentException;

/**
 * Pathfinder uses A* search to find a near optimal path
 * between to locations with given terrain.
 */

public class Pathfinder {

    private PFNode[][] PFNMap;
    private int[][] trackMap;
    private MinPQ<PFNode> minPQ;
    private int N, numLocations; // number of locations that were looked at (placed on priority queue) while finding path
    private float heuristic; // search heuristic
    private Coord startLocation, endLocation;
    private PFNode start, end;
    private Terrain terrain;
    private Iterable<Coord> solution;

    /**
     * PFNode will be the key for MinPQ (used in computePath())
     */
    private class PFNode implements Comparable<PFNode> {
        PFNode fromNode, first;
        Coord loc;

        // loc: the location of the PFNode
        // fromNode: how did we get here? (linked list back to start)
        public PFNode(Coord loc, PFNode fromNode) {
            this.fromNode = fromNode;
            this.loc = loc;
            first = fromNode.neighbors().iterator().next();
        }

        // compares this with that, used to find minimum cost PFNode
        public int compareTo(PFNode that) {
            return 0;
            // if this. getCost is less than that .getCost then return -1
            // so on and so forth
            // how do you get the cost without a heuristic? do you need heuristic?
        }

        // returns the cost to travel from starting point to this
        // via the fromNode chain
        public float getCost(float heuristic) {
            return 0;
        }

        // returns if this PFNode is still valid
        //public boolean isValid() { return false; }

        // invalidates the PFNode
        //public void invalidate() { }

        // returns if the PFNode has been used
        //public boolean isUsed() { return false; }

        // uses the PFNode
        // public void use() { }

        // returns an Iterable of PFNodes that surround this
        public Iterable<PFNode> neighbors() {
            Stack<PFNode> s = new Stack<>();
            s.push(new PFNode(null, null));
            return s;
        }
    }

    // takes terrain obj
    public Pathfinder(Terrain terrain) {
        PFNMap = new PFNode[terrain.getN()][terrain.getN()];
        N = terrain.getN();
        this.terrain = terrain;
        minPQ = new MinPQ<PFNode>();
        trackMap = new int[terrain.getN()][terrain.getN()];
        numLocations = 0;
        heuristic = 0;
        startLocation = null;
        endLocation = null;
        start = null;
        end = null;
        solution = null;
    }

    // set the start location
    // illegalArgumentException if Coord is null
    // indexOutOfBoundsException if Coord is outside acceptable range (use isInBounds())
    public void setPathStart(Coord loc) {
        if (loc == null) throw new IllegalArgumentException();
        if (!loc.isInBounds(0, N, 0, N)) throw new IndexOutOfBoundsException();
        start = new PFNode(loc, null);
        this.startLocation = loc;
        PFNMap[loc.getI()][loc.getJ()] = start;
        minPQ.insert(start); numLocations++;
        trackMap[loc.getI()][loc.getJ()] = 1;
    }

    // return the start location
    public Coord getPathStart() {
        return startLocation;
    }

    // set the ending location
    // illegalArgumentException if Coord is null
    // indexOutOfBoundsException if Coord is outside acceptable range (use isInBounds())
    public void setPathEnd(Coord loc) {
        end = new PFNode(loc, null);
        if (loc == null) throw new IllegalArgumentException();
        if (!loc.isInBounds(0, N, 0, N)) throw new IndexOutOfBoundsException();
        this.endLocation = loc;
        PFNMap[loc.getI()][loc.getJ()] = end;
        minPQ.insert(end); numLocations++;
        trackMap[loc.getI()][loc.getJ()] = 1;
    }

    // return the end location
    public Coord getPathEnd() {
        return endLocation;
    }

    // set the search heuristic
    public void setHeuristic(float v) {
        this.heuristic = v;
    }

    // return the search heuristic
    public float getHeuristic() {
        return this.heuristic;
    }

    // clear out any path information, called before computePath() is called
    public void resetPath() {
        //reset list of nodes
    }

    // simplest method of solving this problem is to ignore the travel costs + just find a path
    // throw illegalArgumentException if is called before start/end locations are set
    // implementation must use linear space, no worse than ~aN^2lgN time (a - 15%)
    public void computePath() {
        if (start == null || end == null) throw new IllegalArgumentException();

    }

    // returns true if path was found
    public boolean foundPath() { return false; }

    // returns total (non-heuristic) cost of traveling along the path
    public float getPathCost() { return terrain.computeTravelCost(solution); }

    // useful for debugging
    // returns number of locations that were looked at (placed on priority queue) while finding path
    public int getSearchSize() { return numLocations; }

    // returns Iterable<Coord> that iterates along the path from start to end
    // used to draw the path in PathfinderVisualizer and for Walker
    // each Coord along the solution must be adjacent to prev and next Coord on path
    public Iterable<Coord> getPathSolution() { return solution; }

    // useful for debugging
    // returns true if Coord was looked at (placed on priority queue) while finding a path
    // visualizer tints purple all Coord 's where this function returns true
    public boolean wasSearched(Coord loc) {
        if (trackMap[loc.getI()][loc.getJ()] == 1) return true;
        else return false;
    }
}
