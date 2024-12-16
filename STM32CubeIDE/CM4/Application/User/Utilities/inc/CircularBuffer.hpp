//#include <vector>
//#include <stdexcept>
//#include <type_traits>
//#include <iostream>
//
//template <typename T, typename = typename std::enable_if<std::is_fundamental<T>::value>::type>
//class CircularBuffer {
//public:
//    explicit CircularBuffer(size_t capacity)
//        : buffer_(capacity), capacity_(capacity), head_(0), tail_(0), size_(0) {}
//
//    // Dodaj pojedynczy element (nadpisuje najstarsze dane, jeśli bufor jest pełny)
//    void push(const T& item) {
//        buffer_[head_] = item;
//        head_ = (head_ + 1) % capacity_;
//        if (size_ < capacity_) {
//            ++size_;
//        } else {
//            tail_ = (tail_ + 1) % capacity_;  // Przesuwamy ogon, bo nadpisujemy najstarszy element
//        }
//    }
//
//    // Dodaj wiele elementów (nadpisuje najstarsze dane w razie potrzeby)
//    void push(const T* items, size_t count) {
//        if (count > capacity_) {
//            // Jeśli próbujemy zapisać więcej danych niż wynosi pojemność bufora,
//            // przechowujemy tylko ostatnie `capacity_` elementów.
//            items += count - capacity_;
//            count = capacity_;
//        }
//
//        if (capacity_ - head_ >= count) {
//            // Jeśli jest wystarczająco miejsca od `head_` do końca bufora
//            std::copy(items, items + count, buffer_.begin() + head_);
//            head_ = (head_ + count) % capacity_;
//        } else {
//            // Jeśli dane nie mieszczą się w jednym ciągłym bloku
//            size_t first_part = capacity_ - head_;
//            std::copy(items, items + first_part, buffer_.begin() + head_);
//            std::copy(items + first_part, items + count, buffer_.begin());
//            head_ = count - first_part;
//        }
//
//        // Zaktualizuj rozmiar i przesuwaj `tail_`, jeśli nadpisujemy
//        if (size_ + count > capacity_) {
//            tail_ = (head_ + capacity_ - size_) % capacity_;
//            size_ = capacity_;
//        } else {
//            size_ += count;
//        }
//    }
//
//    // Odczytaj pojedynczy element
//    T pop() {
//        if (size_ == 0) {
//            throw std::underflow_error("Buffer is empty");
//        }
//        T item = buffer_[tail_];
//        tail_ = (tail_ + 1) % capacity_;
//        --size_;
//        return item;
//    }
//
//    // Odczytaj wiele elementów
//    size_t pop(T* items, size_t count) {
//        size_t num_to_read = std::min(count, size_);
//        if (capacity_ - tail_ >= num_to_read) {
//            // Jeśli dane są w jednym ciągłym bloku
//            std::copy(buffer_.begin() + tail_, buffer_.begin() + tail_ + num_to_read, items);
//        } else {
//            // Jeśli dane są podzielone na dwie części
//            size_t first_part = capacity_ - tail_;
//            std::copy(buffer_.begin() + tail_, buffer_.end(), items);
//            std::copy(buffer_.begin(), buffer_.begin() + (num_to_read - first_part), items + first_part);
//        }
//
//        tail_ = (tail_ + num_to_read) % capacity_;
//        size_ -= num_to_read;
//        return num_to_read;
//    }
//
//    // Sprawdź, czy bufor jest pusty
//    bool empty() const { return size_ == 0; }
//
//    // Sprawdź, czy bufor jest pełny
//    bool full() const { return size_ == capacity_; }
//
//    // Rozmiar aktualnie zajętych elementów
//    size_t size() const { return size_; }
//
//    // Całkowita pojemność bufora
//    size_t capacity() const { return capacity_; }
//
//private:
//    std::vector<T> buffer_;
//    size_t capacity_;
//    size_t head_;
//    size_t tail_;
//    size_t size_;
//};
