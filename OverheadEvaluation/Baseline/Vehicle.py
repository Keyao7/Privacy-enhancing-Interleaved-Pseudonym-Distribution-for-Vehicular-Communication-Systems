import os
import glob
import json
import base64
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import ec
import random
import time
import socket

key_dir = "/Users/keyaohuang/Desktop/Implementation/Baseline/VehicleKeysSet/"

time_s = time.time()
def load_private_key(path):
    with open(path, "rb") as f:
        key_data = f.read()
    private_key = serialization.load_pem_private_key(key_data, password=None, backend=default_backend())
    return private_key

# Load Vehicle's long-term private key (if needed)
V_private_key_path = os.path.join(key_dir, "V_private_key.pem")
if os.path.exists(V_private_key_path):
    V_private_key = load_private_key(V_private_key_path)
else:
    V_private_key = None

# Scan and load pseudonym key pairs
pseudonym_priv_keys = []
pseudonym_pub_keys = []

pub_key_files = sorted(glob.glob(os.path.join(key_dir, "V_Pseudonym_public_key_*.pem")))

for pub_file in pub_key_files:
    index = pub_file.split("_")[-1].split(".")[0]  # Get the last x
    priv_file = os.path.join(key_dir, f"V_Pseudonym_private_key_{index}.pem")
    if os.path.exists(priv_file):
        # Load public key
        with open(pub_file, "rb") as f:
            pub_data = f.read()
        pub_key = serialization.load_pem_public_key(pub_data, backend=default_backend())
        
        # Load private key
        priv_key = load_private_key(priv_file)
        
        pseudonym_priv_keys.append(priv_key)
        # Convert public key PEM to string
        pseudonym_pub_keys.append(pub_data.decode('utf-8'))

# Serialize the public keys list to JSON
public_keys_json = json.dumps(pseudonym_pub_keys, ensure_ascii=False)

# Sign the serialized public keys list with pseudonym private keys
signatures_list = []
for priv_key in pseudonym_priv_keys:
    signature = priv_key.sign(
        public_keys_json.encode('utf-8'),
        ec.ECDSA(hashes.SHA256())
    )
    signatures_list.append(base64.b64encode(signature).decode('utf-8'))

Id_req = 0
N = random.randint(0,10000)
t_now = time.time()
n_tkt = "xxxxxxxxx"
n_tkt_signature = "xxxxxxxx"
pseudonym_request_pack = {
    "Id_req": Id_req,
    "public_keys": pseudonym_pub_keys,
    "signatures": signatures_list,
    "n_tkt": n_tkt,
    "n_tkt_signature": n_tkt_signature,
    "PCAs": [1, 2],
    "N": N,
    "t_now": t_now
}

# Convert the message to JSON string
transmit_str = json.dumps(pseudonym_request_pack, ensure_ascii=False)

# Send via socket to PCA and receive response
HOST = '192.168.209.157'
PORT = 5000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(transmit_str.encode('utf-8'))
    print("Message sent to PCA, waiting for PCA's response...")

    data = s.recv(40960)
    if data:
        response = json.loads(data.decode('utf-8'))
        print("Response received from PCA:", response)

        if response.get("status") == "OK":
            # Retrieve PCA's public key and signature set from the response
            pca_public_key_pem = response.get("pca_public_key")
            pca_signatures = response.get("pca_signatures", [])

            # Use PCA's public key to verify PCA's signatures on each public key
            pca_public_key = serialization.load_pem_public_key(
                pca_public_key_pem.encode('utf-8'), 
                backend=default_backend()
            )

            all_pca_signatures_valid = True
            for i, pca_sig_b64 in enumerate(pca_signatures):
                pca_sig = base64.b64decode(pca_sig_b64)
                pub_key_str = pseudonym_pub_keys[i].encode('utf-8')
                try:
                    pca_public_key.verify(
                        pca_sig,
                        pub_key_str,
                        ec.ECDSA(hashes.SHA256())
                    )
                    print(f"Verification of PCA's signature for the {i+1}th public key succeeded.")
                except Exception:
                    print(f"Verification of PCA's signature for the {i+1}th public key failed.")
                    all_pca_signatures_valid = False
            
            if all_pca_signatures_valid:
                print("All PCA signatures verified successfully, Vehicle can trust PCA's proof.")
            else:
                print("Some PCA signatures failed verification.")

        else:
            print("PCA returned FAIL status, reason:", response.get("reason", "Unknown"))
    else:
        print("No response received from PCA.")

time_e = time.time()

print(time_e - time_s)