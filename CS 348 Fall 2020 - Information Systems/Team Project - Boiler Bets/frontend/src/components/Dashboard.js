import React, {Component} from 'react';
import { withRouter } from 'react-router-dom';
import axios from 'axios';
import 'materialize-css';
import Navbar from './Navbar.js';
import { makeStyles } from '@material-ui/styles';
import '../css/SignIn.css';

const useStyles = makeStyles((theme) => ({
    root: {
      flexGrow: 1,
    },
    menuButton: {
      marginRight: theme.spacing(2),
    },
    title: {
      flexGrow: 1,
    },
  }));

class Dashboard extends Component {

    constructor(props) {
        super(props);
    }

    /* Use axios to get user bets from db here */
    /* Current user info should be stored in this.props.history */
    async getUserBets() {

    }


    /* Once we have user bets, use map() to put them into a materialize table or somethin */
    render() {
        console.log(this.props.location.state);
        return (
            <div className = "container">
                <Navbar />
            </div>
        )
    }
}

//withRouter(Dashboard);
export default withRouter(Dashboard);
