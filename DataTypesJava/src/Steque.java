import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class Steque<Item> implements Iterable<Item> {
    private int N;
    private Node first, last;

    private class Node{
        private Item item;
        private Node next;
    }
    public Steque() {
        N=0;
        first=null;
        last=null;
    }


    // returns the number of items stored
    public int size() {
        return N;
    }

    // returns true if steque is empty
    public boolean isEmpty() {
        return (N==0);
    }

    // enqueues item to bottom of steque
    public void enqueue(Item item) {
        if (isEmpty()){
            first = new Node();
            last = first;
            first.item= item;
        }else {
            Node oldlast = last;
            last = new Node();
            last.item = item;
            oldlast.next = last;
        }
        N++;
    }

    // pushes item to top of steque
    public void push(Item item) {
        if (N==0){
            first = new Node();
            first.item= item;
            last = first;
        }else {
            Node oldfirst = first;
            first = new Node();
            first.item = item;
            first.next = oldfirst;
        }
        ++N;
    }

    // pops and returns top item
    public Item pop() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }else {
            Item item = first.item;
            first = first.next;
            N--;
            if (isEmpty()) {
                first = null;
            }
            return item;
        }
    }

    // returns new Iterator<Item> that iterates over items in steque
    @Override
    public Iterator<Item> iterator() {
        return new Iterator<Item>() {
            private Node current = first;

            public boolean hasNext() { return current != null; }

            public void remove()     {
                Node n = first;
                while (n.next.next != null){
                    n=n.next;
                }
                n.next=null;
                --N;
            }

            public Item next() {
                if (!hasNext()){
                    throw new ConcurrentModificationException();
                }
                Item item = current.item;
                current = current.next;
                return item;
            }
        };
    }

    // perform unit testing here
    public static void main(String[] args) throws NoSuchElementException {
        Stopwatch sw = new Stopwatch();
        Steque<String> steq = new Steque<String>();
        steq.push("1");
        steq.enqueue("2");
        StdOut.println(steq.size());
        steq.pop();
        StdOut.println(steq.size());
        steq.pop();
        StdOut.println(steq.size());
        StdOut.println("Elapsed time: " + sw.elapsedTime());
    }
}