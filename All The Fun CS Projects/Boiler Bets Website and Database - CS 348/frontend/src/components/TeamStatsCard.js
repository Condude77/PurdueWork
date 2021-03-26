import React from "react";
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import {Header} from 'semantic-ui-react';
import {Link} from "react-router-dom";
import Typography from '@material-ui/core/Typography';


export default class TeamStatsCard extends React.Component{

    constructor(props){
        super(props);
        this.state={
            info: []
        }
    }

    async componentDidMount(){
        //console.log(this.props.post);
    }

    render() {
        if (this.props.teamName === 'Select A Team') { //Only is displayed if a team has not been chosen yet
            return(<Card style={{width: '30vw'}} verticalAlign='middle' centered>
                        <CardContent color='grey'>
                            <Header>{this.props.teamName}</Header>
                        </CardContent>
                    </Card>)
        }
        else{ //Else a team has been selected
            return(
                <div>
                    <Card style={{width: "30vw"}} centered >
                        <CardContent>
                            <Header>{this.props.teamName}</Header>
                                Wins: {this.props.info.wins} <br/>
                                Losses: {this.props.info.losses} <br/>
                                Win Percent: {this.props.info.winPct}% <br/>
                                Average 3-Point Percent: {this.props.info.avg3PP}% <br/>
                                Average Assists: {this.props.info.avgAST} <br/>
                                Average Blocks: {this.props.info.avgBLK} <br/>
                                Average Field Goal Percent: {this.props.info.avgFGP}% <br/>
                                Average Free Throw Percent: {this.props.info.avgFTP}% <br/>
                                Average Points: {this.props.info.avgP} <br/>
                                Average Turn-Overs: {this.props.info.avgTO} <br/>
                        </CardContent>
                    </Card>
                </div>
            )//End return(...)
        }
    } //End render(){...}

};