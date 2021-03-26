import java.io.File;

/**
 * Created by brow1325 on 11/29/16.
 */
public class Recursion {

    public static int determinant(int[][] matrix){
        if(matrix.length == 1){
            return matrix[0][0];
        }else {
            int d = 0;
            for (int i = 0; i < matrix.length; i++) {
                int[][] A = new int[matrix.length - 1][matrix[i].length - 1];
                for (int j = 0; j < matrix[i].length; i++) {
                    if (i != j) {
                        for (int row = 0; row < A.length; row++) {
                            for (int col = 0; col < A[row].length; col++) {
                                A[row][col] = matrix[i][j];
                                System.out.println(A[row][col]);
                            }
                        }
                    }

                }
                if (i % 2 == 0) {
                    d = d - matrix[0][i] * determinant(A);
                } else {
                    d = d + matrix[0][i] * determinant(A);
                }
            }
            return d;
        }
    }

    public static int filecount(File f){
        int c = 0;
       if (f.isFile()) {
           c++;
       }else {
            File[] A = f.listFiles();
            for (int i = 0; i < A.length; i++) {
                if (A[i].isFile()) {
                    c++;
                }else if (A[i].isDirectory()) {
                    filecount(A[i]);
                }
            }
        }
        return c;
    }

}
