import React from "react";
import {Grid, Button, Modal, Card, CardContent, Header, Form, Popup} from 'semantic-ui-react';
import '../css/CustomPopup.css'; 
import '../css/SignIn.css';


export default class BetAmount extends React.Component{

    constructor(props){
        super(props);
        this.state = {
            betInfo: '',
            renderPopup: false,
            amount: 0,
            badMessage: ''
        }
        this.handleClick = this.handleClick.bind(this);
        this.togglePopup = this.togglePopup.bind(this);
    }
    // BetId: 7
    // EndDate: "03/24/21"
    // Odds: 0.53
    // StartDate: "03/23/19"
    // TeamID: 20
    // TeamName: "New York Knicks"
    // Type: "Money Line"

    render() {
        console.log(this.props.cardInfo);
        console.log(this.props.cardInfo['Type']);
        return(
           <div>
               <Button style={{width: '40vw'}} onClick = {this.handleClick} top-margin = "0px">
                    <Header> {this.props.cardInfo['TeamName']}: {this.props.cardInfo['Type']} </Header>
                    Odds: {this.props.cardInfo['Odds']}<br/>
                    StartDate:{this.props.cardInfo['StartDate']} <br/>
                    EndDate: {this.props.cardInfo['EndDate']}


               </Button>
               
               {this.state.renderPopup ? 
                <div className='popup'>  
                    <div className='popup\_inner'>  
                        <Form onSubmit={this.togglePopup}>
                            <div className="input-field col s12">
                                <input type="number" id="amount" placeholder="Bet Amount" onChange = {this.handleChange} />
                            </div>
                            <div className= "spacer"></div>
                            <button className = "btn btn-primary" type="submit">Make Bet</button>
                            
                        </Form>
                    </div>  
                </div>  
                : ''}
            </div>
        )//End return(...)
    } //End render(){...}

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
    }).catch(console.log);


        console.log(this.state.betInfo);
        //PARSE BET INFO FOR ID, CALCULATE PAYOUT BASED ON ODDS
        // var index = this.state.betInfo.indexOf("\"");
        // var betID = this.state.betInfo.substr(index + 1, this.state.betInfo.indexOf("\"", index + 1) - index - 1);
        // console.log(betID);
        // index = this.state.betInfo.indexOf("Odds");
        // var multiplier = this.state.betInfo.substr(index + 7, this.state.betInfo.indexOf(",", index + 1) - index -7 );
        // console.log(multiplier);
        var payout = parseFloat(this.state.betInfo['Odds']) * this.state.amount;
        console.log(payout);
        console.log(this.state.betInfo['BetId']);

        await fetch('http://localhost:8080/userBets/insertUserBet', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                userId: id,
                betId: this.state.betInfo['BetId'],
                amount: this.state.amount,
                payout: payout,
                betSuccess: "Pending"
            })
        }).then(res => res.text()).then((data) => {
            console.log(data);
            let popFlag = false;
            let tempMsg = '';
            if ( data == "BLOCKED") {
                console.log("BLOCKED");
                //popFlag = true;
                tempMsg = "Inactive! Please only bet on active cards."
            }
            this.setState({  
                betInfo: '',
                renderPopup: popFlag,
                amount: 0,
                badMessage: tempMsg
            });
            //window.location.replace('http://localhost:3000/UserBets');
        }).catch(console.log)
    }

/* Render amount popup */
async handleClick (e) {
    e.preventDefault();
    this.setState({
        betInfo: this.props.cardInfo,
        renderPopup: true,
        amount: 0,
        badMessage: ''
    })
}

    
handleChange =(e)=>{
    this.setState({
        [e.target.id]: e.target.value,
        badMessage: '',
    })
}

};