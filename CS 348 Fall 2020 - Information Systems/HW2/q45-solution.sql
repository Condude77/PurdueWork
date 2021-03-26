DROP PROCEDURE IF EXISTS college_report;
DELIMITER $$

CREATE PROCEDURE college_report(IN college_name TEXT, OUT full_report TEXT)
report:BEGIN
	DECLARE total_salary INT;
	DECLARE head_name TEXT;
	DECLARE dept_name TEXT;
	DECLARE collegeId INT;
	DECLARE childOrg INT;	
	DECLARE employee_name TEXT;
	DECLARE employee_salary INT;

	DECLARE subOrg_cur CURSOR FOR SELECT SubOrgId FROM SubOrg WHERE OrgId = collegeId;
	DECLARE emp_cur CURSOR FOR SELECT Name, Salary FROM Employee WHERE OrgId = childOrg;
		
	SELECT OrgId INTO collegeId FROM Organization WHERE OrgName = college_name;
	IF collegeId IS NULL THEN
		LEAVE report;
	END IF;

	SELECT SUM(salary) INTO total_salary 
		FROM (SELECT DISTINCT EmpId, Salary FROM Organization o LEFT JOIN SubOrg so ON o.OrgId = so.OrgId, Employee e 
		WHERE o.OrgId = collegeId AND (e.OrgId = o.OrgId OR e.OrgId = so.subOrgId)) salaries;
	SELECT name INTO head_name FROM Organization o JOIN Employee e ON HeadId = EmpId WHERE o.OrgId = collegeId;
	
	SET full_report = CONCAT("<college>\n\t<info>", college_name, ", ", head_name, ", ", total_salary, "</info>\n\t<Departments>\n");

	subOrg:BEGIN
	 	DECLARE done INT;
		DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 0;

		OPEN subOrg_cur;
		ITR:LOOP
			FETCH subOrg_cur INTO childOrg;
			IF done = 0 THEN
				CLOSE subOrg_cur;
				LEAVE ITR;
			END IF;
			emp:BEGIN
				DECLARE done2 INT;
				DECLARE CONTINUE HANDLER FOR NOT FOUND SET done2 = 0;
				
				SELECT SUM(salary) INTO total_salary FROM Employee WHERE OrgId = childOrg;
				SELECT OrgName, Name INTO dept_name, head_name FROM Organization o JOIN Employee e ON HeadId = EmpId WHERE o.OrgId = childOrg;

				SET full_report = CONCAT(full_report, "\t\t<Department>\n\t\t\t<info>", dept_name, ", ", head_name, ", ", total_salary, "</info>\n\t\t\t<Employees>\n");

				OPEN emp_cur;
				ITR2:LOOP
					FETCH emp_cur INTO employee_name, employee_salary;
					IF done2 = 0 THEN
						CLOSE emp_cur;
						LEAVE ITR2;
					END IF;

					SET full_report = CONCAT(full_report, "\t\t\t\t<Employee>", employee_name, ", ", employee_salary, "</Employee>\n");
				END LOOP;
				SET full_report = CONCAT(full_report, "\t\t\t</Employees>\n\t\t</Department>\n");	
			END emp;
		END LOOP;
	END subOrg;

	SET full_report = CONCAT(full_report, "\t</Departments>\n</college>");
END$$

DELIMITER ;


DROP TRIGGER IF EXISTS after_insert_grade;
delimiter $$
create trigger after_insert_grade after insert on Grades
for each row
begin
    if (select avg(grade) from Grades group by sid having sid = NEW.sid) < 90 AND exists(select * from Scholarship where sid = NEW.sid) then
        delete from Scholarship where sid = NEW.sid;
    end if;
    if (select avg(grade) from Grades group by sid having sid = NEW.sid) >= 90 AND not exists(select * from Scholarship where sid = NEW.sid) then
        insert into Scholarship values(NEW.sid, 2020);
    end if;
end;
$$
delimiter ;
