#!/usr/bin/env python3
import sys

def xor_encrypt(data: bytes, key: bytes) -> bytes:
    key_len = len(key)
    return bytes(b ^ key[i % key_len] for i, b in enumerate(data))

def format_c_array(name: str, data: bytes, null_term: bool = True) -> str:
    hex_vals = ', '.join(f'0x{b:02x}' for b in data)
    if null_term:
        hex_vals += ', 0x00'
    return f'unsigned char {name}[] = {{ {hex_vals} }};'

def format_decrypt_stub(key: bytes) -> str:
    key_arr = format_c_array('xor_key', key, null_term=False)
    return (
        f'// XOR decrypt stub\n'
        f'{key_arr}\n'
        f'size_t key_len = {len(key)};\n'
        f'for (size_t i = 0; i < buf_len; i++) {{\n'
        f'    buf[i] ^= xor_key[i % key_len];\n'
        f'}}'
    )

def main():
    if len(sys.argv) < 2:
        print(f'Usage: {sys.argv[0]} <raw shellcode> [key]')
        sys.exit(1)

    key_str = sys.argv[2] if len(sys.argv) > 2 else 'mysecretkey'
    key = key_str.encode()

    try:
        with open(sys.argv[1], 'rb') as f:
            plaintext = f.read()
    except OSError as e:
        print(f'[-] {e}', file=sys.stderr)
        sys.exit(1)

    ciphertext = xor_encrypt(plaintext, key)

    print(format_c_array('buf', ciphertext))
    print(f'\n// Key: {key_str!r} | Key length: {len(key)} bytes | Payload: {len(plaintext)} bytes')
    print(f'\n{format_decrypt_stub(key)}')

if __name__ == '__main__':
    main()