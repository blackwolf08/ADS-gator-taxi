import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collections;
import java.util.List;

import ride.Ride;
import ride.RideRequestManager;
import ride.MinHeap;
import ride.RedBlackTree;

public class gatorTaxi {
	static boolean is_repeated=false;

 public static void main(String[] args) {
	        if (args.length == 0) {
	            System.out.println("Error: Input file name not provided.");
	            return;
	        }
	        String inputFileName = args[0];
	        String outputFileName = "output.txt";

	        // Initialize ride database
	        RideRequestManager rideDataBase = new RideRequestManager();
	        // Open input and output files
	        try (BufferedReader reader = new BufferedReader(new FileReader(inputFileName));
	             FileWriter writer = new FileWriter(outputFileName)) {
	            boolean isrepeated=false;
	            String line;
	            while ((line = reader.readLine()) != null) {
	                String[] parts = line.split("\\(");
	                String command = parts[0];
	                String[] params = parts[1].replaceAll("\\)", "").split(",");
	                if(isrepeated) {
	                    break;
	                }
	                switch (command) {
	                    case "Insert":
	                        int rideNumber = Integer.parseInt(params[0].trim());
	                        int rideCost = Integer.parseInt(params[1].trim());
	                        int tripDuration = Integer.parseInt(params[2].trim());
	                       boolean success= rideDataBase.insertRide(rideNumber, rideCost, tripDuration);
	                        if(is_repeated) {
	                         writer.write("Duplicate RideNumber");
	                        isrepeated=true;
	                         }
	                        if (!success) {
	                            writer.write("heap is full.\n");
	                        }
	                       
	                        break;
	                    case "Print":
	                        if (params.length == 1) {
	                           rideNumber = Integer.parseInt(params[0].trim());
	                            Ride ride=rideDataBase.printRide(rideNumber);
	                        
	                            if (ride == null) {
	                                writer.write("(0,0,0)\n");
	                            } else {
	                                writer.write("("+ride.rideNumber+","+ride.rideCost+","+ride.tripDuration+")"+ "\n");
	                            }
	                        } else if (params.length == 2) {
	                            int rideNumber1 = Integer.parseInt(params[0].trim());
	                            int rideNumber2 = Integer.parseInt(params[1].trim());
	                            List<Ride> ridesInRange = rideDataBase.printRideRange(rideNumber1, rideNumber2);
	                            if (ridesInRange.isEmpty()) {
	                                writer.write("(0,0,0)\n");
	                            } else {
	                                for (Ride ride : ridesInRange) {
	                                    writer.write("("+ride.rideNumber+","+ride.rideCost+","+ride.tripDuration+")"+ ",");
	                                }
	                                writer.write("\n");
	                            }
	                        }
	                        break;
	                    case "UpdateTrip":
	                        rideNumber = Integer.parseInt(params[0].trim());
	                        tripDuration = Integer.parseInt(params[1].trim());
	                        rideDataBase.UpdateTrip(rideNumber, tripDuration);
	                        break;
	                    case "CancelRide":
	                        rideNumber = Integer.parseInt(params[0].trim());
	                        rideDataBase.cancelRide(rideNumber);
	                        break;
	                    case "GetNextRide":
	                        Ride ride = rideDataBase.getNextRide();
	                        if (ride == null) {
	                            writer.write("No active ride requests.\n");
	                        } else {
	                            writer.write("("+ride.rideNumber+","+ride.rideCost+","+ride.tripDuration+")"+ "\n");
	                        }
	                        break;
	                    default:
	                        writer.write("Error: Unknown command.\n");
	                        break;
	                }
	            }
	            
	        } catch (IOException e) {
	            System.out.println("Error: " + e.getMessage());
	       
	    	        }
	    	        
	    	    }
	    

   
	}