package server.Team;

public class TeamStats{

	public int teamId;
    public String teamName;
	public int wins;
    public int losses;
    public Double winPct;
    public Double avgP;
    public Double avgFGP;
    public Double avg3PP;
    public Double avgFTP;
    public Double avgAST;
    public Double avgTO;
    public Double avgBLK;




	public TeamStats(){
		this.teamId = 0;
        this.teamName = "";
	    this.wins = 0;
        this.losses = 0;
        this.winPct = 0.0;
        this.avgP = 0.0;
        this.avgFGP = 0.0;
        this.avg3PP = 0.0;
        this.avgFTP = 0.0;
        this.avgAST = 0.0;
        this.avgTO = 0.0;
        this.avgBLK = 0.0;
	
	}


	public TeamStats(int teamId,
	 int wins,
     int losses,
     Double winPct,
     Double avgP,
     Double avgFGP,
     Double avg3PP,
     Double avgFTP,
     Double avgAST,
     Double avgTO,
     Double avgBLK)
	{
		this.teamId = teamId;
	    this.wins = wins;
        this.losses = losses;
        this.winPct = winPct;
        this.avgP = avgP;
        this.avgFGP = avgFGP;
        this.avg3PP = avg3PP;
        this.avgFTP = avgFTP;
        this.avgAST = avgAST;
        this.avgTO = avgTO;
        this.avgBLK = avgBLK;
	}


}