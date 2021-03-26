/**
 * Created by brow1325 on 11/8/16.
 */
import java.io.*;
import java.net.*;
import java.util.Scanner;
public class Client {
    public static void main(String[] args) throws UnknownHostException, IOException, ClassNotFoundException, InfoNotFoundException{
        Socket socket = new Socket("localhost", 1300);

        ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
        ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());

        Scanner scan = new Scanner(System.in);
        System.out.println("Enter Filename");
        String filename = scan.nextLine();
        System.out.println("Enter Username");
        String username = scan.nextLine();

        oos.writeObject(filename);
        oos.flush();
        oos.writeObject(username);
        oos.flush();
        try {
            String s1 = (String) ois.readObject();
            if (s1.equals("FileNotFoundException")) {
                System.out.println("File does not exist");
            } else if (s1.equals("InfoNotFoundException")) {
                System.out.println(s1 + ": Your information is not in our file");
            } else {
                File f = new File("/homes/brow1325/Desktop/cs180/lab11/Client/info.txt");
                FileOutputStream fos = new FileOutputStream(f, true);
                PrintWriter pw = new PrintWriter(fos);
                pw.println("" + s1);
                //System.out.println(s1);
                pw.close();
                fos.close();
            }
            scan.close();
            oos.close();
            ois.close();
        }catch(FileNotFoundException e){
            System.out.println(e.getMessage());
        }
    }
}
