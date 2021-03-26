import React, {Component} from 'react';
import 'materialize-css';
import Navbar from './Navbar.js';
import '../css/SignIn.css';
import { Grid, Header, Button } from 'semantic-ui-react';
import '../css/TeamStats.css';
import TeamStatsCard from './TeamStatsCard.js';


export default class TeamStats extends Component {

    constructor(props) {
        super(props);
        this.state = {
            teamList: [],
            teamInfo: [],
            teamName: 'Select A Team'
        };
        this.handleGetTeamStats = this.handleGetTeamStats.bind(this);
        this.handleGetTeamInfo = this.handleGetTeamInfo.bind(this);
        this.handleTeamClicked = this.handleTeamClicked.bind(this);
    }

    /* Use axios to get team stats from db here */
    /* Current user info should be stored in this.props.history */
    async componentDidMount() {
        this.handleGetTeamStats();
    }

    /* Once we have user bets, use map() to put them into a materialize table or somethin */
    render() {
        //console.log(this.props.location.state);
        console.log('RERENDER');
        return (
            <div className = "container"> {/* Section that displays the navbar */}
                <Navbar />
                <Grid style={{width: '100vw', height: '90vh'}} columns={2}>
                    <Grid.Column style={{width: '35vw', height: '80vh'}}> {/* Grid colummn for team list */}
                            <div className="scrollBox">
                                {this.state.teamList.map((teamList, index) => {
                                    console.log(teamList[index]);
                                        return( <Button style={{width: '20vw'}} horizontalAlign='middle' onClick={() => {this.handleTeamClicked(this.state.teamList[index]);}}>
                                                    {this.state.teamList[index]}
                                                </Button>)
                                    }
                                )}
                            </div>
                    </Grid.Column>

                    <Grid.Column style={{width: '50vw', height: '80vh'}} verticalAlign='middle' textAlgin='middle'> {/* Grid colummn for team stats once a team is clicked */}
                        <TeamStatsCard teamName={this.state.teamName} info={this.state.teamInfo} />
                        {/* <Header>HEYEYEYEYEYEYEYEYEYEYEYYEY</Header> */}
                    </Grid.Column>
                </Grid>
            </div>
        

            
                
                
        )
    }

    async handleGetTeamStats() {
        console.log("HERE!!!");
        await fetch('http://localhost:8080/team/teamStats' ,{
            method: 'GET',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            }
        }).then(response => response.json())
            .then(data => {
                //console.log("setting state");
                //console.log(Object.keys(data));
                this.setState({teamList: Object.keys(data)});
                //console.log(this.state.teamList);
            }).catch(console.log("ERR")); // here's how u set variables u want to use later
    }

    /* Get team info from teamStats and render in popup */
    async handleGetTeamInfo (teamName) {
        //e.preventDefault();
        //console.log(this.props.teamName);
        if (teamName != this.state.teamName){
            await fetch('http://localhost:8080/team/teamStatsByName', {
                method: 'POST',
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    teamName: teamName
                })
            }).then(res => res.text()).then((data) => {
                console.log(data);
                console.log(Object.keys(JSON.parse(data)));
                console.log(Object.keys(data));
                console.log(Object.values(JSON.parse(data))[0]);
                this.setState({
                    teamInfo: Object.values(JSON.parse(data))[0]
                })
                
            }).catch(console.log)
        }
    }

    async handleTeamClicked(teamName) {
        this.handleGetTeamInfo(teamName);
        this.setState({teamName: teamName})
    }



} //end default class TeamStats

//withRouter(Dashboard);
//withRouter(TeamStats);
