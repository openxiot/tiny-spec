# Test resources

`stanza/` holds the JSON stanza fixtures of the original tiny-spec project
(`test/StanzaCodec/resources/stanza`).  Each file is a complete stanza
document in the shape produced by StanzaCodec:

* `iq/` – IQ queries, results and errors (basic, device/control,
  device/key, device/verify, node/control).
* `message/` – device and node messages, whose payload is a notice.

`tests/test_stanza_resources.c` decodes every file, re-encodes the stanza
and checks that the generated document equals the original one.
