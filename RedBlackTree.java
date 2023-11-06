package ride;

class RedBlackTree {
    private Node root;

    private static final boolean RED = true;
    private static final boolean BLACK = false;

    private class Node {
        Ride ride;
        Node left, right;
        boolean color;
        int size;

        Node(Ride ride, boolean color) {
            this.ride = ride;
            this.color = color;
            this.size=1;
        }
        public void setSize() {
            this.size = size(left) + size(right) + 1;
        }
        public Ride getValue() {
            return ride;
        }

        public Node getRight() {
            return right;
        }
        public Node getLeft() {
            return left;
        }
    }

    private boolean isRed(Node x) {
        if (x == null) {
            return false;
        }
        return x.color == RED;
    }

    private Node rotateLeft(Node h) {
        Node x = h.right;
        h.right = x.left;
        x.left = h;
        x.color = h.color;
        h.color = RED;
        return x;
    }

    private Node rotateRight(Node h) {
        Node x = h.left;
        h.left = x.right;
        x.right = h;
        x.color = h.color;
        h.color = RED;
        return x;
    }

    private void flipColors(Node h) {
        h.color = RED;
        h.left.color = BLACK;
        h.right.color = BLACK;
    }

    public void insert(Ride ride) {
        root = insert(root, ride);
        root.color = BLACK;
    }

    private Node insert(Node h, Ride ride) {
        if (h == null) {
            return new Node(ride, RED);
        }

        int cmp = h.ride.rideNumber - ride.rideNumber;
        if (cmp > 0) {
            h.left = insert(h.left, ride);
        } else if (cmp < 0) {
            h.right = insert(h.right, ride);
        } else {
            // update ride information
            h.ride = ride;
        }

        if (isRed(h.right) && !isRed(h.left)) {
            h = rotateLeft(h);
        }
        if (isRed(h.left) && isRed(h.left.left)) {
            h = rotateRight(h);
        }
        if (isRed(h.left) && isRed(h.right)) {
            flipColors(h);
        }

        return h;
    }

    public void delete(Ride ride) {
        root = delete(root, ride);
    }

    private Node delete(Node h, Ride ride) {
        if (h == null) {
            return null;
        }

        int cmp = h.ride.rideNumber - ride.rideNumber;
        if (cmp > 0) {
            h.left = delete(h.left, ride);
        } else if (cmp < 0) {
            h.right = delete(h.right, ride);
        } else {
            if (h.left == null) {
                return h.right;
            }
            if (h.right == null) {
                return h.left;
            }

            Node min = min(h.right);
            h.ride = min.ride;
            h.right = deleteMin(h.right);
        }

        if (isRed(h.right) && !isRed(h.left)) {
            h = rotateLeft(h);
        }
        if (isRed(h.left) && isRed(h.left.left)) {
            h = rotateRight(h);
        }
        if (isRed(h.left) && isRed(h.right)) {
            flipColors(h);
        }

        return h;
    }

    private Node min(Node h) {
        if (h.left == null) {
            return h;
        }
        return min(h.left);
    }

    private Node deleteMin(Node h) {
        if (h.left == null) {
            return null;
        }

        if (!isRed(h.left) && !isRed(h.left.left)) {
            h = moveRedLeft(h);
        }

        h.left = deleteMin(h.left);
        return balance(h);
    }
    /**
     * Helper method that moves a red node to the left child of its right child
     * @param h the root of the sub-tree to be modified
     * @return the new root of the sub-tree after the modification
     */
    private Node moveRedLeft(Node h) {
        flipColors(h); // Flip the colors of h and its two children
        if (isRed(h.right.left)) { // Check if the grandchild is a red node
            h.right = rotateRight(h.right); // Perform a right rotation on h's right child
            h = rotateLeft(h); // Perform a left rotation on h
            flipColors(h); // Flip the colors of h and its two children again
        }
        return h;
    }
    private Node balance(Node h) {
        if (isRed(h.right)) {
            h = rotateLeft(h);
        }
        if (isRed(h.left) && isRed(h.left.left)) {
            h = rotateRight(h);
        }
        if (isRed(h.left) && isRed(h.right)) {
            flipColors(h);
        }
        h.size = size(h.left) + size(h.right) + 1;
        return h;
    }
    public int size() {
        return size(root);
    }

    private int size(Node x) {
        if (x == null) {
            return 0;
        }
        return x.size;
    }
    public Ride search(int rideNumber) {
        Node current = root;
        while (current != null) {
            if (current.ride.getRideNumber() == rideNumber) {
                return current.ride;
            } else if (current.ride.getRideNumber() > rideNumber) {
                current = current.left;
            } else {
                current = current.right;
            }
        }
        return null;
    }

    public List<Ride> rangeSearch(int low, int high) {
        List<Ride> result = new ArrayList<>();
        rangeSearch(root, low, high, result);
        return result;
    }

    private void rangeSearch(Node node, int low, int high, List<Ride> result) {
        if (node == null) {
            return;
        }
        if (node.getValue().getRideNumber() >= low && node.getValue().getRideNumber() <= high) {
            result.add(node.getValue());
        }
        if (node.getValue().getRideNumber() >= low) {
            rangeSearch(node.getLeft(), low, high, result);
        }
        if (node.getValue().getRideNumber() <= high) {
            rangeSearch(node.getRight(), low, high, result);
        }
    }
}