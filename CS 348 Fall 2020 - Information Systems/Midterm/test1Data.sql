
--
-- Table structure for table `excomputer`
--

CREATE TABLE IF NOT EXISTS `excomputer` (
  `comp_id` int(11) NOT NULL,
  `price` int(11) NOT NULL,
  `discount-price` int(11) NOT NULL,
  `ram` int(11) NOT NULL,
  `cpu` varchar(20) NOT NULL,
  `man_id` int(11) NOT NULL,
  PRIMARY KEY (`comp_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `excomputer`
--

INSERT INTO `excomputer` (`comp_id`, `price`, `discount-price`, `ram`, `cpu`, `man_id`) VALUES
(201, 700, 650, 16, 'I7', 101),
(202, 1200, 1100, 32, 'I5', 102),
(203, 1900, 1900, 64, 'I7', 103),
(204, 2000, 1950, 32, 'I7', 101);

-- --------------------------------------------------------

--
-- Table structure for table `excustomer`
--

CREATE TABLE IF NOT EXISTS `excustomer` (
  `cus_id` int(11) NOT NULL,
  `cus_name` varchar(50) NOT NULL,
  `zipcode` int(11) NOT NULL,
  `age` int(11) NOT NULL,
  PRIMARY KEY (`cus_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `excustomer`
--

INSERT INTO `excustomer` (`cus_id`, `cus_name`, `zipcode`, `age`) VALUES
(301, 'John', 47907, 20),
(302, 'Xie', 47907, 22),
(303, 'Sara', 47906, 55),
(304, ' Niveditha', 47908, 20);

-- --------------------------------------------------------

--
-- Table structure for table `excustomer_computer`
--

CREATE TABLE IF NOT EXISTS `excustomer_computer` (
  `cus_id` int(11) NOT NULL,
  `comp_id` int(11) NOT NULL,
  PRIMARY KEY (`cus_id`,`comp_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `excustomer_computer`
--

INSERT INTO `excustomer_computer` (`cus_id`, `comp_id`) VALUES
(301, 201),
(301, 202),
(302, 202),
(303, 201),
(303, 202),
(303, 203);

--
-- Table structure for table `exmanfacturer`
--

CREATE TABLE IF NOT EXISTS `exmanfacturer` (
  `man_id` int(11) NOT NULL,
  `man_name` varchar(50) NOT NULL,
  PRIMARY KEY (`man_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `exmanfacturer`
--

INSERT INTO `exmanfacturer` (`man_id`, `man_name`) VALUES
(101, 'DELL'),
(102, 'HP'),
(103, 'Lenovo');


-- --------------------------------------------------------

--
-- Table structure for table `exdrug`
--

CREATE TABLE IF NOT EXISTS `exdrug` (
  `drug_id` int(11) NOT NULL,
  `drug_name` varchar(50) NOT NULL,
  PRIMARY KEY (`drug_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `exdrug`
--

INSERT INTO `exdrug` (`drug_id`, `drug_name`) VALUES
(301, 'Tylenol'),
(302, 'B12 Vitamin'),
(303, 'Vitamin C'),
(304, 'Ampicillin');

-- --------------------------------------------------------

-- --------------------------------------------------------

--
-- Table structure for table `expatient`
--

CREATE TABLE IF NOT EXISTS `expatient` (
  `patient_id` int(11) NOT NULL,
  `patient_name` varchar(50) NOT NULL,
  `age` int(11) NOT NULL,
  PRIMARY KEY (`patient_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `expatient`
--

INSERT INTO `expatient` (`patient_id`, `patient_name`, `age`) VALUES
(101, 'David Stewart', 55),
(102, 'Mat Smith', 16),
(103, 'Ronald Allen', 72),
(104, 'Mat Smith', 80),
(105, 'Tom Mcray', 14);

-- --------------------------------------------------------

--
-- Table structure for table `exphysician`
--

CREATE TABLE IF NOT EXISTS `exphysician` (
  `physician_id` int(11) NOT NULL,
  `physician_name` varchar(50) NOT NULL,
  `age` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `exphysician`
--

INSERT INTO `exphysician` (`physician_id`, `physician_name`, `age`) VALUES
(201, 'Wei Xie', 40),
(202, 'Michael James', 33),
(203, 'Elena Jose', 27);

-- --------------------------------------------------------

--
-- Table structure for table `exprescription`
--

CREATE TABLE IF NOT EXISTS `exprescription` (
  `prep_id` int(11) NOT NULL,
  `patient_id` int(11) NOT NULL,
  `drug_id` int(11) NOT NULL,
  `physician_id` int(11) NOT NULL,
  `quantity` int(11) NOT NULL,
  `unit_price` int(11) NOT NULL,
  PRIMARY KEY (`prep_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `exprescription`
--

INSERT INTO `exprescription` (`prep_id`, `patient_id`, `drug_id`, `physician_id`, `quantity`, `unit_price`) VALUES
(1, 102, 302, 201, 2, 12),
(2, 104, 302, 201, 3, 13),
(3, 104, 304, 202, 1, 34);