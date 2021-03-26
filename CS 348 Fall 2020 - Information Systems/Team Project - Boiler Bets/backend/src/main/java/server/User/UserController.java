package server.User;

import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import server.SQL.UserSQL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.security.*;
import java.nio.charset.StandardCharsets;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.Version;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;


@RestController
@CrossOrigin(origins = "http://localhost:3000")
//@CrossOrigin //(origins = "https://fiveo-clocksomewhere.firebaseapp.com/", maxAge =  3600, allowedHeaders = "*")     //production
@RequestMapping(path="/user")
public class UserController {
	//make private and called in insert user or somewhere after testing
	
	int count = 0;

		@PostMapping("/login")
		public String login(@RequestBody String username)
		throws JsonParseException, JsonMappingException, IOException {
		
		ObjectMapper om = new ObjectMapper();
		SimpleModule sm = new SimpleModule("UserDeserializer", new Version(1, 0, 0, null, null, null));
		sm.addDeserializer(User.class, new UserDeserializer());
		om.registerModule(sm);
		User u = om.readValue(username, User.class);

		UserSQL user = new UserSQL();
		String dbPass = user.login(u.username, u.password);
		
		if(dbPass == "")
			return "{ \"status\" : \"Error: Login Failed.\"}";
		//byte[] saltSubstr = dbPass.substring(0, dbPass.indexOf(".")).getBytes();
		System.out.println("dbPass: "+dbPass+" vs attemptedPassword: "+u.password);
		if( dbPass.equals(u.password)){
			return "{ \"status\" : \"ok.\"}";
		} else {
			return "{ \"status\" : \"Error: Login Failed.\"}";
		}
	}

    @GetMapping("")
    public String findAll() {
        //find a single user
		UserSQL users = new UserSQL();
		ArrayList<User> list = users.getAllUsers();
		String out = "";
		for (User user : list) {
			out += user.username + "\t";
			out += user.password + "\t";
			out += user.name + "\t";
			out += user.email + "\t";
			
		}
		return out;
    }

    @GetMapping("/{name}")
    public String findUser(@PathVariable String name) 
			throws JsonParseException, JsonMappingException, IOException {
        //find a single user
		System.out.println("User: "+ name);
		UserSQL users = new UserSQL();

		ObjectMapper om2 = new ObjectMapper();
		SimpleModule sm2 = new SimpleModule("UserSerializer", new Version(1, 0, 0, null, null, null));
		sm2.addSerializer(User.class, new UserSerializer());
		om2.registerModule(sm2);

        return om2.writeValueAsString(users.getUser(name));
    }

	@GetMapping("/searchUsers")
	public String searchUsers(@RequestParam(name = "s") String request, @RequestParam(name = "u") String username) throws JsonProcessingException, IOException {
		System.out.println("SEARCHING USERS");

		UserSQL us = new UserSQL();
		User[] users = us.searchUsers(request);
		us = new UserSQL();
		//users = us.flagFollowedUsers(users, username);

		if(users == null) {
			return "{\"results\": \"DNE\"";
		}

		String out =  "{ \"results\": [ ";
		for (User user : users ) {
			out += new ObjectMapper().writeValueAsString(user) + ",";
			
		}

		out = out.substring(0, out.length()-1) + "] }";

		return out;
	}


    @PostMapping("/")
    public String insertUser(@RequestBody String username) 
			throws JsonParseException, JsonMappingException, IOException {
        //save a single user
		ObjectMapper om = new ObjectMapper();
		SimpleModule sm = new SimpleModule("UserDeserializer", new Version(1, 0, 0, null, null, null));
		sm.addDeserializer(User.class, new UserDeserializer());
		om.registerModule(sm);
		User u = om.readValue(username, User.class);

		UserSQL users = new UserSQL();
		String insert  = users.insertUser(u.username, u.password, u.name, u.email);	

	    return insert;
	}
	
	@PostMapping("/getIdByUsername")
    public String getIdByUsername(@RequestBody String username) 
			throws JsonParseException, JsonMappingException, IOException {

		System.out.println(username);
		username = username.substring(username.indexOf(":") + 2);
		username = username.substring(0, username.length() - 2);
		System.out.println(username);

		UserSQL users = new UserSQL();
		String id  = users.getIdByUsername(username);	

	    return id;
    }
}
