package server.Players;

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

public class PlayerDeserializer extends StdDeserializer<Player> {

    /**
     *
     */
    private static final long serialVersionUID = 1L;

    public PlayerDeserializer() {
        this(null);
    }

    public PlayerDeserializer(Class<Player> t) {
        super(t);
    }

    @Override
    public Player deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        ObjectCodec c = new ObjectMapper();
        JsonNode node = c.readTree(p);
        //ArrayList<String> ingreds = new ArrayList<>();
        //ingreds.add("TEST");
        //String[] tt = new String[10];
        //JsonNode i = node.get("ingredients");
        //System.out.println(i);
        Player u = new Player(
            node.get("teamID").asText(),
            node.get("position").asText(),
            node.get("collegeName").asText(),
            node.get("playerNumber").asText(),
            node.get("playerName").asText());
		

        return u;
    }

	
}