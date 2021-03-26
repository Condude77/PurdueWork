const mongoose = require('mongoose');

const TopicListSchema = new mongoose.Schema ({
  topics: [],
  activity_id: String,
});

module.exports = mongoose.model('Users', TopicListSchema);