package server.SQL;

import server.userBets.userBet;
import java.sql.*;
import java.util.ArrayList;
import server.Team.TeamStats;
import java.text.SimpleDateFormat;
import java.util.Date;

public class userBetSQL {
    private String url;
	private Connection conn;
	Statement smt;
	PreparedStatement psmt;
	ResultSet rs;
    private String database;
    
    public userBetSQL(){	
		url = "jdbc:mysql://35.238.205.120:3306/";
		try{
			conn = DriverManager.getConnection(url, "root", "root");//development
			smt = conn.createStatement();
			
		}catch(Exception e){
			e.printStackTrace();
			
		}
		database = "boiler_bets";
    }

    public String insertBet(userBet betInfo) {
        try{
			//Doing Transaction
			// need to check if bet exists in betStats table first
			conn.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
			conn.setAutoCommit(false);

			//check if bet still exists in betstats and that endDate is not past
			String check = "select betId, endDate from "+this.database+".betStats where betId = ?;";
			PreparedStatement psmtCheck = conn.prepareStatement(check);
			psmtCheck.setLong(1, betInfo.getBetId());
			rs = psmtCheck.executeQuery();

			int existingBetId = 0;//rs.getInt("betId");
			String endDate = "";//rs.getString("endDate");
			if( rs.next() ) {
				existingBetId = rs.getInt("betId");
				endDate = rs.getString("endDate");
			}
			
			System.out.println("endDate: "+endDate+" and betId: "+existingBetId);

			Date currDate = new Date();
			SimpleDateFormat fmt = new SimpleDateFormat("MM/dd/yy");
			String now = fmt.format(currDate);
			System.out.println("CurrDate: "+now);

			Date db = new SimpleDateFormat("MM/dd/yyyy").parse(endDate);
			Date insert = new SimpleDateFormat("MM/dd/yyyy").parse(now);
			if ( !db.before(insert) && existingBetId == betInfo.getBetId() ) {
				System.out.println("Date and betId are ok");
				
				//if ok, insert userbets
				String query = "insert into "+ this.database+".userBets "+ 
					"(userId, betId, amount, payout, betSuccess) "+
					"values "+ 
					"(?, ?, ?, ?, ?)";
				psmt = conn.prepareStatement(query);
				psmt.setLong(1, betInfo.getUserId());
				psmt.setLong(2, betInfo.getBetId());
				psmt.setLong(3, betInfo.getAmount());
				psmt.setDouble(4, betInfo.getPayout());
				psmt.setString(5, betInfo.getBetSuccess());
				System.out.println(query);
				psmt.executeUpdate();
				
				conn.commit();
				psmt.close();
			} else {
				//otherwise rollback
				conn.rollback();
				psmtCheck.close();
				conn.close();
				return "BLOCKED";
			}
			

			psmtCheck.close();
			//psmt.close();
            conn.close();
			return "OK";
		}catch(Exception e){
			e.printStackTrace();
			System.out.println("Error inserting new user to DB.");
			return "ERROR INSERTING USERBET";
		}
    }
}
