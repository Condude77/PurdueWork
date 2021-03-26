import java.io.*;
import java.net.*;
public class Server {
	public static void main(String[] args) throws IOException, ClassNotFoundException, FileNotFoundException{
		ServerSocket serverSocket = new ServerSocket(13);
		if(!(serverSocket.isBound())){
			throw new IOException("Server Creation not Successful");
		}
		System.out.println("Server Waiting for Connection");
		Socket socket = serverSocket.accept();
		System.out.println("Conneftion is successful and waiting for commands");
		
		ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
		oos.flush();
		ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
		
		String filename = (String) ois.readObject();
		System.out.println("Filename: " + filename);
		String username = (String) ois.readObject();
		System.out.println("Username: " + username);
		
		File f = new File("~/Desktop/cs180/lab11/Server/database.txt");
		if(!(f.exists())){
			throw new FileNotFoundException("FileNotFoundException");
		}
		FileOutputStream fos = new FileOutputStream(f, true);
		FileReader fr = new FileReader(f);
		BufferedReader bfr = new BufferedReader(fr);
		while(true){
			String s = bfr.readLine();
			int firstSemi = s.indexOf(";", 2);
			if(s.substring(2, firstSemi) == username){
				int secondSemi = s.indexOf(";", firstSemi + 1);
				int thirdSemi = s.indexOf(";", secondSemi + 1);
				String lastName = s.substring(firstSemi + 1, secondSemi);
				String firstName = s.substring(secondSemi + 1, thirdSemi);
				String marks = s.substring(thirdSemi + 1);
				String o = String.format("LastName: %s%nFirstName: %s%nMarks: %s%n", lastName, firstName, marks);
				oos.writeObject(o);
				break;
			}
			if(bfr.readLine() == null){
				oos.writeObject("InfoNotFoundException");
				break;
			}
		}
		bfr.close();
		fos.close();
		ois.close();
		oos.close();
	}
}
