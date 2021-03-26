DELIMITER $$
DROP PROCEDURE IF EXISTS college_report;

CREATE PROCEDURE college_report(IN college_name TEXT, OUT full_report TEXT)
BEGIN
	/* Declare variables needed for College Info */
	DECLARE d1 INT DEFAULT 0;
	DECLARE retLine1 TEXT;
	DECLARE collHead VARCHAR(30);
	DECLARE collTotalSal INT;

	/* Declare variable needed for Department Info */
	DECLARE currDepId INT;
	DECLARE depHead VARCHAR(30);
	DECLARE depTotalSal INT;
	DECLARE depName VARCHAR(30);
	DECLARE depList CURSOR FOR
		SELECT SubOrgId
		FROM SubOrg JOIN Organization ON SubOrg.OrgId = Organization.OrgId
		WHERE OrgName = college_name;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET d1 = 1;

	/* Get College Head */
	SELECT Name
    INTO collHead
	FROM Employee JOIN Organization
	WHERE EmpId = HeadId AND OrgName = college_name;

	/* Get Total Salary for College */
	SELECT Sum(Salary)
    INTO collTotalSal
	FROM Employee JOIN Organization ON Employee.OrgId = Organization.OrgId
	WHERE Organization.OrgName = college_name OR Employee.OrgId IN (Select SubOrgId
																	From SubOrg JOIN Organization ON SubOrg.OrgId = Organization.OrgId
																	Where Organization.OrgName = college_name);
	/* Combine college info */
	SET retLine1 = CONCAT("<college>",CHAR(10),CHAR(9),"<info>", college_name, ", ", collHead, ", ", collTotalSal, "</info>",CHAR(10),CHAR(9),"<Departments>",CHAR(10));
	/* Declare second continue handler for sub orgs */

	OPEN depList;
	getDeps: LOOP
		FETCH depList INTO currDepId;
		IF d1 = 1 THEN 
			LEAVE getDeps;
		END IF;
			
		/* Get Department Name */
		SELECT OrgName
		INTO depName
		FROM Organization
		WHERE OrgId = currDepId;
		
		/* Get Department Head */
		SELECT Name
		INTO depHead
		FROM Employee JOIN Organization
		WHERE EmpId = HeadId AND Employee.OrgId = currDepId;
			
		/* Get Department Total Salary */
		Select Sum(Salary)
		INTO depTotalSal
		FROM Employee JOIN Organization ON Employee.OrgId = Organization.OrgId
		WHERE Organization.OrgId = currDepId OR Employee.OrgId IN (Select SubOrgId
                                                                   From SubOrg JOIN Organization ON SubOrg.OrgId = Organization.OrgId
                                                                   Where Organization.OrgId = currDepId);
			
		/* Combine Department Info */
		SET retLine1 = CONCAT(retLine1, CHAR(9), CHAR(9), "<Department>",CHAR(10),CHAR(9),CHAR(9),CHAR(9),"<info>", depName, ", ", depHead, ", ", depTotalSal, "</info>",CHAR(10),CHAR(9),CHAR(9),CHAR(9),"<Employees>",CHAR(10));
		
		BEGIN
			DECLARE d2 INT DEFAULT 0;
			DECLARE empName VARCHAR(30);
			DECLARE empSal INT;
			DECLARE empList CURSOR FOR
				SELECT Name, Salary
				FROM Employee
				WHERE OrgId = currDepId;
			DECLARE CONTINUE HANDLER FOR NOT FOUND SET d2 = 1;

			/* Get Employee Name and Salary */
			OPEN empList;
			getEmps: LOOP
				FETCH empList INTO empName,empSal;
				IF d2 = 1 THEN
					LEAVE getEmps;
				END IF;
			
				SET retLine1 = CONCAT(retLine1, CHAR(9),CHAR(9),CHAR(9),CHAR(9),"<Employee>", empName, ", ", empSal, "</Employee>",CHAR(10));
			END LOOP getEmps;
			CLOSE empList;
			SET retLine1 = CONCAT(retLine1, CHAR(9),CHAR(9),CHAR(9),"</Employees>",CHAR(10),CHAR(9),CHAR(9),"</Department>",CHAR(10));
		END;
	END LOOP getDeps;
	CLOSE depList;
	SET retLine1 = CONCAT(retLine1, CHAR(9), "</Departments>",CHAR(10),"</college>");
	SET full_report = retLine1;
END $$
DELIMITER ;

DELIMITER $$
DROP TRIGGER IF EXISTS after_insert_grade;

CREATE TRIGGER after_insert_grade
AFTER INSERT
ON Grades FOR EACH ROW

BEGIN

	DECLARE avgGrade INT;
	DECLARE inList INT;

	SELECT AVG(grade)
	INTO avgGrade
	FROM Grades
	WHERE sid = new.sid;

	SELECT COUNT(*)
	INTO inList
	FROM Scholarship
	WHERE sid = new.sid;

	IF avgGrade >= 90 AND inList = 0 THEN
		INSERT INTO Scholarship(sid, updateYear)
		VALUES(new.sid, 2020);
	ELSE
		IF avgGrade < 90 AND inList > 0 THEN
		DELETE FROM Scholarship WHERE sid = new.sid;
		END IF;
	END IF;
END $$

DELIMITER ;
