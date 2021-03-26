package server.User;

public class User{

	public String username;
	public String password;
	public String name;
	public String email;

	public User(){
		this.username = null;
		this.password = null;
		this.name = null;
		this.email = null;
	
	}


	public User(String userName, String password, String name, String email)
	{
		this.username = userName;
		this.password = password;
		this.name = name;
		this.email = email;
	}


}