import pickle

#load files
with open('data/y_train.pickle', 'rb') as handle:
    Y_train = pickle.load(handle)
with open('data/y_valid.pickle', 'rb') as handle:
    Y_valid = pickle.load(handle)
with open('data/y_test.pickle', 'rb') as handle:
    Y_test = pickle.load(handle)

#get number of 1 in each
oneTrain = Y_train.count(1)
oneValid = Y_valid.count(1)
oneTest = Y_test.count(1)

#get number of 0 in each
zeroTrain = Y_train.count(0)
zeroValid = Y_valid.count(0)
zeroTest = Y_test.count(0)

#total number in each
totalTrain = oneTrain + zeroTrain
totalValid = oneValid + zeroValid
totalTest = oneTest + zeroTest

#percentage calculation in each
percentTrain = (oneTrain / totalTrain) * 100
percentValid = (oneValid / totalValid) * 100
percentTest = (oneTest / totalTest) * 100

#print results 
print("Buggy Rate Training: ", oneTrain, "/", totalTrain, " Percent: ", percentTrain)
print("Buggy Rate Validation: ", oneValid, "/", totalValid, " Percent: ", percentValid)
print("Buggy Rate Testing: ", oneTest, "/", totalTest, " Percent: ", percentTest)
