package server.userBets;

import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import server.SQL.userBetSQL;
import server.SQL.betStatSQL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.security.*;
import java.nio.charset.StandardCharsets;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.Version;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;

@RestController
@CrossOrigin(origins = "http://localhost:3000")
//@CrossOrigin //(origins = "https://fiveo-clocksomewhere.firebaseapp.com/", maxAge =  3600, allowedHeaders = "*")     //production
@RequestMapping(path="/userBets")
public class userBetController {
    @PostMapping("/insertUserBet")
    public String insertUserBet(@RequestBody String betInfo) throws JsonParseException, JsonMappingException, IOException {
		ObjectMapper om = new ObjectMapper();
		SimpleModule sm = new SimpleModule("userBetDeserializer", new Version(1, 0, 0, null, null, null));
		sm.addDeserializer(userBet.class, new userBetDeserializer());
		om.registerModule(sm);
		userBet u = om.readValue(betInfo, userBet.class);

        userBetSQL ub = new userBetSQL();
        return ub.insertBet(u);
    }

    @GetMapping("/getUserBets")
    public String getUserBets(@RequestParam(name="username") String username) {
        betStatSQL bs = new betStatSQL();
        //bs.username = username;
        return bs.getUserBets(username);
    }
}
