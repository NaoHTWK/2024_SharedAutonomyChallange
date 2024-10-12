#!/usr/bin/env sh

set -u
set -e

. ../venv/bin/activate

exec ./main.py
