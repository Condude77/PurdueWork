import React, {Component} from 'react';
import Navbar from './Navbar.js';
import 'materialize-css';
import '../css/SignIn.css';
import '../css/TeamStats.css';
import '../css/Player.css';
import {Grid, Header, Modal, Button, Dropdown} from 'semantic-ui-react';
import BetAmount from './BetAmount.js';


export default class BetStats extends Component {

    constructor(props) {
        super(props);
        this.state = {
            betStatList: [],
            filter: 1
        };
        this.handleGetBetStats = this.handleGetBetStats.bind(this);
        this.handleGetBetsForHighestWinTeam = this.handleGetBetsForHighestWinTeam.bind(this);
        this.handleGetMaxBet = this.handleGetMaxBet.bind(this);
        this.handleGetPopularBets = this.handleGetPopularBets.bind(this);
        this.handleSortByOddsAsc = this.handleSortByOddsAsc.bind(this);
        this.handleSortByOddsDesc = this.handleSortByOddsDesc.bind(this);
        this.handleSortByTeam = this.handleSortByTeam.bind(this);
        this.handleSortByType = this.handleSortByType.bind(this);
        //this.handleClick = this.handleClick.bind(this);
        this.handleFilter = this.handleFilter.bind(this);
    }

    async handleGetBetStats() {
        await fetch('http://localhost:8080/betStats/getAvailableBets' ,{
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


    async handleGetPopularBets() {
        await fetch('http://localhost:8080/betStats/getPopularBets' ,{
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

    async handleGetMaxBet() {
        await fetch('http://localhost:8080/betStats/getMaxBet' ,{
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

    async handleSortByOddsAsc() {
        await fetch('http://localhost:8080/betStats/sortByOddsAsc' ,{
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

    async handleSortByOddsDesc() {
        await fetch('http://localhost:8080/betStats/sortByOddsDesc' ,{
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

    async handleGetBetsForHighestWinTeam() {
        await fetch('http://localhost:8080/betStats/getBetForHighestWinTeam' ,{
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

    async handleSortByType() {
        await fetch('http://localhost:8080/betStats/sortByType' ,{
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

    async handleSortByTeam() {
        await fetch('http://localhost:8080/betStats/sortByTeam' ,{
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

    async handleFilter(e, key) {
        let filter = {key}.key.value;
        if (filter == 1) {
            console.log('1!!!!!!!');
            this.handleGetBetStats();
        }
        else if (filter == 2) {
            console.log('2!!!!!!!');
            this.handleGetBetsForHighestWinTeam();
        }
        else if (filter == 3) {
            console.log('3!!!!!!!');
            this.handleGetMaxBet();
        }
        else if (filter == 4) {
            console.log('2!!!!!!!');
            this.handleGetPopularBets();
        }
        else if (filter == 5) {
            console.log('3!!!!!!!');
            this.handleSortByOddsAsc();
        }
        else if (filter == 6) {
            console.log('2!!!!!!!');
            this.handleSortByOddsDesc();
        }
        else if (filter == 7) {
            console.log('3!!!!!!!');
            this.handleSortByTeam();
        }
        else if (filter == 8) {
            console.log('2!!!!!!!');
            this.handleSortByType();
        }
    }

    /* Once we have user bets, use map() to put them into a materialize table or somethin */
    render() {
        //console.log(this.props.location.state);
        console.log(this.state.betStatList[0]);
        console.log(this.state.betStatList[1]);
        console.log(this.props.location.state.username);
        const searchOptions = [
            { key: '1', value: '1', text: 'All Available Bets' },
            { key: '2', value: '2', text: 'Get Bets For Highest Team Win' },
            { key: '3', value: '3', text: 'Max Bet' },
            { key: '4', value: '4', text: 'Popular Bets' },
            { key: '5', value: '5', text: 'Odds Ascending' },
            { key: '6', value: '6', text: 'Odds Descending' },
            { key: '7', value: '7', text: 'By Team Alphabetically' },
            { key: '8', value: '8', text: 'By Type Alphabetically' },
        ];

        
        
        
        return (
            <div className = "container"> {/* Section that displays the navbar */}
                <Navbar />
                <Grid style={{width: '100vw', height: '90vh'}} columns={2}>
                    <Grid.Column style={{width: '35vw', height: '80vh'}}> {/* Grid colummn for bet list */}
                            <Modal 
                                trigger={<Button style={{width: '39.3vw'}} stacked verticalAlign='left' textAlign='center' color='blue'>Filter</Button>}
                                header={'Filter Available Bets'}
                                content={<Dropdown placeholder='Select Filter' fluid search selection options={searchOptions} onChange={(e, key) => this.handleFilter(e, key)}/>}
                                actions={['Close']}
                                
                            />
                        <div className="scrollBox" style={{width: '40vw'}}>
                            {this.state.betStatList.map((betStatList, index) => {
                                    console.log(this.state.betStatList[index]);
                                    return(<BetAmount cardInfo={this.state.betStatList[index]} userInfo={this.props.location.state.username}/>)
                                }
                            )}
                        </div>
                    </Grid.Column>
                </Grid>
            </div>
        )
    }
}
