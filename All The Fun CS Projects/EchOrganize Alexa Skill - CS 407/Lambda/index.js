/* eslint-disable  func-names */
/* eslint-disable  no-console */
var moment = require('moment-timezone');
const examRemindHelper = require('./helpers/examRemindHelper');
const Alexa = require('ask-sdk');
const todolistHelper = require('./helpers/todolistHelper');
const calendarHelper = require('./helpers/calendarHelper');
const examHelper = require('./helpers/examHelper');
const officeHoursHelper = require('./helpers/officeHoursHelper');
const GENERAL_REPROMPT = "What would you like to do?";
const dynamoDBTableName = "todolist-table";
const dynamoDBCalTableName = "calendar-table";
const dynamoDBExamTableName = "exam-table";

let populateSchedule = false;
let scheduleStart = false;
let scheduleEnd = false;
let addToCal = false;
let conflictExists = false;
let functionalityExists = false;
let noScheduleCreate = false;
let examStart = false;
let examDate = false;
let examTime = false;
let examInProgress = false;
let createExam = false;
let officeHoursTime = false;

const LaunchRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    handle(handlerInput) {
        const speakOutput = 'What would you like to do?';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt()
            .getResponse();

    }
};

/*  Sprint 2 DOCS */

const calendars = [];

const calendarInstance = {
    "isActive": "",
    "name": "",
    "calendarName": "",
    "classes": [],
    "startDate": "",
    "endDate": ""
};

let activeCalendar = calendarInstance;

const classItem = {
    "className": "",
    "classTime": "",
    "DepartureTime": "",
    "reccurence": {
        "startDateTime": "",
        "endDateTime": "",
        "recurrenceRules": []
    }
    
};
const CreateScheduleIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'CreateScheduleIntent';
    },
    
    async handle(handlerInput) {
        scheduleStart = true;
        return handlerInput.responseBuilder
            .speak("What would you like to call your new schedule?")
            .reprompt()
            .getResponse();
    }
};

const ScheduleNameIntentHandler = {
     canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'ScheduleNameIntent';
    },
    
    async handle(handlerInput) {
        let season = handlerInput.requestEnvelope.request.intent.slots.Season.value;
        console.log('SEASON: ' + season);
        let term = handlerInput.requestEnvelope.request.intent.slots.Term.value;
        console.log('TERM: ' + term);
        let year = handlerInput.requestEnvelope.request.intent.slots.Year.value;
        console.log('YEAR: ' + year);
        let name = season + ' ' + term + ' ' + year;
        if (scheduleStart) {
            calendarInstance.name = name;
            return handlerInput.responseBuilder
                .speak("What is the start date for your new schedule?")
                .reprompt()
                .getResponse();
        }
        else {
            calendars.forEach(function(item)
            {
                if (item.name === name) {

                    return handlerInput.responseBuilder
                        .speak("I found a saved + " + name + " schedule that starts on " + item.startDate + " and ends on " + item.endDate + ". Is that what you are looking for?")
                        .reprompt()
                        .getResponse();
                }
            }); 
        }
    }
};

/*
async function f() {

  let promise = new Promise((resolve, reject) => {
    setTimeout(() => resolve("done!"), 1000)
  });

  let result = await promise; // wait until the promise resolves (*)

  alert(result); // "done!"
}

f();
*/

const TimeIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'DateIntent';
    },
    
    async handle(handlerInput) {
        if (examDate && !scheduleStart && !scheduleEnd) {
          examDate = false;
          const date = new Date(handlerInput.requestEnvelope.request.intent.slots.date.value);
          const userId = handlerInput.requestEnvelope.context.System.user.userId;
          /*NEW CODE SPRINT 3 */
          let promise = new Promise((resolve,reject)=> {
            calendarHelper.getSchedules(userId)
              .then((data)=> {
                if (data.length > 0) {
                   const schedule = data.map(e => {
                     let startDate = new Date(e.startDate);
                     let endDate = new Date(e.endDate);
                     if (startDate.getTime() <= date.getTime() && endDate.getTime() >= date.getTime()) {
                       console.log("DATE CHECK: " + startDate.getTime() + ", " + endDate.getTime() + ", " + date.getTime());
                      setTimeout(() => resolve(e),1000);
                      }
                   });
                  setTimeout(() => resolve("NOCAL"),1000);
                }
                else {
                  setTimeout(() => resolve("NOCAL"), 1000);
                }
              })
              .catch((err)=> {
                setTimeout(() => reject("err"), 1000);
              });
          });
          let calMatch = await promise;
          if (calMatch === "NOCAL") {
            noScheduleCreate = true;
            functionalityExists = true;
            examInProgress = true;
            return handlerInput.responseBuilder
              .speak('It doesnt look like youve set up a calendar yet for that date. Would you like to create one?')
              .reprompt()
              .getResponse();
          }
          else {
            examInstance.date = date.toISOString();
            examTime = true;
            return handlerInput.responseBuilder
            .speak(`Ok, and what time is your exam?`)
            .reprompt()
            .getResponse();
          }
          /*NEW CODE SPRINT 3 */
          
          
        }
        else if (examTime && !scheduleStart && !scheduleEnd) {
          const time = handlerInput.requestEnvelope.request.intent.slots.time.value;
          const userId = handlerInput.requestEnvelope.context.System.user.userId;
          examInstance.time = time;
          examTime = false;
          return examHelper.saveNewExam(
            examInstance.name,
            examInstance.date,
            examInstance.time,
            userId
            )
            .then((data) => {
                    const startDateFormatted = new Date(examInstance.date).toDateString();
                    return handlerInput.responseBuilder
                      .speak('Ok, I have saved a new exam for you in ' + examInstance.name + ' on ' + startDateFormatted +' at ' + examInstance.time)
                      .getResponse();
                    })
                    .catch((err) => {
                      console.log("Error occured while saving new examr", err);
                      return handlerInput.responseBuilder
                        .speak("Sorry, I wasn't able to save your exam. Please try again.")
                        .getResponse();
                      })
              }
        
        else if (scheduleStart && !scheduleEnd && !examDate && !examTime) {
        const startDate = new Date(handlerInput.requestEnvelope.request.intent.slots.date.value);
        calendarInstance.startDate = startDate.toISOString();
        scheduleStart = false;
        scheduleEnd = true;
        return handlerInput.responseBuilder
            .speak(`Ok, and when does this schedule end?`)
            .reprompt()
            .getResponse();
        }
        else if (scheduleEnd && !scheduleStart && !examDate && !examTime) {
            const userId = handlerInput.requestEnvelope.context.System.user.userId;
            const endDate = new Date(handlerInput.requestEnvelope.request.intent.slots.date.value);
            calendarInstance.endDate = endDate.toISOString();
            
            const attributesManager = handlerInput;
            scheduleEnd = false;
            addToCal = true;
            
            let promise = new Promise((resolve, reject) => {
              calendarHelper.getSchedules(userId)
              .then((data) => {
                if (data.length > 0) {
                  let scheduleName = "a";
                  const scheduleMap = data.map(e => {
                  let foundStartDate = new Date(e.startDate);
                  console.log("found start date:" + foundStartDate.toDateString());
                  let foundEndDate = new Date(e.endDate);
                  console.log("found end date:" + foundEndDate.toDateString());
                  let startCheck = new Date(calendarInstance.startDate);
                  let endCheck = new Date(calendarInstance.endDate);
                  if (foundStartDate.getTime() <= startCheck.getTime() && foundEndDate >= startCheck.getTime()) {
                    console.log("new start date in range");
                    console.log("name: " + e.calendarName);
                    setTimeout(() => resolve(e.calendarName), 1000)
                  }
                  else if (foundStartDate.getTime() <= endCheck.getTime() && foundEndDate >= endCheck.getTime()) {
                    console.log("new end date in range");
                    console.log("name: " + e.calendarName);
                    setTimeout(() => resolve(e.calendarName), 1000)
                  }
                  else {
                    console.log("empty data!");
                    setTimeout(() => resolve("1"), 1000)
                  }
                  });
                  setTimeout(() => resolve("1"), 1000)
                }
                else {
                  setTimeout(() => resolve("1"), 1000)
                }
            });
              
            });
            let foundCalendar = await promise;
            console.log("found schedule: " + foundCalendar);
             
            if (foundCalendar !== "1") {
              return handlerInput.responseBuilder
                .speak('Your ' + foundCalendar + ' schedule overlaps with your new ' + calendarInstance.calendarName + ' schedule. This may result in event reminders disappearing from your calendar. Are you sure you want to continue?')
                .reprompt()
                .getResponse();
              }
              else {
                console.log("I'm going to return the wrong value, but first here is the foundSchedule  and conflict value: " + foundCalendar);
                return calendarHelper.saveNewSchedule(
                  calendarInstance.name,
                  calendarInstance.startDate,
                  calendarInstance.endDate,
                  userId
                  ).then((data) => {
                    const startDateFormatted = new Date(calendarInstance.startDate).toDateString();
                    const endDateFormatted = new Date(calendarInstance.endDate).toDateString();
                    if (!examInProgress) {
                      createExam = true;
                      functionalityExists = true;
                      return handlerInput.responseBuilder
                      .speak('Ok, I have created a new ' + calendarInstance.name + ' calendar for you that runs from ' + startDateFormatted + ' to ' + endDateFormatted + '. Would you like to add some exams?')
                      .reprompt()
                      .getResponse();
                    }
                    else {
                      functionalityExists = true;
                      return handlerInput.responseBuilder
                        .speak('Ok, I have created a new ' + calendarInstance.name + ' calendar for you that runs from ' + startDateFormatted + ' to ' + endDateFormatted + '. Would you like to continue with saving the data for your ' + examInstance.name + ' exam?')
                        .reprompt()
                        .getResponse();
                    }
                    
                    })
                    .catch((err) => {
                      console.log("Error occured while saving new calendar", err);
                      return handlerInput.responseBuilder
                        .speak("Sorry, I wasn't able to save your new calendar. Please try again.")
                        .getResponse();
                      })
              }
        }
        else if (officeHoursTime) {
          const time = handlerInput.requestEnvelope.request.intent.slots.time.value;
          const userId = handlerInput.requestEnvelope.context.System.user.userId;
          const today = new Date().toISOString();
          officeHoursInstance.time = time;
          return officeHoursHelper.saveNewOfficeHours(
            officeHoursInstance.className,
            today,
            officeHoursInstance.dayOfWeek,
            time,
            userId
            ).then((data) => {
              return handlerInput.responseBuilder
                .speak('Ok, I have saved your office hours in ' + officeHoursInstance.className + '.')
                .getResponse();
            })
        }
        else {
             const requestAttributes = handlerInput.attributesManager.getRequestAttributes();
             const speakOutput = requestAttributes.t('ERROR');
             const repromptOutput = requestAttributes.t('ERROR_REPROMPT');

            return handlerInput.responseBuilder
                .speak(speakOutput)
                .reprompt(repromptOutput)
                .getResponse();
        }
    }
};

const GetScheduleIntentHandler = {
    canHandle(handlerInput) {
        return handlerInput.requestEnvelope.request.type === 'IntentRequest'
            && handlerInput.requestEnvelope.request.intent.name === 'GetScheduleIntent';
    },
    async handle(handlerInput) {
        const { responseBuilder } = handlerInput;
        const userID = handlerInput.requestEnvelope.context.System.user.userId;
        return calendarHelper.getSchedules(userID)
            .then((data) => {
                var speechText = '';
                if (data.length === 0) {
                  speechText += 'I dont see any currently active schedules. Would you like to create one?';
                  functionalityExists = true;
                  noScheduleCreate = true;
                  return responseBuilder
                      .speak(speechText)
                      .reprompt()
                      .getResponse();
                }
                else {
                  let now = new Date();
                  const foundSchedule = data.map(e => {
                    let startDate = new Date(e.startDate);
                    let endDate = new Date(e.endDate);
                    if (startDate.getTime() <= now.getTime() && endDate.getTime() >= now.getTime()) {
                      return e.calendarName;
                    }
                  });
                  return calendarHelper.getClassesBySchedule(userID, foundSchedule)
                    .then((classList) => {
                      speechText += 'Here is your ' + foundSchedule + ' schedule. ';
                      if (classList.length == 0) {
                        speechText += 'It doesnt look like you have any classes saved in your schedule. Would you like to add some?'
                      }
                      else {
                        speechText += 'You are currently enrolled in: ';
                        speechText += data.map(e => e.classes).join(', ');
                      }
                      return responseBuilder
                      .speak(speechText)
                      .reprompt()
                      .getResponse();
                    })
                }
            })
            .catch((err) => {
                const speechText = "Sorry, I'm having trouble finding your schedule right now. Please try again later."
                return responseBuilder
                    .speak(speechText)
                    .getResponse();
            })
    }
}

const YesIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.YesIntent';
  },
  async handle(handlerInput) {
      if (!functionalityExists) {
        return handlerInput.responseBuilder
          .speak('Hmm, Id like to help, but youll have to wait until the next sprint to be able to do that.')
          .getResponse();
      }
      else {
        if (noScheduleCreate) {
          noScheduleCreate = false;
          scheduleStart = true;
          functionalityExists = false;
          return handlerInput.responseBuilder
            .speak("What would you like to call your new schedule?")
            .reprompt()
            .getResponse();
          
        }
        else if (examInProgress) {
          examInProgress = false;
          examDate = true;
          functionalityExists = false;
          return handlerInput.responseBuilder
            .speak("What is the date of your exam?")
            .reprompt()
            .getResponse();
        }
        else if (createExam) {
          createExam = true;
          functionalityExists = false;
          return handlerInput.responseBuilder
            .speak("In what class would you like to save an exam?")
            .reprompt()
            .getResponse();
        }
      }
    
  }
};

const NoIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.NoIntent';
  },
  async handle(handlerInput) {
    if (!functionalityExists) {
        return handlerInput.responseBuilder
          .speak('Hmm, Id like to help, but youll have to wait until the next sprint to be able to do that.')
          .getResponse();
      }
    else {
      if (noScheduleCreate) {
        functionalityExists = false;
        noScheduleCreate = false;
        return handlerInput.responseBuilder
          .speak('Ok, Im here if you want to create one later.')
          .getResponse();
      }
      else if (examInProgress) {
        functionalityExists = false;
        examInProgress = false;
        return handlerInput.responseBuilder
          .speak('Ok. Your new schedule will remain saved if you would like to add your exam later.')
          .getResponse();
      }
      else if (createExam) {
        functionalityExists = false;
        createExam = false;
        return handlerInput.responseBuilder
          .speak('Ok, Im here if you want to create one later.')
          .getResponse();
      }
    }
  },
};

const ClassReminderIntentHandler = {
  canHandle(handlerInput) {
      return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'ClassReminderIntent';
  },
  handle(handlerInput) {
    var inTime = handlerInput.requestEnvelope.request.intent.slots.inputTime.value;
    var inclassName = handlerInput.requestEnvelope.request.intent.slots.className.value;
    var inClassNum = handlerInput.requestEnvelope.request.intent.slots.classNumber.value;
    //var inDay = handlerInput.requestEnvelope.request.intent.slots.day.value;
    var day1 = handlerInput.requestEnvelope.request.intent.slots.dayOne.value;
    var day2 = handlerInput.requestEnvelope.request.intent.slots.dayTwo.value;
    var day3 = handlerInput.requestEnvelope.request.intent.slots.dayThree.value;
    
    //console.log(inClassNum);
    const formatTime = moment().tz("America/New_York").format("YYYY-MM-DDTHH:mm:ss").toLocaleString().split("T")[0]+"T"+inTime+":00";
    //console.log(formatTime);
    //const className = 
    const reminderApiClient = handlerInput.serviceClientFactory.getReminderManagementServiceClient(),
    {permissions} = handlerInput.requestEnvelope.context.System.user;

    if(!permissions){
      return handlerInput.responseBuilder
          .speak("Please go to the Alexa mobile app to grant reminders permissions")
          .withAskForPermissionsConsentCard(['alexa::alerts:reminders:skill:readwrite'])
          .getResponse();
    }
   
    var repDays = new Array();
    var i;
   
    //recurring
    if(day1 !== undefined){
        
            repDays.push(day1.toUpperCase().substring(0,2));
            if(day2 !== undefined){
                repDays.push(day2.toUpperCase().substring(0,2));
            }
            if(day3 !== undefined){
                repDays.push(day3.toUpperCase().substring(0,2));
            }
            
        for(i=0; i<repDays.length;i++){
            const currentTime = moment().tz("America/New_York"), // Use Moment Timezone to get the current time in Eastern Time
              reminderRequest = {
                requestTime: currentTime.format("YYYY-MM-DDTHH:mm:ss"), // Add requestTime
                trigger: {
                  type: "SCHEDULED_ABSOLUTE", // Update from SCHEDULED_RELATIVE
                  scheduledTime: formatTime,//currentTime.add(20, "seconds").format("YYYY-MM-DDTHH:mm:ss"),
                  timeZoneId: "America/New_York", // Set timeZoneId to Eastern Time
                  recurrence: {                     
                    freq: 'WEEKLY',
                    byDay: [repDays[i]],
                  }
                },
                alertInfo: {
                  spokenInfo: {
                    content: [{
                      locale: "en-US",
                      text: "Time for "+inclassName +" "+inClassNum+" ",
                    }]
                  }
                },
                pushNotification: {
                  status: "ENABLED"
                }
              };
              reminderApiClient.createReminder(reminderRequest);
        }
        
    }
    //one time
    else{
     // console.log("here instead?");
      const currentTime = moment().tz("America/New_York"), // Use Moment Timezone to get the current time in Eastern Time
      reminderRequest = {
        requestTime: currentTime.format("YYYY-MM-DDTHH:mm:ss"), // Add requestTime
        trigger: {
          type: "SCHEDULED_ABSOLUTE", // Update from SCHEDULED_RELATIVE
          scheduledTime: formatTime,//currentTime.add(20, "seconds").format("YYYY-MM-DDTHH:mm:ss"),
          timeZoneId: "America/New_York", // Set timeZoneId to Eastern Time
        },
        alertInfo: {
          spokenInfo: {
            content: [{
              locale: "en-US",
              text: "Time for "+inclassName +" "+inClassNum+" ",
            }]
          }
        },
        pushNotification: {
          status: "ENABLED"
        }
      };
      reminderApiClient.createReminder(reminderRequest);
    }
    
    //var inputTime = handlerInput.requestEnvelope.slots.time.value;
    //reminderApiClient.createReminder(reminderRequest);
    //console.log(inTime);
    //console.log(inTime.type)
    var label;
    var compTime = inTime.split(":")[0];
    var compMin = inTime.split(":")[1];
    if(compTime > 12){
        var formattedTime = compTime-12 + ":"+compMin;
        label = "pm";
    }
    else if(compTime === 12){
        label = "pm";
        formattedTime = compTime+":"+compMin;
    }
    else{
        label = "am";
        formattedTime = inTime;
    }
    var speakOutput = "You scheduled a reminder for "+inclassName+" "+inClassNum+" at "+formattedTime+" "+label;

   // var speakOutput = "You have got the intent code working from lambda";
    return handlerInput.responseBuilder
      .speak(speakOutput)
      .getResponse();
  },
};

const RemindExamsIntentHandler = {
  canHandle(handlerInput) {
    return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
      && Alexa.getIntentName(handlerInput.requestEnvelope) === 'RemindExamsIntent';
  },
  
  async handle(handlerInput) {
    const reminderApiClient = handlerInput.serviceClientFactory.getReminderManagementServiceClient(),
    {permissions} = handlerInput.requestEnvelope.context.System.user;

    if(!permissions){
      return handlerInput.responseBuilder
          .speak("Please go to the Alexa mobile app to grant reminders permissions")
          .withAskForPermissionsConsentCard(['alexa::alerts:reminders:skill:readwrite'])
          .getResponse();
    }
    let UserId = handlerInput.requestEnvelope.context.System.user.userId;
    return examRemindHelper.getExams(UserId).then((data) => {
      if (data.length === 0) {
        return handlerInput.responseBuilder
          .speak('You have no upcoming exams')
          .getResponse();
      }
      else {
        var inClass = handlerInput.requestEnvelope.request.intent.slots.className.value;
        var inAmount = handlerInput.requestEnvelope.request.intent.slots.quantity.value;
        var speechText = '';
        var examDays = new Array();
        var examTimes = new Array();
        var examClass = new Array();
        data.map(e => {
          if(inClass === undefined){
            examDays.push(e.examDate);
            examTimes.push(e.examTime);
            examClass.push(e.className);
          }
          else if(inClass === e.className){
            examDays.push(e.examDate);
            examTimes.push(e.examTime);
            examClass.push(e.className);
          }
        });
        if(inAmount != undefined){
          console.log("here1234");
          if(inAmount.toUpperCase() === "ALL"){
            if(examDays.length === 0){
              speechText += 'You have no exams in '+inClass;
            }
            else{
              speechText += 'You have '+examDays.length +" exams in "+inClass+". I have set a reminder for all of them.";
              var i;
              for(i=0; i<examDays.length;i++){
                var formatTime = examDays[i].split("T")[0]+"T"+examTimes[i]+":00";
                const currentTime = moment().tz("America/New_York"), // Use Moment Timezone to get the current time in Eastern Time
                reminderRequest = {
                  requestTime: currentTime.format("YYYY-MM-DDTHH:mm:ss"), // Add requestTime
                  trigger: {
                    type: "SCHEDULED_ABSOLUTE", // Update from SCHEDULED_RELATIVE
                    scheduledTime: formatTime,
                    timeZoneId: "America/New_York", // Set timeZoneId to Eastern Time
                  },
                  alertInfo: {
                    spokenInfo: {
                      content: [{
                        locale: "en-US",
                        text: "Time for " + inClass+ " exam ",
                      }]
                    }
                  },
                  pushNotification: {
                    status: "ENABLED"
                  }
                };
                reminderApiClient.createReminder(reminderRequest);
              }
            }
          }
          else{
          //soonest exam
          var soonestExam = -1;
          var soonestY = 0;
          var soonestM = 0;
          var soonestD = 0;
          var soonestH = 0;
          var soonestm = 0;
          for(i=0; i<examDays.length;i++){
            var dsplit = examDays[i].split("T")[0].split('-');
            var tsplit = examDays[i].split("T")[1].split(':');
            var j;
            if(soonestY === 0 ){
              soonestY = dsplit[0];
              soonestM = dsplit[1];
              soonestD = dsplit[2];
              soonestH = tsplit[0];
              soonestm = tsplit[1];
              soonestExam = i;
            }
            else{
              if(dsplit[0] < soonestY && dsplit[1] < soonestM && dsplit[2] < soonestD){
                soonestExam = i;
              }
              else if(dsplit[0] === soonestY && dsplit[1] === soonestM && dsplit[2] === soonestD){
                if(tsplit[0] < soonestH && tsplit[1] < soonestm){
                  soonestExam = i;
                }
              }
            }
          }
          if(soonestExam === -1 && inClass != undefined){
            speechText += "You don't have any upcoming exams in "+inClass;
          }
          else{
            var label;
            var compTime = examTimes[soonestExam].split(":")[0];
            var compMin = examTimes[soonestExam].split(":")[1];
            if(compTime > 12){
                var formattedTime = compTime-12 + ":"+compMin;
                label = "pm";
            }
            else if(compTime === 12){
                label = "pm";
                formattedTime = compTime+":"+compMin;
            }
            else{
                label = "am";
                formattedTime = examTimes[soonestExam];
            }
            var formattedDate = examDays[soonestExam].split('T')[0];
            if(inClass != undefined){
              speechText += "Your soonest "+inClass+" exam is "+formattedDate +" at "+formattedTime+" "+label+". I have set a reminder for you";
            }
            else{
              speechText += "Your soonest exam in all your classes is on "+ formattedDate +" at "+formattedTime+" "+label+". I have set a reminder for you";
            }
            formatTime = examDays[soonestExam].split("T")[0]+"T"+examTimes[soonestExam]+":00";
            const currentTime = moment().tz("America/New_York"), // Use Moment Timezone to get the current time in Eastern Time
            reminderRequest = {
              requestTime: currentTime.format("YYYY-MM-DDTHH:mm:ss"), // Add requestTime
              trigger: {
                type: "SCHEDULED_ABSOLUTE", // Update from SCHEDULED_RELATIVE
                scheduledTime: formatTime,
                timeZoneId: "America/New_York", // Set timeZoneId to Eastern Time
              },
              alertInfo: {
                spokenInfo: {
                  content: [{
                    locale: "en-US",
                    text: "Time for " + examClass[soonestExam]+ " exam ",
                  }]
                }
              },
              pushNotification: {
                status: "ENABLED"
              }
            };
            reminderApiClient.createReminder(reminderRequest);
          }
        }
        }
        else{
          speechText+="You have "+examDays.length+" exams. I have set a reminder for all of them";
          for(i=0;i<examDays.length;i++){
            formatTime = examDays[i].split("T")[0]+"T"+examTimes[i]+":00";
            const currentTime = moment().tz("America/New_York"), // Use Moment Timezone to get the current time in Eastern Time
            reminderRequest = {
              requestTime: currentTime.format("YYYY-MM-DDTHH:mm:ss"), // Add requestTime
              trigger: {
                type: "SCHEDULED_ABSOLUTE", // Update from SCHEDULED_RELATIVE
                scheduledTime: formatTime,
                timeZoneId: "America/New_York", // Set timeZoneId to Eastern Time
              },
              alertInfo: {
                spokenInfo: {
                  content: [{
                    locale: "en-US",
                    text: "Time for " + examClass[i]+ " exam ",
                  }]
                }
              },
              pushNotification: {
                status: "ENABLED"
              }
            };
            reminderApiClient.createReminder(reminderRequest);
          }
        }
        return handlerInput.responseBuilder
          .speak(speechText)
          .getResponse();
      }
    });
  }
};

const examInstance = {
    "isActive": "",
    "name": "",
    "date": "",
    "time": "",
};

const officeHoursInstance = {
  "className": "",
  "dayOfWeek": {},
  "time": ""
};

const SaveOfficeHoursIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'SaveOfficeHoursIntent';
  },
  async handle(handlerInput) {
    let className = handlerInput.requestEnvelope.request.intent.slots.Class.value;
    officeHoursInstance.className = className;
    return handlerInput.responseBuilder
      .speak("What day or days of the week are your office hours in " + className + "?")
      .reprompt()
      .getResponse();
  }
};

const DaysOfWeekIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'DaysOfWeekIntent';
  },
  async handle(handlerInput) {
    let day1 = handlerInput.requestEnvelope.request.intent.slots.day_A.value;
    let day2 = handlerInput.requestEnvelope.request.intent.slots.day_B.value;
    let day3 = handlerInput.requestEnvelope.request.intent.slots.day_C.value;
    if (day2 !== null) {
      if (day3 !== null) {
        officeHoursInstance.dayOfWeek = [day1, day2, day3];
      }
      else {
        officeHoursInstance.dayOfWeek = [day1, day2];
      }
    }
    else {
      officeHoursInstance.dayOfWeek = day1;
    }
    officeHoursTime = true;
    return handlerInput.responseBuilder
      .speak("What time are your office hours in " + officeHoursInstance.className + "?")
      .reprompt()
      .getResponse();
  }
};

const GetOfficeHoursIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'GetOfficeHoursIntent';
  },
  async handle(handlerInput) {
     let UserId = handlerInput.requestEnvelope.context.System.user.userId;
     let className = handlerInput.requestEnvelope.request.intent.slots.Class.value;
     return officeHoursHelper.getOfficeHours(UserId, className).then((data) => {
       if (data.length === 0) {
         return handlerInput.responseBuilder
          .speak('It doesnt look like youve saved your office hours yet. You might want to do that to stay on top of your classes.')
          .getResponse();
       }
       else {
         var dayOrder = ['Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday'];
         var dayPluralOrder = ['Sundays','Mondays','Tuesdays','Wednesdays','Thursdays','Fridays','Saturdays'];
         var weekOrder = [];
         var days = [];
         data.map(e => {
           for (let i = 0; i < e.dayOfWeek.length; i++) {
             let day = e.dayOfWeek[i].toUpperCase();
             days.push(day);
           }
           console.log("Day of Week: " + days);
         });
         
         let start = new Date().getDay();
         for (let i = 1; i < 8; i++) {
           var d = new Date();
           d.setDate(d.getDate() + i);
           weekOrder.push(d);
           for (let j = 0; j < days.length; j++) {
             if (dayOrder[d.getDay()].toUpperCase() === days[j] || dayPluralOrder[d.getDay()].toUpperCase() === days[j]) {
               return handlerInput.responseBuilder
                .speak("Your next office hours in " + data[0].className + " are on " + d.toDateString() + " at " + data[0].officeHoursTime)
                .getResponse();
             }
           }
         }
         console.error("Error finding next date: " + data + ", " + weekOrder);
         return handlerInput.responseBuilder
          .speak("Ive found an unexpected error looking for your next office hours date. Please check the logs.")
          .getResponse();
       }
     });
  }
}

const NewExamIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'NewExamIntent';
  },
  async handle(handlerInput) {
        examStart = true;
        return handlerInput.responseBuilder
            .speak("What class is your exam in?")
            .reprompt()
            .getResponse();
    }
};

const ClassNameIntentHandler = {
     canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'ClassNameIntent';
    },
    
    async handle(handlerInput) {
        let name = handlerInput.requestEnvelope.request.intent.slots.ClassName.value;
        console.log('Class: ' + name);
        if (examStart) {
            examInstance.name = name;
            console.log('examInstance.name: ' + name);
            examDate = true;
            return handlerInput.responseBuilder
                .speak("What day is your exam?")
                .reprompt()
                .getResponse();
        }
    }
};

const GetExamsIntentHandler = {
  canHandle(handlerInput) {
    return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
      && Alexa.getIntentName(handlerInput.requestEnvelope) === 'GetExamsIntent';
  },
  async handle(handlerInput) {
    let UserId = handlerInput.requestEnvelope.context.System.user.userId;
    return examHelper.getExams(UserId).then((data) => {
      if (data.length === 0) {
        createExam = true;
        return handlerInput.responseBuilder
          .speak('It doesnt look like you have any saved exams. Would you like to add one?')
          .reprompt
          .getResponse();
      }
      else {
        var speechText = 'Here are all your exams.';
        let sortedExams = data.sort((a,b) => new Date(a.examDate).getTime() - new Date(b.examDate).getTime());
        for (let i = 0; i < sortedExams.length; i++) {
          let formattedDay = new Date(sortedExams[i].examDate).toDateString();
          speechText += ' In ' + sortedExams[i].className + ' you have an exam on ' + formattedDay  + ' at ' + sortedExams[i].examTime + '.';
        }
        return handlerInput.responseBuilder
          .speak(speechText)
          .getResponse();
      }
    })
    .catch((err) => {
                const speechText = "Sorry, I'm having trouble finding your exams right now. Please try again later."
                return handlerInput.responseBuilder
                    .speak(speechText)
                    .getResponse();
            })
  }
};

const NextExamIntentHandler = {
  canHandle(handlerInput) {
    return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
      && Alexa.getIntentName(handlerInput.requestEnvelope) === 'NextExamIntent';
  },
  async handle(handlerInput) {
    let UserId = handlerInput.requestEnvelope.context.System.user.userId;
    return examHelper.getExams(UserId).then((data) => {
      if (data.length === 0) {
        createExam = true;
        return handlerInput.responseBuilder
          .speak('It doesnt look like you have any saved exams. Would you like to add one?')
          .reprompt
          .getResponse();
      }
      else {
        let sortedExams = data.sort((a,b) => new Date(a.examDate).getTime() - new Date(b.examDate).getTime());
        let i = 0;
        for (i = 0; i < sortedExams.length; i++) {
          if (new Date(sortedExams[i].examDate).getTime() >= new Date().getTime()) {
            let formattedDate = new Date(sortedExams[i].examDate).toDateString();
            return handlerInput.responseBuilder
              .speak('Your next exam is in ' + sortedExams[i].className + ' on ' + formattedDate + ' at ' + sortedExams[i].examTime + '.')
              .getResponse();
          }
          
        }
        return handlerInput.responseBuilder
          .speak('It doesnt look like you have any saved exams. Would you like to add one?')
          .reprompt
          .getResponse();
        
      }
    });
  }
};

const nextExamStruct = {
  className: "",
  date: "",
  time: ""
};

const getExamsByClassIntentHandler = {
  canHandle(handlerInput) {
    return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
      && Alexa.getIntentName(handlerInput.requestEnvelope) === 'getExamsByClass';
  },
  
  async handle(handlerInput) {
    let UserId = handlerInput.requestEnvelope.context.System.user.userId;
    let className = handlerInput.requestEnvelope.request.intent.slots.Class.value;
    let today = new Date(Date.now());
    return examHelper.getExamsByClass(UserId, className).then((data) => {
      if (data.length === 0) {
        return handlerInput.responseBuilder
          .speak('It doesnt look like you have any upcoming exams in ' + className + '. Take some time for yourself.')
          .getResponse();
      }
      else if (data.length === 1) {
        var speechText;
        
        data.map(e => {
          let examDate = new Date(e.examDate);
          let dates = [today,examDate];
          if (examDate.getTime() > today.getTime()) {
            let formattedDay = new Date(e.examDate).toDateString();
            speechText = 'Your next exam in ' + className + ' is on ' + formattedDay + ' at ' + e.examTime;
          }
          else {
            speechText = 'It doesnt look like you have any upcoming exams in ' + className + '. Take some time for yourself.';
          }
        });
        return handlerInput.responseBuilder
          .speak(speechText)
          .getResponse();
      }
      else {
        var speechText;
        var dateArray = [];
        let sortedExams = data.sort((a,b) => new Date(a.examDate).getTime() - new Date(b.examDate).getTime());
        let index = 0;
        for (var i = 0; index < sortedExams.length; index++) {
          let checkExamDate = new Date(sortedExams[i].examDate);
          console.log('Checking ' + checkExamDate);
          if (checkExamDate.getTime() > today.getTime()) {
            index = i;
            break;
          }
        }
        let formattedDay = new Date(sortedExams[index].examDate).toDateString();
        speechText = 'Your next exam in ' + className + ' is on ' + formattedDay + ' at ' + sortedExams[index].examTime;
        return handlerInput.responseBuilder
          .speak(speechText)
          .getResponse();
      }
    });
  }
}

/* ADIL'S PART            */
/*  Sprint 3 DOCS */


const InProgressGradeCalculatorIntentHandler = {
    canHandle(handlerInput) {
      const request = handlerInput.requestEnvelope.request;
      return request.type === 'IntentRequest' &&
        request.intent.name === 'GradeCalculatorIntent' &&
        request.dialogState !== 'COMPLETED';
    },
    handle(handlerInput) {
      const currentIntent = handlerInput.requestEnvelope.request.intent;
      return handlerInput.responseBuilder
        .addDelegateDirective(currentIntent)
        .getResponse();
    }
}

const GradeCalculatorIntentHandler = {
    canHandle(handlerInput) {
        return handlerInput.requestEnvelope.request.type === 'IntentRequest'
            && handlerInput.requestEnvelope.request.intent.name === 'GradeCalculatorIntent';  
    },
    handle(handlerInput) {
        const earnedPoints = handlerInput.requestEnvelope.request.intent.slots.earnedPoints.value;
        const totalPoints = handlerInput.requestEnvelope.request.intent.slots.totalPoints.value;
        var percent = (earnedPoints * 100) / totalPoints;
        percent = percent.toFixed(1);
        var speakOutput = "error occured, try again"
        
        switch(true) {
            case percent >= 97:
                speakOutput = " it's an A plus! Great job!";
                break;
            case percent >= 93:
                speakOutput = " it's a strong A! Awesome!";
                break;
            case percent >= 90:
                speakOutput = " it's A minus, almost perfect.";
                break;
            case percent >= 87:
                speakOutput = " it's B plus, which is good";
                break;
            case percent >= 83:
                speakOutput = " it's B";
                break;
            case percent >= 80:
                speakOutput = " it's B minus";
                break;
            case percent >= 77:
                speakOutput = " it's C plus";
                break;
            case percent >= 73:
                speakOutput = " it's a strong C, you are passing";
                break;
            case percent >= 70:
                speakOutput = " it's C minus";
                break;
            default:
                speakOutput = " it's below C minus. This might end up bad...";
                break;
        }
        
        
        return handlerInput.responseBuilder
            .speak(`It is ${percent} percent. By default ` + speakOutput)
            .reprompt(GENERAL_REPROMPT)
            .getResponse();

    }
    
};
/*  Sprint 3 DOCS */

/*  Sprint 2*/
/*  Adding To Do List   */
const InProgressAddToDoListIntentHandler = {
    canHandle(handlerInput) {
        const request = handlerInput.requestEnvelope.request;
        return request.type === 'IntentRequest' &&
          request.intent.name === 'AddToDoListIntent' &&
          request.dialogState !== 'COMPLETED';
      },
      handle(handlerInput) {
        const currentIntent = handlerInput.requestEnvelope.request.intent;
        return handlerInput.responseBuilder
          .addDelegateDirective(currentIntent)
          .getResponse();
      }
}
const AddToDoListIntentHandler = {
    canHandle(handlerInput) {
        return handlerInput.requestEnvelope.request.type === 'IntentRequest'
            && handlerInput.requestEnvelope.request.intent.name === 'AddToDoListIntent';  
    },
    async handle(handlerInput) {
        const userID = handlerInput.requestEnvelope.context.System.user.userId; 
        const todoString = handlerInput.requestEnvelope.request.intent.slots.todoString.value;
        /*working with the date*/
        const todoData = handlerInput.requestEnvelope.request.intent.slots.todoData.value.toString();
        console.log(todoData);
        /*working with the date*/
        return todolistHelper.addToDoList(todoString, todoData, userID)
            .then((data) => {
                const speechText = `You have added ${todoString}  objective for ${todoData}. Say add to add another objective, or remove to remove objectives from your to do list.`;
                return handlerInput.responseBuilder
                    .speak(speechText)
                    .reprompt("to do list intent handler waiting for response")
                    .getResponse();
            })
            .catch((err) => {
                console.log("Error occured while saving to do list", err);
                return handlerInput.responseBuilder
                    .speak("we cannot save your to do list right now. Try again!")
                    .getResponse();
            })
    }
};
/*  Adding To Do List   */

/*  Getting To Do List   */
const GetToDoListsIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'GetToDoListsIntent';
  },
  async handle(handlerInput) {
    const {responseBuilder } = handlerInput;
    const userID = handlerInput.requestEnvelope.context.System.user.userId;
    const todoData = handlerInput.requestEnvelope.request.intent.slots.todoData.value.toString();
    return todolistHelper.getToDoLists(userID, todoData)
      .then((data) => {
        var speechText = "Your objectives are "
        if (data.length == 0) {
          speechText = "You do not have any objectives for this date. Say add to add an objective "
        } else {
          speechText += data.map(e => e.todoString).join(", ")
        }
        return responseBuilder
          .speak(speechText)
          .reprompt(GENERAL_REPROMPT)
          .getResponse();
      })
      .catch((err) => {
        const speechText = "we cannot get your to do list right now. Try again!"
        return responseBuilder
          .speak(speechText)
          .getResponse();
      })
  }
}
/*  Getting To Do List   */

/*  Removing objectives from To Do List   */
const InProgressRemoveToDoListIntentHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest' &&
      request.intent.name === 'RemoveToDoListIntent' &&
      request.dialogState !== 'COMPLETED';
  },
  handle(handlerInput) {
    const currentIntent = handlerInput.requestEnvelope.request.intent;
    return handlerInput.responseBuilder
      .addDelegateDirective(currentIntent)
      .getResponse();
  }
}

const RemoveToDoListIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'RemoveToDoListIntent';
  }, 
  handle(handlerInput) {
    const {responseBuilder } = handlerInput;
    const userID = handlerInput.requestEnvelope.context.System.user.userId; 
    const slots = handlerInput.requestEnvelope.request.intent.slots;
    const todoString = slots.todoString.value;
    return todolistHelper.removeToDoList(todoString, userID)
      .then((data) => {
        const speechText = `You have removed objective: ${todoString}, you can remove more, by saying remove or complete`
        return responseBuilder
          .speak(speechText)
          .reprompt(GENERAL_REPROMPT)
          .getResponse();
      })
      .catch((err) => {
        const speechText = `You do not have objective: ${todoString}, you can add it by saying add`
        return responseBuilder
          .speak(speechText)
          .reprompt(GENERAL_REPROMPT)
          .getResponse();
      })
  }
}
/*  Removing objectives from To Do List   */

/*  Sprint 2 DOCS */
/* ADIL'S PART            */

const HelpIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.HelpIntent';
  },
  handle(handlerInput) {
    const speechText = 'You can introduce yourself by telling me your name';

    return handlerInput.responseBuilder
      .speak(speechText)
      .reprompt(speechText)
      .getResponse();
  },
};

const CancelAndStopIntentHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'IntentRequest'
      && (handlerInput.requestEnvelope.request.intent.name === 'AMAZON.CancelIntent'
        || handlerInput.requestEnvelope.request.intent.name === 'AMAZON.StopIntent');
  },
  handle(handlerInput) {
    const speechText = 'Goodbye!';

    return handlerInput.responseBuilder
      .speak(speechText)
      .getResponse();
  },
};

const SessionEndedRequestHandler = {
  canHandle(handlerInput) {
    return handlerInput.requestEnvelope.request.type === 'SessionEndedRequest';
  },
  handle(handlerInput) {
    console.log(`Session ended with reason: ${handlerInput.requestEnvelope.request.reason}`);

    return handlerInput.responseBuilder.getResponse();
  },
};

const ErrorHandler = {
  canHandle() {
    return true;
  },
  handle(handlerInput, error) {
    console.log(`Error handled: ${error.message}`);

    return handlerInput.responseBuilder
      .speak('Sorry, I can\'t understand the command. Please say again.')
      .reprompt('Sorry, I can\'t understand the command. Please say again.')
      .getResponse();
  },
};

const skillBuilder = Alexa.SkillBuilders.standard();

exports.handler = skillBuilder
  .addRequestHandlers(
    LaunchRequestHandler,
    ClassReminderIntentHandler,
    RemindExamsIntentHandler,
    InProgressAddToDoListIntentHandler,
    AddToDoListIntentHandler,
    GetToDoListsIntentHandler,
    InProgressRemoveToDoListIntentHandler,
    RemoveToDoListIntentHandler,
    GradeCalculatorIntentHandler,
    InProgressGradeCalculatorIntentHandler,
    HelpIntentHandler,
    CancelAndStopIntentHandler,
    SessionEndedRequestHandler,
    CreateScheduleIntentHandler,
    ScheduleNameIntentHandler,
    TimeIntentHandler,
    GetScheduleIntentHandler,
    YesIntentHandler,
    NoIntentHandler,
    NewExamIntentHandler,
    ClassNameIntentHandler,
    GetExamsIntentHandler,
    getExamsByClassIntentHandler,
    NextExamIntentHandler,
    SaveOfficeHoursIntentHandler,
    DaysOfWeekIntentHandler,
    GetOfficeHoursIntentHandler
  )
  .addErrorHandlers(ErrorHandler)
  .withTableName(dynamoDBTableName)
  .withAutoCreateTable(true)
  //.withApiClient(new Alexa.DefaultApiClient())
  .lambda();
