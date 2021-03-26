package server.Players;

import org.springframework.web.bind.annotation.*;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.http.ResponseEntity;
import javax.validation.Valid;
import java.util.List;

import java.io.IOException;
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
import org.springframework.context.annotation.Bean;
import java.sql.*;
import server.SQL.PlayerStatsSQL;
import server.PlayerStats.*;

@RestController
@CrossOrigin(origins = "http://localhost:3000")
@RequestMapping(path="/player")
public class PlayerController {

    private String url;
	private Connection conn;
	Statement smt;
	PreparedStatement psmt;
	ResultSet rs;
	private String database;

    public PlayerController(){	
		url = "jdbc:mysql://35.238.205.120:3306/";
		try{
			conn = DriverManager.getConnection(url, "root", "root");//development
			smt = conn.createStatement();
			
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
		database = "boiler_bets";

	}

    @Autowired
    PlayerRepository playerRepository;

	@GetMapping("/")
	public String base(){
		return "\"help\": \"me\"";
	}

    @PostMapping("/insertPlayer")
    public Player addPlayer(@RequestBody String playerInfo) throws JsonParseException, JsonMappingException, IOException {
		// Parse the arguments
		ObjectMapper om = new ObjectMapper();
		SimpleModule sm = new SimpleModule("PlayerDeserializer", new Version(1, 0, 0, null, null, null));
		sm.addDeserializer(Player.class, new PlayerDeserializer());
		om.registerModule(sm);

		Player newPlayer = om.readValue(playerInfo, Player.class);

		String result = "";
		try{
			psmt = conn.prepareStatement("select teamName from " +this.database+".teams where teamId = ?");
			psmt.setString(1, Long.toString(newPlayer.getTeamId()));
			rs = psmt.executeQuery();
			while (rs.next())
			{
                result += rs.getString("teamName");
			}
			System.out.println(result);

			rs.close();
			psmt.close();
            conn.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		newPlayer.setTeam(newPlayer.getTeamId(), result);
		Player savedPlayer = playerRepository.save(newPlayer);

		Long newPlayerID = savedPlayer.getId();
		PlayerStatsSQL psSQL = new PlayerStatsSQL();

		psSQL.insertPlayerStats(newPlayerID, playerInfo);

		return savedPlayer;
    }

    @GetMapping("/getAllPlayers")
    public Iterable<Player> getPlayers() {
		System.out.println("HERE");
        return playerRepository.findAll();
	}
	
	@PostMapping("/getPlayerStats")
	public Player findPlayer(@RequestBody String player_id) {
		System.out.println("Getting player stats");
		System.out.println(player_id);
		
		int startIndex = player_id.indexOf(":");
		String player_num = player_id.substring(startIndex + 1, player_id.length()-1);//, startIndex + 2);
		System.out.println(player_num);
		PlayerStatsSQL pss = new PlayerStatsSQL();
		String t  = pss.getPlayersStats(Integer.parseInt(player_num));
		Iterable<Player> playerList = playerRepository.findAll();
		for (Player player : playerList ) {
			System.out.println(player.toString());
			if (player.getId() == Long.parseLong(player_num)) {
				//System.out.println(player.toString());
				player.playerStats = t;
				return player;
			}
		}

		System.out.println("returning null");
		return null;
	}


	@PostMapping("/deletePlayer")
	public String deletePlayer(@RequestParam(name="playerId") String player_id) {
		System.out.println(player_id);

		Player removePlayer = new Player();
		removePlayer.setId(Long.parseLong(player_id));
		playerRepository.delete(removePlayer);

		try {
			psmt = conn.prepareStatement("delete from "+ this.database +".playerStats where playerId = ?");
			psmt.setString(1, player_id);
			psmt.executeUpdate();

			//rs.close();
			psmt.close();
			//conn.close();

		} catch (Exception e) {
			//return "ERR";
			e.printStackTrace();
			return "ERR";
		}

		return "OK";
	}

	@PostMapping("/updatePlayer")
	public String updatePlayer(@RequestParam(name="playerId") String player_id, @RequestBody String playerInfo) throws JsonParseException, JsonMappingException, IOException {
		System.out.println(player_id);
		System.out.println(playerInfo);

		ObjectMapper om = new ObjectMapper();
		SimpleModule sm = new SimpleModule("PlayerStatsDeserializer", new Version(1, 0, 0, null, null, null));
		sm.addDeserializer(PlayerStats.class, new PlayerStatsDeserializer());
		om.registerModule(sm);

		PlayerStats newPlayer = om.readValue(playerInfo, PlayerStats.class);
		newPlayer.setID(Long.parseLong(player_id));
		
		try {
			psmt = conn.prepareStatement("update " + this.database + ".playerStats set avgP = ?, avgAST = ?, avgBLK = ?, avgSTL = ?, avgTO = ?, avgMin = ?, avgFG = ?, avgFG3 = ?, avgFT = ? where playerId = ?");
			psmt.setDouble(1, newPlayer.getAvgP());
			psmt.setDouble(2, newPlayer.getAvgAST());
			psmt.setDouble(3, newPlayer.getAvgBLK());
			psmt.setDouble(4, newPlayer.getAvgSTL());
			psmt.setDouble(5, newPlayer.getAvgTO());
			psmt.setDouble(6, newPlayer.getAvgMin());
			psmt.setDouble(7, newPlayer.getAvgFG());
			psmt.setDouble(8, newPlayer.getAvgFG3());
			psmt.setDouble(9, newPlayer.getAvgFT());
			psmt.setLong(10, Long.parseLong(player_id));
			psmt.executeUpdate();

			//rs.close();
			psmt.close();
			//conn.close();

		} catch (Exception e) {
			//return "ERR";
			e.printStackTrace();
		}

		return "OK";
	}
}
