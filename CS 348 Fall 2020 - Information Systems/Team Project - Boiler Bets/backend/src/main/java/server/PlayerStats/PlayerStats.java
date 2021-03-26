package server.PlayerStats;

public class PlayerStats {
    
    private long playerId;
    private Double avgP;
    private Double avgAST;
    private Double avgBLK;
    private Double avgSTL;
    private Double avgTO;
    private Double avgMin;
    private Double avgFG;
    private Double avgFG3;
    private Double avgFT;

    public PlayerStats(long playerId, String avgP, String avgAST, String avgBLK, String avgSTL, String avgTO, String avgMin, String avgFG, String avgFG3, String avgFT) {
        this.playerId = playerId;
        this.avgP = Double.parseDouble(avgP);
        this.avgAST = Double.parseDouble(avgAST);
        this.avgBLK = Double.parseDouble(avgBLK);
        this.avgSTL = Double.parseDouble(avgSTL);
        this.avgTO = Double.parseDouble(avgTO);
        this.avgMin = Double.parseDouble(avgMin);
        this.avgFG = Double.parseDouble(avgFG);
        this.avgFG3 = Double.parseDouble(avgFG3);
        this.avgFT = Double.parseDouble(avgFT);
    }

    public void setID(long id) {
        this.playerId = id;
    }

    public Double getAvgP() {
        return avgP;
    }

    public Double getAvgAST() {
        return avgAST;
    }

    public Double getAvgBLK() {
        return avgBLK;
    }

    public Double getAvgSTL() {
        return avgSTL;
    }

    public Double getAvgTO() {
        return avgTO;
    }

    public Double getAvgMin() {
        return avgMin;
    }

    public Double getAvgFT() {
        return avgFT;
    }

    public Double getAvgFG() {
        return avgFG;
    }

    public Double getAvgFG3() {
        return avgFG3;
    }
}
