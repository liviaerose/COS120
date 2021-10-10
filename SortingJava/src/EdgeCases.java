public class EdgeCases {
    /***********************************************************************
     *  main() function
     *  Place all of your unit tests here
     *  Hint: created additional functions to help organize your tests
     ***********************************************************************/

    public static void main(String[] args) {
        Double[] a = { 0.0 };

        Double[] b = {1.0,2.0,3.0,4.0,5.0};
        Double[] c = {5.0, 4.0,3.0,2.0,1.0};
        Double[] d = {5.0,5.0,5.0,5.0,5.0};
        Double[] e = {5.0,4.0,5.0,4.0};
        Double[] f = {1.0};
        Double[] g = {};

        Bubble.sort(a);     // bubble sort
        Selection.sort(a);  // selection sort
        Insertion.sort(a);  // insertion sort
        Shell.sort(a);      // Shellsort
        Quick.sort(a);      // quicksort

        Bubble.sort(b);     // bubble sort
        Selection.sort(b);  // selection sort
        Insertion.sort(b);  // insertion sort
        Shell.sort(b);      // Shellsort
        Quick.sort(b);      // quicksort

        Bubble.sort(c);     // bubble sort
        Selection.sort(c);  // selection sort
        Insertion.sort(c);  // insertion sort
        Shell.sort(c);      // Shellsort
        Quick.sort(c);      // quicksort

        Bubble.sort(d);     // bubble sort
        Selection.sort(d);  // selection sort
        Insertion.sort(d);  // insertion sort
        Shell.sort(d);      // Shellsort
        Quick.sort(d);      // quicksort

        Bubble.sort(e);     // bubble sort
        Selection.sort(e);  // selection sort
        Insertion.sort(e);  // insertion sort
        Shell.sort(e);      // Shellsort
        Quick.sort(e);      // quicksort

        Bubble.sort(f);     // bubble sort
        Selection.sort(f);  // selection sort
        Insertion.sort(f);  // insertion sort
        Shell.sort(f);      // Shellsort
        Quick.sort(f);      // quicksort

        Bubble.sort(g);     // bubble sort
        Selection.sort(g);  // selection sort
        Insertion.sort(g);  // insertion sort
        Shell.sort(g);      // Shellsort
        Quick.sort(g);      // quicksort
    }
}
