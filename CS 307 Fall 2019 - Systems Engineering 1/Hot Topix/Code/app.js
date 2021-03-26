var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const mongoose = require('mongoose');
const passport = require('passport');
const flash = require('connect-flash');
const session = require('express-session');
var bodyParser = require('body-parser')


//const { ensureAuthenticated, forwardAuthenticated } = require('./config/auth');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();

//passport config
require('./config/passport')(passport);

//db config
const db = require('./config/keys').mongoURI;

//connect to db
mongoose.connect(db, { useNewUrlParser: true }).then(() => console.log('MongoDB Connected')).catch(err => console.log(err));

//body parser
app.use(express.urlencoded({ extended:true }));
app.use(bodyParser.urlencoded({ extended: true }));


// view engine setup
app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');

//express session
app.use(session({
  secret: 'secret key',
  resave: false,
  saveUninitialized: false
}));

//passport middleware
app.use(passport.initialize());
app.use(passport.session());

//connect flash
app.use(flash());

//global variables
app.use(function(req, res, next) {
  res.locals.success_msg = req.flash('success_msg');
  res.locals.error_msg = req.flash('error_msg');
  res.locals.error = req.flash('error');
  next();
});

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

//routes
app.use('/', indexRouter);
app.use('/users', usersRouter);

//if (app.get('env') === 'development') {
if (false) {
  app.use(express.static(path.join(__dirname, 'client')));
} else {
  app.use(express.static(path.join(__dirname, 'build')));
}


// app.get('/home', ensureAuthenticated, (req, res) => {
//   //if (app.get('env') === 'development') {
//   if (false) {
//     res.sendFile(path.join(__dirname, 'client', 'public/index.html'));
//   } else {
//     res.sendFile(path.join(__dirname, 'build', 'index.html'));
//   }
// });


module.exports = app;
