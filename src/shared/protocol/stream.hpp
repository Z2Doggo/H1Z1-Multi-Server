typedef struct Input_Stream Input_Stream;
typedef struct Output_Stream Output_Stream;

#define INPUT_STREAM_CALLBACK_ACK(name) void name(i32 ack, void *session)
typedef INPUT_STREAM_CALLBACK_ACK(input_stream_callback_ack);

#define INPUT_STREAM_CALLBACK_DATA(name)                                       \
  void name(void *server, void *session, u8 *data, u32 data_length)
typedef INPUT_STREAM_CALLBACK_DATA(input_stream_callback_data);

struct Input_Stream {
  Rc4_State rc4_state;

  input_stream_callback_ack **ack_callback_ptr;
  input_stream_callback_data **data_callback_ptr;

  i32 sequence_next;
  i32 ack_previous;
  i32 fragment_next;
  i32 processed_fragment_previous;
  b32 use_encryption;

  Fragment_Pool *fragment_pool;
};

#define OUTPUT_STREAM_CALLBACK_DATA(name)                                      \
  void name(void *server, void *session, u8 *data, u32 data_length,            \
            i32 sequence, b32 is_fragment)
typedef OUTPUT_STREAM_CALLBACK_DATA(output_stream_callback_data);

struct Output_Stream {
  Rc4_State rc4_state;

  output_stream_callback_data **data_callback_ptr;

  i32 sequence;
  i32 ack_previous;
  b32 use_encryption;

  Fragment_Pool *fragment_pool;
};
