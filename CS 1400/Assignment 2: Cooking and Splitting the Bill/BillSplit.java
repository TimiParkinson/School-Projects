//Christian O. Parkinson
//CS1400
//Assignment 2
//2025-09-22

import java.util.Scanner;

class BillSplit {
    final static double TAX_RATE = 0.105; // 10.5% tax
    final static double TIP_RATE = 0.20;  // 20% tip
    public static double calculateTotal(double billAmount) {
        return billAmount + (billAmount * TAX_RATE) + (billAmount * TIP_RATE);
    }

    public static void main(String[] args) {
        int numberOfPeople;
        double billAmount;
        double totalBill;
        Scanner scanner = new Scanner(System.in);
        System.out.println("Welcome to the Bill Splitter!");
        System.out.print("Enter the number of people to split the bill: ");
        numberOfPeople = scanner.nextInt();
        System.out.print("Enter the total bill amount: $");
        billAmount = scanner.nextDouble();
        totalBill = calculateTotal(billAmount);
        System.out.printf("Total bill including tax and tip: $%.2f%n", totalBill);
        System.out.printf("Each person should pay: $%.2f%n", totalBill / numberOfPeople);
    }
}