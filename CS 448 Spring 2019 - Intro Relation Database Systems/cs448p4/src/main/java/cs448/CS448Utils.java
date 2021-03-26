package cs448;

public class CS448Utils {

    public static String HDFS_URI = "hdfs://scholar-h000.rcac.purdue.edu:8020";

    //Testing values for Q1
    public static int TEST_q1Rating = 3;
    public static int TEST_q1Occupation = 12;

    //Testing values for Q2
    public static int TEST_q2Occupation1 = 12;
    public static int TEST_q2Occupation2 = 11;

    //Testing values for Q3
    public static int TEST_q3Occupation = 12;
    public static int TEST_q3Rating = 3;

    //Testing values for Q4
    public static int TEST_q4Age = 18;

    public static String resolveUri(String path){
        StringBuilder sb = new StringBuilder();
        sb.append(HDFS_URI);
        sb.append('/');
        sb.append(path);
        return sb.toString();
    }

    public static String resolveUri(String dirPath, String filename){
        StringBuilder sb = new StringBuilder();
        sb.append(dirPath);
        sb.append('/');
        sb.append(filename);
        return resolveUri(sb.toString());
    }

    public static String getTestUri(int i){
        return resolveUri(String.format("/user/tqadah/test/query-%d",i));
    }

}
