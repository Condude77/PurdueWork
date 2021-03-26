package server.SQL;

import java.sql.*;
import java.util.ArrayList;
import server.Team.TeamStats;

public class betStatSQL {
	private String url;
	private Connection conn;
    //public String username;
	Statement smt;
	PreparedStatement psmt;
	ResultSet rs;
	private String database;

	public betStatSQL(){	
		url = "jdbc:mysql://35.238.205.120:3306/";
		try{
			conn = DriverManager.getConnection(url, "root", "root");//development
			smt = conn.createStatement();
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
		database = "boiler_bets";
    }
    
    public String getAvailableBets() {
        try {
            psmt = conn.prepareStatement("select * from "+ this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            System.out.println(result);
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

        
    public String getMaxBet() {
        try {
            psmt = conn.prepareStatement("select * from "+ this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId where odds = (SELECT max(odds) FROM "+this.database+".betStats)");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String getPopularBets() {
        try {
            psmt = conn.prepareStatement("select * from "+this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId, (SELECT betId, count(betId) as cnt FROM "+this.database+".userBets group by betId having count(betId) > 1 order by count(betId) DESC) as b where a.betId = b.betId order by b.cnt DESC;");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
                result+= ",\"Count\": "+rs.getInt("cnt");
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String sortByOddsAsc() {
        try {
            psmt = conn.prepareStatement("select * from "+ this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId order by odds ASC");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String sortByOddsDesc() {
        try {
            psmt = conn.prepareStatement("select * from "+ this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId order by odds DESC");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String getBetForHighestWinPercTeam() {
        try {
            psmt = conn.prepareStatement("select * from "+this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId where a.teamId in (select teamId from "+this.database+".teamStats where winPct = (select max(winPct) from "+this.database+".teamStats));");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String sortByType() {
        try {
            psmt = conn.prepareStatement("SELECT * FROM "+this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId order by type asc;");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String sortByTeam() {
        try {
            psmt = conn.prepareStatement("SELECT * FROM "+this.database+".betStats as a JOIN "+this.database+".teams as b on a.teamId = b.teamId order by b.teamName asc;");
            rs = psmt.executeQuery();
            String result = "{";
			int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
				result+= ",\"TeamID\": "+rs.getInt("teamId");
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";
            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }

    public String getUserBets(String username) {
        try {

            conn.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
            conn.setAutoCommit(false);
//String username = "";
            psmt = conn.prepareStatement("SELECT u.username, a.userId, a.betId, amount, payout, betSuccess, startDate, endDate, odds, type, teamName FROM "+this.database+".users as u, "+this.database+".userBets as a JOIN ("+this.database+".betStats as b JOIN "+this.database+".teams as t on b.teamId = t.teamId) on a.betId = b.betId where u.userId = a.userId and u.username = ?;");
            psmt.setString(1, username);

            rs = psmt.executeQuery();
            String result = "{";
            int x = 0;
			while (rs.next())
			{
                result+= "\""+x+"\": { \"BetId\": "+rs.getString("betId");
                result+= ",\"username\": \""+rs.getString("username")+"\"";
				result+= ",\"UserId\": "+rs.getInt("userId");
                result+= ",\"amount\": "+rs.getInt("amount");
                result+= ",\"Payout\": "+rs.getDouble("Payout");
                result+= ",\"BetSuccess\": \""+rs.getString("BetSuccess")+"\"";
                result+= ",\"TeamName\": \""+rs.getString("teamName")+"\"";
				result+= ",\"StartDate\": \""+rs.getString("startDate")+"\"";
				result+= ",\"EndDate\": \""+rs.getString("endDate")+"\"";
                result+= ",\"Odds\": "+rs.getDouble("odds");
                result+= ",\"Type\": \""+rs.getString("type")+"\"";
				result+=" }, ";
                x++;
			}
            if ( result.equals("{")){
                System.out.println("NO BETS FOR USER");
                result = "{}";
                rs.close();
                conn.commit();
                psmt.close();
                conn.close();
                return result;
            }
            result = result.substring(0, result.length()-2);
            result +=" }";

            conn.commit();

            rs.close();
			psmt.close();
            conn.close();
            
            return result;
        } catch (Exception e) {
            e.printStackTrace();
			return null; //"/bet Fail";
        }
    }




}