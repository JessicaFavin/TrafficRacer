#!/bin/bash 
var=$(ps -ef | grep "play -q -v 0.99 $1" | grep -v grep | awk '{ print $2 }' );
kill -9 $var;