import java.util.ArrayList;

/**
 * Class CDPlayer
 */
public class CDPlayer extends MusicPlayer {

    private int deviceID;
    private int thisTrack;

    /**
     * Constructor for CD-Player
     */
    public CDPlayer(int id) {
    	this.deviceID = id;
    }

    /**
     * Over-ride Method: turnOn
     */
    public void turnOn() {
        super.turnOn();
    	this.thisTrack = 0;
    }

    /**
     * Over-ride Method: turnOff
     */
    public void turnOff() {
        super.turnOff();
    	this.thisTrack = 0;
    }

    /**
     * Method to play next track in the playlist by
     * printing it to stdout and changing current
     */
    public void nextTrack() {
    	thisTrack += 1;
    	System.out.println("Playing: " + playlist.get(thisTrack));
    }

    /**
     * Method to play previous track in the playlist by
     * printing it to stdout and changing current
     */
    public void previousTrack() {
    	thisTrack -= 1;
    	System.out.println("Playing: " + playlist.get(thisTrack));
    }

    /**
     * Method to play current track
     */
    public void play() {
    	System.out.println("Playing: " + playlist.get(thisTrack));
    }

}
