import javax.swing.JOptionPane;
/**
 * Created by brow1325 on 10/18/16.
 */
public class FAFSAGUI{

    public static void main(String[] args){
        boolean c;
        do {
            JOptionPane.showMessageDialog(null, "Welcome to the FAFSA!", "Welcome", JOptionPane.INFORMATION_MESSAGE);
            int isAccepted = JOptionPane.showOptionDialog(null, "Have you been accepted into a degree or certificate " +
                            "program?", "Program Acceptance", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null,
                    null, null);
            boolean accepted;
            if (isAccepted == 0) {
                accepted = true;
            } else {
                accepted = false;
            }
            int isSSregistered = JOptionPane.showOptionDialog(null, "Are you registered for the selective service?",
                    "Selective Service", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, null, null);
            boolean registered;
            if (isSSregistered == 0) {
                registered = true;
            } else {
                registered = false;
            }
            int hasSSN = JOptionPane.showOptionDialog(null, "Do you have a social security number?",
                    "Social Security Number", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, null, null);
            boolean SSN;
            if (hasSSN == 0) {
                SSN = true;
            } else {
                SSN = false;
            }
            int hasValidResidency = JOptionPane.showOptionDialog(null, "Do you have valid residency status?",
                    "Residency Status", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, null, null);
            boolean residency;
            if (hasValidResidency == 0) {
                residency = true;
            } else {
                residency = false;
            }
            String age = JOptionPane.showInputDialog(null, "How old are you?", "Age", JOptionPane.QUESTION_MESSAGE);
            int a = Integer.parseInt(age);
            while (a < 0) {
                JOptionPane.showMessageDialog(null, "Age cannot be a negative number.", "Error: Age", JOptionPane
                        .ERROR_MESSAGE);
                age = JOptionPane.showInputDialog(null, "How old are you?", "Age", JOptionPane.QUESTION_MESSAGE);
                a = Integer.parseInt(age);
            }
            String creditHours = JOptionPane.showInputDialog(null, "How many credit hours do you plan on taking?",
                    "Credit Hours", JOptionPane.QUESTION_MESSAGE);
            int cH = Integer.parseInt(creditHours);
            while (cH < 1 || cH > 24) {
                JOptionPane.showMessageDialog(null, "Credit hours must be between 1 and 24, inclusive.", "Error: Credit " +
                        "Hours", JOptionPane.ERROR_MESSAGE);
                creditHours = JOptionPane.showInputDialog(null, "How many credit hours do you plan on taking?",
                        "Credit Hours", JOptionPane.QUESTION_MESSAGE);
                cH = Integer.parseInt(creditHours);
            }
            String studentIncome = JOptionPane.showInputDialog(null, "What is your total yearly income?", "Student Income",
                    JOptionPane.QUESTION_MESSAGE);
            double sI = Double.parseDouble(studentIncome);
            while (sI < 0) {
                JOptionPane.showMessageDialog(null, "Income cannot be a negative number.", "Error: Student Income",
                        JOptionPane.ERROR_MESSAGE);
                studentIncome = JOptionPane.showInputDialog(null, "What is your total yearly income?", "Student Income",
                        JOptionPane.QUESTION_MESSAGE);
                sI = Double.parseDouble(studentIncome);
            }
            String parentIncome = JOptionPane.showInputDialog(null, "What is your parent's total yearly income?",
                    "Parent Income", JOptionPane.QUESTION_MESSAGE);
            double pI = Double.parseDouble(parentIncome);
            while (pI < 0) {
                JOptionPane.showMessageDialog(null, "Income cannot be a negative number.", "Error: Parent Income",
                        JOptionPane.ERROR_MESSAGE);
                parentIncome = JOptionPane.showInputDialog(null, "What is your parent's total yearly income?", "Parent " +
                        "Income", JOptionPane.QUESTION_MESSAGE);
                pI = Double.parseDouble(parentIncome);
            }
            int isIndependent = JOptionPane.showOptionDialog(null, "Are you a dependent?", "Dependency",
                    JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, null, null);
            boolean independent;
            if (isIndependent == 0) {
                independent = true;
            } else {
                independent = false;
            }
            String[] selectionValues = {"Freshman", "Sophomore", "Junior", "Senior", "Graduate"};
            String classStanding = (String) JOptionPane.showInputDialog(null, "What is your current class Standing?",
                    "Class standing", JOptionPane.PLAIN_MESSAGE, null, selectionValues, null);
            if (classStanding.equals("Graduate")) {
                classStanding = "Graduate";
            } else {
                classStanding = "Undergraduate";
            }
            FAFSA f = new FAFSA(accepted, registered, SSN, residency, independent, a, cH,
                    sI, pI, classStanding);
            double loan = f.calcStaffordLoan();
            double grant = f.calcFederalGrant();
            double workStudy = f.calcWorkStudy();
            double award = f.calcFederalAidAmount();
            String results = String.format("Loan: $%.2f%nGrants: %.2f%nWork Study: $%.2f%n------------%nTotal: $%.2f",
                    loan, grant, workStudy, award);
            JOptionPane.showMessageDialog(null, results, "FAFSA Results", JOptionPane.INFORMATION_MESSAGE);
            int cont = JOptionPane.showOptionDialog(null, "Would you like to compete another Application?",
                    "Continue",
                    JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, null, null);
            if (cont == 0) {
                c = true;
            } else {
                c = false;
            }
        }while(c != false);
    }
}
