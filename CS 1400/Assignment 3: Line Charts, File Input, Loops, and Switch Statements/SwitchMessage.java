/*
Name: Christian O. Parkinson
Course: CS 1400
Assignment: 3
Task: D
*/

class SwitchMessage {
    public static void main(String[] args) {
        int position = 2; // Example position, can be changed to test different outputs
        switch (position) {
            case 1 -> System.out.println("You won!");
            case 2, 3 -> System.out.println("You were so close!");
            default -> System.out.println("Time to train harder!");  
        }
    }
}