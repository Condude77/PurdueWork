package cs448;

import org.mapdb.DB;
import org.mapdb.DBMaker;

import java.io.IOException;
import java.util.HashMap;
import java.util.concurrent.ConcurrentMap;
import java.io.BufferedReader;
import java.io.FileReader;


public class project1 {

    /* Data structure for Main-memory back-end */
    HashMap<String,String> mm_map = new HashMap<String,String>();

    /* Data structure for MapDB persistent storage*/
    String dbfile = "data.db";
    DB db = DBMaker.fileDB(dbfile).make();

    // use this for MapDB storage
    ConcurrentMap mapdb = db.hashMap("map").make();

    void load_mainmemory(String file_path) throws IOException {
        /** Put your code here **/
	BufferedReader reader = new BufferedReader(new FileReader(file_path));
	String row = "";
	while((row = reader.readLine()) != null){
		String split[] = row.split("\t", 2);
		mm_map.put(split[0], split[1]);
	}
    }

    void load_mapdb(String file_path) throws IOException{
        /** Put your code here **/
	BufferedReader reader = new BufferedReader(new FileReader(file_path));
	String row = "";
	while((row = reader.readLine()) != null){
		String split[] = row.split("\t", 2);
		mapdb.put(split[0], split[1]);
	}
    }

    String select_file(String key, String file_path) throws IOException{
        /** Put your code here **/
	BufferedReader reader = new BufferedReader(new FileReader(file_path));
	String out = "";
	String row = "";
	while((row = reader.readLine()) != null){
		String split[] = row.split("\t", 2);
		if(split[0].equals(key)){
			out = split[1];
			break;
		}
	}
	return out;
    }

    String select_mainmemory(String key){
        /** Put your code here **/
	String out = mm_map.get(key);
        return out;
    }
    String select_mapdb(String key){
        /** Put your code here **/
        String out = (String) mapdb.get(key);
	return out;
    }
}
