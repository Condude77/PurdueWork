CREATE EXTERNAL TABLE cities2
(city VARCHAR(100),
state_id VARCHAR(5),
state_name VARCHAR(50),
county_name VARCHAR(50),
population BIGINT,
military BOOLEAN,
incorporated BOOLEAN,
timezone VARCHAR(50),
zips array<INT>,
id BIGINT)
ROW FORMAT DELIMITED FIELDS TERMINATED BY ',' 
COLLECTION ITEMS TERMINATED BY ' ' 
LOCATION 'gs://cs348-bucket2/hw7/'; -- Write the location of your file in Google storage