#include "login_udp_11_kotk.hpp"

typedef struct Login_Packet_LoginRequest Login_Packet_LoginRequest;
struct Login_Packet_LoginRequest {
  u32 session_id_length;
  string session_id;
  u32 system_fingerprint_length;
  string system_fingerprint;
  u32 locale;
  u32 third_party_auth_ticket;
  u32 third_party_user_id;
  u32 third_party_id;
};

typedef struct Login_Packet_LoginReply Login_Packet_LoginReply;
struct Login_Packet_LoginReply {
  bool is_logged_in;
  u32 status;
  u32 result_code;
  bool is_member;
  bool is_internal;
  u32 namespace_name_length;
  string namespace_name;
  u32 account_features_count;
  struct account_features_s {
    u32 key;
    u32 id;
    bool active;
    u32 remaining_count;
    u32 raw_data_length;
    u8 *raw_data;
  } *account_features;
  u32 application_payload_length;
  u8 *application_payload;
  u32 error_details_count;
  struct error_details_s {
    u32 unknown_dword1;
    u32 name_length;
    string name;
    u32 value_length;
    string value;
  } *error_details;
  u32 ip_country_code_length;
  string ip_country_code;
};

typedef struct Login_Packet_CharacterCreateRequest
    Login_Packet_CharacterCreateRequest;
struct Login_Packet_CharacterCreateRequest {
  u32 server_id;
  u32 unk_u32;
  u32 char_payload_length;
  struct char_payload_s {
    bool empire_id;
    u32 head_type;
    u32 profile_type;
    u32 gender;
    u32 character_name_length;
    string character_name;
  } *char_payload;
};

typedef struct Login_Packet_CharacterCreateReply
    Login_Packet_CharacterCreateReply;
struct Login_Packet_CharacterCreateReply {
  u32 status;
  u64 character_id;
};

typedef struct Login_Packet_CharacterLoginRequest
    Login_Packet_CharacterLoginRequest;
struct Login_Packet_CharacterLoginRequest {
  u64 character_id;
  u32 server_id;
  u32 status;
  u32 payload2_length;
  struct payload2_s {
    u32 locale_length;
    string locale;
    u32 locale_id;
    u32 preferred_gateway_id;
  } *payload2;
};

typedef struct Login_Packet_CharacterLoginReply
    Login_Packet_CharacterLoginReply;
struct Login_Packet_CharacterLoginReply {
  u64 character_id;
  u32 server_id;
  u32 last_login;
  u32 status;
  u32 login_payload_length;
  struct login_payload_s {
    u32 server_address_length;
    string server_address;
    u32 server_ticket_length;
    string server_ticket;
    u32 encryption_key_length;
    u8 *encryption_key;
    u32 soe_protocol_version;
    u64 character_id;
    u64 unk_u64;
    u32 station_name_length;
    string station_name;
    u32 character_name_length;
    string character_name;
    u32 unk_str_length;
    string unk_str;
    u64 server_feature_bit;
  } *login_payload;
};

typedef struct Login_Packet_CharacterDeleteRequest
    Login_Packet_CharacterDeleteRequest;
struct Login_Packet_CharacterDeleteRequest {
  u64 character_id;
};

typedef struct Login_Packet_TunnelAppPacketClientToServer
    Login_Packet_TunnelAppPacketClientToServer;
struct Login_Packet_TunnelAppPacketClientToServer {
  u32 server_id;
  u32 unk1;
  u32 data_client_length;
  struct data_client_s {
    bool tunnel_op_code;
    bool sub_op_code;
    u32 character_name_length;
    string character_name;
  } *data_client;
};

typedef struct Login_Packet_TunnelAppPacketServerToClient
    Login_Packet_TunnelAppPacketServerToClient;
struct Login_Packet_TunnelAppPacketServerToClient {
  u32 server_id;
  u32 unk1;
  u32 data_server_length;
  struct data_server_s {
    bool tunnel_op_code;
    bool sub_op_code;
    u32 character_name_length;
    string character_name;
    u32 character_name2_length;
    string character_name2;
    u32 status;
  } *data_server;
};

typedef struct Login_Packet_CharacterDeleteReply
    Login_Packet_CharacterDeleteReply;
struct Login_Packet_CharacterDeleteReply {
  u64 character_id;
  u32 status;
  u32 payload3_length;
  u8 *payload3;
};

typedef struct Login_Packet_CharacterSelectInfoReply
    Login_Packet_CharacterSelectInfoReply;
struct Login_Packet_CharacterSelectInfoReply {
  u32 character_status;
  bool can_bypass_server_lock;
  u32 characters_count;
  struct characters_s {
    u64 character_id;
    u32 server_id;
    u64 last_login_date;
    u32 null_field;
    u32 status;
    u32 payload4_length;
    struct payload4_s {
      u32 name_length;
      string name;
      u8 empire_id;
      u32 battle_rank;
      u32 next_battle_rank_percent;
      u32 head_id;
      u32 model_id;
      u32 gender;
      u32 profile_id;
      u32 unknown_dword;
      u32 loadout_id;
      u32 unknown_dword_1;
      u8 unknown_byte_1;
      u32 unknown_dword_2;
      u32 unknown_dword_3;
      u32 loadout_name_length;
      string loadout_name;
      u32 tint_item_id;
      u32 unknown_dword_4;
      u32 decal_item_id;
      u32 loadout_slots_count;
      struct loadout_slots_s {
        u32 slot_id;
        u32 index;
        u32 item_line_id;
        u8 flags;
        u32 attachments_count;
        struct attachments_s {
          u32 attachment_id;
        } *attachments;
        u32 attachment_classes_count;
        struct attachment_classes_s {
          u32 class_id;
          u32 attachment_id;
        } *attachment_classes;
        u32 tint_item_id;
        u32 item_slot;
      } *loadout_slots;
      u32 item_definitions_count;
      struct item_definitions_s {
        u32 item_id;
        u32 item_id_2;
        u8 flags_1;
        u8 flags_2;
        u32 name_id;
        u32 description_id;
        u32 unknown_dword_1;
        u32 icon_id;
        u32 unknown_dword_2;
        u32 hudImageSetId;
        u32 hud_image_set_id;
        u32 unknown_dword_4;
        u32 cost;
        u32 item_class;
        u32 unknown_dword_5;
        u32 item_slot;
        u32 slot_override_key;
        u8 unknown_dword6;
        u32 model_name_length;
        string model_name;
        u32 unknown_string_length;
        string unknown_string;
        u8 unknown_byte_1;
        u32 item_type;
        u32 category_id;
        u32 unknown_dword_7;
        u32 unknown_dword_8;
        u32 unknown_dword_9;
        u32 unknown_dword_10;
        u32 unknown_dword_11;
        u32 activatable_ability_id;
        u32 passive_ability_id;
        u32 unknown_dword_12;
        u32 max_stack_size;
        u32 tint_name_length;
        string tint_name;
        u32 unknown_dword_13;
        u32 unknown_dword_14;
        u32 unknown_dword_15;
        u32 unknown_dword_16;
        u32 ui_model_camera;
        u32 equip_count_max;
        u32 currency_type;
        u32 unknown_dword_17;
        u32 client_item_type;
        u32 skill_set_id;
        u32 overlay_texture_length;
        string overlay_texture;
        u32 decal_slot_length;
        string decal_slot;
        u32 unknown_dword_18;
        u32 trial_duration_sec;
        u32 trial_exclusion_sec;
        u32 client_use_requirement_id;
        u32 override_appearance_length;
        string override_appearance;
        u32 unknown_dword_19;
        u32 client_use_requirement_id_2;
      } *item_definitions;
      u32 attachment_definitions_count;
      struct attachment_definitions_s {
        u32 attachment_id;
        u32 attachment_id_2;
        u32 group_id;
        u32 item_line_id;
        u32 flags;
        u32 classes_count;
        struct classes_s {
          u32 element_type;
        } *classes;
      } *attachment_definitions;
      u64 last_use_date;
    } *payload4;
  } *characters;
};

typedef struct Login_Packet_ServerListReply Login_Packet_ServerListReply;
struct Login_Packet_ServerListReply {
  u32 servers_count;
  struct servers_s {
    u32 id;
    u32 state;
    bool is_locked;
    u32 name_length;
    string name;
    u32 name_id;
    u32 description_length;
    string description;
    u32 description_id;
    u32 req_feature_id;
    u32 server_info_length;
    string server_info;
    u32 population_level;
    u32 population_data_length;
    string population_data;
    u32 access_expression_length;
    string access_expression;
    bool is_access_allowed;
  } *servers;
};

// TODO: write a packet pack and unpack func
int main() {
  string test = "yes very much so!";
  u32 length = test.length();
}
