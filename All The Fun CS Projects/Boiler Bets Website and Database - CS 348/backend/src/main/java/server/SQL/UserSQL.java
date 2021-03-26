package server.SQL;

import java.sql.*;
import java.util.ArrayList;
import server.User.User;

public class UserSQL {

	private String url;
	private Connection conn;
	Statement smt;
	PreparedStatement psmt;
	ResultSet rs;
	private String database;

	public UserSQL(){	
		url = "jdbc:mysql://35.238.205.120:3306/";
		try{
			conn = DriverManager.getConnection(url, "root", "root");//development
			smt = conn.createStatement();
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
		database = "boiler_bets";

	}

	public ArrayList<User> getAllUsers(){
		try{
			psmt = conn.prepareStatement("select * from "+ this.database+".users");
			rs = psmt.executeQuery();
			String all = "User Info:<br>";
			ArrayList<User> user = new ArrayList<User>();


			while (rs.next())
			{
				String username=rs.getString("username");
				String password = rs.getString("password");
				String fullName = rs.getString("name");
				String email = rs.getString("email");

				// String lastLogin = rs.getString("lastLogin");
				User u = new User();
				user.add(u);
			
				all+=username+"\t"+password+"\t"+fullName+"\t"+email+"\t";//+profilePhoto+"\t"+bio+"\t"+likedDrinks+"\t"+dislikedDrinks+"\t"+favoriteDrink+"\t"+publishedDrinks+"\t"+postHistory+"\t"+friendsList+"\t"+dateCreated+"\t"+lastLogin;
				all+="<br>";
			}
			rs.close();
			psmt.close();
			conn.close();
			System.out.println(all);
			return user;

		}catch(Exception e){
			e.printStackTrace();
			return null;//"/user Fail";
		}
	}

	public String login(String userName, String password){
		try{
			psmt = conn.prepareStatement("select password from "+this.database+".users where username = ?");
			psmt.setString(1, userName);
			//password hashing
			rs = psmt.executeQuery();

			String password1 = "";
			while(rs.next()){
				password1 = rs.getString("password");
			}
			rs.close();
			psmt.close();
			conn.close();
			
			return password1;
			
		}catch(Exception e){
			e.printStackTrace();
			
			return "{ \"status\" : \"Error: Login Failed.\"}";
		}
	}


	public User getUser(String name){
		try{
	
			String query = "select * from "+ this.database+".users where username = ?";
			psmt = conn.prepareStatement(query);
			psmt.setString(1, name);
			System.out.println(query);
			rs = psmt.executeQuery();
			//String returnUser = "User: "+name+"<br>";
			User u = new User();

			while (rs.next())
			{
				//u.userId=rs.getInt("userId");
				u.username=rs.getString("username");
				u.password = rs.getString("password");
				u.name = rs.getString("name");
				u.email = rs.getString("email");
				
			}
			rs.close();
			psmt.close();
			conn.close();
			//System.out.println(returnUser);
			return u;


		}catch(Exception e){
			e.printStackTrace();
			return null;
		}
	}

	public String getIdByUsername(String name) {
		try{
			String query = "select userId from "+ this.database+".users where username = ?";
			psmt = conn.prepareStatement(query);
			psmt.setString(1, name);
			rs = psmt.executeQuery();
			String userId = "";

			while (rs.next())
			{
				userId = rs.getString("userId");
			}
			rs.close();
			psmt.close();
			conn.close();
			//System.out.println(returnUser);
			return userId;


		}catch(Exception e){
			e.printStackTrace();
			return null;
		}
	}

	public User[] searchUsers(String request){
		StringBuilder searchString = new StringBuilder("%" + request + "%");
		for (int i = 0; i < request.length(); i++) {
			if (searchString.charAt(i) == ' ') {
				searchString.setCharAt(i, '%');
			}
		}
		System.out.println(searchString);
		try{
			String query = "Select * FROM "+ this.database+".users WHERE username LIKE ?";
			System.out.println(query);
			psmt = conn.prepareStatement(query);
			psmt.setString(1, searchString.toString());
			rs = psmt.executeQuery();
			ArrayList<User> user = new ArrayList<User>();
			String all = "User Info:<br>";
			while (rs.next())
			{
				//int userId=rs.getInt("userId");
				String username=rs.getString("username");
				String password = rs.getString("password");
				String fullName = rs.getString("name");
				String email = rs.getString("email");
	
				User u = new User(username, password, fullName, email);
				user.add(u);
			}
			rs.close();
			psmt.close();
			conn.close();
			
			for (int x = 0; x < user.size(); x++){
				System.out.println("INSQL "+user.get(x).username);

			}
			

			User[] outUser = new User[user.size()];
			outUser = user.toArray(outUser);
			return outUser;

		}catch(Exception e){
			e.printStackTrace();
			return null;
		}
	}

	public String insertUser(String username, String password, String name, String email){
		try{
			//first need to checkUniqueUserName
			// if (!checkUniqueUserName(username)) {
			// 	System.out.println("Username not unqiue. Cannot insert new user.");
			// 	return "{ \"status\" : \"Error: user already exists.\"}";
			// }
			//if unique then can insert User
			String query = "insert into "+ this.database+".users "+ 
				"(username, password, email, name) "+
				"values "+ 
				"(?, ?, ?, ?)";
			System.out.println(query);
			psmt = conn.prepareStatement(query);
			psmt.setString(1, username);
			psmt.setString(2, password);
			psmt.setString(3, email);
			psmt.setString(4, name);
			
			int insertResult = psmt.executeUpdate();

			psmt.close();
			conn.close();
			return "{ \"status\" : \"ok\" }";
		}catch(Exception e){
			e.printStackTrace();
			System.out.println("Error inserting new user to DB.");
			return "{ \"status\" : \"Error: SQL insert failed.\"}";
		}


	}

}