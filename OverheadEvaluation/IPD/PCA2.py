import socket
import json
import base64
import os
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import ec

HOST = '127.0.0.1'  # Local listening address
PORT = 5001         # Listening port

# Load PCA's private and public keys
pca_key_dir = "/Users/keyaohuang/Desktop/Implementation/BaselineTwo/PCA2KeysSet/"
with open(os.path.join(pca_key_dir, "PCA2_private_key.pem"), "rb") as f:
    pca_private_key = serialization.load_pem_private_key(f.read(), password=None, backend=default_backend())

with open(os.path.join(pca_key_dir, "PCA2_public_key.pem"), "rb") as f:
    pca_public_key_pem = f.read().decode('utf-8')  # Keep PEM format string

def load_public_key(pem_data):
    return serialization.load_pem_public_key(pem_data.encode('utf-8'), backend=default_backend())

def verify_signature(public_key_pem, message, signature_b64):
    signature = base64.b64decode(signature_b64)
    pub_key = load_public_key(public_key_pem)
    try:
        pub_key.verify(
            signature,
            message,
            ec.ECDSA(hashes.SHA256())
        )
        return True
    except Exception:
        return False

def sign_with_pca_private_key(message_bytes):
    """ Use PCA's private key to sign the message and return the Base64 encoded signature string """
    signature = pca_private_key.sign(
        message_bytes,
        ec.ECDSA(hashes.SHA256())
    )
    return base64.b64encode(signature).decode('utf-8')

def process_message(msg):
    """
    Receive the message from Vehicle and verify the signatures.
    If verification succeeds, PCA signs each public key and returns.
    """
    public_keys = msg.get("public_keys", [])
    signatures = msg.get("signatures", [])

    # Original message used for verification
    public_keys_json = json.dumps(public_keys, ensure_ascii=False).encode('utf-8')

    if len(public_keys) != len(signatures):
        print("The number of public keys does not match the number of signatures, cannot verify.")
        response = {"status": "FAIL", "reason": "Key and signature count mismatch"}
        return response

    all_valid = True
    for i, sig in enumerate(signatures):
        pub_key_pem = public_keys[i]
        is_valid = verify_signature(pub_key_pem, public_keys_json, sig)
        if not is_valid:
            print(f"Signature {i+1} verification failed.")
            all_valid = False
        else:
            print(f"Signature {i+1} verified successfully.")

    if not all_valid:
        print("Some signatures failed verification.")
        response = {"status": "FAIL", "reason": "Some signatures invalid"}
        return response

    print("All signatures have been successfully verified.")

    # If all signatures pass, PCA signs each public key
    # Here PCA signs each public key (its PEM string) individually.
    pca_signatures = []
    for pub_key_pem in public_keys:
        # Encode each public key and sign
        pca_signature = sign_with_pca_private_key(pub_key_pem.encode('utf-8'))
        pca_signatures.append(pca_signature)

    # Package PCA's public key and signatures
    response = {
        "status": "OK",
        "pca_public_key": pca_public_key_pem,
        "pca_signatures": pca_signatures
    }

    return response

def main():
    # Create server socket and listen
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen(1)
        print(f"PCA listening on {HOST}:{PORT}...")

        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            data = conn.recv(40960)
            if not data:
                return
            # data should be a JSON string
            msg = json.loads(data.decode('utf-8'))

            # Call the function to process the message
            response = process_message(msg)

            # Send the result back to Vehicle
            conn.sendall(json.dumps(response, ensure_ascii=False).encode('utf-8'))

if __name__ == "__main__":
    main()
