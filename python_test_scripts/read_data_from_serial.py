import serial
import pandas as pd
import enum

# Configure serial port
baudrate = 115200
serial_port = 'COM3'

class EDoIPRequest(enum.Enum):
    REQUEST_INVALID = -1,
    DME_ENGINE_OIL_TEMPERATURE = 0,
    DME_ENGINE_ROTATIONAL_SPEED = 1,
    DME_COOLANT_TEMPERATURE = 2,
    DME_BATTERY_VOLTAGE = 3,
    DME_AMBIENT_TEMPERATURE = 4,
    DME_AIR_MASS = 5,
    DME_RAIL_PRESSURE = 6,
    DME_ACCELERATOR_PEDAL_POSITION = 7,
    KOMBI_TOTAL_DISTANCE = 8,
    KOMBI_SPEED = 9,
    KOMBI_OUTSIDE_TEMP_SENSOR = 10,
    KOMBI_ENGINE_SPEED_ON_DISP = 11,
    KOMBI_FUEL_LEVEL = 12,
    IHKA_EVAPORATOR_TEMPERATURE_SENSOR = 13,
    IHKA_TEMPERATURE_SELECTOR = 14


param_string_dict = {
    EDoIPRequest.DME_ENGINE_OIL_TEMPERATURE: "DME_ENG_OIL_TEMP",
    EDoIPRequest.DME_ENGINE_ROTATIONAL_SPEED: "DME_ENG_ROT_SPEED",
    EDoIPRequest.DME_COOLANT_TEMPERATURE: "DME_COOL_TEMP",
    EDoIPRequest.DME_BATTERY_VOLTAGE: "DME_BAT_VOL",
    EDoIPRequest.DME_AMBIENT_TEMPERATURE: "DME_AMB_TEMP",
    EDoIPRequest.DME_AIR_MASS: "DME_AIR_MASS",
    EDoIPRequest.DME_RAIL_PRESSURE: "DME_RAIL_PRES",
    EDoIPRequest.DME_ACCELERATOR_PEDAL_POSITION: "DME_ACC_PEDAL_POS",
    EDoIPRequest.KOMBI_TOTAL_DISTANCE: "KOMBI_DIST",
    EDoIPRequest.KOMBI_SPEED: "KOMBI_SPEED",
    EDoIPRequest.KOMBI_OUTSIDE_TEMP_SENSOR: "KOMBI_OUT_TEMP",
    EDoIPRequest.KOMBI_ENGINE_SPEED_ON_DISP: "KOMBI_ENG_SPEED_DISP",
    EDoIPRequest.KOMBI_FUEL_LEVEL: "KOMBI_FUEL_LEVEL",
    EDoIPRequest.IHKA_EVAPORATOR_TEMPERATURE_SENSOR: "IHKA_EVAP_TEMP",
    EDoIPRequest.IHKA_TEMPERATURE_SELECTOR: "IHKA_TEMP_SEL",
}

def main():
    # Open the serial port
    serial_session = serial.Serial(port=serial_port, baudrate=baudrate, timeout=2)  # Set timeout for reads
    data_dict = {}
    if serial_session.is_open:
        print("Serial port opened successfully.")
        try:
            while True:
                line = serial_session.readline()  # Read a line from the serial port
                if line:  # Check if data was received
                    line_str = line.decode('utf-8').strip()
                    data_uart_prefix_str = "[UART_DATA]"
                    if line_str.startswith(data_uart_prefix_str):
                        data_str = line_str.lstrip(data_uart_prefix_str)
                        data_str = data_str.lstrip(" ")
                        data_list = data_str.split(',')
                        param_name_str = data_list[0]
                        if param_name_str not in data_dict:
                            data_dict[param_name_str] = {'time': [], 'value': []}
                        
                        timestamp = data_list[1]
                        data_dict[param_name_str]['time'].append(timestamp)
                        if param_name_str == param_string_dict[EDoIPRequest.DME_ENGINE_OIL_TEMPERATURE]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_ENGINE_ROTATIONAL_SPEED]:
                            data_dict[param_name_str]['value'].append(int(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_COOLANT_TEMPERATURE]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_BATTERY_VOLTAGE]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_AMBIENT_TEMPERATURE]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_AIR_MASS]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_RAIL_PRESSURE]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.DME_ACCELERATOR_PEDAL_POSITION]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))# / 2.55)
                        elif param_name_str == param_string_dict[EDoIPRequest.KOMBI_TOTAL_DISTANCE]:
                            data_dict[param_name_str]['value'].append(int(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.KOMBI_SPEED]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.KOMBI_OUTSIDE_TEMP_SENSOR]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.KOMBI_ENGINE_SPEED_ON_DISP]:
                            data_dict[param_name_str]['value'].append(int(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.KOMBI_FUEL_LEVEL]:
                            data_dict[param_name_str]['value'].append((int(data_list[2]), int(data_list[3]), int(data_list[4])))
                        elif param_name_str == param_string_dict[EDoIPRequest.IHKA_EVAPORATOR_TEMPERATURE_SENSOR]:
                            data_dict[param_name_str]['value'].append(float(data_list[2]))
                        elif param_name_str == param_string_dict[EDoIPRequest.IHKA_TEMPERATURE_SELECTOR]:
                            data_dict[param_name_str]['value'].append((int(data_list[2]), int(data_list[3])))
        except KeyboardInterrupt:
            print("Stopped reading. Saving files.")
            for key in data_dict:
                if key == param_string_dict[EDoIPRequest.KOMBI_FUEL_LEVEL]:
                    right_fuel_string = key + "_RIGHT"
                    sum_fuel_string = key + "_SUM"
                    left_fuel_string = key + "_LEFT"
                    
                    values = data_dict[key]['value']
                    values_right = [row[0] for row in values]
                    values_sum = [row[1] for row in values]
                    values_left = [row[2] for row in values]
                    df = pd.DataFrame({
                        'time': data_dict[key]['time'],
                        right_fuel_string: values_right  # Use the key name for the column name
                    })
                    df.to_csv(right_fuel_string + "_param_value.csv", index=False)  # Print without the index
                    df = pd.DataFrame({
                        'time': data_dict[key]['time'],
                        sum_fuel_string: values_sum  # Use the key name for the column name
                    })
                    df.to_csv(sum_fuel_string + "_param_value.csv", index=False)  # Print without the index
                    df = pd.DataFrame({
                        'time': data_dict[key]['time'],
                        left_fuel_string: values_left  # Use the key name for the column name
                    })
                    df.to_csv(left_fuel_string + "_param_value.csv", index=False)  # Print without the index
                elif key == param_string_dict[EDoIPRequest.IHKA_TEMPERATURE_SELECTOR]:
                    left_temp_string = key + "_LEFT"
                    right_temp_string = key + "_RIGHT"
                    values = data_dict[key]['value']
                    values_left  = [row[0] for row in values]
                    values_right = [row[1] for row in values]
                    df = pd.DataFrame({
                        'time': data_dict[key]['time'],
                        left_temp_string: values_left  # Use the key name for the column name
                    })
                    df.to_csv(left_temp_string + "_param_value.csv", index=False)  # Print without the index
                    df = pd.DataFrame({
                        'time': data_dict[key]['time'],
                        right_temp_string: values_right  # Use the key name for the column name
                    })
                    df.to_csv(right_temp_string + "_param_value.csv", index=False)  # Print without the index
                else:
                    df = pd.DataFrame({
                        'time': data_dict[key]['time'],
                        key: data_dict[key]['value']  # Use the key name for the column name
                    })
                    df.to_csv(key + "_param_value.csv", index=False)  # Print without the index
        finally:
            serial_session.close()  # Close the serial port when done
    else:
        print("Failed to open serial port.")


if __name__ == "__main__":
    main()