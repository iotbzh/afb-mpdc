
static const char _afb_description_v2_mpdc[] =
    "{\"openapi\":\"3.0.0\",\"$schema\":\"http:iot.bzh/download/openapi/schem"
    "a-3.0/default-schema.json\",\"info\":{\"description\":\"\",\"title\":\"c"
    "ontroller\",\"version\":\"1.0\",\"x-binding-c-generator\":{\"api\":\"mpd"
    "c\",\"version\":2,\"prefix\":\"mpdcapi_\",\"postfix\":\"\",\"start\":nul"
    "l,\"onevent\":\"mpdcDispatchEvent\",\"init\":\"mpdcBindingInit\",\"scope"
    "\":\"\",\"noconcurrency\":true,\"private\":false}},\"servers\":[{\"url\""
    ":\"ws://{host}:{port}/api/polctl\",\"description\":\"Music Player Client"
    " Binding.\",\"variables\":{\"host\":{\"default\":\"localhost\"},\"port\""
    ":{\"default\":\"1234\"}},\"x-afb-events\":[{\"$ref\":\"#/components/sche"
    "mas/afb-event\"}]}],\"components\":{\"schemas\":{\"afb-reply\":{\"$ref\""
    ":\"#/components/schemas/afb-reply-v2\"},\"afb-event\":{\"$ref\":\"#/comp"
    "onents/schemas/afb-event-v2\"},\"afb-reply-v2\":{\"title\":\"Generic res"
    "ponse.\",\"type\":\"object\",\"required\":[\"jtype\",\"request\"],\"prop"
    "erties\":{\"jtype\":{\"type\":\"string\",\"const\":\"afb-reply\"},\"requ"
    "est\":{\"type\":\"object\",\"required\":[\"status\"],\"properties\":{\"s"
    "tatus\":{\"type\":\"string\"},\"info\":{\"type\":\"string\"},\"token\":{"
    "\"type\":\"string\"},\"uuid\":{\"type\":\"string\"},\"reqid\":{\"type\":"
    "\"string\"}}},\"response\":{\"type\":\"object\"}}},\"afb-event-v2\":{\"t"
    "ype\":\"object\",\"required\":[\"jtype\",\"event\"],\"properties\":{\"jt"
    "ype\":{\"type\":\"string\",\"const\":\"afb-event\"},\"event\":{\"type\":"
    "\"string\"},\"data\":{\"type\":\"object\"}}}},\"x-permissions\":{\"mpdcl"
    "ient\":{\"permission\":\"urn:AGL:permission:audio:public:mpdclient\"}},\""
    "responses\":{\"200\":{\"description\":\"A complex object array response\""
    ",\"content\":{\"application/json\":{\"schema\":{\"$ref\":\"#/components/"
    "schemas/afb-reply\"}}}}}},\"paths\":{\"/ping\":{\"description\":\"Ping B"
    "inding\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x-permissio"
    "ns/mpdclient\"},\"parameters\":[{\"in\":\"query\",\"name\":\"args\",\"re"
    "quired\":false,\"schema\":{\"type\":\"json\"}}],\"responses\":{\"200\":{"
    "\"$ref\":\"#/components/responses/200\"}}}},\"/subscribe\":{\"descriptio"
    "n\":\"Subscribe Music Player Events\",\"get\":{\"x-permissions\":{\"$ref"
    "\":\"#/components/x-permissions/mpdclient\"},\"parameters\":[{\"in\":\"q"
    "uery\",\"name\":\"event\",\"required\":true,\"schema\":{\"type\":\"strin"
    "g\"}}],\"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}"
    "}},\"/search\":{\"description\":\"List by type with optional filter\",\""
    "get\":{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclien"
    "t\"},\"parameters\":[{\"in\":\"query\",\"name\":\"type\",\"required\":tr"
    "ue,\"schema\":{\"type\":\"string\"}},{\"in\":\"query\",\"name\":\"query\""
    ",\"required\":false,\"schema\":{\"type\":\"string\"}},{\"in\":\"query\","
    "\"name\":\"exact\",\"required\":false,\"schema\":{\"type\":\"boolean\"}}"
    ",{\"in\":\"query\",\"name\":\"add\",\"required\":false,\"schema\":{\"typ"
    "e\":\"boolean\"}},{\"in\":\"query\",\"name\":\"play\",\"required\":false"
    ",\"schema\":{\"type\":\"boolean\"}},{\"in\":\"query\",\"name\":\"filter\""
    ",\"required\":false,\"schema\":{\"type\":\"json\"}}],\"responses\":{\"20"
    "0\":{\"$ref\":\"#/components/responses/200\"}}}},\"/playlist\":{\"descri"
    "ption\":\"List Playlist Content.\",\"get\":{\"x-permissions\":{\"$ref\":"
    "\"#/components/x-permissions/mpdclient\"},\"parameters\":[{\"in\":\"quer"
    "y\",\"name\":\"name\",\"required\":false,\"schema\":{\"type\":\"integer\""
    "}}],\"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},"
    "\"/connect\":{\"description\":\"Connect to local or remote Music Player "
    "Daemon.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x-permissi"
    "ons/mpdclient\"},\"parameters\":[{\"in\":\"query\",\"name\":\"hostname\""
    ",\"required\":false,\"schema\":{\"type\":\"string\"}},{\"in\":\"query\","
    "\"name\":\"port\",\"required\":false,\"schema\":{\"type\":\"integer\"}}]"
    ",\"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/"
    "version\":{\"description\":\"Get Music Player Daemon Version.\",\"get\":"
    "{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},\""
    "responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/out"
    "put\":{\"description\":\"Return Current Available Outputs.\",\"get\":{\""
    "x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},\"re"
    "sponses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/statu"
    "s\":{\"description\":\"Return Current Daemon Status.\",\"get\":{\"x-perm"
    "issions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},\"response"
    "s\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/control\":{"
    "\"description\":\"Play Current song.\",\"get\":{\"x-permissions\":{\"$re"
    "f\":\"#/components/x-permissions/mpdclient\"},\"parameters\":[{\"in\":\""
    "query\",\"name\":\"pause\",\"required\":false,\"schema\":{\"type\":\"boo"
    "lean\"}},{\"in\":\"query\",\"name\":\"resume\",\"required\":false,\"sche"
    "ma\":{\"type\":\"boolean\"}},{\"in\":\"query\",\"name\":\"toggle\",\"req"
    "uired\":false,\"schema\":{\"type\":\"integer\"}},{\"in\":\"query\",\"nam"
    "e\":\"play\",\"required\":false,\"schema\":{\"type\":\"integer\"}},{\"in"
    "\":\"query\",\"name\":\"prev\",\"required\":false,\"schema\":{\"type\":\""
    "boolean\"}},{\"in\":\"query\",\"name\":\"next\",\"required\":false,\"sch"
    "ema\":{\"type\":\"boolean\"}}],\"responses\":{\"200\":{\"$ref\":\"#/comp"
    "onents/responses/200\"}}}}}}"
;

static const struct afb_auth _afb_auths_v2_mpdc[] = {
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission:audio:public:mpdclient" }
};

 void mpdcapi_ping(struct afb_req req);
 void mpdcapi_subscribe(struct afb_req req);
 void mpdcapi_search(struct afb_req req);
 void mpdcapi_playlist(struct afb_req req);
 void mpdcapi_connect(struct afb_req req);
 void mpdcapi_version(struct afb_req req);
 void mpdcapi_output(struct afb_req req);
 void mpdcapi_status(struct afb_req req);
 void mpdcapi_control(struct afb_req req);

static const struct afb_verb_v2 _afb_verbs_v2_mpdc[] = {
    {
        .verb = "ping",
        .callback = mpdcapi_ping,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Ping Binding",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "subscribe",
        .callback = mpdcapi_subscribe,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Subscribe Music Player Events",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "search",
        .callback = mpdcapi_search,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "List by type with optional filter",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "playlist",
        .callback = mpdcapi_playlist,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "List Playlist Content.",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "connect",
        .callback = mpdcapi_connect,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Connect to local or remote Music Player Daemon.",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "version",
        .callback = mpdcapi_version,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Get Music Player Daemon Version.",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "output",
        .callback = mpdcapi_output,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Return Current Available Outputs.",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "status",
        .callback = mpdcapi_status,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Return Current Daemon Status.",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "control",
        .callback = mpdcapi_control,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = "Play Current song.",
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = NULL,
        .callback = NULL,
        .auth = NULL,
        .info = NULL,
        .session = 0
	}
};

const struct afb_binding_v2 afbBindingV2 = {
    .api = "mpdc",
    .specification = _afb_description_v2_mpdc,
    .info = "",
    .verbs = _afb_verbs_v2_mpdc,
    .preinit = NULL,
    .init = mpdcBindingInit,
    .onevent = mpdcDispatchEvent,
    .noconcurrency = 1
};

