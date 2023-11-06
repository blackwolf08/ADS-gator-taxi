package ride;

class MinHeap {
    Ride[] heap;
    int size;

    public MinHeap(int maxSize) {
        this.heap = new Ride[maxSize];
        this.size = 0;
    }

    public boolean insert(Ride ride) {
        for (int i = 0; i < size; i++) {
            if (heap[i].rideNumber == ride.rideNumber) {
                is_repeated=true;
                return false;
            }
        }
        if (size == heap.length) {
            return false;
        }

        int index = size;
        heap[index] = ride;
        size++;

        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (heap[parentIndex].rideCost < heap[index].rideCost ||
                (heap[parentIndex].rideCost == heap[index].rideCost &&
                    heap[parentIndex].tripDuration <= heap[index].tripDuration)) {
                break;
            }
            Ride temp = heap[parentIndex];
            heap[parentIndex] = heap[index];
            heap[index] = temp;
            index = parentIndex;
            
        }
        return true;
    }

    public Ride extractMin() {
        if (size == 0) {
            return null;
        }
        
        Ride min = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        
        return min;
    }
    
    private void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        
        if (left < size && compare(heap[left], heap[smallest]) < 0) {
            smallest = left;
        }
        
        if (right < size && compare(heap[right], heap[smallest]) < 0) {
            smallest = right;
        }
        
        if (smallest != index) {
            swap(index, smallest);
            heapifyDown(smallest);
        }
    }

    private void swap(int i, int j) {
        Ride temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    private int compare(Ride r1, Ride r2) {
        int cmp = Double.compare(r1.rideCost, r2.rideCost);
        if (cmp == 0) {
            cmp = Integer.compare(r1.tripDuration, r2.tripDuration);
        }
        return cmp;
    }
        
    public boolean isEmpty() {
        return size == 0;
    }
    public void remove(Ride ride) {
        int index = indexOf(ride);
        if (index == -1) {
            return;
        }
        heap[index] = heap[size - 1];
        size--;
        heapifyDown(index);
    }

    private int indexOf(Ride ride) {
        for (int i = 0; i < size; i++) {
            if (heap[i].equals(ride)) {
                return i;
            }
        }
        return -1;
    }

}