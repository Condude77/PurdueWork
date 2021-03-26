/**
 * Created by brow1325 on 11/15/16.
 */
public class Divider implements Runnable {
    private int start;
    private int stop;
    private Counter counter;
    public void run(){
        Thread.yield();
        for(int i = start; i < stop; i++){
            if(i % 7 == 0){
                counter.inc();
                //System.out.println("test: " + i + " " + counter.get());
            }
        }
    }
    public Divider(int start, int stop, Counter counter){
        this.start = start;
        this.stop = stop;
        this.counter = counter;
    }
    public static void main(String[] args) throws InterruptedException{
        Counter counter = new Counter2();
        Thread t1 = new Thread(new Divider(1, 1000, counter));
        Thread t2 = new Thread(new Divider(1001, 2000, counter));
        Thread t3 = new Thread(new Divider(2001, 3000, counter));
        t1.start();
        t2.start();
        t3.start();
        try{
            t1.join();
            t2.join();
            t3.join();
        }catch(InterruptedException e){
            System.out.println("Big Problem");
        }
        System.out.println(counter.get());
    }
}
