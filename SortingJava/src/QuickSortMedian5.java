/**
 * The following code is *mostly* a copy of Quick class (quick sort) from algs4.jar
 */
import java.util.*;
public class QuickSortMedian5 extends QuickSortMedian {

    public static class MedianOf5 extends MedianOfN {
        public MedianOf5() {
            // tell QuickSortMedian.MedianOfN we will find the median of 5 items
            super(5);
        }

        /***********************************************************
         * Determines which index in parameter indices points to
         * the median value in parameter a
         * @param a the array containing values
         * @param indices the array containing indices into a
         * @return the index of median value
         ***********************************************************/
        public int median(Comparable[] a, int[] indices) {
            // get values at specified indices
            int i0 = indices[0];
            int i1 = indices[1];
            int i2 = indices[2];
            int i3 = indices[3];
            int i4 = indices[4];
            Comparable a0 = a[i0];
            Comparable a1 = a[i1];
            Comparable a2 = a[i2];
            Comparable a3 = a[i3];
            Comparable a4 = a[i4];

            int[] temp = {i0, i1, i2, i3, i4};

            Arrays.sort(temp);

            if(temp[2] == i0)
                return i0;
            else if(temp[2] == i1)
                return i1;
            else if(temp[2] == i2)
                return i2;
            else if(temp[2] == i3)
                return i3;
            return i4;

            // find median in a0,a1,a2, and return respective index
            // if a0 is median, return i0
            // if a1 is median, return i1
            // if a2 is median, return i2


        }

    }

    /***********************************************************************
     * Rearranges the array in ascending order, using the natural order.
     * @param a the array to be sorted
     ***********************************************************************/
    public static void sort(Comparable[] a) {
        MedianOf5 median = new MedianOf5();
        sort(a, median);
    }


    /***********************************************************************
     *  main() function
     *  Place all of your unit tests here
     *  Hint: created additional functions to help organize your tests
     ***********************************************************************/

    public static void main(String[] args) {
        Double[] a = {0.0};
        QuickSortMedian5.sort(a);
    }
}
