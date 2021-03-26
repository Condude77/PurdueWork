SET FOREIGN_KEY_CHECKS = 0;

CREATE TABLE Organization (
	OrgId INTEGER,
	OrgName VARCHAR(255),
	HeadId    INTEGER,
	PRIMARY KEY (OrgId),
	FOREIGN KEY (HeadId) REFERENCES Employee(EmpId)
);

CREATE TABLE Employee (
	EmpId INTEGER,
	Name VARCHAR(20),
	Job VARCHAR(20),
	Salary INTEGER,
	OrgId INTEGER,
	PRIMARY KEY (EmpId),
	FOREIGN KEY (OrgId) REFERENCES Organization(OrgId)
);

CREATE TABLE SubOrg (
	OrgId INTEGER,
	SubOrgId INTEGER,
	PRIMARY KEY (OrgId, SubOrgId),
	FOREIGN KEY (OrgId) REFERENCES Organization(OrgId),
	FOREIGN KEY (SubOrgId) REFERENCES Organization(OrgId)
);

CREATE TABLE Students (
	sid INTEGER,
	sname VARCHAR(30),
	birthdate DATE,
	PRIMARY KEY(sid)
);

CREATE TABLE Courses (
	cid INTEGER,
	cname VARCHAR(30),
	description VARCHAR(50),
	PRIMARY KEY(cid)
);

CREATE TABLE Grades (
	sid INTEGER,
	cid INTEGER,
	grade INTEGER,
	PRIMARY KEY(sid, cid),
	FOREIGN KEY(sid) REFERENCES Students(sid),
	FOREIGN KEY(cid) REFERENCES Courses(cid)
);

CREATE TABLE Scholarship (
	sid INTEGER,
	updateYear INTEGER,
	PRIMARY KEY(sid),
	FOREIGN KEY(sid) REFERENCES Students(sid)
);

SET FOREIGN_KEY_CHECKS = 1;
