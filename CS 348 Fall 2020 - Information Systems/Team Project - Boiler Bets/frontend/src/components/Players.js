import React, {Component} from 'react';
import 'materialize-css';
import Navbar from './Navbar.js';
import '../css/SignIn.css';
import PlayerCard from './PlayerCard';
import '../css/TeamStats.css';
import {Grid, Button, Modal} from 'semantic-ui-react';
import '../css/Player.css';
import AddPlayer from './AddPlayer.js';
// import { Modal } from '@material-ui/core';
import { Add } from '@material-ui/icons';


export default class Players extends Component {

    constructor(props) {
        super(props);
        this.state = {
            playerList: [],
            playerId: '',
            playerName: 'Select A Player',
            teamName: '',
            position: '',
            playerNumber: '' //NEEDS TO BE ADDED... I AM NOT GETTING IT IN THE DATA RETURNED TO FRONT END
        };
        this.handleGetPlayers = this.handleGetPlayers.bind(this);
        this.handlePlayerClicked = this.handlePlayerClicked.bind(this);
        this.handleGetPlayerInfo = this.handleGetPlayerInfo.bind(this);
        this.handleAddPlayer = this.handleAddPlayer.bind(this);
    }

    /* Once we have user bets, use map() to put them into a materialize table or somethin */
    render() {
        //console.log(this.props.location.state);
        console.log(this.state.playerList);
        var playerCardValue;
        if (this.state.playerInfo === undefined) {
            playerCardValue = 
            <PlayerCard playerName={this.state.playerName}/>
        }
        else {
            console.log(this.state.playerInfo);
            playerCardValue = 
            <PlayerCard style={{width: '35vw'}} stacked teamName={this.state.teamName} playerName={this.state.playerName} position={this.state.position} keys={this.state.keys} values={this.state.values} />
        }

        return (
            <div className = "container"> {/* Section that displays the navbar */}
                <Navbar />
                <Grid style={{width: '100vw', height: '90vh'}} columns={2}>
                    <Grid.Column style={{width: '35vw', height: '80vh'}}> {/* Grid colummn for player list */}
                            <div className="scrollbox">
                                {this.state.playerList.map((playerList, index) => {
                                    console.log(playerList);
                                    console.log(this.state.playerList[index]);
                                    return( <Button style={{width: '20vw'}} horizontalAlign='middle' onClick={() => {this.handlePlayerClicked(this.state.playerList[index].id);}}>
                                                {this.state.playerList[index].playerName}
                                            </Button>)
                                    }
                                )}
                            </div>
                    </Grid.Column>

                    <Grid.Column style={{width: '35vw', height: '80vh'}} verticalAlign='middle' textAlign='center'> {/* Grid colummn for player stats once a player is clicked */}
                        {playerCardValue}
                        <Modal 
                            trigger={<Button style={{width: '35vw'}} stacked verticalAlign='left' textAlign='center' color='blue'>Add Player</Button>}
                            header={'Add Player'}
                            content={<AddPlayer/>}
                            actions={['Close']}
                        />
                    </Grid.Column>
                </Grid>
            </div>
        )
    }

    async handleAddPlayer(){

    }

    /* Get player info from playerStats and render in popup */
    async handleGetPlayerInfo (playerId) {
        console.log("HEREEEEEEEEE");
        console.log(this.state.playerId)
        console.log(playerId);
        
        await fetch('http://localhost:8080/player/getPlayerStats', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                playerId: parseInt(playerId)
            })
        }).then(res => res.text()).then((data) => {
            console.log(data);
            // console.log(JSON.parse(data));
            // console.log(JSON.parse(data).teamName);
            // console.log(JSON.parse(data).playerId);
            // console.log(JSON.parse(data).playerStats);
            // console.log(JSON.parse(JSON.parse(data).playerStats));
            // console.log(Object.keys(JSON.parse(data).playerStats));
            // console.log(Object.values(JSON.parse(data).playerStats));
            //console.log(Object.keys(JSON.parse(data).playerStats)[0]);
            //console.log(Object.values(JSON.parse(data).playerStats)[0]);
            var temp = JSON.parse(data).playerStats.replace(/'/g,'"');
            // console.log(JSON.parse(temp));
            // console.log("KEYS: ");
            // console.log(Object.keys(JSON.parse(JSON.parse(data).playerStats.replace(/'/g,'"'))));
            // console.log(Object.values(JSON.parse(JSON.parse(data).playerStats.replace(/'/g,'"'))));




            this.setState({
                teamName: JSON.parse(data).teamName,
                playerId: JSON.parse(data).playerId,
                playerName: JSON.parse(data).playerName,
                position: JSON.parse(data).position,
                playerInfo: JSON.parse(JSON.parse(data).playerStats.replace(/'/g,'"')),
                keys: Object.keys(JSON.parse(JSON.parse(data).playerStats.replace(/'/g,'"'))),
                values: Object.values(JSON.parse(JSON.parse(data).playerStats.replace(/'/g,'"')))
            })
        }).catch(console.log)
    }

    async handleGetPlayers() {
        await fetch('http://localhost:8080/player/getAllPlayers' ,{
            method: 'GET',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            }
        }).then(response => response.json())
            .then(data => {
                console.log(data);
                console.log(Object.values(data));
                this.setState({playerList: data});
                //console.log(this.state.playerList);
            }).catch(console.log("ERR")); // here's how u set variables u want to use later
    }

    /* Current user info should be stored in this.props.history */
    async componentDidMount() {
        this.handleGetPlayers();
    }

    async handlePlayerClicked(playerId) {
        this.handleGetPlayerInfo(playerId);
        this.setState({playerId: playerId})
    }
    
} 
