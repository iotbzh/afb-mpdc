
static const char _afb_description_v2_mpdc[] =
    "{\"openapi\":\"3.0.0\",\"$schema\":\"http:iot.bzh/download/openapi/schem"
    "a-3.0/default-schema.json\",\"info\":{\"description\":\"\",\"title\":\"c"
    "ontroler\",\"version\":\"1.0\",\"x-binding-c-generator\":{\"api\":\"mpdc"
    "\",\"version\":2,\"prefix\":\"mpdcapi_\",\"postfix\":\"\",\"start\":null"
    ",\"onevent\":\"mpdcDispatchEvent\",\"init\":\"mpdcBindingInit\",\"scope\""
    ":\"\",\"noconcurrency\":true,\"private\":false}},\"servers\":[{\"url\":\""
    "ws://{host}:{port}/api/polctl\",\"description\":\"Music Player Client Bi"
    "nding.\",\"variables\":{\"host\":{\"default\":\"localhost\"},\"port\":{\""
    "default\":\"1234\"}},\"x-afb-events\":[{\"$ref\":\"#/components/schemas/"
    "afb-event\"}]}],\"components\":{\"schemas\":{\"afb-reply\":{\"$ref\":\"#"
    "/components/schemas/afb-reply-v2\"},\"afb-event\":{\"$ref\":\"#/componen"
    "ts/schemas/afb-event-v2\"},\"afb-reply-v2\":{\"title\":\"Generic respons"
    "e.\",\"type\":\"object\",\"required\":[\"jtype\",\"request\"],\"properti"
    "es\":{\"jtype\":{\"type\":\"string\",\"const\":\"afb-reply\"},\"request\""
    ":{\"type\":\"object\",\"required\":[\"status\"],\"properties\":{\"status"
    "\":{\"type\":\"string\"},\"info\":{\"type\":\"string\"},\"token\":{\"typ"
    "e\":\"string\"},\"uuid\":{\"type\":\"string\"},\"reqid\":{\"type\":\"str"
    "ing\"}}},\"response\":{\"type\":\"object\"}}},\"afb-event-v2\":{\"type\""
    ":\"object\",\"required\":[\"jtype\",\"event\"],\"properties\":{\"jtype\""
    ":{\"type\":\"string\",\"const\":\"afb-event\"},\"event\":{\"type\":\"str"
    "ing\"},\"data\":{\"type\":\"object\"}}}},\"x-permissions\":{\"mpdclient\""
    ":{\"permission\":\"urn:AGL:permission:audio:public:mpdclient\"}},\"respo"
    "nses\":{\"200\":{\"description\":\"A complex object array response\",\"c"
    "ontent\":{\"application/json\":{\"schema\":{\"$ref\":\"#/components/sche"
    "mas/afb-reply\"}}}}}},\"paths\":{\"/ping\":{\"description\":\"Ping Bindi"
    "ng\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/m"
    "pdclient\"},\"parameters\":[{\"in\":\"query\",\"name\":\"args\",\"requir"
    "ed\":false,\"schema\":{\"type\":\"json\"}}],\"responses\":{\"200\":{\"$r"
    "ef\":\"#/components/responses/200\"}}}},\"/subscribe\":{\"description\":"
    "\"Subcribe Music Player Events\",\"get\":{\"x-permissions\":{\"$ref\":\""
    "#/components/x-permissions/mpdclient\"},\"parameters\":[{\"in\":\"query\""
    ",\"name\":\"event\",\"required\":true,\"schema\":{\"type\":\"string\"}}]"
    ",\"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/"
    "search\":{\"description\":\"List by type with optional filter\",\"get\":"
    "{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},\""
    "parameters\":[{\"in\":\"query\",\"name\":\"type\",\"required\":true,\"sc"
    "hema\":{\"type\":\"string\"}},{\"in\":\"query\",\"name\":\"query\",\"req"
    "uired\":false,\"schema\":{\"type\":\"string\"}},{\"in\":\"query\",\"name"
    "\":\"exact\",\"required\":false,\"schema\":{\"type\":\"boolean\"}},{\"in"
    "\":\"query\",\"name\":\"add\",\"required\":false,\"schema\":{\"type\":\""
    "boolean\"}},{\"in\":\"query\",\"name\":\"play\",\"required\":false,\"sch"
    "ema\":{\"type\":\"boolean\"}},{\"in\":\"query\",\"name\":\"filter\",\"re"
    "quired\":false,\"schema\":{\"type\":\"json\"}}],\"responses\":{\"200\":{"
    "\"$ref\":\"#/components/responses/200\"}}}},\"/play\":{\"description\":\""
    "Play a Song from Playlist.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/co"
    "mponents/x-permissions/mpdclient\"},\"parameters\":[{\"in\":\"query\",\""
    "name\":\"index\",\"required\":false,\"schema\":{\"type\":\"integer\"}}],"
    "\"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/l"
    "istsong\":{\"description\":\"List Songs From Music Library.\",\"get\":{\""
    "x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},\"pa"
    "rameters\":[{\"in\":\"query\",\"name\":\"path\",\"required\":false,\"sch"
    "ema\":{\"type\":\"integer\"}}],\"responses\":{\"200\":{\"$ref\":\"#/comp"
    "onents/responses/200\"}}}},\"/playlist\":{\"description\":\"List Playlis"
    "t Content.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x-permi"
    "ssions/mpdclient\"},\"parameters\":[{\"in\":\"query\",\"name\":\"name\","
    "\"required\":false,\"schema\":{\"type\":\"integer\"}}],\"responses\":{\""
    "200\":{\"$ref\":\"#/components/responses/200\"}}}},\"/connect\":{\"descr"
    "iption\":\"Connect to local or remote Music Player Daemon.\",\"get\":{\""
    "x-permissions\":{\"$ref\":\"#/components/x-permissions/mpdclient\"},\"pa"
    "rameters\":[{\"in\":\"query\",\"name\":\"hostname\",\"required\":false,\""
    "schema\":{\"type\":\"string\"}},{\"in\":\"query\",\"name\":\"port\",\"re"
    "quired\":false,\"schema\":{\"type\":\"integer\"}}],\"responses\":{\"200\""
    ":{\"$ref\":\"#/components/responses/200\"}}}},\"/version\":{\"descriptio"
    "n\":\"Get Music Player Daemon Version.\",\"get\":{\"x-permissions\":{\"$"
    "ref\":\"#/components/x-permissions/mpdclient\"},\"responses\":{\"200\":{"
    "\"$ref\":\"#/components/responses/200\"}}}},\"/output\":{\"description\""
    ":\"Return Current Avaliable Ouputs.\",\"get\":{\"x-permissions\":{\"$ref"
    "\":\"#/components/x-permissions/mpdclient\"},\"responses\":{\"200\":{\"$"
    "ref\":\"#/components/responses/200\"}}}},\"/status\":{\"description\":\""
    "Return Current Daemon Status.\",\"get\":{\"x-permissions\":{\"$ref\":\"#"
    "/components/x-permissions/mpdclient\"},\"responses\":{\"200\":{\"$ref\":"
    "\"#/components/responses/200\"}}}},\"/control\":{\"description\":\"Play "
    "Current song.\",\"get\":{\"x-permissions\":{\"$ref\":\"#/components/x-pe"
    "rmissions/mpdclient\"},\"parameters\":[{\"in\":\"query\",\"name\":\"paus"
    "e\",\"required\":false,\"schema\":{\"type\":\"boolean\"}},{\"in\":\"quer"
    "y\",\"name\":\"resume\",\"required\":false,\"schema\":{\"type\":\"boolea"
    "n\"}},{\"in\":\"query\",\"name\":\"next\",\"required\":false,\"schema\":"
    "{\"type\":\"boolean\"}},{\"in\":\"query\",\"name\":\"stop\",\"required\""
    ":false,\"schema\":{\"type\":\"boolean\"}}],\"responses\":{\"200\":{\"$re"
    "f\":\"#/components/responses/200\"}}}}}}"
;

static const struct afb_auth _afb_auths_v2_mpdc[] = {
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission:audio:public:mpdclient" }
};

 void mpdcapi_ping(struct afb_req req);
 void mpdcapi_subscribe(struct afb_req req);
 void mpdcapi_search(struct afb_req req);
 void mpdcapi_play(struct afb_req req);
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
        .verb = "play",
        .callback = mpdcapi_play,
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

