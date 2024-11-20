def crc8_sae_j1850(data: bytes) -> int:
    polynomial = 0x1D
    crc = 0xFF

    for i, byte in enumerate(data):
        if i == 0:
            byte ^= 0b00100000
            
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ polynomial
            else:
                crc <<= 1

    return crc ^ 0xFF

data = bytes([0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
crc_value = crc8_sae_j1850(data) & 0xFF
print(f"CRC-8 SAE J1850: 0x{crc_value:02X}")