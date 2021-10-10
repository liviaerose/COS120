public abstract class MedianOfN {
    private int N;
    private int[] indices;

    public MedianOfN(int N) {
        this.N = N;
        indices = new int[N];
    }

    public abstract int median(Comparable[] a, int[] indices);

    public int median(Comparable[] a, int lo, int hi) {
        // if hi and lo are "close enough", simply return lo
        if(hi - lo + 1 < N) return lo;
        // choose N random indices in [lo, hi]
        for(int i = 0; i < N; i++) indices[i] = StdRandom.uniform(lo, hi + 1);
        return median(a, indices);
    }
}

