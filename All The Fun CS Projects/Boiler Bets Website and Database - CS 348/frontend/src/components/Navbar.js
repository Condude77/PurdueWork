import React, {Component} from 'react';
import { makeStyles } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import { withRouter } from 'react-router-dom';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import IconButton from '@material-ui/core/IconButton';
import { withStyles } from '@material-ui/core/styles';
import { DesktopWindows } from '@material-ui/icons';

const styles = theme => ({
  root: {
    flexGrow: 1,
  },
  menuButton: {
    marginRight: theme.spacing(2),
  },
  title: {
    flexGrow: 1,
  },
});

class Navbar extends Component {

  constructor(props) {
    super(props);
    this.handleSignOut = this.handleSignOut.bind(this);
    this.handleTeamStatsPage = this.handleTeamStatsPage.bind(this);
    this.handlePlayersPage = this.handlePlayersPage.bind(this);
    this.handleAvailableBetsPage = this.handleAvailableBetsPage.bind(this);
    this.handleUserBetsPage = this.handleUserBetsPage.bind(this);
    this.state = {
      username: this.props.location.state.username,
      loggedIn: this.props.location.state.loggedIn
    }
  }

  handleSignOut(e) {
    e.preventDefault();
    this.setState({
      username: '',
      loggedIn: false
    })
    this.props.history.push('/');
  }

  /* --------------------------------------------------------------------------------- VERY IMPORTANTE ------------------------------------------------------------------------------------ */
  //ALWAYS use this format when redirecting, so we keep the current userID across all pages
  handleTeamStatsPage() {
    this.props.history.push({
      pathname: '/TeamStats',
      state: {
          username: this.state.username,
          userId: this.state.userId,
          loggedIn: this.state.loggedIn
      }
  });
  }

    /* --------------------------------------------------------------------------------- VERY IMPORTANTE ------------------------------------------------------------------------------------ */
  //ALWAYS use this format when redirecting, so we keep the current userID across all pages
  handlePlayersPage() {
    this.props.history.push({
      pathname: '/Players',
      state: {
          username: this.state.username,
          userId: this.state.userId,
          loggedIn: this.state.loggedIn
      }
  });
  }

  handleAvailableBetsPage() {
    this.props.history.push({
      pathname: '/BetStats',
      state: {
          username: this.state.username,
          userId: this.state.userId,
          loggedIn: this.state.loggedIn
      }
  });
  }

  handleUserBetsPage() {
    this.props.history.push({
      pathname: '/UserBets',
      state: {
          username: this.state.username,
          userId: this.state.userId,
          loggedIn: this.state.loggedIn
      }
  });
  }

  render() {
    const { classes } = this.props;
    return (
        <div className={classes.root}>
            <AppBar position="static">
            <Toolbar>
                <IconButton edge="start" className={classes.menuButton} color="inherit" aria-label="menu">
                </IconButton>
                <Typography variant="h6" className={classes.title}>
                BoilerBets
                </Typography>
                <Button color="inherit" onClick={this.handlePlayersPage}>Players</Button>
                <Button color="inherit" onClick={this.handleTeamStatsPage}>Teams</Button>
                <Button color="inherit" onClick={this.handleAvailableBetsPage}>Available Bets</Button>
                <Button color="inherit" onClick={this.handleUserBetsPage}>Your Bets</Button>
                <Button color="inherit" onClick={this.handleSignOut}>Sign Out</Button>
            </Toolbar>
            </AppBar>
        </div>
      );
  }
}

//withRouter(Navbar);
export default withRouter(withStyles(styles)(Navbar));