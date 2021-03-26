import React, {Component} from 'react';

import 'materialize-css';
import Navbar from './Navbar.js';
import '../css/SignIn.css';
import { Grid } from 'semantic-ui-react';

import '../css/TeamStats.css';

import '../css/Player.css';

import UserBetAmount from './UserBetAmount.js';


export default class UserBets extends Component {

    constructor(props) {
        super(props);
        this.state = {
            betStatList: [],
        };
        this.handleGetBetStats = this.handleGetBetStats.bind(this);
        //this.handleClick = this.handleClick.bind(this);
    }

    async handleGetBetStats() {
        console.log(this.props.location.state.userId);
        await fetch('http://localhost:8080/userBets/getUserBets?username='+this.props.location.state.username ,{
            method: 'GET',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            }
        }).then(response => response.text())
            .then(data => {
                console.log(data);
                console.log(JSON.parse(data));
                let t = JSON.parse(data);
                console.log(Object.values(t));
                // var index = 0;
                var i = 0;
                this.setState({betStatList: Object.values(JSON.parse(data))});
                
                //console.log(this.state.playerList);
            }).catch(console.log("ERR"));
    }

    /* Current user info should be stored in this.props.history */
    async componentDidMount() {
        this.handleGetBetStats();
    }

    /* Once we have user bets, use map() to put them into a materialize table or somethin */
    render() {
        //console.log(this.props.location.state);
        console.log(this.state.betStatList[0]);
        console.log(this.state.betStatList[1]);
        console.log(this.props.location.state);
        return (
            <div className = "container"> {/* Section that displays the navbar */}
                <Navbar /><br/><br/>
                <Grid centered  style={{width: '100vw', height: '60vh'}} columns={1}>
                    
                    <Grid.Column textAlign='center' style={{width: '25vw'}}>  
                    
                            {this.state.betStatList.map((betStatList, index) => {
                                    console.log(this.state.betStatList[index]);
                                    return(<UserBetAmount cardInfo={this.state.betStatList[index]} userInfo={this.props.location.state.username}/>)
                                }
                            )}              
                    </Grid.Column>   
                         
                </Grid>
            </div>
        )
    }
} 
