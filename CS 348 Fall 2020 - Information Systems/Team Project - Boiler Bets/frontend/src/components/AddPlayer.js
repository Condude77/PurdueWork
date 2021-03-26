import React, {Component} from 'react';
import { withRouter } from 'react-router-dom';
import {Redirect} from 'react-router-dom';
import axios from 'axios';
import { config } from '../config/config.js';
import 'materialize-css';
import '@material-ui/core';
import { Person, Book, Email, PersonOutline } from '@material-ui/icons';
import '../css/SignIn.css';

class AddPlayer extends Component {

    constructor(props) {
        super(props);
        this.handelSubmit = this.handelSubmit.bind(this);
        this.state = {
            team_id: '',
            position: '',
            college_name: '',
            player_name: '',
            player_number: '',
            avgP: 0,
            avgAST: 0,
            avgBLK: 0,
            avgSTL: 0,
            avgTO: 0,
            avgMin: 0,
            avgFG: 0,
            avgFG3: 0,
            avgFT: 0
        }
    }

    
    handleChange =(e)=>{
        this.setState({
            [e.target.id]: e.target.value
        })
    }

    /* Here we need to insert the new fields into the database, log the user in, and redirect to dashboard*/
    /* Also perform any error checking (duplicate values, whatever) */
    async handelSubmit (e) {
        e.preventDefault();
        console.log(this.state);
        await fetch('http://localhost:8080/player/insertPlayer', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                teamID: this.state.team_id,
                position: this.state.position,
                collegeName: this.state.college_name,
                playerName: this.state.player_name,                
                playerNumber: this.state.player_number,
                avgP: this.state.avgP,
                avgAST: this.state.avgAST,
                avgBLK: this.state.avgBLK,
                avgSTL: this.state.avgSTL,
                avgTO: this.state.avgTO,
                avgMin: this.state.avgMin,
                avgFG: this.state.avgFG,
                avgFG3: this.state.avgFG3,
                avgFT: this.state.avgFT
            })
        }).then(res => res.text()).then((data) => {
            console.log(data);
            this.props.history.push({
                pathname: '/Players',
                state: {
                    username: this.props.location.state.username,
                    loggedIn: true
                }
            });
            window.location.replace(window.location.href);
        }).catch(console.log)
    }

    render() {
        return (
            <div className = "container">
                <div className = "centered">
                    <div className="col s6">
                        <form onSubmit={this.handelSubmit}>
                            <div className="input-field col s12">
                                <Person />
                                <input type="number" min = "1" max = "30" id="team_id" placeholder="Team ID" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <Book />
                                <input type="text" id="position" placeholder="Position" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <Email />
                                <input type="text" id="college_name" placeholder="College Name" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <PersonOutline />
                                <input type="text" id="player_name" placeholder="Player Name" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <PersonOutline />
                                <input type="text" id="player_number" placeholder="Player Number" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <PersonOutline />
                                <input type="number" id="avgP" placeholder="Average Points" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgAST" placeholder="Average Assists" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgBLK" placeholder="Average Blocks" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgSTL" placeholder="Average Steals" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgTO" placeholder="Average Turnovers" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgMin" placeholder="Average Minutes" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgFG" placeholder="Average Field Goals" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgFG3" placeholder="Average 3 Pointers" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                            <PersonOutline />
                                <input type="number" id="avgFT" placeholder="Average Freethrows" onChange = {this.handleChange} />
                            </div>
                            <div className= "spacer"></div>
                            <button className = "btn btn-primary" type="submit">Add Player</button>
                        </form>
                    </div>
                </div>
            </div>
        )
    }
}

export default withRouter(AddPlayer);
