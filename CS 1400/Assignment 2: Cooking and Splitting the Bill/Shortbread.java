//Christian O. Parkinson
//CS1400
//Assignment 2
//2025-09-22

import java.util.Scanner;

class Shortbread {
    public static void main(String[] args) {
        final int STD_SERVINGS = 24;
        String[] ingredients = {"Butter", "Sugar", "Flour"};
        double[] ingredientsGrams = {125, 55, 180};
        double[] ingredientsOunces = {4, 2, 6};
        Scanner scanner = new Scanner(System.in);
        System.out.println("Hello! This program will help you make shortbread cookies.");
        System.out.println("How many servings would you like to make?");
        int servings = scanner.nextInt();
        System.out.println("For " + servings + " servings of shortbread, you will need:");
        for (int i = 0; i < ingredients.length; i++) {
            double gramsNeeded = (ingredientsGrams[i] / STD_SERVINGS) * servings;
            double ouncesNeeded = (ingredientsOunces[i] / STD_SERVINGS) * servings;
            System.out.printf("- %.2f grams (or %.2f ounces) of %s%n", gramsNeeded, ouncesNeeded, ingredients[i]); 
        }
    }
}