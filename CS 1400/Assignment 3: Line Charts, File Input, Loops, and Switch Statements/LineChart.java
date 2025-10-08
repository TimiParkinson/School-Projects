/*
Name: Christian O. Parkinson
Course: CS 1400
Assignment: 3
Task: A
*/

import java.util.Scanner;

class LineChart {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String bar = "";
        int[] abilityScores = new int[5];

        for (int i = 0; i < 5; i++) {
            System.out.print("Enter Ability Score " + (i + 1) + ": ");
            abilityScores[i] = scanner.nextInt();
        }

        System.out.println("\nLine Chart:");
        for (int i = 0; i < abilityScores.length; i++) {
            bar = "";
            for (int j = 0; j < abilityScores[i]/10; j++) {
                bar += "-";
            }
            System.out.println("Ability " + i + ": " + bar);
        }
    }
}