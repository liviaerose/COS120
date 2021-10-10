import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;


public class OnePointerQueue<Item> implements Iterable<Item> {
    private Node first, last;
    private int count=0;
    private class Node {
        private Item item;
        private Node next;
    }
    public OnePointerQueue() {

    }

    // returns the number of items stored
    public int size() {
        return count;
    }

    // returns true iff empty
    public boolean isEmpty() {
        return (first==null);
    }

    // enqueue item to "back"
    public void enqueue(Item item) {
        if (count==0){
            first = new Node();
            last = first;
            first.item= item;
        }else {
            Node oldlast = last;
            last = new Node();
            last.item = item;
            oldlast.next = last;
        }
        count++;
    }

    // dequeue item from "front"
    public Item dequeue() throws NoSuchElementException {
        if (count == 0) {
            throw new NoSuchElementException();
        } else{
            Item item = first.item;
            first = first.next;
            count--;
            return item;
        }
    }

    // returns new Iterator<Item> that iterates over items
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
                --count;
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
    public static void main(String[] args) {
        Stopwatch sw = new Stopwatch();
        OnePointerQueue<String> steq = new OnePointerQueue<String>();
        steq.enqueue("1");
        StdOut.println(steq.size());
        steq.enqueue("2");
        StdOut.println(steq.size());
        steq.dequeue();
        StdOut.println(steq.size());
        steq.dequeue();
        StdOut.println(steq.size());
        StdOut.println("Elapsed time: " + sw.elapsedTime());
    }
}
