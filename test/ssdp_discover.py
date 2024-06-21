import socket
import struct
import time

def send_ssdp_discover():
    SSDP_ADDR = "239.255.255.250"
    SSDP_PORT = 1900
    SSDP_MX = 1
    SSDP_ST = "ssdp:all"

    ssdpRequest = f"M-SEARCH * HTTP/1.1\r\n" + \
                  f"HOST: {SSDP_ADDR}:{SSDP_PORT}\r\n" + \
                  f"MAN: \"ssdp:discover\"\r\n" + \
                  f"MX: {SSDP_MX}\r\n" + \
                  f"ST: {SSDP_ST}\r\n" + "\r\n"

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
    sock.sendto(ssdpRequest.encode(), (SSDP_ADDR, SSDP_PORT))

    return sock

def receive_ssdp_responses(sock, timeout=5):
    responses = []
    start_time = time.time()
    while True:
        current_time = time.time()
        if current_time - start_time > timeout:
            break
        sock.settimeout(timeout - (current_time - start_time))
        try:
            data, addr = sock.recvfrom(1024)
            responses.append((data.decode(), addr))
        except socket.timeout:
            break
    return responses

def main():
    sock = send_ssdp_discover()
    responses = receive_ssdp_responses(sock)
    for response, addr in responses:
        print(f"Response from {addr}:\n{response}\n")

if __name__ == "__main__":
    main()