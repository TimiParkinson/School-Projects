/*
Name: Christian O. Parkinson
Course: CS 1400
Assignment: 3
Task: C
*/

class CheeringCrowd {
    public static void main(String[] args) {
        int crowdSize = 5;
        
        for (int i = crowdSize; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                System.out.print("\\o/ ");
            }
            System.out.println();
        }
    }
}