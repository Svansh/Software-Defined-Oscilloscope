import serial


def SDO_Connect(port, baud_rate, byte_size, parity, stopbit) -> serial.Serial:
    conn = serial.Serial(port, baud_rate, byte_size, parity, stopbit)
    if conn.is_open == True:
        return conn
    else:
        return 0

def SDO_WriteCommand(connection: serial.Serial, command: int):
    byte = command.to_bytes(1, "little")
    if connection.writable():
       status = connection.write(byte)
    else:
        status = 0
    return status

def SDO_Read(connection: serial.Serial):
    if connection.readable():
        buffer = connection.read()
        return buffer.hex()

def SDO_Start_Conversion(connection: serial.Serial):
    command = 0x31
    SDO_WriteCommand(connection, command)
    buffer = SDO_Read(connection)
    return buffer
    
def SDO_Stop_Conversion(connection: serial.Serial):
    command = 0x32
    SDO_WriteCommand(connection, command)
    buffer = SDO_Read(connection)
    return buffer


conn = SDO_Connect("COM3", 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
# print(SDO_WriteCommand(conn, 0x31))
# print(SDO_Read(conn))
print(SDO_Start_Conversion(conn))
print(SDO_Stop_Conversion(conn))

# print(SDO_Read(conn))