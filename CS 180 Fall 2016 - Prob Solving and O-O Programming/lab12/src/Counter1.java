/**
 * Created by brow1325 on 11/15/16.
 */
public class Counter1 implements Counter {
    private int value = 0;

    public synchronized void inc() {
        value++;
    }

    public synchronized void dec() {
        value--;
    }

    public synchronized int get() {
        return value;
    }
}