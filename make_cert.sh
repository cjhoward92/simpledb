#!/bin/bash

set -uex

openssl req -x509 -newkey rsa:4096 -sha256 -days 3650 -nodes \
          -keyout key.pem -out cert.pem -outform PEM -subj "/CN=localhost"