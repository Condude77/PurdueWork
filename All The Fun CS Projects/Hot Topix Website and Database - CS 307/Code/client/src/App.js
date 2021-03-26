import React from "react";
import {StreamApp, UserBar, Button, FlatFeed, LikeButton, Activity, CommentList, CommentField, StatusUpdateForm, RepostButton} from "react-activity-feed";
import "react-activity-feed/dist/index.css";
//import TopicSelect from './topic_selector_comp.js';
import "./topic_selector_comp.css";
import "./index.css";

var router = require("./index.js");

var divStyle = {
  display: "inline-flex",
  width: "90%",
  justifyContent: "flex-end"
};

//load user model
//const TopicList = require('../models/TopicList.js');

//const User = require('../models/User.js');

var urlString = window.location.href;
var splitString = urlString.split("/");
urlString =
  splitString[0] + "/" + splitString[1] + "/" + splitString[2] + "/getToken";
console.log(urlString);
//urlString = "http://localhost:3001/getToken";
// if (!process.env.NODE_ENV || process.env.NODE_ENV === 'development') {
//     // dev code
//     urlString = "http://localhost:3001/getToken";
// } else {
//     // production code
//     urlString = "https://hottopix.herokuapp.com/getToken";
// }

const ListItem = ({value, onClick}) => (
  <li class="item" align="center" onClick={onClick}>
    {value}
  </li>
);

const List = ({items, onItemClick}) => (
  <ul align="center">
    {items.map((item, i) => (
      <ListItem key={i} value={item} onClick={onItemClick(item)} />
    ))}
  </ul>
);

const FinalListItem = ({value}) => <li class="final_item">{value}</li>;

const FinalList = ({items}) => (
  <ul class="final_list">
    {items.map((item, i) => (
      <FinalListItem key={i} value={item} />
    ))}
  </ul>
);

class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      inputValue: "",
      topics: [],
      showSelector: false,
      postTopics: [],
      showFinalTopicsList: false,
      apiResponse: "",
      loaded: false
    };
    this.removeTopic = this.removeTopic.bind(this);
  }

  getToken() {
    console.log("called getToken");
    fetch(urlString)
      .then(res => res.text())
      .then(res => {
        this.setState({
          apiResponse: res,
          loaded: true
        });
        //console.log(res);
      })
      .catch(err => {
        console.log(err);
      });
  }

  triggerShowTopicSelector = () => {
    this.setState({
      ...this.state,
      showSelector: true,
      showFinalTopicsList: false
    });
  };

  triggerHideTopicSelector = () => {
    this.setState({
      ...this.state,
      showSelector: false
    });
  };

  setFinalTopicList = () => {
    if (this.state.topics.length > 0) {
      this.setState({
        ...this.state,
        showFinalTopicsList: true,
        showSelector: false
      });
    } else {
      this.setState({
        ...this.state,
        showSelector: false
      });
    }
  };

  componentWillMount() {
    this.getToken();
  }

  signout() {
    router.router.get("/logout", function (req, res) {
      req.logout();
      res.redirect("/");
    });
  }

  removeTopic(t) {
    var topics = [...this.state.topics]; // make a separate copy of the array
    var index = topics.indexOf(t);
    if (index !== -1) {
      topics.splice(index, 1);
      this.setState({topics: topics, inputValue: ""});
    }
  }

  onClick = () => {
    const {inputValue, topics} = this.state;
    if (inputValue) {
      const nextState = [...topics, inputValue];
      this.setState({topics: nextState, inputValue: ""});
    }
  };

  onItemClick = item => () => {
    this.removeTopic(item);
  };

  onChange = e => this.setState({inputValue: e.target.value});

  render() {
    if (!this.state.loaded) {
      return null;
    }

    const {topics, inputValue} = this.state;

    return (
      <div>
        <br></br>

        <StreamApp
          apiKey="sjc92jugd7js"
          appId="62811"
          token={this.state.apiResponse}
        >
          <StatusUpdateForm
            feedGroup="Timeline"
            FooterItem={
              <div style={divStyle} align="left">
                {!this.state.showSelector && (
                  <Button
                    buttonStyle="primary"
                    onClick={this.triggerShowTopicSelector}
                  >
                    Add Topics
                  </Button>
                )}

                {this.state.showSelector && (
                  <div class="comp_div">
                    <div class="inner_div">
                      <input
                        class="inner_div"
                        type="text"
                        value={inputValue}
                        onChange={this.onChange}
                      />
                      <Button buttonStyle="primary" onClick={this.onClick}>
                        Add
                      </Button>
                      <Button
                        buttonStyle="info"
                        onClick={this.triggerHideTopicSelector}
                      >
                        Cancel
                      </Button>
                      <Button
                        buttonStyle="primary"
                        onClick={this.setFinalTopicList}
                      >
                        Done
                      </Button>
                    </div>

                    <div class="list_div">
                      <List items={topics} onItemClick={this.onItemClick} />
                    </div>
                  </div>
                )}

                {this.state.showFinalTopicsList && (
                  <div class="final_comp_div" align="center">
                    <div class="list_div" align="center">
                      <FinalList items={topics} />
                    </div>
                  </div>
                )}
              </div>
            }
          />

          <FlatFeed
            feedGroup="Timeline"
            options={{reactions: {recent: true}}}
            notify
            Activity={props => (
              <Activity
                {...props}
                Footer={() => (
                  <div style={{padding: "8px 16px"}}>
                    <LikeButton {...props} />
                    <RepostButton {...props} />
                    <CommentField
                      activity={props.activity}
                      onAddReaction={props.onAddReaction}
                    />
                    <CommentList activityId={props.activity.id} />
                  </div>
                )}
              />
            )}
          />
        </StreamApp>
      </div>
    );
  }
}

export default App;
