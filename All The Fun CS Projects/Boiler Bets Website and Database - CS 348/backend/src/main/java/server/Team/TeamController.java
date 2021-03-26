package server.Team;

import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import server.SQL.TeamSQL;
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
@RequestMapping(path="/team")
public class TeamController {
    @GetMapping("/teamStats")
    public String getTeamStats(){
        TeamSQL ts = new TeamSQL();
        return ts.getTeamStats();
    }

    @PostMapping("/teamStatsByName")
    public String getTeamInfo(@RequestBody String teamName){
        TeamSQL ts = new TeamSQL();
        String trimmedName = "";
        boolean startName = false;
        for (int i = 0; i < teamName.length(); i++) {
            if (teamName.charAt(i) == ':') {
                i += 2;
                startName = true;
            }
            if (startName) {
                if (teamName.charAt(i) != '}' && teamName.charAt(i) != '"')
                trimmedName +=teamName.charAt(i);
            }
        }
        return ts.getTeamNameStat(trimmedName);
    }

}