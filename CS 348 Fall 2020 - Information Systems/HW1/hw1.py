import sys

#TODO: Write your username and answer to each query as a string in the return statements in the functions below. Do not change the function names. If you have a partner, write your partner's username in the partnerUsername function. Each student in the team needs to submit a copy of the homework on Brightspace.

#Your resulting tables should have the attributes in the same order as appeared in the sample answers. 

#Make sure to test that python prints out the strings correctly.

#usage: python hw1.py

def username():
	return "brow1325\n"
    
def partnerUsername():
	return "" 
    #remember that each student in the team needs to submit a copy of the homework on Brightspace.
    #Return an empty string if you do not have a partner for this homework.

def query1():
	return """Select RecName, Recruiter.RecId, count(Internship.RecId) as NumOfInternships
From Recruiter Join Internship
Where Recruiter.RecId = Internship.RecId
Group by RecName
Having NumOfInternships >= 2;\n"""

def query2():
	return """Select Distinct CompName
From Company
Where Company.CompId NOT IN (Select CompId
                         From Job
                         Where Job.OfferYear = 2020)
Group By CompName;\n"""

def query3():
	return """Select StudentName, YEAR(BirthDate) AS BirthYear, 2020 - Year(BirthDate) AS Age
From Student
Where 2020 - Year(BirthDate) = (Select Max(2020-Year(BirthDate))
             From Student)
Group By StudentName;\n"""
	
def query4():
	return """Select count(distinct JobId) AS NumberOfJobs
From Job;\n"""

def query5():
	return """Select C1.CompName
From Company C1 Join Job J1 ON C1.CompId = J1.CompId Left Join JobApplication A1 ON J1.JobId = A1.JobId Join (Select T1.StudentId, S1.SchoolName, T1.Grade AS MaxGrade
                                                                                                              From Student T1 Join School S1
                                                                                                              Group by T1.StudentId, S1.SchoolName
                                                                                                              Having (S1.SchoolName, T1.Grade) IN (Select S2.SchoolName, Max(T2.Grade)
                                                                                                                                                   From Student T2 Join School S2
                                                                                                                                                   Where T2.SchoolId = S2.SchoolId 
                                                                                                                                                   Group by T2.SchoolId)) BrightStudents
Where A1.StudentId = BrightStudents.StudentId
Group by C1.CompName;\n"""

def query6():
	return """Select T1.StudentName, S1.SchoolName, T1.Grade AS MaxGrade
From Student T1 Join School S1
Group by T1.StudentName, S1.SchoolName
Having (S1.SchoolName, T1.Grade) IN (Select S2.SchoolName, Max(T2.Grade)
                                     From Student T2 Join School S2
                                     Where T2.SchoolId = S2.SchoolId 
                                     Group by T2.SchoolId)
Order By Grade DESC;\n"""

def query7():
	return """Select StudentName, Salary
From Student Join Job Join JobApplication
Where Student.StudentId = JobApplication.StudentId AND Job.JobId = JobApplication.JobId AND Salary >= 120000 AND Salary <= 150000 AND (OfferYear = 2019 OR OfferYear = 2020);\n"""

def query8():
	return """Select C1.CompName, J1.JobTitle, J1.Salary
From Company C1 Join Job J1
Having (C1.CompName, J1.Salary) IN (Select C2.CompName, min(J2.Salary)
                              From Company C2 Join Job J2
                              Where C2.CompId = J2.CompId
                              Group by C2.CompName);\n"""

def query9():
	return """Select C1.CompName, S1.StudentName, Z1.SchoolName
From Company C1 Join Student S1 Join School Z1 Join Internship I1
Where C1.CompId = I1.CompId AND S1.StudentId = I1.StudentId AND S1.SchoolId = Z1.SchoolId AND OfferYear = 2019;\n"""

def query10():
	return """Select Z1.SchoolName, count(*) as NumOfStudent, avg(S1.grade) as AverageGrade
From Student S1 Join School Z1
Where S1.SchoolId = Z1.SchoolId
Group by Z1.SchoolName
Having count(*)>1 AND avg(S1.grade) > 60
Order by AverageGrade Asc;\n"""

def query11():
	return """Select S1.StudentName, S1.Grade, R2.RecId
From Student S1 Left Join Internship I2 ON S1.StudentId = I2.StudentId Left Join Recruiter R2 ON R2.RecId = I2.RecId;\n"""

def query12():
	return """Select S1.StudentName, Z1.SchoolName
From Student S1 Join School Z1 Join JobApplication J1
Where Year(J1.ApplicationDate) >= 2018 And J1.StudentId = S1.StudentId AND S1.SchoolId = Z1.SchoolId;\n"""

def query13():
	return """Select C1.CompName, count(distinct J1.JobId) as NumJobs
From Company C1 Join Job J1
Where C1.CompId = J1.CompId
Group by C1.CompName;\n"""

def query14():
	return """Select StudentName, Year(BirthDate) AS BirthYear
From Student
Where Year(BirthDate) = 1992 OR Year(BirthDate) = 1993;\n"""

def query15():
	return """Select CompName
From Job Join Company
Where Job.CompId = Company.CompId AND Salary < 120000
Group by CompName;\n"""

def query16():
	return """Select SUM(Salary)
From Job
Where JobTitle = 'Job1' OR JobTitle = 'Job2';"""

#Do not edit below

def main():
	query_options = {1: query1(), 2: query2(), 3: query3(), 4: query4(), 5: query5(), 6: query6(), 7: query7(), 8: query8(), 
		9: query9(), 10: query10(), 11: query11(), 12: query12(), 13: query13(), 14: query14(), 15: query15(), 16: query16()}
	
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
