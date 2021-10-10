public class Bubble {

    public static void sort(Comparable[] a) {
        boolean sorted = false;
        while(!sorted) {
            sorted = true;
            for(int i = 0; i < a.length - 1; i++)
                if(less(a[i + 1], a[i])) {
                    sorted = false;
                    exch(a, i, i + 1);
                }
        }
    }

    /***********************************************************************
     *  Helper sorting functions
     ***********************************************************************/

    // is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return (v.compareTo(w) < 0);
    }

    // exchange a[i] and a[j]
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    /***********************************************************************
     *  main() function is used to experimentally verify bubble sort works
     *  - generate a Double array
     *  - fill array with random values
     *  - sort
     *  - assert that each value is smaller than or equal to next value
     ***********************************************************************/

    public static void main(String[] args) {
        int N = 1000;
        Double[] a = new Double[N];
        for(int i = 0; i < N; i++) a[i] = StdRandom.uniform();
        Bubble.sort(a);
        for(int i = 0; i < N-1; i++) assert a[i] <= a[i+1];
    }
}
