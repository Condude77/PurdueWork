import React from "react";
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import {Header, Modal, Button} from 'semantic-ui-react';
import AddPlayer from './AddPlayer.js';
import EditPlayer from './EditPlayer.js';


export default class PlayerCard extends React.Component{

    constructor(props){
        super(props);
        this.handleDeletePlayer = this.handleDeletePlayer.bind(this);
    }

    render() {
    console.log('IN Player Card');
    
        if (this.props.playerName === 'Select A Player' || this.props.values === undefined) { //Only is displayed if a team has not been chosen yet
            return(<Card style={{width: '35vw'}} verticalAlign='middle' centered>
                        <CardContent color='grey'>
                            <Header>{this.props.playerName}</Header>
                        </CardContent>
                    </Card>)
        }
        else{ //Else a team has been selected
            // console.log(this.props.info[0]);
            console.log(this.props.keys);
            console.log(this.props.values[0]);

            return(
                <div>
                    <Card style={{width: '35vw'}} verticalAlign='middle' centered>
                        <CardContent>
                            <Header>{this.props.playerName}: {this.props.teamName}</Header>
                                Position: {this.props.position} <br/>
                                Average Assists: {this.props.values[2]} <br/>
                                Average Blocks: {this.props.values[3]} <br/>
                                Average Field Goals: {this.props.values[7]} <br/>
                                Average 3-Point Field Goals: {this.props.values[8]} <br/>
                                Average Free Throws: {this.props.values[9]} <br/>
                                Average Minutes: {this.props.values[6]} <br/>
                                Average Points: {this.props.values[1]} <br/>
                                Average Steals: {this.props.values[4]} <br/>
                                Average Turn Overs: {this.props.values[5]} <br/>
                                <Modal 
                                    trigger={<Button style={{width: '30w'}} stacked verticalAlign='left' textAlign='center' color='yellow'>Edit Player</Button>}
                                    header={'Edit Player'}
                                    content={<EditPlayer playerId={this.props.values[0]} playerName={this.props.playerName} avgAST={this.props.values[2]} avgBLK={this.props.values[3]} avgFG={this.props.values[7]} avgFG3={this.props.values[8]} avgFT={this.props.values[9]} avgMIN={this.props.values[6]} avgP={this.props.values[1]} avgSTL={this.props.values[4]} avgTO={this.props.values[5]}/>}
                                    actions={['Close']}
                                />
                                <Modal 
                                    trigger={<Button style={{width: '30w'}} stacked verticalAlign='left' textAlign='center' color='red'>Delete Player</Button>}
                                    header={'Delete Player'}
                                    content={<Button style={{width: '30w'}} onClick={() => {this.handleDeletePlayer()}}color='red'>Are you sure you want to delete this Player?</Button>}
                                    actions={['Close']}
                                />
                        </CardContent>
                    </Card>
                </div>
            )//End return(...)
        }
    } //End render(){...}

    async handleDeletePlayer() {
        console.log("DELETING PLAYER");
        //need request
        console.log(this.props.values[0]);
        await fetch('http://localhost:8080/player/deletePlayer?playerId='+this.props.values[0], {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                playerId: parseInt(this.props.values[0])
            })
        }).then(res => res.text()).then((data) => {
            console.log("HERE"+data);
            window.location.replace(window.location.href);
        }).catch(console.log)
    }
};