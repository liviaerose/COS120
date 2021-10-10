import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class MinimumStack<Item extends Comparable> implements Iterable<Item> {
    private int N;
    private Node first, last;
    private Stack<Item> minStack = new Stack<>();

    private class Node{
        private Item item;
        private Node next;
    }
    public MinimumStack() {
        N=0;
        first=null;
        last=null;
    }

    // returns the number of items stored
    public int size() {
        return N;
    }

    // returns true iff empty
    public boolean isEmpty() {
        return (N==0);
    }

    // push item onto stack
    public void push(Item item) {
        if (N==0){
            first = new Node();
            last = first;
            first.item= item;
        }else {
            Node oldfirst = first;
            first = new Node();
            first.item = item;
            first.next = oldfirst;
        }
        if(minStack.isEmpty()){
            minStack.push(item);
        }if (!minStack.isEmpty() && item.compareTo(minStack.peek()) < 0) {
            minStack.push(item);
        }
        N++;
    }

    // pop and return the top item
    public Item pop() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }else {
            Item item = first.item;
            first = first.next;
            if(minStack.peek()==item){
                minStack.pop();
            }
            N--;
            if (isEmpty()) {
                first = null;
            }
            return item;
        }
    }

    // returns the minimum item in constant time
    public Item minimum() throws NoSuchElementException {
        if (isEmpty()){
            throw new NoSuchElementException();
        }else {
            return (minStack.peek());
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

    public static void main(String[] args) {
        Stopwatch sw = new Stopwatch();

        MinimumStack<String> steq = new MinimumStack<String>();
        steq.push("1");
        steq.push("0");
        steq.push("2");
        StdOut.println(steq.minimum());
        steq.pop();
        StdOut.println(steq.minimum());
        steq.pop();
        StdOut.println(steq.minimum());
        steq.pop();

        StdOut.println("Elapsed time: " + sw.elapsedTime());
    }
}
