/**
 * Created by brow1325 on 10/4/16.
 */
public class Matrix {
    /**
     * Checks fist to make sure if the diagonal is square. if not it returns false.
     * Then checks if the values of row i column j equal row j column i, if not returns false.
     *
     * @param matrix
     * @return
     */
    public boolean isSymmetric(int[][] matrix){
        for(int i = 0; i < matrix.length; i++){
            for(int j = 0; j < matrix[i].length; j++){
                if(matrix.length != matrix[j].length) {
                    return false;
                }
                if(matrix[i][j] != matrix[j][i]){
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Checks first that when i and j are not the same value that the entries are equal to 0.
     *
     *
     * @param matrix
     * @return
     */
    public boolean isDiagonal(int[][] matrix){
        for(int i = 0; i < matrix.length; i++){
            for(int j = 0; j < matrix[i].length; j++){
                if(i != j && matrix[i][j] != 0){
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Checks that it is square. Checks that the main diagonal has values of 1.
     * Checks that all other values are 0.
     *
     * @param matrix
     * @return
     */
    public boolean isIdentity(int[][] matrix){
        for(int i = 0; i < matrix.length; i++){
            for(int j = 0; j < matrix[i].length; j++){
                if(matrix.length != matrix[j].length) {
                    return false;
                }
                if(i == j && matrix[i][j] != 1){
                    return false;
                }
                if(i != j && matrix[i][j] != 0){
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Checks that it is square. Checks that squares below the main diagonal equal 0.
     *
     * @param matrix
     * @return
     */
    public boolean isUpperTriangular(int[][] matrix){
        for(int i = 0; i < matrix.length; i++){
            for(int j = 0; j < matrix[i].length; j++){
                if(matrix.length != matrix[j].length) {
                    return false;
                }
                if(matrix[i][j] != 0 && i > j){
                    return false;
                }

            }
        }
        return true;
    }

    /**
     * Ckecks that it is square. Checks that all entries other than main diagonal, and upper and lower diagonal are 0.
     *
     * @param matrix
     * @return
     */
    public boolean isTridiagonal(int[][] matrix) {
        for(int i = 0; i < matrix.length; i++){
            for(int j = 0; j < matrix[i].length; j++){
                if(matrix.length != matrix[j].length) {
                    return false;
                }
                if((i != j && i +1 != j && i-1 != j) && matrix[i][j] != 0){
                    return false;
                }
            }
        }
        return true;
    }

    public static void main(String[] args){
        Matrix m = new Matrix();
        int[][] matrix =
                {{1, 1, 0, 0},
                        {1, 1, 1, 0},
                        {0, 1, 1, 1},
                        {0, 0, 1, 1}};
        System.out.println(m.isSymmetric(matrix));
        System.out.println(m.isDiagonal(matrix));
        System.out.println(m.isIdentity(matrix));
        System.out.println(m.isUpperTriangular(matrix));
        System.out.println(m.isTridiagonal(matrix));
    }

}
