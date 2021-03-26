package server.SQL;

import java.sql.*;
import java.util.ArrayList;
import server.Team.TeamStats;

public class TeamSQL {

	private String url;
	private Connection conn;
	Statement smt;
	PreparedStatement psmt;
	ResultSet rs;
	private String database;

	public TeamSQL(){	
		url = "jdbc:mysql://35.238.205.120:3306/";
		try{
			conn = DriverManager.getConnection(url, "root", "root");//development
			smt = conn.createStatement();
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
		database = "boiler_bets";

	}

	public String getTeamID(String teamName){
		try {
			System.out.println(teamName);
			psmt = conn.prepareStatement("select teamId from "+this.database+".teams where teamName = ?");
			psmt.setString(1, teamName);
			rs = psmt.executeQuery();
			String teamId = "";
			while(rs.next()){
				teamId = rs.getString("teamId");
			}
			rs.close();
			psmt.close();
			conn.close();
			System.out.println(teamId);
			return teamId;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	public String getTeamNameStat(String teamName) {
		try {
			/* Get ID */

			System.out.println(teamName);
			psmt = conn.prepareStatement("select teamId from "+this.database+".teams where teamName = ?");
			psmt.setString(1, teamName);
			rs = psmt.executeQuery();
			String teamId = "";
			while(rs.next()){
				teamId = rs.getString("teamId");
			}
			System.out.println(teamId);

			psmt = conn.prepareStatement("select * from "+this.database+".teamStats where teamId = ?");
			psmt.setString(1, teamId);
			rs = psmt.executeQuery();
			String result = "{ ";
			/*
			while (rs.next()) {
				result += "wins: " + rs.getInt("wins");
				result += ", losses: " + rs.getInt("losses");
				result += ", winPct: " + rs.getInt("winPct");
				result += ", avgPoints: " + rs.getInt("avgP");
				result += ", avgFGP: " + rs.getInt("avgFGP");
				result += ", avg3PP: " + rs.getInt("avg3PP");
				result += ", avgFTP: " + rs.getInt("avgFTP");
				result += ", avgAST: " + rs.getInt("avgAST");
				result += ", avgTO:" + rs.getInt("avgTO");
				result += ", avgBLK: " + rs.getInt("avgBLK");
			}
			*/
			while (rs.next())
			{
                result+= "\""+teamName+"\": {";
				result+= "\"wins\": "+rs.getInt("wins");
				result+= ",\"losses\": "+rs.getInt("losses");
				result+= ",\"winPct\": "+rs.getDouble("winPct");
                result+= ",\"avgP\": "+rs.getDouble("avgP");
                result+= ",\"avgFGP\": "+rs.getDouble("avgFGP");
                result+= ",\"avg3PP\": "+rs.getDouble("avg3PP");
                result+= ",\"avgFTP\": "+rs.getDouble("avgFTP");
                result+= ",\"avgAST\": "+rs.getDouble("avgAST");
                result+= ",\"avgTO\": "+rs.getDouble("avgTO");
                result+= ",\"avgBLK\": "+rs.getDouble("avgBLK");
				result+=" }, ";
			}
            result = result.substring(0, result.length()-2);
            result+=" }";
			rs.close();
			psmt.close();
			conn.close();
			System.out.println(result);
			return result;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}



public String getTeamStats(){
		try{
			psmt = conn.prepareStatement("select * from (select * from "+ this.database+".teamStats) as team_stats,"+ 
            "(select teamId, teamName from "+ this.database+".teams) as teams where team_stats.teamId = teams.teamId");
			rs = psmt.executeQuery();
			String all = "{ ";
			//ArrayList<TeamStats> stats = new ArrayList<TeamStats>();
            //TeamStats ts = new TeamStats();

			while (rs.next())
			{
                //TeamStats u = new TeamStats();

				//u.teamId=  rs.getInt("teamId");
                //u.teamName = 
                all+= "\""+rs.getString("teamName")+"\": {";
				all+= "\"wins\": "+rs.getInt("wins");
				all+= ",\"losses\": "+rs.getInt("losses");
				all+= ",\"winPct\": "+rs.getDouble("winPct");
                all+= ",\"avgP\": "+rs.getDouble("avgP");
                all+= ",\"avgFGP\": "+rs.getDouble("avgFGP");
                all+= ",\"avg3PP\": "+rs.getDouble("avg3PP");
                all+= ",\"avgFTP\": "+rs.getDouble("avgFTP");
                all+= ",\"avgAST\": "+rs.getDouble("avgAST");
                all+= ",\"avgTO\": "+rs.getDouble("avgTO");
                all+= ",\"avgBLK\": "+rs.getDouble("avgBLK");

				// String lastLogin = rs.getInt("lastLogin");
				all+=" }, ";
				//stats.add(u);
			
				
			}
            all = all.substring(0, all.length()-2);
            all+=" }";
			rs.close();
			psmt.close();
			conn.close();
			System.out.println(all);
			return all;

		}catch(Exception e){
			e.printStackTrace();
			return null;//"/user Fail";
		}
	}

}