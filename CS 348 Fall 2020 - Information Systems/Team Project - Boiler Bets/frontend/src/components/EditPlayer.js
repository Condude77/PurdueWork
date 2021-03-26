import React, {Component} from 'react';
import { withRouter } from 'react-router-dom';
import {Redirect} from 'react-router-dom';
import axios from 'axios';
import { config } from '../config/config.js';
import 'materialize-css';
import '@material-ui/core';
import { Person, Book, Email, PersonOutline } from '@material-ui/icons';
import '../css/SignIn.css';

class EditPlayer extends Component {

    constructor(props) {
        super(props);
        this.handelSubmit = this.handelSubmit.bind(this);
        this.state = {
            team_id: '',
            position: '',
            college_name: '',
            player_name: '',
            player_number: '',
            avgP: -1,
            avgAST: -1,
            avgBLK: -1,
            avgSTL: -1,
            avgTO: -1,
            avgMIN: -1,
            avgFG: -1,
            avgFG3: -1,
            avgFT: -1
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
        console.log(this.props);
        var ft;
        var fg3;
        var fg;
        var min;
        var to;
        var stl;
        var blk;
        var ast;
        var p;
        if (this.state.avgFT == -1){
            console.log("FT doesnt work");
            ft = this.props.avgFT;
        } else {
            ft = this.state.avgFT;
        }
        if (this.state.avgFG3 == -1){
            console.log("FG3 doesnt work");
            fg3 = this.props.avgFG3;
        } else {
            fg3 = this.state.avgFG3;
        }
        if (this.state.avgFG == -1){
            console.log("FG doesnt work");
            fg = this.props.avgFG;
        } else {
            fg = this.state.avgFG;
        }
        if (this.state.avgMIN == -1){
            console.log("MIN doesnt work");
            min = this.props.avgMIN;
        } else {
            min = this.state.avgMIN;
        }
        if (this.state.avgTO == -1){
            console.log("TO doesnt work");
            to = this.props.avgTO;
        } else {
            to = this.state.avgTO;
        }
        if (this.state.avgSTL == -1){
            console.log("STL doesnt work");
            stl = this.props.avgSTL;
        } else {
            stl = this.state.avgSTL;
        }
        if (this.state.avgAST == -1){
            console.log("AST doesnt work");
            ast = this.props.avgAST;
        } else {
            ast = this.state.avgAST;
        }
        if (this.state.avgBLK == -1){
            console.log("BLK doesnt work");
            blk = this.props.avgBLK;
        } else {
            blk = this.state.avgBLK;
        }
        if (this.state.avgP == -1){
            console.log("FT doesnt work");
            p = this.props.avgP;
        } else {
            p = this.state.avgP;
        }
        console.log(this.props.playerId);
        await fetch('http://localhost:8080/player/updatePlayer?playerId='+this.props.playerId, {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                avgP: p,
                avgAST: ast,
                avgBLK: blk,
                avgSTL: stl,
                avgTO: to,
                avgMin: min,
                avgFG: fg,
                avgFG3: fg3,
                avgFT: ft
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
        console.log(this.props);
        return (
            <div className = "container">
                <div className = "centered">
                    <div className="col s6">
                        <form onSubmit={this.handelSubmit}>
                            
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
                            <button className = "btn btn-primary" type="submit">Edit Player</button>
                        </form>
                    </div>
                </div>
            </div>
        )
    }
}

export default withRouter(EditPlayer);
