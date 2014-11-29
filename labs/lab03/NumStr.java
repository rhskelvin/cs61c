/* 
 * CS 61C Fall 2013 Lab 03: MapReduce with EC2
 * NumStr.java
 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;

import org.apache.hadoop.io.WritableComparable;

/* An example class that contains a long and a String and can be used as
 * a key or a value in Hadoop. Values must implement the Writable
 * interface, and keys must implement the WritableComparable interface,
 * which in turn implements both the Writable and Comparable interfaces.
 */
public class NumStr implements WritableComparable<NumStr> {
    private long num;
    private String str; 

    // Constructs a NumStr with values (0, "")
    public NumStr() { 
        // do nothing
    }

    // Constructs a NumStr with values (num, str)
    public NumStr(long num, String str) {
        this.num = num;
        this.str = str;
    }

    /* Setters for num and str */
    public void setNum(long num) {
        this.num = num;
    }

    public void setStr(String str) {
        this.str = str;
    }

    /* Getters for num and str */
    public long getNum() {
        return num;
    }

    public String getStr() {
        return str;
    }
        

    /** write() is required to implement Writable. This method serializes the object.
     * 
     * @param out  The DataOutput object to which we serialize this NumStr.
     */
    public void write(DataOutput out) throws IOException {
        // YOUR CODE HERE
        out.writeLong(num);
        out.writeUTF(str);
        

    }

    /** readFields() is required to implement Writable. This method deserializes the
     * object. You should read in the same order that you wrote to the DataOutput object above.
     * 
     * @param in  The DataInput object from which we deserializing this NumStr.
     */
    public void readFields(DataInput in) throws IOException {
        // YOUR CODE HERE
        num = in.readLong();
        str = in.readUTF();

    }

    /** compareTo() is required to implement WritableComparable. This methods should 
     * first compare num, returning -1 if this.num is less than o.num or 1 if this.num
     * is greater than o.num. If this.num == o.num, this method should compare str
     * lexicographically by using the default compareTo() method of the String class. 
     *
     * @param o  The other NumStr we're comparing this NumStr to.
     */ 
    public int compareTo(NumStr o) {
        // YOUR CODE HERE
        if(num!=o.num) return (num > o.num)? 1:-1;
        return str.compareTo(o.str);

    }

    /** hashCode() is required to implement WritableComparable. From the Hadoop API: 
     * It's important that your implementation of hashCode() returns the same result across 
     * different instances of the JVM. Note also that the default hashCode() implementation
     * in Object does not satisfy this property.
     */
    public int hashCode() {
        int hash = 17;
        hash = hash * 31 + new Long(num).hashCode();
        hash = hash * 31 + str.hashCode();
        return hash;
    }

    public String toString(){
         return "(" + Long.toString(num) + ", " + str + ")";
    }
    
    /* Test code */
    public static void main(String[] args) {
        NumStr blank = new NumStr();
        NumStr first = new NumStr(1L, "b");
        NumStr second = new NumStr(2L, "a");
        NumStr third = new NumStr(1L, "c");

        System.out.println("Testing serialization/deserialization:");
        String dataFile = "numstrtestdata";

        try {
            DataOutputStream out = null;
            try {
                out = new DataOutputStream(new BufferedOutputStream(new
                    FileOutputStream(dataFile)));
                first.write(out);
            } finally {
                out.close();
            }
    
            DataInputStream in = null;
            try {
                in = new DataInputStream(new BufferedInputStream(new
                    FileInputStream(dataFile)));
                blank.readFields(in);
            } finally {
                in.close();
            }
            System.out.print("NumStr fourth should have the same value as NumStr first. Result is ");
            System.out.println((first.num == blank.num && first.str.equals(blank.str)) ? "CORRECT." : "INCORRECT");
        } catch (IOException e) {
            System.err.println("Error with serialization test.");
        }

        System.out.println();

        System.out.println("Testing comparison:");
        System.out.println("first compareTo second should output -1. Result is " + 
            (first.compareTo(second)== -1 ? "CORRECT." : "INCORRECT."));
        System.out.println("second compareTo first should output 1. Result is " +
            (second.compareTo(first)== 1 ? "CORRECT." : "INCORRECT."));
        System.out.println("first compareTo third should output -1. Result is " + 
            (first.compareTo(third)== -1 ? "CORRECT." : "INCORRECT."));
        System.out.println("third compareTo first should output 1. Result is " +
            (third.compareTo(first)== 1 ? "CORRECT." : "INCORRECT."));
        System.out.println("first compareTo blank should output 0. Result is " + 
            (first.compareTo(blank)== 0 ? "CORRECT." : "INCORRECT."));
    }
}
