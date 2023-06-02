#include <unordered_map>

#define LOGIN_LOGINREQUEST_ID 0x1
#define LOGIN_LOGINREPLY_ID 0x2
#define LOGIN_LOGOUT_ID 0x3
#define LOGIN_FORCEDISCONNECT_ID 0x4
#define LOGIN_CHARACTERCREATEREQUEST_ID 0x5
#define LOGIN_CHARACTERCREATEREPLY_ID 0x6
#define LOGIN_CHARACTERLOGINREQUEST_ID 0x7
#define LOGIN_CHARACTERLOGINREPLY_ID 0x8
#define LOGIN_CHARACTERDELETEREQUEST_ID 0x9
#define LOGIN_TUNNELAPPPACKETCLIENTTOSERVER_ID 0x10
#define LOGIN_TUNNELAPPPACKETSERVERTOCLIENT_ID 0x11
#define LOGIN_CHARACTERTRANSFERREQUEST_ID 0x12
#define LOGIN_CHARACTERTRANSFERREPLY_ID 0x13
#define LOGIN_CHARACTERDELETEREPLY_ID 0xa
#define LOGIN_CHARACTERSELECTINFOREQUEST_ID 0xb
#define LOGIN_CHARACTERSELECTINFOREPLY_ID 0xc
#define LOGIN_SERVERLISTREQUEST_ID 0xd
#define LOGIN_SERVERLISTREPLY_ID 0xe
#define LOGIN_SERVERUPDATE_ID 0xf

#define LOGIN_PACKET_KINDS                                                     \
  LOGIN_PACKET_KIND(Login_Packet_Kind_Unhandled, "Unhandled"),                 \
      LOGIN_PACKET_KIND(Login_Packet_Kind_LoginRequest, "LoginRequest"),       \
      LOGIN_PACKET_KIND(Login_Packet_Kind_LoginReply, "LoginReply"),           \
      LOGIN_PACKET_KIND(Login_Packet_Kind_Logout, "Logout"),                   \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ForceDisconnect, "ForceDisconnect"), \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterCreateRequest,              \
                        "CharacterCreateRequest"),                             \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterCreateReply,                \
                        "CharacterCreateReply"),                               \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterLoginRequest,               \
                        "CharacterLoginRequest"),                              \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterLoginReply,                 \
                        "CharacterLoginReply"),                                \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterDeleteRequest,              \
                        "CharacterDeleteRequest"),                             \
      LOGIN_PACKET_KIND(Login_Packet_Kind_TunnelAppPacketClientToServer,       \
                        "TunnelAppPacketClientToServer"),                      \
      LOGIN_PACKET_KIND(Login_Packet_Kind_TunnelAppPacketServerToClient,       \
                        "TunnelAppPacketServerToClient"),                      \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterTransferRequest,            \
                        "CharacterTransferRequest"),                           \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterTransferReply,              \
                        "CharacterTransferReply"),                             \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterDeleteReply,                \
                        "CharacterDeleteReply"),                               \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterSelectInfoRequest,          \
                        "CharacterSelectInfoRequest"),                         \
      LOGIN_PACKET_KIND(Login_Packet_Kind_CharacterSelectInfoReply,            \
                        "CharacterSelectInfoReply"),                           \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ServerListRequest,                   \
                        "ServerListRequest"),                                  \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ServerListReply, "ServerListReply"), \
      LOGIN_PACKET_KIND(Login_Packet_Kind_ServerUpdate, "ServerUpdate"),       \
      LOGIN_PACKET_KIND(Login_Packet_Kind__End, "")

enum class Login_Packet_Kind {
#define LOGIN_PACKET_KIND(e, s) e
  LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
};

string login_packet_names[static_cast<int>(
                              Login_Packet_Kind::Login_Packet_Kind__End) +
                          1] = {
#define LOGIN_PACKET_KIND(e, s) s
    LOGIN_PACKET_KINDS
#undef LOGIN_PACKET_KIND
};

std::unordered_map<Login_Packet_Kind, u32> login_registered_ids = {
    {Login_Packet_Kind::Login_Packet_Kind_LoginRequest, 0x1},
    {Login_Packet_Kind::Login_Packet_Kind_LoginReply, 0x2},
    {Login_Packet_Kind::Login_Packet_Kind_Logout, 0x3},
    {Login_Packet_Kind::Login_Packet_Kind_ForceDisconnect, 0x4},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterCreateRequest, 0x5},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterCreateReply, 0x6},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterLoginRequest, 0x7},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterLoginReply, 0x8},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterDeleteRequest, 0x9},
    {Login_Packet_Kind::Login_Packet_Kind_TunnelAppPacketClientToServer, 0x10},
    {Login_Packet_Kind::Login_Packet_Kind_TunnelAppPacketServerToClient, 0x11},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterTransferRequest, 0x12},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterTransferReply, 0x13},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterDeleteReply, 0xa},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterSelectInfoRequest, 0xb},
    {Login_Packet_Kind::Login_Packet_Kind_CharacterSelectInfoReply, 0xc},
    {Login_Packet_Kind::Login_Packet_Kind_ServerListRequest, 0xd},
    {Login_Packet_Kind::Login_Packet_Kind_ServerListReply, 0xe},
    {Login_Packet_Kind::Login_Packet_Kind_ServerUpdate, 0xf}};
