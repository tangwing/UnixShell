#!/bin/bash

G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --gen-suppressions=no ./myshell

