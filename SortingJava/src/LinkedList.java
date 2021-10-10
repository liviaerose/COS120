import java.util.Iterator;

public class LinkedList<Item extends Comparable<Item>> implements Iterable<Item> {
    private Node first = null;
    private int count = 0;

    private class Node {
        final Item item;    // cannot change item once it is set in constructor
        Node next;          // can change next, though

        public Node(Item i, Node n) {
            item = i;
            next = n;
        }
    }

    public LinkedList() { }

    public LinkedList(Item[] fromList) {
        for(Item item : fromList) insert(item);
    }

    public void insert(Item item) {
        first = new Node(item, first);
        count++;
    }

    public Item remove() {
        Item item = first.item;
        first = first.next;
        count--;
        return item;
    }

    public boolean isEmpty() { return count == 0; }

    public Iterator<Item> iterator() {
        return new Iterator<Item>() {
            Node current = first;

            @Override
            public boolean hasNext() {
                return current != null;
            }

            @Override
            public Item next() {
                Item item = current.item;
                current = current.next;
                return item;
            }
        };
    }

    /***********************************************************************
     * Rearranges the linked list in ascending order, using the natural order
     * and mergesort.
     ***********************************************************************/

    private static void merge(Comparable[] a, Comparable[] aux,
                              int lo, int mid, int hi) {
        int i = lo, j = mid + 1;
        for(int k = lo; k <= hi; k++) {
            if(i > mid)                   aux[k] = a[j++];
            else if(j > hi)               aux[k] = a[i++];
            else if(aux[j].compareTo(aux[i]) <= 0) aux[k] = a[j++];
            else                          aux[k] = a[i++];
        }
    }

    public void sort(Comparable[] a) {
        if (first == null || first.next == null) return;
        int N = a.length;
        Comparable[] aux = new Comparable[N];
        for(int sz = 1; sz < N; sz = sz+sz)
            for(int lo = 0; lo < N-sz; lo += sz+sz)
                merge(a, aux, lo, lo+sz-1, Math.min(lo+sz+sz-1, N-1));
        }



    /***********************************************************************
     *  main() function
     *  Place all of your unit tests here
     *  Hint: created additional functions to help organize your tests
     ***********************************************************************/

    public static void main(String[] args) {
        Stopwatch sw = new Stopwatch();
        Double[] a = {0.0};
        LinkedList<Double> linkedlist = new LinkedList<>(a);
        StdOut.println("Elapsed time: " + sw.elapsedTime());
    }
}
