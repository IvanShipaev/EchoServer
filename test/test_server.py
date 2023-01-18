import socket
import sys 
import time

id = sys.argv[1]

sock = socket.socket()
sock.connect(('localhost', 5000))

def test():
    count = 0
    for i in range(100):
        data_send = (id + ' :' + str(count)).encode()
        sock.send(data_send)
        data_recv = sock.recv(1024)
        if len(data_recv) == 0:
            print(id + ' error close client ')
            sock.close()
            return
        if data_send != data_recv:
           print(id + 'error data recv')
           sock.close()
           return
        count = count + 1
        time.sleep(0.5)

    print(id + ' test OK')
    sock.close()

test()

