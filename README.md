# Simple DB

A simple database system over telnet with TLS (using OpenSSL).

## Simple design

The db will support the following commands:

- get <key>
- store <key> <value>
- delete <key>
- search <key_prefix>

Each command will be built to support new operations.

### What is a key

We support very limited keys at this time for simplicity. Keys have the following
limitations:

Supported Characters - [0-9]
Max Length - 512
Min Length - 1

Example key:
`84930246732898439230`

### What kind of data can be stored with a key

Any data, but we don't capture data types. We store the data as raw bytes for now
and return the data in the same format. Since this is over telnet, the data is
effectively a string. It's up to the consumer to handle deserialization.

We do have a maximum size of 4KB (4 * 1024 bytes) per value.

### Operations

Below is a list of the support operations with in-depth details

#### get

Returns a single value for the specified key.
If no key is found the value sent back will be
`nil`.

`get <key> -> <value>`

Where

`key` - A key
`value` - A blob of contiguous binary data

#### store

Stores the specified value at the specified key. Acts as an
upsert in that it will create or overwrite the data at the
specified key.

`store <key> <value> -> <ok|error>`

Where

`key` - The key used to store the value
`value` - The value to store
`ok|error` - Response status codes. `ok` means the value was stored properly. `error` signals some storage error.

#### delete

Deletes the value stored at a specified key. If no value
exists this is essentially a no-op.

`delete <key> -> <ok|error>`

Where

`key` - The key used to identify the value to delete
`ok|error` - Response status codes. `ok` means the value was deleted or no-op. `error` signals some delete error.

#### search

Searches for a key prefix and returns all values nested under the prefix.
If no values are found, returns `[]`.

`select <key_prefix> -> [<value>]`

Where

`key_prefix` - Similar to a key, but represents a path to search for all nested values.
`[<value>]` - An array of values found nested under the specific search key prefix.