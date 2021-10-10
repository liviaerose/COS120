public class DoublingTest {
    /***********************************************************************
     *  main() function
     *  Place all of your unit tests here
     *  Hint: created additional functions to help organize your tests
     ***********************************************************************/
    private static final int max=1000;
    public static double time(int n){
        Double[] a = { 0.0 };
        Stopwatch sw = new Stopwatch();
        Selection.sort(a);  // selection sort
        Insertion.sort(a);  // insertion sort
        Shell.sort(a);      // Shellsort
        return sw.elapsedTime();
    }
    public static void main(String[] args) {

        for(int n=1000; true; n+=n) {
            double timeN = time(n);
            StdOut.println("Elapsed time: " + timeN);
        }
    }
}
