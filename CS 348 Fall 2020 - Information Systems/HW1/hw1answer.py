import sys

def query1():
	return "SELECT R.RecName, R.RecId, COUNT(StudentId) as NumOfInternships FROM Recruiter R, Internship I WHERE R.RecId = I.RecId GROUP BY R.RecName, R.RecId HAVING COUNT(StudentId)> 1;"

def query2():
	return "SELECT DISTINCT CompName FROM Company Cm Left Join Job J ON Cm.CompId = J.CompId Where NOT EXISTS (select * from Job J2 WHERE J2.CompId = J.CompId and J2.OfferYear =2020);"

def query3():
	return "Select StudentName, EXTRACT(YEAR FROM BirthDate) As BirthYear , (2020-EXTRACT(YEAR FROM BirthDate)) as Age FROM Student WHERE (2020-EXTRACT(YEAR FROM BirthDate)) = (Select MAX(2020-EXTRACT(YEAR FROM BirthDate)) as MaxAge FROM Student);"
	
def query4():
	return "SELECT COUNT(JobId) as NumberOfJobs FROM Job;"

def query5():
	return "select Distinct C.CompName From  Company C join Job J on C.compid = J.compid join JobApplication JA on J.jobid = JA.jobid where JA.studentid in (select studentid from Student where (schoolid, grade) in (SELECT sc.schoolid, max(grade) FROM Student st join School sc on st.schoolid = sc.schoolid group by sc.schoolid));"

def query6():
	return "SELECT S1.StudentName, Sch1.SchoolName, Grade FROM Student S1, School Sch1, (SELECT SchoolName, MAX(S2.Grade) as MaxGrade FROM Student S2, School Sch2 WHERE S2.SchoolId = Sch2.SchoolId GROUP BY SchoolName) AS temp WHERE S1.SchoolId = Sch1.SchoolId  AND Sch1.SchoolName = temp.SchoolName AND S1.Grade = MaxGrade ORDER BY MaxGrade DESC;"

def query7():
	return "SELECT S.StudentName, J.Salary FROM JobApplication JA, Job J, Student S WHERE J.JobId = JA.JobId AND JA.StudentId = S.StudentId AND J.Salary BETWEEN '120000' AND '150000' AND J.OfferYear BETWEEN '2019' AND '2020';"

def query8():
	return "SELECT CompName, JobTitle, Salary FROM Company C, Job J WHERE C.CompId = J.CompId AND (C.CompId,Salary) IN (SELECT CompId, MIN(Salary) FROM Job GROUP BY CompId);"

def query9():
	return "SELECT CompName, StudentName, SchoolName FROM Student S, Internship I, School Sch, Company C WHERE S.StudentId = I.StudentId AND S.SchoolId = Sch.SchoolId AND I.compid = C.compid AND OfferYear = '2019' ORDER BY CompName ASC;"

def query10():
	return "SELECT SchoolName, COUNT(StudentId) as NumOfStudent, AVG(Grade) as AverageGrade FROM Student S, School Sch WHERE S.SchoolId = Sch.SchoolId GROUP BY SchoolName HAVING COUNT(StudentId)> 1 AND  AVG(Grade) > 60.0 ORDER BY AverageGrade ASC;"

def query11():
	return "SELECT StudentName, Grade, RecId FROM Student S LEFT OUTER JOIN Internship I ON S.StudentId = I.StudentId ORDER BY StudentName ASC;"

def query12():
	return "SELECT StudentName, SchoolName FROM Student S, School Sch, JobApplication JA WHERE S.StudentId = JA.StudentId AND S.SchoolId = Sch.SchoolId AND JA.ApplicationDate >= '2018';"

def query13():
	return "SELECT CompName, Count(JobId) As NumJobs FROM Company, Job WHERE Company.CompId = Job.CompId GROUP BY CompName;"

def query14():
	return "Select StudentName, EXTRACT(YEAR FROM BirthDate) As BirthYear  FROM Student WHERE EXTRACT(YEAR FROM BirthDate) IN ('1992', '1993');"

def query15():
	return "SELECT CompName FROM Company WHERE CompId = ANY (SELECT CompId FROM Job WHERE Salary < '120000');"

def query16():
	return "SELECT SUM(Salary) FROM Job WHERE JobTitle = 'job1' OR JobTitle = 'job2';"
	
def main():
	query_options = {1: query1(), 2: query2(), 3: query3(), 4: query4(), 5: query5(), 6: query6(), 7: query7(), 8: query8(), 
		9: query9(), 10: query10(), 11: query11(), 12: query12(), 13: query13(), 14: query14(), 15: query15(), 16: query16()}
	print(query_options[int(sys.argv[1])])
	
if __name__ == "__main__":
   main()
