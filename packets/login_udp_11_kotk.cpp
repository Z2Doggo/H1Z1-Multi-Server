#include "login_udp_11_kotk.hpp"

struct Login_Packet_LoginRequest {
  u32 session_id_length;
  str session_id;
  u32 system_fingerprint_length;
  str system_fingerprint;
  u32 locale;
  u32 third_party_auth_ticket;
  u32 third_party_user_id;
  u32 third_party_id;
};

struct Login_Packet_LoginReply {
  bool is_logged_in;
  u32 status;
  u32 result_code;
  bool is_member;
  bool is_internal;
  u32 namespace_name_length;
  str namespace_name;
  u32 account_features_count;
  struct account_features_s {
    u32 key;
    u32 id;
    bool active;
    u32 remaining_count;
    u32 raw_data_length;
    u8 * raw_data;
  }* account_features;
  u32 application_payload_length;
  u8 * application_payload;
  u32 error_details_count;
  struct error_details_s {
    u32 unknown_dword1;
    u32 name_length;
    str name;
    u32 value_length;
    str value;
  }* error_details;
  u32 ip_country_code_length;
  str ip_country_code;
};