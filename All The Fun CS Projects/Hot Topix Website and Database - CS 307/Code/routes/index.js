var express = require('express');
var router = express.Router();
var path = require('path')
const bcrypt = require('bcryptjs');
const passport = require('passport');
const { ensureAuthenticated, forwardAuthenticated } = require('../config/auth');
//var popup = require('popups');

//load user model
const User = require('../models/User.js');


// npm install getstream --save
var stream = require('getstream');

client = stream.connect('sjc92jugd7js', 'rhtnurcusnqwkw4gpe2tx84wdd9wg6k92zn6q2wh9fs77t7bb8zzu8wbdvnfxhzm', '62811');
//client = stream.connect('sjc92jugd7js', null, '62811');

//client.user('vpan').delete();
var loggedInUser;
var currentUser;
var feed;
var currentFollowing;
var globalTopics;


async function getFollowers(){
  followers = await feed.followers();
  //console.log(followers)
  return followers;
}

async function getFollowing(){
  followers = await feed.following();
  //console.log(followers)
  return followers;
}

async function getActivities(){
  activities = await feed.get({ limit:5, offset:5 });
  return activities;
}

//welcome page
router.get('/', (req, res) => {
  res.render('index');
});

router.get('/home', ensureAuthenticated, (req, res) => {
    console.log("getting activities");
    getActivities().then(function(result){
      console.log("got activities");
      console.log(result);
    });

    User.findOne({ username: "topicHolder" }).then(user => {
      if (user) {
        topics = user.topicList;

        res.render(path.join(__dirname, '../build', 'index.html'), {
          user: loggedInUser,
          topics: topics
        });
      } else {
        res.redirect('/home');
      }
    });

});

//logout
router.get('/logout', function(req, res){
  req.logout();
  res.redirect('/');
});

//get stream token
router.get('/getToken', ensureAuthenticated, function(req, res) {
  res.send(currentUser);
});

//about page
router.get('/about', (req,res) => {
  res.render('about');
});

//contact page
router.get('/contact', (req,res) => {
  res.render('contact');
});

//login page
router.get('/login', forwardAuthenticated, (req, res) => {
  res.render('login');
});

//signup page
router.get('/signup', forwardAuthenticated, (req, res) => {
  res.render('signup');
});

//edit profile page
router.get('/edit_profile', ensureAuthenticated, (req,res) => {
  // console.log(loggedInUser);
  //
  // User.findOne({ username: loggedInUser.username }).then(user => {
  //   if (user) {
  //     loggedInUser = user;
  //
  //   } else {
  //     console.log("error finding user")
  //   }
  // });

  res.render('edit_profile', {
    user: loggedInUser
  });
});

//profile page
router.get('/profile', ensureAuthenticated, (req,res) => {
  res.render('profile', {
    user: loggedInUser
  });
});

//follow user page
router.get('/follow_topic', ensureAuthenticated, (req,res) => {
  var topics;
  var yourTopics;
  User.findOne({ username: "topicHolder" }).then(user => {
    if (user) {
      topics = user.topicList;
      globalTopics = topics;
      User.findOne({ username: loggedInUser.username }).then(user => {
        if (user) {
          yourTopics = user.topicList;

          res.render('follow_topic', {
            topics: topics,
            yourTopics: yourTopics,
            loggedInUser: loggedInUser
          });
        } else {
          res.redirect('/home');
        }
      });
    } else {
      res.redirect('/home');
    }
  });
});

//follow user
router.post('/follow_topic', (req, res) => {
  console.log(req.body);

  User.findOne({ username: loggedInUser.username }).then(user => {
    if (user) {
      topics = user.topicList;
      if(!topics.includes(req.body.follow_username) && globalTopics.includes(req.body.follow_username)){
        topics.push(req.body.follow_username);
      }
      console.log("topics:", topics);

      User.updateOne(
        {username : loggedInUser.username},
        {$set: { topicList : topics}},
        function(err, raw) {
          if (err) {
            res.send(err);
          }
          res.redirect('/follow_topic');
        });

      //res.redirect('/follow_topic');
    } else {
      res.redirect('/follow_topic');
    }
  });
});

//unfollow topic
router.post('/unfollow_topic', (req, res) => {
  console.log(req.body);

  User.findOne({ username: loggedInUser.username }).then(user => {
    if (user) {
      topics = user.topicList;
      var index = topics.indexOf(req.body.unfollow_username);
      console.log(index);
      if(index > -1){
        topics.splice(index, 1);
      }
      console.log("topics:", topics);

      User.updateOne(
        {username : loggedInUser.username},
        {$set: { topicList : topics}},
        function(err, raw) {
          if (err) {
            res.send(err);
          }
          res.redirect('/follow_topic');
        });

      //res.redirect('/follow_topic');
    } else {
      res.redirect('/follow_topic');
    }
  });
});

//add topic
router.post('/add_topic', (req, res) => {
  console.log(req.body);

  User.findOne({ username: "topicHolder" }).then(user => {
    if (user) {
      topics = user.topicList;
      if(!topics.includes(req.body.add_topic)){
        topics.push(req.body.add_topic);
      }
      console.log("topics:", topics);

      User.updateOne(
        {username : "topicHolder"},
        {$set: { topicList : topics}},
        function(err, raw) {
          if (err) {
            res.send(err);
          }
          res.redirect('/follow_topic');
        });

      //res.redirect('/follow_topic');
    } else {
      res.redirect('/follow_topic');
    }
  });
});




//follow user page
router.get('/follow_user', ensureAuthenticated, (req,res) => {
  var usersList;
  User.find({"isDeleted": "0"}, function(err, users){
    if(err){
      console.log(err);
    } else {
        usersList = users;
        console.log('retrieved list of names', users);
    }
  });

  getFollowers().then(function(result){
    getFollowing().then(function(result2){
      console.log("result");
      console.log(result);
      console.log("result2");
      console.log(result2);
      currentFollowing = result2;
      console.log("loggedInUser");
      console.log(loggedInUser);

      res.render('follow_user', {
        followers: result,
        following: result2,
        users: usersList,
        loggedInUser: loggedInUser
      });
    });
  });


});

//follow user page ERROR
router.get('/follow_user_error', ensureAuthenticated, (req,res) => {
  res.render('follow_user_error', {
    loggedInUser: loggedInUser
  });
});

//follow user page SUCCESS
router.get('/follow_user_success', ensureAuthenticated, (req,res) => {
  res.render('follow_user_success', {
    loggedInUser: loggedInUser
  });
});

//follow user
router.post('/follow_user', (req, res) => {
  console.log(req.body);

  User.findOne({ username: req.body.follow_username }).then(user => {
    if (user) {
      userfound = 0;
      //let feed = client.feed('timeline', email);
      check_string = "Timeline:" + req.body.follow_username;
      console.log(check_string);
      console.log("currentFollowing");
      console.log(currentFollowing);
      for(var i = 0; i < currentFollowing.results.length; i++){
        console.log(currentFollowing.results[i].target_id);
        if(check_string === currentFollowing.results[i].target_id){
          userfound = 1;
          break;
        }
      }
      if(userfound == 0 && user.isDeleted == "0"){
        feed.follow('Timeline', req.body.follow_username);
        res.redirect('/follow_user_success');
      }
      else{
        // popup.alert({
        //     content: "You are already following this user"
        // });
        console.log("You are already following this user");
        res.redirect('/follow_user_error');
      }

    } else {
      res.redirect('/follow_user_error');
    }
  });

  //res.render('follow_user');
});

//unfollow user
router.post('/unfollow_user', (req, res) => {
  console.log(req.body);

  User.findOne({ username: req.body.unfollow_username }).then(user => {
    if (user) {
      userfound = 0;
      //let feed = client.feed('timeline', email);
      check_string = "Timeline:" + req.body.unfollow_username;
      console.log(check_string);
      console.log("currentFollowing");
      console.log(currentFollowing);
      for(var i = 0; i < currentFollowing.results.length; i++){
        console.log(currentFollowing.results[i].target_id);
        if(check_string === currentFollowing.results[i].target_id){
          userfound = 1;
          break;
        }
      }
      if(userfound == 1){
        feed.unfollow('Timeline', req.body.unfollow_username);
        res.redirect('/follow_user_success');
      }
      else{
        // popup.alert({
        //     content: "You are already following this user"
        // });
        console.log("You are not following this user");
        res.redirect('/follow_user_error');
      }

    } else {
      res.redirect('/follow_user_error');
    }
  });
});

//login action
router.post('/login', (req, res, next) => {

  //connect to user's timeline
  User.findOne({ email: req.body.email }).then(user => {
    if(user.isDeleted == "1")
    {
      res.redirect('/login');
    }
    else{
      console.log(user);
      loggedInUser = user;

      userToken = client.createUserToken(user.username);
      //console.log(userToken);
      feed = client.feed('Timeline', user.username);
      //currentUser = feed.token;
      currentUser = userToken;
      //console.log(currentUser)
    }

  });


  passport.authenticate('local', {
    successRedirect: '/home',
    failureRedirect: '/login',
    failureFlash: true
  })(req, res, next);


});

//signup action
router.post('/signup', (req, res) => {
  const username = req.body.username;
  const email = req.body.email;
  const password = req.body.password;
  const fullname = req.body.lastname;
  const bio = "";
  const backgroundColor1 = "#ffffff";
  const backgroundColor2 = "#000000";
  const topicList = [];
  const isDeleted = "0";

  let errors = [];

  console.log(req.body);

  if(!username || !email || !password) {
    errors.push({ message: 'Enter Required Fields' });
  }

  if(errors.length > 0) {
    res.render('signup', {
      errors,
      username,
      email,
      password,
      fullname
    });
  } else {
    User.findOne({ email: email }).then(user => {
      if(user) {
        errors.push({ msg: 'Email already exists' });
        res.render('signup', {
          errors,
          username,
          email,
          password,
          fullname
        })
      } else {
        const newUser = new User({
          username,
          email,
          password,
          fullname,
          bio,
          backgroundColor1,
          backgroundColor2,
          topicList,
          isDeleted
        });

        client.user(username).create({
            name: fullname,
            profileImage: 'https://www.cs.purdue.edu/people/images/faculty/turkstra.jpg',
        });

        bcrypt.genSalt(10, (err, salt) => {
          bcrypt.hash(newUser.password, salt, (err, hash) => {
            if (err) throw err;
            newUser.password = hash;
            newUser.save().then(user => {
              req.flash('success_msg', 'You are now registered and can log in');
              console.log("User added to database");
              res.redirect('/login');
            })
            .catch(err => console.log(err));
          });
        });
      }
    });
  }
});

router.post('/edit_profile/delete', (req, res) => {

  console.log("Deleting a user :(");
  console.log(req.user.username);
  client.user(req.user.username).delete();
  User.findOne({ username: req.user.username}, (err, oldUser) => {
    if(err) throw err;
    console.log("oldUser: ");
    console.log(oldUser);
    const email = oldUser.email;
    const username = oldUser.username;
    const password = oldUser.password;
    const fullname = oldUser.fullname;
    const bio = oldUser.bio;
    const backgroundColor1 = oldUser.backgroundColor1;
    const backgroundColor2 = oldUser.backgroundColor2;
    const isDeleted = "1";
    var newUser = new User({
      email,
      username,
      password,
      fullname,
      bio,
      backgroundColor1,
      backgroundColor2,
      isDeleted
    });

    newUser._id = oldUser._id;

    User.updateOne({ email: oldUser.email }, newUser, function(err, numEntries) {
      if (err) throw err;
      else {
        console.log("Successfully updated deleted user. New user:");
        console.log(newUser)
      }
    })
  });
  /*
  getFollowers().then(function(result) {
    console.log("Followers:");
    for(var i = 0; i < result.results.length; i++)
    {
      console.log(result.results[i].target_id);
      feed.unfollow()
    }

  });
  */

   getFollowing().then(function(result) {
      console.log("Following:");
      for(var i = 0; i < result.results.length; i++) 
      {
        console.log(result.results[i].target_id);
        var str = result.results[i].target_id;
        str = str.slice(9, str.length);
        console.log("User:");
        console.log(str);
        feed.unfollow('Timeline', str);
      }
    });
    
    req.logout();
    res.redirect('/');

});

router.post('/edit_profile', (req, res) => {

  console.log(req.pass);
  console.log(req.body.username);
  console.log(req.body.email);
  console.log(req.body.bio);
  console.log(req.body.fullname);
  console.log(req.body.colorSelect);

  const email = req.body.email;
  const username = req.body.username;
  const password = req.body.password;
  const confirmPassword = req.body.confirmpass;
  //const lastname = req.body.lastname;
  const fullname = req.body.fullname;
  const bio = req.body.bio;
  const colorSelect = req.body.colorSelect

  let errors = [];

  var color1;
  var color2;
  //var oldUser;
  if(colorSelect.localeCompare("light")==0) {
    console.log("light mode")
    //white: #ffffff
    color1 = "#ffffff";
    color2 = "#000000";
  }
  else if(colorSelect.localeCompare("dark")==0) {
    console.log("dark mode")
    //dark: #2e2c2c
    color1 = "#2e2c2c";
    color2 = "#ffffff";
  }
  else if (colorSelect.localeCompare("red")==0) {
    console.log("red mode")
    //red: #f07b73
    color1 = "#f07b73";
    color2 = "#000000";
  }
  else if (colorSelect.localeCompare("green")==0) {
    console.log("green mode")
    //green: #7fdb98
    color1 = "#7fdb98";
    color2 = "#000000";
  }
  else if (colorSelect.localeCompare("blue")==0) {
    console.log("blue mode")
    //blue: #50abcc
    color1 = "#50abcc";
    color2 = "#000000";
  }

  if(!email) {
    errors.push({message: 'Enter your email.'});
  }
  if(password != confirmPassword) {
    errors.push({message: 'Your passwords do not match.'});
  }

  if(errors.length > 0) {
    console.log("rendering errors");
    res.render('edit_profile', {
      errors,
      username,
      email,
      password,
      fullname,
      bio,
      user: loggedInUser
    });
  } else {
    User.findOne({ email : email }, function (err, oldUser) {
      if(err)
      {
        console.log(err);
      }
      else {
        console.log("Old user inside else loop:");
        console.log(oldUser);

        var newUser = new User({
          email,
          username,
          password,
          fullname,
          bio,
          backgroundColor1: color1,
          backgroundColor2: color2
        });
        //console.log("new user:")
        //console.log(newUser);
        let encrypt_password = true;

        if (email == '' || email == null) {
          newUser.email = oldUser.email;
        }
        if (password == '' || password == null) {
          console.log("Oh good god");
          newUser.password = oldUser.password;
          encrypt_password = false;
        }
        // if (fullname == '' || lastname == null) {
        //   newUser.lastname = oldUser.lastname;
        // }
        // if (firstname == '' || firstname == null) {
        //   newUser.firstname = oldUser.firstName;
        // }

        if (bio == '' || bio == null) {
          newUser.bio = oldUser.bio;
        }

        if(encrypt_password == true) {
          bcrypt.genSalt(10, (err, salt) => {
            bcrypt.hash(newUser.password, salt, (err, hash) => {
              if (err) throw err;
              newUser.password = hash;
              console.log(newUser);

              newUser._id = oldUser._id;

              User.updateOne({ email: oldUser.email }, newUser, function(err, numEntries, result) {
                if(err)
                {
                  console.log(err);
                }
                else {
                  loggedInUser = newUser;
                  //console.log(loggedInUser);

                  req.flash('success_msg', 'Profile has been updated');
                  console.log("User has updated successfully");
                  res.redirect('/home');
                }

              });
            });
          });
        }

        else {
        console.log("New user:");
        console.log(newUser);

        newUser._id = oldUser._id;

        User.updateOne({ email: oldUser.email }, newUser, function(err, numEntries, result) {
          if(err)
          {
            console.log(err);
          }
          else {
            loggedInUser = newUser;
            //console.log(loggedInUser);

            req.flash('success_msg', 'Profile has been updated');
            console.log("User has updated successfully");
            res.redirect('/home');
          }

        });
        }



      } //Else end

    }); //FindOne end

  } //Else end

}); //Method end

// router.get('/*', (req,res) => {
//   res.render('404');
// });

module.exports = router;
