typedef struct Fragment_Entry Fragment_Entry;
typedef struct Fragment_Pool Fragment_Pool;

struct Fragment_Entry {
  b32 is_fragment;
  u32 data_length;
  u8 *data;
};

struct Fragment_Pool {
  i32 capacity;
  u32 packet_length;

  i32 sequence_base;
  std::vector<Fragment_Entry> fragments;
  u32 fragments_count;

  std::vector<char> buffer;
  u32 buffer_tail;
  u32 buffer_target_length;
};

internal Fragment_Pool fragment_pool_create(u32 capacity, u32 packet_length,
                                            Arena *arena) {
  Fragment_Pool result;
  result.capacity = capacity;
  result.packet_length = packet_length;
  result.fragments.resize(capacity);
  result.buffer.resize(packet_length * capacity);

  return result;
}

internal void fragment_pool_insert(Fragment_Pool *pool, int32_t sequence,
                                   uint8_t *data, uint32_t data_length,
                                   bool is_fragment) {
  if (sequence < pool->sequence_base) {
    printf("[!!!] Attempting to insert older sequence (%d), rejecting\n",
           sequence);
    return;
  }

  int32_t index = sequence - pool->sequence_base;

  printf("[***] fragment_pool_insert: sequence=%d, data=%p, data_length=%d, "
         "is_fragment=%d, index=%d\n",
         sequence, data, data_length, is_fragment, index);

  ASSERT_MSG(index < pool->capacity, "exceeds pool capacity");

  ASSERT_MSG(data_length < pool->packet_length, "exceeds packet length");

  ASSERT_MSG(!pool->fragments[index].data_length, "already in use");

  if (index == 0 && is_fragment) {
    pool->buffer_target_length = endian_read_u32_big(data);
    printf("[***] Fragment_Pool target length updated: %d\n",
           pool->buffer_target_length);
    data_length -= 4;
    data += 4;
  }

  pool->fragments[index].is_fragment = is_fragment;
  pool->fragments[index].data_length = data_length;
  if (is_fragment) {
    // Overlap fragment boundaries
    pool->fragments[index].data = reinterpret_cast<u8 *>(
        reinterpret_cast<Fragment_Entry *>(pool->buffer.data()) +
        pool->buffer_tail);
    pool->buffer_tail += data_length;
  } else {
    // Align to fragment boundaries
    pool->fragments[index].data = reinterpret_cast<u8 *>(
        reinterpret_cast<Fragment_Entry *>(pool->buffer.data()) +
        (index * pool->packet_length));
    pool->buffer_tail += (index + 1) * pool->packet_length;
  }

  std::memcpy(pool->fragments[index].data, data, data_length);
  pool->fragments_count++;
}

internal void fragment_pool_advance(Fragment_Pool *pool) {
  pool->sequence_base += pool->fragments_count;
  printf("[***] Advancing Fragment_Pool; New base: %d\n", pool->sequence_base);
  pool->buffer_target_length = 0;

  // for (u32 i = 0; i < pool->buffer_tail; i++)
  //     {
  //     pool->buffer[i] = 0;
  //     }
  pool->buffer_tail = 0;

  for (u32 i = 0; i < pool->fragments_count; i++) {
    pool->fragments[i].data = 0;
    pool->fragments[i].data_length = 0;
    pool->fragments[i].is_fragment = 0;
  }
  pool->fragments_count = 0;
}
