SET FOREIGN_KEY_CHECKS = 0;

INSERT INTO Organization VALUES (1, "Purdue University", 1);
INSERT INTO Organization VALUES (2, "College of Science", 2);
INSERT INTO Organization VALUES (3, "College of Engineering", 3);
INSERT INTO Organization VALUES (4, "Dept of Computer Science", 4);
INSERT INTO Organization VALUES (5, "Dept of Statistics", 5);

INSERT INTO SubOrg VALUES (1, 2);
INSERT INTO SubOrg VALUES (1, 3);
INSERT INTO SubOrg VALUES (2, 4);
INSERT INTO SubOrg VALUES (2, 5);

INSERT INTO `Employee` (`EmpId`, `Name`, `Job`, `Salary`, `OrgId`) VALUES
(1, 'John', 'University President', 170000, 1),
(2, 'Smith', 'College Dean', 150000, 2),
(3, 'Alice', 'College Dean', 150000, 3),
(4, 'Fei', 'Department head', 130000, 4),
(5, 'Bob', 'Department head', 130000, 5),
(6, 'Mary', 'Advisor', 75000, 4),
(7, 'Sara', 'Professor', 125000, 4),
(8, 'Suresh', 'Professor', 120000, 5),
(9, 'Ali', 'Professor', 120000, 5);

INSERT INTO Students VALUES (01001,'Jack',STR_TO_DATE('19980101','%Y%m%d'));
INSERT INTO Students VALUES (02002,'A.Smith',STR_TO_DATE('19950101','%Y%m%d'));
INSERT INTO Students VALUES (12002,'Banks',STR_TO_DATE('19970101','%Y%m%d'));

INSERT INTO Courses VALUES (1,'ENG40000','Advanced Poetry Writing');
INSERT INTO Courses VALUES (2,'ENG32000', 'Advanced Writing In The Arts And Sciences');
INSERT INTO Courses VALUES (3,'COM10000', 'Intro to Communications');
INSERT INTO Courses VALUES (4,'ME30800', 'Fluid Mechanics');

INSERT INTO Grades VALUES (01001,1, 90);
INSERT INTO Grades VALUES (01001,2, 93);
INSERT INTO Grades VALUES (02002,1, 77);
INSERT INTO Grades VALUES (02002,4, 100);
INSERT INTO Grades VALUES (12002,2, 88);
INSERT INTO Grades VALUES (12002,3, 81);
INSERT INTO Grades VALUES (12002,4, 93);

INSERT INTO Scholarship VALUES(01001, 2016);


SET FOREIGN_KEY_CHECKS = 1;
