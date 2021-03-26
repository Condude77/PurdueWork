import React, {Component} from 'react';
import { withRouter } from 'react-router-dom';
import Popup from 'react-popup';
import {Redirect} from 'react-router-dom';
import {Link} from 'react-router-dom'
import axios from 'axios';
import { config } from '../config/config.js';
import 'materialize-css';
import '@material-ui/core';
import { Person, Book } from '@material-ui/icons';
import '../css/SignIn.css';
import PopupReact from 'react-popup/dist/Popup.react';

class SignIn extends Component {

    constructor(props) {
        super(props);
        this.handelSubmit = this.handelSubmit.bind(this);
        this.state = {
            username: '',
            password: '',
            loggedIn: false,
            logBool: ""
        }
    }

    
    handleChange =(e)=>{
        this.setState({
            [e.target.id]: e.target.value
        })
    }

    handleRedirect = (e) =>{
        e.preventDefault();
        this.props.history.push('/SignUp');
    }

    /* Here we need to check the username with backend and then log the user in, redirect to dashboard */
    async handelSubmit (e) {
        e.preventDefault();
        await fetch(config.url.API_URL + '/user/login', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                username: this.state.username,
                password: this.state.password,
                name: '',
                email: '',
            })
        }).then(res => res.json()).then((data) => {
            console.log(data);
            this.setState({logBool: data}) 
        }).catch(console.log)

        if (this.state.logBool.status == "ok.") {
            console.log("Credentials are correct");
            this.setState({loggedIn: true, user: this.state.username});
            this.props.history.push({
                pathname: '/Dashboard',
                state: {
                    username: this.state.username,
                    userId: this.state.userId,
                    loggedIn: this.state.loggedIn
                }
            });
        }
        else{
            Popup.alert('Error: Incorrect Username or Password!');
            console.log("WRONG CREDENTIALS");
        }
    }

    render() {
        console.log(this.state);
        return (
            <div className = "container">
                <div className = "centered">
                    <div className="col s6">
                        <form onSubmit={this.handelSubmit}>
                            <div className="input-field col s12">
                                <Person />
                                <input type="text" id="username" placeholder="Username" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <Book />
                                <input type="password" id="password" placeholder="Password" onChange = {this.handleChange} />
                            </div>
                            <div className= "spacer"></div>
                            <button className = "btn btn-primary" type="submit">Log in</button>
                        </form>
                        <div className= "spacer"></div>
                        <Link to="/SignUp" className = "btn btn-primary">Sign Up</Link>
                    </div>
                </div>
            </div>
        )
    }
}

//withRouter(SignIn);
export default withRouter(SignIn);
