package ride;

public class RideRequestManager {
    MinHeap minHeap;
    RedBlackTree rbt;

    public RideRequestManager() {
        this.minHeap = new MinHeap(100);
        this.rbt = new RedBlackTree();
    }
        
    public boolean insertRide(int rideNumber, int rideCost, int tripDuration) {
        Ride ride = new Ride(rideNumber, rideCost, tripDuration);
        boolean inserted= minHeap.insert(ride);
        if(inserted=true) {
        rbt.insert(ride);
        }
        return inserted;
    }
        
    public Ride getNextRide() {
        if (minHeap.isEmpty()) {
            
            return null;
        } else {
            Ride nextRide = minHeap.extractMin();
            rbt.delete(nextRide);
            return nextRide;
        }
    }

    public void cancelRide(int rideNumber) {
        Ride ride = rbt.search(rideNumber);
        if (ride != null) {
            rbt.delete(ride);
            minHeap.remove(ride);
        }
    }
    public void UpdateTrip(int rideNumber, int new_tripDuration) {
        Ride rideToUpdate = rbt.search(rideNumber);
        if (rideToUpdate == null) {
            return;
        }
        if (new_tripDuration <= rideToUpdate.getTripDuration()) {
            rideToUpdate.setTripDuration(new_tripDuration);
        } else if (new_tripDuration <= 2 * rideToUpdate.getTripDuration()) {
            int newRideCost = rideToUpdate.getRideCost() + 10;
            cancelRide(rideNumber);
            insertRide(rideNumber, newRideCost, new_tripDuration);
        } else {
            cancelRide(rideNumber);
        }
    }


    public Ride printRide(int rideNumber) {
        Ride ride = rbt.search(rideNumber);
        return ride;
    }

    public List<Ride> printRideRange(int rideNumber1, int rideNumber2) {
        List<Ride> ridesInRange = rbt.rangeSearch(rideNumber1, rideNumber2);
    return ridesInRange;  
    
    }


}