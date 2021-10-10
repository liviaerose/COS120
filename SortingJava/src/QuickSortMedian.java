/**
 * The following code is *mostly* a copy of Quick class (quick sort) from algs4.jar
 */

public class QuickSortMedian {

    /***********************************************************************
     * Rearranges the array in ascending order, using the natural order.
     * @param a the array to be sorted
     ***********************************************************************/

    public static void sort(Comparable[] a, MedianOfN median) {
        sort(a, median, 0, a.length - 1);
    }

    // quicksort the subarray from a[lo] to a[hi]
    private static void sort(Comparable[] a, MedianOfN median, int lo, int hi) {
        if(hi <= lo) return;
        int j = partition(a, median, lo, hi);   // partition data into parts, returning pivot index
        sort(a, median, lo, j-1);   // recursively sort lower part
        sort(a, median, j+1, hi);   // recursively sort higher part
    }

    // partition the subarray a[lo..hi] so that a[lo..j-1] <= a[j] <= a[j+1..hi]
    // and return the index j.  note: v = a[j]
    private static int partition(Comparable[] a, MedianOfN median, int lo, int hi) {
        int i = lo;
        int j = hi + 1;

        int k = median.median(a, lo, hi);   // determine position of a pivot
        exch(a, lo, k);                     // move pivot to lo position
        Comparable v = a[lo];               // v is pivot value

        while(true) {

            // find item on lo to swap
            while(less(a[++i], v))
                if(i == hi) break;

            // find item on hi to swap
            while(less(v, a[--j]))
                if(j == lo) break;      // redundant since a[lo] acts as sentinel

            // check if pointers cross
            if(i >= j) break;

            exch(a, i, j);
        }

        // put partitioning item v at a[j]
        exch(a, lo, j);

        // now, a[lo .. j-1] <= a[j] <= a[j+1 .. hi]
        return j;
    }


    /***********************************************************************
     *  Helper sorting functions
     ***********************************************************************/

    // is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return (v.compareTo(w) < 0);
    }

    // exchange a[i] and a[j]
    static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }


    /***********************************************************************
     *  main() function
     *  Place all of your unit tests here
     *  Hint: created additional functions to help organize your tests
     ***********************************************************************/

    public static void main(String[] args) {

    }
}
