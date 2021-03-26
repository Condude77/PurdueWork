const mongoose = require('mongoose');

const UserSchema = new mongoose.Schema ({
  username: String,
  email: String,
  password: String,
  fullname: String,
  //firstname: String,
  bio: String,
  backgroundColor1: String,
  backgroundColor2: String,
  isDeleted: String,
  topicList: Array
});

module.exports = mongoose.model('Users', UserSchema);
