import heapq
import time

class StreamingTwoHeapStats:
    def __init__(self):
        self.max_heap = []  # Simulated max-heap via value negation
        self.min_heap = []  # Standard min-heap

    def insert(self, num: float):
        start_time = time.perf_counter()

        if not self.max_heap or num <= -self.max_heap[0]:
            heapq.heappush(self.max_heap, -num)
        else:
            heapq.heappush(self.min_heap, num)

        if len(self.max_heap) > len(self.min_heap) + 1:
            val = -heapq.heappop(self.max_heap)
            heapq.heappush(self.min_heap, val)
        elif len(self.min_heap) > len(self.max_heap):
            val = heapq.heappop(self.min_heap)
            heapq.heappush(self.max_heap, -val)

        if len(self.max_heap) == len(self.min_heap):
            running_val = (-self.max_heap[0] + self.min_heap[0]) / 2.0
        else:
            running_val = -self.max_heap[0]

        elapsed_ms = (time.perf_counter() - start_time) * 1000.0
        return running_val, elapsed_ms

if __name__ == "__main__":
    stream = [5.0, 15.0, 1.0, 3.0, 8.0, 9.0, 10.0, 12.0, 14.0, 20.0, 2.0, 4.0, 6.0]
    processor = StreamingTwoHeapStats()
    for val in stream:
        res, t_ms = processor.insert(val)
        print(f"Value: {val:4.1f} | Median: {res:5.1f} | Time: {t_ms:6.4f} ms")