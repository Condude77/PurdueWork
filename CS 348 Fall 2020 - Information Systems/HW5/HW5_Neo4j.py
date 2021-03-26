import sys


#TODO: Write your username and answer to each query as a string in the return statements 
# in the functions below. Do not change the function names. 

# Write your queries using multi-line strings and use proper indentation to increase readability

#Your result should have the attributes in the same order as appeared in the sample answers. 

#Make sure to test that python prints out the strings correctly.

#usage: python hw5.py

def username():
	return "brow1325"
    
def query5():
    return """
            MATCH (auth:Author)-[:WRITES]->(art:Article)
            RETURN auth.name, art.title;
           """ 

def query6():
    return """
            MATCH (auth:Author)-[:WRITES]->(article_or_chapter)
            RETURN auth.name, article_or_chapter.title, labels(article_or_chapter) AS publicationType;
           """ 

def query7():
    return """
            MATCH (auth:Author)-[:WRITES|:EDITS]->(publication) 
            RETURN auth.name, publication.title, labels(publication) AS publicationType;
           """ 


def query8():
    return """
            MATCH (auth:Author)-[:WRITES|:EDITS]->(publication)
            RETURN auth.name, count(publication) AS publication_count;
           """ 


def query9():
    return """
            MATCH (a:Article)-[i:IN]->(Issue)
            WITH a, (i.pp[1] - i.pp[0] + 1) AS numberOfPages
            WHERE numberOfPages < 11
            RETURN a.title, numberOfPages;
           """ 


def query10():
    return """
            MATCH (a1:Author)-[:WRITES]->(p1:Article)-[c:CITES]->(p2:Article), (a1)-[:WRITES]->(p2)
            RETURN a1.name, p1.title, p2.title;
           """ 


def query11():
    return """
            MATCH ()-[c:CITES]->(publication)
            WITH publication, COUNT(c) AS publication_count
            WHERE publication_count > 1
            RETURN publication.title, publication_count;
           """ 


def query12():
    return """
            MATCH (auth:Author)-[:WRITES]->(art:Article)-[:IN]->(:Issue)-[:OF]->(j:Journal), ()-[c:CITES]->(art)
            WITH art, j, auth, COUNT(c) AS citation_count
            WHERE citation_count > 1
            RETURN j.title, art.title, citation_count, auth.name;
           """ 


def query13():
    return """
            MATCH (a2:Article)<-[:CITES]-(a1:Article)-[:IN]->(i1:Issue)-[:OF]->(:Journal)<-[:OF]-(i2:Issue)<-[:IN]-(a2:Article)
            WHERE NOT i1 = i2
            RETURN a1.title, i1.issue, a2.title, i2.issue;
           """ 


#Do not edit below

def main():
	query_options = {1: query5(), 6: query6(), 7: query7(), 8: query8(), 
		9: query9(), 10: query10(), 11: query11(), 12: query12(), 13: query13()}
	
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
