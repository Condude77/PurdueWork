import React, {Component} from 'react';
import { withRouter } from 'react-router-dom';
import {Redirect} from 'react-router-dom';
import axios from 'axios';
import { config } from '../config/config.js';
import 'materialize-css';
import '@material-ui/core';
import { Person, Book, Email, PersonOutline } from '@material-ui/icons';
import '../css/SignIn.css';

class SignUp extends Component {

    constructor(props) {
        super(props);
        this.handelSubmit = this.handelSubmit.bind(this);
        this.state = {
            username: '',
            password: '',
            email: '',
            name: ''
        }
    }

    
    handleChange =(e)=>{
        this.setState({
            [e.target.id]: e.target.value
        })
    }

    /* Here we need to insert the new fields into the database, log the user in, and redirect to dashboard*/
    /* Also perform any error checking (duplicate values, whatever) */
    async handelSubmit (e) {
        e.preventDefault();
        await fetch(config.url.API_URL + '/user/', {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                username: this.state.username,
                password: this.state.password,
                email: this.state.email,
                name: this.state.name
            })
        }).then(res => res.json()).then((data) => {
            this.setState({response: data})
        }).catch(console.log);

        let m = this.state.response.status;
        if (m === "ok") {
            console.log("Successful signup/signin");
            this.setState({loggedIn: true, user: this.state.username});
            this.props.history.push({
                pathname: '/Dashboard',
                state: {
                    username: this.state.username,
                    userId: this.state.userId,
                    loggedIn: this.state.loggedIn
                }
            });
        }else{
            console.log("Error signing up");
        }
    }

    render() {
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
                            <div className="input-field col s12">
                                <Email />
                                <input type="email" id="email" placeholder="Email" onChange = {this.handleChange} />
                            </div>
                            <div className="input-field col s12">
                                <PersonOutline />
                                <input type="text" id="name" placeholder="Name" onChange = {this.handleChange} />
                            </div>
                            <div className= "spacer"></div>
                            <button className = "btn btn-primary" type="submit">Create Account!</button>
                        </form>
                    </div>
                </div>
            </div>
        )
    }
}

export default withRouter(SignUp);
