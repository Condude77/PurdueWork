import './App.css';
import React, {Component} from 'react';
import {BrowserRouter, Switch, Route} from 'react-router-dom';
import Popup from 'react-popup';
import SignIn from './components/SignIn.js';
import SignUp from './components/SignUp.js';
import Dashboard from './components/Dashboard.js';
import TeamStats from './components/TeamStats.js';
import Players from './components/Players.js';
import AddPlayer from './components/AddPlayer.js';
import BetStats from './components/BetStats.js';
import UserBets from './components/UserBets.js';

import { render } from '@testing-library/react';

class App extends Component {
  
  constructor(props) {
    super(props);
    this.state = {
      user: undefined
    }
  }

  render() {
    return (
      <BrowserRouter>
      <div className="App">
      <Popup />
          <Switch>
            <Route exact path ='/' component ={SignIn}/>
            <Route exact path ='/SignUp' component ={SignUp}/>
            <Route exact path ='/Dashboard' component ={Dashboard}/>
            <Route exact path ='/TeamStats' component = {TeamStats}/>
            <Route exact path ='/Players' component = {Players}/>
            <Route exact path ='/AddPlayer' component={AddPlayer}/>
            <Route exact path ='/BetStats' component={BetStats}/>
            <Route exact path ='/UserBets' component ={UserBets}/>
          </Switch>
      </div>
      </BrowserRouter>
    )
  }
}

export default App;
