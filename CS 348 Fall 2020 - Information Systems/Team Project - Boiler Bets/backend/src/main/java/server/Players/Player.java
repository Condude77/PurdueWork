package server.Players;

import javax.persistence.*;
import javax.validation.constraints.*;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;

@Entity
@Table(name="players")
@EntityListeners(AuditingEntityListener.class)
public class Player {
    @Id
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    private long playerId;

    @NotNull(message = "Please enter teamId")
    private long teamId;

    @NotBlank
    private String teamName;

    @NotBlank
    private String position;

    private String collegeName;

    private String playerNumber;

    public String playerStats;

    private String playerName;

    public Player() {
        super();
    }

    public Player(long teamId, String teamName, String Position, String playerName) {
        super();
        this.teamId = teamId;
        this.teamName = teamName;
        this.position = Position;
        this.playerName = playerName;
    }

    public Player(String teamID, String position, String collegeName, String playerNumber, String playerName) {
        super();
        this.teamId = Long.parseLong(teamID);
        this.position = position;
        this.collegeName = collegeName;
        this.playerNumber = playerNumber;
        this.playerName = playerName;
    }

    public Player(long teamId, String teamName, String Position, String collegeName, String playerNumber, String playerName) {
        this(teamId, teamName, Position, playerName);
        this.collegeName = collegeName;
        this.playerNumber = playerNumber;
    }

    public long getId() {
        return this.playerId;
    }

    public void setId(long id) {
        this.playerId = id;
    }

    public long getTeamId() {
        return this.teamId;
    }

    public String getPlayerName() {
        return this.playerName;
    }

    public String getTeamName() {
        return this.teamName;
    }

    public String getPosition() {
        return this.position;
    }

    public void setTeam(long teamId, String teamName) {
        this.teamId = teamId;
        this.teamName = teamName;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public void setPlayerName(String playerName) {
        this.playerName = playerName;
    }

    public String toString() {
        return "playerId: " + this.playerId + ", teamId: " + this.teamId + ", teamName: " + this.teamName + ", playerName: " + this.playerName +", position: " + this.position + ", collegeName: " + this.collegeName + ", playerNumber: " + this.playerNumber;
    }
}
