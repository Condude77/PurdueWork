/*--Company */
insert into Company(CompId, CompName, Address) values (1, 'Amazon', '410 Terry Ave, N Seattle');
insert into Company(CompId, CompName, Address) values (2, 'Microsoft', 'Redmond, WA');
insert into Company(CompId, CompName, Address) values (3, 'Facebook', 'Menlo Park, CA');
insert into Company(CompId, CompName, Address) values (4, 'UC Berkely', 'Berkeley, CA');
insert into Company(CompId, CompName, Address) values (5, 'Spotify', 'World Trade Center, New York');
/*--Recruiter */
insert into Recruiter(RecId, RecName) values (1, 'Rec1');
insert into Recruiter(RecId, RecName) values (2, 'Rec2');
insert into Recruiter(RecId, RecName) values (3, 'Rec3');
insert into Recruiter(RecId, RecName) values (4, 'Rec4');
insert into Recruiter(RecId, RecName) values (5, 'Rec5');
/*--School */
insert into School(SchoolId, SchoolName, Address) values (1, 'Purdue', 'West Lafayette, Indiana');
insert into School(SchoolId, SchoolName, Address) values (2, 'Indiana University', 'Bloomington, IN');
insert into School(SchoolId, SchoolName, Address) values (3, 'Ball State University', 'Muncie, Indiana');
insert into School(SchoolId, SchoolName, Address) values (4, 'University of Illinois', 'Chicago');
insert into School(SchoolId, SchoolName, Address) values (5, 'Indiana University - Purdue University', 'Indianapolis, Indiana');
/*--Student */
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (1, 'student1', 1, str_to_date('1990 January 01','%Y %M %D'),90.90);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (2, 'student2', 1, str_to_date('1992 January 01','%Y %M %D'),80.80);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (3, 'student3', 2, str_to_date('1992 February 02','%Y %M %D'),70.70);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (4, 'student4', 3, str_to_date('1993 March 03','%Y %M %D'),60.60);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (5, 'student5', 3, str_to_date('1994 April 04','%Y %M %D'),50.50);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (6, 'student6', 1, str_to_date('1995 May 05','%Y %M %D'),75.75);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (7, 'student7', 4, str_to_date('1996 June 06','%Y %M %D'),40.40);
insert into Student(StudentId, StudentName, SchoolId, BirthDate, Grade) values (8, 'student8', 4, str_to_date('1989 July 06','%Y %M %D'),92.00);
/*--Job */
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (1, 1, 11,'job1',150000,2019);
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (2, 1, 22,'job2',130000,2020);
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (3, 1, 22,'job3',110000,2020);
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (4, 2, 12,'job4',100000,2017);
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (5, 3, 22,'job5',120000,2018);
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (6, 3, 22,'job6',160000,2019);
insert into Job(JobId, CompId, JobNum, JobTitle, Salary, OfferYear) values (7, 2, 12,'job7',090000,2019);
/*--Internship */
insert into  Internship(StudentId, CompId, RecId, OfferYear) values (1, 1, 1,2018);
insert into  Internship(StudentId, CompId, RecId, OfferYear) values (1, 1, 1,2019);
insert into  Internship(StudentId, CompId, RecId, OfferYear) values (2, 2, 2,2018);
insert into  Internship(StudentId, CompId, RecId, OfferYear) values (3, 3, 3,2019);
/*--JobApplication */
insert into  JobApplication(JobId, StudentId, ApplicationDate) values (1,1,str_to_date('2019 January 01','%Y %M %D'));
insert into  JobApplication(JobId, StudentId, ApplicationDate) values (2,2,str_to_date('2017 January 12','%Y %M %D'));
insert into  JobApplication(JobId, StudentId, ApplicationDate) values (3,3,str_to_date('2017 January 11','%Y %M %D'));
insert into  JobApplication(JobId, StudentId, ApplicationDate) values (1,3,str_to_date('2018 June 01','%Y %M %D'));
insert into  JobApplication(JobId, StudentId, ApplicationDate) values (2,4,str_to_date('2018 February 01','%Y %M %D'));
