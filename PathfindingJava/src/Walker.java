import java.util.Iterator;

/**
 * Walker takes an Iterable of Coords and simulates an individual
 * walking along the path over the given Terrain
 */
public class Walker {

    private Terrain terrain;
    private Iterable<Coord> path;
    private Iterable<Coord> tempPath;

    // terrain: the Terrain the Walker traverses
    // path: the sequence of Coords the Walker follows
    public Walker(Terrain terrain, Iterable<Coord> path) {
        this.terrain = terrain;
        this.path = path;
        this.tempPath = this.path;
    }

    // returns the Walker's current location
    public Coord getLocation() {
        Coord current = tempPath.iterator().next();
        tempPath = path;
        return current;
    }

    // returns true if Walker has reached the end Coord (last in path)
    public boolean doneWalking() {
        if (path.iterator().hasNext()) return false;
        else return true;
    }

    // advances the Walker along path
    // byTime: how long the Walker should traverse (may be any non-negative value)
    public void advance(float byTime) {
        path.iterator().next();
        //terrain.computeTravelCost(path);
    }

}
