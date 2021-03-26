package cs448;

import org.apache.commons.cli.*;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.Optional;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.SparkSession;
import scala.Tuple2;
import scala.Tuple4;
import org.apache.spark.api.java.function.PairFunction;

import java.io.IOException;
import java.io.Serializable;
import java.util.List;

public class Project4 {
    public void runSparkApp1(App.Conf conf){
        System.out.println("Running Your First Spark App!");
        /* Hint: @see App#warmupExercise() for a sample Spark application code
        * using SparkRDD API
        */

        // Create a Spark Session.
	SparkSession spark = SparkSession.builder().appName("CS 448 Project 4 -- SparkApp1").getOrCreate();
	String dataFiles[] = {conf.usersFName, conf.moviesFName, conf.ratingsFName};
	Dataset<String> data;

        // Write data processing code here
        //get users
        JavaRDD<User> userRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.usersFName)).cache().javaRDD().map(User::parseUser);
        //get list of movies
        JavaRDD<Movie> movieRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.moviesFName)).cache().javaRDD().map(Movie::parseMovie);
	//get ratings
	JavaRDD<Rating> ratingRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.ratingsFName)).cache().javaRDD().map(Rating::parseRating);

	//start to filter
	//Filter users based on correct occupation
	Function<User, Boolean> userFilter = k -> (k.getOccupation() == conf.q1Occupation);
	JavaRDD<User> userRDDF = userRDD.filter(userFilter);
	PairFunction<User, Integer, User> pair1 = k -> new Tuple2(k.getUserId(), k);
	JavaPairRDD<Integer, User> userIdRDD = userRDDF.mapToPair(pair1);
	
	Function<Rating, Boolean> ratingFilter = k -> (k.getRating() >= conf.q1Rating);
	JavaRDD<Rating> ratingRDDF = ratingRDD.filter(ratingFilter);
	PairFunction<Rating, Integer, Rating> pair2 = k -> new Tuple2(k.getUserId(), k);
	JavaPairRDD<Integer, Rating> ratingIdRDD = ratingRDDF.mapToPair(pair2);

	//Join data sets that have same user ID in user and rating
	JavaRDD<Tuple2<User, Rating>> idRDD = userIdRDD.join(ratingIdRDD).values();
	
	//remove invalid users from list
	Function<Tuple2<User, Rating>, Rating> rem = k -> k._2;
	ratingRDDF = idRDD.map(rem);

	//filter movies and rating
	pair2 = k -> new Tuple2(k.getMovieId(), k);
	ratingIdRDD = ratingRDDF.mapToPair(pair2);
	PairFunction<Movie, Integer, Movie> pair3 = k -> new Tuple2(k.getMovieId(), k);
	JavaPairRDD<Integer, Movie> movieIdRDD = movieRDD.mapToPair(pair3);
	Function<Tuple2<Movie, Rating>, String> movieName = k -> k._1.getTitle();
	JavaRDD<String> movieNames = movieIdRDD.join(ratingIdRDD).values().map(movieName);

	movieNames = movieNames.distinct();	
	//output to file
	String out = CS448Utils.resolveUri(conf.outPath, "query-1");
	movieNames.saveAsTextFile(out);

        //Don't forget to stop spark session
        spark.stop();
    }

    public void runSparkApp2(App.Conf conf){
        System.out.println("Running Spark App for Query 2");
        // Write your code here
	//Create Spark Session
	SparkSession spark = SparkSession.builder().appName("CS 448 Project 4 -- SparkApp2").getOrCreate();
        String dataFiles[] = {conf.usersFName, conf.moviesFName, conf.ratingsFName};
        Dataset<String> data;
	
	//Create user RDD
	JavaRDD<User> userRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.usersFName)).cache().javaRDD().map(User::parseUser);
	
	//filter user RDD for occupations
	Function<User, Boolean> occupationFilter = k -> (k.getOccupation() == conf.q2Occupation1 || k.getOccupation() == conf.q2Occupation2);
	userRDD = userRDD.filter(occupationFilter);
	
	//map zipcodes 
	Function<User, String> zipCode = k -> k.getZipcode();
	JavaRDD<String> userZip = userRDD.map(zipCode).distinct();

	//output to file
	String out = CS448Utils.resolveUri(conf.outPath, "query-2");
	userZip.saveAsTextFile(out);

	//stop spark session
	spark.stop();

    }

    public void runSparkApp3(App.Conf conf){
        System.out.println("Running Spark App for Query 3");
        // Write your code here
        //Create Spark Session
        SparkSession spark = SparkSession.builder().appName("CS 448 Project 4 -- SparkApp2").getOrCreate();
        String dataFiles[] = {conf.usersFName, conf.moviesFName, conf.ratingsFName};
        Dataset<String> data;

	//get users
        JavaRDD<User> userRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.usersFName)).cache().javaRDD().map(User::parseUser);
        //get list of movies
        JavaRDD<Movie> movieRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.moviesFName)).cache().javaRDD().map(Movie::parseMovie);
        //get ratings
        JavaRDD<Rating> ratingRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.ratingsFName)).cache().javaRDD().map(Rating::parseRating);

	//Filter user
	Function<User, Boolean> userFilter = k -> (k.getOccupation() == conf.q3Occupation);
	userRDD = userRDD.filter(userFilter);
	
	//FIlter rating
	Function<Rating, Boolean> ratingFilter = k -> (k.getRating() == conf.q3Rating);
	ratingRDD = ratingRDD.filter(ratingFilter);

	//Join user and rating based on user id
	PairFunction<User, Integer, User> userPair = k -> new Tuple2(k.getUserId(), k);
	JavaPairRDD<Integer, User> userId = userRDD.mapToPair(userPair);
	
	PairFunction<Rating, Integer, Rating> ratingPair = k -> new Tuple2(k.getUserId(), k);
	JavaPairRDD<Integer, Rating> ratingId = ratingRDD.mapToPair(ratingPair);
	
	JavaRDD<Tuple2<User, Rating>> idRDD = userId.join(ratingId).values();
	
	//remove invalid vals
	Function<Tuple2<User, Rating>, Rating> rem = k -> k._2;
	ratingRDD = idRDD.map(rem);
	
	//Filter movie
	PairFunction<Movie, Integer, Movie> moviePair = k -> new Tuple2(k.getMovieId(), k);
	JavaPairRDD<Integer, Movie> movieId = movieRDD.mapToPair(moviePair);
	
	ratingPair = k -> new Tuple2(k.getMovieId(), k);
	ratingId = ratingRDD.mapToPair(ratingPair);
	
	Function<Tuple2<Movie, Rating>, Integer> movieFilter = k -> k._1.getMovieId();
	JavaRDD<Integer> movieRDDF = movieId.join(ratingId).values().map(movieFilter);
	
	movieRDDF = movieRDDF.distinct();
	
	//output to file
	String out = CS448Utils.resolveUri(conf.outPath, "query-3");
	movieRDDF.saveAsTextFile(out);
	
	//Stop Spark Session
	spark.stop();
    }

    public void runSparkApp4(App.Conf conf){
        System.out.println("Running Spark App for Query 4");
        // Write your code here
        //Create spark session
        SparkSession spark = SparkSession.builder().appName("CS 448 Project 4 -- SparkApp2").getOrCreate();
        String dataFiles[] = {conf.usersFName, conf.moviesFName, conf.ratingsFName};
        Dataset<String> data;

        //get users
        JavaRDD<User> userRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.usersFName)).cache().javaRDD().map(User::parseUser);
        //get list of movies
        JavaRDD<Movie> movieRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.moviesFName)).cache().javaRDD().map(Movie::parseMovie);
        //get ratings
        JavaRDD<Rating> ratingRDD = spark.read().textFile(CS448Utils.resolveUri(conf.inPath, conf.ratingsFName)).cache().javaRDD().map(Rating::parseRating);

	//filter user
	Function<User, Boolean> userFilter = k -> (k.getAge() == conf.q4Age);
	userRDD = userRDD.filter(userFilter);
	
	//join user and rating based on user id
	PairFunction<User, Integer, User> userPair = k -> new Tuple2(k.getUserId(), k);
        JavaPairRDD<Integer, User> userId = userRDD.mapToPair(userPair);

        PairFunction<Rating, Integer, Rating> ratingPair = k -> new Tuple2(k.getUserId(), k);
        JavaPairRDD<Integer, Rating> ratingId = ratingRDD.mapToPair(ratingPair);

        JavaRDD<Tuple2<User, Rating>> idRDD = userId.join(ratingId).values();

        //remove invalid vals
        Function<Tuple2<User, Rating>, Rating> rem = k -> k._2;
        ratingRDD = idRDD.map(rem);

	//get movie and rating pair
	PairFunction<Rating, Integer, Integer> ratingPair2 = k -> new Tuple2(k.getMovieId(), k.getRating());
	JavaPairRDD<Integer, Integer> intId = ratingRDD.mapToPair(ratingPair2);
	
	//get average of ratings
	Tuple2<Double, Integer> zero = new Tuple2(0.0, 0);
	Function2<Tuple2<Double, Integer>, Integer, Tuple2<Double, Integer>> increase = (k, j) -> new Tuple2(k._1 + j, k._2 + 1);
	Function2<Tuple2<Double, Integer>, Tuple2<Double, Integer>, Tuple2<Double, Integer>> merger = (k, j) -> new Tuple2(k._1 + j._1, k._2 + j._2);
	
	JavaPairRDD<Integer, Tuple2<Double, Integer>> total = intId.aggregateByKey(zero, increase, merger);
	Function<Tuple2<Double, Integer>, Double> avgFilter = k -> (k._1 / k._2);
	JavaPairRDD<Integer, Double> average = total.mapValues(avgFilter);
	
	//filter movie
	PairFunction<Movie, Integer, Movie> moviePair = k -> new Tuple2(k.getMovieId(), k);
	JavaPairRDD<Integer, Movie> movieId = movieRDD.mapToPair(moviePair);

	//pair movie titles and average rating
	Function<Tuple2<Double, Movie>, String> grouping = k -> (k._2.getTitle() + "::" + k._1);
	JavaRDD<String> movieAvg = average.join(movieId).values().map(grouping);
	
	//output to file
	String out = CS448Utils.resolveUri(conf.outPath, "query-4");
	movieAvg.saveAsTextFile(out);
	
	//close spark session
	spark.close();

    }
}
