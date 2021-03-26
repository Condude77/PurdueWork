import java.io.*;
import java.net.*;
import java.util.Scanner;
public class Client {
	public static void main(String[] args) throws UnknownHostException, IOException, ClassNotFoundException, InfoNotFoundException{
		Socket socket = new Socket("localhost", 13);
		
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
		
		String s1 = (String) ois.readObject();
		if(s1.equals("FileNotFoundException")){
			System.out.println("File does not exist");
		}else if(s1.equals("InfoNotFoundException")){
			throw new InfoNotFoundException("InfoNorFoundException");
		}else{
			File f = new File("~/Desktop/cs180/lab11/Client/info.txt");
			FileOutputStream fos = new FileOutputStream(f, true);
			PrintWriter pw = new PrintWriter(fos);
			pw.println(s1);
			pw.close();
			fos.close();
		}
		scan.close();
		oos.close();
		ois.close();
	}
}
