#include "login_udp_11_kotk.hpp"

struct Login_Packet_LoginRequest {
  str session_id;
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
  str namespace_name;
  u32 account_features_count;
  struct account_features_s {
    u32 key;
    u32 id;
    bool active;
    u32 remaining_count;
    u8 * raw_data;
  }* account_features;
  u8 * application_payload;
  u32 error_details_count;
  struct error_details_s {
    u32 unknown_dword1;
    str name;
    str value;
  }* error_details;
  str ip_country_code;
};


// test func for testing purposes
int main() {
  str test = "yes very much so!";
  u32 length = test.length();

}