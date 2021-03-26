/**
 * Created by brow1325 on 11/8/16.
 */
import java.io.*;
import java.net.*;
public class Server {
    public static void main(String[] args) throws IOException, ClassNotFoundException{
        ServerSocket serverSocket = new ServerSocket(1300);
        if(!(serverSocket.isBound())){
            throw new IOException("Server Creation not Successful");
        }
        System.out.println("Server Waiting for Connection");
        Socket socket = serverSocket.accept();
        System.out.println("Connection is successful and waiting for commands");

        ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
        oos.flush();
        ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());

        String filename = (String) ois.readObject();
        System.out.println("Filename: " + filename);
        String username = (String) ois.readObject();
        System.out.println("Username: " + username);
        File f = new File("/homes/brow1325/Desktop/cs180/lab11/Server/" + filename);
        try {
            //FileOutputStream fos = new FileOutputStream(f, true);
            FileReader fr = new FileReader(f);
            BufferedReader bfr = new BufferedReader(fr);
            boolean b = true;
            String s;
            while ((s= bfr.readLine()) != null) {
                //System.out.println(s);
                int firstSemi = s.indexOf(';');
                int secondSemi = s.indexOf(';', firstSemi + 1);
                String user = s.substring(firstSemi + 1, secondSemi);
                //System.out.println("test" + user);
                if (user.equals(username)) {
                    //System.out.println("User = username");
                    int thirdSemi = s.indexOf(';', secondSemi + 1);
                    int fourthSemi = s.indexOf(';', thirdSemi + 1);
                    String lastName = s.substring(secondSemi + 1, thirdSemi);
                    String firstName = s.substring(thirdSemi + 1, fourthSemi);
                    String marks = s.substring(fourthSemi + 1);
                    String o = String.format("LastName: %s%nFirstName: %s%nMarks: %s%n", lastName, firstName, marks);
                    //System.out.println("test" + o);
                    oos.writeObject(o);
                    break;
                }

            }
            if (s == null) {
                throw new InfoNotFoundException("Your Information is not in the files");
            }
            bfr.close();
            //fos.close();
            ois.close();
            oos.close();
        }catch(FileNotFoundException e) {
            oos.writeObject("FileNotFoundException");
        /*}catch(NullPointerException e){
            System.out.println(e.getMessage());*/
        }catch(IOException e){
            System.out.println(e.getMessage());
        }catch(InfoNotFoundException e){
            oos.writeObject("InfoNotFoundException");
        }
    }
}
