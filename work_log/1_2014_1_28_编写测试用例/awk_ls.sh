#!/bin/bash

ls -l $* | awk 'BEGIN{print "BYTES", "\t", "FILE"}NF==9&&/^-/{sum+= $5 ++filenum print $5, "\t", $9}NF==9&&/^d/{print "<dir>", "\t", $9}1~/^\..*$/{print "\t" $0}END{print "Total:", sum, "bytes"}'
