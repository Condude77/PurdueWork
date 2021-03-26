package server.userBets;

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

public class userBetDeserializer extends StdDeserializer<userBet> {
    private static final long serialVersionUID = 1L;

    public userBetDeserializer() {
        this(null);
    }

    public userBetDeserializer(Class<userBet> t) {
        super(t);
    }

    @Override
    public userBet deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        ObjectCodec c = new ObjectMapper();
        JsonNode node = c.readTree(p);

        userBet u = new userBet(
            node.get("userId").asText(),
            node.get("betId").asText(),
            node.get("amount").asText(),
            node.get("payout").asText(),
            node.get("betSuccess").asText());

        return u;
    }
}
