import socket
import threading
import struct
import enum
import pandas as pd

# Configuration
TCP_IP = "0.0.0.0"
TCP_PORT = 6801
UDP_IP = "0.0.0.0"
UDP_PORT = 6811

class EDmeDynamicData(enum.Enum):
    DME_ENGINE_ROTATIONAL_SPEED = bytes.fromhex("48 07 01 02")
    DME_COOLANT_TEMPERATURE = bytes.fromhex("43 00 01 01")
    DME_ENGINE_OIL_TEMPERATURE = bytes.fromhex("44 02 01 02")
    DME_BATTERY_VOLTAGE = bytes.fromhex("46 09 01 01")
    DME_AMBIENT_TEMPERATURE = bytes.fromhex("42 04 01 01")
    DME_AIR_MASS = bytes.fromhex("42 08 01 02")
    DME_RAIL_PRESSURE = bytes.fromhex("48 1A 01 02")
    DME_ACCELERATOR_PEDAL_POSITION = bytes.fromhex("4A 29 01 01")

DYNAMIC_DATA_VALUES = set(item.value for item in EDmeDynamicData)

class EDid(enum.Enum):
    KOMBI_ENGINE_SPEED_ON_DISP = bytes.fromhex("D1 06")
    KOMBI_TOTAL_DISTANCE = bytes.fromhex("D1 0D")
    KOMBI_FUEL_LEVEL = bytes.fromhex("D1 1F")
    KOMBI_SPEED = bytes.fromhex("D1 07")
    KOMBI_OUTSIDE_TEMP_SENSOR = bytes.fromhex("D1 12")
    IHKA_EVAPORATOR_TEMPERATURE_SENSOR = bytes.fromhex("D9 5C")
    IHKA_TEMPERATURE_SELECTOR = bytes.fromhex("D9 77")

DYNAMIC_DATA_DID = bytes.fromhex("F3 00")

DID_VALUES = set(item.value for item in EDid)

response_size_map = {
    EDmeDynamicData.DME_ENGINE_ROTATIONAL_SPEED: 2,
    EDmeDynamicData.DME_COOLANT_TEMPERATURE: 1,
    EDmeDynamicData.DME_ENGINE_OIL_TEMPERATURE: 2,
    EDmeDynamicData.DME_BATTERY_VOLTAGE: 1,
    EDmeDynamicData.DME_AMBIENT_TEMPERATURE: 1,
    EDmeDynamicData.DME_AIR_MASS: 2,
    EDmeDynamicData.DME_RAIL_PRESSURE: 2,
    EDmeDynamicData.DME_ACCELERATOR_PEDAL_POSITION: 1,
    
    EDid.KOMBI_ENGINE_SPEED_ON_DISP: 2,
    EDid.KOMBI_TOTAL_DISTANCE: 4,
    EDid.KOMBI_FUEL_LEVEL: 9,
    EDid.KOMBI_SPEED: 2,
    EDid.KOMBI_OUTSIDE_TEMP_SENSOR: 2,
    
    EDid.IHKA_EVAPORATOR_TEMPERATURE_SENSOR: 2,
    EDid.IHKA_TEMPERATURE_SELECTOR: 4,
}

row_to_process = {
    EDmeDynamicData.DME_ENGINE_ROTATIONAL_SPEED: 0,
    EDmeDynamicData.DME_COOLANT_TEMPERATURE: 0,
    EDmeDynamicData.DME_ENGINE_OIL_TEMPERATURE: 0,
    EDmeDynamicData.DME_BATTERY_VOLTAGE: 0,
    EDmeDynamicData.DME_AMBIENT_TEMPERATURE: 0,
    EDmeDynamicData.DME_AIR_MASS: 0,
    EDmeDynamicData.DME_RAIL_PRESSURE: 0,
    EDmeDynamicData.DME_ACCELERATOR_PEDAL_POSITION: 0,
    EDid.KOMBI_ENGINE_SPEED_ON_DISP: 0,
    EDid.KOMBI_TOTAL_DISTANCE: 0,
    EDid.KOMBI_FUEL_LEVEL: 0,
    EDid.KOMBI_SPEED: 0,
    EDid.KOMBI_OUTSIDE_TEMP_SENSOR: 0,
    EDid.IHKA_EVAPORATOR_TEMPERATURE_SENSOR: 0,
    EDid.IHKA_TEMPERATURE_SELECTOR: 0,
}

string_map = {
    EDmeDynamicData.DME_ENGINE_ROTATIONAL_SPEED: "DME_101",
    EDmeDynamicData.DME_COOLANT_TEMPERATURE: "DME_102",
    EDmeDynamicData.DME_ENGINE_OIL_TEMPERATURE: "DME_103",
    EDmeDynamicData.DME_BATTERY_VOLTAGE: "DME_104",
    EDmeDynamicData.DME_AMBIENT_TEMPERATURE: "DME_117",
    EDmeDynamicData.DME_AIR_MASS: "DME_201",
    EDmeDynamicData.DME_RAIL_PRESSURE: "DME_407",
    EDmeDynamicData.DME_ACCELERATOR_PEDAL_POSITION: "DME_905",
    EDid.KOMBI_ENGINE_SPEED_ON_DISP: "KOMBI_Rpm",
    EDid.KOMBI_TOTAL_DISTANCE: "KOMBI_Distance",
    EDid.KOMBI_FUEL_LEVEL: "KOMBI_Fuel",
    EDid.KOMBI_SPEED: "KOMBI_Speed",
    EDid.KOMBI_OUTSIDE_TEMP_SENSOR: "KOMBI_Temp",
    EDid.IHKA_EVAPORATOR_TEMPERATURE_SENSOR: "IHKA_Evap",
    EDid.IHKA_TEMPERATURE_SELECTOR: "IHKA_Temp",
}

# Mock UDP response
UDP_RESPONSE = bytes.fromhex(
    "00 00 00 32 00 11"
    "44 49 41 47 41 44 52 31 30 42 4d 57 4d 41 43 30"
    "38 38 66 63 33 32 38 32 38 62 66 42 4d 57 56 49"
    "4e 57 42 41 46 55 39 43 35 31 42 43 37 38 36 38"
    "32 32"
)

currently_assigned_dynamic_data = None

df = None
rows = 0

def udp_server():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as udp_sock:
        udp_sock.bind((UDP_IP, UDP_PORT))
        print(f"UDP server listening on {UDP_IP}:{UDP_PORT}")

        while True:
            data, addr = udp_sock.recvfrom(1024)
            print(f"Received UDP message from {addr[0]}:{addr[1]}: {data.hex()}")

            if data == b"\x00\x00\x00\x00\x00\x11":
                print(f"Matching payload received, sending response to {addr[0]}:{addr[1]}")
                try:
                    udp_sock.sendto(UDP_RESPONSE, addr)
                except OSError as e:
                    print(f"Failed to send response: {e}")
            else:
                print(f"Unexpected payload received: {data.hex()}")

class TCPHandler(threading.Thread):
    def __init__(self, client_socket, client_address):
        super().__init__()
        self.client_socket = client_socket
        self.client_address = client_address

    def handle_dynamic_data_request(self, payload):
        global currently_assigned_dynamic_data
        ftype = payload[9]
        new_payload = bytearray(payload[:13])  # Convert to mutable bytearray and limit length
        new_payload[5] = 0x02  # Set a specific byte value
        
        if ftype == 0x01:
            new_payload[3] = 0x07
            # change currently assigned dynamic data
            requested_dynamic_assing = bytes(payload[12:16])
            if requested_dynamic_assing in DYNAMIC_DATA_VALUES:
                currently_assigned_dynamic_data = EDmeDynamicData(requested_dynamic_assing)
            else:
                print(f"could not assign dynamic data identifier: {requested_dynamic_assing.hex()}")
        print("Sending SID: 2C (echo frame with additional info)")
        self.client_socket.sendall(bytes(new_payload))  # Convert back to bytes for sending
        print(f"Echoed 2C frame: {new_payload.hex()}")
        # Immediately send the 6C frame
        self.send_additional_6c_frame(ftype)

    def send_additional_6c_frame(self, ftype):
        print("Sending SID: 6C frame")
        payload = struct.pack(">BBBBBB", 0x00, 0x00, 0x00, 0x06, 0x00, 0x01) + b"\x12\xf4\x6c" + bytes([ftype]) + b"\xf3\x00"
        try:
            self.client_socket.sendall(payload)
            print(f"Sent SID: 6C frame: {payload.hex()}")
        except BrokenPipeError:
            print("Error: Broken pipe while sending SID: 6C frame")

    def handle_data_request(self, payload):
        # Correctly construct the response frame
        response_size = 0x05
        did_bytes = ((payload[9] << 8) | payload[10]).to_bytes(2, byteorder="big")
        tester_device_internal_addr = payload[6]
        ecu_internal_addr = payload[7]
        
        response = []
        if did_bytes == DYNAMIC_DATA_DID:
            #dynamic data request
            response_size += response_size_map[EDmeDynamicData(currently_assigned_dynamic_data)]
            response = get_next_data(EDmeDynamicData(currently_assigned_dynamic_data))
        else:
            if did_bytes in DID_VALUES:
                response_size += response_size_map[EDid(did_bytes)]
                response = get_next_data(EDid(did_bytes))
            else:
                print(f"Unknown DID! {did_bytes}")
                return
        
        return (
            struct.pack(">BBB", 0x00, 0x00, 0x00)
            + bytes([response_size])
            + struct.pack(">BB", 0x00, 0x01)
            + bytes([ecu_internal_addr])
            + bytes([tester_device_internal_addr])
            + b"\x62"
            + did_bytes
            + response
        )

    def run(self):
        print(f"TCP connection established with {self.client_address}")

        try:
            while True:
                data = self.client_socket.recv(1024)
                if not data:
                    print(f"Connection closed by client {self.client_address}")
                    break

                print(f"Received TCP data from {self.client_address}: {data.hex()}")
                if len(data) < 11:
                    print("Malformed request received.")
                    continue

                sid = data[8]

                if sid == 0x2C:  # DYNAMICALLY_DEFINE_DATA_IDENTIFIER
                    self.handle_dynamic_data_request(data)
                elif sid == 0x22:  # READ_DATA_BY_IDENTIFIER
                    echo_payload = bytearray(data)  # Convert to mutable bytearray
                    echo_payload[5] = 0x02
                    # Echoed 22 frame:
                    self.client_socket.sendall(bytes(echo_payload))  # Convert back to bytes for sending
                    response = self.handle_data_request(data)
                    self.client_socket.sendall(response)
                else:
                    print(f"Unhandled SID: {sid:#02X}")

        except Exception as e:
            print(f"Error during TCP communication with {self.client_address}: {e}")
        finally:
            self.client_socket.close()

def tcp_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as tcp_sock:
        tcp_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        tcp_sock.bind((TCP_IP, TCP_PORT))
        tcp_sock.listen()
        print(f"TCP server listening on {TCP_IP}:{TCP_PORT}")

        while True:
            client_socket, client_address = tcp_sock.accept()
            handler = TCPHandler(client_socket, client_address)
            handler.start()
        
def get_next_data(id):
    str_id = string_map[id]
    processed_row = row_to_process[id]
    if processed_row >= rows - 1:
        row_to_process[id] = 0
    else:
        row_to_process[id] += 1
    str_data = str(df[str_id].iloc[processed_row])
    return bytes.fromhex(str_data)

if __name__ == "__main__":
    
    df = pd.read_csv("python_test_scripts/parameters_merged_for_sym_modified.csv")
    rows = len(df.index)
    
    udp_thread = threading.Thread(target=udp_server, daemon=True)
    udp_thread.start()

    tcp_thread = threading.Thread(target=tcp_server, daemon=True)
    tcp_thread.start()

    print("Servers are running. Press Ctrl+C to exit.")
    udp_thread.join()
    tcp_thread.join()
