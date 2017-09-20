#!/bin/bash
mount | grep loadapp | awk '{print $1}'
