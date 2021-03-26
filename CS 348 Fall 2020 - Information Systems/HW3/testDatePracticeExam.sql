CREATE TABLE IF NOT EXISTS `ex1departments` (
  `dept_no` char(4) NOT NULL,
  `dept_name` varchar(40) NOT NULL,
  `budget` decimal(13,2) DEFAULT NULL,
  PRIMARY KEY (`dept_no`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ex1departments`
--

INSERT INTO `ex1departments` (`dept_no`, `dept_name`, `budget`) VALUES
('1', 'Accounting', 2000000.00),
('2', 'Research and Development', 2000000.00),
('3', 'Facilities', 1500000.00);

-- --------------------------------------------------------

--
-- Table structure for table `ex1dept_emp`
--

CREATE TABLE IF NOT EXISTS `ex1dept_emp` (
  `emp_no` int(11) NOT NULL,
  `dept_no` char(4) NOT NULL,
  PRIMARY KEY (`emp_no`,`dept_no`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ex1dept_emp`
--

INSERT INTO `ex1dept_emp` (`emp_no`, `dept_no`) VALUES
(101, '1'),
(101, '2'),
(102, '2'),
(103, '3'),
(104, '2'),
(104, '3');

-- --------------------------------------------------------

--
-- Table structure for table `ex1employees`
--

CREATE TABLE IF NOT EXISTS `ex1employees` (
  `emp_no` int(11) NOT NULL,
  `name` varchar(30) NOT NULL,
  `salary` decimal(9,1) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  `office_no` varchar(14) DEFAULT NULL,
  PRIMARY KEY (`emp_no`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ex1employees`
--

INSERT INTO `ex1employees` (`emp_no`, `name`, `salary`, `age`, `office_no`) VALUES
(101, 'John Smith', 80000.0, 35, 'ABC 213'),
(102, 'Fei Xie', 90000.0, 58, 'XYZ 456'),
(103, 'Brandon Carlson', 75000.0, 37, 'ABC 213'),
(104, 'Michael Hudson', 97000.0, 43, 'XYZ 456');

-- --------------------------------------------------------

--
-- Table structure for table `ex1offices`
--

CREATE TABLE IF NOT EXISTS `ex1offices` (
  `office_no` varchar(14) NOT NULL,
  `building` varchar(14) NOT NULL,
  `floor` smallint(6) NOT NULL,
  PRIMARY KEY (`office_no`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ex1offices`
--

INSERT INTO `ex1offices` (`office_no`, `building`, `floor`) VALUES
('ABC 213', 'ABC', 2),
('XYZ 456', 'XYZ', 4);

