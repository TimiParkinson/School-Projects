/*
Name: Christian O. Parkinson
Course: CS 1400
Assignment: 3
Task: B
*/

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

class FileReader {
    public static void main(String[] args) {
        Scanner fileScanner = null;
        Scanner inputScanner = new Scanner(System.in);
        String filename;

        System.out.print("Enter the filename to read: ");
        filename = inputScanner.nextLine();

        try {
            fileScanner = new Scanner(new File(filename));

            int totalLines = 0;
            while (fileScanner.hasNextLine()) {
                totalLines++;
                fileScanner.nextLine();
            }

            // Calculate width of largest line number (to keep "|" aligned)
            int width = String.valueOf(totalLines).length();

            fileScanner = new Scanner(new File(filename));
            int lineNum = 1;

            while (fileScanner.hasNextLine()) {
                String line = fileScanner.nextLine();
                System.out.printf("%" + width + "d | %s%n", lineNum, line);
                lineNum++;
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + e.getMessage());
        } finally {
            if (fileScanner != null) {
                fileScanner.close();
            }
            inputScanner.close();
        }
    }
}