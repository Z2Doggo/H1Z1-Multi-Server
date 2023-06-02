typedef struct Packet_Queue_Entry Packet_Queue_Entry;
struct Packet_Queue_Entry {
  i32 buffer_offset;
  i32 length;
  void *session_state;
};

#define PACKET_QUEUE_SEND(name)                                                \
  void name(void *server_state, void *session_state, u8 *base_buffer,          \
            i32 total_length)
typedef PACKET_QUEUE_SEND(packet_queue_send_t);

typedef struct Packet_Queue Packet_Queue;
struct Packet_Queue {
  std::vector<Packet_Queue_Entry> *entries;
  i32 entries_tail;
  i32 entries_capacity;

  std::vector<char> buffer;
  i32 buffer_tail;
  i32 buffer_capacity;

  packet_queue_send_t *packet_queue_send;
};

internal Packet_Queue
packet_queue_create(Arena *arena, i32 entries_capacity, i32 buffer_capacity,
                    packet_queue_send_t *packet_queue_send_func) {
  Packet_Queue result = {0};

  result.entries = new std::vector<Packet_Queue_Entry>(entries_capacity);
  result.entries_capacity = entries_capacity;

  result.buffer.resize(buffer_capacity);
  result.buffer_capacity = buffer_capacity;

  result.packet_queue_send = packet_queue_send_func;

  return result;
}

internal void packet_queue_push(Packet_Queue *queue, void *session_state,
                                // i32 max_packed_length,
                                // i32 packet_kind,
                                u8 *data, i32 data_length) {
  ASSERT_MSG(queue->entries_tail != queue->entries_capacity,
             "Packet Queue entries capacity exceeded");

  ASSERT_MSG(!(queue->buffer_tail + data_length > queue->buffer_capacity),
             "Packet Queue buffer capacity exceeded");

  Packet_Queue_Entry *entry = &(*queue->entries)[queue->entries_tail];
  entry->buffer_offset = queue->buffer_tail;
  entry->length = data_length;
  entry->session_state = session_state;
  queue->entries_tail += 1;

  base_memory_copy(&queue->buffer[queue->buffer_tail], data, data_length);
  queue->buffer_tail += data_length;
}

internal void packet_queue_pop_and_send(Packet_Queue *queue,
                                        void *server_state) {
  ASSERT_MSG(queue->entries_tail, "Nothing to pop from Packet Queue");
  ASSERT_MSG(queue->buffer_tail, "No data in Packet Queue buffer");

  Packet_Queue_Entry *entry = &(*queue->entries)[queue->entries_tail - 1];

  queue->packet_queue_send(
      server_state, entry->session_state,
      reinterpret_cast<u8 *>(&queue->buffer[entry->buffer_offset]),
      entry->length);

  base_memory_fill(&queue->buffer[entry->buffer_offset], 0, entry->length);

  queue->buffer_tail -= entry->length;
  queue->entries_tail -= 1;
  *entry = Packet_Queue_Entry{}; // Zero-initialize the structure
}
