#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import select, socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('<broadcast>', 9876))
s.setblocking(0)

while True:
    result = select.select([s],[],[])
    msg = result[0][0].recv(1024)
    if (type(msg) is bytes):
        msg = msg.decode('utf-8')
    print(msg)
