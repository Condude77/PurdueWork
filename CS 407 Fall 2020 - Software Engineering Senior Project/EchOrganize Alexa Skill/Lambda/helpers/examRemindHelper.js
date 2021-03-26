var AWS = require("aws-sdk");
AWS.config.update({region: "us-east-1"});
const tableName = "exam-table";

var examRemindHelper = function () {};
var docClient = new AWS.DynamoDB.DocumentClient();

examRemindHelper.prototype.getExams = (userID) => {
    console.log('userID: ' + userID);
    return new Promise((resolve, reject) => {
        var params = {
            ExpressionAttributeValues: {
                ':user_id': userID,
            },
            KeyConditionExpression: 'userId = :user_id',
            ProjectionExpression: 'className, examDate, examTime',
            TableName: tableName
};
        docClient.query(params, (err, data) => {
            if (err) {
                console.error("Unable to read item. Error JSON:", JSON.stringify(err, null, 2));
                return reject(JSON.stringify(err, null, 2))
            }
            console.log("GetItem succeeded:", JSON.stringify(data, null, 2));
            resolve(data.Items)
        })

    });
}

module.exports = new examRemindHelper();