import sys


#TODO: Write your username and answer to each query as a string in the return statements 
# in the functions below. Do not change the function names. 

# Write your queries using multi-line strings and use proper indentation to increase readability.
# For Q6 & Q7 write your map/reduce code as multi-line string. For example:
# 
# def query6():
#     return """
# 
#             var mapFunction1 = function() {
#                ...
#             }
# 
#             var reduceFunction1 = function(a, b) {
#                return ...
#             }
# 
#             db.countries.mapReduce(mapFunction1, reduceFunction1, {
#                ...
#             })
#           """

# Your result should have the attributes in the same order as appeared in the sample answers. 

# Make sure to test that python prints out the strings correctly.

# usage: python hw6.py

def username():
	return "brow1325"
    
def query1():
    return """
db.countries.aggregate([ 
{$match:{$or:[{area:{"$lt":10}},{area:{"$gt":10000000}}]}}, 
{$project:{_id:0, area:1, name:"$name.common"}}
])
		   """ 

def query2():
    return """
db.countries.aggregate([ 
{$match:{"translations.ita.official": "Repubblica di Malta"}}, 
{$project:{_id:0, capital:1}}
])
           """
            
def query3():
    return """
db.countries.aggregate([ 
{$match:{$and:[{"latlng.0": {"$gte":40}}, {"latlng.0": {"$lte":45}}, {"latlng.1": {"$gte":10}}, {"latlng.1": {"$lte":20}}]}}, 
{$project:{_id:0, capital:1, name:"$name.common"}}
])
           """ 

def query4():
    return """
db.countries.aggregate([
{$project: {_id:1, name:"$name.common", numberOfCurrencies: {$size: "$currency"}}},
{$sort: {numberOfCurrencies:-1, name:1}},
{$limit:5},
{$project: {_id:0, name:1, numberOfCurrencies:1}}
])
           """

def query5():
    return """
db.countries.aggregate([
{$group: {_id:"$subregion", totalArea:{$sum:"$area"}}},
{$sort: {totalArea:-1}}
])
           """ 

def query6():
    return """
db.countries.mapReduce(
function() {
    emit(this.subregion, this.area);
},
function(key, values) {
    return Array.sum(values)
},
{out:{inline:1}}
)
           """

def query7():
    return """
db.countries.mapReduce(
function() {
    var splitName = this.name.official.split(" ");
    for( var idx = 0; idx < splitName.length; idx++) {
        var key = splitName[idx];
        var value = {count: 1};
        emit(key, value);
    }
},
function(key, values) {
    totalVal = {count:0};
    for(var idx = 0; idx < values.length; idx++){
        totalVal.count += values[idx].count;
    }
return totalVal;
},
{out:{inline:1}, query:{"subregion":"Southern Asia"}}
)
           """ 

def query9():
    return """
db.countries.aggregate([
{$match: {"name.common":"United States"}},
{$project: {_id:0, name:"$name.common", borders:1}},
{$lookup:{from:"countries", localField: "borders", foreignField: "cca3", as: "neighborCountryname"}},
{$unwind:"$neighborCountryname"},
{$project: {_id:0, name:1, neighborCountryname:"$neighborCountryname.name.common"}}
])
           """


#Do not edit below

def main():
	query_options = {1: query1(), 2: query2(), 3: query3(), 4: query4(), 
		5: query5(), 6: query6(), 7: query7(), 9: query9()}
	
	if len(sys.argv) == 1:
		if username() == "username":
			print("Make sure to change the username function to return your username.")
			return
		else:
			print(username())
		for query in query_options.values():
			print(query)
	elif len(sys.argv) == 2:
		if sys.argv[1] == "username":
			print(username())
		else:
			print(query_options[int(sys.argv[1])])

	
if __name__ == "__main__":
   main()
