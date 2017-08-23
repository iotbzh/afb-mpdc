
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
    "0\":{\"$ref\":\"#/components/responses/200\"}}}},\"/listsong\":{\"descri"
    "ption\":\"List Songs From Music Library.\",\"get\":{\"x-permissions\":{\""
    "$ref\":\"#/components/x-permissions/mpdclient\"},\"parameters\":[{\"in\""
    ":\"query\",\"name\":\"path\",\"required\":false,\"schema\":{\"type\":\"i"
    "nteger\"}}],\"responses\":{\"200\":{\"$ref\":\"#/components/responses/20"
    "0\"}}}},\"/playlist\":{\"description\":\"List Playlist Content.\",\"get\""
    ":{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},"
    "\"parameters\":[{\"in\":\"query\",\"name\":\"name\",\"required\":false,\""
    "schema\":{\"type\":\"integer\"}}],\"responses\":{\"200\":{\"$ref\":\"#/c"
    "omponents/responses/200\"}}}},\"/connect\":{\"description\":\"Connect to"
    " local or remote Music Player Daemon.\",\"get\":{\"x-permissions\":{\"$r"
    "ef\":\"#/components/x-permissions/mpdclient\"},\"parameters\":[{\"in\":\""
    "query\",\"name\":\"hostname\",\"required\":false,\"schema\":{\"type\":\""
    "string\"}},{\"in\":\"query\",\"name\":\"port\",\"required\":false,\"sche"
    "ma\":{\"type\":\"integer\"}}],\"responses\":{\"200\":{\"$ref\":\"#/compo"
    "nents/responses/200\"}}}},\"/version\":{\"description\":\"Get Music Play"
    "er Daemon Version.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components"
    "/x-permissions/mpdclient\"},\"responses\":{\"200\":{\"$ref\":\"#/compone"
    "nts/responses/200\"}}}},\"/output\":{\"description\":\"Return Current Av"
    "ailable Outputs.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x"
    "-permissions/mpdclient\"},\"responses\":{\"200\":{\"$ref\":\"#/component"
    "s/responses/200\"}}}},\"/status\":{\"description\":\"Return Current Daem"
    "on Status.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x-permi"
    "ssions/mpdclient\"},\"responses\":{\"200\":{\"$ref\":\"#/components/resp"
    "onses/200\"}}}},\"/control\":{\"description\":\"Play Current song.\",\"g"
    "et\":{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient"
    "\"},\"parameters\":[{\"in\":\"query\",\"name\":\"pause\",\"required\":fa"
    "lse,\"schema\":{\"type\":\"boolean\"}},{\"in\":\"query\",\"name\":\"resu"
    "me\",\"required\":false,\"schema\":{\"type\":\"boolean\"}},{\"in\":\"que"
    "ry\",\"name\":\"toggle\",\"required\":false,\"schema\":{\"type\":\"integ"
    "er\"}},{\"in\":\"query\",\"name\":\"play\",\"required\":false,\"schema\""
    ":{\"type\":\"integer\"}},{\"in\":\"query\",\"name\":\"prev\",\"required\""
    ":false,\"schema\":{\"type\":\"boolean\"}},{\"in\":\"query\",\"name\":\"n"
    "ext\",\"required\":false,\"schema\":{\"type\":\"boolean\"}}],\"responses"
    "\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}}}}"
;

static const struct afb_auth _afb_auths_v2_mpdc[] = {
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission:audio:public:mpdclient" }
};

 void mpdcapi_ping(struct afb_req req);
 void mpdcapi_subscribe(struct afb_req req);
 void mpdcapi_search(struct afb_req req);
 void mpdcapi_listsong(struct afb_req req);
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
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "subscribe",
        .callback = mpdcapi_subscribe,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "search",
        .callback = mpdcapi_search,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "listsong",
        .callback = mpdcapi_listsong,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "playlist",
        .callback = mpdcapi_playlist,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "connect",
        .callback = mpdcapi_connect,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "version",
        .callback = mpdcapi_version,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "output",
        .callback = mpdcapi_output,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "status",
        .callback = mpdcapi_status,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    {
        .verb = "control",
        .callback = mpdcapi_control,
        .auth = &_afb_auths_v2_mpdc[0],
        .info = NULL,
        .session = AFB_SESSION_NONE_V2
    },
    { .verb = NULL }
};

const struct afb_binding_v2 afbBindingV2 = {
    .api = "mpdc",
    .specification = _afb_description_v2_mpdc,
    .info = NULL,
    .verbs = _afb_verbs_v2_mpdc,
    .preinit = NULL,
    .init = mpdcBindingInit,
    .onevent = mpdcDispatchEvent,
    .noconcurrency = 1
};

