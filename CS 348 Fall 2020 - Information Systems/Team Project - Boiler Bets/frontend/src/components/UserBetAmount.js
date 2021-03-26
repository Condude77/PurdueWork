import React from "react";
// import Card from '@material-ui/core/Card';
// import CardContent from '@material-ui/core/CardContent';
import {Grid, Button, Modal, Card, CardContent, Header} from 'semantic-ui-react';
//import Button from '@material-ui/core/Button';
import '../css/CustomPopup.css'; 
import '../css/SignIn.css';


export default class UserBetAmount extends React.Component{

    constructor(props){
        super(props);
        this.handleClick = this.handleClick.bind(this);
        this.togglePopup = this.togglePopup.bind(this);
        this.state = {
            betInfo: '',
            renderPopup: false,
            amount: 0
        }
    }

    /* Create new userBet in database */
   async togglePopup(e) {
        e.preventDefault();
        var id;
        //GET USER ID
        await fetch('http://localhost:8080/user/getIdByUsername', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                username: this.props.userInfo
            })
        }).then(res => res.json()).then((data) => {
            console.log(data);
            id = data;
        }).catch(console.log)



        //PARSE BET INFO FOR ID, CALCULATE PAYOUT BASED ON ODDS
        var index = this.state.betInfo.indexOf("\"");
        var betID = this.state.betInfo.substr(index + 1, this.state.betInfo.indexOf("\"", index + 1) - index - 1);
        console.log(betID);
        index = this.state.betInfo.indexOf("Odds");
        var multiplier = this.state.betInfo.substr(index + 7, this.state.betInfo.indexOf(",", index + 1) - index -7 );
        console.log(multiplier);
        var payout = parseFloat(multiplier) * this.state.amount;
        console.log(payout);
        //console.log(this.props.userInfo);
        await fetch('http://localhost:8080/userBets/insertUserBet', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                userId: id,
                betId: betID,
                amount: this.state.amount,
                payout: payout,
                betSuccess: "Pending"
            })
        }).then(res => res.text()).then((data) => {
            console.log(data);
            this.setState({  
                betInfo: '',
                renderPopup: false,
                amount: 0
            });
        }).catch(console.log)
    }

    /* Render amount popup */
    async handleClick (e) {
        e.preventDefault();
        this.setState({
            betInfo: this.props.cardInfo,
            renderPopup: true,
            amount: 0
        })
    }

        
    handleChange =(e)=>{
        this.setState({
            [e.target.id]: e.target.value
        })
    }

    render() {
        console.log(this.props.cardInfo['username']);
        return(
           <div>
               {/* <Button onClick = {this.handleClick} top-margin = "0px"> */}
                    <Card textAlign='center' horizontalAlign='middle' verticalAlign='middle' style={{width: "30vw"}} >
                        <CardContent>
                            {/* <div className="middle aligned"> */}
                            <Header> {this.props.cardInfo['TeamName']}: {this.props.cardInfo['Type']} </Header>
                            
                            Date Bet Submitted: {this.props.cardInfo['StartDate']} - End Date: {this.props.cardInfo['EndDate']} <br/>
                            Odds: {this.props.cardInfo['Odds']}<br/>
                            Bet Amount: ${parseFloat(this.props.cardInfo['amount']).toFixed(2)} - To Win: ${parseFloat(this.props.cardInfo['Payout']).toFixed(2)} <br/>
                            <br/>
                            Bet Status: {this.props.cardInfo['BetSuccess']}



                            {/* </div> */}
                        </CardContent>
                    </Card>
               {/* </Button> */}
               {/* {this.state.renderPopup ? 
                <div className='popup'>  
                    <div className='popup\_inner'>  
                        <form onSubmit={this.togglePopup}>
                            <div className="input-field col s12">
                                <input type="number" id="amount" placeholder="Bet Amount" onChange = {this.handleChange} />
                            </div>
                            <div className= "spacer"></div>
                            <button className = "btn btn-primary" type="submit">Make Bet</button>
                        </form>
                    </div>  
                </div>  
                : ''} */}
            </div>
        )//End return(...)
    } //End render(){...}

};