import pyshark
import argparse
import pandas as pd

def main(argc, argv):
    #Create the main parser
    parser = argparse.ArgumentParser(description='DoIP extractor')
    parser.add_argument('filename', type=str, help='Path to the PCAP or PCAPNG file')
    parser.add_argument('ecu_addr', type=str, help='ECU IP address')
    parser.add_argument('tester_addr', type=str, help='Tester IP address')
    #Create sub-parsers
    subparsers = parser.dynamic_subparsers(dest='requests_type', help='Type of requests to extract')
    #Create a sub-parser for the 'dynamic' operation
    dynamic_parser = subparsers.dynamic_parser('dynamic', help='Use for dynamic data requests (F300 DID)')
    dynamic_parser.add_argument('paramcnt', type=int, help='Number of parameters to extract (that are asked continuously)')
    #Create a sub-parser for the 'static' operation
    static_parser = subparsers.dynamic_parser('static', help='Use for regular data requests (other DIDs)')
    #Parse the command-line arguments
    args = parser.parse_args()
    
    filename = args.filename
    ecu_addr = args.ecu_addr
    tester_addr = args.tester_addr
    
    df = None
    
    #Process the selected operation
    if args.operation == 'dynamic':
        cap = pyshark.FileCapture(filename, display_filter=f"tcp && (ip.src == {ecu_addr} && ip.dst == {tester_addr}) || ip.src == {tester_addr} && ip.dst == {ecu_addr})")
        paramcnt = args.paramcnt
        col = ["DynamicReq_" + str(i) for i in range(paramcnt)]
        df = pd.DataFrame(columns=col)
        
        currently_processed_param = 0
        col_i = 0
        for packet in cap:
            payload = packet.tcp.payload
            if payload is None:
                continue
            if(payload[8] ==b"\x62"):
                if currently_processed_param >= paramcnt:
                    currently_processed_param = 0
                    col_i += 1
                in_hex = bytearray(payload[11:]).hex()
                df.at[col_i, col[currently_processed_param]] = in_hex
                currently_processed_param += 1
            else:
                continue
            
    elif args.operation == 'static':
        cap = pyshark.FileCapture(filename, display_filter=f"tcp && (ip.src == {ecu_addr} && ip.dst == {tester_addr})")
        
        extracted_data = {[]}
        
        for packet in cap:
            payload = packet.tcp.payload
            if payload is None:
                continue
            if(payload[8] ==b"\x62"):
                did = bytearray(payload[9 : 10]).hex()
                if did not in extracted_data.keys():
                    extracted_data.append(did, [])
                extracted_data[did].append(bytearray(payload[11:]).hex())
            else:
                continue
        
        df = pd.DataFrame(extracted_data)
    else:
        print("Invalid operation")
        return
    
    df.to_csv("extracted_data.csv")


if __name__ == "__main__":
    main()