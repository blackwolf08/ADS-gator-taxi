package ride;

class Ride {
    int rideNumber;
    int rideCost;
    int tripDuration;

    public Ride(int rideNumber, int rideCost, int tripDuration) {
        this.rideNumber = rideNumber;
        this.rideCost = rideCost;
        this.tripDuration = tripDuration;
    }
    public int getRideCost() {
        return rideCost;
    }

    public int getTripDuration() {
        return tripDuration;
    }

    public void setTripDuration(int tripDuration) {
        this.tripDuration = tripDuration;
    }
    public int getRideNumber() {
        return rideNumber;
    }

}