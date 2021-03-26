package server.PlayerStats;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.ObjectCodec;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;

public class PlayerStatsDeserializer  extends StdDeserializer<PlayerStats> {

    private static final long serialVersionUID = 1L;

    public PlayerStatsDeserializer() {
        this(null);
    }

    public PlayerStatsDeserializer(Class<PlayerStats> t) {
        super(t);
    }

    @Override
    public PlayerStats deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        ObjectCodec c = new ObjectMapper();
        JsonNode node = c.readTree(p);
        //ArrayList<String> ingreds = new ArrayList<>();
        //ingreds.add("TEST");
        //String[] tt = new String[10];
        //JsonNode i = node.get("ingredients");
        //System.out.println(i);
        PlayerStats u = new PlayerStats(
            0,
            node.get("avgP").asText(),
            node.get("avgAST").asText(),
            node.get("avgBLK").asText(),
            node.get("avgSTL").asText(),
            node.get("avgTO").asText(),
            node.get("avgMin").asText(),
            node.get("avgFG").asText(),
            node.get("avgFG3").asText(),
            node.get("avgFT").asText());
		

        return u;
    }
    
}
