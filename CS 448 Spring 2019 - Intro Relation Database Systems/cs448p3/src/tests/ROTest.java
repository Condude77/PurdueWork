package tests;

import org.junit.After;
import org.junit.Before;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.FixMethodOrder;
import org.junit.Test;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import global.AttrOperator;
import global.AttrType;
import global.RID;
import global.SearchKey;
import heap.HeapFile;
import index.HashIndex;
import relop.FileScan;
import relop.HashJoin;
import relop.IndexScan;
import relop.KeyScan;
import relop.Predicate;
import relop.Projection;
import relop.Schema;
import relop.Selection;
import relop.SimpleJoin;
import relop.Tuple;
import relop.Iterator;

import java.util.Scanner;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;

/**
 * Test suite for the relop layer.
 */
public class ROTest extends TestDriver {

	private static ROTest rot;

	/** The display name of the test suite. */
	private static final String TEST_NAME = "relational operator tests";

	/** Size of tables in test3. */
	private static final int SUPER_SIZE = 2000;
	
	private Scanner in;

	/** Drivers schema/table/index */
	private static Schema s_drivers;
	private static HeapFile f_drivers;
	private static HashIndex idx_drivers;

	/** Rides schema/table/index */
	private static Schema s_rides;
	private static HeapFile f_rides;
	private static HashIndex idx_rides;

	/** Groups schema/table/index */
	private static Schema s_groups;
	private static HeapFile f_groups;
	private static HashIndex idx_groups;

	/** Expected result strings for test cases */
	private static HashMap<String, String> results;
	
	protected void execute_and_compare(String testDesc, String id, Iterator it)
	{
		it.execute();
		it.close();
		String[] sol = results.get(id).split("|");
		Arrays.sort(sol);
		String[] res = it.getResult().split("|");
		Arrays.sort(res);
		assertTrue("FAILURE: " + testDesc + " output did not match expected result, should be " + results.get(id), Arrays.equals(sol, res));
	}

	public static void main(String[] args) {
		Result result = JUnitCore.runClasses(tests.ROTest.class);

		for (Failure failure : result.getFailures()) {
			System.out.println(failure.toString());
		}

		System.out.println(result.wasSuccessful());
	}
	
	@BeforeClass
	public static void setupDB() {
		// create a clean Minibase instance
		rot = new ROTest();
		rot.create_minibase();
		
		results = new HashMap<String, String>();

		try
		{
			Scanner in = new Scanner(new File("solution.txt"));
			String line;
			String[] res;
			while(in.hasNextLine())
			{
				line = in.nextLine();
				res = line.split("=");
				results.put(res[0],res[1]);
				
			}
		} catch (IOException e)
		{
			e.printStackTrace();
			System.exit(1);
		}
		
		Tuple d_tuple, r_tuple, g_tuple;

		// initialize schema for the "Drivers" table
		s_drivers = new Schema(5);
		s_drivers.initField(0, AttrType.INTEGER, 4, "DriverId");
		s_drivers.initField(1, AttrType.STRING, 20, "FirstName");
		s_drivers.initField(2, AttrType.STRING, 20, "LastName");
		s_drivers.initField(3, AttrType.FLOAT, 4, "Age");
		s_drivers.initField(4, AttrType.INTEGER, 4, "NumSeats");
		
		//Create and populate "Drivers" relation;
		f_drivers = new HeapFile("drivers");
		idx_drivers = new HashIndex("drivers_idx");
		d_tuple = new Tuple(s_drivers);
		d_tuple.setAllFields(1, "Ahmed", "Elmagarmid", 25F, 5);
		idx_drivers.insertEntry(new SearchKey(25F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(2, "Walid", "Aref", 20F, 13);
		idx_drivers.insertEntry(new SearchKey(20F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(3, "Christopher", "Clifton", 18F, 4);
		idx_drivers.insertEntry(new SearchKey(18F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(4, "Sunil", "Prabhakar", 22F, 7);
		idx_drivers.insertEntry(new SearchKey(22F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(5, "Elisa", "Bertino", 26F, 5);
		idx_drivers.insertEntry(new SearchKey(26F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(6, "Susanne", "Hambrusch", 23F, 3);
		idx_drivers.insertEntry(new SearchKey(23F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(7, "David", "Eberts", 24F, 8);
		idx_drivers.insertEntry(new SearchKey(24F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(8, "Arif", "Ghafoor", 20F, 5);
		idx_drivers.insertEntry(new SearchKey(20F), f_drivers.insertRecord(d_tuple.getData()));
		d_tuple.setAllFields(9, "Jeff", "Vitter", 19F, 10);
		idx_drivers.insertEntry(new SearchKey(19F), f_drivers.insertRecord(d_tuple.getData()));

		// initialize schema for the "Rides" table
		s_rides = new Schema(4);
		s_rides.initField(0, AttrType.INTEGER, 4, "DriverId");
		s_rides.initField(1, AttrType.INTEGER, 4, "GroupId");
		s_rides.initField(2, AttrType.STRING, 10, "FromDate");
		s_rides.initField(3, AttrType.STRING, 10, "ToDate");
		
		// Create and populate "Rides" table
		f_rides = new HeapFile("rides");
		idx_rides = new HashIndex("rides_idx");
		r_tuple = new Tuple(s_rides);
		r_tuple.setAllFields(3, 5, "2/10/2006", "2/13/2006");
		idx_rides.insertEntry(new SearchKey(5), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(1, 2, "2/12/2006", "2/14/2006");
		idx_rides.insertEntry(new SearchKey(2), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(9, 1, "2/15/2006", "2/15/2006");
		idx_rides.insertEntry(new SearchKey(1), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(5, 7, "2/14/2006", "2/18/2006");
		idx_rides.insertEntry(new SearchKey(7), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(1, 3, "2/15/2006", "2/16/2006");
		idx_rides.insertEntry(new SearchKey(3), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(2, 6, "2/17/2006", "2/20/2006");
		idx_rides.insertEntry(new SearchKey(6), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(3, 4, "2/18/2006", "2/19/2006");
		idx_rides.insertEntry(new SearchKey(4), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(4, 1, "2/19/2006", "2/19/2006");
		idx_rides.insertEntry(new SearchKey(1), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(2, 7, "2/18/2006", "2/23/2006");
		idx_rides.insertEntry(new SearchKey(7), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(8, 5, "2/20/2006", "2/22/2006");
		idx_rides.insertEntry(new SearchKey(5), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(3, 2, "2/24/2006", "2/26/2006");
		idx_rides.insertEntry(new SearchKey(2), f_rides.insertRecord(r_tuple.getData()));
		r_tuple.setAllFields(6, 6, "2/25/2006", "2/26/2006");
		idx_rides.insertEntry(new SearchKey(6), f_rides.insertRecord(r_tuple.getData()));

		// initialize schema for the "Groups" table
		s_groups = new Schema(2);
		s_groups.initField(0, AttrType.INTEGER, 4, "GroupId");
		s_groups.initField(1, AttrType.STRING, 10, "GroupName");
		
		// Create and populate "Groups" table
		f_groups = new HeapFile("groups");
		idx_groups = new HashIndex("groups_idx");
		g_tuple = new Tuple(s_groups);
		for(int i = 1; i <= 7; i++)
		{
			g_tuple.setAllFields(i, "Purdue" + i);
			idx_groups.insertEntry(new SearchKey(i), f_groups.insertRecord(g_tuple.getData()));
		}
		
		System.out.println();
		
	}
	
	@AfterClass
	public static void tearDownDB()
	{
		idx_drivers.deleteFile();
		idx_rides.deleteFile();
		idx_groups.deleteFile();
		f_rides.deleteFile();
		f_drivers.deleteFile();
		f_groups.deleteFile();
		rot.delete_minibase();
	}

	@Test
	public void testFileScan() {
		//Scan drivers table
		Iterator fscan = new FileScan(s_drivers, f_drivers);
		execute_and_compare("Filescan", "filescan", fscan);
	}

	@Test
	public void testIndexScan() {
		//Scan drivers index
		Iterator idxscan = new IndexScan(s_drivers, idx_drivers, f_drivers);
		execute_and_compare("IndexScan", "idxscan", idxscan);
	}

	@Test
	public void testKeyScan() {
		//Scan drivers index for key 20f
		Iterator keyscan = new KeyScan(s_drivers, idx_drivers, new SearchKey(20f), f_drivers);
		execute_and_compare("KeyScan", "keyscan", keyscan);
	}

	@Test
	public void testSelection() {
		//Selection drivers with age > 20
		Iterator selection = new Selection(new FileScan(s_drivers, f_drivers),
			new Predicate(AttrOperator.GT, AttrType.COLNAME, "age", AttrType.FLOAT, 20F));
		execute_and_compare("Selection", "selection", selection);
	}
	
	@Test
	public void testSelectionMultiplePredicates() {
		Iterator selection_preds = new Selection(new FileScan(s_drivers, f_drivers),
			new Predicate(AttrOperator.GT, AttrType.COLNAME, "age", AttrType.FLOAT, 23F),
			new Predicate(AttrOperator.LT, AttrType.COLNAME, "age", AttrType.FLOAT, 19F));
		execute_and_compare("Selection Multipled Predicates", "selection_preds", selection_preds);
	}
	
	@Test
	public void testProjection() {
		//Projection on Drivers: {FirstName, NumSeats}
		Iterator projection = new Projection(new FileScan(s_drivers, f_drivers), s_drivers.fieldNumber("FirstName"), s_drivers.fieldNumber("NumSeats"));
		execute_and_compare("Projection", "projection", projection);
	}
	
	@Test
	public void testHashJoin() {
		//HashJoin on Drivers X Rides on DriverID
		Iterator hashjoin = new HashJoin(new FileScan(s_drivers, f_drivers),
										new FileScan(s_rides, f_rides), 0, 0);
		execute_and_compare("Hash Join", "hashjoin", hashjoin);
	}
	
	@Test
	public void testSelectionPipelining() {
		//Test all possible Iterator inputs to Selection
		Iterator sel_idx = new Selection(new IndexScan(s_drivers, idx_drivers, f_drivers),
										new Predicate(AttrOperator.EQ, AttrType.COLNAME, "FirstName", AttrType.STRING, "Walid"));
		execute_and_compare("Selection - Pipelining IndexScan", "sel_idx", sel_idx);
		Iterator sel_key = new Selection(new KeyScan(s_drivers, idx_drivers, new SearchKey(20F), f_drivers),
										new Predicate(AttrOperator.EQ, AttrType.COLNAME, "FirstName", AttrType.STRING, "Walid"));
		execute_and_compare("Selection - Pipelining Keyscan", "sel_key", sel_key);
		Iterator sel_sel = new Selection(new Selection(new FileScan(s_drivers, f_drivers),
											new Predicate(AttrOperator.EQ, AttrType.COLNAME, "Age", AttrType.FLOAT, 20F)),
											new Predicate(AttrOperator.EQ, AttrType.COLNAME, "FirstName", AttrType.STRING, "Walid"));
		execute_and_compare("Selection - Pipelining Selection", "sel_sel", sel_sel);
		Iterator sel_proj = new Selection(new Projection(
											new FileScan(s_drivers, f_drivers), s_drivers.fieldNumber("DriverId"), s_drivers.fieldNumber("FirstName")),
											new Predicate(AttrOperator.EQ, AttrType.COLNAME, "FirstName", AttrType.STRING, "Walid"));
		execute_and_compare("Selection - Pipelining Projection", "sel_proj", sel_proj);
		Iterator sel_sj = new Selection(new SimpleJoin(new FileScan(s_drivers, f_drivers), new FileScan(s_rides, f_rides),
										new Predicate(AttrOperator.EQ, AttrType.FIELDNO, 0, AttrType.FIELDNO, 5)),
										new Predicate(AttrOperator.EQ, AttrType.COLNAME, "FirstName", AttrType.STRING, "Walid"));
		execute_and_compare("Selection - Pipelining Simple Join", "sel_sj", sel_sj);
		Iterator sel_hj = new Selection(new HashJoin(new FileScan(s_drivers, f_drivers), new FileScan(s_rides, f_rides),0,0),
										new Predicate(AttrOperator.EQ, AttrType.COLNAME, "FirstName", AttrType.STRING, "Walid"));
		execute_and_compare("Selection - Pipelining Hash Join", "sel_jh", sel_hj);
	}
	
	@Test
	public void testProjectionPipelining() {
		//Test all possible Iterator inputs to HashJoin
		Iterator proj_idx = new Projection(new IndexScan(s_drivers, idx_drivers, f_drivers),
										s_drivers.fieldNumber("DriverId"), s_drivers.fieldNumber("Age"));
		execute_and_compare("Projection - Pipelining IndexScan", "proj_idx", proj_idx);
		Iterator proj_key = new Projection(new KeyScan(s_drivers, idx_drivers, new SearchKey(20F), f_drivers),
										s_drivers.fieldNumber("DriverId"), s_drivers.fieldNumber("Age"));
		execute_and_compare("Projection - Pipelining KeyScan", "proj_key", proj_key);
		Iterator proj_sel = new Projection(new Selection(new FileScan(s_drivers, f_drivers),
											new Predicate(AttrOperator.EQ, AttrType.COLNAME, "Age", AttrType.FLOAT, 20F)),
											s_drivers.fieldNumber("DriverId"), s_drivers.fieldNumber("Age"));
		execute_and_compare("Projection - Pipelining Selection", "proj_sel", proj_sel);
		Iterator proj_proj = new Projection(new Projection(new FileScan(s_drivers, f_drivers),
											s_drivers.fieldNumber("DriverId"), s_drivers.fieldNumber("FirstName"), s_drivers.fieldNumber("Age")),
											0, 2);
		execute_and_compare("Projection - Pipelining Projection", "proj_proj", proj_proj);
		Iterator proj_sj = new Projection(new SimpleJoin(new FileScan(s_drivers, f_drivers), new FileScan(s_rides, f_rides),
										new Predicate(AttrOperator.EQ, AttrType.FIELDNO, 0, AttrType.FIELDNO, 5)),
										0, 3);
		execute_and_compare("Projection - Pipelining Simple Join", "proj_sj", proj_sj);
		Iterator proj_hj = new Projection(new HashJoin(new FileScan(s_drivers, f_drivers), new FileScan(s_rides, f_rides),0,0),
										0, 3);
		execute_and_compare("Projection - Pipelining Hash Join", "proj_hj", proj_hj);
	}
	
	@Test
	public void testHashjoinPipelining() {
		//Test all possible Iterator inputs to HashJoin
		Iterator hj_sel_sj = new HashJoin(new Selection(new FileScan(s_drivers, f_drivers),
												new Predicate(AttrOperator.GTE, AttrType.COLNAME, "DriverId", AttrType.INTEGER, 1)),
											new SimpleJoin(new FileScan(s_rides, f_rides), new FileScan(s_groups, f_groups),
												new Predicate(AttrOperator.EQ, AttrType.COLNAME, "GroupId", AttrType.COLNAME, "DriverId")),0,0);
		execute_and_compare("Hash Join - Pipelining Selection/Simple Join", "hj_sel_sj", hj_sel_sj);
		Iterator hj_proj_hj = new HashJoin(new Projection(new FileScan(s_drivers, f_drivers), s_drivers.fieldNumber("DriverId"), s_drivers.fieldNumber("Age")),
											new HashJoin(new FileScan(s_rides, f_rides), new FileScan(s_groups, f_groups), 1, 0), 0, 0);
		execute_and_compare("Hash Join - Pipelining Projection/Hash Join", "hj_proj_hj", hj_proj_hj);
		Iterator hj_iscan_kscan = new HashJoin(new IndexScan(s_drivers, idx_drivers, f_drivers),
												new KeyScan(s_rides, idx_rides, new SearchKey(1), f_rides), 0, 0);
		execute_and_compare("Hash Join - Pipelining IndexScan/KeyScan", "hj_iscan_kscan", hj_iscan_kscan);
	}

} // class ROTest extends TestDriver
