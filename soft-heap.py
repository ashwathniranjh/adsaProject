# Python translation of Chazelle's paper

import math 

class ItemListCell:
    def __init__(self):
        self.key = None
        self.next = None # Points to ItemListCell

class Node:
    def __init__(self):
        self.ckey = None
        self.rank = None
        self.next = None # Points to next Node in the rank-1 tree
        self.child = None # Points to child rank-1 tree
        self.il = None # Points to beginning of item list
        self.il_tail = None # Points to end of item list

class Head:
    def __init__(self):
        self.queue = None # Points to the head's queue
        self.next = None # Points to the next Head in the head list
        self.prev = None # Points the previous Head in the head list
        self.suffix_min = None # Points to the Head with the queue of minimum ckey
        self.rank = None # Rank of the queue

class SoftHeap:
    # Initialize the heap
    # head -> tail
    #         (inf)
    def __init__(self, r):
        self.header = Head()
        self.tail = Head()
        self.tail.rank = float("inf")
        self.header.next = self.tail
        self.tail.prev = self.header
        self.r = 2 + 2 * math.ceil(math.log(1/eps))

    # Insertion
    def insert(self, new_key):
        # Make a new queue
        q = Node();
        l = ItemListCell();
        l.key = new_key; l.next = None
        q.rank = 0; q.ckey = new_key
        q.il = l; q.il_tail = l
        # And meld it into the head list
        self.meld(q)
    
    def meld(self, q):
        h = prev_head = to_head = self.header.next
        # Move up to_head until has rank >= q.rank
        while q.rank > to_head.rank:
            to_head = to_head.next
        prev_head = to_head.prev
        # head ->..-> prev_head -> to_head ->..-> tail
        #             rank <k      rank=k
        #                       ^ q.rank <=k
        # If to_head has the same rank as q, perform the carry propogation
        while q.rank == to_head.rank:
        # head ->..-> prev_head -> to_head ->..-> tail
        #             rank <k      rank=k
        #                       ^ q.rank==k
            # Let top be the queue with lower ckey
            if to_head.queue.ckey > q.ckey:
                top = q
                bottom = to_head.queue
            else:
                top = to_head.queue
                bottom = q
            # Modify q so it's the combination of q and to_head, using the top
            # and bottom we specified above
            q = Node()
            q.ckey = top.ckey
            q.rank = top.rank + 1
            q.child = bottom; q.next = top
            q.il = top.il; q.il_tail = top.il_tail
            to_head = to_head.next
            # q's rank has increased by 1, but it's possible that to_head still 
            # has the same rank because it's moved forward; in that case, continue
            # the while loop
        
        if prev_head == to_head.prev: # No carry was performed
            h = Head()
        else:
            h = prev_head.next
        # Wire in q
        # head ->..-> prev_head -> q  -> (..) to_head ->..-> tail
        #             rank <k      rank=k     rank > k
        h.queue = q
        h.rank = q.rank
        h.prev = prev_head; h.next = to_head
        prev_head.next = h; to_head.prev = h
        # q is in the right place now (h), but must fix suffix_min's for h onwards
        self.fix_minlist(h)
    
    def fix_minlist(self, h):
        tmp_min = None
        # If h is the last real queue, it is it's own suffix min
        if h.next == self.tail:
            tmp_min = h
        # Else, grab the suffix min of the head after it
        else:
            tmp_min = h.next.suffix_min
        # Walk backwards and set everyone's suffix min correctly;
        # tmp_min always holds, or is changed to hold, the cumulative min
        # from the right
        while h != self.header:
            if h.queue.ckey < tmp_min.queue.ckey:
                tmpmin = h
            h.suffix_min = tmp_min
            h = h.prev
    
    def sift(self, v):
        # Empty out the item list
        v.il = None
        v.il_tail = None
        if v.next == None and v.child == None:
            # v is a leaf; stay at the bottom of the queue
            v.ckey = float("inf")
            return v
        v.next = self.sift(v.next)
        # If we violate the heap ordering, perform a rotation
        if v.next.ckey > v.child.ckey:
            tmp = v.child
            v.child = v.next
            v.next = tmp
        # Pass along the pointers of v.next to to v
        v.il = v.next.il
        v.il_tail = v.next.il_tail
        v.ckey = v.next.ckey
        # Possibly sift again if the rank of v is large and odd
        if (v.rank > self.r and v.rank % 2 == 1) or (v.child.rank < v.rank-1):
            v.next = self.sift(v.next)
            # If we violate heap ordering, perform a rotation
            if v.next.ckey > v.child.ckey:
                tmp = v.child
                v.child = v.next
                v.next = tmp
            # Concatenate the item list of v.next to that of v unless it is
            # empty or undefined (which could happen if ckey is inf in v.child
            # and v.next)
            if v.next.ckey != float("inf") and v.next.il != None:
                v.next.il_tail.next = v.il
                v.il = v.next.il
                if v.il_tail is None:
                    v.il_tail = v.next.il_tail
                v.ckey = v.next.ckey
        # Remove nodes with infinite ckeys
        # After this, it's possible that rank and number of children are different
        if v.child.ckey == float("inf"):
            if (v.next.ckey == float("inf")):
                v.child = None
                v.next = None
            else:
                v.child = v.next.child
                v.next = v.next.next
        return v

    def delete_min(self):
        h = self.header.next.suffix_min
        # If that suffix min has an item list that's empty, we need to 
        # sift up item lists into it, and then do the deletion from
        # that item list
        while h.queue.il == None:
            # Before we sift, we have to check whether the rank invariant
            # is violated (that is, rank(root)/2 <= #children <= rank(root)).
            # (It could be violated because of previous sifting.)
            # So, count the children of h.queue
            tmp = h.queue
            childcount = 0
            while tmp.next != None:
                tmp =  tmp.next
                childcount += 1
            # Check if we're lower than the lower bound
            if (childcount < h.rank/2):
                # Remove the min
                h.prev.next = h.next
                h.next.prev = h.prev
                # Fixup suffix mins
                self.fix_minlist(h.prev)
                # Meld in the children
                tmp = h.queue
                while tmp.next is not None:
                    self.meld(tmp.child)
                    tmp = tmp.next
            else:
                h.queue = self.sift(h.queue)
                # Get rid of infinity ckeys
                if h.queue.ckey == float('inf'):
                    h.prev.next = h.next
                    h.next.prev = h.prev
                    h = h.prev
                self.fix_minlist(h)
        # Now we can get the minimum key and delete one item from the 
        # corresponding item list
        min = h.queue.il.key
        h.queue.il = h.queue.il.next
        # Fixup if we deleted the last node in the item list
        if h.queue.il == None:
            h.queue.il_tail = None
        return min

# Note - this doesn't work right currently - I haven't take the time
# to debug it yet, but it's very close to the original papef so should
# be pretty close to working

eps = 1e-3
sheap = SoftHeap(eps)
for i in range(10):
    sheap.insert(i)
x = sheap.delete_min()
print(x)
x = sheap.delete_min()
print(x)
x = sheap.delete_min()
print(x)
x = sheap.delete_min()
print(x)
x = sheap.delete_min()
print(x)

sheap1 = SoftHeap(eps)
for i in range(11,15):
    sheap1.insert(i)

x = sheap1.delete_min()
print(x)