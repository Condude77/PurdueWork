import java.util.ArrayList;

/**
 * Main MusicPlayer class
 */
public class MusicPlayer {

    protected ArrayList<String> playlist = new ArrayList<String>();
    protected int status;
    protected int volume;

    static final int OFF = 0;
    static final int ON = 1;
    static final int PLAYING = 2;
    static final int VOLUME_MAX = 100;
    static final int VOLUME_MIN = 0;
    static final int PLAYLIST_SIZE = 10;

    public MusicPlayer() {
        volume = VOLUME_MIN;
        status = OFF;
        playlist.add("Words");
        playlist.add("Stairway To Heaven");
        playlist.add("Dream On");
        playlist.add("Thrift Shop");
        playlist.add("Buzzcut Season");
    }

    /**
     * play: Plays track from playlist based on mentioned trackNo
     * by printing the track name to stdout
     */
    public void play(int trackNo) {
    	if(trackNo < playlist.size()){
    		this.status = PLAYING;
    		System.out.println("Playing: " + playlist.get(trackNo));
    	}
    	if(trackNo > playlist.size()){
    		System.out.println("Track " + trackNo + " doesn't exist");
    	}
    }

    /**
     * turnOn: Turn the player on
     * and print a msg to stdout
     */
    public void turnOn() {
    	this.status = ON;
    	System.out.println("Player ON");
    }

    /**
     * turnOff: Turn the player off and
     * print a msg to stdout
     */
    public void turnOff() {
    	this.status = OFF;
    	System.out.println("Player OFF");
    }

    /**
     * increaseVolume: Method to increase volume
     * and print current volume to stdout
     *
     */
    public void increaseVolume(int increment) {
    	if(this.volume + increment <= VOLUME_MAX){
    		this.volume += increment;
    		System.out.println("Volume: " + this.volume);
    	}
    	if(this.volume + increment > VOLUME_MAX){
    		this.volume = VOLUME_MAX;
    		System.out.println("Volume: " + this.volume);
    	}
    }

    /**
     * decreaseVolume: Method to decrease volume
     * and print current volume to stdout
     *
     */
    public void decreaseVolume(int decrement)  {
    	if(this.volume - decrement >= VOLUME_MIN){
    		this.volume -= decrement;
    		System.out.println("Volume: " + this.volume);
    	}
    	if(this.volume + decrement < VOLUME_MAX){
    		this.volume = VOLUME_MIN;
    		System.out.println("Volume: " + this.volume);
    	}
    }

    /**
     * showPlaylist: Displays current playlist
     * to stdout
     */
    public void showPlaylist() {
    	for(int i = 0; i < playlist.size(); i++){
    		System.out.printf("%d: %s%n", i, playlist.get(i));
    	}
    }

    /**
     * Main method
     *
     */
    public static void main(String[] args) {
        System.out.println("\nCDPlayer");
        CDPlayer cd1 = new CDPlayer(1234);
        cd1.turnOn();
        System.out.println("\nCDPlayer Playlist:");
        cd1.showPlaylist();
        cd1.increaseVolume(20);
        cd1.play();
        cd1.nextTrack();
        cd1.previousTrack();
        cd1.play(21);
        cd1.decreaseVolume(30); // less than VOLUMN_MIN
        cd1.turnOff();

        System.out.println("\nEthernetPlayer");
        EthernetPlayer mp1 = new EthernetPlayer(123);
        mp1.turnOn();
        System.out.println("the playlist has: " + mp1.playlist.size()
                           + " songs");
        mp1.increaseVolume(50);
        mp1.play(2);
        mp1.play(21);
        mp1.addToPlaylist("Broken");
        mp1.addToPlaylist("Sweet Child'O Mine");
        mp1.deleteFromPlaylist("Broken");
        mp1.download("haha"); // not in the download list.
        mp1.download("Dark Horse");
        mp1.download("Royals");
        mp1.download("Let Her Go");
        mp1.download("The Fox");
        mp1.download("Counting Stars");

        mp1.increaseVolume(60); // too loud
        System.out.println("\nEthernetPlayer Playlist:");
        mp1.showPlaylist();
        mp1.turnOff();
    }

}
